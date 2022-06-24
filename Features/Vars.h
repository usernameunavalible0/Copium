#pragma once

#include "../SDK/SDK.h"

template <class T>
class CVar
{
public:
	T m_Var;
	const wchar_t* m_szDisplayName;
};

namespace Vars
{
	namespace Aimbot
	{
		inline bool Enabled = true;

		namespace Hitscan
		{
			inline int SortMethod = 0; // 0 fov, 1 distance
			inline CVar<float> AimFOV{ 45.0f, L"Aim FOV" };
			inline CVar<bool> ScanHitboxes{ true, L"Scan Body" };
			inline CVar<bool> ScanHead{ true, L"Scan Head" };
			inline CVar<bool> ScanBuildings{ true, L"Scan Buildings" };
			inline CVar<bool> SpectatedSmooth{ true, L"Spectate Smooth" };
			inline CVar<int> SmoothingAmount { 2, L"Smoothing Amount" };
			inline CVar<bool> WaitForCharge{ true, L"Wait for charge" };
			inline CVar<bool> WaitForHeadshot{ false, L"Wait for headshot" };
		}

		namespace Global
		{

			inline CVar<int> AimKey{ 1, L"Active" };
			inline CVar<int> AimMethod{ 2, L"Aim Method" }; // 0 normal, 1 smooth, 2 silent
			inline CVar<bool> AutoShoot{ true, L"Autoshoot?" };
			inline bool WaitForCharge = true;

			inline bool AimPlayers = true;
			inline CVar<bool> AimSentry{ false, L"Aim Sentry" };
			inline CVar<bool> AimDispencer{ false, L"Aim Dispencer" };
			inline CVar<bool> AimTele{ false, L"Aim Teleporter" };

			inline bool IgnoreCloaked = false;
			inline bool IgnoreFriends = false;
			inline bool IgnoreInvunirible = true;
			inline bool IgnoreTaunting = false;
			inline int AimHitbox = 2; // 0 fov, 1 dist, 2 auto
			inline bool bAimLethal = false;

			inline int SortMethod = 2; // 0 fov, 1 dist, 2 auto
			inline bool NoMelee = false;
			inline bool PredictSwing = true;
			inline bool RangeCheck = true;

			inline float SmoothDuration = 0.2f;
			inline float SmoothSimple = 5.0f;

			inline float DrawAimFov = 0.1f;

		}
	}

	namespace ESP
	{
		inline bool Enabled = true;

		namespace Players
		{
			inline bool CustomBoxColor = false;
			inline CVar<bool> Enabled{ true, L"Enabled" };
			inline bool IgnoreTeam = true;
			inline bool ActiveWeapon = true;
			inline bool HealthText = true;
			inline bool Name = true;
			inline bool Box = true;
			inline Color BoxColor = { 50, 100, 255, 255 };
			inline bool Outline = true;
			inline Color OutlineColor = { 242, 194, 0, 255 };
			inline bool OutlineOtherNoVisable = true;
			inline Color OutlineOtherColor = { 0, 0, 205, 255 };
			inline bool HealthBar = true;
		}

		namespace Buildings
		{
			inline bool Enabled = true;
			inline bool IgnoreTeam = false;
			inline bool HealthText = true;
			inline bool Name = true;
			inline bool Info = true;
		}
	}

	namespace Visual
	{
		inline CVar<bool> RemoveVisualRecoil { true, L"Remove Visual Recoil" };
		inline bool Tracers = false;
		inline bool Thirdperson = true;
		inline int CustomFOV = 110;
		inline bool ChangeFov = true;
		inline CVar<bool> RemoveScope { true, L"RemoveScope" };
	}

	namespace Menu
	{
		inline CVar<bool> ToolTips{ false, L"ToolTips" };
		inline Rect_t Position = { 100, 100, 800, 420 };
		inline int TitleBarH = 16;

		inline int SpacingX = 8;
		inline int SpacingY = 4;
		inline int SpacingText = 4;

		inline int CheckBoxW = 14;
		inline int CheckBoxH = 14;
		inline int CheckBoxFillSize = 3;

		inline int ButtonW = 90;
		inline int ButtonH = 20;

		inline int ButtonWSmall = 70;
		inline int ButtonHSmall = 16;

		inline int ComboBoxW = 90;
		inline int ComboBoxH = 14;

		inline int InputBoxW = 90;
		inline int InputBoxH = 14;
		inline int InputColorBoxW = 30;

		namespace Colors
		{
			inline Color WindowBackground = { 30, 30, 30, 255 };
			inline Color TitleBar = { 50, 50, 50, 255 };
			inline Color Text = { 255, 255, 255, 190 };
			inline Color Widget = { 20, 20, 20, 255 };
			inline Color WidgetActive = { 70, 70, 70, 255 };
			inline Color OutlineMenu = { 255, 255, 255, 20 };
		}
	}

	namespace Trigger
	{
		inline int Hitbox = 0; // 1 = head
		inline float Headscale = 0.9f;
		inline bool Active = true;
		inline bool IgnoreCloaked = false;
		inline bool ScopedOnly = false;
		inline bool AutoBackstab = true;
	}

	namespace Misc
	{
		inline CVar<bool> Bunnyhop{ true, L"Bunnyhop" };
		inline CVar<bool> AutoStrafe{ true, L"Autostrafe" };
		inline CVar<bool> BypassPure{ true, L"Bypass Pure" };
	}
}