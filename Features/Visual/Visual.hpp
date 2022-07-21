#pragma once

#include "../../SDK/SDK.h"
#include "../Misc/Misc.h"

class CFeatures_Visual
{
private:
	int m_nHudZoom = 0;
public:
	void Run(C_TFPlayer* pLocal, CViewSetup* pSetup);

	bool RemoveScope(int nPanel);
	void Thirdperson(C_TFPlayer* pLocal);
	void DoFov(CViewSetup* pViewSetup);
	void DoSpectators();

/*

	Spectator List

*/

private:
	struct Spectator_t { std::wstring m_sName; std::wstring m_sMode; bool m_bIsFriend; int m_nTeam; int m_nIndex; };
	std::vector<Spectator_t> m_vecSpectators;
	bool GetSpectators(C_TFPlayer* pLocal);
	bool ShouldRun();
	void DragSpecList(int& x, int& y, int w, int h, int offsety);
	void DrawDefault();
	void DrawClassic();

private:
	int m_nSpecListW = 160, m_nSpecListTitleBarH = 12;

public:
	int m_nSpecListX = 500, m_nSpecListY = 500;

public:
	void RunSpecs();

};

namespace F { inline CFeatures_Visual Visual; }