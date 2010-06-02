#include <engine/shared/config.h>

#include <base/math.h>
#include <game/collision.h>
#include <game/client/gameclient.h>
#include <game/client/component.h>

#include "camera.h"
#include "controls.h"

CCamera::CCamera()
{
}

void CCamera::OnRender()
{
	//vec2 center;
	m_Zoom = 1.0f;

	// update camera center		

		
	if(sizeof(g_Config.m_cl_dyncam_key) >= 0 && g_Config.m_cl_dyncam_switch){
	if(!g_Config.m_cl_dc_state)
	{
				g_Config.m_cl_mouse_followfactor = g_Config.m_cl_mouse_followfactor_dc_off;
				g_Config.m_cl_mouse_max_distance = g_Config.m_cl_mouse_max_distance_dc_off;
				g_Config.m_cl_mouse_deadzone = g_Config.m_cl_mouse_deadzone_dc_off;
	}
	if(g_Config.m_cl_dc_state)
	{
				g_Config.m_cl_mouse_followfactor = g_Config.m_cl_mouse_followfactor_dc_on;
				g_Config.m_cl_mouse_max_distance = g_Config.m_cl_mouse_max_distance_dc_on;
				g_Config.m_cl_mouse_deadzone = g_Config.m_cl_mouse_deadzone_dc_on;
	}}
		
	if(m_pClient->m_Snap.m_Spectate){
		if(m_pClient->freeview)
			m_Center = m_pClient->m_pControls->m_MousePos;
		if(!m_pClient->freeview)
			m_Center = m_pClient->spectate_pos;
	}
	else
	{

		float l = length(m_pClient->m_pControls->m_MousePos);
		float DeadZone = g_Config.m_ClMouseDeadzone;
		float FollowFactor = g_Config.m_ClMouseFollowfactor/100.0f;
		vec2 CameraOffset(0, 0);

		float OffsetAmount = max(l-DeadZone, 0.0f) * FollowFactor;
		if(l > 0.0001f) // make sure that this isn't 0
			CameraOffset = normalize(m_pClient->m_pControls->m_MousePos)*OffsetAmount;
		
		m_Center = m_pClient->m_LocalCharacterPos + CameraOffset;
	}
}
