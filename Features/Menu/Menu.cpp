#include "Menu.h"
#include "Input/Input.h"

#include "../Aimbot/Aimbot.h"
#include "../Vars.h"

//TODO: stop using me
namespace InputHelper
{
	enum State_t
	{
		NONE,
		PRESSED,
		HELD
	};

	State_t GetKey(int16_t key)
	{
		static State_t keys[256] = { NONE };

		if (GetKeyState(key) & 0x8000)
		{
			if (keys[key] == PRESSED)
				keys[key] = HELD;

			else if (keys[key] != HELD)
				keys[key] = PRESSED;
		}

		else keys[key] = NONE;

		return keys[key];
	}

	bool IsPressed(int16_t key)
	{
		return (GetKey(key) == PRESSED);
	}

	bool IsHeld(int16_t key)
	{
		return (GetKey(key) == HELD);
	}

	bool IsPressedAndHeld(int16_t key)
	{
		State_t key_state = GetKey(key);

		static std::chrono::time_point<std::chrono::steady_clock> start[256] = { std::chrono::steady_clock::now() };

		if (key_state == PRESSED) {
			start[key] = std::chrono::steady_clock::now();
			return true;
		}

		if (key_state == HELD && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start[key]).count() > 400)
			return true;

		return false;
	}
}

void CMenu::CreateLists()
{
	static bool bDone = false;

	if (!bDone)
	{
		m_ItemLists =
		{
			//new CItemList
			//{ "ESP",
			//{
			//		new CItemGroup { "Main", {
			//			new CItemBool(Vars::ESP::)
			//		}
			//	}
			//}
			//},

			new CItemList
			{ "Misc",
			{
					new CItemGroup { "Main", {
						new CItemBool(Vars::Misc::Bunnyhop.m_szDisplayName, &Vars::Misc::Bunnyhop.m_Var),
						new CItemBool(Vars::Misc::AutoStrafe.m_szDisplayName, &Vars::Misc::AutoStrafe.m_Var),
					}
				}
			}
			}
		};

		bDone = true;
	}
}

CMenu::~CMenu()
{
	for (auto ItemList : m_ItemLists)
	{
		for (auto ItemGroup : ItemList->m_ItemGroups)
		{
			for (auto Item : ItemGroup->m_Items)
			{
				delete Item;
			}

			delete ItemGroup;
		}

		delete ItemList;
	}
}

