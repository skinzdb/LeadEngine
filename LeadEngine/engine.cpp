#include <thread>
#include <stdlib.h>

#include <chrono>
using namespace std::chrono;

#include "engine.h"

/**********************************************************************
 * ENGINE															  *
 **********************************************************************/

Engine::Engine(const char* windowTitle, int width, int height) : display(windowTitle, width, height), timer(), test(NULL, NULL)
{
	init();
	gameLoop();
}

void Engine::init()
{
	display.init();
	renderer.init(&display);
	timer.init();

	cam = Camera(0, 0, 5);

	//test = Entity(ModelLoader::load("C:/Users/david/Documents/Java/MyGame/res/blacksmith.obj"), new Texture("C:/Users/david/Documents/Java/MyGame/res/blendmap.png", 0.7f, 0.3f, false));
	//test = Entity(ModelLoader::load("C:/Users/David/Documents/negev.obj"), new Texture("C:/Users/David/Pictures/white.png"));
	test.translate(0, 0, -10);
	test.multScale(1.55f);
	entities.push_back(&test);
}

void Engine::update(float interval)
{
	test.rotate(.0f, 1.5f, .0f);
}


void Engine::gameLoop()
{
	float elapsed;
	float accumulator = 0;
	float interval = 1.0f / TARGET_UPS;

	while (!display.shouldClose())
	{
		elapsed = timer.getElapsedTime();
		accumulator += elapsed;

		//float fps = 1.0f / elapsed;	
		//fprintf(stdout, "FPS: %.1f\n", fps);

		while (accumulator >= interval)
		{
			update(interval);
			accumulator -= interval;
		}

		render();

		if (!display.isVSync())
		{
			sync();
		}
	}

	/* Cleanup windows and terminate GLFW. */
	glfwDestroyWindow(display.getWindow());
	glfwTerminate();
}


void Engine::sync()
{
	float loopSlot = 1.0f / TARGET_FPS;
	double endTime = timer.getLastTime() + loopSlot;
	while (timer.getTime() < endTime)
	{
		std::this_thread::sleep_for(1ms);
	}
}


void Engine::render()
{
	renderer.render(cam, entities, true);
	display.update();
}


void Engine::cleanup()
{
	renderer.cleanup();
	for (Entity* entity : entities)
	{
		entity->cleanup();
	}
}

/**********************************************************************
 * TIMER    														  *
 **********************************************************************/

double Timer::getTime()
{
	auto now = high_resolution_clock::now();
	return time_point_cast<microseconds>(now).time_since_epoch().count() / 1000000.0;
}

void Timer::init()
{
	lastTime = getTime();
}

double Timer::getLastTime()
{
	return lastTime;
}


float Timer::getElapsedTime()
{
	double now = getTime();
	float duration = (float)(now - lastTime);
	lastTime = now;	/* Update to current time */
	return duration;
}

/**********************************************************************
 * DISPLAY   														  *
 **********************************************************************/

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

Display::Display(const GLchar* title, GLsizei width, GLsizei height) : window(NULL), vSync(true)
{
	this->title = title;
	this->width = width;
	this->height = height;
}

void Display::init()
{
	glfwSetErrorCallback(error_callback); /* Setup an error callback */

	if (!glfwInit()) /* Initialise GLFW. */
		exit(EXIT_FAILURE);

	/* Window hints. OPENGL version 3.2 is minimum requirement. */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); /* Required for macOS. */
#endif

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)	/* Window creation unsuccessful. */
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	GLenum err = glewInit(); /* Initialise GLEW. */
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	glfwSwapInterval(1); /* Single buffer v-sync. */

	glViewport(0, 0, width, height);
}


void Display::update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Display::clearColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}

const GLchar* Display::getTitle()
{
	return title;
}

void Display::setTitle(const GLchar* title)
{
	this->title = title;
	glfwSetWindowTitle(window, title);
}

GLsizei Display::getWidth()
{
	return width;
}

GLsizei Display::getHeight()
{
	return height;
}


bool Display::isVSync()
{
	return vSync;
}

void Display::setVSync(bool value)
{
	vSync = value;
	if (value)
	{
		glfwSwapInterval(1);
	} 
	else
	{
		glfwSwapInterval(0);
	}

}

GLFWwindow* Display::getWindow()
{
	return window;
}

void Display::resize(GLsizei width, GLsizei height)
{
	this->width = width;
	this->height = height;
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

bool Display::shouldClose()
{
	return glfwWindowShouldClose(window);
}

/**********************************************************************
 * TRANSFORM														  *
 **********************************************************************/

Transform::Transform(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

Transform::Transform() : Transform(0, 0, 0)
{
}

void Transform::translate(float x, float y, float z)
{
	position += glm::vec3(x, y, z);
}

void Transform::rotate(float x, float y, float z)
{
	rotation += glm::vec3(x, y, z);
}

void Transform::multScale(float factor)
{
	scale *= factor;
}
