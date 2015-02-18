/*
Dominick Cuccurullo
Entity header file
*/
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity {
public:
	Entity(float x, float y, float width, float height, GLuint texture);


	//Accessors
	float getX();
	float getY();
	float getSpeed();
	float getWidth();
	float getHeight();
	float getDirectionX();
	float getDirectionY();
	bool getCollided();

	//Mutators
	void setX(float newX);
	void setY(float newY);
	void setSpeed(float newSpeed);
	void setXDirection(float newDir);
	void setYDirection(float newDir);
	void setCollided(bool flip);

	//Methods
	void draw();

private:

	float posX;
	float posY;
	float rotation;

	GLuint texture;

	float scale;
	float width;
	float height;

	float speed;
	float directionX;
	float directionY;

	bool collided;


};