void CMenu::Run()
{
	CreateLists();

	if (InputHelper::IsPressed(VK_INSERT))
	{
		m_Open = !m_Open;
		I::VGuiSurface->SetCursorAlwaysVisible(m_Open);
		I::VGuiSurface->UnlockCursor();
	}

	if (m_Open)
	{
		static const Color Text = { 220, 220, 220, 255 };
		static const int ListWidth = 200;
		static const int ListBarH = 30;
		static const Color ListBG = { 40, 40, 40, 255 };
		static const Color ListBar = { 30, 30, 30, 255 };
		static const Color ListOutline = { 100, 100, 100, 255 };
		static const int GroupH = 20;
		static const Color GroupBG = { 35, 35, 35, 255 };
		static const Color GroupHover = { 60, 60, 60, 255 };
		static const Color ItemBG = { 80, 80, 80, 255 };
		static const Color ItemHover = { 225, 177, 44, 255 };

		int x = 0, y = 0;
		I::VGuiSurface->SurfaceGetCursorPos(x, y);

		int nHeight = 650;

		for (size_t nItemList = 0; nItemList < m_ItemLists.size(); nItemList++)
		{
			CItemList* ItemList = m_ItemLists.at(nItemList);

			//main background
			G::Draw.Rect(ListWidth * nItemList, 0, ListWidth, nHeight, ListBG);
			//bar
			G::Draw.Rect(ListWidth * nItemList, 0, ListWidth, ListBarH, ListBar);
			//name
			G::Draw.String(EFonts::MENU_TAHOMA, (ListWidth * nItemList) + ListWidth / 2, ListBarH / 2, Text, TXT_CENTERXY, ItemList->m_Name.c_str());
			//outline
			G::Draw.OutlinedRect(ListWidth * nItemList, 0, ListWidth, nHeight, ListOutline);

			int nDrawnItems = 0;

			for (size_t nItemGroup = 0; nItemGroup < ItemList->m_ItemGroups.size(); nItemGroup++)
			{
				CItemGroup* ItemGroup = ItemList->m_ItemGroups.at(nItemGroup);

				int nGroupX = ListWidth * nItemList;
				int nGroupY = ListBarH + (GroupH * nItemGroup) + (nDrawnItems * GroupH);

				bool bHovered = (x > nGroupX && x < (nGroupX + (ListWidth - 2)) && y > nGroupY && y < (nGroupY + GroupH));

				if (bHovered && InputHelper::IsPressed(VK_LBUTTON))
					ItemGroup->m_Open = !ItemGroup->m_Open;

				//main background
				G::Draw.Rect(nGroupX + 1, nGroupY, ListWidth - 2, GroupH, bHovered ? GroupHover : GroupBG);
				//name
				G::Draw.String(EFonts::MENU_TAHOMA, nGroupX + (ListWidth / 2) + 1, nGroupY + GroupH / 2, Text, TXT_CENTERXY, ItemGroup->m_Name.c_str());

				if (ItemGroup->m_Open)
				{
					for (size_t nItems = 0; nItems < ItemGroup->m_Items.size(); nItems++)
					{
						CItemBase* Item = ItemGroup->m_Items.at(nItems);

						m_OnAimbotFOV = Item->m_Name == "Aim FOV";

						int nItemY = nGroupY + (GroupH * (nItems + 1));
						bool bHovered = (x > nGroupX && x < (nGroupX + (ListWidth - 2)) && y > nItemY && y < (nItemY + GroupH));

						int nDrawX = (nGroupX + 10);
						int nDrawValX = (nGroupX + 160);
						int nDrawY = (nItemY + (GroupH / 2));
						Color DrawCol = (bHovered ? ItemHover : Text);

						switch (Item->e_Type)
						{
						case EItemType::INT:
						{
							CItemInt* Int = reinterpret_cast<CItemInt*>(Item);

							//name
							G::Draw.String(EFonts::MENU_TAHOMA, nDrawX, nDrawY, DrawCol, TXT_CENTERXY, Int->m_Name.c_str());

							if (!Int->m_Aliases.empty())
							{
								if (bHovered)
								{
									if (InputHelper::IsPressedAndHeld(VK_RBUTTON)) {
										Int->m_AliasIdx++;
										Int->m_AliasIdx = std::clamp(Int->m_AliasIdx, 0, static_cast<int32_t>(Int->m_Aliases.size() - 1));
										*reinterpret_cast<int*>(Int->m_Ptr) = Int->m_Aliases.at(Int->m_AliasIdx).first;
									}

									if (InputHelper::IsPressedAndHeld(VK_LBUTTON)) {
										Int->m_AliasIdx--;
										Int->m_AliasIdx = std::clamp(Int->m_AliasIdx, 0, static_cast<int32_t>(Int->m_Aliases.size() - 1));
										*reinterpret_cast<int*>(Int->m_Ptr) = Int->m_Aliases.at(Int->m_AliasIdx).first;
									}
								}

								//value
								G::Draw.String(EFonts::MENU_TAHOMA, nDrawValX, nDrawY, DrawCol, TXT_CENTERXY, Int->m_Aliases.at(Int->m_AliasIdx).second.c_str());
							}

							else
							{
								if (bHovered)
								{
									if (InputHelper::IsPressedAndHeld(VK_RBUTTON)) {
										*reinterpret_cast<int*>(Int->m_Ptr) += 1;
										*reinterpret_cast<int*>(Int->m_Ptr) = std::clamp(*reinterpret_cast<int*>(Int->m_Ptr), Int->m_Min, Int->m_Max);
									}

									if (InputHelper::IsPressedAndHeld(VK_LBUTTON)) {
										*reinterpret_cast<int*>(Int->m_Ptr) -= 1;
										*reinterpret_cast<int*>(Int->m_Ptr) = std::clamp(*reinterpret_cast<int*>(Int->m_Ptr), Int->m_Min, Int->m_Max);
									}
								}

								//value
								G::Draw.String(EFonts::MENU_TAHOMA, nDrawValX, nDrawY, DrawCol, TXT_CENTERXY, reinterpret_cast<wchar_t*>(Int->m_Ptr));
							}

							break;
						}

						case EItemType::FLOAT:
						{
							CItemFloat* Float = reinterpret_cast<CItemFloat*>(Item);

							//name
							G::Draw.String(EFonts::MENU_TAHOMA, nDrawX, nDrawY, DrawCol, TXT_CENTERXY, Float->m_Name.c_str());

							if (bHovered)
							{
								if (InputHelper::IsPressedAndHeld(VK_RBUTTON)) {
									*reinterpret_cast<float*>(Float->m_Ptr) += Float->m_Step;
									*reinterpret_cast<float*>(Float->m_Ptr) = std::clamp(*reinterpret_cast<float*>(Float->m_Ptr), Float->m_Min, Float->m_Max);
								}

								if (InputHelper::IsPressedAndHeld(VK_LBUTTON)) {
									*reinterpret_cast<float*>(Float->m_Ptr) -= Float->m_Step;
									*reinterpret_cast<float*>(Float->m_Ptr) = std::clamp(*reinterpret_cast<float*>(Float->m_Ptr), Float->m_Min, Float->m_Max);
								}
							}

							//value
							G::Draw.String(EFonts::MENU_TAHOMA, nDrawValX, nDrawY, DrawCol, TXT_CENTERXY, reinterpret_cast<wchar_t*>(Float->m_Ptr));

							break;
						}

						case EItemType::BOOL:
						{
							CItemBool* Bool = reinterpret_cast<CItemBool*>(Item);

							//name
							G::Draw.String(EFonts::MENU_TAHOMA, nDrawX, nDrawY, DrawCol, TXT_CENTERXY, Bool->m_Name.c_str());

							if (bHovered && InputHelper::IsPressedAndHeld(VK_LBUTTON))
								*Bool->m_Ptr = !*Bool->m_Ptr;

							//value
							G::Draw.String(EFonts::MENU_TAHOMA, nDrawValX, nDrawY, DrawCol, TXT_CENTERXY, *Bool->m_Ptr ? "On" : "Off");

							break;
						}
						}
					}
				}

				if (ItemGroup->m_Open)
					nDrawnItems += ItemGroup->m_Items.size();
			}
		}
	}

	//gInput.Update();
}

CMenu gMenu;