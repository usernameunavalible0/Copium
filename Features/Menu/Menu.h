#pragma once

#include "MenuItem/MenuItem.h"

class c_menu {
public:
	c_menu();
	~c_menu();

public:
	void initialize();
	void think();

	bool is_open() const;

private:
	void add_item(void* var_addr, const itemtype_t type, std::vector<const char*>* items = nullptr);
	void handle_drag();

private:
	std::vector<std::unique_ptr<c_menu_item>> m_items;

	std::vector<const char*> m_aimbot_modes = { };
	std::vector<const char*> m_aimbot_priorities = { };
	std::vector<const char*> m_health_items = { };
	std::vector<const char*> m_aimbot_positions = { };

	int m_menu_x = 190;
	int m_menu_y = 201;
	int m_item_x = 0;
	int m_item_y = 0;

	bool m_open = false;
	bool m_drag = false;
};

namespace F { inline c_menu Menu; }