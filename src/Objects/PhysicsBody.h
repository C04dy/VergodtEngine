#pragma once
#include "Node.h"
#include "Collider.h"
#include <box2d/box2d.h>

class PhysicsBody : public Node
{
public:
	enum Layers 
	{
			LAYER1 = 0x00000001,
			LAYER2 = 0x00000002,
			LAYER3 = 0x00000004,
			LAYER4 = 0x00000008,
			LAYER5 = 0x00000016,
			LAYER6 = 0x00000032,
			LAYER7 = 0x00000064,
			LAYER8 = 0x00000128,
			LAYER9 = 0x00000256,
			LAYER10 = 0x00000512,
			LAYER11 = 0x00001024,
			LAYER12 = 0x00002048,
			LAYER13 = 0x00004096,
			LAYER14 = 0x00008192,
			LAYER15 = 0x00016384,
	};
public:
	~PhysicsBody();
	
	void InitPhysicsBody(std::vector<Node*>& Nodes, b2WorldId& PhysicsWorldID, b2BodyType BodyType, std::vector<int>& Layer, float Friction = 0.3f, float Density = 1.0f);

	void SetCollider(Collider* _Collider);

	void DeletePhysicsBody();

	void UpdatePhysicsNode();

	b2BodyId BodyID;
private:
	float m_Friction = 0.0f, m_Density = 0.0f;

	uint64_t m_Layer = Layers::LAYER1;
};
