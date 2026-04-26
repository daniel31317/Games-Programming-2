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
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);

	}

	void move(const glm::vec3& movement)
	{
		pos += movement;
	}
 
	void rotate(float angle, const glm::vec3& axis)
	{
		glm::mat4 rotation = glm::rotate(angle, axis);
		forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
		up = glm::vec3(rotation * glm::vec4(up, 0.0f));
	}

    glm::vec3 GetForward() const
    {
        return glm::normalize(forward);
    }



	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	inline glm::vec3 GetPosition() const
	{
		return pos;
	}

	void SetPosition(const glm::vec3& pos) 
	{
		this->pos = pos;
	}



private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;

};