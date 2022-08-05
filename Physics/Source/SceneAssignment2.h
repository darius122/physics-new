#ifndef SCENE_ASSIGNMENT2_H
#define SCENE_ASSIGNMENT2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAssignment2 : public SceneBase
{
public:
	SceneAssignment2();
	~SceneAssignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	void ReturnGO(GameObject *go);

	bool CheckCollision(GameObject* go1, GameObject* go2);
	bool CheckCollision2(GameObject* go1, GameObject* go2);
	void CollisionResponse(GameObject* go1, GameObject* go2);

	void MakeThinWall(float width, float height, const Vector3& normal, const Vector3& pos, int id);
	void MakeThickWall(float width, float height, const Vector3& normal, const Vector3& pos, int id, int hp, Vector3 color);
	void spawnPlayerBall();
	void spawnPowerup();

protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	GameObject *m_playerBase;
	GameObject *m_playerL;
	GameObject *m_playerR;
	
	int m_objectCount;
	int playerBoundary;
	int noOfBricks;
	float movePlatform;
	float playerWidth;
	float playerHeight;
	int playerLives;
	int gameState;
	int specialBalls;
	float rotateCannon;
	bool rotateCannonLeft;
	bool rotateCannonRight;
	double elapsedTime;
	double enemyBallSpawnTime;
	double slowPlatformTimer;
	double extendPlatformTimer;
	double freezeCannonTimer;

	bool lostALife;
	bool brickDestroyed;
	bool slowPlatform;
	bool powerupChance;
	bool freezeCannon;
	bool extendPlatform;
	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};
static Vector3 rotateVector(const Vector3& vec, float radian);
#endif