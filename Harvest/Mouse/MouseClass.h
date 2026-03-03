#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass
{
public:
	void OnLeftButtonpressed(int x, int y);
	void OnLeftButtonReleased(int x, int y);
	void OnRightButtonPresed(int x, int y);
	void OnRightButtonReleased(int x, int y);
	void OnMiddleButtonPressed(int x, int y);
	void OnMiddleButtonReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> m_eventBuffer;

	bool m_leftIsDown = false;
	bool m_middleIsDown = false;
	bool m_rightIsDown = false;

	int m_x = 0;
	int m_y = 0;
};