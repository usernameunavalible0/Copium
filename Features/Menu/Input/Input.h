#pragma once

#include "../../../SDK/DrawManager/DrawManager.h"

class c_input {
public:
	void think();

public:
	bool is_pressed(const int vk) const;
	bool is_held(const int vk) const;
	bool is_any_key_pressed(int& vk);

	int cursor_x() const;
	int cursor_y() const;

private:
	enum {
		keystate_pressed = 0,
		keystate_held,
		keystate_max
	};

private:
	bool m_keystates[256][keystate_max] = { };

	int m_cursor_x = 0;
	int m_cursor_y = 0;
};

inline c_input gInput;