#pragma once
#include <iostream>	
#include <cstdlib>
#include <SDL\SDL.h>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"

struct ColliderEditor
{

	public:

		ColliderEditor() 
		{ 
			shaderManager = nullptr;
			textureManager = nullptr;
			meshManager	= nullptr;
		
		}


		ColliderEditor(ShaderManager* shaderManager, TextureManager* textureManager, MeshManager* meshManager)
		{
			this->shaderManager = shaderManager;
			this->textureManager = textureManager;
			this->meshManager = meshManager;


			loadCollidersFromBinary("..\\res\\colliders.bin");
		}

		void OpenEditor()
		{
			std::cout << "Collider Editor\nNew Collider - N\nDelete Current Collider - Backspace\nPosition - ,\n";
			std::cout << "Rotation - .\nScale - / \nAxis - X Y Z\n";
			std::cout << "Type Change Stat Value - V\nChange Stat + (Increase)-(Decrease)\nMultiply Change State By 10 - 1 or ! (NOT NUMPAD)\n";
			std::cout << "Change Collider Index Up - X\nChange Collider Index Down - C\n";	
			std::cout << "Hide All Meshes - H\n";
			std::cout << "Force Save - P\n";
			std::cout << "Last Action : \n";

			UpdateEditorStateText();

			hideMeshes = false;
		}


