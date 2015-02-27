/*
Dominick Cuccurullo
Entity cpp file
*/

#include "Entity.h"

Entity::Entity(float x, float y, float width, float height, GLuint texture, bool isBullet, float u, float v, float timeCreated){

	posX = x;
	posY = y;
	Entity::width = width;
	Entity::height = height;
	rotation = 0.0f;
	Entity::texture = texture;

	if (isBullet)
	{
		speed = 2.0;
		directionY = 1.0;
	}
	else
	{
		speed = 0.0f;
		float directionY = 0.0;
	}
	float directionX = 0.0;
	collided = false;
	this->u = u;
	this->v = v;
	this->timeCreated = timeCreated;
}
//Accessors
float Entity::getX() {
	return posX;
}

float Entity::getY() {
	return posY;
}

float Entity::getDirectionX() {
	return directionX;
}

float Entity::getDirectionY() {
	return directionY;
}

float Entity::getSpeed() {
	return speed;
}

float Entity::getWidth() {
	return width;
}

float Entity::getHeight() {
	return height;
}

//Mutators
void Entity::setX(float newX) {
	posX = newX;
}

void Entity::setY(float newY) {
	posY = newY;
}

void Entity::setSpeed(float newSpeed) {
	speed = newSpeed;
}

void Entity::setXDirection(float newDir) {
	directionX = newDir;
}
void Entity::setYDirection(float newDir) {
	directionY = newDir;
}
void Entity::setCollided(bool flip){
	collided = flip;
}

//Methods
void Entity::draw() { 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(posX, posY, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { width*-0.5f, height*0.5f, width*-0.5f, height*-0.5f, width*0.5f, height*-0.5f, width*0.5f, height*0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	//GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);

}
