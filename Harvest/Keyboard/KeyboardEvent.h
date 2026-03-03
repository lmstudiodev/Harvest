#pragma once


class KeyboardEvent
{
public:
	enum EventType
	{
		Press,
		Release,
		Invalid
	};

	KeyboardEvent();
	KeyboardEvent(const EventType eType, const unsigned char key);
	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;

	unsigned char GetKeycode() const;

private:
	EventType m_eType;
	unsigned char m_keycode;
};
