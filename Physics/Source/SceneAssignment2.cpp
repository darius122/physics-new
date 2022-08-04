#include "SceneAssignment2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAssignment2::SceneAssignment2()
{
}

SceneAssignment2::~SceneAssignment2()
{
}

void SceneAssignment2::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	movePlatform = 1.0;
	enemyBallSpawnTime = elapsedTime = 0.0;
	gameState = 0;
	rotateCannon = 0;
	rotateCannonLeft = false;
	rotateCannonRight = true;
	slowPlatform = false;
	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;
	bLightEnabled = true;

	m_ghost = new GameObject(GameObject::GO_BALL);

	float angle = Math::QUARTER_PI;
	float wallLength = 30;
	float radius = wallLength * 0.5f / tan(angle * 0.5f);

	/*for (int i = 0; i < 4; ++i) {
		GameObject* go = FetchGO();
		go->type = GameObject::GO_WALL;
		go->scale.Set(2.0f, wallLength + 0.9f, 1.f);
		go->pos = Vector3(radius * cosf(i * angle) + m_worldWidth/2, radius * sinf(i * angle) + m_worldHeight/2, 0.0f);
		go->normal = Vector3(cosf(i * angle), sinf(i * angle), 0.f);
		go->color.Set(1, 0, 0);
		go->vel.SetZero();
	}*/
	/*GameObject* go = FetchGO();
	go->type = GameObject::GO_WALL;
	go->scale.Set(2.0f, wallLength + 0.9f, 1.f);
	go->pos = Vector3(m_worldWidth / 2, m_worldHeight / 2, 0.0f);
	go->normal = Vector3(0, sinf(angle), 0.f);
	go->vel.SetZero();*/

	MakeThinWall(5.0f, 120.0f, Vector3(1, 0, 0), Vector3(m_worldWidth +1.5, m_worldHeight / 2, 0), 100);
	MakeThinWall(5.0f, 120.0f, Vector3(1, 0, 0), Vector3(m_worldWidth -180.1, m_worldHeight / 2, 0), 100);
	//MakeThinWall(5.0f, 180.0f, Vector3(0, 1, 0), Vector3(m_worldWidth/2, m_worldHeight+2, 0));
	GameObject* topBar = FetchGO();
	topBar->type = GameObject::GO_WALL;
	topBar->scale.Set(5.0f, 180.f, 1.0f);
	topBar->pos = Vector3(m_worldWidth / 2, m_worldHeight - 2, 0);
	topBar->normal = Vector3(0, 1, 0);
	topBar->color.Set(0.376, 0.376, 0.376);
	topBar->id = 100;
	topBar->hp = 1000;
	topBar->vel.SetZero();

	/*m_player = new GameObject(GameObject::GO);
	m_player->active = true;
	m_player->scale.Set(5, 1, 1);
	m_player->pos.Set(m_worldWidth / 2, m_worldHeight / 2 - 35);
	m_player->normal = Vector3(1, 0, 0);*/
	float playerWidth = 3;
	float playerHeight = 10;

	m_playerBase = FetchGO();
	m_playerBase->type = GameObject::GO_WALL;
	m_playerBase->scale.Set(playerWidth, playerHeight, 1.0f);
	m_playerBase->pos.Set(m_worldWidth / 2, m_worldHeight / 2 - 35);
	m_playerBase->normal = Vector3(0, 1, 0);
	m_playerBase->color.Set(1, 1, 1);
	m_playerBase->id = 0;
	m_playerBase->vel.SetZero();

	Vector3 tangent(-Vector3(0, 1, 0).y, Vector3(0, 1, 0).x);
	
	m_playerR = FetchGO();
	m_playerR->type = GameObject::GO_PILLAR;
	m_playerR->color.Set(1, 1, 1);
	m_playerR->scale.Set(playerWidth * 0.5f, playerWidth * 0.5f, 1);
	m_playerR->pos = Vector3(m_worldWidth / 2, m_worldHeight / 2 - 35) + playerHeight * 0.5f * tangent;
	m_playerR->id = 0;
	
	m_playerL = FetchGO();
	m_playerL->type = GameObject::GO_PILLAR;
	m_playerL->color.Set(1, 1, 1);
	m_playerL->scale.Set(playerWidth * 0.5f, playerWidth * 0.5f, 1);
	m_playerL->pos = Vector3(m_worldWidth / 2, m_worldHeight / 2 - 35) - playerHeight * 0.5f * tangent;
	m_playerL->id = 0;

	for (int i = 0; i < (m_worldWidth/6); i++) {
		MakeThickWall(5.0f, 10.0f, Vector3(0, 1, 0), Vector3(m_worldWidth / 2 - 83 + (11 * i), m_worldHeight - 13.7, 0), i + 2, 3, Vector3(1, 1, 0));
		MakeThickWall(5.0f, 10.0f, Vector3(0, 1, 0), Vector3(m_worldWidth / 2 - 83 + (11 * i), m_worldHeight - 19.7, 0), i + 18, 2, Vector3(1, 1, 0));
		MakeThickWall(5.0f, 10.0f, Vector3(0, 1, 0), Vector3(m_worldWidth / 2 - 83 + (11 * i), m_worldHeight - 25.7, 0), i + 34, 1, Vector3(1, 1, 0));
		MakeThickWall(5.0f, 10.0f, Vector3(0, 1, 0), Vector3(m_worldWidth / 2 - 83 + (11 * i), m_worldHeight - 31.7, 0), i + 50, 1, Vector3(1, 1, 0));
	}

	GameObject* pball = FetchGO();
	pball->type = GameObject::GO_BALL;
	pball->color.Set(0, 1, 0);
	pball->scale.Set(2, 2, 2);
	pball->pos.Set(m_worldWidth / 2, m_worldHeight / 2);
	pball->vel.Set(0, -40, 0);
	pball->damage = 1;
	pball->mass = 10;

	GameObject* cannon = FetchGO();
	cannon->type = GameObject::GO_CANNON;
	cannon->color.Set(0.501, 0.501, 0.501);
	cannon->scale.Set(10, 30, 5);
	cannon->pos.Set(m_worldWidth / 2, m_worldHeight + 4);
	cannon->normal.Set(1, 0, 0);
}

