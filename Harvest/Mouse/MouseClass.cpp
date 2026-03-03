#include "MouseClass.h"

void MouseClass::OnLeftButtonpressed(int x, int y)
{
	this->m_leftIsDown = true;

	MouseEvent me(MouseEvent::MouseEventType::LPress, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnLeftButtonReleased(int x, int y)
{
	this->m_leftIsDown = false;

	MouseEvent me(MouseEvent::MouseEventType::LRelease, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnRightButtonPresed(int x, int y)
{
	this->m_rightIsDown = true;

	MouseEvent me(MouseEvent::MouseEventType::RPress, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnRightButtonReleased(int x, int y)
{
	this->m_rightIsDown = false;

	MouseEvent me(MouseEvent::MouseEventType::RRelease, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnMiddleButtonPressed(int x, int y)
{
	this->m_middleIsDown = true;

	MouseEvent me(MouseEvent::MouseEventType::MPress, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnMiddleButtonReleased(int x, int y)
{
	this->m_middleIsDown = false;

	MouseEvent me(MouseEvent::MouseEventType::MRelease, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnWheelUp(int x, int y)
{
	MouseEvent me(MouseEvent::MouseEventType::WheelUp, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnWheelDown(int x, int y)
{
	MouseEvent me(MouseEvent::MouseEventType::WheelDown, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnMouseMove(int x, int y)
{
	this->m_x = x;
	this->m_y = y;

	MouseEvent me(MouseEvent::MouseEventType::Move, x, y);
	this->m_eventBuffer.push(me);
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	MouseEvent me(MouseEvent::MouseEventType::RAW_MOVE, x, y);
	this->m_eventBuffer.push(me);
}

bool MouseClass::IsLeftDown()
{
	return this->m_leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return this->m_middleIsDown;
}

bool MouseClass::IsRightDown()
{
	return this->m_rightIsDown;
}

MousePoint MouseClass::GetPos() const
{
	return { this->m_x, this->m_y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return m_eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if(this->m_eventBuffer.empty())
		return MouseEvent();

	MouseEvent me = this->m_eventBuffer.front();
	this->m_eventBuffer.pop();

	return me;
}
