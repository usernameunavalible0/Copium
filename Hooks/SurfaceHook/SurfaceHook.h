#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace VGuiSurface
	{
		inline Hook::CTable Table;

		namespace LockCursor
		{

			using FN = void(__fastcall*)(vgui::ISurface*);
			constexpr uint32 Index = 62u;

			void __fastcall Detour();
		}

		void Initialize();
	}
}