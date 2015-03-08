/*Dominick Cucccurullo
	PONG HW2
*/
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include <string>
using namespace std;
SDL_Window* displayWindow;

//Setup function, initializes SDL, window, and Orthopedic viewport
void Setup()
{
	//setup SDL
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	
	//setup OpenGL
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//set Our viewport/projection matrix
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
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

	//Check to see if keys are pressed and change the direction ints accordingly (actual movement is done in Update)
	//1
	if (keys[SDL_SCANCODE_W] && p1-> y < .8)
	{
		//p1s direction of travel is UP
		p1->direction_y = 1;
	}
	else if (keys[SDL_SCANCODE_S] &&  p1->y > -.8)
	{
		//p1s direction of travel is DOWN
		p1->direction_y = -1;
	}
	else
	{
		//p1 direction of travel is NOWHERE
		p1->direction_y = 0;
	}
	//p2
	if (keys[SDL_SCANCODE_UP] && p2->y < .8)
	{
		//p2s direction of travel is UP
		p2->direction_y = 1;
	}
	else if (keys[SDL_SCANCODE_DOWN] && p2->y > -.8)
	{
		//p2s direction of travel is DOWN
		p2->direction_y = -1;
	}
	else
	{
		//p2s direction of travel is NOWHERE
		p2->direction_y = 0;
	}
	return true;
}
//Update function, moves the ball, paddles, and checks for collision
void Update(Entity* p1, Entity* p2, Entity* ball, float elapsed)
{
	//move paddles based off of their direction variables
	p1->y = p1->y + ((p1->speed * elapsed * p1->direction_y));
	p2->y = p2->y + ((p2->speed * elapsed * p2->direction_y));

	//update ball position based off of both x and y directions
	ball->x = ball->x + (elapsed * ball->speed * ball->direction_x);
	ball->y = ball->y + (elapsed * ball->speed * ball->direction_y);	
	
	//test for collisions with paddles
	if ((ball->x < -1.2f && ball->y > (p1->y - .18f) && ball->y < (p1->y + .18)) ||
		(ball->x > 1.2f && ball->y > (p2->y - .18f) && ball->y < (p2->y + .18)))
	{
		//ball hit a paddle, send direction of ball to the other side
		ball->direction_x *= -1;
		//shhh, the ball is getting faster
		ball->speed += .01;
	}
	
	//Test for ceiling collision
	if (ball->y > 1)
	{
		//send ball -DOWN- while maintaining x direction
		ball->direction_y = -1;
	}
	if (ball->y < -1)
	{
		//send ball +UP+ while maintaining x direction
		ball->direction_y = 1;
	}

	//Test for ball passing paddles
	if (ball->x > 1.6)
	{
		//ball has passed p2s paddle, p1 wins, reposition ball on p1 side, showing they won
		ball->x = -.7;
		ball->y = .8;
	}
	if (ball->x < -1.6)
	{
		//ball has passed p1s paddle, p2 winsm reposition ball on p2 side, showing they won
		ball->x = .7;
		ball->y = .8;
	}
	
}

void Render(Entity* p1, Entity* p2, Entity* ball)
{
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//draw game items
	p1->Draw();
	p2->Draw();
	ball->Draw();
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

	//Create and set variables of players and ball
	Entity* player1 = new Entity();
	player1->direction_x = 0;
	player1->direction_y = 0;
	player1->rotation = 90;
	player1->speed = 2.0;
	player1->textureID = LoadTexture("paddleRed.png");
	player1->height = .1;
	player1->width = .3;
	player1->x = -1.3;
	player1->y = 0;

	Entity* player2 = new Entity();
	player2->direction_x = 0;
	player2->direction_y = 0;
	player2->rotation = 90;
	player2->speed = 2.0;
	player2->textureID = LoadTexture("paddleBlu.png");
	player2->height = .1;
	player2->width = .3;
	player2->x = 1.3;
	player2->y = 0;

	Entity* ball = new Entity();
	ball->direction_x = 1;
	ball->direction_y = 1;
	ball->rotation = 0;
	ball->speed = 1.0;
	ball->textureID = LoadTexture("ballGrey.png");
	ball->height = .1;
	ball->width = .1;
	ball->x = 0;
	ball->y = 0;

	//call functions for game engine, passing in these entities and the clock
	//new game loop using ProcessInput function as the loop conditional
	while (ProcessInput(player1, player2, event, keys))
	{
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		Update(player1, player2, ball, elapsed);
		Render(player1, player2, ball);
	}
	return 0;
}
