/*
Dominick Cuccurullo
Entity header file.
Contains all the necessary function/constructor declarations of the Entity class
*/

//includes
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

//Entity class declarations
class Entity
{
public:
	void Draw();
	bool collided(Entity* other);

	float x;
	float y;
	float rotation;

	int textureID;

	float width;
	float height;

	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;
	bool enableCollisions;

	bool inAir;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
};