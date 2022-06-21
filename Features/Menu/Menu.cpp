#include "Menu.h"
#include "../Vars.h"

#include "InputHelper/InputHelper.h"

void CMenu::DrawTooltip()
{
	if (m_szCurTip.empty() || !Vars::Menu::ToolTips)
		return;

	int w, h;
	//I::VGuiSurface->GetTextSize(G::Draw.m_Fonts.operator[EFonts::MENU_TAHOMA], m_szCurTip.data(), w, h);

	//G::Draw.OutlinedRect(g_InputHelper.m_nMouseX + 13, g_InputHelper.m_nMouseY + 13, w + 4, h + 4, { 0, 0, 0, 255 });
	//G::Draw.Rect(g_InputHelper.m_nMouseX + 14, g_InputHelper.m_nMouseY + 14, w + 2, h + 2, Vars::Menu::Colors::WindowBackground);
	//G::Draw.String(EFonts::MENU_TAHOMA, g_InputHelper.m_nMouseX + 15, g_InputHelper.m_nMouseY + 15, Vars::Menu::Colors::Text, ALIGN_DEFAULT, m_szCurTip.data());
}

void CMenu::Separator()
{
	int x = m_LastWidget.x + m_LastWidget.width + Vars::Menu::SpacingX;
	int y = Vars::Menu::Position.y;
	int w = 0;
	int h = 0;

	G::Draw.Line(x, y, x, (y + Vars::Menu::Position.height - 1), Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x + Vars::Menu::SpacingX;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;
}

bool CMenu::CheckBox(bool& Var, const wchar_t* const szToolTip)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::CheckBoxW;
	int h = Vars::Menu::CheckBoxH;

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (g_InputHelper.IsPressed(VK_LBUTTON)) {
			callback = true;
			Var = !Var;
		}

		G::Draw.Rect(x, y, w, h, Vars::Menu::Colors::WidgetActive);
		m_szCurTip += szToolTip;
	}

	if (Var)
	{
		G::Draw.GradientRect(
			x,
			y,
			x + w,
			y + h,
			Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive,
			false);
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	//G::Draw.String(EFonts::MENU_TAHOMA, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL,);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}