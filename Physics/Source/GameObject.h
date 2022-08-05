#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_ASTEROID, //asteroid
		GO_SHIP, //player ship
		GO_BULLET, //player bullet
		GO_BLACKHOLE, //Black Hole
		GO_WHITEHOLE, //White Hole
		GO_WALL,
		GO_CANNON,
		GO_PILLAR,
		GO_ENEMYBALL,
		GO_POWERUPFREEZE,
		GO_POWERUPEXTEND,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	float angle;
	bool active;
	float mass;
	int damage;
	int hp;
	bool visible;
	int id;
	Vector3 color;

	//Rotation variables
	Vector3 direction;
	float momentOfInertia;
	float angularVelocity;

	GameObject* otherWall;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif