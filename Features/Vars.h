#pragma once

namespace Vars
{
	namespace Aimbot
	{
		inline bool Enabled = true;

		inline int AimKey = 1;
		inline int AimMethod = 3; // 0 normal, 1 smooth, 2 simple smooth, 3 silent
		inline bool AutoShoot = true;
		inline bool WaitForCharge = true;
		
		inline bool AimPlayers = true;
		inline bool AimBuildings = true;

		inline bool IgnoreCloaked = false;
		inline bool IgnoreFriends = false;
		inline int AimHitbox = 2; // 0 fov, 1 dist, 2 auto

		inline int SortMethod = 2; // 0 fov, 1 dist, 2 auto
		inline float AimFov = 45.0f;
		inline bool NoMelee = false;
		inline bool PredictSwing = true;
		inline bool RangeCheck = true;

		inline float SmoothDuration = 0.2f;
		inline float SmoothSimple = 5.0f;

		inline float DrawAimFov = 0.1f;

	}

	namespace ESP
	{
		inline bool Enabled = true;

		namespace Players
		{
			inline bool Enabled = true;
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
		inline bool RemoveVisualRecoil = true;
		inline bool Tracers = false;
		inline bool Thirdperson = true;
		inline int CustomFOV = 110;
		inline bool ChangeFov = true;
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
		inline bool Bunnyhop = true;
		inline bool AutoStrafe = true;
		inline bool BypassPure = true;
	}
}