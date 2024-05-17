#pragma once

#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

#define PATCHOULI_EVENT_TOPIC_APPLICATION Patchouli::AppUpdateEvent, Patchouli::AppRenderEvent
#define PATCHOULI_EVENT_TOPIC_KEYBOARD Patchouli::KeyPressedEvent, Patchouli::KeyReleasedEvent, Patchouli::KeyTypedEvent
#define PATCHOULI_EVENT_TOPIC_MOUSE Patchouli::MouseButtonPressedEvent, Patchouli::MouseButtonReleasedEvent, Patchouli::MouseMovedEvent, Patchouli::MouseScrolledEvent
#define PATCHOULI_EVENT_TOPIC_WINDOW Patchouli::WindowCloseEvent, Patchouli::WindowResizeEvent
#define PATCHOULI_EVENT_TOPIC_INPUT PATCHOULI_EVENT_TOPIC_KEYBOARD, PATCHOULI_EVENT_TOPIC_MOUSE