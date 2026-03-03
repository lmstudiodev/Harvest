#pragma once
#include "KeyboardEvent.h"
#include <queue>

class KeyboardClass
{
public:
	KeyboardClass();

	bool KeyIsPressed(const unsigned char keycode);
	bool KeyboardEventBufferIsEmpty();
	bool CharBufferIsEmpty();

	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeatKey();
	void DisableAutoRepeatKey();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();

	bool IsKeyAutoRepeat();
	bool IsCharAutoRepeat();

private:
	bool m_autoRepeatKey = false;
	bool m_autoRepeatChars = false;
	bool m_keyStates[256];

	std::queue<KeyboardEvent> m_keyboardEventBuffer;
	std::queue<unsigned char> m_charBufer;
};
