#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace VGuiSurface
	{
		inline Hook::CTable Table;

		namespace LockCursor
		{
			using FN = void(__fastcall*)(void*, void*);
			constexpr uint32 Index = 65u;

			void __fastcall Detour(void* ecx, void* edx);
		}

		void Initialize();
	}
}