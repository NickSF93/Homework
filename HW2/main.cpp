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

GLuint LoadTexture(const char *image_path) 
{
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

void DrawSprite(GLint texture, float x, float y, float rotation) 
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

typedef struct {
	float x;
	float y;
	float r;
	float g;
	float b;
} Vertex2D;
void Setup()
{
	//SDL and Viewport code from template
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("~Pong~", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}
bool ProcessInput(const Uint8 *keys, Entity* p1, Entity* p2, Entity* Ball, SDL_Event &event, float time)
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}
	//handle controls
	if (keys[SDL_SCANCODE_W] && p1->getY() < .85){
		//p1 go up
		p1->setY(p1->getY() + (p1->getSpeed() *time));
	}
	if (keys[SDL_SCANCODE_S] && p1->getY() > -.85){
		//p1 go down
		p1->setY(p1->getY()- (p1->getSpeed() * time));
	}
	if (keys[SDL_SCANCODE_UP] && p2->getY() < .85){
		//p2 go up
		p2->setY(p2->getY() + (p2->getSpeed() * time));
	}
	if (keys[SDL_SCANCODE_DOWN] && p2->getY() > -.85){
		//p2 go down
		p2->setY(p2->getY() - (p2->getSpeed() * time));
	}
	return true;
}
//Update function
void UpdateGame(float elapsed, Entity* p1, Entity* p2, Entity* ball, bool &p1Win, bool &p2Win)
{
	//maintain velocity
	ball->setX( ball->getX() + ( elapsed * ball->getDirectionX() * ball->getSpeed() ) );
	
	ball->setY( ball->getY() + ( elapsed * ball->getDirectionY() * ball->getSpeed() ) );

	//collision detection
	float directionX = ball->getDirectionX();
	float directionY = ball->getDirectionY();

	if (ball->getX() < -1.2f && ball->getY() > p1->getY() - .18f && ball->getY() < p1->getY() + .18)
	{//ball hit p1s paddle
		directionX = -1 * directionX;
		ball->setXDirection(directionX);
	}
	if (ball->getX() > 1.2f && ball->getY() > p2->getY() - .18f && ball->getY() < p2->getY() + .18)
	{//ball hit p2s paddle
		directionX = -1 * directionX;
		ball->setXDirection(directionX);
	}
	//ceiling
	if (ball->getY() > 1)
	{
		directionY = -1 * ball->getDirectionY();
		ball->setYDirection(directionY);
	}
	//floor
	if (ball->getY() < -1)
	{
		directionY = -1 * ball->getDirectionY();
		ball->setYDirection(directionY);
	}
	if (ball->getX() > 1.6)
	{
		p1Win = true;
		//reset ball on p1 top side to show p1 won the round
		ball->setX(-1.0);
		ball->setY(.7);
		ball->setXDirection(ball->getDirectionX());
		ball->setYDirection(-.3);
		ball->setSpeed(ball->getSpeed() + .001);
	}
	if (ball->getX() < -1.6)
	{
		p2Win = true;
		//reset ball on p2 top side to show p2 won the round
		ball->setX(1.0);
		ball->setY(.7);
		ball->setXDirection(ball->getDirectionX());
		ball->setYDirection(-.3);
		ball->setSpeed(ball->getSpeed() + .001);
	}
	p1Win = false;
	p2Win = false;
}

void Render( Entity* p1, Entity* p2, Entity* ball)
{
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//draw game items
	p1->draw();
	p2->draw();
	ball->draw();
	SDL_GL_SwapWindow(displayWindow);
}
void Cleanup(){ SDL_Quit; }

int main(int argc, char *argv[])
{
	SDL_Event event;
	Setup();
	//Instantiate elapsed variable, keyboard state, and conditional booleans
	float lastFrameTicks = 0.0f;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	bool p1Win = false;
	bool p2Win = false;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	//player entities
	Entity* p1 = new Entity(-1.3, .2, .1, .3, LoadTexture("paddleBlu.png"));

	Entity* p2 = new Entity(1.3, -.2, .1, .3, LoadTexture("paddleRed.png"));

	Entity* ball = new Entity(0, 0, .1, .1, LoadTexture("ballGrey.png"));

	p1->setSpeed(2);
	p2->setSpeed(2);
	ball->setSpeed(1);
	ball->setXDirection(1);
	ball->setYDirection(.3);
	//Loop through process update and render untilhe game is closed.  This allows for every frame to appear.
	while (ProcessInput(keys,p1, p2, ball, event, elapsed)){
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		UpdateGame(elapsed,p1, p2, ball, p1Win, p2Win);
		Render(p1,p2,ball);
	}


SDL_Quit();
return 0;
}