#include "input.h"

void c_input::think() {
	I::VGuiSurface->SurfaceGetCursorPos(m_cursor_x, m_cursor_y);

	for (int n = 1; n < 256; n++) {
		const short k = GetAsyncKeyState(n);

		m_keystates[n][keystate_pressed] = (k & 0x0001);
		m_keystates[n][keystate_held] = (k);
	}
}

bool c_input::is_pressed(const int vk) const {
	return m_keystates[vk][keystate_pressed];
}

bool c_input::is_held(const int vk) const {
	return m_keystates[vk][keystate_held];
}

bool c_input::is_any_key_pressed(int& vk) {
	vk = 0;

	for (int n = 1; n < 256; n++) {
		if (is_pressed(n)) {
			vk = n;
			return true;
		}
	}

	return (vk != 0);
}

int c_input::cursor_x() const {
	return m_cursor_x;
}

int c_input::cursor_y() const {
	return m_cursor_y;
}

