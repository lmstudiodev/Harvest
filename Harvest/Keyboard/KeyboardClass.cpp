#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
	for (int i = 0; i < 256; i++)
	{
		this->m_keyStates[i] = false;
	}
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
	return this->m_keyStates[keycode];
}

bool KeyboardClass::KeyboardEventBufferIsEmpty()
{
	return this->m_keyboardEventBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
	return this->m_charBufer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if(this->m_keyboardEventBuffer.empty())
		return KeyboardEvent();

	KeyboardEvent e = this->m_keyboardEventBuffer.front();
	this->m_keyboardEventBuffer.pop();

	return e;
}

unsigned char KeyboardClass::ReadChar()
{
	if(this->m_charBufer.empty())
		return 0u;

	unsigned char c = this->m_charBufer.front();
	this->m_charBufer.pop();

	return c;
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	this->m_keyStates[key] = true;
	this->m_keyboardEventBuffer.push(KeyboardEvent(KeyboardEvent::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	this->m_keyStates[key] = false;
	this->m_keyboardEventBuffer.push(KeyboardEvent(KeyboardEvent::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	this->m_charBufer.push(key);
}

void KeyboardClass::EnableAutoRepeatKey()
{
	this->m_autoRepeatKey = true;
}

void KeyboardClass::DisableAutoRepeatKey()
{
	this->m_autoRepeatKey = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	this->m_autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	this->m_autoRepeatChars = false;
}

bool KeyboardClass::IsKeyAutoRepeat()
{
	return this->m_autoRepeatKey;
}

bool KeyboardClass::IsCharAutoRepeat()
{
	return this->m_autoRepeatChars;
}
