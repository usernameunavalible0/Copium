#include "Visual.hpp"

#include "../Vars.h"
#include "../Menu/Menu.h"

bool CFeatures_Visual::GetSpectators(C_TFPlayer* pLocal)
{
	m_vecSpectators.clear();

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
			C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

			if (!pPlayer->InLocalTeam())
				return;

			IClientEntity* pObservedPlayer = I::ClientEntityList->GetClientEntityFromHandle(pPlayer->GetObserverTarget()->GetRefEHandle());

			if (pPlayer && !pPlayer->IsAlive() && pObservedPlayer == pLocal)
			{
				std::wstring szMode = L"";

				switch (pPlayer->GetObserverMode()) {
				case OBS_MODE_IN_EYE: { szMode = _(L"1st"); break; }
				case OBS_MODE_CHASE: { szMode = _(L"3rd"); break; }
				default: continue;
				}

				player_info_t PlayerInfo;
				if (I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &PlayerInfo))
					m_vecSpectators.push_back({ G::Util.ConvertUtf8ToWide(PlayerInfo.name), szMode, I::RunGameEngine->GetPlayerFriendsID(pPlayer->entindex()), pPlayer->GetTeamNumber(), pPlayer->entindex()});
				}
			}
		}
	}

	return !m_vecSpectators.empty();
}