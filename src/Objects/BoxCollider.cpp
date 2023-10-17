#include "BoxCollider.h"

void BoxCollider::InitBoxCollider()
{
	m_collisionshapebox.SetAsBox((ColliderSize.x / 2) * Transform.Size.x, (ColliderSize.y / 2) * Transform.Size.y);
	m_fixturedef.shape = &m_collisionshapebox;
	m_fixturedef.density = 1.0f;
	m_fixturedef.friction = 0.3f;
}