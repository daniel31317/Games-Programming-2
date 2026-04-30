#pragma once
#include "Tank.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <array>
#include <random>
#include <cmath>

struct EnemyTank
{

public:

	struct MapIndex 
	{
		int row;
		int col;
	
	};
	EnemyTank() {}

	EnemyTank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* mainCamera, GameObject* playerTank) : m_tank(shaderManager, textureManager, meshManager, mainCamera, false)
	{

		GenerateStartingIndices(currentIndex, nextIndex, 9, 8);

		m_tank.SetPosition(glm::vec3(movementGrid[currentIndex.row][currentIndex.col].x, -0.5, -movementGrid[currentIndex.row][currentIndex.col].y));

		m_positionRef = m_tank.GetBody()->GetTransform()->GetPosition();
		m_currentSpeedRef = m_tank.GetCurrentSpeed();
		m_brakeForceRef = m_tank.GetBrakeForce();
		m_tankBodyRef = m_tank.GetBody();
		m_playerRef = playerTank;
	}


	void Update(float deltaTime)
	{
		
		HandleDriving();

		HandleBodyRotation(deltaTime);

		HandleTurretRotation(deltaTime);

		m_tank.Update(deltaTime);
	}




	void HandleDriving()
	{
		float stoppingDistance = (*m_currentSpeedRef * *m_currentSpeedRef) / (2.0f * *m_brakeForceRef);
		float distanceToTarget = glm::distance(*m_positionRef, nextPoint);

		//braking 
		if (distanceToTarget <= stoppingDistance || stopMovingForward)
		{
			stopMovingForward = true; //stopmoving forward aimlessly

			//brake
			if (*m_currentSpeedRef > 0.1f)
			{
				m_tank.MoveBackwards();
			}
			//correct overshoot
			else if (*m_currentSpeedRef < -0.1f)
			{
				m_tank.MoveForward();
			}
			//we have arrived
			else
			{
				stopMovingForward = false;

				//get new index
				MapIndex tempOldCurrent = currentIndex;
				currentIndex = nextIndex;

				nextIndex = GetNextRandomPoint(currentIndex, tempOldCurrent, 9, 8);

				//update position
				glm::vec2 gridPos = movementGrid[nextIndex.row][nextIndex.col];
				nextPoint = glm::vec3(gridPos.x, m_positionRef->y, -gridPos.y);

				//get new rotation
				glm::vec3 direction = nextPoint - *m_positionRef;
				targetAngleY = std::atan2(direction.x, direction.z);
			}
		}
		//driving forward forever
		else
		{
			if (!*m_tank.GetCollidedLastFrame())
			{
				m_tank.MoveForward();
			}
		}

	}

	void HandleBodyRotation(float deltaTime)
	{
		float currentAngleYBody = m_tankBodyRef->GetTransform()->GetRotation()->y;

		//normalize between -180 to 180 so it dont do cool spin
		float angleDiff = targetAngleY - currentAngleYBody;
		while (angleDiff < -glm::pi<float>()) angleDiff += glm::two_pi<float>();
		while (angleDiff > glm::pi<float>()) angleDiff -= glm::two_pi<float>();

		//only rotate if difference is small
		if (std::abs(angleDiff) > 0.0045f)
		{
			if (angleDiff > 0)
			{
				m_tank.RotateBodyLeft(deltaTime, true, *tankColliderOffset);
				m_tank.RotateTurretLeft(deltaTime);
			}
			else
			{
				m_tank.RotateBodyRight(deltaTime, true, *tankColliderOffset);
				m_tank.RotateTurretRight(deltaTime);
			}
		}

	}

	void HandleTurretRotation(float deltaTime)
	{
		float currentAngleYTurret = m_tank.GetTurret()->GetTransform()->GetRotation()->y;

		//normalize between -180 to 180 so it dont do cool spin
		float angleDiff = targetAngleY - currentAngleYTurret;
		while (angleDiff < -glm::pi<float>()) angleDiff += glm::two_pi<float>();
		while (angleDiff > glm::pi<float>()) angleDiff -= glm::two_pi<float>();

		//only rotate if difference is small
		if (std::abs(angleDiff) > 0.0045f)
		{
			if (angleDiff > 0)
			{
				m_tank.RotateTurretLeft(deltaTime);
			}
			else
			{
				m_tank.RotateTurretRight(deltaTime);
			}
		}
	}

	MapIndex GetNextRandomPoint(MapIndex current, MapIndex previous, int maxRows, int maxCols) {
		std::vector<MapIndex> validPoints;

		//determine direction of previous travel
		bool movingHorizontally = (current.row == previous.row);
		bool movingVertically = (current.col == previous.col);

		
		//cjeck points in the same row
		for (int c = 0; c < maxCols; ++c) {
			if (c == current.col) continue;

			if (movingHorizontally) {

				//ignore previous moves to the left if we just moved right
				if (current.col > previous.col && c < current.col) continue;
				//vice versa
				if (current.col < previous.col && c > current.col) continue;
			}

			validPoints.push_back({ current.row, c });
		}

		
		//check points in same column
		for (int r = 0; r < maxRows; ++r) {
			if (r == current.row) continue;

			if (movingVertically) {
				//same as before but with up and down
				if (current.row > previous.row && r < current.row) continue;

				if (current.row < previous.row && r > current.row) continue;
			}

			validPoints.push_back({ r, current.col });
		}

		//if somehow no tiles are found return current tile
		if (validPoints.empty()) return current;

		//pick random valid point
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<> dis(0, validPoints.size() - 1);

		return validPoints[dis(gen)];
	}


	void GenerateStartingIndices(MapIndex& start, MapIndex& previous, int maxRows, int maxCols) {
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<> rowDist(0, maxRows - 1);
		std::uniform_int_distribution<> colDist(0, maxCols - 1);

		
		start.row = rowDist(gen);
		start.col = colDist(gen);

		previous = start;
	}



	Tank* GetTank() { return &m_tank; }

	void SetTankColliderOffset(glm::vec3* tankColliderOffset) { this->tankColliderOffset = tankColliderOffset; }



	

