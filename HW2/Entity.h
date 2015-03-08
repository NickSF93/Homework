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

	float x;
	float y;
	float rotation;

	int textureID;

	float width;
	float height;

	float speed;
	float direction_x;
	float direction_y;
};