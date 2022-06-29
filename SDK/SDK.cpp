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
		if (pWep->m_iItemDefinitionIndex() != Sniper_m_TheHuntsman && pWep->m_iItemDefinitionIndex() != Sniper_m_TheFortifiedCompound)
			return true;

	if (pWep->m_iItemDefinitionIndex() == Spy_m_TheAmbassador || pWep->m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador)
		return true;

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