#pragma once
#include <iostream>	
#include <cstdlib>
#include <SDL\SDL.h>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Collider.h"

struct ColliderEditor
{

public:

	struct BulletHitData
	{
		Collider* colliderHit;
		float distance;
	};


	ColliderEditor()
	{
		shaderManager = nullptr;
		textureManager = nullptr;
		meshManager = nullptr;
		enemyTankBody = nullptr;
		playerTankBody = nullptr;
	}


	ColliderEditor(ShaderManager* shaderManager, TextureManager* textureManager, MeshManager* meshManager, Tank* playerTank, Tank* enemyTank)
	{
		this->shaderManager = shaderManager;
		this->textureManager = textureManager;
		this->meshManager = meshManager;
		this->playerTankRef = playerTank;
		this->enemyTankRef = enemyTank;
		this->playerTankBody = playerTankRef->GetBody();
		this->enemyTankBody = enemyTankRef->GetBody();

		CreateTankBodyCollider();
		CreateEnemyTankBodyCollider();

		//we save colliders to a binary file for quick loading
		//if this file is deleted the player will be shot thorugh all walls
		loadCollidersFromBinary("..\\res\\Binary\\DONTDELETE\\colliders.bin");
	}

	void OpenEditor()
	{
		std::system("cls");

		std::cout << "Move Forward - W\nMove Backward - S\n";
		std::cout << "Move Left - A\nMove Right - D\n";
		std::cout << "Move Up - LSHIFT\nMove Down - LCTRL\n";
		std::cout << "Look Around - Mouse\n\n\n";

		std::cout << "Collider Editor\nNew Collider - N\nDelete Current Collider - Backspace\nPosition - ,\n";
		std::cout << "Rotation - .\nScale - / \nAxis - X Y Z\n";
		std::cout << "Type Change Stat Value - C\nChange Stat + (Increase)-(Decrease)\nMultiply Change State By 10,100 - 1,2 (NOT NUMPAD)\n";
		std::cout << "Change Collider Index Up - E\nChange Collider Index Down - Q\n";
		std::cout << "Hide All Meshes - H\n";
		std::cout << "Force Save - P\n\n\n";

		std::cout << "V-Sync Toggle - V\n";
		std::cout << "Unlock/Lock Mouse - ESC\n";
		std::cout << "Fullscreen - F11\n";
		std::cout << "Last Action : \n";
		

		UpdateEditorStateText();

		hideMeshes = false;
	}


