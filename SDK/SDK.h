#pragma once

#include "DrawManager/DrawManager.h"

#define XASSERT(x) if (x) Error(#x)

#define PI 3.14159265358979323846f
#define ZOOM_BASE_DAMAGE 150.0f;

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

class CUtil
{
public:
	void FixMovement(const QAngle& va, CUserCmd* cmd);

	bool IsVisible(const Vector& vStart, const Vector& vEnd);

	Color GetTeamColor(const int nTeam);
	Color GetHealthColor(const int nHealth, const int nMaxHealth);

	IMaterial* CreateMaterial(const char* szVars);

	bool VisPos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to);

	bool VisPosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int nHitbox);

	bool VisPosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int& nHitboxOut);

	void ClampAngle(QAngle& qaAng)
	{
		while (qaAng.x > 89)
			qaAng.x -= 180;

		while (qaAng.x < -89)
			qaAng.x += 180;

		while (qaAng.y > 180)
			qaAng.y -= 360;

		while (qaAng.y < -180)
			qaAng.y += 360;

		while (qaAng.z != 0)
			qaAng.z = 0;
	}

	bool IsHeadshotWeapon(C_TFPlayer* pLocal, C_BaseCombatWeapon* pWep);

	bool IsVisible2(C_TFPlayer* pLocal, C_BaseEntity* pEntity, Vector vStart, Vector vEnd);

	float flGetDistance(Vector vOrigin, Vector vLocalOrigin);

	void AngleNormalize(Vector& v);

	bool CanShoot(C_TFPlayer* pLocal);

	bool GetProjectileInfo(C_TFPlayer* pLocal, float& speed, float& gravity);

	Vector VecViewAngles(CUserCmd* cmd);


	Vector CalcAngleProjectile(const Vector& source, const Vector& destination);

	static std::wstring util_get_vkey_name(const int v_key) {
		switch (v_key) {
		case 0: return L"none";
		case 1: return L"left mouse";
		case 2: return L"right mouse";
		case 3: return L"cancel";
		case 4: return L"middle mouse";
		case 5: return L"xbutton 1";
		case 6: return L"xbutton 2";
		case 46: return L"delete";
		default: break;
		}

		wchar_t wbuff[16] = { L"\0" };

		if (GetKeyNameTextW(MapVirtualKeyW(v_key, 0) << 16, wbuff, 16)) {
			std::wstring key_name = wbuff;

			std::transform(key_name.begin(), key_name.end(), key_name.begin(),
				[](wchar_t c) { return ::towlower(c); });

			return key_name;
		}

		return L"unknown key";
	}


	std::wstring ConvertUtf8ToWide(const std::string_view& str)
	{
		int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), NULL, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), &wstr[0], count);
		return wstr;
	}
public:
	template<typename T>
	inline T clamp(const T val, const T min, const T max) {
		const T ret = (val < min) ? min : val;
		return (ret > max) ? max : ret;
	}
};

struct Priority {
	int Mode = 2; // 0 - Friend, 1 - Ignore, 2 - Default, 3 - Rage, 4 - Cheater
};



namespace G { inline CUtil Util; }