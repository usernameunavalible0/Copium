#include "Aimbot.h"

bool CAimbot::CanAmbassadorHeadshot(C_TFPlayer* pLocal)
{
	return I::GlobalVars->curtime - pLocal->GetActiveTFWeapon()->m_flLastFireTime() >= 0.930;//1.0;
}

CAimbot Aimbot;

Vector CAimbot::EstimateAbsVelocity(C_BaseEntity* entity)
{
	typedef void(__thiscall* estimate_abs_velocity_fn)(C_BaseEntity*, Vector&);
	static auto function = g_Pattern.Find("client.dll", "E8 ? ? ? ? F3 0F 10 4D ? 8D 85 ? ? ? ? F3 0F 10 45 ? F3 0F 59 C9 56 F3 0F 59 C0 F3 0F 58 C8 0F 2F 0D ? ? ? ? 76 07") + 0x1;
	static uintptr_t estimate = ((*(PDWORD)(function)) + function + 4);

	estimate_abs_velocity_fn vel = (estimate_abs_velocity_fn)estimate;
	Vector v; vel(entity, v); return v;
}

void CAimbot::Projectile(C_TFPlayer* local_player, C_BaseEntity* entity, C_BaseCombatWeapon* local_weapon, Vector& vec_hitbox)
{
	auto item_index = local_weapon->m_iItemDefinitionIndex();
	float get_speed, get_gravity;
	G::Util.GetProjectileInfo(local_player, get_speed, get_gravity);

	auto distance_to_ground = [&entity]() -> float {
		if (entity->m_fFlags().IsFlagSet(FL_ONGROUND)) return 0.0f;
		auto distance_to_ground = [&entity](Vector origin) -> float
		{
			trace_t ground_trace; Ray_t ray;
			CTraceFilterSimpleList filter = NULL;
			filter.AddEntityToIgnore(entity);
			Vector endpos = origin;

			endpos.z -= 8192;
			ray.Init(origin, endpos);
			I::EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &ground_trace);
			return (8192.0f * ground_trace.fraction);
		};

		Vector origin = entity->GetAbsOrigin();
		float v1 = distance_to_ground(origin + Vector(10.0f, 10.0f, 0.0f));
		float v2 = distance_to_ground(origin + Vector(-10.0f, 10.0f, 0.0f));
		float v3 = distance_to_ground(origin + Vector(10.0f, -10.0f, 0.0f));
		float v4 = distance_to_ground(origin + Vector(-10.0f, -10.0f, 0.0f));
		return min(v1, min(v2, min(v3, v4)));
	};
	if (IsProjectileWeapon(local_player, local_weapon)) {
		bool on_ground = entity->m_fFlags().IsFlagSet(FL_ONGROUND);
		if (local_player->m_iClass() == TF_CLASS_MEDIC || local_player->m_iClass() == TF_CLASS_ENGINEER || local_player->m_iClass() == TF_CLASS_PYRO)
			vec_hitbox = entity->GetHitboxPos(HITBOX_BODY);
		else if (local_player->m_iClass() == TF_CLASS_SOLDIER || local_player->m_iClass() == TF_CLASS_DEMOMAN) {
			vec_hitbox = entity->GetAbsOrigin();
			vec_hitbox[2] += 10.0f;
		}

		auto hitbox_pred = [&local_player, &entity, &on_ground](Vector hitbox, float speed, float gravity, float distance_to_ground) -> Vector {
			Vector result = hitbox, bestpos = result;

			auto vec_velocity = Aimbot.EstimateAbsVelocity(entity);
			auto medianTime = (local_player->Weapon_ShootPosition().DistTo(result) / speed), range = 1.5f,
				currenttime = medianTime - range;

			if (currenttime <= 0.0f) currenttime = 0.01f;

			auto besttime = currenttime, mindelta = 65536.0f; auto maxsteps = 300;
			for (int steps = 0; steps < maxsteps; steps++, currenttime += ((float)(2 * range) / (float)maxsteps)) {
				Vector curpos = result; curpos += vec_velocity * currenttime;

				if (distance_to_ground > 0.0f) {
					curpos.z -= currenttime * currenttime * 400.0f * ((entity->As<C_TFPlayer*>()->m_nPlayerCondEx2() & TF_COND_PARACHUTE_ACTIVE) ? 0.448f : 1.0f);
					if (curpos.z < result.z - distance_to_ground) {
						curpos.z = result.z - distance_to_ground;
					}
				}

				auto rockettime = (local_player->Weapon_ShootPosition().DistTo(curpos) / speed);
				if (fabs(rockettime - currenttime) < mindelta) {
					besttime = currenttime; bestpos = curpos;
					mindelta = fabs(rockettime - currenttime);
				}
			}
			bestpos.z += (400 * besttime * besttime * gravity);
			return bestpos;
		};

		vec_hitbox = hitbox_pred(vec_hitbox, get_speed, get_gravity, distance_to_ground());
	}
}

