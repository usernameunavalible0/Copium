#pragma once
#include "../../SDK/SDK.h"

/*
bool C_BaseEntity::IsVisible(C_BaseEntity* pLocal)
{
	trace_t trace;
	Ray_t ray;
	CTraceFilterSimpleList filter = NULL;

	filter.AddEntityToIgnore(pLocal);
	
	auto local_eye = pLocal->EyePosition();
	auto entity_eye = this->EyePosition();
	ray.Init(local_eye, entity_eye);

	I::EngineTrace->TraceRay(ray, MASK_SOLID, &filter, &trace);

	//if (trace.m_pEnt == this)
	//return true;
	return (trace.m_pEnt == this);
} */