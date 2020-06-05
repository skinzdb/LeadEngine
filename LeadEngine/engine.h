/*!	\file engine.h
	\brief Handles displaying the window, game logic and calls to the renderer
*/

#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.h"

/**********************************************************************
\class Display
\brief Creates a window using GLFW.
**********************************************************************/

/*! \fn static void error_callback()
	\brief Prints error message whenever GLFW encounters an error.
*/

/*! \fn void init()
	\brief Initialises GLFW and GLEW. Creates a new window using \a width, \a height and \a title.
*/

/*! \fn void update()
	\brief Renders other framebuffer and lets GLFW poll events.
*/

/*! \fn void clearColour()
	\brief Sets the clear colour.

	\param r
	\param g
	\param b
	\param a
*/

/*! \fn const GLchar* getTitle()
	\brief Returns the title of window.
*/

/*! \fn void setTitle()
	\brief Sets title of window.

	\param title
*/

/*! \fn GLsizei getWidth()
	\brief Returns the width of window.
*/

/*! \fn GLsizei getHeight()
	\brief Returns the height of window.
*/

/*! \fn bool isVSync()
	\brief Returns whether the window is using v-sync.
*/

/*! \fn void setVSync()
	\brief Set the window to be v-sync or not.

	\param value
*/

/*! \fn GLFWwindow* getWindow()
	\brief Return the GLFW window handle.
*/

/*! \fn void resize(GLsizei width, GLsizei height)
	\brief Resizes window.

	\param width
	\param height
*/

/*! \fn bool shouldClose()
	\brief Returns if the window should close or not.
	Closing the window will mean this outputs \a TRUE.
*/

class Display
{
private:
	const char* title;
	int width;
	int height;
	bool vSync;
	GLFWwindow* window;
public:
	Display(const GLchar* title, GLsizei width, GLsizei height);
	void init();
	void update();
	void clearColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	const GLchar* getTitle();
	void setTitle(const GLchar* title);
	GLsizei getWidth();
	GLsizei getHeight();
	bool isVSync();
	void setVSync(bool value);
	GLFWwindow* getWindow();
	void resize(GLsizei width, GLsizei height);
	bool shouldClose();
};

/**********************************************************************
\class Timer
\brief Functionality to get elapsed times.
**********************************************************************/

/*! \fn double getTime()
	\brief Returns time since epoch in seconds.
*/

/*! \fn void init()
	\brief Sets current time to \a lastTime.
*/

/*! \fn double getLastTime()
	\brief Returns \a lastTime.
*/

/*! \fn float getElapsedTime()
	\brief Returns (current time - \a lastTime)
*/

class Timer
{
private:
	double lastTime;
public:
	void init();
	double getTime();
	double getLastTime();
	float getElapsedTime();
};

/**********************************************************************
\class Engine
\brief Contains the main game loop for displaying the window and rendering objects
**********************************************************************/

/*! \fn void init()
	\brief Initialises the display, renderer and timer, plus any game objects the user requries.
*/

/*! \fn void update(float interval)
	\brief Called every frame, place any game logic that needs to be updated here.

	\param interval The time (in seconds) since the last frame.
*/

/*! \fn void gameLoop()
	\brief Calls the update and render functions in accordance to the \a TARGET_UPS.
*/

/*! \fn void sync()
	\brief Syncs the frametime to match screen refresh rate.
*/

/*! \fn void render()
	\brief Use renderer functions inside here.
*/

/*! \fn void cleanup()
	\brief Cleans up renderer and entities after game is finished.
*/

class Engine
{
private:
	const int TARGET_FPS = 60;
	const int TARGET_UPS = 30;

	Display display;
	Renderer renderer;
	Timer timer;

	Camera cam;

	Entity test;
	std::vector<Entity*> entities;

public:
	Engine(const char* windowTitle, int width, int height);
	void init();
	void update(float interval);
	void gameLoop();
	void sync();
	void render();
	void cleanup();
};

class Transform
{
private:

public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform(float x, float y, float z);
	Transform();
	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);
	void multScale(float factor);
};
#endif

