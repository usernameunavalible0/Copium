#pragma once

/*
* Copyright (C) 2020  nullworks
* Adapted by DepressedPotato (2022)
*/

#include "../../SDK/SDK.h"

class CFeatures_NoSpread
{
	bool is_syncing = false;
	bool shouldNoSpread(bool _projectile);
	void CreateMove();

	//const InitRoutine init([]() { EC::Register(EC::CreateMoveLate, CreateMove, "nospread_cm", EC::very_late); });

	enum nospread_sync_state
	{
		NOT_SYNCED = 0,
		CORRECTING,
		SYNCED,
		DEAD_SYNC,
	};

	const bool waiting_perf_data = false;
	const bool should_update_time = false;
	const bool waiting_for_post_SNM = false;
	const bool resync_needed = false;
	const bool last_was_player_perf = false;
	const bool resynced_this_death = false;
	const nospread_sync_state no_spread_synced = NOT_SYNCED; // this will be set to 0 each level init / level shutdown
	const bool bad_mantissa = false;      // Also reset every levelinit/shutdown
	const double float_time_delta = 0.0;
	const double ping_at_send = 0.0;
	const double last_ping_at_send = 0.0;
	const double sent_client_floattime = 0.0;
	const double last_correction = 0.0;
	const double write_usercmd_correction = 0.0;
	const double last_sync_delta_time = 0.0;
	const float prediction_seed = 0.0;
	const bool use_usercmd_seed = false;
	const float current_weapon_spread = 0.0;
	const bool first_usercmd = false;
	const bool called_from_sendmove = false;
	const bool should_update_usercmd_correction = false;
	const CUserCmd user_cmd_backup;

	static float CalculateMantissaStep(float flValue);
	float GetServerCurTime();

	// Does the shot have any spread in general?
	bool IsPerfectShot(IClientEntity* weapon, float provided_time = 0.0 /*used for optimization*/);

	// Applies nospread
	void ApplySpreadCorrection(Vector& angles, int seed, float spread);

	bool SendNetMessage(INetMessage* data);
	bool DispatchUserMessage(bf_read* buf, int type);

	/*
	void CL_SendMove_hook();	NOT NEEDED AS THIS HOOK IS IN HOOKS FOLDER
	*/

	//static Timer update_nospread_timer{};
	void CreateMove2();

};

namespace F { inline CFeatures_NoSpread NoSpread; }