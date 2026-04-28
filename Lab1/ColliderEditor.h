#pragma once
#include <iostream>	
#include <cstdlib>
#include <SDL\SDL.h>
#include <memory>
#include <vector>
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


			for (int i = 0; i < m_colliders.size(); i++)
			{
				m_colliders[i] = std::make_unique<GameObject>();
				m_colliders[i]->GetTransform()->SetPosition(glm::vec3(0.0, 0.0, 0.0));
				m_colliders[i]->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
				m_colliders[i]->GetTransform()->SetScale(glm::vec3(1.0, 1.0, 1.0));
				m_colliders[i]->SetShader(*shaderManager->GetShader(RIM_LIGHT));
				m_colliders[i]->SetTexture(*textureManager->GetTexture(NONE));
				m_colliders[i]->SetMesh(*meshManager->GetMesh(CUBE));
			}
		}

		void OpenEditor()
		{
			std::cout << "Collider Editor\nNew Collider - N\nDelete Current Collider - B\nPosition - ,\nRotation - .\nScale - /\nAxis - X Y Z\nChange Stat +(Increase) -(Decrease)\nType Change Stat Value - V\nChange Collider Index Up - X\nChange Collider Index Down - C\n";		
			std::cout << "Last Action - \n";
		}


		void UpdateEditor()
		{
			const Uint8* state = SDL_GetKeyboardState(NULL);

			//transform type
			if (state[SDL_SCANCODE_COMMA] && currentEditorState != EditorState::POSITION)
			{
				refreshEditorOutput();
				std::cout << "Position\n";
				currentEditorState = EditorState::POSITION;
			}
			if (state[SDL_SCANCODE_PERIOD] && currentEditorState != EditorState::ROTATION)
			{
				refreshEditorOutput();
				std::cout << "Rotation\n";
				currentEditorState = EditorState::ROTATION;
			}
			if (state[SDL_SCANCODE_SLASH] && currentEditorState != EditorState::SCALE)
			{
				refreshEditorOutput();
				std::cout << "Scale\n";
				currentEditorState = EditorState::SCALE;
			}


			//axis
			if (state[SDL_SCANCODE_X] && currentAxisState != AxisState::X)
			{
				refreshEditorOutput();
				std::cout << "Editing X axis\n";
				currentAxisState = AxisState::X;
			}
			if (state[SDL_SCANCODE_Y] && currentAxisState != AxisState::Y)
			{
				refreshEditorOutput();
				std::cout << "Editing Y axis\n";
				currentAxisState = AxisState::Y;
			}
			if (state[SDL_SCANCODE_Z] && currentAxisState != AxisState::Z)
			{
				refreshEditorOutput();
				std::cout << "Editing Z axis\n";
				currentAxisState = AxisState::Z;
			}



			if(state[SDL_SCANCODE_V])
			{
				if (!vDown)
				{
					vDown = true;
					while (true)
					{
						std::cout << "Enter Value: ";

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
							std::cout << "Invalid input, try again\n";
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
			if(state[SDL_SCANCODE_B])
			{
				if(!bDown)
				{
					if(m_colliders.size() > 0)
					{
						bDown = true;
						m_colliders.erase(m_colliders.begin() + currentColliderIndex);

						if(currentColliderIndex > 0)
							currentColliderIndex--;

						refreshEditorOutput();
						std::cout << "Collider Created\n";
					}
					
				}
				
			}
			else
			{
				bDown = false;
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
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(changeAmountPos, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(0.0f, changeAmountPos, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(0.0f, 0.0f, changeAmountPos));
							break;
						}

						break;



					case ColliderEditor::ROTATION:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(glm::radians(changeAmountRot), 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(0.0f, glm::radians(changeAmountRot), 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(0.0f, 0.0f, glm::radians(changeAmountRot)));
							break;
						}

						break;



					case ColliderEditor::SCALE:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(changeAmountScale, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, changeAmountScale, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, 0.0f, changeAmountScale));
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
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(changeAmountPos, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(0.0f, changeAmountPos, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(0.0f, 0.0f, changeAmountPos));
							break;
						}

						break;



					case ColliderEditor::ROTATION:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(glm::radians(changeAmountRot), 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(0.0f, glm::radians(changeAmountRot), 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(0.0f, 0.0f, glm::radians(changeAmountRot)));
							break;
						}

						break;



					case ColliderEditor::SCALE:

						switch (currentAxisState)
						{
						case ColliderEditor::X:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(changeAmountScale, 0.0f, 0.0f));
							break;
						case ColliderEditor::Y:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, changeAmountScale, 0.0f));
							break;
						case ColliderEditor::Z:
							m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, 0.0f, changeAmountScale));
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
			std::cout << "\033[13;1H";

			std::cout << "\033[J";

			std::cout << std::flush;
		}



		void CloseEditor()
		{
			std::system("cls");
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


	private:

		enum EditorState { POSITION, ROTATION, SCALE };
		enum AxisState { X, Y, Z };

		EditorState currentEditorState = EditorState::POSITION;
		AxisState currentAxisState = AxisState::X;

		bool changingChangeState = false;	

		std::vector<std::unique_ptr<GameObject>> m_colliders;

		int currentColliderIndex = 0;
		int numberOfColliders = 0;


		bool plusDown = false;
		bool minusDown = false;
		bool nDown = false;
		bool bDown = false;
		bool vDown = false;
		bool upArrowDown = false;
		bool downArrowDown = false;

		float changeAmountPos = 1.0f;	
		float changeAmountRot = 5.0f;	
		float changeAmountScale = 1.0f;	

		ShaderManager* shaderManager;
		TextureManager* textureManager;
		MeshManager* meshManager;
};