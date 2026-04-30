#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Collider.h"


struct Transform
{
public:
    Transform(const glm::vec3& pos = glm::vec3(), 
        const glm::vec3& rot = glm::vec3(),
        const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
    {
        this->m_pos = pos;
        this->m_rot = rot;
        this->m_scale = scale;
    }
	
	const glm::mat4 GetModel() const //runs as compile time
	{
		glm::mat4 posMat = glm::translate(m_pos);
		glm::mat4 scaleMat = glm::scale(m_scale);

		glm::mat4 rotX = glm::rotate(m_rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(m_rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(m_rot.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;

		return posMat * rotMat * scaleMat;
	}

	void move(const glm::vec3& movement) { m_pos += movement; } //move the transform by a certain amount

    void rotate(const glm::vec3& rotation) //rotate the transform by a certain amount
	{
		m_rot += rotation;
	}

	const glm::vec3 GetForward() const 
	{
		glm::mat4 rotation = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		return glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
	}

	const glm::vec3 GetUp() const 
	{
		glm::mat4 rotation = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		return glm::vec3(rotation * glm::vec4(0, 1, 0, 0));
	}

	const glm::vec3 GetRight() const 
	{
		glm::mat4 rotation = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		return glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
	}

	const glm::vec3* GetPosition() const { return &m_pos; } //getters
	const glm::vec3* GetRotation() const { return &m_rot; }
	const glm::vec3* GetScale() const { return &m_scale; }

	void SetPosition(const glm::vec3& pos) { this->m_pos = pos; } // setters
	void SetRotation(const glm::vec3& rot) { this->m_rot = rot; }
	void SetScale(const glm::vec3& scale) { this->m_scale = scale; }



	Transform& operator=(const Transform& other)
	{
		if (this == &other) return *this;

		this->m_pos = other.m_pos;
		this->m_rot = other.m_rot;
		this->m_scale = other.m_scale;

		return *this;
	}



protected:
private:

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};