		void UpdateEditor()
		{
			const Uint8* state = SDL_GetKeyboardState(NULL);

			//transform type
			if (state[SDL_SCANCODE_COMMA])
			{
				if (!commaDown)
				{
					currentEditorState = EditorState::POSITION;
					commaDown = true;

					UpdateEditorStateText();
					
				}
				
			}
			else
			{
				commaDown = false;
			}


			if (state[SDL_SCANCODE_PERIOD])
			{
				if(!periodDown)
				{
					currentEditorState = EditorState::ROTATION;
					periodDown = true;
					UpdateEditorStateText();
					
				}
				
			}
			else
			{
				periodDown = false;
			}


			if (state[SDL_SCANCODE_SLASH])
			{
				if(!slashDown)
				{
					currentEditorState = EditorState::SCALE;
					slashDown = true;
					UpdateEditorStateText();
				}
				
			}
			else
			{
				slashDown = false;
			}


			//axis
			if (state[SDL_SCANCODE_X])
			{
				if (!xDown)
				{
					currentAxisState = AxisState::X;
					xDown = true;
					UpdateEditorStateText();
					
				}
				
			}
			else
			{
				xDown = false;
			}


			if (state[SDL_SCANCODE_Y])
			{
				if(!yDown)
				{
					currentAxisState = AxisState::Y;
					yDown = true;
					UpdateEditorStateText();			
				}	
				
			}
			else
			{
				yDown = false;
			}


			if (state[SDL_SCANCODE_Z])
			{
				if (!zDown)
				{
					currentAxisState = AxisState::Z;
					zDown = true;
					UpdateEditorStateText();
					
				}
				
			}
			else
			{
				zDown = false;
			}



			if(state[SDL_SCANCODE_V])
			{
				if (!vDown)
				{
					vDown = true;
					refreshEditorOutput();
					while (true)
					{
						std::string statType;
						float currentValue = 0.0f;
						switch (currentEditorState)
						{
							case EditorState::POSITION:
								statType = "Position";
								currentValue = changeAmountPos;
								break;
							case EditorState::ROTATION:
								statType = "Rotation";
								currentValue = changeAmountRot;
								break;
							case EditorState::SCALE:
								statType = "Scale";
								currentValue = changeAmountScale;
								break;

						}
						std::cout << "Current Change Stat Value For " + statType + " : " + std::to_string(currentValue);
						std::cout << "\nEnter New Change Stat Value For " + statType + " : ";

						float changeAmount = 0;

						if (std::cin >> changeAmount)
						{
							switch (currentEditorState)
							{
							case ColliderEditor::POSITION:
								changeAmountPos = changeAmount;
								break;
							case ColliderEditor::ROTATION:
								changeAmountRot = changeAmount;
								break;
							case ColliderEditor::SCALE:
								changeAmountScale = changeAmount;
								break;
							default:
								break;
							}
							refreshEditorOutput();
							break;

						}
						else
						{
							std::cout << "Invalid Input, Try Again\n";
							std::cin.clear();
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							refreshEditorOutput();
						}
					}
				}
								
			}
			else
			{
				vDown = false;
			}


			//force save
			if(state[SDL_SCANCODE_P])
			{
				if(!pDown)
				{
					pDown = true;
					refreshEditorOutput();
					saveCollidersToBinary("..\\res\\colliders.bin");
					std::cout << "Colliders Saved\n";
				}
				
			}
			else
			{
				pDown = false;
			}





			//new collider
			if(state[SDL_SCANCODE_N])
			{
				if(!nDown)
				{
					nDown = true;
					m_colliders.push_back(std::make_unique<GameObject>());
					m_colliders.back()->GetTransform()->SetPosition(glm::vec3(0.0, 0.0, 0.0));
					m_colliders.back()->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
					m_colliders.back()->GetTransform()->SetScale(glm::vec3(1.0, 1.0, 1.0));
					m_colliders.back()->SetShader(*shaderManager->GetShader(RIM_LIGHT));
					m_colliders.back()->SetTexture(*textureManager->GetTexture(NONE));
					m_colliders.back()->SetMesh(*meshManager->GetMesh(CUBE));
					currentColliderIndex = m_colliders.size() - 1;
					refreshEditorOutput();
					std::cout << "New Collider Created\n";
				}
				
			}
			else
			{
				nDown = false;
			}

			//delete collider
			if(state[SDL_SCANCODE_BACKSPACE])
			{
				if(!backspaceDown)
				{
					if(m_colliders.size() > 0)
					{
						backspaceDown = true;
						m_colliders.erase(m_colliders.begin() + currentColliderIndex);

						if(currentColliderIndex > 0)
							currentColliderIndex--;

						refreshEditorOutput();
						std::cout << "Collider Destoryed\n";
					}
					
				}
				
			}
			else
			{
				backspaceDown = false;
			}



			//hide meshes
			if(state[SDL_SCANCODE_H])
			{
				if(!hDown)
				{
					hDown = true;
					hideMeshes = !hideMeshes;
					refreshEditorOutput();
					std::cout << "Hide Meshes : " + std::string(hideMeshes ? "True" : "False") + "\n";
				}
				
			}
			else
			{
				hDown = false;
			}


			//change index
			if(state[SDL_SCANCODE_X])
			{
				if(!upArrowDown)
				{
					upArrowDown = true;
					if (currentColliderIndex < m_colliders.size() - 1)
					{
						currentColliderIndex++;
					}
					else
					{
						currentColliderIndex = 0;
					}
				}
				
			}
			else
			{
				upArrowDown = false;
			}


			if (state[SDL_SCANCODE_C])
			{
				if (!downArrowDown)
				{
					downArrowDown = true;
					if (currentColliderIndex > 0)
					{
						currentColliderIndex--;
					}
					else
					{
						currentColliderIndex = m_colliders.size() - 1;
					}
				}

			}
			else
			{
				downArrowDown = false;
			}




			float factor = 1.0f;

			if (state[SDL_SCANCODE_1])
			{
				factor = 10.0f;
			}


			//add stuff
			if(state[SDL_SCANCODE_EQUALS])
			{
				if (currentColliderIndex >= m_colliders.size() || m_colliders[currentColliderIndex] == nullptr)
				{
					return;
				}
				if (!plusDown)
				{
					plusDown = true;
					switch (currentEditorState)
					{
					case ColliderEditor::POSITION:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(changeAmountPos * factor, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(0.0f, changeAmountPos * factor, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(0.0f, 0.0f, changeAmountPos * factor));
							break;
						}

						break;



					case ColliderEditor::ROTATION:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(glm::radians(changeAmountRot * factor), 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(0.0f, glm::radians(changeAmountRot * factor), 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(0.0f, 0.0f, glm::radians(changeAmountRot * factor)));
							break;
						}

						break;



					case ColliderEditor::SCALE:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(changeAmountScale * factor, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, changeAmountScale * factor, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, 0.0f, changeAmountScale * factor));
							break;
						}

						break;

					default:
						break;
					}
				}
			}			
			else
			{
				plusDown = false;
			}

			



			//minus stuff
			if(state[SDL_SCANCODE_MINUS])
			{
				if (currentColliderIndex >= m_colliders.size() || m_colliders[currentColliderIndex] == nullptr)
				{
					return;
				}
				if (!minusDown)
				{
					minusDown = true;
					switch (currentEditorState)
					{
					case ColliderEditor::POSITION:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(changeAmountPos * factor, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(0.0f, changeAmountPos * factor, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(0.0f, 0.0f, changeAmountPos * factor));
							break;
						}

						break;



					case ColliderEditor::ROTATION:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(glm::radians(changeAmountRot * factor), 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(0.0f, glm::radians(changeAmountRot * factor), 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(0.0f, 0.0f, glm::radians(changeAmountRot * factor)));
							break;
						}

						break;



					case ColliderEditor::SCALE:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(changeAmountScale * factor, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, changeAmountScale * factor, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, 0.0f, changeAmountScale * factor));
							break;
						}

						break;

					default:
						break;
					}
				}		
			}
			else
			{
				minusDown = false;
			}


		}


		void refreshEditorOutput() {
			std::cout << "\033[16;1H";

			std::cout << "\033[J";

			std::cout << std::flush;
		}



		void CloseEditor()
		{
			std::system("cls");
			saveCollidersToBinary("..\\res\\colliders.bin");
			hideMeshes = false;
		}	


		void DrawEditor(Camera& mainCamera)
		{
			for (int i = 0; i < m_colliders.size(); i++)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glLineWidth(2.0f);
				m_colliders[i]->GetShader()->Bind();

				if(i == currentColliderIndex)
					m_colliders[i]->GetShader()->Update(*m_colliders[i]->GetTransform(), mainCamera, true);
				else
					m_colliders[i]->GetShader()->Update(*m_colliders[i]->GetTransform(), mainCamera, false);
				
				m_colliders[i]->GetTexture()->Bind(0);
				m_colliders[i]->GetMesh()->draw();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}



		void saveCollidersToBinary(const std::string& filename)
		{
			std::ofstream file(filename, std::ios::binary);

			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> rotations;
			std::vector<glm::vec3> scales;

			for(int i = 0; i < m_colliders.size(); i++)
			{
				positions.push_back(*m_colliders[i]->GetTransform()->GetPosition());
				rotations.push_back(*m_colliders[i]->GetTransform()->GetRotation());
				scales.push_back(*m_colliders[i]->GetTransform()->GetScale());
			}

			uint32_t posCount = positions.size();
			uint32_t rotCount = rotations.size();
			uint32_t scaleCount = scales.size();

			file.write(reinterpret_cast<char*>(&changeAmountPos), sizeof(float));
			file.write(reinterpret_cast<char*>(&changeAmountRot), sizeof(float));
			file.write(reinterpret_cast<char*>(&changeAmountScale), sizeof(float));
			file.write(reinterpret_cast<char*>(&posCount), sizeof(uint32_t));
			file.write(reinterpret_cast<char*>(&rotCount), sizeof(uint32_t));
			file.write(reinterpret_cast<char*>(&scaleCount), sizeof(uint32_t));
			

			file.write(reinterpret_cast<char*>(positions.data()), posCount * sizeof(glm::vec3));
			file.write(reinterpret_cast<char*>(rotations.data()), rotCount * sizeof(glm::vec3));
			file.write(reinterpret_cast<char*>(scales.data()), scaleCount * sizeof(glm::vec3));
		}

		void loadCollidersFromBinary(const std::string& filename)
		{
			std::ifstream file(filename, std::ios::binary);

			if(!file.good())
			{
				return;
			}

			uint32_t posCount, rotCount, scaleCount;
			file.read(reinterpret_cast<char*>(&changeAmountPos), sizeof(float));
			file.read(reinterpret_cast<char*>(&changeAmountRot), sizeof(float));
			file.read(reinterpret_cast<char*>(&changeAmountScale), sizeof(float));
			file.read(reinterpret_cast<char*>(&posCount), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&rotCount), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&scaleCount), sizeof(uint32_t));


			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> rotations;
			std::vector<glm::vec3> scales;

			positions.resize(posCount);
			rotations.resize(rotCount);
			scales.resize(scaleCount);


			file.read(reinterpret_cast<char*>(positions.data()), posCount * sizeof(glm::vec3));
			file.read(reinterpret_cast<char*>(rotations.data()), rotCount * sizeof(glm::vec3));
			file.read(reinterpret_cast<char*>(scales.data()), scaleCount * sizeof(glm::vec3));


			m_colliders.clear();

			m_colliders.resize(posCount);

			for(int i = 0; i < m_colliders.size(); i++)
			{
				m_colliders[i] = std::make_unique<GameObject>();
				m_colliders[i]->GetTransform()->SetPosition(positions[i]);
				m_colliders[i]->GetTransform()->SetRotation(rotations[i]);
				m_colliders[i]->GetTransform()->SetScale(scales[i]);
				m_colliders[i]->SetShader(*shaderManager->GetShader(RIM_LIGHT));
				m_colliders[i]->SetTexture(*textureManager->GetTexture(NONE));
				m_colliders[i]->SetMesh(*meshManager->GetMesh(CUBE));
			}

		}




		void UpdateEditorStateText()
		{
			refreshEditorOutput();

			switch (currentEditorState)
			{
			case ColliderEditor::POSITION:
				std::cout << "Editing Position\n";
				break;
			case ColliderEditor::ROTATION:
				std::cout << "Editing Rotation\n";
				break;
			case ColliderEditor::SCALE:
				std::cout << "Editing Scale\n";
				break;
			}


			switch (currentAxisState)
			{
			case ColliderEditor::X:
				std::cout << "Editing X axis\n";
				break;
			case ColliderEditor::Y:
				std::cout << "Editing Y axis\n";
				break;
			case ColliderEditor::Z:
				std::cout << "Editing Z axis\n";
				break;
			}
		}


		bool GetHideMeshes() { return hideMeshes; }


	private:

		enum EditorState { POSITION, ROTATION, SCALE };
		enum AxisState { X, Y, Z };

		EditorState currentEditorState = EditorState::POSITION;
		AxisState currentAxisState = AxisState::X;


		std::vector<std::unique_ptr<GameObject>> m_colliders;

		int currentColliderIndex = 0;

		bool commaDown = false;
		bool periodDown = false;
		bool slashDown = false;
		bool xDown = false;
		bool yDown = false;
		bool zDown = false;
		bool plusDown = false;
		bool minusDown = false;
		bool nDown = false;
		bool backspaceDown = false;
		bool vDown = false;
		bool upArrowDown = false;
		bool downArrowDown = false;
		bool hDown = false;
		bool oneDown = false;
		bool pDown = false;



		bool hideMeshes = false;



		float changeAmountPos = 1.0f;	
		float changeAmountRot = 5.0f;	
		float changeAmountScale = 1.0f;	

		ShaderManager* shaderManager;
		TextureManager* textureManager;
		MeshManager* meshManager;
};