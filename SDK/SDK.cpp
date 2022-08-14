#include "SDK.h"

void CUtil::FixMovement(const QAngle& va, CUserCmd* cmd)
{
	const Vector vMove = { cmd->forwardmove, cmd->sidemove, cmd->upmove };

	QAngle vAng;
	VectorAngles(vMove, vAng);

	const float flSpeed = FastSqrt(vMove.x * vMove.x + vMove.y * vMove.y);

	float s, c;
	SinCos(DEG2RAD(va.y - cmd->viewangles.y + vAng.y), &s, &c);

	cmd->sidemove = (s * flSpeed);
	cmd->forwardmove = (c * flSpeed);
}

bool CUtil::IsVisible(const Vector& vStart, const Vector& vEnd)
{
	trace_t tr;
	CTraceFilterWorldAndPropsOnly filter;
	UTIL_TraceLine(vStart, vEnd, (MASK_SHOT | CONTENTS_GRATE), &filter, &tr);

	return !(tr.DidHit());
}

Vector CUtil::CalcAngleProjectile(const Vector& source, const Vector& destination) {
	Vector angles = Vector(0.0f, 0.0f, 0.0f);
	Vector delta = (source - destination);
	float fHyp = FastSqrt((delta.x * delta.x) + (delta.y * delta.y));

	angles.x = (atanf(delta.z / fHyp) * M_RADPI);
	angles.y = (atanf(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}

Vector CUtil::VecViewAngles(CUserCmd* cmd)
{
	Vector ang;
	AngleVectors(cmd->viewangles, &ang);
	return ang;
}

bool CUtil::GetProjectileInfo(C_TFPlayer* pLocal, float& speed, float& gravity)
{
	switch (pLocal->GetActiveTFWeapon()->m_iItemDefinitionIndex())
	{
	case Soldier_m_RocketLauncher:
	case Soldier_m_RocketLauncherR:
	case Soldier_m_TheBlackBox:
	case Soldier_m_TheCowMangler5000:
	case Soldier_m_TheOriginal:
	case Soldier_m_FestiveRocketLauncher:
	case Soldier_m_TheBeggarsBazooka:
	case Soldier_m_SilverBotkillerRocketLauncherMkI:
	case Soldier_m_GoldBotkillerRocketLauncherMkI:
	case Soldier_m_RustBotkillerRocketLauncherMkI:
	case Soldier_m_BloodBotkillerRocketLauncherMkI:
	case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
	case Soldier_m_DiamondBotkillerRocketLauncherMkI:
	case Soldier_m_SilverBotkillerRocketLauncherMkII:
	case Soldier_m_GoldBotkillerRocketLauncherMkII:
	case Soldier_m_FestiveBlackBox:
	case Soldier_m_TheAirStrike:
	case Soldier_m_WoodlandWarrior:
	case Soldier_m_SandCannon:
	case Soldier_m_AmericanPastoral:
	case Soldier_m_SmalltownBringdown:
	case Soldier_m_ShellShocker:
	case Soldier_m_AquaMarine:
	case Soldier_m_Autumn:
	case Soldier_m_BlueMew:
	case Soldier_m_BrainCandy:
	case Soldier_m_CoffinNail:
	case Soldier_m_HighRollers:
	case Soldier_m_Warhawk: {
		speed = 1100.0f;
		gravity = 0.0f;
		break;
	}
	case Soldier_m_TheDirectHit: {
		speed = 1980.0f;
		gravity = 0.0f;
		break;
	}

	case Soldier_m_TheLibertyLauncher: {
		speed = 1540.0f;
		gravity = 0.0f;
		break;
	}
	case Demoman_m_GrenadeLauncher:
	case Demoman_m_GrenadeLauncherR:
	case Demoman_m_FestiveGrenadeLauncher:
	case Demoman_m_TheIronBomber:
	case Demoman_m_Autumn:
	case Demoman_m_MacabreWeb:
	case Demoman_m_Rainbow:
	case Demoman_m_SweetDreams:
	case Demoman_m_CoffinNail:
	case Demoman_m_TopShelf:
	case Demoman_m_Warhawk:
	case Demoman_m_ButcherBird: {
		speed = 1217.0f;
		gravity = 0.4f;
		break;
	}
	case Soldier_s_TheRighteousBison:
	case Engi_m_ThePomson6000: {
		speed = 1200.0f;
		gravity = 0.0f;
		break;
	}
	case Demoman_m_TheLooseCannon: {
		speed = 1453.9f;
		gravity = 0.4f;
		break;
	}
	case Demoman_m_TheLochnLoad: {
		speed = 1513.3f;
		gravity = 0.4f;
		break;
	}
	case Engi_m_TheRescueRanger:
	case Medic_m_FestiveCrusadersCrossbow:
	case Medic_m_CrusadersCrossbow: {
		speed = 2400.0f;
		gravity = 0.2f;
		break;
	}
	case Pyro_m_DragonsFury: {
		speed = 3000.0f;
		gravity = 0.0f;
		break;
	}
	case Pyro_m_FlameThrower:
	case Pyro_m_ThePhlogistinator:
	case Pyro_m_TheBackburner:
	case Pyro_m_TheDegreaser: {
		speed = 1000.0f;
		gravity = 0.0f;
		break;
	}
	case Pyro_s_TheDetonator:
	case Pyro_s_TheFlareGun:
	case Pyro_s_FestiveFlareGun:
	case Pyro_s_TheScorchShot: {
		speed = 2000.0f;
		gravity = 0.3f;
		break;
	}
	case Pyro_s_TheManmelter: {
		speed = 3000.0f;
		gravity = 0.2f;
		break;
	}
	case Medic_m_SyringeGun:
	case Medic_m_SyringeGunR:
	case Medic_m_TheBlutsauger:
	case Medic_m_TheOverdose: {
		speed = 1000.0f;
		gravity = 0.2f;
		break;
	}

	case Sniper_m_TheHuntsman:
	case Sniper_m_FestiveHuntsman:
	case Sniper_m_TheFortifiedCompound: {
		float charge = (I::GlobalVars->curtime - pLocal->GetActiveTFWeapon()->m_flChargeBeginTime());
		speed = ((fminf(fmaxf(charge, 0.0f), 1.0f) * 800.0f) + 1800.0f);
		gravity = ((fminf(fmaxf(charge, 0.0f), 1.0f) * -0.4f) + 0.5f);
		break;
	}
	default:
		speed = 0.0f;
		gravity = 0.0f;
		return false;
	}
	return true;
}

bool CUtil::VisPos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to)
{
	trace_t tr;
	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pSkip);
	UTIL_TraceLine(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &tr);
	return ((tr.m_pEnt && tr.m_pEnt == pEntity) || tr.fraction > 0.99f);
}

