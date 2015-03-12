/*
Dominick Cuccurullo
Entity cpp file, all the functions are defined
*/

#include "Entity.h"

bool Entity::collided(Entity* other)
{
	if (this->y - (this->height * .5f) < other->y + (other->height * .5f) &&
		this->y + (this->height * .5f) > other->y - (other->height * .5f) &&
		this->x - (this->width * .5f) < other->x + (other->height * .5f) &&
		this->x + (this->width * .5f) > other->x + (other->height * .5f))
	{
		//colliding with other Entity
		if (!other->isStatic) // with other player
		{
			if (this->y < other->y)
			{
				this->collidedTop = true;
				other->collidedBottom = true;
			}
			if (this->y > other->y)
			{
				this->collidedBottom = true;
				other->collidedTop = true;
			}
			if (this->x < other->x)
			{
				this->collidedRight = true;
				other->collidedLeft = true;
			}
			if (this->x > other->x)
			{
				this->collidedLeft = true;
				other->collidedRight = true;
			}
		}
		if (other->isStatic)// with static entity 
		{
			if (this->y < other->y)
			{
				this->collidedTop = true;
			}
			if (this->y > other->y)
			{
				this->collidedBottom = true;
			}
			if (this->x < other->x)
			{
				this->collidedRight = true;
			}
			if (this->x > other->x)
			{
				this->collidedLeft = true;
			}
		}
		return true;
	}
	else
		return false;

}
void Entity::Draw()
{
	//Draw  this entities texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { width*-0.5f, height*0.5f, width*-0.5f, height*-0.5f, width*0.5f, height*-0.5f, width*0.5f, height*0.5f };
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
