#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Aimbot
{
public:
	void CreateNewCommands(CUserCmd* OldCmd, CUserCmd* cur_cmd);
	void DrawFps();
private:
	void GetTarget(Vector& WorldPos);

private:
	int m_target = -1;
	int m_local_client_num = 0;

	QAngle m_delta;

	Vector m_localPos = { };
};

namespace F { inline CFeatures_Aimbot Aimbot; }