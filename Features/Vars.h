#pragma once

#include "../SDK/SDK.h"

template <class T>
class CVar {
public:
	T m_Var;
	const char* m_szDisplayName;
};

namespace Vars
{
	namespace Aimbot
	{
		inline CVar<bool> Active{ true, "Enabled" };
		inline CVar<int> AimKey{ VK_SHIFT, "Key to toggle Aimbot" };
		inline CVar<bool> WaitForCharge{ true, "Ambassador wait for charge" };
		inline CVar<int> AimMethod{ 2, "0 = Plain, 1 = Smooth, 2 = Silent" };
		inline CVar<float> SmoothAmount{ 0.0, "Aimbot Smoothing amount" };
		inline CVar<bool> AutoShoot{ true, "Should the Aimbot shoot for you?" };
		inline CVar<bool> IgnoreTeam{ false, "Should the Aimbot aim at both teams?" };
		inline CVar<bool> IgnoreCloaked{ true, "Should the Aimbot aim at cloaked spys?" };
		inline CVar<bool> IgnoreDisguised{ true, "Should the Aimbot aim at disguised spys?" };
		inline CVar<bool> ZoomedOnly{ true, "Scoped Only?" };
		inline CVar<int> AimFOV{ 180, "Aimbot Field of View" };
		inline CVar<int> AimHitbox{ 0, "0 = Auto, 1 = Head, 2 = Body" };
	}

	namespace ESP
	{
		inline CVar<bool> Enabled{ true, "ESP" };

		namespace Players
		{
			inline bool CustomBoxColor = false;
			inline CVar<bool> Enabled{ true, "Enabled" };
			inline bool IgnoreTeam = true;
			inline bool ActiveWeapon = true;
			inline bool HealthText = true;
			inline bool Name = true;
			inline bool Box = false;
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
		inline CVar<bool> RemoveVisualRecoil { true, "Remove Visual Recoil" };
		inline bool Tracers = false;
		inline bool Thirdperson = true;
		inline int CustomFOV = 110;
		inline bool ChangeFov = true;
		inline CVar<bool> RemoveScope { true, "RemoveScope" };
		inline CVar<bool> SpyWarningVisibleOnly{ false, "Spy Warning visual only?" };
		inline CVar<int> SpyWarningStyle{ 1, "Spy warning style" };

	}

	namespace Menu
	{
		inline CVar<bool> ToolTips{ false, "ToolTips" };
		inline Rect_t Position = { 100, 100, 800, 420 };
		inline int TitleBarH = 16;
		inline bool Enabled = true;

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
		inline CVar<bool> Detonate{ true, "Auto Detonate" };
		inline CVar<float> DRadiusScale{ 1100.f, "Detonate Radius" };
	}

	namespace Misc
	{
		inline CVar<bool> Bunnyhop{ true, "Bunnyhop" };
		inline CVar<bool> AutoStrafe{ true, "Autostrafe" };
		inline CVar<bool> BypassPure{ true, "Bypass Pure" };
		inline CVar<bool> AutoDisguise{ true, "Auto Disguise" };
		inline CVar<bool> SpyWarning{ true, "Spy Warning" };
	}
}