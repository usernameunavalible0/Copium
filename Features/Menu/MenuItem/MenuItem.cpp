#include "MenuItem.h"
#include "../../Vars.h"

c_menu_item::c_menu_item() {
	m_var_addr = nullptr;
	m_type = item_none;
	m_items = nullptr;
}

c_menu_item::c_menu_item(void* addr, const itemtype_t type, std::vector<const char*>* items) {
	m_var_addr = addr;
	m_type = type;
	m_items = items;
}

void c_menu_item::render(int& x, int& y) {
	const Color clr = is_hovered(x, y) ? Color(15, 150, 150) : Color(255, 255, 255);

	switch (m_type) {
	case item_bool: {
		CVar<bool>* var = reinterpret_cast<CVar<bool>*>(m_var_addr);

		G::Draw.String(EFonts::MENU_TAHOMA, x, y, clr, TXT_DEFAULT, L"%ws:", var->get_name());
		G::Draw.String(EFonts::MENU_TAHOMA, x + (menu_w - 3), y, clr, TXT_LEFT, var->get() ? L"on" : L"off");
		break;
	}
	case item_int: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		G::Draw.String(EFonts::MENU_TAHOMA, x, y, clr, TXT_DEFAULT, L"%ws:", var->get_name());
		G::Draw.String(EFonts::MENU_TAHOMA, x + (menu_w - 3), y, clr, TXT_LEFT, L"%i", var->get());
		break;
	}
	case item_enum: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		G::Draw.String(EFonts::MENU_TAHOMA, x, y, clr, TXT_DEFAULT, L"%ws:", var->get_name());
		G::Draw.String(EFonts::MENU_TAHOMA, x + (menu_w - 3), y, clr, TXT_LEFT, m_items->at(var->get()));
		break;
	}
	case item_float: {
		CVar<float>* var = reinterpret_cast<CVar<float>*>(m_var_addr);

		G::Draw.String(EFonts::MENU_TAHOMA, x, y, clr, TXT_DEFAULT, L"%ws:", var->get_name());
		G::Draw.String(EFonts::MENU_TAHOMA, x + (menu_w - 3), y, clr, TXT_LEFT, L"%.1f", var->get());
		break;
	}
	case item_float_hp: {
		CVar<float>* var = reinterpret_cast<CVar<float>*>(m_var_addr);

		G::Draw.String(EFonts::MENU_TAHOMA, x, y, clr, TXT_DEFAULT, L"%ws:", var->get_name());
		G::Draw.String(EFonts::MENU_TAHOMA, x + (menu_w - 3), y, clr, TXT_LEFT, L"%.4f", var->get());
		break;
	}
	case item_key: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		G::Draw.String(EFonts::MENU_TAHOMA, x, y, clr, TXT_DEFAULT, L"%ws:", var->get_name());
		G::Draw.String(EFonts::MENU_TAHOMA, x + (menu_w - 3), y, clr, TXT_LEFT, G::Util.util_get_vkey_name(var->get()).c_str());
		break;
	}
	default:
		break;
	}

	y += G::Draw.GetFontHeight(EFonts::MENU_TAHOMA);
}

void c_menu_item::think(const int x, const int y, const bool dragged) {
	if (is_hovered(x, y) && !dragged) {
		if (m_type == item_key) {
			move_left(); //hackhack...
		}
		else {
			if (gInput.is_pressed(1))
				move_left();
			else if (gInput.is_pressed(2))
				move_right();
		}
	}
}

void c_menu_item::move_left() {
	switch (m_type) {
	case item_bool: {
		CVar<bool>* var = reinterpret_cast<CVar<bool>*>(m_var_addr);

		var->set(!var->get());
		break;
	}
	case item_int: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		var->set(G::Util.clamp((var->get() - var->get_step()), var->get_min(), var->get_max()));
		break;
	}
	case item_enum: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		var->set(G::Util.clamp((var->get() - 1), 0, (static_cast<int>(m_items->size()) - 1)));
		break;
	}
	case item_float:
	case item_float_hp: {
		CVar<float>* var = reinterpret_cast<CVar<float>*>(m_var_addr);

		var->set(G::Util.clamp((var->get() - var->get_step()), var->get_min(), var->get_max()));
		break;
	}
	case item_key: {
		int v_key;
		if (gInput.is_any_key_pressed(v_key) && (v_key != 45)) //don't allow insert...
			reinterpret_cast<CVar<int>*>(m_var_addr)->set((v_key == 27) ? 0 : v_key); //esc = 0 / none

		break;
	}
	default:
		break;
	}
}

void c_menu_item::move_right() {
	switch (m_type) {
	case item_bool: {
		CVar<bool>* var = reinterpret_cast<CVar<bool>*>(m_var_addr);

		var->set(!var->get());
		break;
	}
	case item_int: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		var->set(G::Util.clamp((var->get() + var->get_step()), var->get_min(), var->get_max()));
		break;
	}
	case item_enum: {
		CVar<int>* var = reinterpret_cast<CVar<int>*>(m_var_addr);

		var->set(G::Util.clamp((var->get() + 1), 0, (static_cast<int>(m_items->size()) - 1)));
		break;
	}
	case item_float:
	case item_float_hp: {
		CVar<float>* var = reinterpret_cast<CVar<float>*>(m_var_addr);

		var->set(G::Util.clamp((var->get() + var->get_step()), var->get_min(), var->get_max()));
		break;
	}
	case item_key: {
		//this logic has bee written in move_left.
		break;
	}
	default:
		break;
	}
}

bool c_menu_item::is_hovered(const int x, const int y) const {
	static const int s_item_height = (G::Draw.GetFontHeight(EFonts::MENU_TAHOMA) + 1);
	return (gInput.cursor_x() > x && gInput.cursor_x() < (x + menu_w) && gInput.cursor_y() > y && gInput.cursor_y() < (y + s_item_height));
}