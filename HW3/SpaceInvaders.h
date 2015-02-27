#pragma once
#include "Entity.h"

class SpaceInvaders{
public:
	SpaceInvaders();
	~SpaceInvaders();

	void Init();
	bool UpdateandRender();

	void Render();
	void Update(float elapsed);

private:
	bool done;
	float lastFrameTicks;
	//SDL_Window displayWindow;
};