bool CUtil::VisPosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int nHitbox)
{
	trace_t tr;
	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pSkip);
	UTIL_TraceLine(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &tr);
	return (tr.m_pEnt && tr.m_pEnt == pEntity && tr.hitbox == nHitbox);
}

float CUtil::flGetDistance(Vector vOrigin, Vector vLocalOrigin)
{
	Vector vDelta = vOrigin - vLocalOrigin;

	float m_fDistance = sqrt(vDelta.Length());

	if (m_fDistance < 1.0f)
		return 1.0f;

	return m_fDistance;
}

void CUtil::AngleNormalize(Vector& v)
{
	for (auto i = 0; i < 3; i++)
	{
		if (v[i] < -180.0f) v[i] += 360.0f;
		if (v[i] > 180.0f) v[i] -= 360.0f;
	}
}

bool CUtil::CanShoot(C_TFPlayer* pLocal)
{
	C_BaseCombatWeapon* pWep = pLocal->GetLastWeapon();

	if (!pWep)
		return false;

	if (!pLocal->IsAlive())
		return false;

	return (pWep->GetWeaponIdleTime() < pLocal->m_nTickBase() * I::GlobalVars->interval_per_tick);
}

bool CUtil::VisPosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int& nHitboxOut)
{
	trace_t trace;
	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pSkip);
	UTIL_TraceLine(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

	if (trace.m_pEnt && trace.m_pEnt == pEntity) {
		nHitboxOut = trace.hitbox;
		return true;
	}

	return false;
}

Color CUtil::GetTeamColor(const int nTeam)
{
	if (nTeam == 2)
		return COLOR_TF_RED;
	else if (nTeam == 3)
		return COLOR_TF_BLUE;
	else
		return COLOR_TF_SPECTATOR;
}

Color CUtil::GetHealthColor(const int nHealth, const int nMaxHealth)
{
	if (nHealth > nMaxHealth)
		return COLOR_BLUE;

	const int nHP = Max(0, Min(nHealth, nMaxHealth));

	const uint32 nR = static_cast<uint32>(Min((510 * (nMaxHealth - nHP)) / nMaxHealth, 200));
	const uint32 nG = static_cast<uint32>(Min((510 * nHP) / nMaxHealth, 200));

	return { static_cast<byte>(nR), static_cast<byte>(nG), 0u, 255u };
}

IMaterial* CUtil::CreateMaterial(const char* szVars)
{
	static int nCreated = 0;

	char szOut[512];
	sprintf_s(szOut, sizeof(szOut), "pol_mat_%i.vmt", nCreated++);

	char szMat[512];
	sprintf_s(szMat, sizeof(szMat), szVars);

	KeyValues* pVals = new KeyValues;
	g_KeyValues.Initialize(pVals, (char*)szOut);
	g_KeyValues.LoadFromBuffer(pVals, szOut, szMat);

	IMaterial* pMaterial = I::MaterialSystem->CreateMaterial(szOut, pVals);
	pMaterial->AddRef();

	return pMaterial;
}

bool CUtil::IsHeadshotWeapon(C_TFPlayer* pLocal, C_BaseCombatWeapon* pWep)
{
	if (pWep->GetSlot() == 0 && pLocal->m_iClass() == TF_CLASS_SNIPER)
	{
		if (pWep->m_iItemDefinitionIndex() != Sniper_m_TheHuntsman && pWep->m_iItemDefinitionIndex() != Sniper_m_TheFortifiedCompound)
			return true;
	}
	else if (pWep->GetSlot() == 0 && pLocal->m_iClass() == TF_CLASS_SPY)
	{
		if (pWep->m_iItemDefinitionIndex() == Spy_m_TheAmbassador || pWep->m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador)
			return true;
	}

	return false;
}

bool CUtil::IsVisible2(C_TFPlayer* pLocal, C_BaseEntity* pEntity, Vector vStart, Vector vEnd)
{
	trace_t Trace;
	Ray_t Ray;			 // the future of variable naming
	CTraceFilterSimpleList Filter = NULL;

	Filter.AddEntityToIgnore(pLocal);

	Ray.Init(vStart, vEnd);

	I::EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity || Trace.fraction == 1.0);
}