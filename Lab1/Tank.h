#pragma once

#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"

struct Tank
{

public:

	struct MuzzleFlash
	{
		float lifetime = 0.0f;   
		float maxLifetime = 0.1f;

		bool IsAlive() { return lifetime > 0; }
	};

	float currentSpeed = 0.0f;

	float acceleration = 1.5f;
	float deceleration = 2.5f;

	float maxForwardSpeed = 7.5f;
	float maxBackwardSpeed = 3.0f;

	bool movingForward = false;	
	bool movingBackward = false;	

	float turnSpeedPenalty = 0.75f;
	float brakeForce = 5.f;


	float bodyRotSpeed = 1.0f;
	float turretRotSpeed = 1.0f;

	float maxReloadTime = 6.0f;
	float currentReloadTime = 0.0f;

	glm::vec3 cameraOffset = glm::vec3();
	glm::vec3 turretOffset = glm::vec3();
	float muzzleFlashOffset = 0.0;

	Tank()
	{
		camera = nullptr;
	}


	Tank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* camera)
	{
		//leclerc body
		body.GetTransform()->SetPosition(glm::vec3(0.0, -0.5, -1.5));
		body.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		body.GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		body.SetShader(*shaderManager.GetShader(ADS));
		body.SetTexture(*textureManager.GetTexture(NONE));
		body.SetMesh(*meshManager.GetMesh(LECLERCBODY));

		//turret
		turret.GetTransform()->SetPosition(glm::vec3(0.0, -0.575, -1.5));
		turret.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		turret.GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		turret.SetShader(*shaderManager.GetShader(ADS));
		turret.SetTexture(*textureManager.GetTexture(NONE));
		turret.SetMesh(*meshManager.GetMesh(LECLERCTURRET));

		//muzzle flash
		muzzleFlash.GetTransform()->SetPosition(glm::vec3(0.0, -0.45, -0.1));
		muzzleFlash.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		muzzleFlash.GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		muzzleFlash.SetShader(*shaderManager.GetShader(REMOVE_BACKGRROUND));
		muzzleFlash.SetTexture(*textureManager.GetTexture(MUZZLEFLASH));
		muzzleFlash.SetMesh(*meshManager.GetMesh(QUAD));

		this->camera = camera;


		cameraOffset = camera->GetPosition() - *turret.GetTransform()->GetPosition();

		turretOffset = *turret.GetTransform()->GetPosition() - *body.GetTransform()->GetPosition();

		muzzleFlashOffset = muzzleFlash.GetTransform()->GetPosition()->y - turret.GetTransform()->GetPosition()->y;
	}


	void Update(float deltaTime)
	{
		// Detect direction change and brake
		bool braking = (movingForward && currentSpeed < 0) || (movingBackward && currentSpeed > 0);

		if (braking)
		{
			if (currentSpeed > 0)
			{
				currentSpeed -= brakeForce * deltaTime;
				if (currentSpeed < 0) currentSpeed = 0;
			}
			else if (currentSpeed < 0)
			{
				currentSpeed += brakeForce * deltaTime;
				if (currentSpeed > 0) currentSpeed = 0;
			}
		}
		else if (movingForward)
		{
			currentSpeed += acceleration * deltaTime;
			if (currentSpeed > maxForwardSpeed)
				currentSpeed = maxForwardSpeed;
		}
		else if (movingBackward)
		{
			currentSpeed -= acceleration * deltaTime;
			if (currentSpeed < -maxBackwardSpeed)
				currentSpeed = -maxBackwardSpeed;
		}
		else
		{
			if (currentSpeed > 0)
			{
				currentSpeed -= deceleration * deltaTime;
				if (currentSpeed < 0) currentSpeed = 0;
			}
			else if (currentSpeed < 0)
			{
				currentSpeed += deceleration * deltaTime;
				if (currentSpeed > 0) currentSpeed = 0;
			}
		}

		if (currentSpeed != 0.0f)
		{
			glm::vec3 forward = body.GetTransform()->GetForward();
			body.GetTransform()->move(forward * currentSpeed * deltaTime);
			turret.GetTransform()->move(forward * currentSpeed * deltaTime);
			muzzleFlash.GetTransform()->move(forward * currentSpeed * deltaTime);
			camera->move(forward * currentSpeed * deltaTime);
		}

		movingForward = false;
		movingBackward = false;


		muzzleFlashData.lifetime -= deltaTime;
		currentReloadTime -= deltaTime;
	}

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
		movingForward = true;
	}
	

    void MoveBackwards(float deltaTime)
    {
		movingBackward = true;
    }

	void RotateBodyLeft(float deltaTime, bool checkMovebackwards)
	{
		//avoid infinite loop with checkMovebackwards
		if (checkMovebackwards && movingBackward)
		{
			RotateBodyRight(deltaTime ,false);
			return;
		}
	
		//apply turn speed penalty based on current speed
		float effectiveRotSpeed = bodyRotSpeed * (1.0f - (currentSpeed / maxForwardSpeed) * 0.5f);
		float rotationAmount = effectiveRotSpeed * deltaTime;

		body.GetTransform()->rotate(glm::vec3(0.0, rotationAmount, 0.0));

		//change current speed
		currentSpeed *= (1.0f - rotationAmount * turnSpeedPenalty);

		glm::vec3 bodyPos = *body.GetTransform()->GetPosition();
		glm::mat4 bodyRot = glm::rotate(body.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(bodyRot * glm::vec4(turretOffset, 0.0f));
		turret.GetTransform()->SetPosition(bodyPos + rotatedOffset);


		glm::mat4 camRot = glm::rotate(turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turret.GetTransform()->GetPosition() + camOffset);
	}

	//like RotateBodyLeft but with opposite rotation
    void RotateBodyRight(float deltaTime, bool checkMovebackwards)
    {

		if (checkMovebackwards && movingBackward)
		{
			RotateBodyLeft(deltaTime, false);
			return;
		}
		float effectiveRotSpeed = bodyRotSpeed * (1.0f - (currentSpeed / maxForwardSpeed) * 0.5f);
		float rotationAmount = effectiveRotSpeed * deltaTime;

		body.GetTransform()->rotate(glm::vec3(0.0, -rotationAmount, 0.0));

		currentSpeed *= (1.0f - rotationAmount * turnSpeedPenalty);

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
		Transform* turretTransform = turret.GetTransform();

		turretTransform->rotate(glm::vec3(0.0f, turretRotSpeed * deltaTime, 0.0f));

		muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));

		camera->rotate(turretRotSpeed * deltaTime, glm::vec3(0, 1, 0));
		glm::mat4 rotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);
	}

    void RotateTurretRight(float deltaTime)
    {
		Transform* turretTransform = turret.GetTransform();

		turretTransform->rotate(glm::vec3(0.0f, -turretRotSpeed * deltaTime, 0.0f));

		muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));

		camera->rotate(-turretRotSpeed * deltaTime, glm::vec3(0, 1, 0));
		glm::mat4 rotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);
    }

    
	void Shoot()
	{
		if(currentReloadTime > 0.0f)
		{
			return;
		}
		muzzleFlashData.lifetime = muzzleFlashData.maxLifetime;
		currentReloadTime = maxReloadTime;
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
		body.GetShader()->Update(*body.GetTransform(), *camera, false);
		body.GetTexture()->Bind(0);
		body.GetMesh()->draw();

		turret.GetShader()->Bind();
		turret.GetShader()->Update(*turret.GetTransform(), *camera, false);
		turret.GetTexture()->Bind(0);
		turret.GetMesh()->draw();

		if (muzzleFlashData.IsAlive())
		{
			muzzleFlash.GetShader()->Bind();
			muzzleFlash.GetShader()->Update(*muzzleFlash.GetTransform(), *camera, true);
			muzzleFlash.GetTexture()->Bind(0);
			muzzleFlash.GetMesh()->draw();
		}
	}



private:
	GameObject body;
	GameObject turret;
	GameObject muzzleFlash;

	MuzzleFlash muzzleFlashData;

	Camera* camera;
};
