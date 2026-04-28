#pragma once
#include <iostream>	
#include <cstdlib>
#include <SDL\SDL.h>
#include <memory>
#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"

#define NUM_COLLIDERS 64

struct ColliderEditor
{

	public:

		ColliderEditor() {}


		ColliderEditor(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager)
		{
			for (int i = 0; i < NUM_COLLIDERS; i++)
			{
				m_colliders[i] = std::make_unique<GameObject>();
				m_colliders[i]->GetTransform()->SetPosition(glm::vec3(0.0, 0.0, 0.0));
				m_colliders[i]->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
				m_colliders[i]->GetTransform()->SetScale(glm::vec3(1.0, 1.0, 1.0));
				m_colliders[i]->SetShader(*shaderManager.GetShader(RIM_LIGHT));
				m_colliders[i]->SetTexture(*textureManager.GetTexture(NONE));
				m_colliders[i]->SetMesh(*meshManager.GetMesh(CUBE));
			}
		}

		void OpenEditor()
		{
			std::cout << "Collider Editor\n, - Position\n. - Rotation\n/ - Scale\nAxis - x/y/z\nChange Stat +/-\nSpace - Type Change Value\n";				
		}


		void UpdateEditor()
		{
			const Uint8* state = SDL_GetKeyboardState(NULL);

			if (state[SDL_SCANCODE_COMMA] && currentEditorState != EditorState::POSITION)
			{
				std::cout << "Position\n";
				currentEditorState = EditorState::POSITION;
			}
			if (state[SDL_SCANCODE_PERIOD] && currentEditorState != EditorState::ROTATION)
			{
				std::cout << "Rotation\n";
				currentEditorState = EditorState::ROTATION;
			}
			if (state[SDL_SCANCODE_SLASH] && currentEditorState != EditorState::SCALE)
			{
				std::cout << "Scale\n";
				currentEditorState = EditorState::SCALE;
			}

			if (state[SDL_SCANCODE_X] && currentAxisState != AxisState::X)
			{
				std::cout << "Editing X axis\n";
				currentAxisState = AxisState::X;
			}
			if (state[SDL_SCANCODE_Y] && currentAxisState != AxisState::Y)
			{
				std::cout << "Editing Y axis\n";
				currentAxisState = AxisState::Y;
			}
			if (state[SDL_SCANCODE_Z] && currentAxisState != AxisState::Z)
			{
				std::cout << "Editing Z axis\n";
				currentAxisState = AxisState::Z;
			}


			if(state[SDL_SCANCODE_EQUALS])
			{
				if (!plusDown)
				{
					plusDown = true;
					std::cout << "plus down\n";
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

			if(state[SDL_SCANCODE_MINUS])
			{
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


		void CloseEditor()
		{
			std::system("cls");
		}	


		void DrawEditor(Camera& mainCamera)
		{
			for (int i = 0; i < NUM_COLLIDERS; i++)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Switch to wireframe
				glLineWidth(2.0f); // Make the lines thicker (optional)
				m_colliders[i]->GetShader()->Bind();
				m_colliders[i]->GetShader()->Update(*m_colliders[i]->GetTransform(), mainCamera, true);
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

		std::unique_ptr<GameObject> m_colliders[NUM_COLLIDERS];
		int currentColliderIndex = 0;


		bool plusDown = false;
		bool minusDown = false;

		float changeAmountPos = 1.0f;	
		float changeAmountRot = 5.0f;	
		float changeAmountScale = 1.0f;	
};