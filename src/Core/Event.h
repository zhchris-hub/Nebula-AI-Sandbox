#pragma once

#include <string>
#include <functional>

namespace nebula {

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None        = 0,
    CatWindow   = 1 << 0,
    CatInput    = 1 << 1,
    CatKeyboard = 1 << 2,
    CatMouse    = 1 << 3
};

class Event {
public:
    virtual ~Event() = default;
    virtual EventType GetEventType() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const = 0;

    bool Handled = false;
};

// ── Window Events ─────────────────────────────────────────────────────

class WindowCloseEvent : public Event {
public:
    EventType GetEventType() const override { return EventType::WindowClose; }
    int GetCategoryFlags() const override { return CatWindow; }
    std::string ToString() const override { return "WindowCloseEvent"; }
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

    EventType GetEventType() const override { return EventType::WindowResize; }
    int GetCategoryFlags() const override { return CatWindow; }
    std::string ToString() const override {
        return "WindowResizeEvent: " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
    }

private:
    int m_Width, m_Height;
};

// ── Key Events ────────────────────────────────────────────────────────

class KeyEvent : public Event {
public:
    int GetKeyCode() const { return m_KeyCode; }
    int GetCategoryFlags() const override { return CatKeyboard | CatInput; }

protected:
    KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keyCode, bool repeat) : KeyEvent(keyCode), m_Repeat(repeat) {}
    bool IsRepeat() const { return m_Repeat; }

    EventType GetEventType() const override { return EventType::KeyPressed; }
    std::string ToString() const override {
        return "KeyPressedEvent: " + std::to_string(m_KeyCode) + (m_Repeat ? " (repeat)" : "");
    }

private:
    bool m_Repeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

    EventType GetEventType() const override { return EventType::KeyReleased; }
    std::string ToString() const override {
        return "KeyReleasedEvent: " + std::to_string(m_KeyCode);
    }
};

// ── Mouse Events ──────────────────────────────────────────────────────

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : m_X(x), m_Y(y) {}

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

    EventType GetEventType() const override { return EventType::MouseMoved; }
    int GetCategoryFlags() const override { return CatMouse | CatInput; }
    std::string ToString() const override {
        return "MouseMovedEvent: " + std::to_string(m_X) + ", " + std::to_string(m_Y);
    }

private:
    float m_X, m_Y;
};

class MouseButtonPressedEvent : public Event {
public:
    MouseButtonPressedEvent(int button) : m_Button(button) {}

    int GetButton() const { return m_Button; }

    EventType GetEventType() const override { return EventType::MouseButtonPressed; }
    int GetCategoryFlags() const override { return CatMouse | CatInput; }
    std::string ToString() const override {
        return "MouseButtonPressedEvent: " + std::to_string(m_Button);
    }

private:
    int m_Button;
};

class MouseButtonReleasedEvent : public Event {
public:
    MouseButtonReleasedEvent(int button) : m_Button(button) {}

    int GetButton() const { return m_Button; }

    EventType GetEventType() const override { return EventType::MouseButtonReleased; }
    int GetCategoryFlags() const override { return CatMouse | CatInput; }
    std::string ToString() const override {
        return "MouseButtonReleasedEvent: " + std::to_string(m_Button);
    }

private:
    int m_Button;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }

    EventType GetEventType() const override { return EventType::MouseScrolled; }
    int GetCategoryFlags() const override { return CatMouse | CatInput; }
    std::string ToString() const override {
        return "MouseScrolledEvent: " + std::to_string(m_XOffset) + ", " + std::to_string(m_YOffset);
    }

private:
    float m_XOffset, m_YOffset;
};

using EventCallbackFn = std::function<void(Event&)>;

} // namespace nebula
