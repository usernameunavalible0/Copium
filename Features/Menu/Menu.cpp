#include "Menu.h"
#include "../Vars.h"
c_menu::c_menu() {
	m_menu_x = 190;
	m_menu_y = 201;
	m_item_x = 0;
	m_item_y = 0;
	m_open = false;
	m_drag = false;

	m_aimbot_modes = { "plain", "smooth", "silent" };
	m_aimbot_priorities = { "closest to crosshair", "smallest distance" };
	m_health_items = { "off", "text", "bar", "both" };
	m_aimbot_positions = { "head", "body" };
}

c_menu::~c_menu() {
	m_menu_x = 0;
	m_menu_y = 0;
	m_item_x = 0;
	m_item_y = 0;
	m_open = false;
	m_drag = false;

	m_aimbot_modes.clear();
	m_aimbot_priorities.clear();
	m_health_items.clear();
	m_aimbot_positions.clear();

	for (size_t n = 0; n < m_items.size(); n++)
		m_items[n].reset();

	m_items.clear();
}

void c_menu::initialize() {
	add_item(&Vars::Aimbot::Enabled, item_bool);
	add_item(&Vars::Aimbot::Global::AimKey, item_key);
	add_item(&Vars::Aimbot::Global::AimHitbox, item_enum, &m_aimbot_positions);
	add_item(&Vars::Aimbot::Hitscan::AimFOV, item_float);
	add_item(&Vars::Aimbot::Global::AimMethod, item_enum, &m_aimbot_modes);
	add_item(&Vars::Aimbot::Global::SortMethod, item_enum, &m_aimbot_priorities);
	add_item(&Vars::Aimbot::Global::SmoothDuration, item_float_hp);
	add_item(&Vars::Aimbot::Hitscan::WaitForCharge, item_bool);

	Msg("Menu Init!");
}

void c_menu::think() {
	if (GetAsyncKeyState(VK_INSERT))
	{
		m_open = true;
	}


	if (!m_open)
		return;

	static const int item_height = G::Draw.GetFontHeight(EFonts::MENU_TAHOMA);
	static const size_t item_amount = m_items.size();

	handle_drag();

	m_menu_x = G::Util.clamp(m_menu_x, 0, (ScreenWidth() - menu_w));
	m_menu_y = G::Util.clamp(m_menu_y, 0, (ScreenHeight() - (item_height * static_cast<int>(item_amount))));

	m_item_x = (m_menu_x + 2);
	m_item_y = m_menu_y;

	G::Draw.Rect(m_item_x - 2, m_item_y, menu_w, 200, { 15, 150, 150 });
	m_item_y += 5;

	G::Draw.Rect(m_item_x - 2, m_item_y, menu_w, (item_height * static_cast<int>(item_amount)) + 1, { 69, 69, 69, 211 });

	for (size_t n = 0; n < item_amount; n++) {
		m_items[n]->think(m_item_x, m_item_y, m_drag);
		m_items[n]->render(m_item_x, m_item_y);
	}
}

bool c_menu::is_open() const {
	return m_open;
}

void c_menu::add_item(void* var_addr, const itemtype_t type, std::vector<const char*>* items) {
	m_items.push_back(std::make_unique<c_menu_item>(var_addr, type, items));
}

//idk where this originates from lmao.
void c_menu::handle_drag() {
	static screen_t delta = { };
	static bool s_move = false;

	const bool is_held = (gInput.is_held(1) || gInput.is_held(2));

	if ((gInput.cursor_x() > m_menu_x && gInput.cursor_x() < m_menu_x + menu_w && gInput.cursor_y() > m_menu_y && gInput.cursor_y() < m_menu_y + 5) && is_held) {
		m_drag = true;

		if (!s_move) {
			delta.x = (gInput.cursor_x() - m_menu_x);
			delta.y = (gInput.cursor_y() - m_menu_y);
			s_move = true;
		}
	}

	if (m_drag) {
		m_menu_x = (gInput.cursor_x() - delta.x);
		m_menu_y = (gInput.cursor_y() - delta.y);
	}

	if (!is_held) {
		m_drag = false;
		s_move = false;
	}
}