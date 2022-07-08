#include "Misc.h"

#include "../Vars.h"

void CFeatures_Misc::Run(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	if (bBunnyhopActive)
		Bunnyhop(pLocal, cmd);

	if (bAutostrafe)
		DoStrafe(pLocal, cmd);
}

std::vector<std::wstring> CFeatures_Misc::get_spectators()
{
	auto local_player = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!local_player)
		return {};

	auto pLocal = local_player->As<C_TFPlayer*>();

	if (!pLocal->IsAlive())
		return {};

	std::vector<std::wstring> list = {};
	for (int i = 1; i < I::EngineClient->GetMaxClients(); i++)
	{
		auto pEnt = I::ClientEntityList->GetClientEntity(i);
		if (!pEnt || pEnt == local_player)
			continue;

		auto pEntity = pEnt->As<C_TFPlayer*>();

		if (pEntity->deadflag())
			continue;

		auto handle = pEntity->GetObserverTarget();

		if (!handle)
			continue;

		auto pHandle = handle->GetRefEHandle();

		if (pLocal == reinterpret_cast<C_TFPlayer*>(I::ClientEntityList->GetClientEntityFromHandle(pHandle)))
		{
			player_info_t info;
			if (I::EngineClient->GetPlayerInfo(pEntity->entindex(), &info))
			{
				auto mode = pEntity->GetObserverMode();
				auto type = mode == OBS_MODE_IN_EYE ?
					L"(1st) " : mode == OBS_MODE_CHASE ?
					L"(3rd) " : mode == OBS_MODE_DEATHCAM ?
					L"(death) " : mode == OBS_MODE_FREEZECAM ?
					L"(freeze) " : mode == OBS_MODE_FIXED ?
					L"(fixed) " : mode == OBS_MODE_ROAMING ?
					L"(roaming) " : L"";

				wchar_t* thing = NULL;
				mbstowcs(&thing[0], info.name, MAX_PLAYER_NAME_LENGTH);
				list.push_back(std::wstring(type) + thing);

			}
		}
	}

	return list;
}

void CFeatures_Misc::Bunnyhop(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	static bool s_bState = false;

	if (cmd->buttons & IN_JUMP)
	{
		if (!s_bState && !pLocal->m_fFlags().IsFlagSet(FL_ONGROUND))
		{
			cmd->buttons &= ~IN_JUMP;
		}
		else if (s_bState)
		{
			s_bState = false;
		}
	}
	else if (!s_bState)
	{
		s_bState = true;
	}
}

void CFeatures_Misc::DoStrafe(C_TFPlayer* pLocal, CUserCmd* cmd)
{
		if (!pLocal->IsAlive()
			|| pLocal->m_fFlags().IsFlagSet(FL_SWIM)
			|| pLocal->m_fFlags().IsFlagSet(FL_ONGROUND))
			return;

		if (cmd->mousedx > 1 || cmd->mousedx < -1)
			cmd->sidemove = cmd->mousedx > 1 ? 450.f : -450.f;
}

