#pragma once

#include "le_pch.h"

#include "LeadEngine/core.h"
#include "LeadEngine/event.h"

namespace le
{
	struct WindowData
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowData(const std::string& title = "Lead Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	/* Interface for a desktop based window. */
	class LE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void update() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		/* Window attributes. */
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		/* Implemented in the .cpp file depending on the platform. */
		static Window* create(const WindowData& properties = WindowData());
	};
}

