/*Dominick Cucccurullo
PLatformer HW4
*/
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <iostream>
#include <SDL_image.h>
#include "Entity.h"
#include <string>
using namespace std;
SDL_Window* displayWindow;
// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
float timeLeftOver = 0.0f;
float gravity_y = -10;

//Setup function, initializes SDL, window, and Orthopedic viewport
void Setup()
{
	//setup SDL
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Simple platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

	//setup OpenGL
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//set Our viewport/projection matrix
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}
//lerp function, used for deceleration
float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}
//LoadTexture function, returns a GLUint of the ball/paddle sprites identified by the image name
GLuint LoadTexture(const char *image_path)
{
	//Load texture identified by the image_path parameter
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}

//ProcessInput function
bool ProcessInput(Entity* p1, Entity* p2, SDL_Event& event, const Uint8* keys)
{
	//SDL event loop
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}

	//Check to see if keys are pressed and adjust velocities/accelerations accordingly
	if (keys[SDL_SCANCODE_W] && !p1->inAir)
	{
		p1->velocity_y = 3.0;
		p1->inAir = true;
	}

	//x movement
	if (keys[SDL_SCANCODE_A])
	{
		p1->acceleration_x = -3;
	}
	else if (keys[SDL_SCANCODE_D])
	{
		p1->acceleration_x = 3;
	}
	else
	{
		p1->velocity_x = 0;
		p1->acceleration_x = 0;
	}
	//p2
	if (keys[SDL_SCANCODE_UP] && !p2->inAir)
	{
		p2->velocity_y = 3.0;
		p2->inAir = true;
	}
	if (keys[SDL_SCANCODE_LEFT] && !p2->collidedLeft)
	{
		p2->acceleration_x = -3;
	}
	else if (keys[SDL_SCANCODE_RIGHT] && !p2->collidedRight)
	{
		p2->acceleration_x = 3;
	}
	else
		p2->acceleration_x = 0;

	return true;
}
//Update function, moves the ball, paddles, and checks for collision
void FixedUpdate(Entity* p1, Entity* p2, vector<Entity*> statics, float elapsed)
{
	//lerp velocity/decelerate
	p1->velocity_x = lerp(p1->velocity_x, 0.0f, elapsed * p1->friction_x);
	p2->velocity_x = lerp(p2->velocity_x, 0.0f, elapsed * p2->friction_x);

	//update x velocity to movement
	p1->velocity_x += p1->acceleration_x * elapsed;
	p2->velocity_x += p2->acceleration_x * elapsed;
	
	//adjust gravity influenced y positions
	if (p1->inAir)
		p1->velocity_y += gravity_y * elapsed;
	else
		p1->velocity_y = 0;

	if (p2->inAir)
		p2->velocity_y += gravity_y * elapsed;
	else
		p2->velocity_y = 0;

	//move on y axis
	p1->y += p1->velocity_y * elapsed;
	p2->y += p2->velocity_y * elapsed;

	//check against all collidable entities, adjusts for penetration
	if (p1->collided(p2))
	{
		if (p1->collidedTop)
		{
		}
		if (p2->collidedTop)
		{
		}
		if (p1->collidedRight)
		{
		}
	}
	for (size_t i = 0; i < statics.size(); i++)
	{
		if (p1->collided(statics[i]))
		{

			if (p1->y > statics[i]->y && (p1->x < statics[i]->x + statics[i]->width / 2 && p1->x > statics[i]->x - statics[i]->width / 2))
			{
				float yPen = fabs(p1->y - (statics[i]->y) - p1->height / 2 - statics[i]->height / 2);
				p1->y += yPen;
				p2->velocity_y = 4;
				p1->inAir = false;
			}
			if (p1->y > statics[i]->y)
			{
				float yPen = fabs(p1->y - (statics[i]->y) - p1->height / 2 - statics[i]->height / 2);
				p1->y -= yPen;
				p2->velocity_y = 4;
				p1->collidedTop = true;
			}
		}
	if (p2->collided(statics[i]))
		{

			if (p2->y > statics[i]->y && (p2->x < statics[i]->x + statics[i]->width / 2 && p2->x > statics[i]->x - statics[i]->width / 2))
			{
				float yPen = fabs(p1->y - (statics[i]->y) - p1->height / 2 - statics[i]->height / 2);
				p2->y += yPen;
				p2->inAir = false;
			}
			if (p2->y > statics[i]->y)
			{
				float yPen = fabs(p2->y - (statics[i]->y) - p2->height / 2 - statics[i]->height / 2);
				p2->y -= yPen;
				p2->collidedTop = true;
			}
		}
	}

	//update x positions
	p1->x += p1->velocity_x * elapsed;
	p2->x += p2->velocity_x * elapsed;

	//checks for collisions and adjusts for penetrations
	//with player

	//with statics
	for(size_t i = 0; i < statics.size(); i++)
	{
		if (p1->collided(statics[i]))
		{

			if (p1->x > statics[i]->x) // p1 on the right
			{
				float xPen = fabs(p1->x - (statics[i]->x) - p1->width / 2 - statics[i]->width / 2);
				p1->x += xPen;
				p1->collidedLeft = true;
			}
			else if (p1->x < statics[i]->x) // on left
			{
				float xPen = fabs(p1->x - (statics[i]->x) - p1->width / 2 - statics[i]->width / 2);
				p1->x -= xPen;
				p1->velocity_x = 0;
				p1->collidedRight = true;
			}
		}

		if (p2->collided(statics[i]))
		{

			if (p2->x > statics[i]->x) // p1 on the right
			{
				float xPen = fabs(p2->x - (statics[i]->x) - p2->width / 2 - statics[i]->width / 2);
				p2->x += xPen;
				p2->collidedLeft = true;
			}
			else if (p1->x < statics[i]->x) // on left
			{
				float xPen = fabs(p2->x - (statics[i]->x) - p2->width / 2 - statics[i]->width / 2);
				p2->x -= xPen;
				p2->velocity_x = 0;
				p2->collidedRight = true;
			}
		}
	}



	if (p1->y <= -.81) // make collided bottom
	{
		p1->inAir = false;
		p1->velocity_y = 0;
	}
	if (p2->y <= -.81)
	{
		p2->inAir = false;
		p2->velocity_y = 0;
	}
}

