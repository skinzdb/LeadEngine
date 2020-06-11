#include "le_pch.h"

#include "win_window.h"

namespace le
{
	static bool GLFWInitialised = false;

	Window* Window::create(const WindowData& properties)
	{
		return new WinWindow(properties);
	}

	WinWindow::WinWindow(const WindowData& properties)
	{
		init(properties);
	}

	WinWindow::~WinWindow()
	{
		close();
	}

	void WinWindow::init(const WindowData& properties)
	{
		data.title = properties.title;
		data.width = properties.width;
		data.height = properties.height;

		LE_CORE_INFO("Creating window {0} ({1}, {2})", properties.title, properties.width, properties.height);

		if (!GLFWInitialised)
		{
			int success = glfwInit();
			LE_CORE_ASSERT(success, "Could not initialise GLFW!");

			GLFWInitialised = true;
		}

		/* Window hints. */
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		window = glfwCreateWindow((int)properties.width, (int)properties.height, data.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		setVSync(true);
	}

	void WinWindow::close()
	{
		glfwDestroyWindow(window);
	}

	void WinWindow::update()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void WinWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		data.vSync = enabled;
	}

	bool WinWindow::isVSync() const
	{
		return data.vSync;
	}

}