private:

	Tank m_tank;
	GameObject* m_tankBodyRef = nullptr;
	GameObject* m_playerRef = nullptr;
	const glm::vec3* m_positionRef = nullptr;
	const float* m_currentSpeedRef = nullptr;
	const float* m_brakeForceRef = nullptr;

	float targetAngleY = 0.0f;

	MapIndex currentIndex;

	MapIndex nextIndex;

	bool stopMovingForward = true;
	bool arrived = false;

	glm::vec3* tankColliderOffset;

	glm::vec3 nextPoint;

	//really cursed layout but hey
	//bottom left (x,z)	(0,0)																																																//top left				
	const std::array<std::array<glm::vec2, 8>, 9> movementGrid = { {
	{ glm::vec2(99.5f, 101.5f),  glm::vec2(99.5f, 78.5f),  glm::vec2(99.5f, 55.5f),  glm::vec2(99.5f, 33.0f),  glm::vec2(99.5f, 10.0f),  glm::vec2(99.5f, -13.5f),  glm::vec2(99.5f, -35.5f),  glm::vec2(99.5f, -57.0f)  },
	{ glm::vec2(75.0f, 101.5f),  glm::vec2(75.0f, 78.5f),  glm::vec2(75.0f, 55.5f),  glm::vec2(75.0f, 33.0f),  glm::vec2(75.0f, 10.0f),  glm::vec2(75.0f, -13.5f),  glm::vec2(75.0f, -35.5f),  glm::vec2(75.0f, -57.0f)  },
	{ glm::vec2(50.0f, 101.5f),  glm::vec2(50.0f, 78.5f),  glm::vec2(50.0f, 55.5f),  glm::vec2(50.0f, 33.0f),  glm::vec2(50.0f, 10.0f),  glm::vec2(50.0f, -13.5f),  glm::vec2(50.0f, -35.5f),  glm::vec2(50.0f, -57.0f)  },
	{ glm::vec2(25.0f, 101.5f),  glm::vec2(25.0f, 78.5f),  glm::vec2(25.0f, 55.5f),  glm::vec2(25.0f, 33.0f),  glm::vec2(25.0f, 10.0f),  glm::vec2(25.0f, -13.5f),  glm::vec2(25.0f, -35.5f),  glm::vec2(25.0f, -57.0f)  },
	{ glm::vec2(-0.5f, 101.5f),  glm::vec2(-0.5f, 78.5f),  glm::vec2(-0.5f, 55.5f),  glm::vec2(-0.5f, 33.0f),  glm::vec2(-0.5f, 10.0f),  glm::vec2(-0.5f, -13.5f),  glm::vec2(-0.5f, -35.5f),  glm::vec2(-0.5f, -57.0f)  },
	{ glm::vec2(-25.0f, 101.5f), glm::vec2(-25.0f, 78.5f), glm::vec2(-25.0f, 55.5f), glm::vec2(-25.0f, 33.0f), glm::vec2(-25.0f, 10.0f), glm::vec2(-25.0f, -13.5f), glm::vec2(-25.0f, -35.5f), glm::vec2(-25.0f, -57.0f) },
	{ glm::vec2(-49.0f, 101.5f), glm::vec2(-49.0f, 78.5f), glm::vec2(-49.0f, 55.5f), glm::vec2(-49.0f, 33.0f), glm::vec2(-49.0f, 10.0f), glm::vec2(-49.0f, -13.5f), glm::vec2(-49.0f, -35.5f), glm::vec2(-49.0f, -57.0f) },
	{ glm::vec2(-76.0f, 101.5f), glm::vec2(-76.0f, 78.5f), glm::vec2(-76.0f, 55.5f), glm::vec2(-76.0f, 33.0f), glm::vec2(-76.0f, 10.0f), glm::vec2(-76.0f, -13.5f), glm::vec2(-76.0f, -35.5f), glm::vec2(-76.0f, -57.0f) },
	{ glm::vec2(-99.0f, 101.5f), glm::vec2(-99.0f, 78.5f), glm::vec2(-99.0f, 55.5f), glm::vec2(-99.0f, 33.0f), glm::vec2(-99.0f, 10.0f), glm::vec2(-99.0f, -13.5f), glm::vec2(-99.0f, -35.5f), glm::vec2(-99.0f, -57.0f) }
} };
	//bottom right																																																	//top right

};