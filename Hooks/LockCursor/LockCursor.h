#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace VGuiSurface
	{
		inline Hook::CTable Table;

		namespace OnScreenSizeChanged
		{
			constexpr uint32 Index = 111u;
			using FN = void(__fastcall*)(void*, void*, int, int);
			void __fastcall Detour(void* ecx, void* edx, int oldWidth, int oldHeight);
		}

		void Initialize();
	}
}