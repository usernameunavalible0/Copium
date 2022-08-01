#include "Misc.h"

#include "../Vars.h"

void CFeatures_Misc::Run(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	if (bBunnyhopActive)
		Bunnyhop(pLocal, cmd);

	if (bAutostrafe)
		DoStrafe(pLocal, cmd);

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

