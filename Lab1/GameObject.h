#pragma once

#include "Transform.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Collider.h"
#include <memory>

struct GameObject
{
public: 
	
	GameObject(const Transform& transform)
	{
		this->m_transform = Transform(transform);
		this->m_collider.UpdateCollider(*transform.GetPosition(), *transform.GetRotation(), *transform.GetScale());
	}

	GameObject() : m_transform(Transform()) {}

	GameObject::~GameObject() = default;




	Transform* GetTransform() { return &m_transform; }
	Collider* GetCollider() { return &m_collider; }
	Shader* GetShader() { return &m_shader; }
	Texture* GetTexture() { return &m_texture; }
	Mesh* GetMesh() { return &m_mesh; }

	void SetTransform(const Transform& transform) { this->m_transform = transform; }
	void SetShader(const Shader& shader) { this->m_shader = shader; }
	void SetTexture(const Texture& texture) { this->m_texture = texture; }
	void SetMesh(const Mesh& mesh) { this->m_mesh = mesh; }




	GameObject& operator=(const GameObject& other)
	{
		if (this == &other) return *this;

		m_transform = Transform(other.m_transform);

		return *this;
        
	}

private:

	Transform m_transform;
	Shader m_shader;
	Texture m_texture;
	Mesh m_mesh;
	Collider m_collider;
};
