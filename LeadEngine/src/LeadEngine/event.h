#pragma once

#include "core.h"

namespace le
{
	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
		KEY_PRESS, KEY_RELEASE, KEY_HELD,
		MOUSE_PRESS, MOUSE_RELEASE, MOUSE_MOVE, MOUSE_SCROLL
	};

	enum EventCategory
	{
		NONE = 0,
		EVENT_APP = 1,
		EVENT_INPUT = 2,
		EVENT_KEYBOARD = 4,
		EVENT_MOUSE = 8,
		EVENT_MOUSE_BTN = 16,
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
							   virtual EventType getEventType() const override { return getStaticType(); }\
							   virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class LE_API Event
	{
		friend class EventDispatcher;
	protected:
		bool handled = false;
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool inCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using eventFn = std::function<bool(T&)>;
	private:
		Event& event;
	public:
		EventDispatcher(Event& event) : event(event) {}

		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (event.getEventType() == T::getStaticType())
			{
				event.handled = func(static_cast<T&>(event));
				return true;
			}
			return false;
		}
	};

	inline std::ostream& operator <<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

	/**************************************************
	APP EVENTS
	**************************************************/

	class LE_API WindowResizeEvent : public Event
	{
	private:
		unsigned int width, height;
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENT_APP)
	};

	class LE_API WindowCloseEvent : public Event
	{
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENT_APP)
	};

	/**************************************************
	KEY EVENTS
	**************************************************/

	class LE_API KeyEvent : public Event
	{
	protected:
		KeyEvent(int keycode) : keycode(keycode) {}

		int keycode;
	public:
		inline int getKeyCode() const { return keycode; }

		EVENT_CLASS_CATEGORY(EVENT_KEYBOARD | EVENT_INPUT)
	};

	class LE_API KeyPressEvent : public KeyEvent
	{
	private:
		int repeats;
	public:
		KeyPressEvent(int keycode, int repeats) : KeyEvent(keycode), repeats(repeats) {}

		inline int getRepeatCount() const { return repeats; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << keycode << " (" << repeats << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_PRESS);
	};

	class LE_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_RELEASE);
	};

	/**************************************************
	MOUSE EVENTS
	**************************************************/

	class LE_API MouseMoveEvent : public Event
	{
	private:
		float mouseX, mouseY;
	public:
		MouseMoveEvent(float x, float y) : mouseX(x), mouseY(y) {}

		inline float getX() const { return mouseX; }
		inline float getY() const { return mouseY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << mouseX << ", " << mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOVE);
		EVENT_CLASS_CATEGORY(EVENT_MOUSE | EVENT_INPUT)
	};

	class LE_API MouseScrollEvent : public Event
	{
	private:
		float xOffset, yOffset;
	public:
		MouseScrollEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

		inline float getXOffset() const { return xOffset; }
		inline float getYOffset() const { return yOffset; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << xOffset << ", " << yOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLL)
		EVENT_CLASS_CATEGORY(EVENT_MOUSE | EVENT_INPUT)
	};

	class LE_API MouseButtonEvent : public Event
	{
	protected:
		MouseButtonEvent(int button) : button(button) {}

		int button;
	public:
		inline int getMouseButton() const { return button; }

		EVENT_CLASS_CATEGORY(EVENT_MOUSE | EVENT_INPUT)
	};

	class LE_API MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(int button) : MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_PRESS)
	};

	class LE_API MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button) : MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_RELEASE)
	};
}