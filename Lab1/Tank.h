#pragma once

#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include <glm/gtc/quaternion.hpp>

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
		noiseTexture = nullptr;
	}


	Tank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* camera, bool isPlayer)
	{
		RespawnTank();
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

			muzzleFlashOffset = 0.6;

			m_muzzleFlash.GetTransform()->SetPosition(glm::vec3(0.0, m_barrel.GetTransform()->GetPosition()->y + muzzleFlashOffset, -0.1));


			//crosshair
			m_crosshair.GetTransform()->SetPosition(glm::vec3(0.0, 0.0, 0.0));
			m_crosshair.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
			m_crosshair.GetTransform()->SetScale(glm::vec3(1.0, 1.0, 1.0));
			m_crosshair.SetShader(*shaderManager.GetShader(UIELEMENT));
			m_crosshair.SetTexture(*textureManager.GetTexture(NONE));
			m_crosshair.SetMesh(*meshManager.GetMesh(CROSSHAIR_M));
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

			muzzleFlashOffset = 0.3;

			m_muzzleFlash.GetTransform()->SetPosition(glm::vec3(0.0, m_turret.GetTransform()->GetPosition()->y + muzzleFlashOffset, -0.1));
		}
		

		m_muzzleFlash.GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
		m_muzzleFlash.GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		m_muzzleFlash.SetShader(*shaderManager.GetShader(REMOVE_BACKGRROUND));
		m_muzzleFlash.SetTexture(*textureManager.GetTexture(MUZZLEFLASH));
		m_muzzleFlash.SetMesh(*meshManager.GetMesh(QUAD_M));


		
		noiseTexture = textureManager.GetTexture(NOISE);

		this->camera = camera;

		cameraOffset = camera->GetPosition() - *m_turret.GetTransform()->GetPosition();

		turretOffset = *m_turret.GetTransform()->GetPosition() - *m_body.GetTransform()->GetPosition();

		this->isPlayer = isPlayer;

		tankCollider = nullptr;
	}



	void Update(float deltaTime)
	{
		//check for a change in direction and apply braking if true
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
		//if not braking and we are moving forward check for the last collisions side of the collider to make sure its not the front
		else if (movingForward)
		{
			Collider::CollisionSide collision = *tankCollider->GetCollider()->GetCollisionSide();
			if (collision != Collider::CollisionSide::FrontRight && collision != Collider::CollisionSide::FrontLeft)
			{
				//if not colliding accelerate
				currentSpeed += acceleration * deltaTime;
				if (currentSpeed > maxForwardSpeed)
					currentSpeed = maxForwardSpeed;
			}
		}
		//same as forward but backward
		else if (movingBackward)
		{
			Collider::CollisionSide collision = *tankCollider->GetCollider()->GetCollisionSide();
			if (collision != Collider::CollisionSide::BackRight && collision != Collider::CollisionSide::BackLeft)
			{
				//decelerate
				currentSpeed -= acceleration * deltaTime;
				if (currentSpeed < -maxBackwardSpeed)
					currentSpeed = -maxBackwardSpeed;
			}
			
		}
		else
		{
			//if not moving but still have speed slow down
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


		//if we are moving update everythings transform position to be inline with the body
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


		//reset for next frame
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


	//rotate the body of the tank left
	void RotateBodyLeft(float deltaTime, bool checkMovebackwards, glm::vec3& tankColliderOffset)
	{
		if (!alive)
		{
			return;
		}

		//cant rotate this way if these sides are colliding
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

		//update turret position
		glm::vec3 bodyPos = *m_body.GetTransform()->GetPosition();
		glm::mat4 bodyRot = glm::rotate(m_body.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(bodyRot * glm::vec4(turretOffset, 0.0f));
		m_turret.GetTransform()->SetPosition(bodyPos + rotatedOffset);

		//update collider position and rotation
		glm::vec3 rotatedColliderOffset = glm::vec3(bodyRot * glm::vec4(tankColliderOffset, 0.0f));
		tankCollider->GetTransform()->SetPosition(bodyPos + rotatedColliderOffset);
		tankCollider->GetCollider()->UpdateCollider(*tankCollider->GetTransform()->GetPosition(), *tankCollider->GetTransform()->GetRotation());

		//move camera if player
		if (isPlayer)
		{
			glm::mat4 camRot = glm::rotate(m_turret.GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
			glm::vec3 camOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
			camera->SetPosition(*m_turret.GetTransform()->GetPosition() + camOffset);
		}


	}



	//like RotateBodyLeft above but with opposite rotation
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



	//handles the yaw and pitch of the turret and barrel
	void UpdateTurretAim(float deltaTime, float targetHorizontalAngle, float& targetPitch, bool isZooming, float distanceToTarget) 
	{
		if (!alive) return;


		Transform* turretTransform = m_turret.GetTransform();

		//clamp to elevation adnd depression
		targetPitch = glm::clamp(targetPitch, glm::radians(depression), glm::radians(elevation));

		//yaw system to calculate the final yaw of the turret
		float currentTurretRotY = turretTransform->GetRotation()->y;
		float yawDiff = targetHorizontalAngle - currentTurretRotY;
		while (yawDiff < -glm::pi<float>()) yawDiff += glm::two_pi<float>();
		while (yawDiff > glm::pi<float>()) yawDiff -= glm::two_pi<float>();

		//slow down if zoomed
		float currentRotSpeed = isZooming ? turretRotSpeed / 2.0f : turretRotSpeed;
		float maxYawThisFrame = currentRotSpeed * deltaTime;
		float actualYawMove = 0.0f;

		//if the yaw is substantial 
		if (std::abs(yawDiff) > 0.001f)
		{
			actualYawMove = glm::clamp(yawDiff, -maxYawThisFrame, maxYawThisFrame);
		}

		//set yaw
		float finalTurretYaw = currentTurretRotY + actualYawMove;
		turretTransform->SetRotation(glm::vec3(0.0f, finalTurretYaw, 0.0f));

		//barrel system
		m_barrel.GetTransform()->SetRotation(glm::vec3(targetPitch, finalTurretYaw, 0.0f));

		//update positon of barrel
		glm::mat4 turretPosMat = glm::rotate(glm::mat4(1.0f), finalTurretYaw, glm::vec3(0, 1, 0));
		glm::vec3 rotatedBarrelOffset = glm::vec3(turretPosMat * glm::vec4(barrelOffset, 1.0f));
		m_barrel.GetTransform()->SetPosition(*turretTransform->GetPosition() + rotatedBarrelOffset);

		//update camera to be the right rotation and position
		camera->SetRotation(glm::vec3(targetPitch, targetHorizontalAngle, 0.0f));
		glm::mat4 camRot = glm::mat4(1.0f);
		camRot = glm::rotate(camRot, targetHorizontalAngle, glm::vec3(0, 1, 0));
		camRot = glm::rotate(camRot, targetPitch, glm::vec3(1, 0, 0));
		glm::vec3 rotatedCamOffset = glm::vec3(camRot * glm::vec4(cameraOffset, 0.0f));
		camera->SetPosition(*turretTransform->GetPosition() + rotatedCamOffset);

		//update muzzleflash position and rotation
		m_muzzleFlash.GetTransform()->SetRotation(*m_barrel.GetTransform()->GetRotation());
		glm::vec4 localOffset = glm::vec4(0.0f, m_barrel.GetTransform()->GetPosition()->y + muzzleFlashOffset, 2.65f, 1.0f);
		glm::mat4 barrelMatrix = m_barrel.GetTransform()->GetModel();
		glm::vec3 barrelTip = glm::vec3(barrelMatrix * localOffset);
		m_muzzleFlash.GetTransform()->SetPosition(barrelTip);


		//update crossahair, its basically like the muzzle flash where 
		//its stuck to the barrel and is just moved back and forward depending 
		//on the distance to target but obviously in reference to the camera 
		glm::vec3 barrelForward = m_barrel.GetTransform()->GetForward();
		glm::vec3 aimPoint = barrelTip + barrelForward * (distanceToTarget);

		glm::mat4 viewProj = camera->GetProjection() * camera->GetView();
		glm::vec4 clipPos = viewProj * glm::vec4(aimPoint, 1.0f);
		glm::vec3 ndcPos = glm::vec3(clipPos) / clipPos.w;
		ndcPos.z = -0.99f; // force to always be in front in depth buffer
		m_crosshair.GetTransform()->SetPosition(ndcPos);
		m_crosshair.GetTransform()->SetPosition(ndcPos);
	}


	//for the ai to rotate left
	void RotateTurretLeftAI(float deltaTime)
	{
		if (!alive)
		{
			return;
		}


		//rotate turret and muzzle flash
		Transform* turretTransform = m_turret.GetTransform();

		turretTransform->rotate(glm::vec3(0.0f, turretRotSpeed * deltaTime, 0.0f));

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * (m_barrel.GetTransform()->GetPosition()->y + muzzleFlashOffset)));
	}


	//same as above but opposite direction
    void RotateTurretRightAI(float deltaTime)
    {
		if (!alive)
		{
			return;
		}
		Transform* turretTransform = m_turret.GetTransform();

		turretTransform->rotate(glm::vec3(0.0f, -turretRotSpeed * deltaTime, 0.0f));

		m_muzzleFlash.GetTransform()->SetRotation(*turretTransform->GetRotation());

		m_muzzleFlash.GetTransform()->SetPosition(*turretTransform->GetPosition()
			+ (turretTransform->GetForward() * 1.4f)
			+ (turretTransform->GetUp() * (m_barrel.GetTransform()->GetPosition()->y + muzzleFlashOffset)));

    }



	//if we have collided and its a new collsion, then we dampen the current speed
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

		//draw body
		m_body.GetShader()->Bind();
		m_body.GetShader()->Update(*m_body.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
		m_body.GetTexture()->Bind(0);
		noiseTexture->Bind(1);
		m_body.GetMesh()->draw();

		//draw turret
		m_turret.GetShader()->Bind();
		m_turret.GetShader()->Update(*m_turret.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
		m_turret.GetTexture()->Bind(0);
		noiseTexture->Bind(1);
		m_turret.GetMesh()->draw();


		if (isPlayer)
		{
			//draw barrel
			m_barrel.GetShader()->Bind();
			m_barrel.GetShader()->Update(*m_barrel.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
			m_barrel.GetTexture()->Bind(0);
			noiseTexture->Bind(1);
			m_barrel.GetMesh()->draw();

			//so the crosshair is always active
			glDisable(GL_DEPTH_TEST);
			m_crosshair.GetShader()->Bind();
			m_crosshair.GetShader()->Update(*m_crosshair.GetTransform(), *camera, true, alive ? 0.0f : 0.95f);
			m_crosshair.GetTexture()->Bind(0);
			m_crosshair.GetMesh()->draw();
			glEnable(GL_DEPTH_TEST);
		}



		//show muzzle if needed
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
	GameObject* GetCrosshair() { return &m_crosshair; }
	const bool GetIfCanShoot() const { return canShoot && alive; }
	const bool IsAlive() const { return alive; }


private:
	GameObject m_body;
	GameObject m_turret;
	GameObject m_barrel;
	GameObject m_muzzleFlash;
	GameObject m_crosshair;
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

	//how high the gun can go
	const float elevation = 20.0f; //degrees
	//how low the gun can go
	const float depression = -8.0f; //degrees

	const float bodyRotSpeed = 1.0f;
	const float turretRotSpeed = 1.0f;

	const float maxReloadTime = 6.0f;
	float currentReloadTime = 0.0f;

	float muzzleFlashOffset = 0.0;

	bool canShoot = true;

	bool alive = true;

	bool isPlayer = false;

	glm::vec3 cameraOffset = glm::vec3();
	glm::vec3 turretOffset = glm::vec3();
	glm::vec3 barrelOffset = glm::vec3();

	
};
