#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
	Camera()
	{

	}

	void initCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->rot = glm::vec3();
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->right = glm::vec3(1.0f, 0.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);

	}

	void move(const glm::vec3& movement)
	{
		pos += movement;
	}
 
	void rotate(float angle, const glm::vec3& axis)
	{
		glm::mat4 rotation = glm::rotate(angle, axis);
		forward = glm::normalize(glm::vec3(rotation * glm::vec4(forward, 0.0f)));
		right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, forward));

		// Track pitch separately
		if (glm::abs(glm::dot(axis, glm::vec3(1, 0, 0))) > 0.5f ||
			glm::abs(glm::dot(axis, right)) > 0.5f)
			rot.x += angle;
		if (axis.y > 0.5f) rot.y += angle;
	}

    glm::vec3 GetForward() const
    {
        return glm::normalize(forward);
    }

	glm::vec3 GetUp() const 
	{ 
		return glm::normalize(up); 
	}

	glm::vec3 GetRight() const 
	{ 
		return glm::normalize(right); 
	}

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(pos, pos + forward, up);
	}

	inline glm::mat4 GetProjection() const
	{
		return projection;
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	inline glm::vec3 GetPosition() const
	{
		return pos;
	}

	inline glm::vec3 GetRotation() const { return rot; }

	void SetPosition(const glm::vec3& pos) 
	{
		this->pos = pos;
	}

	void SetRotation(const glm::vec3& newRot)
	{
		this->rot = newRot;
		this->rot.x = glm::clamp(rot.x, glm::radians(-89.0f), glm::radians(89.0f));

		forward = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1, 0, 0));

		forward = glm::normalize(glm::vec3(rotY * rotX * glm::vec4(forward, 0.0f)));
		right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, forward));
	}

	void updateProjection(float fov, float aspect, float nearClip, float farClip)
	{
		projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	float GetPitch() const { return m_pitch; }
	void AddPitch(float angle) { m_pitch += angle; }
	void ResetPitch() { m_pitch = 0.0f; }

private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	float m_pitch = 0.0f;

};