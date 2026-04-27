#pragma once

#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"

struct Tank
{

public:

	float forwardSpeed = 5.0f;	
	float backwardSpeed = 2.5f;	

	float bodyRotSpeed = 1.0f;
	float turretRotSpeed = 1.0f;

	glm::vec3 cameraOffset = glm::vec3();
	glm::vec3 turretOffset = glm::vec3();

	Tank()
	{
		camera = nullptr;
	}


	Tank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* camera)
	{
		//leclerc body
		body.GetTransform()->SetPosition(glm::vec3(0.0, -0.80, -1.5));
		body.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		body.GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		body.SetShader(*shaderManager.GetShader(ADS));
		body.SetTexture(*textureManager.GetTexture(NONE));
		body.SetMesh(*meshManager.GetMesh(LECLERCBODY));

		//turret
		turret.GetTransform()->SetPosition(glm::vec3(0.0, -1.04, -1.35));
		turret.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		turret.GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		turret.SetShader(*shaderManager.GetShader(ADS));
		turret.SetTexture(*textureManager.GetTexture(NONE));
		turret.SetMesh(*meshManager.GetMesh(LECLERCTURRET));

		this->camera = camera;


		cameraOffset = camera->GetPosition() - *turret.GetTransform()->GetPosition();

		turretOffset = *turret.GetTransform()->GetPosition() - *body.GetTransform()->GetPosition();
	}


    // Replace all occurrences of turret-> with turret. in Tank methods

    void SetPosition(glm::vec3 position)
    {
		body.GetTransform()->SetPosition(position);
		turret.GetTransform()->SetPosition(position);
    }
    void SetRotation(glm::vec3 rotation)
    {
		body.GetTransform()->SetRotation(rotation);
		turret.GetTransform()->SetRotation(rotation);
    }
    void SetScale(glm::vec3 scale)
    {
		body.GetTransform()->SetScale(scale);
		turret.GetTransform()->SetScale(scale);
    }

    void MoveForward(float deltaTime)
    {
		glm::vec3 forward = body.GetTransform()->GetForward();
		body.GetTransform()->move(forward * forwardSpeed * deltaTime);
		turret.GetTransform()->move(forward * forwardSpeed * deltaTime);
		camera->move(forward * forwardSpeed * deltaTime);
	}
	

    void MoveBackwards(float deltaTime)
    {
		glm::vec3 backward = -body.GetTransform()->GetForward();
		body.GetTransform()->move(backward * forwardSpeed * deltaTime);
		turret.GetTransform()->move(backward * forwardSpeed * deltaTime);
		camera->move(backward * forwardSpeed * deltaTime);
    }

	void RotateBodyLeft(float deltaTime)
	{
		body.GetTransform()->rotate(glm::vec3(0.0, bodyRotSpeed * deltaTime, 0.0));
		glm::vec3 bodyPos = *body.GetTransform()->GetPosition();
		glm::mat4 bodyRot = glm::rotate(body.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(bodyRot * glm::vec4(turretOffset, 0.0f));
		turret.GetTransform()->SetPosition(bodyPos + rotatedOffset);


		glm::mat4 camRot = glm::rotate(turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turret.GetTransform()->GetPosition() + camOffset);
	}

    void RotateBodyRight(float deltaTime)
    {
		body.GetTransform()->rotate(glm::vec3(0.0, -bodyRotSpeed * deltaTime, 0.0));
		glm::vec3 bodyPos = *body.GetTransform()->GetPosition();
		glm::mat4 bodyRot = glm::rotate(body.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(bodyRot * glm::vec4(turretOffset, 0.0f));
		turret.GetTransform()->SetPosition(bodyPos + rotatedOffset);

		glm::mat4 camRot = glm::rotate(turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turret.GetTransform()->GetPosition() + camOffset);
    }

	void RotateTurretLeft(float deltaTime)
	{
		turret.GetTransform()->rotate(glm::vec3(0.0f, turretRotSpeed * deltaTime, 0.0f));
		camera->rotate(turretRotSpeed * deltaTime, glm::vec3(0, 1, 0));

		glm::mat4 rotMat = glm::rotate(turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));

		camera->SetPosition(*turret.GetTransform()->GetPosition() + rotatedOffset);
	}

    void RotateTurretRight(float deltaTime)
    {
		turret.GetTransform()->rotate(glm::vec3(0.0f, -turretRotSpeed * deltaTime, 0.0f));
		camera->rotate(-turretRotSpeed * deltaTime, glm::vec3(0, 1, 0));

		glm::mat4 rotMat = glm::rotate(turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));

		camera->SetPosition(*turret.GetTransform()->GetPosition() + rotatedOffset);
    }

    


	void ResetCameraToTank()
	{
		glm::mat4 rotMat = glm::rotate(turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turret.GetTransform()->GetPosition() + rotatedOffset);
		camera->SetRotation(glm::vec3(0, turret.GetTransform()->GetRotation()->y, 0));
	}

	void Draw()
	{
		body.GetShader()->Bind();
		body.GetShader()->Update(*body.GetTransform(), *camera);
		body.GetTexture()->Bind(0);
		body.GetMesh()->draw();

		turret.GetShader()->Bind();
		turret.GetShader()->Update(*turret.GetTransform(), *camera);
		turret.GetTexture()->Bind(0);
		turret.GetMesh()->draw();
	}



private:
	GameObject body;
	GameObject turret;

	Camera* camera;
};
