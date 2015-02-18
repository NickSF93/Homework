#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

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
int main(int argc, char *argv[])
{
	//
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	float lastFrameTicks = 0.0f;
	float starAngle = 1.0f;
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	GLfloat triangle[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	glVertexPointer(2, GL_FLOAT, 0, triangle);

	GLuint falconImage = LoadTexture("Captain_Falcon.png");
	GLuint star = LoadTexture("starSilver.png");
	GLuint falconCrest = LoadTexture("j.png");


	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}


		//texture code
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		glClearColor(0.26666666666f, 0.26666666666f, 0.26666666666f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		starAngle += elapsed;
		DrawSprite(falconCrest, 0, .5, 0);
		DrawSprite(falconImage, 0, 0.35, 0);

		DrawSprite(falconCrest, 1, .5, 0);
		DrawSprite(falconImage, 1, 0.35, 0);

		DrawSprite(falconCrest, -1, .5, 0);
		DrawSprite(falconImage, -1, 0.35, 0);

		DrawSprite(star, 0, -0.5, 10 * starAngle);
		DrawSprite(falconCrest, 0, -.4, 100 * starAngle);


		//draw a rotating quad
		glBegin(GL_QUADS);
		glColor3f(0, 0, 0);
		glVertex3f(-2, -1, 0.0);

		glColor3f(1, 1, 1);
		glVertex3f(-1, 1, 0);

		glColor3f(.5, .4, .7);
		glVertex3f(-3, -.7, 0);

		glColor3f(.2, .9, 1);
		glVertex3f(-1, -.9, 0);
		glEnd();

		SDL_GL_SwapWindow(displayWindow);




	}

	SDL_Quit();
	return 0;
}