
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static void Quit(bool yes);
	static int sceneNum;
	static bool sceneChange;
	static void sceneChanger(int num);

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
};

#endif