	void HandleInputForCreatingAndDeletingColliders(const Uint8* state)
	{
		//create new collider
		if (state[SDL_SCANCODE_N])
		{
			if (!nDown)
			{
				nDown = true;
				m_colliders.push_back(std::make_unique<GameObject>());
				m_colliders.back()->GetTransform()->SetPosition(glm::vec3(0.0, 0.0, 0.0));
				m_colliders.back()->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
				m_colliders.back()->GetTransform()->SetScale(glm::vec3(1.0, 1.0, 1.0));
				m_colliders.back()->GetCollider()->SetScale(glm::vec3(1.0, 1.0, 1.0));
				m_colliders.back()->GetCollider()->UpdateCollider(*m_colliders.back()->GetTransform()->GetPosition(), *m_colliders.back()->GetTransform()->GetRotation());
				m_colliders.back()->SetShader(*shaderManager->GetShader(COLLIDEROUTLINE));
				m_colliders.back()->SetTexture(*textureManager->GetTexture(NONE));
				m_colliders.back()->SetMesh(*meshManager->GetMesh(CUBE_M));
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
		if (state[SDL_SCANCODE_BACKSPACE])
		{
			if (!backspaceDown)
			{
				if (m_colliders.size() > 0)
				{
					backspaceDown = true;
					m_colliders.erase(m_colliders.begin() + currentColliderIndex);

					if (currentColliderIndex > 0)
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
	}


	//based on input either ,./ we switch what transform element we are editing
	void HandleInputForEditorType(const Uint8* state)
	{
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
			if (!periodDown)
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
			if (!slashDown)
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
	}



	//based on xyz input we are now editing that axis
	void HandleInputForAxisType(const Uint8* state)
	{
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
			if (!yDown)
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
	}





	void HandleInputForTypingStatChange(const Uint8* state)
	{

		//allow user to enter new change value
		if (state[SDL_SCANCODE_C])
		{
			if (!cDown)
			{
				cDown = true;
				refreshEditorOutput();

				while (true)
				{

					//get current value
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

					//display
					std::cout << "Current Change Stat Value For " + statType + " : " + std::to_string(currentValue);
					std::cout << "\nEnter New Change Stat Value For " + statType + " : ";

					float changeAmount = 0;


					//get new amount and update the right change amount based on waht we are editing
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
					//i love input validation
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
			cDown = false;
		}
	}


	//stat change
	void HandleInputForStatChanging(const Uint8* state)
	{
		float factor = 1.0f;


		//multipliers
		if (state[SDL_SCANCODE_1])
		{
			factor = 10.0f;
		}

		if (state[SDL_SCANCODE_2])
		{
			factor = 100.0f;
		}


		//add stuff
		//basically a huge switch statement for each axis and transofrm element and it adds the value to the axis and transform element we are editing
		if (state[SDL_SCANCODE_EQUALS])
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
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Y:
						m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(0.0f, changeAmountPos * factor, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Z:
						m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() + glm::vec3(0.0f, 0.0f, changeAmountPos * factor));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					}

					break;



				case ColliderEditor::ROTATION:

					switch (currentAxisState)
					{
					case ColliderEditor::X:
						m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(glm::radians(changeAmountRot * factor), 0.0f, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Y:
						m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(0.0f, glm::radians(changeAmountRot * factor), 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Z:
						m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() + glm::vec3(0.0f, 0.0f, glm::radians(changeAmountRot * factor)));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					}

					break;



				case ColliderEditor::SCALE:

					switch (currentAxisState)
					{
					case ColliderEditor::X:
						m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(changeAmountScale * factor, 0.0f, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(changeAmountScale * factor, 0.0f, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Y:
						m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, changeAmountScale * factor, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, changeAmountScale * factor, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Z:
						m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, 0.0f, changeAmountScale * factor));
						m_colliders[currentColliderIndex]->GetCollider()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() + glm::vec3(0.0f, 0.0f, changeAmountScale * factor));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
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





		//same as add here but minus
		if (state[SDL_SCANCODE_MINUS])
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
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Y:
						m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(0.0f, changeAmountPos * factor, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Z:
						m_colliders[currentColliderIndex]->GetTransform()->SetPosition(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition() - glm::vec3(0.0f, 0.0f, changeAmountPos * factor));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					}

					break;



				case ColliderEditor::ROTATION:

					switch (currentAxisState)
					{
					case ColliderEditor::X:
						m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(glm::radians(changeAmountRot * factor), 0.0f, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Y:
						m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(0.0f, glm::radians(changeAmountRot * factor), 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Z:
						m_colliders[currentColliderIndex]->GetTransform()->SetRotation(*m_colliders[currentColliderIndex]->GetTransform()->GetRotation() - glm::vec3(0.0f, 0.0f, glm::radians(changeAmountRot * factor)));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					}

					break;



				case ColliderEditor::SCALE:

					switch (currentAxisState)
					{
					case ColliderEditor::X:
						m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(changeAmountScale * factor, 0.0f, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(changeAmountScale * factor, 0.0f, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Y:
						m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, changeAmountScale * factor, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, changeAmountScale * factor, 0.0f));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
						break;
					case ColliderEditor::Z:
						m_colliders[currentColliderIndex]->GetTransform()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, 0.0f, changeAmountScale * factor));
						m_colliders[currentColliderIndex]->GetCollider()->SetScale(*m_colliders[currentColliderIndex]->GetTransform()->GetScale() - glm::vec3(0.0f, 0.0f, changeAmountScale * factor));
						m_colliders[currentColliderIndex]->GetCollider()->UpdateCollider(*m_colliders[currentColliderIndex]->GetTransform()->GetPosition(), *m_colliders[currentColliderIndex]->GetTransform()->GetRotation());
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



	//index chaning
	void HandleInputForChangingIndex(const Uint8* state)
	{

		//change index
		if (state[SDL_SCANCODE_E])
		{
			if (!eDown)
			{
				eDown = true;
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
			eDown = false;
		}


		if (state[SDL_SCANCODE_Q])
		{
			if (!qDown)
			{
				qDown = true;
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
			qDown = false;
		}
	}



	//hide those meshes so you can see the beautiful colliders
	void HandleInputForHidingMesh(const Uint8* state)
	{
		//hide meshes
		if (state[SDL_SCANCODE_H])
		{
			if (!hDown)
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
	}


	//save the work
	void HandleInputForForceSaving(const Uint8* state)
	{
		//force save
		if (state[SDL_SCANCODE_P])
		{
			if (!pDown)
			{
				pDown = true;
				refreshEditorOutput();
				saveCollidersToBinary("..\\res\\Binary\\DONTDELETE\\colliders.bin");
				std::cout << "Colliders Saved\n";
			}

		}
		else
		{
			pDown = false;
		}
	}




	void UpdateEditor()
	{
		const Uint8* state = SDL_GetKeyboardState(NULL);

		HandleInputForCreatingAndDeletingColliders(state);

		HandleInputForEditorType(state);

		HandleInputForAxisType(state);

		HandleInputForTypingStatChange(state);

		HandleInputForStatChanging(state);

		HandleInputForChangingIndex(state);

		HandleInputForHidingMesh(state);

		HandleInputForForceSaving(state);

	}




	//30 is how many lines we keep and clear the rest
	void refreshEditorOutput() 
	{
		std::cout << "\033[30;1H";

		std::cout << "\033[J";

		std::cout << std::flush;
	}


	//on closing editor (not closing program) save
	void CloseEditor()
	{
		std::system("cls");
		saveCollidersToBinary("..\\res\\Binary\\DONTDELETE\\colliders.bin");
		hideMeshes = false;
	}


	//draw the lovely colliders in polygon mode
	void DrawEditor(Camera& mainCamera)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0f);
		for (int i = 0; i < m_colliders.size(); i++)
		{

			m_colliders[i]->GetShader()->Bind();

			if (i == currentColliderIndex)
				m_colliders[i]->GetShader()->Update(*m_colliders[i]->GetTransform(), mainCamera, true);
			else
				m_colliders[i]->GetShader()->Update(*m_colliders[i]->GetTransform(), mainCamera, false);

			m_colliders[i]->GetTexture()->Bind(0);
			m_colliders[i]->GetMesh()->draw();

		}

		//tank colliders
		m_tankBodyCollider.GetShader()->Bind();
		m_tankBodyCollider.GetShader()->Update(*m_tankBodyCollider.GetTransform(), mainCamera, false);
		m_tankBodyCollider.GetTexture()->Bind(0);
		m_tankBodyCollider.GetMesh()->draw();

		m_enemyTankBodyCollider.GetShader()->Bind();
		m_enemyTankBodyCollider.GetShader()->Update(*m_enemyTankBodyCollider.GetTransform(), mainCamera, false);
		m_enemyTankBodyCollider.GetTexture()->Bind(0);
		m_enemyTankBodyCollider.GetMesh()->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}



	//for drawing wall hacks when not in editor mode
	void WallHacks(Camera& mainCamera)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0f);
		m_enemyTankBodyCollider.GetShader()->Bind();
		m_enemyTankBodyCollider.GetShader()->Update(*m_enemyTankBodyCollider.GetTransform(), mainCamera, false);
		m_enemyTankBodyCollider.GetTexture()->Bind(0);
		m_enemyTankBodyCollider.GetMesh()->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}


	//save the collider data to binary file
	void saveCollidersToBinary(const std::string& filename)
	{
		std::ofstream file(filename, std::ios::binary);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> rotations;
		std::vector<glm::vec3> scales;

		for (int i = 0; i < m_colliders.size(); i++)
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


	//load that binary file
	void loadCollidersFromBinary(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);

		if (!file.good())
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

		for (int i = 0; i < m_colliders.size(); i++)
		{
			m_colliders[i] = std::make_unique<GameObject>();
			m_colliders[i]->GetTransform()->SetPosition(positions[i]);
			m_colliders[i]->GetTransform()->SetRotation(rotations[i]);
			m_colliders[i]->GetTransform()->SetScale(scales[i]);
			m_colliders[i]->GetCollider()->SetScale(*m_colliders[i]->GetTransform()->GetScale());
			m_colliders[i]->GetCollider()->UpdateCollider(*m_colliders[i]->GetTransform()->GetPosition(), *m_colliders[i]->GetTransform()->GetRotation());
			m_colliders[i]->SetShader(*shaderManager->GetShader(COLLIDEROUTLINE));
			m_colliders[i]->SetTexture(*textureManager->GetTexture(NONE));
			m_colliders[i]->SetMesh(*meshManager->GetMesh(CUBE_M));
		}

	}



	//make collider for tank
	void CreateTankBodyCollider()
	{
		m_tankBodyCollider = GameObject();
		m_tankBodyColliderOffset = glm::vec3(0.0, 0.3, 0.1);
		m_tankBodyCollider.GetTransform()->SetPosition(*playerTankBody->GetTransform()->GetPosition() + m_tankBodyColliderOffset);
		m_tankBodyCollider.GetTransform()->SetRotation(glm::vec3(0.0));
		m_tankBodyCollider.GetTransform()->SetScale(glm::vec3(0.8, 0.6, 1.5));
		m_tankBodyCollider.GetCollider()->SetScale(*m_tankBodyCollider.GetTransform()->GetScale());
		m_tankBodyCollider.GetCollider()->UpdateCollider(*m_tankBodyCollider.GetTransform()->GetPosition(), *m_tankBodyCollider.GetTransform()->GetRotation());
		m_tankBodyCollider.SetShader(*shaderManager->GetShader(COLLIDEROUTLINE));
		m_tankBodyCollider.SetTexture(*textureManager->GetTexture(NONE));
		m_tankBodyCollider.SetMesh(*meshManager->GetMesh(CUBE_M));
	}


	//make colldier for enemy
	void CreateEnemyTankBodyCollider()
	{
		m_enemyTankBodyCollider = GameObject();
		m_tankBodyColliderOffset = glm::vec3(0.0, 0.3, 0.0);
		m_enemyTankBodyCollider.GetTransform()->SetPosition(*enemyTankBody->GetTransform()->GetPosition() + m_tankBodyColliderOffset);
		m_enemyTankBodyCollider.GetTransform()->SetRotation(glm::vec3(0.0));
		m_enemyTankBodyCollider.GetTransform()->SetScale(glm::vec3(0.8, 0.8, 1.5));
		m_enemyTankBodyCollider.GetCollider()->SetScale(*m_enemyTankBodyCollider.GetTransform()->GetScale());
		m_enemyTankBodyCollider.GetCollider()->UpdateCollider(*m_enemyTankBodyCollider.GetTransform()->GetPosition(), *m_enemyTankBodyCollider.GetTransform()->GetRotation());
		m_enemyTankBodyCollider.SetShader(*shaderManager->GetShader(COLLIDEROUTLINE));
		m_enemyTankBodyCollider.SetTexture(*textureManager->GetTexture(NONE));
		m_enemyTankBodyCollider.SetMesh(*meshManager->GetMesh(CUBE_M));
	}




	//displays what the user is editing in the console
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



	//{collided with anything, collided with tank}
	std::array<bool, 2> CollisionDetection()
	{

		if (m_tankBodyCollider.GetCollider()->IsCollidingWith(m_enemyTankBodyCollider.GetCollider()))
		{
			return { true, true };
		}


		for (int i = 0; i < m_colliders.size(); i++)
		{
			if (m_tankBodyCollider.GetCollider()->IsCollidingWith(m_colliders[i]->GetCollider()))
			{
				return { true, false };
			}
		}

		

		return { false, false };
	}


	glm::vec3 ShootDetection(const glm::vec3& rayStartPos, const glm::vec3& rayDirection, bool isPlayer, bool kill)
	{
		//bullet data
		std::vector<BulletHitData> bullletHitData = std::vector<BulletHitData>();


		//check colliders for hits (could be optimized but no time)
		for (int i = 0; i < m_colliders.size(); i++)
		{
			float distance = 0.0f;
			Collider* collider = m_colliders[i]->GetCollider();
			if (collider->CheckRayHit(rayStartPos, rayDirection, *collider, distance))
			{
				BulletHitData temp;
				temp.colliderHit = collider;
				temp.distance = distance;
				bullletHitData.push_back(temp);
			}
		}


		//closest hit from that data
		BulletHitData* closestHit = GetClosestHit(bullletHitData);


		//we check the closest building hit with the enemy tank and if the tank is closer kill that fella, if not we shot a building 
		float distance = 0.0f;
		if (m_enemyTankBodyCollider.GetCollider()->CheckRayHit(rayStartPos, rayDirection, *m_enemyTankBodyCollider.GetCollider(), distance))
		{
			if (closestHit != nullptr)
			{
				if (distance < closestHit->distance)
				{
					if (kill)
					{
						enemyTankRef->KillTank();
					}			
					return *enemyTankRef->GetBody()->GetTransform()->GetPosition();
				}
			}
			else
			{
				if (kill)
				{
					enemyTankRef->KillTank();
				}
				return *enemyTankRef->GetBody()->GetTransform()->GetPosition();
			}
		}


		//if we hit a building
		if (closestHit != nullptr)
		{
			return closestHit->colliderHit->GetPosition();
		}

		//if nothing was hit (looking at lovely skybox) return a value big enough to work
		return glm::vec3(50.0);
	}



	BulletHitData* GetClosestHit(std::vector<BulletHitData>& hits) const
	{
		if (hits.empty())
		{
			return nullptr;
		}

		//assume first hit was closest
		int closestIndex = 0;
		float minDistance = hits[0].distance;

		for (int i = 1; i < hits.size(); i++)
		{
			//if closer
			if (hits[i].distance < minDistance)
			{
				//thats our guy
				minDistance = hits[i].distance;
				closestIndex = i;
			}
		}

		return &hits[closestIndex];
	}



	//ai check for player in 180 cone ahead of it 
	bool CheckForPlayer()
	{
		glm::vec3 enemyPos = *enemyTankRef->GetBody()->GetTransform()->GetPosition();
		glm::vec3 playerPos = *playerTankRef->GetBody()->GetTransform()->GetPosition();
		glm::vec3 toPlayer = glm::normalize(playerPos - enemyPos);


		//180 degree radius in front
		float turretRot = enemyTankRef->GetTurret()->GetTransform()->GetRotation()->y;
		glm::vec3 turretForward = glm::vec3(std::sin(turretRot), 0, std::cos(turretRot));
		float dot = glm::dot(turretForward, toPlayer);

		//if the player is behind the turret dont bother
		if (dot <= 0.0f) 
		{
			return false;
		}

		//things for line of sight checks
		float distToPlayer = glm::distance(enemyPos, playerPos);
		float closestObstacleDist = 100000.0f;
		bool buildingInWay = false;

		
		//check if it hit any buildings
		for (int i = 0; i < m_colliders.size(); i++)
		{
			float d = 0.0f;
			Collider* collider = m_colliders[i]->GetCollider();
			if (collider->CheckRayHit(enemyPos, toPlayer, *collider, d))
			{
				if (d < closestObstacleDist) 
				{
					closestObstacleDist = d;
				}
			}
		}
			

		//if the closest thing we hit was closer than the player then we didnt hit the plyer
		if (closestObstacleDist > distToPlayer) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	

	GameObject* GetTankCollider() { return &m_tankBodyCollider; }
	GameObject* GetEnemyTankCollider() { return &m_enemyTankBodyCollider; }
	glm::vec3* GetTankColliderOffset() { return &m_tankBodyColliderOffset; }

	private:

		enum EditorState { POSITION, ROTATION, SCALE };
		enum AxisState { X, Y, Z };

		EditorState currentEditorState = EditorState::POSITION;
		AxisState currentAxisState = AxisState::X;


		std::vector<std::unique_ptr<GameObject>> m_colliders;

		int currentColliderIndex = 0;

		//key downs
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
		bool cDown = false;
		bool eDown = false;
		bool qDown = false;
		bool hDown = false;
		bool oneDown = false;
		bool twoDown = false;
		bool pDown = false;

		

		bool hideMeshes = false;



		float changeAmountPos = 1.0f;	
		float changeAmountRot = 5.0f;	
		float changeAmountScale = 1.0f;	

		ShaderManager* shaderManager;
		TextureManager* textureManager;
		MeshManager* meshManager;
		GameObject* playerTankBody;
		GameObject* enemyTankBody;

		Tank* playerTankRef = nullptr;
		Tank* enemyTankRef = nullptr;

		GameObject m_tankBodyCollider;
		GameObject m_enemyTankBodyCollider;

		glm::vec3 m_tankBodyColliderOffset;
};