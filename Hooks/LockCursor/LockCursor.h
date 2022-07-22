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
			using FN = void(__fastcall*)(int, int);
			void __fastcall Detour(int oldWidth, int oldHeight);
		}

		namespace LockCursor
		{
			using FN = void(__fastcall*)(void*, void*);
			constexpr uint32 Index = 65u; //used to be 65

			void __fastcall Detour(void* ecx, void* edx);
		}

		void Initialize();
	}
}