void Render(Entity* p1, Entity* p2, vector<Entity*> statics)
{
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//draw game items
	p1->Draw();
	p2->Draw();
	for (size_t i = 0; i < statics.size(); i++)
	{
		statics[i]->Draw();
	}
	SDL_GL_SwapWindow(displayWindow);
}
void Cleanup(){ SDL_Quit; }

int main(int argc, char *argv[])
{
	SDL_Event event;
	Setup();
	//Setup clock
	float lastFrameTicks = 0.0f;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	//Keys for keyboard input
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//Create and set variables of players

	Entity* player1 = new Entity();
	player1->friction_x = 3.0;
	player1->height = .3;
	player1->width = .1;
	player1->textureID = LoadTexture("paddleRed.png"); // i couldnt get any other png from the internet to work :|
	player1->x = -.9;
	player1->rotation = 0;
	player1->y = .3;
	player1->inAir = true;
	player1->collidedBottom = false;
	player1->collidedLeft = false;
	player1->collidedRight = false;
	player1->collidedTop = false;

	Entity* player2 = new Entity();
	player2->textureID = LoadTexture("paddleRed.png");
	player2->height = .3;
	player2->width = .1;
	player2->rotation = 0;
	player2->friction_x = 3.0;
	player2->x = .9;
	player2->y = 0;
	player2->collidedBottom = false;
	player2->inAir = true;
	player2->collidedBottom = false;
	player2->collidedLeft = false;
	player2->collidedRight = false;
	player2->collidedTop = false;

	vector<Entity*> statics;
	float staticX = -1;
	for (int i = 0; i < 5; i++)
	{
		Entity* block = new Entity();
		block->width = .5;
		block->height = .1;
		block->isStatic = true;
		block->inAir = false;
		block->textureID = LoadTexture("paddleBlu.png");
		block->x = staticX;
		block->y = -1;
		statics.push_back(block);
		staticX += .5;
	}
	Entity* blockL = new Entity();
	blockL->width = .5;
	blockL->height = .1;
	blockL->isStatic = true;
	blockL->inAir = false;
	blockL->textureID = LoadTexture("paddleBlu.png");
	blockL->x = -.4;
	blockL->y = -.8;
	statics.push_back(blockL);

	Entity* blockR = new Entity();
	blockR->width = .5;
	blockR->height = .1;
	blockR->isStatic = true;
	blockR->inAir = false;
	blockR->textureID = LoadTexture("paddleBlu.png");
	blockR->x = .4;
	blockR->y = -.8;
	statics.push_back(blockR);
	
	while (ProcessInput(player1, player2, event, keys))
	{
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		float fixedElapsed = elapsed + timeLeftOver;
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
			fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
		}
		while (fixedElapsed >= FIXED_TIMESTEP) {
			fixedElapsed -= FIXED_TIMESTEP;
			FixedUpdate(player1, player2, statics, fixedElapsed);
		}
		timeLeftOver = fixedElapsed;
		//FixedUpdate(player1, player2, statics, fixedElapsed);
		Render(player1, player2, statics);
	}
	return 0;
}