GameObject* SceneAssignment2::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it) {
		GameObject* go = (GameObject*)*it;
		if (go->active) {
			continue;
		}
		go->active = true;
		go->visible = true;
		go->otherWall = nullptr;
		++m_objectCount;
		return go;
	}
	
	//Exercise 2b: increase object count every time an object is set to active
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; ++i) {
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	m_goList.at(prevSize)->active = true;
	m_goList.at(prevSize)->visible = true;
	m_goList.at(prevSize)->otherWall = nullptr;
	++m_objectCount;

	return m_goList[prevSize];
}

void SceneAssignment2::ReturnGO(GameObject *go)
{
	//Exercise 3: implement ReturnGO()
	if (go->active) {
		go->active = false;
		--m_objectCount;
	}
}

void SceneAssignment2::Update(double dt)
{
	SceneBase::Update(dt);
	if (gameState == 0) {
		if (Application::IsKeyPressed(' ')) {
			gameState = 1;
		}
		return;
	}
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	double x, y, windowWidth, windowHeight;
	Application::GetCursorPos(&x, &y);
	windowWidth = Application::GetWindowWidth();
	windowHeight = Application::GetWindowHeight();
	Vector3 mousePos(x * (m_worldWidth / windowWidth), (windowHeight - y) * (m_worldHeight / windowHeight), 0);
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		m_ghost->active = true;
		m_ghost->pos = mousePos;
		//m_ghost->pos.y = m_worldHeight / 2.0f;
		m_ghost->scale.Set(2, 2, 2);
		m_ghost->mass = 8;
		m_ghost->color.Set(Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1));
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		GameObject* go = FetchGO();
		go->type = GameObject::GO_BALL;
		go->pos = m_ghost->pos;

		go->vel = m_ghost->pos - mousePos;
		//go->vel.y = 0;

		go->scale = m_ghost->scale;
		go->mass = m_ghost->mass;

		go->color = m_ghost->color;
		go->damage = 1;
		go->hp = 99;
		m_ghost->active = false;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		m_ghost->active = true;
		m_ghost->pos = mousePos;
		//m_ghost->pos.y = m_worldHeight / 2.0f;
		m_ghost->scale.Set(10, 10, 10);
		m_ghost->mass = 27;
		m_ghost->color.Set(Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1));
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 10: spawn large GO_BALL
		GameObject* go = FetchGO();
		go->type = GameObject::GO_BALL;
		go->pos = m_ghost->pos;

		go->vel = m_ghost->pos - mousePos;
		//go->vel.y = 0;

		go->scale = m_ghost->scale;
		go->mass = m_ghost->mass;

		go->color = m_ghost->color;
		m_ghost->active = false;
		
	}
	if (Application::IsKeyPressed('A') && m_playerL->pos.x - 7 > 5) {
		m_playerBase->pos.x -= movePlatform;
		m_playerL->pos.x -= movePlatform;
		m_playerR->pos.x -= movePlatform;
	}
	if (Application::IsKeyPressed('D') && m_playerR->pos.x +7 < m_worldWidth - 5) {
		m_playerBase->pos.x += movePlatform;
		m_playerL->pos.x += movePlatform;
		m_playerR->pos.x += movePlatform;
	}
	
	if (rotateCannon > 1.01) {
		rotateCannonLeft = true;
		rotateCannonRight = false;
	}
	else if (rotateCannon < -1.01) {
		rotateCannonRight = true;
		rotateCannonLeft = false;
	}
	if (rotateCannonLeft == true) {
		rotateCannon -= 0.02;
	}
	else if (rotateCannonRight == true) {
		rotateCannon += 0.02;
	}
	if (slowPlatform) {
		movePlatform = 0.5;
		m_playerBase->color.Set(1, 0, 0);
		m_playerL->color.Set(1, 0, 0);
		m_playerR->color.Set(1, 0, 0);
		static double slowPlatformTime;
		slowPlatformTime += dt;
		if (slowPlatformTime > 3) {
			slowPlatform = false;
		}
	}
	if (slowPlatform == false) {
		movePlatform = 1;
		m_playerBase->color.Set(1, 1, 1);
		m_playerL->color.Set(1, 1, 1);
		m_playerR->color.Set(1, 1, 1);
	}
	//Physics Simulation Section
	unsigned size = m_goList.size();
	for(unsigned i = 0; i < size; ++i)
	{
		GameObject* go = m_goList[i];
		if(go->active)
		{
			//Exercise 7a: implement movement for game objects
			go->pos += go->vel * dt * m_speed;
			//Exercise 7b: handle out of bound game objects
			if (((go->pos.x - go->scale.x < 0) && go->vel.x < 0) || ((go->pos.x + go->scale.x > m_worldWidth) && go->vel.x > 0))
			{
				go->vel.x = -go->vel.x;
			}
			if (go->pos.x < 0 - go->scale.x || go->pos.x > m_worldWidth + go->scale.x) {
				ReturnGO(go);
				continue;
			}
			if((go->pos.y - go->scale.y < 0) && go->vel.y < 0){
				go->active = false;
				--m_objectCount;
			}
			if (((go->pos.y + go->scale.y > m_worldHeight) && go->vel.y > 0)) {
				go->vel.y = -go->vel.y;
			}
			/*if (go->pos.y < 0 - go->scale.y || go->pos.y > m_worldHeight + go->scale.y) {
				go->active = false;
			}*/
			if (go->type != GameObject::GO_BALL) {
				if (go->hp == 1) {
					go->color.Set(1, 1, 0);
				}
				else if (go->hp == 2) {
					go->color.Set(0, 0, 1);
				}
				else if (go->hp == 3) {
					go->color.Set(0, 1, 0);
				}
			}
			GameObject* go2 = nullptr;
			for (int j = i + 1; j < size; ++j) {
				go2 = m_goList[j];
				GameObject* actor(go);
				GameObject* actee(go2);

				if (go->type != GameObject::GO_BALL) {
					actor = go2;
					actee = go;
				}
				if (go2->active && CheckCollision(actor, actee)) {
					CollisionResponse(actor, actee);
					int hpleft = actee->hp - actor->damage;
					actee->hp = hpleft;
					for (int i = 0; i < size; ++i) {
						if (actee->id > 1) {
							if (actee->id == m_goList[i]->id) {
								m_goList[i]->hp = actee->hp;
								if (actee->hp <= 0 || m_goList[i]->hp <= 0) {
								
									ReturnGO(actee);
									ReturnGO(m_goList[i]);
								}
							}
						}
					}
				}
				if (go2->active && CheckCollision2(actor, actee)) {
					CollisionResponse(actor, actee);
					ReturnGO(actor);
					slowPlatform = true;
				}
			}
			if (go->type == GameObject::GO_CANNON) {
				go->direction = rotateVector(go->normal, rotateCannon);
				go->angle = Math::RadianToDegree(atan2(go->direction.y, go->direction.x));
				static double diffEnemyBallSpawn;
				diffEnemyBallSpawn += dt;
				if (diffEnemyBallSpawn > 1) {
					for (int i = 0; i < 1; ++i) {
						GameObject* eBall = FetchGO();
						Vector3 tangent(-go->direction.y, go->direction.x);
						eBall->type = GameObject::GO_ENEMYBALL;
						eBall->pos = go->pos - tangent.Normalize() * go->scale.y * 0.4;
						eBall->vel = -tangent.Normalized() * 50;
						eBall->color.Set(1, 0, 0);
						eBall->scale.Set(3, 3, 3);
						eBall->id = -99;
						eBall->hp = 0;
						eBall->mass = 10;
					}
					diffEnemyBallSpawn = 0;
				}

			}
			//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap

			//Exercise 8b: store values in auditing variables

			//Exercise 10: handle collision using momentum swap instead

			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm 
		}
	}
}
void SceneAssignment2::MakeThinWall(float width, float height, const Vector3& normal, const Vector3& pos, int id) {
	GameObject* mid = FetchGO();
	mid->type = GameObject::GO_WALL;
	mid->scale.Set(width, height, 1.0f);
	mid->pos = pos;
	mid->normal = normal;
	mid->color.Set(0.376, 0.376, 0.376);
	mid->id = id;
	mid->hp = 1000;
	mid->vel.SetZero();

	Vector3 tangent(-normal.y, normal.x);
	//Pillar 1
	GameObject* pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->color.Set(0, 1, 0);
	pillar->scale.Set(width * 0.5f, width * 0.5f, 1);
	pillar->pos = pos + height * 0.5f * tangent;
	pillar->hp = 1000;
	pillar->id = id;

	//Pillar 2
	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->color.Set(0, 1, 0);
	pillar->scale.Set(width * 0.5f, width * 0.5f, 1);
	pillar->pos = pos - height * 0.5f * tangent;
	pillar->hp = 1000;
	pillar->id = id;
}
void SceneAssignment2::MakeThickWall(float width, float height, const Vector3& normal, const Vector3& pos, int id, int hp, Vector3 color) {
	Vector3 tangent(-normal.y, normal.x);

	float size = 0.1f;

	GameObject* wall1 = FetchGO();
	wall1->type = GameObject::GO_WALL;
	wall1->scale.Set(width, height, 1.0f);
	wall1->pos = pos;
	wall1->normal = normal;
	if (hp == 1) {
		wall1->color.Set(1, 1, 0);
	}
	else if (hp == 2) {
		wall1->color.Set(0, 0, 1);
	}
	else if (hp == 3) {
		wall1->color.Set(0, 1, 0);
	}

	wall1->id = id;
	wall1->hp = hp;
	wall1->vel.SetZero();

	//4 pillars
	GameObject* pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->color = wall1->color;
	pillar->scale.Set(size, size, 1);
	pillar->pos = pos + height * 0.5f * tangent + width * 0.5f * normal;
	pillar->id = id;
	pillar->hp = hp;

	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->color = wall1->color;
	pillar->scale.Set(size, size, 1);
	pillar->pos = pos + height * 0.5f * tangent - width * 0.5f * normal;
	pillar->id = id;
	pillar->hp = hp;

	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->color = wall1->color;
	pillar->scale.Set(size, size, 1);
	pillar->pos = pos - height * 0.5f * tangent + width * 0.5f * normal;
	pillar->id = id;
	pillar->hp = hp;

	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->color = wall1->color;
	pillar->scale.Set(size, size, 1);
	pillar->pos = pos - height * 0.5f * tangent - width * 0.5f * normal;
	pillar->id = id;
	pillar->hp = hp;

	

	GameObject* wall2 = FetchGO();
	wall2->type = GameObject::GO_WALL;
	wall2->scale.Set(height, width, 1.0f);
	wall2->pos = pos;
	wall2->normal = tangent;
	wall2->color = wall1->color;
	wall2->id = id;
	wall2->hp = hp;
	wall2->vel.SetZero();
	wall2->visible = false;
	wall1->otherWall = wall2;
	wall2->otherWall = wall1;
}
bool SceneAssignment2::CheckCollision(GameObject* go1, GameObject* go2) {
	//Prevent non ball vs non ball code
	if (go1->type != GameObject::GO_BALL) {
		return false;
	}
	switch (go2->type) {
		case GameObject::GO_ENEMYBALL:
		case GameObject::GO_PILLAR:
		case GameObject::GO_BALL:
		{
			Vector3 relativeVel = go1->vel - go2->vel;
			Vector3 disDiff = go2->pos - go1->pos;

			if (relativeVel.Dot(disDiff) <= 0) {
				return false;
			}
			return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		}
		case GameObject::GO_WALL:
		{
			Vector3 diff = go1->pos - go2->pos;
			Vector3 axisX = go2->normal;
			Vector3 axisY = Vector3(-go2->normal.y, go2->normal.x, 0);

			float projectedDist = diff.Dot(axisX);

			//if it is a thick wall
			if (go2->otherWall != nullptr) {
				if (Math::FAbs(projectedDist) / go2->scale.x < Math::FAbs(diff.Dot(axisY)) / go2->otherWall->scale.x)
					return false;
			}

			if (projectedDist > 0) {
				axisX = -axisX;
			}

			return go1->vel.Dot(axisX) >= 0 && //Check 1: Travelling towards the wall?
				go2->scale.x * 0.5 + go1->scale.x > -diff.Dot(axisX) && //Check 2: Radius + thickness vs Distance
				go2->scale.y * 0.5 > fabs(diff.Dot(axisY)); // Check 3: Length check
		}
	}



	
}
bool SceneAssignment2::CheckCollision2(GameObject* go1, GameObject* go2) {
	//Prevent non ball vs non ball code
	if (go1->type != GameObject::GO_ENEMYBALL || go2->id > 0) {
		return false;
	}
	switch (go2->type) {
	case GameObject::GO_ENEMYBALL:
	case GameObject::GO_PILLAR:
	case GameObject::GO_BALL:
	{
		Vector3 relativeVel = go1->vel - go2->vel;
		Vector3 disDiff = go2->pos - go1->pos;

		if (relativeVel.Dot(disDiff) <= 0) {
			return false;
		}
		return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
	}
	case GameObject::GO_WALL:
	{
		Vector3 diff = go1->pos - go2->pos;
		Vector3 axisX = go2->normal;
		Vector3 axisY = Vector3(-go2->normal.y, go2->normal.x, 0);

		float projectedDist = diff.Dot(axisX);

		//if it is a thick wall
		if (go2->otherWall != nullptr) {
			if (Math::FAbs(projectedDist) / go2->scale.x < Math::FAbs(diff.Dot(axisY)) / go2->otherWall->scale.x)
				return false;
		}

		if (projectedDist > 0) {
			axisX = -axisX;
		}

		return go1->vel.Dot(axisX) >= 0 && //Check 1: Travelling towards the wall?
			go2->scale.x * 0.5 + go1->scale.x > -diff.Dot(axisX) && //Check 2: Radius + thickness vs Distance
			go2->scale.y * 0.5 > fabs(diff.Dot(axisY)); // Check 3: Length check
	}
	}




}
void SceneAssignment2::CollisionResponse(GameObject* go1, GameObject* go2) {

	u1 = go1->vel;
	u2 = go2->vel;
	m1 = go1->mass;
	m2 = go2->mass;

	switch (go2->type) {
	case GameObject::GO_ENEMYBALL:
		case GameObject::GO_BALL:
		{
			//2D Version 2
			Vector3 n = go1->pos - go2->pos;
			Vector3 vec = (u1 - u2).Dot(n) / (n).LengthSquared() * n;
			go1->vel = u1 - (2 * m2 / (m1 + m2)) * vec;
			go2->vel = u1 - (2 * m1 / (m2 + m1)) * -vec;
			break;

		}
		case GameObject::GO_WALL:
		{
			go1->vel = u1 - (2.0 * u1.Dot(go2->normal)) * go2->normal;
			break;
		}
		case GameObject::GO_PILLAR:
		{
			Vector3 n = (go2->pos - go1->pos).Normalize();
			go1->vel = u1 - (2.0 * u1.Dot(n)) * n;
			break;
		}
		
	}
}

void SceneAssignment2::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_PILLAR:
	case GameObject::GO_ENEMYBALL:
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
		//Exercise 11: think of a way to give balls different colors
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CANNON:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAssignment2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);
	if(m_ghost->active)
		RenderGO(m_ghost);
	RenderGO(m_playerBase);
	RenderGO(m_playerL);
	RenderGO(m_playerR);
	
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text
	std::ostringstream ss;

	////Exercise 5: Render m_objectCount
	//RenderTextOnScreen(meshList[GEO_TEXT], "Object count: " + std::to_string(m_objectCount), Color(1, 1, 1), 2, 35, 57.5);
	////Exercise 8c: Render initial and final momentum

	/*ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 57.5);*/
	
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 0, 57.5);

	if (gameState == 0) {
		RenderTextOnScreen(meshList[GEO_TEXT], "Press space to start the game", Color(1, 0, 0), 2, 30, 30);
	}
	
	//RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneAssignment2::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
static Vector3 rotateVector(const Vector3& vec, float radian) {
	return Vector3(vec.x * cos(radian) + vec.y * -sin(radian), vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}