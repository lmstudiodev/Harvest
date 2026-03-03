#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : m_eType(EventType::Invalid), m_keycode(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType eType, const unsigned char key) : m_eType(eType), m_keycode(key)
{
}

bool KeyboardEvent::IsPress() const
{
	return this->m_eType == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
	return this->m_eType == EventType::Release;
}

bool KeyboardEvent::IsValid() const
{
	return this->m_eType != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeycode() const
{
	return m_keycode;
}
