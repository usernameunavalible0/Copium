#include "Aimbot.h"
#include "../Vars.h"

bool CFeatures_Aimbot::CanAmbassadorHeadshot(C_TFPlayer* pLocal)
{
	return I::GlobalVars->curtime - pLocal->GetActiveTFWeapon()->m_flLastFireTime() >= 0.930;
}

Vector CFeatures_Aimbot::EliminateAbsVelocity(C_BaseEntity* entity)
{
	
}

void CFeatures_Aimbot::Projectile(C_TFPlayer* pLocal, C_BaseEntity* entity, C_BaseCombatWeapon* pWeapon, int& vec_hitbox)
{
	auto item_index = pWeapon->m_iItemDefinitionIndex();
	auto get_speed = [&pLocal, &pWeapon, &entity, &item_index]() -> float {
		auto weapon_speed = 0.0f;
		switch (item_index) {
		case Soldier_m_TheDirectHit:
			weapon_speed = 1980.0f; break;
		case Soldier_m_BloodBotkillerRocketLauncherMkI:
		case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
		case Soldier_m_DiamondBotkillerRocketLauncherMkI:
		case Soldier_m_GoldBotkillerRocketLauncherMkII:
		case Soldier_m_SilverBotkillerRocketLauncherMkII:
		case Soldier_m_GoldBotkillerRocketLauncherMkI:
		case Soldier_m_RustBotkillerRocketLauncherMkI:
		case Soldier_m_SilverBotkillerRocketLauncherMkI:
		case Soldier_m_FestiveRocketLauncher:
		case Soldier_m_RocketLauncherR:
		case Soldier_m_RocketLauncher:
		case Soldier_m_TheOriginal:
		case Soldier_m_TheAirStrike:
		case Soldier_m_TheBlackBox:
		case Soldier_m_FestiveBlackBox:
			weapon_speed = 1100.0f; break;
		case Pyro_s_FestiveFlareGun:
		case Pyro_s_TheFlareGun:
			weapon_speed = 2000.0f; break;
		case Medic_m_SyringeGun:
		case Medic_m_SyringeGunR:
		case Medic_m_TheBlutsauger:
		case Medic_m_TheOverdose:
			weapon_speed = 1000.0f; break;
		case Engi_m_TheRescueRanger:
		case Medic_m_CrusadersCrossbow:
		case Medic_m_FestiveCrusadersCrossbow:
			weapon_speed = 2400.0f; break;
		case Demoman_m_GrenadeLauncher:
		case Demoman_m_GrenadeLauncherR:
		case Demoman_m_FestiveGrenadeLauncher:
			weapon_speed = 1216.6f; break;
		case Demoman_m_TheLochnLoad:
			weapon_speed = 1513.3f; break;
		case Demoman_m_TheLooseCannon:
			weapon_speed = 1453.9f; break;
		case Demoman_m_TheIronBomber:
			weapon_speed = 1216.6f; break;
		default: weapon_speed = 0.0f; break;

		}
		return weapon_speed;
	};
	auto get_gravity = [&pLocal, &pWeapon, &entity, &item_index]() -> float {
		auto weapon_gravity = 0.0f;
		switch (item_index) {
		case Engi_m_TheRescueRanger:
		case Medic_m_CrusadersCrossbow:
		case Medic_m_FestiveCrusadersCrossbow:
			weapon_gravity = 0.2f; break;
		case Demoman_m_GrenadeLauncher:
		case Demoman_m_GrenadeLauncherR:
		case Demoman_m_FestiveGrenadeLauncher:
		case Demoman_m_TheLooseCannon:
		case Demoman_m_TheLochnLoad:
		case Demoman_m_TheIronBomber:
			weapon_gravity = 0.4f; break;
		default: weapon_gravity = 0.0f; break;
		}
		return weapon_gravity;
	};

	auto distance_to_ground = [&entity]() -> float {
		if (entity->GetEFlags() & FL_ONGROUND) return 0.0f;
		auto distance_to_ground = [&entity](Vector origin) -> float
		{
			trace_t ground_trace; Ray_t ray;
			CTraceFilterSimpleList filter = NULL; filter.AddEntityToIgnore(entity);
			Vector endpos = origin;

			endpos.z -= 8192;
			ray.Init(origin, endpos);
			I::EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &ground_trace);
			return 8192.0f * ground_trace.fraction;
		};
		Vector origin = entity->GetAbsOrigin();
		float v1 = distance_to_ground(origin + Vector(10.0f, 10.0f, 0.0f));
		float v2 = distance_to_ground(origin + Vector(-10.0f, 10.0f, 0.0f));
		float v3 = distance_to_ground(origin + Vector(10.0f, -10.0f, 0.0f));
		float v4 = distance_to_ground(origin + Vector(-10.0f, -10.0f, 0.0f));
		return min(v1, min(v2, min(v3, v4)));
	};

	if (IsProjectileWeapon(pLocal, pWeapon)) {
		bool on_ground = entity->GetEFlags() & FL_ONGROUND;
		if (pLocal->m_iClass() == TF_CLASS_MEDIC || pLocal->m_iClass() == TF_CLASS_ENGINEER || pLocal->m_iClass() == TF_CLASS_PYRO)
			vec_hitbox = HITBOX_BODY;
		else if (pLocal->m_iClass() == TF_CLASS_SOLDIER || pLocal->m_iClass() == TF_CLASS_DEMOMAN) {
			vec_hitbox = HITBOX_PELVIS;
		}

		auto hitbox_pred = [&pLocal, &entity, &on_ground](Vector hitbox, float speed, float gravity, float distance_to_ground) -> Vector {
			Vector result = hitbox, bestpos = result;

			auto vec_velocity = F::Aimbot.EliminateAbsVelocity(entity);
			auto medianTime = (pLocal->EyePosition().DistTo(result) / speed), range = 1.5f;
			auto currenttime = medianTime - range;

			if (currenttime <= 0.0f) currenttime = 0.01f;

			auto besttime = currenttime, mindelta = 65536.0f; auto maxsteps = 300;
			for (int steps = 0; steps < maxsteps; steps++, currenttime += ((float)(2 * range) / (float)maxsteps)) {
				Vector curpos = result; curpos += vec_velocity * currenttime;

				if (distance_to_ground > 0.0f) {
					curpos.z -= currenttime * currenttime * 400.0f * ((entity, TF_COND_PARACHUTE_ACTIVE) ? 0.448f : 1.0f);
					if (curpos.z < result.z - distance_to_ground) {
						curpos.z = result.z - distance_to_ground;
					}
				}

				auto rockettime = (pLocal->EyePosition().DistTo(curpos) / speed);
				if (fabs(rockettime - currenttime) < mindelta) {
					besttime = currenttime; bestpos = curpos;
					mindelta = fabs(rockettime - currenttime);
				}
			}
			bestpos.z += (400 * besttime * besttime * gravity);
			return bestpos;
		};
	}
}