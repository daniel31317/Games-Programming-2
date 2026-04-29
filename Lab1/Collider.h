#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//collision method from https://dev.to/pratyush_mohanty_6b8f2749/the-math-behind-bounding-box-collision-detection-aabb-vs-obbseparate-axis-theorem-1gdn 
//as previous aabb was not working

struct Collider
{
	public:
		Collider() : position(0.0f), rotation(0.0f), scale(1.0f) {}


		void UpdateCollider(glm::vec3 position, glm::vec3 rotation)
		{
			this->position = position;

			glm::mat4 rotMatrix = glm::mat4(1.0f);
			rotMatrix = glm::rotate(rotMatrix, rotation.x, glm::vec3(1, 0, 0));
			rotMatrix = glm::rotate(rotMatrix, rotation.y, glm::vec3(0, 1, 0));
			rotMatrix = glm::rotate(rotMatrix, rotation.z, glm::vec3(0, 0, 1));

			axes[0] = glm::vec3(rotMatrix[0]); // right
			axes[1] = glm::vec3(rotMatrix[1]); // up
			axes[2] = glm::vec3(rotMatrix[2]); // forward
		}


		float ProjectOntoAxis(const Collider& box, const glm::vec3& axis)
		{
			// How far this box extends along the given axis
			int x = 1;
			return box.halfExtents.x * std::abs(glm::dot(box.axes[0], axis)) +
				box.halfExtents.y * std::abs(glm::dot(box.axes[1], axis)) +
				box.halfExtents.z * std::abs(glm::dot(box.axes[2], axis));
		}



		bool IsCollidingWith(const Collider* b)
		{
			Collider a = *this;
			glm::vec3 delta = b->position - a.position;

			// 15 axes to test
			glm::vec3 testAxes[15];

			// 3 axes from A
			testAxes[0] = a.axes[0];
			testAxes[1] = a.axes[1];
			testAxes[2] = a.axes[2];

			// 3 axes from B
			testAxes[3] = b->axes[0];
			testAxes[4] = b->axes[1];
			testAxes[5] = b->axes[2];

			// 9 cross products
			int idx = 6;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					testAxes[idx++] = glm::cross(a.axes[i], b->GetAxes()[j]);

			for (int i = 0; i < 15; i++)
			{
				// Skip near-zero axes (can happen with parallel edges)
				if (glm::length(testAxes[i]) < 0.0001f) continue;

				glm::vec3 axis = glm::normalize(testAxes[i]);

				float projA = ProjectOntoAxis(a, axis);
				float projB = ProjectOntoAxis(*b, axis);
				float dist = std::abs(glm::dot(delta, axis));

				// Gap found on this axis = no collision
				if (dist > projA + projB)
					return false;
			}

			// No separating axis found = collision
			return true;
		}


		const glm::vec3* GetAxes() const { return axes; }

		void SetScale(const glm::vec3& scale) { this->scale = scale; halfExtents = scale * 0.5f; }


	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 halfExtents;
		glm::vec3 axes[3];


};