#pragma once

#include "LeadEngine/Window.h"

#include <GLFW/glfw3.h>

namespace le
{
	class WinWindow : public Window
	{
	private:
		virtual void init(const WindowData& properties);
		virtual void close();

		GLFWwindow* window;

		struct WinWindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WinWindowData data;
	public:
		WinWindow(const WindowData& properties);
		virtual ~WinWindow();

		void update() override;

		inline unsigned int getWidth() const override { return data.width; }
		inline unsigned int getHeight() const override { return data.height; }

		/* Window attributes. */
		inline void setEventCallback(const EventCallbackFn& callback) override { data.eventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;
	};
}

