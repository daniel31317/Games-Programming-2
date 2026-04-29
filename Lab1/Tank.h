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



	Tank()
	{
		camera = nullptr;
		tankCollider = nullptr;
	}


	Tank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* camera)
	{
		//leclerc body
		m_body.GetTransform()->SetPosition(glm::vec3(0.0, -0.5, -1.5));
		m_body.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		m_body.GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		m_body.SetShader(*shaderManager.GetShader(ADS));
		m_body.SetTexture(*textureManager.GetTexture(NONE));
		m_body.SetMesh(*meshManager.GetMesh(LECLERCBODY));

		//turret
		m_turret.GetTransform()->SetPosition(glm::vec3(0.0, -0.575, -1.5));
		m_turret.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		m_turret.GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		m_turret.SetShader(*shaderManager.GetShader(ADS));
		m_turret.SetTexture(*textureManager.GetTexture(NONE));
		m_turret.SetMesh(*meshManager.GetMesh(LECLERCTURRET));

		//muzzle flash
		m_muzzleFlash.GetTransform()->SetPosition(glm::vec3(0.0, -0.45, -0.1));
		m_muzzleFlash.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		m_muzzleFlash.GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		m_muzzleFlash.SetShader(*shaderManager.GetShader(REMOVE_BACKGRROUND));
		m_muzzleFlash.SetTexture(*textureManager.GetTexture(MUZZLEFLASH));
		m_muzzleFlash.SetMesh(*meshManager.GetMesh(QUAD));

		this->camera = camera;



		cameraOffset = camera->GetPosition() - *m_turret.GetTransform()->GetPosition();

		turretOffset = *m_turret.GetTransform()->GetPosition() - *m_body.GetTransform()->GetPosition();

		muzzleFlashOffset = m_muzzleFlash.GetTransform()->GetPosition()->y - m_turret.GetTransform()->GetPosition()->y;


		tankCollider = nullptr;
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
			if (*tankCollider->GetCollider()->GetCollisionSide() != Collider::CollisionSide::FrontRight && *tankCollider->GetCollider()->GetCollisionSide() != Collider::CollisionSide::FrontLeft)
			{
				currentSpeed += acceleration * deltaTime;
				if (currentSpeed > maxForwardSpeed)
					currentSpeed = maxForwardSpeed;
			}
		}
		else if (movingBackward)
		{
			if (*tankCollider->GetCollider()->GetCollisionSide() != Collider::CollisionSide::BackRight && *tankCollider->GetCollider()->GetCollisionSide() != Collider::CollisionSide::BackLeft)
			{
				currentSpeed -= acceleration * deltaTime;
				if (currentSpeed < -maxBackwardSpeed)
					currentSpeed = -maxBackwardSpeed;
			}
			
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
			glm::vec3 forward = m_body.GetTransform()->GetForward();
			m_body.GetTransform()->move(forward * currentSpeed * deltaTime);
			tankCollider->GetTransform()->move(forward * currentSpeed * deltaTime);
			tankCollider->GetCollider()->UpdateCollider(*tankCollider->GetTransform()->GetPosition(), *tankCollider->GetTransform()->GetRotation());
			m_turret.GetTransform()->move(forward * currentSpeed * deltaTime);
			m_muzzleFlash.GetTransform()->move(forward * currentSpeed * deltaTime);
			camera->move(forward * currentSpeed * deltaTime);
		
		}

		movingForward = false;
		movingBackward = false;


		muzzleFlashData.lifetime -= deltaTime;
		currentReloadTime -= deltaTime;
	}

    void SetPosition(glm::vec3 position)
    {
		m_body.GetTransform()->SetPosition(position);
		m_turret.GetTransform()->SetPosition(position);
    }
    void SetRotation(glm::vec3 rotation)
    {
		m_body.GetTransform()->SetRotation(rotation);
		m_turret.GetTransform()->SetRotation(rotation);
    }
    void SetScale(glm::vec3 scale)
    {
		m_body.GetTransform()->SetScale(scale);
		m_turret.GetTransform()->SetScale(scale);
    }

	void SetTankCollider(GameObject* tankCollider)
	{
		this->tankCollider = tankCollider;
	}


	GameObject* GetBody() { return &m_body; }
	GameObject* GetTurret() { return &m_turret; }

    void MoveForward(float deltaTime)
    {
		movingForward = true;
	}
	

    void MoveBackwards(float deltaTime)
    {
		movingBackward = true;
    }

	void RotateBodyLeft(float deltaTime, bool checkMovebackwards, glm::vec3& tankColliderOffset)
	{
		if (collidedLastFrame)
		{
			switch (*tankCollider->GetCollider()->GetCollisionSide())
			{
				case Collider::CollisionSide::FrontLeft :
				case Collider::CollisionSide::BackRight :
					return;
				default: break;
			}
		}
		//avoid infinite loop with checkMovebackwards
		if (checkMovebackwards && movingBackward)
		{
			RotateBodyRight(deltaTime ,false, tankColliderOffset);
			return;
		}
	
		//apply turn speed penalty based on current speed
		float effectiveRotSpeed = bodyRotSpeed * (1.0f - (currentSpeed / maxForwardSpeed) * 0.5f);
		float rotationAmount = effectiveRotSpeed * deltaTime;

		m_body.GetTransform()->rotate(glm::vec3(0.0, rotationAmount, 0.0));
		tankCollider->GetTransform()->rotate(glm::vec3(0.0, rotationAmount, 0.0));

		//change current speed
		currentSpeed *= (1.0f - rotationAmount * turnSpeedPenalty);

		glm::vec3 bodyPos = *m_body.GetTransform()->GetPosition();
		glm::mat4 bodyRot = glm::rotate(m_body.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(bodyRot * glm::vec4(turretOffset, 0.0f));
		m_turret.GetTransform()->SetPosition(bodyPos + rotatedOffset);


		glm::vec3 rotatedColliderOffset = glm::vec3(bodyRot * glm::vec4(tankColliderOffset, 0.0f));
		tankCollider->GetTransform()->SetPosition(bodyPos + rotatedColliderOffset);
		tankCollider->GetCollider()->UpdateCollider(*tankCollider->GetTransform()->GetPosition(), *tankCollider->GetTransform()->GetRotation());

		glm::mat4 camRot = glm::rotate(m_turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*m_turret.GetTransform()->GetPosition() + camOffset);

	}

	//like RotateBodyLeft but with opposite rotation
    void RotateBodyRight(float deltaTime, bool checkMovebackwards, glm::vec3& tankColliderOffset)
    {
		if (collidedLastFrame)
		{
			switch (*tankCollider->GetCollider()->GetCollisionSide())
			{
			case Collider::CollisionSide::FrontRight:
			case Collider::CollisionSide::BackLeft:
				return;
			default: break;
			}
		}
		if (checkMovebackwards && movingBackward)
		{
			RotateBodyLeft(deltaTime, false, tankColliderOffset);
			return;
		}
		float effectiveRotSpeed = bodyRotSpeed * (1.0f - (currentSpeed / maxForwardSpeed) * 0.5f);
		float rotationAmount = effectiveRotSpeed * deltaTime;

		m_body.GetTransform()->rotate(glm::vec3(0.0, -rotationAmount, 0.0));
		tankCollider->GetTransform()->rotate(glm::vec3(0.0, -rotationAmount, 0.0));

		currentSpeed *= (1.0f - rotationAmount * turnSpeedPenalty);

		glm::vec3 bodyPos = *m_body.GetTransform()->GetPosition();
		glm::mat4 bodyRot = glm::rotate(m_body.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(bodyRot * glm::vec4(turretOffset, 0.0f));
		m_turret.GetTransform()->SetPosition(bodyPos + rotatedOffset);

		glm::vec3 rotatedColliderOffset = glm::vec3(bodyRot * glm::vec4(tankColliderOffset, 0.0f));
		tankCollider->GetTransform()->SetPosition(bodyPos + rotatedColliderOffset);
		tankCollider->GetCollider()->UpdateCollider(*tankCollider->GetTransform()->GetPosition(), *tankCollider->GetTransform()->GetRotation());

		glm::mat4 camRot = glm::rotate(m_turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*m_turret.GetTransform()->GetPosition() + camOffset);
    }

	void RotateTurretLeft(float deltaTime)
	{
		Transform* turretTransform = m_turret.GetTransform();

		turretTransform->rotate(glm::vec3(0.0f, turretRotSpeed * deltaTime, 0.0f));

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));

		camera->rotate(turretRotSpeed * deltaTime, glm::vec3(0, 1, 0));
		glm::mat4 rotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);
	}

    void RotateTurretRight(float deltaTime)
    {
		Transform* turretTransform = m_turret.GetTransform();

		turretTransform->rotate(glm::vec3(0.0f, -turretRotSpeed * deltaTime, 0.0f));

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));

		camera->rotate(-turretRotSpeed * deltaTime, glm::vec3(0, 1, 0));
		glm::mat4 rotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);
    }



	void HandleColliison(bool collided)
	{
		if (collided && collided != collidedLastFrame)
		{
			currentSpeed = -currentSpeed / 8;
		}	
		collidedLastFrame = collided;
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
		glm::mat4 rotMat = glm::rotate(m_turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*m_turret.GetTransform()->GetPosition() + rotatedOffset);
		camera->SetRotation(glm::vec3(0, m_turret.GetTransform()->GetRotation()->y, 0));
	}

	void Draw()
	{
		m_body.GetShader()->Bind();
		m_body.GetShader()->Update(*m_body.GetTransform(), *camera, false);
		m_body.GetTexture()->Bind(0);
		m_body.GetMesh()->draw();

		m_turret.GetShader()->Bind();
		m_turret.GetShader()->Update(*m_turret.GetTransform(), *camera, false);
		m_turret.GetTexture()->Bind(0);
		m_turret.GetMesh()->draw();

		if (muzzleFlashData.IsAlive())
		{
			m_muzzleFlash.GetShader()->Bind();
			m_muzzleFlash.GetShader()->Update(*m_muzzleFlash.GetTransform(), *camera, true);
			m_muzzleFlash.GetTexture()->Bind(0);
			m_muzzleFlash.GetMesh()->draw();
		}
	}



private:
	GameObject m_body;
	GameObject m_turret;
	GameObject m_muzzleFlash;
	GameObject* tankCollider;

	MuzzleFlash muzzleFlashData;

	bool collidedLastFrame;

	Camera* camera;

	float currentSpeed = 0.0f;

	const float acceleration = 1.5f;
	const float deceleration = 2.5f;

	const float maxForwardSpeed = 7.5f;
	const float maxBackwardSpeed = 3.0f;

	bool movingForward = false;
	bool movingBackward = false;

	const float turnSpeedPenalty = 0.9f;
	const float brakeForce = 5.f;


	const float bodyRotSpeed = 1.0f;
	const float turretRotSpeed = 1.5f;

	const float maxReloadTime = 6.0f;
	float currentReloadTime = 0.0f;

	glm::vec3 cameraOffset = glm::vec3();
	glm::vec3 turretOffset = glm::vec3();
	float muzzleFlashOffset = 0.0;
};