void FixMove(CUserCmd* pCmd, Vector m_vOldAngles, float m_fOldForward, float m_fOldSidemove)
{
	float deltaView = pCmd->viewangles.y - m_vOldAngles.y;
	float f1;
	float f2;

	if (m_vOldAngles.y < 0.f)
		f1 = 360.0f + m_vOldAngles.y;
	else
		f1 = m_vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * m_fOldForward + cos(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * m_fOldForward + sin(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
}

void CAimbot::MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw) * tmp);
	vector[2] = float(-sin(pitch));
}


float CAimbot::GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = CalcAngle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}


void CAimbot::Run(C_TFPlayer* pLocal, CUserCmd* pCommand)
{
	QAngle m_vOldViewAngle = pCommand->viewangles;
	float m_fOldSideMove = pCommand->sidemove;
	float m_fOldForwardMove = pCommand->forwardmove;

	auto iAimbotIndex = -1;



	if (!Vars::Aimbot::Active.m_Var)
		return;

	if (!GetAsyncKeyState(Vars::Aimbot::AimKey.m_Var))
		return;

	if (!pLocal->IsAlive())
		return;

	if (!pLocal->GetActiveTFWeapon()) // IS THIS EVEN NEEDED?
		return;

	auto id = pLocal->GetActiveTFWeapon()->m_iItemDefinitionIndex(); //This ignores all projectile weapons, doesn't work for strange/killstreak/etc
	if (
#pragma region scout
		id == (int)Scout_t_TheSandman
		|| id == (int)Scout_s_CritaCola
		//|| id == (int)Scout_s_CritaCola
		|| id == (int)Scout_s_TheFlyingGuillotine
		|| id == (int)Scout_s_TheFlyingGuillotineG
		|| id == (int)Scout_s_MadMilk
		|| id == (int)Scout_s_MutatedMilk
		|| id == (int)Scout_t_TheWrapAssassin
#pragma endregion
#pragma region pyro
		|| id == (int)Pyro_s_TheDetonator
		//	|| id == (int)pyroweapons::WPN_FestiveFlaregun
		//	|| id == (int)pyroweapons::WPN_Flaregun
		|| id == (int)Pyro_s_TheScorchShot
#pragma endregion
#pragma region demo
		//|| id == (int)demomanweapons::WPN_FestiveGrenadeLauncher
		//|| id == (int)demomanweapons::WPN_FestiveStickyLauncher
		//|| id == (int)demomanweapons::WPN_GrenadeLauncher
		//|| id == (int)demomanweapons::WPN_IronBomber
		//|| id == (int)demomanweapons::WPN_LochNLoad
		//|| id == (int)demomanweapons::WPN_LoooseCannon
		//|| id == (int)demomanweapons::WPN_QuickieBombLauncher
		//|| id == (int)demomanweapons::WPN_StickyLauncher
		|| id == (int)Demoman_t_TheScotsmansSkullcutter
#pragma endregion
#pragma region heavy
		|| id == (int)Heavy_s_FestiveSandvich
		|| id == (int)Heavy_s_RoboSandvich
		|| id == (int)Heavy_s_Sandvich
		|| id == (int)Heavy_s_TheBuffaloSteakSandvich
		|| id == (int)Heavy_m_BrainCandy
		|| id == (int)Heavy_s_Fishcake
#pragma endregion
#pragma region engineer
		|| id == (int)Engi_p_ConstructionPDA
		|| id == (int)Engi_p_ConstructionPDAR
		|| id == (int)Engi_p_DestructionPDA
		|| id == (int)Engi_m_ThePomson6000
		|| id == (int)Engi_s_FestiveWrangler
		//|| id == (int)engineerweapons::WPN_RescueRanger
		|| id == (int)Engi_s_TheShortCircuit
		|| id == (int)Engi_s_TheWrangler
		|| id == (int)Engi_p_PDA
#pragma endregion
#pragma region medic
		//	|| id == (int)medicweapons::WPN_Blutsauger
		|| id == (int)Medic_m_FestiveCrusadersCrossbow
		|| id == (int)Medic_s_FestiveMediGun
		|| id == (int)Medic_s_TheKritzkrieg
		|| id == (int)Medic_s_MediGun
		|| id == (int)Medic_s_MediGunR
		//	|| id == (int)medicweapons::WPN_Overdose
		|| id == (int)Medic_s_TheQuickFix
		//|| id == (int)medicweapons::WPN_SyringeGun
		|| id == (int)Medic_s_TheVaccinator

#pragma endregion
#pragma region sniper
		|| id == (int)Sniper_m_TheFortifiedCompound
		|| id == (int)Sniper_s_FestiveJarate
		|| id == (int)Sniper_m_FestiveHuntsman
		|| id == (int)Sniper_m_TheHuntsman
		|| id == (int)Sniper_s_Jarate
#pragma endregion
#pragma region spy
		|| id == (int)Spy_s_TheApSapG
		|| id == (int)Spy_d_DisguiseKitPDA
		|| id == (int)Spy_s_TheRedTapeRecorder
		|| id == (int)Spy_s_TheRedTapeRecorderG
		|| id == (int)Spy_s_Sapper
		|| id == (int)Spy_s_SapperR
		|| id == (int)Spy_s_FestiveSapper
#pragma endregion
		)
		return;


	auto pEntity = I::ClientEntityList->GetClientEntity(GetBestTarget(pLocal, pCommand))->As<C_BaseEntity*>();

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	auto pWep = pLocal->GetActiveWeapon();
	if (Vars::Aimbot::WaitForCharge.m_Var) //Advanced Ambassador Wait For Charge!
		if (pWep->m_iItemDefinitionIndex() == Spy_m_TheAmbassador || pWep->m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador)
			if (!CanAmbassadorHeadshot(pLocal)) return;


	if (iBestHitbox == -1)
		return;
	Vector vEntity = pEntity->GetHitboxPos(iBestHitbox);

	Vector vLocal = pLocal->Weapon_ShootPosition();

	QAngle vAngs;


	C_BaseCombatWeapon* pWeapon = pLocal->GetActiveWeapon();
	Projectile(pLocal, pEntity, pWeapon, vEntity);
	VectorAngles((vEntity - vLocal), vAngs);

	G::Util.ClampAngle(vAngs);

	iAimbotIndex = pEntity->entindex();


	if (Vars::Aimbot::AutoShoot.m_Var)
	{
		pCommand->buttons |= IN_ATTACK;
	}

	if (G::Util.CanShoot(pLocal) && (pCommand->buttons & IN_ATTACK)) {
		//if (GAME_TF2)
		//{

		if (Vars::Aimbot::AimMethod.m_Var == 0)
		{
			pCommand->viewangles = vAngs;
			I::EngineClient->SetViewAngles(pCommand->viewangles);
		}

		if (Vars::Aimbot::SmoothAmount.m_Var > 0.0 && Vars::Aimbot::AimMethod.m_Var == 1)
		{
			QAngle vecDelta = vAngs - pCommand->viewangles;
			G::Util.ClampAngle(vecDelta);
			pCommand->viewangles += vecDelta / Vars::Aimbot::SmoothAmount.m_Var;
			I::EngineClient->SetViewAngles(pCommand->viewangles);
		}

		if (Vars::Aimbot::AimMethod.m_Var == 2)
		{
			G::Util.FixMovement(vAngs, pCommand);
			pCommand->viewangles = vAngs;
		}

	}

	//Vector n_QOldViewAngle;
	//AngleVectors(m_vOldViewAngle, &n_QOldViewAngle); // LAST TIME IM DOIN THIS SHIT
	//FixMove(pCommand, n_QOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
}

