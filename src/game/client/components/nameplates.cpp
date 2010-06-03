#include <engine/textrender.h>
#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/generated/client_data.h>

#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include "nameplates.h"
#include "controls.h"

#include <game/client/teecomp.h>

void CNamePlates::RenderNameplate(
	const CNetObj_Character *pPrevChar,
	const CNetObj_Character *pPlayerChar,
	const CNetObj_PlayerInfo *pPlayerInfo
	)
{
	float IntraTick = Client()->IntraGameTick();
	
	vec2 Position = mix(vec2(pPrevChar->m_X, pPrevChar->m_Y), vec2(pPlayerChar->m_X, pPlayerChar->m_Y), IntraTick);
	
	// render name plate
	if(!pPlayerInfo->m_Local || g_Config.m_hud_show_own_nameplate)
	{
		//TextRender()->TextColor
		float a = 1;
		if(g_Config.m_ClNameplatesAlways == 0)
			a = clamp(1-powf(distance(m_pClient->m_pControls->m_TargetPos, Position)/200.0f,16.0f), 0.0f, 1.0f);
			

			
			
		char pName[256]; // = m_pClient->m_aClients[pPlayerInfo->m_ClientId].m_aName;
		
					if(!g_Config.m_tc_nameplate_score)
			str_format(pName, 256, "%s", m_pClient->m_aClients[pPlayerInfo->m_ClientId].m_aName);
		else
			str_format(pName, 256, "%s (%d)", m_pClient->m_aClients[pPlayerInfo->m_ClientId].m_aName, pPlayerInfo->m_Score);
			
		float tw = TextRender()->TextWidth(0, 28.0f, pName, -1);
		
			if(g_Config.m_tc_nameplate_shadow)
		{
			TextRender()->TextColor(0,0,0,a);
			TextRender()->Text(0, Position.x-tw/2.0f+2, Position.y-60+2, 28.0f, pName, -1);
		}
		bool is_teamplay;
		is_teamplay = m_pClient->m_Snap.m_pGameobj->m_Flags&GAMEFLAG_TEAMS;
		
		if(g_Config.m_tc_colored_nameplates&1 && is_teamplay)
		{
			vec3 col = TeecompUtils::getTeamColor(
				m_pClient->m_aClients[pPlayerInfo->m_ClientId].m_Team,
				m_pClient->m_Snap.m_pLocalInfo->m_Team,
				g_Config.m_tc_colored_nameplates_team1,
				g_Config.m_tc_colored_nameplates_team2,
				g_Config.m_tc_colored_nameplates&2);
			TextRender()->TextColor(col.r, col.g, col.b, a);
		}
		else // FFA or no colored plates
		TextRender()->TextColor(1,1,1,a);
		
		
		TextRender()->Text(0, Position.x-tw/2.0f, Position.y-60, 28.0f, pName, -1);
		
		if(g_Config.m_Debug) // render client id when in debug aswell
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf),"%d", pPlayerInfo->m_ClientId);
			TextRender()->Text(0, Position.x, Position.y-90, 28.0f, aBuf, -1);
		}

		TextRender()->TextColor(1,1,1,1);
	}
}

void CNamePlates::OnRender()
{
	if (!g_Config.m_ClNameplates)
		return;

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		// only render active characters
		if(!m_pClient->m_Snap.m_aCharacters[i].m_Active)
			continue;

		const void *pInfo = Client()->SnapFindItem(IClient::SNAP_CURRENT, NETOBJTYPE_PLAYERINFO, i);

		if(pInfo)
		{
			RenderNameplate(
				&m_pClient->m_Snap.m_aCharacters[i].m_Prev,
				&m_pClient->m_Snap.m_aCharacters[i].m_Cur,
				(const CNetObj_PlayerInfo *)pInfo);
		}
	}
}
