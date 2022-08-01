#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Misc
{
public:
	bool bBunnyhopActive = true;
	bool bAutostrafe = true;

	void Run(C_TFPlayer* pLocal, CUserCmd* cmd);

private:
	void Bunnyhop(C_TFPlayer* pLocal, CUserCmd* cmd);
	void AntiWarp(C_TFPlayer* pLocal, CUserCmd* cmd);
	void DoStrafe(C_TFPlayer* pLocal, CUserCmd* cmd);
	void DoEdgeJump(C_TFPlayer* pLocal, CUserCmd* cmd);
	void AutoDisguise(C_TFPlayer* pLocal, CUserCmd* cmd);
	bool ShouldRun(C_TFPlayer* pLocal);
	void DrawArrowTo(const Vector& vecFromPos, const Vector& vecToPos);
public:
	void WarningRun(C_TFPlayer* pLocal);
private:
	std::vector<Vector> m_vecSpies = {};

};

namespace F { inline CFeatures_Misc Misc; }