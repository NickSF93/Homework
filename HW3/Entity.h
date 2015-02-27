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
	Entity(float x, float y, float width, float height, GLuint texture, bool isBullet, float u, float v,float timeCreated);
	//Sprite::Sprite(float xPos, float yPos, float uVal, float vVal, float swidth, float sheight, bool visibility, bool isAlive, GLuint spriteTexture) {


	//Accessors
	float getX();
	float getY();
	float getSpeed();
	float getWidth();
	float getHeight();
	float getDirectionX();
	float getDirectionY();
	bool getCollided();
	float getTimeCreated();
	//Mutators
	void setX(float newX);
	void setY(float newY);
	void setSpeed(float newSpeed);
	void setXDirection(float newDir);
	void setYDirection(float newDir);
	void setCollided(bool flip);

	//Methods
	void draw();
	void shoot();
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
	float timeCreated;
	float u;
	float v;
};