//---------------------------------------//
//										 //
//				    Bruh				 //
//										 //
//---------------------------------------//


int CAimbot::GetBestTarget(C_TFPlayer* pLocal, CUserCmd* pCommand)
{
	auto iAimbotIndex = -1;
	int iBestTarget = -1;
	int bestTarget = -1;					 //this num could be smaller 
	float flDistToBest = 99999.f;
	double minimalDistance = 99999.0;

	Vector vLocal = pLocal->Weapon_ShootPosition();

	for (int i = 1; i <= I::EngineClient->GetMaxClients(); i++)
	{
		if (i == g_Globals.m_nLocalIndex)
			continue;

		IClientEntity* iEntity = I::ClientEntityList->GetClientEntity(i);
		auto pEntity = iEntity->As<C_BaseEntity*>();

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive())
			continue;

		if (pEntity->GetTeamNumber() == pLocal->GetTeamNumber())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->GetHitboxPos(iBestHitbox);

		//if (!gCvars.PlayerMode[i]) WAT
			//continue; IS DIS


		//if (GAME_TF2)
		//{
		auto weap = pLocal->GetActiveWeapon()->m_iItemDefinitionIndex();

		if (pEntity->As<C_TFPlayer*>()->IsInvulnerable())
			continue;

		if (pEntity->As<C_TFPlayer*>()->IsCloaked() && Vars::Aimbot::IgnoreCloaked.m_Var)
			continue;

		if (pEntity->As<C_TFPlayer*>()->InCond(TF_COND_DISGUISED) && Vars::Aimbot::IgnoreDisguised.m_Var)
			continue;

		auto pWep = pLocal->GetActiveWeapon();
		auto pClass = pWep->m_iItemDefinitionIndex();
		auto urmomgay = pClass == Demoman_t_TheEyelander || pClass == Demoman_t_FestiveEyelander || pClass == Demoman_t_NessiesNineIron || pClass == Demoman_t_TheScotsmansSkullcutter || pClass == Demoman_t_HorselessHeadlessHorsemannsHeadtaker;
		if (pWep->GetSlot() == 2 && !urmomgay)
			minimalDistance = 8.4;

		if (pClass == Demoman_t_TheEyelander || pClass == Demoman_t_FestiveEyelander || pClass == Demoman_t_NessiesNineIron || pClass == Demoman_t_TheScotsmansSkullcutter || pClass == Demoman_t_HorselessHeadlessHorsemannsHeadtaker)
			minimalDistance = 12.0;

		if (pLocal->m_iClass() == TF_CLASS_PYRO && (pClass == Pyro_m_TheBackburner || pClass == Pyro_m_TheDegreaser || pClass == Pyro_m_FestiveBackburner || pClass == Pyro_m_FestiveFlameThrower || pClass == Pyro_m_FlameThrower || pClass == Pyro_m_ThePhlogistinator || pClass == Pyro_m_TheRainblower))
			minimalDistance = 17.0;

		if (pLocal->m_iClass() == TF_CLASS_SNIPER && pLocal->GetActiveWeapon()->GetSlot() == 0)
		{
			float damage = pLocal->GetActiveWeapon()->m_flChargedDamage();
			if (Vars::Aimbot::ZoomedOnly.m_Var)// || gCvars.aimbot.waitforcharge)
			{
				if (!(pLocal->IsZoomed()))
					return -1;

				if (damage < 10.f)
					return -1;


			}
			if (Vars::Aimbot::WaitForCharge.m_Var) //This isn't the best code but it works for raging.
				if (Vars::Aimbot::WaitForCharge.m_Var && ZOOM_BASE_DAMAGE + damage < pEntity->GetHealth())//<
					return -1;
		}
		//}

		Vector bruh;
		AngleVectors(pCommand->viewangles, &bruh);
		float flFOV = GetFOV(bruh, vLocal, vEntity);
		float distance = G::Util.flGetDistance(vEntity, pLocal->Weapon_ShootPosition());

		if (distance < minimalDistance)//gCvars.aimbot.fov)
		{
			if (flFOV < flDistToBest && flFOV < Vars::Aimbot::AimFOV.m_Var)
			{
				//if (gCvars.PlayerMode[i] == 2)
					//return i;
				//flDistToBest = flDistToTarget;
				flDistToBest = flFOV;
				iAimbotIndex = i;
				bestTarget = i;
			}
		}

		//if (gCvars.PlayerMode[i] == 2) //always aim at rage targets first
			//return i;
	}


	return bestTarget;
}

int CAimbot::GetBestHitbox(C_TFPlayer* pLocal, C_BaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (Vars::Aimbot::AimHitbox.m_Var == 0)
	{
		if (G::Util.IsHeadshotWeapon(pLocal, pLocal->GetActiveTFWeapon()))
			iBestHitbox = HITBOX_HEAD;
		else
			iBestHitbox = HITBOX_BODY;
		for (int i = iBestHitbox; i < HITBOX_RIGHT_FOREARM; i++) // int i equals prioritized hitbux, so above we check the weapon so it prioritizes the proper hitbox.
		{
			if (G::Util.IsVisible(pLocal->Weapon_ShootPosition(), pEntity->GetHitboxPos(i)))
				return i;
		}
	}
	else if (Vars::Aimbot::AimHitbox.m_Var == 1)
		iBestHitbox = HITBOX_HEAD;
	else if (Vars::Aimbot::AimHitbox.m_Var == 2)
		iBestHitbox = HITBOX_BODY;


	if (pEntity->GetHitboxPos(iBestHitbox).IsZero())
		return -1;

	if (!G::Util.IsVisible(pLocal->Weapon_ShootPosition(), pEntity->GetHitboxPos(iBestHitbox)))
		return -1;

	return iBestHitbox;
}