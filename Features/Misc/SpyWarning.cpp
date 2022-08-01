#include "Misc.h"
#include "../Vars.h"

bool CFeatures_Misc::ShouldRun(C_TFPlayer* pLocal)
{
	if (!Vars::Misc::SpyWarning.m_Var || I::EngineVGui->IsGameUIVisible())
		return false;

	if (!pLocal->IsAlive())
		return false;

	if (pLocal->IsTaunting() || pLocal->IsDormant())
		return false;

	return true;
}

void CFeatures_Misc::DrawArrowTo(const Vector& vecFromPos, const Vector& vecToPos)
{
	auto GetClockwiseAngle = [&](const Vector& vecViewAngle, const Vector& vecAimAngle) -> float
	{
		Vector vecAngle = Vector();
		AngleVectors(vecViewAngle, &vecAngle);

		Vector vecAim = Vector();
		AngleVectors(vecAimAngle, &vecAim);

		return -atan2(vecAngle.x * vecAim.y - vecAngle.y * vecAim.x, vecAngle.x * vecAim.x + vecAngle.y * vecAim.y);
	};

	auto MapFloat = [&](float x, float in_min, float in_max, float out_min, float out_max) -> float {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	};


	Vector vecAngleTo = CalcAngle(vecFromPos, vecToPos);
	QAngle vecViewAngle;
	Vector VviewAngle;
	I::EngineClient->GetViewAngles(vecViewAngle);

	AngleVectors(vecViewAngle, &VviewAngle);

	const float deg = GetClockwiseAngle(VviewAngle, vecAngleTo);
	const float xrot = cos(deg - 3.14159265359 / 2);
	const float yrot = sin(deg - 3.14159265359 / 2);

	const float x1 = (20.0f + 5.0f) * xrot;
	const float y1 = (20.0f + 5.0f) * yrot;
	const float x2 = (20.0f + 15.0f) * xrot;
	const float y2 = (20.0f + 15.0f) * yrot;

	constexpr float arrow_angle = DEG2RAD(90.0f);
	constexpr float arrow_lenght = 10.0f;

	const Vector line{ x2 - x1, y2 - y1, 0.0f };
	const float length = line.Length();

	const float fpoint_on_line = arrow_lenght / (atanf(arrow_angle) * length);
	const Vector point_on_line = Vector(x2, y2, 0) + (line * fpoint_on_line * -1.0f);
	const Vector normal_vector{ -line.y, line.x, 0.0f };
	const Vector normal = Vector(arrow_lenght, arrow_lenght, 0.0f) / (length * 2);

	const Vector rotation = normal * normal_vector;
	const Vector left = point_on_line + rotation;
	const Vector right = point_on_line - rotation;


	float cx = static_cast<float>(g_Globals.m_nScreenWidht / 2);
	float cy = static_cast<float>((g_Globals.m_nScreenHeight / 2) - 100);

	G::Draw.Line(cx + x2, cy + y2, cx + left.x, cy + left.y, { 255, 255, 255, 255 });
	G::Draw.Line(cx + x2, cy + y2, cx + right.x, cy + right.y, { 255, 255, 255, 255 });

	float fMap = std::clamp(MapFloat(vecFromPos.DistTo(vecToPos), 400.0f, 200.0f, 0.0f, 1.0f), 0.0f, 1.0f);
	Color HeatColor = { 255, 0, 0, static_cast<byte>(fMap * 255.0f) };

	G::Draw.Line(cx + x2, cy + y2, cx + left.x, cy + left.y, HeatColor);
	G::Draw.Line(cx + x2, cy + y2, cx + right.x, cy + right.y, HeatColor);
}

void CFeatures_Misc::WarningRun(C_TFPlayer* pLocal)
{

	static bool bAnnounce = false;

	if (!ShouldRun(pLocal))
		return;

	Vector vLocalPos = pLocal->WorldSpaceCenter();

	m_vecSpies.clear();

	for (int n = 1; n < (g_Globals.m_nMaxEntities + 1); n++)
	{
		if (n == g_Globals.m_nLocalIndex)
			continue;

		IClientEntity* pEntity = UTIL_EntityByIndex(n);

		if (!pEntity || pEntity->IsDormant())
			continue;

		ClientClass* pCC = pEntity->GetClientClass();

		if (!pCC)
			continue;

		switch (pCC->GetTFClientClass())
		{
			case ETFClientClass::CTFPlayer:
			{
				C_TFPlayer* pEnemy = pEntity->As<C_TFPlayer*>();

				if (!pEnemy || !pEnemy->IsAlive() || pEnemy->m_iClass() != TF_CLASS_SPY || pEnemy->IsCloaked() || pEnemy->IsDormant())
					return;

				Vector vEnemyPos = pEnemy->WorldSpaceCenter();

				if (vLocalPos.DistTo(vEnemyPos) > 400.0f)
					return;

				Vector vAngleToEnemy = CalcAngle(vLocalPos, vEnemyPos);
				float fFovToEnemy = CalcFov(pLocal->EyeAngles(), vAngleToEnemy);

				if (fFovToEnemy < 70.0f)
					return;

				if (Vars::Visual::SpyWarningVisibleOnly.m_Var)
				{
					CGameTrace Trace = {};
					CTraceFilterWorldAndPropsOnly Filter = {};

					UTIL_TraceLine(vEnemyPos, vLocalPos, MASK_SOLID, &Filter, &Trace);

					if (Trace.fraction < 1.0f)
						continue;
				}

				m_vecSpies.push_back(vEnemyPos);

				float flAlpha = RemapValClamped(I::GlobalVars->curtime, 0.0f, 0.1, 0.5f, 0.0f);

				G::Draw.Rect(0, 0, g_Globals.m_nScreenWidht, g_Globals.m_nScreenHeight, { 255, 0, 0, static_cast<byte>(flAlpha * 255.0f) });


				break;
			}
			default:
				break;
		}
	}

	if (Vars::Misc::SpyWarning.m_Var)
	{
		static bool bOldEmpty = m_vecSpies.empty();

		if (m_vecSpies.empty() != bOldEmpty)
		{
			if (!m_vecSpies.empty())
				I::EngineClient->ClientCmd_Unrestricted(_("voicemenu 1 1"));

			bOldEmpty = m_vecSpies.empty();
		}
	}

	if (Vars::Visual::SpyWarningStyle.m_Var == 0)
	{
		if (m_vecSpies.empty())
			return;

		static const int size = 40;
		G::Draw.Texture(((g_Globals.m_nScreenWidht / 2) - (size / 2)), (((g_Globals.m_nScreenHeight / 2) - 100) - (size / 2)), size, size, { 255, 255, 255, 255 }, 8);

		for (const auto& Spy : m_vecSpies)
			DrawArrowTo(vLocalPos, Spy);
	}
	else
	{
		static float flTimer = 0.0f;
		static const float flDuration = 0.1f; //100ms
		static bool bOldEmpty = m_vecSpies.empty();

		if (m_vecSpies.empty() != bOldEmpty)
		{
			if (!m_vecSpies.empty())
				flTimer = I::GlobalVars->curtime;

			bOldEmpty = m_vecSpies.empty();
		}

		if (flTimer)
		{
			float flAlpha = RemapValClamped(I::GlobalVars->curtime - flTimer, 0.0f, flDuration, 0.5f, 0.0f);

			if (flAlpha <= 0.0f)
				flTimer = 0.0f;

			
		}
	}
}