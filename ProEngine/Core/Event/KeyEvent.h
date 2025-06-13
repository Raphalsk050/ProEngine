#pragma once

#include "Core/Input/KeyCodes.h"
#include "Event.h"

namespace ProEngine {
    class KeyEvent : public Event {
    public:
        KeyCode GetKeyCode() const { return key_code_; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(const KeyCode keycode): key_code_(keycode) {}

        KeyCode key_code_;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
            : KeyEvent(keycode), is_repeat_(isRepeat)
        {}

        bool IsRepeat() const { return is_repeat_; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << key_code_
               << " (repeat = " << is_repeat_ << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool is_repeat_;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(const KeyCode keycode): KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << key_code_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(const KeyCode keycode): KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << key_code_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
} // namespace ProEngine
