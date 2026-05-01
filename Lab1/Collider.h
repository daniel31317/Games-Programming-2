#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iterator>
#include <array>

//collision method from https://dev.to/pratyush_mohanty_6b8f2749/the-math-behind-bounding-box-collision-detection-aabb-vs-obbseparate-axis-theorem-1gdn 
//as previous aabb was not working

struct Collider
{
	public:
		enum class CollisionSide
		{
			None,
			FrontLeft,
			FrontRight,
			BackLeft,
			BackRight
		};

		struct SideDir { CollisionSide side; glm::vec2 direction; };
		

		

		Collider() : position(0.0f), rotation(0.0f), scale(1.0f)  
		{
			lastCollisionSide = CollisionSide::None;
		}


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
			//how far is box into axis
			int x = 1;
			return box.halfExtents.x * std::abs(glm::dot(box.axes[0], axis)) +
				box.halfExtents.y * std::abs(glm::dot(box.axes[1], axis)) +
				box.halfExtents.z * std::abs(glm::dot(box.axes[2], axis));
		}



		bool CheckRayHit(glm::vec3 rayOrigin, glm::vec3 rayDir, const Collider& target, float& distance) 
		{
			//start distance
			float minD = 0.0f;     
			//max distnace
			float maxD = 100000.0f;    

			glm::vec3 worldPos = target.GetPosition(); 
			glm::vec3 deltaPos = worldPos - rayOrigin;

			//slabs algorithm for obb
			for (int i = 0; i < 3; i++) 
			{
				glm::vec3 axis = target.GetAxes()[i];
				float e = glm::dot(axis, deltaPos);
				float f = glm::dot(rayDir, axis);

				//if the ray is not parallel to the planes
				if (std::abs(f) > 0.0001f) 
				{
					//get extent
					float t1 = (e + target.GetHalfExtents()->x) / f; 

					//objects are indexed by i so we map extents
					float extent;
					if (i == 0) 
					{
						extent = target.GetHalfExtents()->x;
					}
					else if (i == 1) 
					{
						extent = target.GetHalfExtents()->y;
					}
					else
					{
						extent = target.GetHalfExtents()->z;
					}

					//intersection logic
					float t1_actual = (e + extent) / f;
					float t2_actual = (e - extent) / f;

					if (t1_actual > t2_actual) std::swap(t1_actual, t2_actual);

					if (t1_actual > minD) minD = t1_actual;
					if (t2_actual < maxD) maxD = t2_actual;

					//missed
					if (minD > maxD)
					{
						return false;
					}
					//box beyond the ray
					if (maxD < 0)
					{
						return false;
					}
				}
				else 
				{
					//ray is parrallel so check if the origin is inside the slab
					float extent;

					if (i == 0) 
					{
						extent = target.GetHalfExtents()->x;
					}
					else if (i == 1) 
					{
						extent = target.GetHalfExtents()->y;
					}
					else 
					{
						extent = target.GetHalfExtents()->z;
					}



					if (-e - extent > 0 || -e + extent < 0)
					{
						return false;
					}	
				}
			}

			distance = minD;
			return true;
		}



		bool IsCollidingWith(const Collider* b)
		{
			Collider a = *this;
			glm::vec3 delta = b->position - a.position;

			//15 axes
			glm::vec3 testAxes[15];

			//3 from a
			testAxes[0] = a.axes[0];
			testAxes[1] = a.axes[1];
			testAxes[2] = a.axes[2];

			//3 from b
			testAxes[3] = b->axes[0];
			testAxes[4] = b->axes[1];
			testAxes[5] = b->axes[2];

			//get the cros products
			int idx = 6;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					testAxes[idx++] = glm::cross(a.axes[i], b->GetAxes()[j]);
				}
					
			}
				

			for (int i = 0; i < 15; i++)
			{
				//skip near zero (parallel edges)
				if (glm::length(testAxes[i]) < 0.0001f) continue;

				glm::vec3 axis = glm::normalize(testAxes[i]);

				float projA = ProjectOntoAxis(a, axis);
				float projB = ProjectOntoAxis(*b, axis);
				float dist = std::abs(glm::dot(delta, axis));

				// gap found on axis means no collision
				if (dist > projA + projB)
				{
					lastCollisionSide = CollisionSide::None;
					return false;					
				}
					
			}


			//get the closest point of b collider
			glm::vec3 closestPointOnB = b->position;
			for (int i = 0; i < 3; i++) 
			{
				float dist = glm::dot(a.position - b->position, b->axes[i]);
				dist = glm::clamp(dist, -(*b->GetHalfExtents())[i], (*b->GetHalfExtents())[i]);
				closestPointOnB += b->axes[i] * dist;
			}

			
			//get real collision delta to the closest point on the building
			glm::vec3 actualCollisionDelta = closestPointOnB - a.position;

			//translate to tank space
			float xDot = glm::dot(actualCollisionDelta, axes[0]); 
			float zDot = glm::dot(actualCollisionDelta, axes[2]);			

			//normalize
			glm::vec2 localDir = glm::normalize(glm::vec2(xDot, zDot));

			//find the best corner that we hit with
			float maxDot = -1.0f;

			for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++)
			{
				float dot = glm::dot(localDir, directions[i].direction);
				if (dot > maxDot) 
				{
					maxDot = dot;
					lastCollisionSide = directions[i].side;
				}
			}
			return true;
		}


		const glm::vec3* GetAxes() const { return axes; }
		const glm::vec3* GetHalfExtents() const { return &halfExtents; }
		const glm::vec3& GetPosition() const { return position; }

		const CollisionSide* GetCollisionSide() { return &lastCollisionSide; }

		void SetScale(const glm::vec3& scale) { this->scale = scale; halfExtents = scale * 0.5f; }


	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 halfExtents;
		glm::vec3 axes[3];
		CollisionSide lastCollisionSide;

		std::array<SideDir, 4> directions = 
		{ {
			{ Collider::CollisionSide::FrontLeft,  glm::vec2(0.7071f,  0.7071f) },
			{ Collider::CollisionSide::FrontRight, glm::vec2(-0.7071f,  0.7071f) },
			{ Collider::CollisionSide::BackLeft,   glm::vec2(0.7071f, -0.7071f) },
			{ Collider::CollisionSide::BackRight,  glm::vec2(-0.7071f, -0.7071f) }
		} };

		
};
