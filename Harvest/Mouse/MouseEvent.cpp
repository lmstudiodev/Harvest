#include "MouseEvent.h"

MouseEvent::MouseEvent() : m_etype(MouseEventType::Invalid), m_x(0), m_y(0)
{
}

MouseEvent::MouseEvent(const MouseEventType type, const int x, const int y) : m_etype(type), m_x(x), m_y(y)
{
}

bool MouseEvent::IsValid() const
{
	return this->m_etype != MouseEventType::Invalid;
}

MouseEvent::MouseEventType MouseEvent::GetType() const
{
	return this->m_etype;
}

MousePoint MouseEvent::GetPoint() const
{
	MousePoint mp{this->m_x, this->m_y };

	return mp;
}

int MouseEvent::GetPosX() const
{
	return this->m_x;
}

int MouseEvent::GetPosY() const
{
	return this->m_y;
}
