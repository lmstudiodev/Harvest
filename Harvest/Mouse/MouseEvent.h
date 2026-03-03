#pragma once

struct  MousePoint
{
	int x;
	int y;
};

class MouseEvent
{
public:
	enum MouseEventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};

private:
	MouseEventType m_etype;
	int m_x;
	int m_y;

public:
	MouseEvent();
	MouseEvent(const MouseEventType type, const int x, const int y);

	bool IsValid() const;

	MouseEventType GetType() const;
	MousePoint GetPoint() const;
	int GetPosX() const;
	int GetPosY() const;
};