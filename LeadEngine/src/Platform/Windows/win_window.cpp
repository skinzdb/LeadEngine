#include "le_pch.h"

#include "win_window.h"

#include "LeadEngine/event.h"

#include "glad/glad.h"

namespace le
{
	static bool GLFWInitialised = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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

			glfwSetErrorCallback(GLFWErrorCallback);

			GLFWInitialised = true;
		}

		/* Window hints. */
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		window = glfwCreateWindow((int)properties.width, (int)properties.height, data.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LE_CORE_ASSERT(status, "Failed to initialise Glad!");
		glfwSetWindowUserPointer(window, &data);
		setVSync(true);

		/* Set GLFW callbacks. */
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				WinWindowData& data = *(WinWindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				WindowResizeEvent event(width, height);
				data.eventCallback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WinWindowData& data = *(WinWindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;

				data.eventCallback(event);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scan, int action, int mods)
			{
				WinWindowData& data = *(WinWindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressEvent event(key, 0);
						data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleaseEvent event(key);
						data.eventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressEvent event(key, 1);
						data.eventCallback(event);
					}
				}
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WinWindowData& data = *(WinWindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressEvent event(button);
						data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleaseEvent event(button);
						data.eventCallback(event);
					}
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WinWindowData& data = *(WinWindowData*)glfwGetWindowUserPointer(window);

				MouseScrollEvent event((float)xOffset, (float)yOffset);
				data.eventCallback(event);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WinWindowData& data = *(WinWindowData*)glfwGetWindowUserPointer(window);

				MouseMoveEvent event((float)xPos, (float)yPos);
				data.eventCallback(event);
			});
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

