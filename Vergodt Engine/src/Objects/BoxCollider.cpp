#include "BoxCollider.h"

void BoxCollider::InitBoxCollider()
{
	m_collisionshapebox.SetAsBox(ColliderSize.getX() * Transform.Size.getX(), ColliderSize.getY() * Transform.Size.getY());
	m_fixturedef.shape = &m_collisionshapebox;
	m_fixturedef.density = 1.0f;
	m_fixturedef.friction = 0.3f;
}