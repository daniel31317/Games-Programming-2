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


	Tank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* camera, bool isPlayer)
	{

		if (isPlayer)
		{
			//leclerc body
			m_body.GetTransform()->SetPosition(glm::vec3(0.0, -0.98, -1.5));
			m_body.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
			m_body.GetTransform()->SetScale(glm::vec3(0.35f, 0.35f, 0.35f));
			m_body.SetShader(*shaderManager.GetShader(TANK));
			m_body.SetTexture(*textureManager.GetTexture(LECLERCHULL_T));
			m_body.SetMesh(*meshManager.GetMesh(LECLERCHULL_M));

			//turret
			m_turret.GetTransform()->SetPosition(glm::vec3(0.0, -0.68, -1.25));
			m_turret.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
			m_turret.GetTransform()->SetScale(glm::vec3(0.35f, 0.35f, 0.35f));
			m_turret.SetShader(*shaderManager.GetShader(TANK));
			m_turret.SetTexture(*textureManager.GetTexture(LECLERCTURRET_T));
			m_turret.SetMesh(*meshManager.GetMesh(LECLERCTURRET_M));


			//barrel only for player
			m_barrel.GetTransform()->SetPosition(glm::vec3(0.0, -0.55, -0.975));
			m_barrel.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
			m_barrel.GetTransform()->SetScale(glm::vec3(0.35f, 0.35f, 0.35f));
			m_barrel.SetShader(*shaderManager.GetShader(TANK));
			m_barrel.SetTexture(*textureManager.GetTexture(LECLERCBARREL_T));
			m_barrel.SetMesh(*meshManager.GetMesh(LECLERCBARREL_M));
			barrelOffset = *m_barrel.GetTransform()->GetPosition() - *m_turret.GetTransform()->GetPosition();

		}
		else
		{
			//t80 body
			m_body.GetTransform()->SetPosition(glm::vec3(0.0, -0.94, -1.5));
			m_body.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
			m_body.GetTransform()->SetScale(glm::vec3(0.20f, 0.20f, 0.20f));
			m_body.SetShader(*shaderManager.GetShader(TANK));
			m_body.SetTexture(*textureManager.GetTexture(T80HULL_T));
			m_body.SetMesh(*meshManager.GetMesh(T80HULL_M));

			//turret
			m_turret.GetTransform()->SetPosition(glm::vec3(0.0, -0.94, -1.5));
			m_turret.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
			m_turret.GetTransform()->SetScale(glm::vec3(0.20f, 0.20f, 0.20f));
			m_turret.SetShader(*shaderManager.GetShader(TANK));
			m_turret.SetTexture(*textureManager.GetTexture(T80TURRET_T));
			m_turret.SetMesh(*meshManager.GetMesh(T80TURRET_M));
		}
		

		//muzzle flash
		m_muzzleFlash.GetTransform()->SetPosition(glm::vec3(0.0, -0.45, -0.1));
		m_muzzleFlash.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		m_muzzleFlash.GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		m_muzzleFlash.SetShader(*shaderManager.GetShader(REMOVE_BACKGRROUND));
		m_muzzleFlash.SetTexture(*textureManager.GetTexture(MUZZLEFLASH));
		m_muzzleFlash.SetMesh(*meshManager.GetMesh(QUAD_M));

		noiseTexture = textureManager.GetTexture(NOISE);

		this->camera = camera;

		cameraOffset = camera->GetPosition() - *m_turret.GetTransform()->GetPosition();

		turretOffset = *m_turret.GetTransform()->GetPosition() - *m_body.GetTransform()->GetPosition();

		muzzleFlashOffset = m_muzzleFlash.GetTransform()->GetPosition()->y - m_turret.GetTransform()->GetPosition()->y;

		this->isPlayer = isPlayer;

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
			Collider::CollisionSide collision = *tankCollider->GetCollider()->GetCollisionSide();
			if (collision != Collider::CollisionSide::FrontRight && collision != Collider::CollisionSide::FrontLeft)
			{
				currentSpeed += acceleration * deltaTime;
				if (currentSpeed > maxForwardSpeed)
					currentSpeed = maxForwardSpeed;
			}
		}
		else if (movingBackward)
		{
			Collider::CollisionSide collision = *tankCollider->GetCollider()->GetCollisionSide();
			if (collision != Collider::CollisionSide::BackRight && collision != Collider::CollisionSide::BackLeft)
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

			if (isPlayer)
			{
				camera->move(forward * currentSpeed * deltaTime);
				m_barrel.GetTransform()->move(forward * currentSpeed * deltaTime);
			}
			
			
		
		}

		movingForward = false;
		movingBackward = false;


		muzzleFlashData.lifetime -= deltaTime;
		currentReloadTime -= deltaTime;

		if (currentReloadTime < 0)
		{
			canShoot = true;
		}
	}

    void SetPosition(glm::vec3 position)
    {
		m_body.GetTransform()->SetPosition(position);
		m_turret.GetTransform()->SetPosition(position + turretOffset);
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
	GameObject* GetBarrel() { return &m_barrel; }

    void MoveForward()
    {
		if (!alive)
		{
			movingForward = false;
			return;
		}
		movingForward = true;
	}
	

    void MoveBackwards()
    {
		if (!alive)
		{
			movingBackward = false;
			return;
		}
		movingBackward = true;
    }

	void RotateBodyLeft(float deltaTime, bool checkMovebackwards, glm::vec3& tankColliderOffset)
	{
		if (!alive)
		{
			return;
		}
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

		if (isPlayer)
		{
			glm::mat4 camRot = glm::rotate(m_turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
			glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
			camera->SetPosition(*m_turret.GetTransform()->GetPosition() + camOffset);
		}


	}



	//like RotateBodyLeft but with opposite rotation
    void RotateBodyRight(float deltaTime, bool checkMovebackwards, glm::vec3& tankColliderOffset)
    {
		if (!alive)
		{
			return;
		}
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

		if (isPlayer)
		{
			glm::mat4 camRot = glm::rotate(m_turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
			glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
			camera->SetPosition(*m_turret.GetTransform()->GetPosition() + camOffset);
		}
		
    }


	void UpdateTurretAim(float deltaTime, float targetAngle, bool isZooming)
	{
		if (!alive) return;

		Transform* turretTransform = m_turret.GetTransform();

		//update camera to where player is aiming
		camera->SetRotation(glm::vec3(0.0f, targetAngle, 0.0f));

		//update camera position
		glm::mat4 rotMat = glm::rotate(targetAngle, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);

		//turret lag behind
		float currentTurretRot = turretTransform->GetRotation()->y;
		float angleDiff = targetAngle - currentTurretRot;

		//shortest path
		while (angleDiff < -glm::pi<float>()) angleDiff += glm::two_pi<float>();
		while (angleDiff > glm::pi<float>()) angleDiff -= glm::two_pi<float>();


		float speed = isZooming ? turretRotSpeed / 2.0f : turretRotSpeed;
		float maxRotThisFrame = speed * deltaTime;

		if (std::abs(angleDiff) > 0.001f)
		{
			//clamp
			float actualMove = glm::clamp(angleDiff, -maxRotThisFrame, maxRotThisFrame);
			turretTransform->rotate(glm::vec3(0.0f, actualMove, 0.0f));
		}

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());
		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));


		//rotate barrel
		m_barrel.GetTransform()->SetRotation(*turretTransform->GetRotation());
		glm::mat4 turretRotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedBarrelOffset = glm::vec3(turretRotMat * glm::vec4(barrelOffset, 1.0f));
		m_barrel.GetTransform()->SetPosition(*turretTransform->GetPosition() + rotatedBarrelOffset);

	}



	void RotateTurretLeftAI(float deltaTime, bool isZooming)
	{
		if (!alive)
		{
			return;
		}
		Transform* turretTransform = m_turret.GetTransform();

		float rotSpeed = isZooming ? turretRotSpeed / 2 : turretRotSpeed;

		turretTransform->rotate(glm::vec3(0.0f, rotSpeed * deltaTime, 0.0f));

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));

		if (isPlayer)
		{
			camera->rotate(rotSpeed * deltaTime, glm::vec3(0, 1, 0));
			glm::mat4 rotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
			glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
			camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);
		}

	}

    void RotateTurretRightAI(float deltaTime, bool isZooming)
    {
		if (!alive)
		{
			return;
		}
		Transform* turretTransform = m_turret.GetTransform();

		float rotSpeed = isZooming ? turretRotSpeed / 2 : turretRotSpeed;

		turretTransform->rotate(glm::vec3(0.0f, -rotSpeed * deltaTime, 0.0f));

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * muzzleFlashOffset));

		if (isPlayer)
		{
			camera->rotate(-rotSpeed * deltaTime, glm::vec3(0, 1, 0));
			glm::mat4 rotMat = glm::rotate(turretTransform->GetRotation()->y, glm::vec3(0, 1, 0));
			glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
			camera->SetPosition(*turretTransform->GetPosition() + rotatedOffset);
		}

    }



	void HandleColliison(bool collided, float collsionForce)
	{
		if (collided && collided != collidedLastFrame)
		{
			//dampening
			currentSpeed = -currentSpeed / collsionForce;
		}	
		collidedLastFrame = collided;
	}

    
	void Shoot()
	{
		if (!alive)
		{
			return;
		}
		if(currentReloadTime > 0.0f)
		{
			return;
		}

		canShoot = false;
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
		m_body.GetShader()->Update(*m_body.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
		m_body.GetTexture()->Bind(0);
		noiseTexture->Bind(1);
		m_body.GetMesh()->draw();

		m_turret.GetShader()->Bind();
		m_turret.GetShader()->Update(*m_turret.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
		m_turret.GetTexture()->Bind(0);
		noiseTexture->Bind(1);
		m_turret.GetMesh()->draw();


		if (isPlayer)
		{
			m_barrel.GetShader()->Bind();
			m_barrel.GetShader()->Update(*m_barrel.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
			m_barrel.GetTexture()->Bind(0);
			noiseTexture->Bind(1);
			m_barrel.GetMesh()->draw();
		}


		if (muzzleFlashData.IsAlive())
		{
			m_muzzleFlash.GetShader()->Bind();
			m_muzzleFlash.GetShader()->Update(*m_muzzleFlash.GetTransform(), *camera, true);
			m_muzzleFlash.GetTexture()->Bind(0);
			m_muzzleFlash.GetMesh()->draw();
		}
	}


	void KillTank()
	{
		alive = false;
	}

	void RespawnTank()
	{
		alive = true;
	}



	const float* GetCurrentSpeed() const { return &currentSpeed; }
	const float* GetBrakeForce() const { return &brakeForce; }
	const bool* GetCollidedLastFrame() const { return &collidedLastFrame; }
	const bool GetIfCanShoot() const { return canShoot && alive; }
	const bool IsAlive() const { return alive; }


private:
	GameObject m_body;
	GameObject m_turret;
	GameObject m_barrel;
	GameObject m_muzzleFlash;
	GameObject* tankCollider;

	Texture* noiseTexture;

	MuzzleFlash muzzleFlashData;

	bool collidedLastFrame = false;

	Camera* camera;

	float currentSpeed = 0.0f;

	const float acceleration = 1.f;
	const float deceleration = 2.5f;

	const float maxForwardSpeed = 7.0f;
	const float maxBackwardSpeed = 2.5f;

	bool movingForward = false;
	bool movingBackward = false;

	const float turnSpeedPenalty = 0.8f;
	const float brakeForce = 7.f;


	const float bodyRotSpeed = 1.0f;
	const float turretRotSpeed = 1.0f;

	const float maxReloadTime = 6.0f;
	float currentReloadTime = 0.0f;


	bool canShoot = true;

	bool alive = true;

	bool isPlayer = false;

	glm::vec3 cameraOffset = glm::vec3();
	glm::vec3 turretOffset = glm::vec3();
	glm::vec3 barrelOffset = glm::vec3();
	float muzzleFlashOffset = 0.0;

	
};
