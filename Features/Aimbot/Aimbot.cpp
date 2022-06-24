#include "Aimbot.h"
#include "../Vars.h"

void CFeatures_Aimbot::DrawFps()
{
	m_target = -1;

	if (Vars::Aimbot::Global::Enabled.m_Var)
	{
		if (!GetAsyncKeyState(Vars::Aimbot::Global::AimKey.m_Var))
			return;

		m_local_client_num = I::EngineClient->GetLocalPlayer();

		Vector target_pos;
		GetTarget(target_pos);

		if (m_target != -1)
		{
			const QAngle aim_angles = GetAngleToPosition(target_pos, m_localPos);

			QAngle view_angles;
			I::EngineClient->GetViewAngles(view_angles);

			m_delta[1] = (aim_angles.x - view_angles[1]);
			m_delta[0] = (aim_angles.y - view_angles[0]);

			if (Vars::Aimbot::Global::AimMethod.m_Var != 2)
			{
				if (Vars::Aimbot::Global::AimMethod.m_Var == 1)
				{
					ClampAngles(m_delta);

					m_delta[1] /= Vars::Aimbot::Hitscan::SmoothingAmount.m_Var;
					m_delta[0] /= Vars::Aimbot::Hitscan::SmoothingAmount.m_Var;

					ClampAngles(m_delta);
				}

				I::EngineClient->SetViewAngles(m_delta);
			}
		}
	}
}

void CFeatures_Aimbot::CreateNewCommands(CUserCmd* old_cmd, CUserCmd* cur_cmd)
{
	if (m_target != -1)
	{
		if (Vars::Aimbot::Global::AimMethod.m_Var == 2)
		{
			old_cmd->viewangles[0] += m_delta[0];
			old_cmd->viewangles[1] += m_delta[1];
		}

		if (Vars::Aimbot::Global::AutoShoot.m_Var)
		{
			
		}
	}
}

void CFeatures_Aimbot::GetTarget(Vector& world_pos)
{
	float min_distance = FLT_MAX;

	if (const auto& local_client = I::ClientEntityList->GetClientEntity(m_local_client_num))
	{
		int n;
		for (n = 0; n < (g_Globals.m_nMaxEntities + 1); n++)
		{
			const auto& client = UTIL_EntityByIndex(n);

			if (!client || client->IsDormant())
				continue;

			C_TFPlayer* pPlayer = client->As<C_TFPlayer*>();

			if (pPlayer->deadflag() || pPlayer->InLocalTeam())
				continue;

			if (pPlayer->IsInvulnerable())
				continue;


		}
	}
}