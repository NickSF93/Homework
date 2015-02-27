#include "SpaceInvaders.h"

SpaceInvaders::SpaceInvaders()
{
}
SpaceInvaders::~SpaceInvaders()
{
}
void SpaceInvaders::Init()
{
	//SDL_Init(SDL_INIT_VIDEO);
	//displayWindow = SDL_CreateWindow("Space Invaders!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	//SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	//SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}
bool SpaceInvaders::UpdateandRender()
{
	return true;
}
void SpaceInvaders::Render()
{
}
void SpaceInvaders::Update(float elapsed)
{
}