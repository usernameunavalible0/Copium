#pragma once

#include "../SDK/SDK.h"

template <class T>

class CVar {
public:
	CVar(const T v, const char* const name) {
		m_Var = v;
		m_name = name;
	}

	CVar(const T v, const T step, const T min, const T max, const char* const name) {
		m_Var = v;
		m_step = step;
		m_min = min;
		m_max = max;
		m_name = name;
	}

public:
	__inline T get() const {
		return m_Var;
	}

	__inline void set(const T v) {
		m_Var = v;
	}

	__inline const char* get_name() const {
		return m_name;
	}

	__inline T get_step() const {
		return m_step;
	}

	__inline T get_min() const {
		return m_min;
	}

	__inline T get_max() const {
		return m_max;
	}

public:
	T m_Var = { };
	T m_min = { };
	T m_max = { };
	T m_step = { };

	const char* m_name = "";
};

namespace Vars
{
	namespace Aimbot
	{
		inline bool Enabled = true;

		namespace Hitscan
		{
			inline CVar<bool> ZoomedOnly{ false, "Zoomed Only" };
			inline int SortMethod = 1; // 0 fov, 1 distance
			inline CVar<float> AimFOV{ 180.0f, "Aim FOV" };
			inline CVar<bool> ScanHitboxes{ true, "Scan Body" };
			inline CVar<bool> ScanHead{ true, "Scan Head" };
			inline CVar<bool> ScanBuildings{ true, "Scan Buildings" };
			inline CVar<bool> SpectatedSmooth={ true, "Spectate Smooth" };
			inline CVar<int> SmoothingAmount={ 2, "Smoothing Amount" };
			inline CVar<bool> WaitForCharge{ true, "Wait for charge" };
			inline CVar<bool> WaitForHeadshot{ false, "Wait for headshot" };
		}

		namespace Global
		{
			inline CVar<bool> SilentAim{ true, "Enable Silent Aimbot" };
			inline CVar<bool> IgnoreDisguised{ false, "Ignore Disguised" };
			inline CVar<bool> Enabled{ true, "Enabled" };
			inline CVar<int> AimKey{ VK_SHIFT, "Aim Key" };
			inline CVar<bool> AlwaysActive{ false, "Always Active" };
			inline CVar<int> AimMethod{ 0, "Aim Method" }; // 0 normal, 1 smooth, 2 silent
			inline CVar<bool> AutoShoot{ true, "Autoshoot?" };
			inline bool WaitForCharge = true;

			inline bool AimPlayers = true;
			inline CVar<bool> AimSentry{ false, "Aim Sentry" };
			inline CVar<bool> AimDispencer{ false, "Aim Dispencer" };
			inline CVar<bool> AimTele{ false, "Aim Teleporter" };

			inline bool IgnoreCloaked = true;
			inline bool IgnoreFriends = false;
			inline bool IgnoreInvunirible = true;
			inline bool IgnoreTaunting = false;
			inline int AimHitbox = 0; // 0 auto, 1 head, 2 body
			inline bool bAimLethal = false;

			inline int SortMethod = 1; // 0 fov, 1 dist, 2 auto
			inline bool NoMelee = false;
			inline bool PredictSwing = true;
			inline bool RangeCheck = true;

			inline float SmoothDuration = 0.0f;

			inline float DrawAimFov = 0.1f;

		}
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
		inline CVar<bool> RemoveVisualRecoil { true, "Remove Visual Recoil" };
		inline bool Tracers = false;
		inline bool Thirdperson = true;
		inline int CustomFOV = 110;
		inline bool ChangeFov = true;
		inline CVar<bool> RemoveScope { true, "RemoveScope" };
	}

	namespace Menu
	{
		inline CVar<bool> ToolTips{ false, "ToolTips" };
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
		inline CVar<bool> Bunnyhop{ true, "Bunnyhop" };
		inline CVar<bool> AutoStrafe{ true, "Autostrafe" };
		inline CVar<bool> BypassPure{ true, "Bypass Pure" };
	}
}