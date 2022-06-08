#include "triggerbot.hpp"
#include "../Vars.h"

void CFeatures_Triggerbot::Run(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	if (Vars::Trigger::Active)
		Triggerbot(pLocal, cmd);
}

void CFeatures_Triggerbot::Triggerbot(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	trace_t trace;
	Ray_t ray;

	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pLocal);
	

	QAngle viewAngles = {};
	Vector vecForward = {};
	I::EngineClient->GetViewAngles(viewAngles);
	AngleVectors(viewAngles, &vecForward);

	Vector start = pLocal->EyePosition();
	Vector end = vecForward * 8192 + start;

	ray.Init(start, end);

	I::EngineTrace->TraceRay(ray, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

	C_BaseCombatWeapon* pWep = pLocal->GetActiveTFWeapon();

	if (Vars::Trigger::AutoBackstab)
	{
		if ((pLocal->m_iClass() == TF_CLASS_SPY) && (pWep->m_bReadyToBackstab() == true))
			cmd->buttons |= IN_ATTACK;
	}

	if (trace.m_pEnt->IsPlayer()) {
		if (trace.m_pEnt->GetTeamNumber() == pLocal->GetTeamNumber()) {
			return;
		}





		if (Vars::Trigger::Hitbox == 0 ? true : trace.hitbox == HITBOX_HEAD) {
			if (!(pLocal->m_iClass() == TF_CLASS_SPY))
			cmd->buttons |= IN_ATTACK;
		}
	}
}