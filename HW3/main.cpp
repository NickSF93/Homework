/*Dominick Cucccurullo
main.cpp 
SPACE INVADERS
*/
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include <string>
#include <vector>
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
	displayWindow = SDL_CreateWindow("Space Invaders!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}
bool ProcessInput(const Uint8 *keys, Entity* p1, SDL_Event &event, float elapsed, vector<Entity*>& bullets)
{
	
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}
	unsigned int bulletTexture = LoadTexture("megaman7.png");
	//handle controls
	if (keys[SDL_SCANCODE_LEFT] && p1->getX() > -1){
		//p1 go left
		p1->setX(p1->getX() - (p1->getSpeed() * elapsed));
	}
	if (keys[SDL_SCANCODE_RIGHT] && p1->getX() < 1){
		//p1 go RIGHT
		p1->setX(p1->getX() + (p1->getSpeed() * elapsed));
	}
	if (keys[SDL_SCANCODE_E])
	{
		//SHOOT
		Entity* newBullet = new Entity(p1->getX(), p1->getY(), 2.0f/56.0f, 3.0f/146.0f, bulletTexture, true, 18.0f/56.0f, 102.0f/146.0f, elapsed);
		bullets.push_back(newBullet);
	}
	return true;
}
/*Update functions*/
bool timeAlive(float timeCreated, float elapsed)
{
	if (elapsed - timeCreated > 1.0f)
	{
		return true;
	}
	else
		return false;

}
void UpdateGame(float elapsed, Entity* p1, vector<Entity*>& enemies, vector<Entity*>& bullets)
{
	float bulletSpeed = 2.0;
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i]->setY(bullets[i]->getY() + bulletSpeed * elapsed);
		//
		//Code for checking if bullet sould be deleted, for some reason im getting an error here : ( Memory leaks galore
		//
		//if (timeAlive(bullets[i]->getTimeCreated(), elapsed))
		//{
		//	bullets.erase(bullets.begin());
		//}

		//check for collision
		for (size_t j = 0; j < enemies.size(); j++)
		{
			if (!((enemies[j]->getX() > bullets[i]->getX() ||
				enemies[j]->getX() < bullets[i]->getX()) &&
				(enemies[j]->getY() > bullets[i]->getY() ||
				enemies[j]->getY() < bullets[i]->getY())))
			{
				//collided!
				enemies[j]->setCollided(true);
				bullets[i]->setCollided(true);
			}
		}
	}
	//loop to delete collided thing (doesnt work)
	//for (size_t i = 0; i < bullets.size(); i++)
	//{
	//	if (bullets[i]->getCollided() == true)
	//	{
	//		bullets[i]->setX(-3);
	//	}
	//}
	//for (size_t i = 0; i < enemies.size(); i++)
	//{
	//	if(enemies[i]->getCollided() == true)
	//		enemies[i]->setX(3);
}
void Render(Entity* p1, vector<Entity*>& enemies, vector<Entity*>& bullets)
{
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//draw player
	p1->draw();
	//enemies
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw();
	}
	//render bullets
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i]->draw();
	}
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
	bool p1lose = false;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//player entity
	unsigned int texture = LoadTexture("megaman7.png");
	Entity* p1 = new Entity(0,-.9f, 40.0f/56.0f, .26, texture, false, 9.0f/56.0f, 5.0f/146.0f, elapsed);
	vector<Entity*> enemies;
	vector<Entity*> bullets;
	float enemyXLoc = -1;
	for (int i = 0; i < 6; i++)
	{
		//create enemies with their varying x locations
	//	Entity* enemy = new Entity(enemyXLoc, .6, .2, .3 , enemyText, false, 43.0f / 56.0f, 45.0f / 146.0f);
		Entity* enemy = new Entity(enemyXLoc, .9, 22 / 56.0f, 19 / 146.0f, texture, false, 17.0f / 56.0f, 65.0f / 146.0f, elapsed);
		enemies.push_back(enemy);
		Entity* enemy2 = new Entity(enemyXLoc, .7, 22 / 56.0f, 19 / 146.0f, texture, false, 17.0f / 56.0f, 65.0f / 146.0f, elapsed);
		enemies.push_back(enemy2);
		enemyXLoc += .4;
	}

	p1->setSpeed(2);
	while (ProcessInput(keys, p1, event, elapsed, bullets))
	{
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		UpdateGame(elapsed, p1,enemies, bullets);
		Render(p1, enemies, bullets);
	}
	SDL_Quit();
	return 0;
}
//int main(int argc, char *argv[])
//{
//	SpaceInvaders app;
//	while (!app.Update()) {}
//	return 0;
//}