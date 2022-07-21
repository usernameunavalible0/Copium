#pragma once

#include "../Input/Input.h"

#define menu_w 200

enum itemtype_t {
	item_none = -1,
	item_bool,
	item_float,
	item_float_hp,
	item_int,
	item_enum,
	item_key, //todo
	item_max
};

class c_menu_item {
public:
	c_menu_item();
	c_menu_item(void* addr, const itemtype_t type, std::vector<const char*>* items = nullptr);

public:
	void render(int& x, int& y);
	void think(const int x, const int y, const bool dragged);

private:
	void move_left();
	void move_right();

	bool is_hovered(const int x, const int y) const;

private:
	void* m_var_addr = nullptr;

	itemtype_t m_type = item_none;

	std::vector<const char*>* m_items = nullptr;
};