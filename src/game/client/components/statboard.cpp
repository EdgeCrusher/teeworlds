#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/generated/client_data.h>
#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/client/render.h>
#include <game/client/components/motd.h>
#include <game/localization.h>
#include "statboard.h"


#include <game/client/teecomp.h>

CStatboard::CStatboard()
{
	OnReset();
}

void CStatboard::ConKeyStatboard(IConsole::IResult *pResult, void *pUserData)
{
	((CStatboard *)pUserData)->m_Active = pResult->GetInteger(0) != 0;
}

void CStatboard::OnReset()
{
	m_Active = false;
}

void CStatboard::OnConsoleInit()
{
	Console()->Register("+statboard", "", CFGFLAG_CLIENT, ConKeyStatboard, this, "Show scoreboard");
}

void CStatboard::RenderGoals(float x, float y, float w)
{
	float h = 50.0f;

	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.5f);
	RenderTools()->DrawRoundRect(x-10.f, y-10.f, w, h, 10.0f);
	Graphics()->QuadsEnd();

	// render goals
	//y = ystart+h-54;
	float tw = 0.0f;
	if(m_pClient->m_Snap.m_pGameobj)
	{
		if(m_pClient->m_Snap.m_pGameobj->m_ScoreLimit)
		{
			char aaBuf[64];
			str_format(aaBuf, sizeof(aaBuf), "%s: %d", Localize("Score limit"), m_pClient->m_Snap.m_pGameobj->m_ScoreLimit);
			TextRender()->Text(0, x+20.0f, y, 22.0f, aaBuf, -1);
			tw += TextRender()->TextWidth(0, 22.0f, aaBuf, -1);
		}
		if(m_pClient->m_Snap.m_pGameobj->m_TimeLimit)
		{
			char aaBuf[64];
			str_format(aaBuf, sizeof(aaBuf), "%s: %d min", Localize("Time limit"), m_pClient->m_Snap.m_pGameobj->m_TimeLimit);
			TextRender()->Text(0, x+220.0f, y, 22.0f, aaBuf, -1);
			tw += TextRender()->TextWidth(0, 22.0f, aaBuf, -1);
		}
		if(m_pClient->m_Snap.m_pGameobj->m_RoundNum && m_pClient->m_Snap.m_pGameobj->m_RoundCurrent)
		{
			char aaBuf[64];
			str_format(aaBuf, sizeof(aaBuf), "%s %d/%d", Localize("Round"), m_pClient->m_Snap.m_pGameobj->m_RoundCurrent, m_pClient->m_Snap.m_pGameobj->m_RoundNum);
			TextRender()->Text(0, x+450.0f, y, 22.0f, aaBuf, -1);
			
		/*[48c3fd4c][game/scoreboard]: timelimit x:219.428558
		[48c3fd4c][game/scoreboard]: round x:453.142822*/
		}
	}
}

void CStatboard::RenderSpectators(float x, float y, float w)
{
	/*char aaBuffer[1024*4];
	int Count = 0;
	float h = 120.0f;
	
	str_format(aaBuffer, sizeof(aaBuffer), "%s: ", Localize("Spectators"));

	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.5f);
	RenderTools()->DrawRoundRect(x-10.f, y-10.f, w, h, 10.0f);
	Graphics()->QuadsEnd();
	
	for(int i = 0; i < Client()->SnapNumItems(IClient::SNAP_CURRENT); i++)
	{
		IClient::CSnapItem Item;
		const void *pData = Client()->SnapGetItem(IClient::SNAP_CURRENT, i, &Item);

		if(Item.m_Type == NETOBJTYPE_PLAYERINFO)
		{
			const CNetObj_PlayerInfo *pInfo = (const CNetObj_PlayerInfo *)pData;
			if(pInfo->m_Team == -1)
			{
				if(Count)
					str_append(aaBuffer, ", ", sizeof(aaBuffer));
				str_append(aaBuffer, m_pClient->m_aClients[pInfo->m_ClientId].m_aName, sizeof(aaBuffer));
				Count++;
			}
		}
	}
	
	TextRender()->Text(0, x+10, y, 32, aaBuffer, (int)w-20);*/
	
RenderScoreboard(500, 980, 900, -1, "Spec");
}

void CStatboard::RenderScoreboard(float x, float y, float w, int Team, const char *pTitle)
{
	//float ystart = y;
	float h = 750.0f;

	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.5f);
	if(Team!=-1)
	RenderTools()->DrawRoundRect(x-10.f, y-10.f, w+200.0f, h, 17.0f);
	if(Team==-1)
	RenderTools()->DrawRoundRect(x-10.f, y-10.f, w, 200.0f, 17.0f);
	Graphics()->QuadsEnd();

	// render title
	if(!pTitle)
	{
		if(m_pClient->m_Snap.m_pGameobj->m_GameOver)
			pTitle = Localize("Game over");
		else
			pTitle = Localize("Score board");
	}

	float tw = TextRender()->TextWidth(0, 48, pTitle, -1);

	if(Team == -1)
	{
		//TextRender()->Text(0, x+w/2-tw/2, y, 48, pTitle, -1);
	}
	else
	{
		TextRender()->Text(0, x+10, y, 48, pTitle, -1);

		if(m_pClient->m_Snap.m_pGameobj)
		{
			char aaBuf[128];
			int Score = Team ? m_pClient->m_Snap.m_pGameobj->m_TeamscoreBlue : m_pClient->m_Snap.m_pGameobj->m_TeamscoreRed;
			str_format(aaBuf, sizeof(aaBuf), "%d", Score);
			tw = TextRender()->TextWidth(0, 48, aaBuf, -1);
			TextRender()->Text(0, x+w-tw-30, y, 48, aaBuf, -1);
		}
	}
	if(Team != -1)
	y += 54.0f;

	// find players
	const CNetObj_PlayerInfo *paPlayers[MAX_CLIENTS] = {0};
	int NumPlayers = 0;
	for(int i = 0; i < Client()->SnapNumItems(IClient::SNAP_CURRENT); i++)
	{
		IClient::CSnapItem Item;
		const void *pData = Client()->SnapGetItem(IClient::SNAP_CURRENT, i, &Item);

		if(Item.m_Type == NETOBJTYPE_PLAYERINFO)
		{
			const CNetObj_PlayerInfo *pInfo = (const CNetObj_PlayerInfo *)pData;
			if(pInfo->m_Team == Team)
			{
				paPlayers[NumPlayers] = pInfo;
				if(++NumPlayers == MAX_CLIENTS)
					break;
			}
		}
	}

	// sort players
	for(int k = 0; k < NumPlayers-1; k++) // ffs, bubblesort
	{
		for(int i = 0; i < NumPlayers-k-1; i++)
		{
			if(paPlayers[i]->m_Score < paPlayers[i+1]->m_Score)
			{
				const CNetObj_PlayerInfo *pTmp = paPlayers[i];
				paPlayers[i] = paPlayers[i+1];
				paPlayers[i+1] = pTmp;
			}
		}
	}

	// render headlines
	/*TextRender()->Text(0, x+10, y, 24.0f, Localize("Score"), -1);
	TextRender()->Text(0, x+125, y, 24.0f, Localize("Name"), -1);
	TextRender()->Text(0, x+w-70, y, 24.0f, Localize("Ping"), -1);*/
	
	
	if(Team == -1){
	// Don't render Headlines
	}
	if(Team != -1){
	TextRender()->Text(0, x+20, y, 24.0f, "Score", -1);
	TextRender()->Text(0, x+100, y, 24.0f, "Ping", -1);
	TextRender()->Text(0, x+170, y, 24.0f, "Time", -1);
	TextRender()->Text(0, x+250, y, 24.0f, "ID", -1);
	TextRender()->Text(0, x+300, y, 24.0f, "FPM", -1);
	TextRender()->Text(0, x+370, y, 24.0f, "Ratio", -1);
	TextRender()->Text(0, x+450, y, 24.0f, "Flags", -1);
	TextRender()->Text(0, x+w-80, y, 24.0f, "Name", -1);}
	
	if(Team != -1)
	y += 29.0f;
	
	

	float FontSize = 35.0f;
	float LineHeight = 50.0f;
	float TeeSizeMod = 1.0f;
	float TeeOffset = 0.0f;
	
	if(Team == -1){
	FontSize = 30.0f;
	LineHeight = 40.0f;
	TeeSizeMod = 0.5f;
	TeeOffset = -5.0f;
	}
	if(Team != -1){
	FontSize = 35.0f;
	LineHeight = 50.0f;
	TeeSizeMod = 1.0f;
	}
	
	
	if(NumPlayers > 13)
	{
	if(Team != -1){
		FontSize = 30.0f;
		LineHeight = 40.0f;
		TeeSizeMod = 0.8f;
		TeeOffset = -5.0f;}
	if(Team == -1){
		FontSize = 30.0f;
		LineHeight = 40.0f;
		TeeSizeMod = 0.5f;
		TeeOffset = -5.0f;
	}
	}
	
	// render player scores
	for(int i = 0; i < NumPlayers; i++)
	{
		const CNetObj_PlayerInfo *pInfo = paPlayers[i];

		// make sure that we render the correct team

		char aaBuf[128];
		if(!m_pClient->m_Snap.m_Spectate){
		if(pInfo->m_Local)
		{
			Graphics()->TextureSet(-1);
			Graphics()->QuadsBegin();
			Graphics()->SetColor(1,1,1,0.25f);
			
			//RenderTools()->DrawRoundRect(x, y, w-20, LineHeight*0.95f, 17.0f); default rect
			if(Team != -1)
				RenderTools()->DrawRoundRect(x, y, w+190.0f, LineHeight*0.95f, 17.0f);
			if(Team == -1)
				RenderTools()->DrawRoundRect(x, y, w-20, LineHeight*0.95f, 17.0f);
			
			Graphics()->QuadsEnd();
			// background so it's easy to find the local player
		}
		}
		else{
		if(pInfo->m_ClientId == m_pClient->spectate_cid && !m_pClient->freeview){
		
	
			Graphics()->TextureSet(-1);
			Graphics()->QuadsBegin();
			Graphics()->SetColor(1,1,1,0.25f);
			RenderTools()->DrawRoundRect(x, y, w-20, LineHeight*0.95f, 17.0f);
			Graphics()->QuadsEnd();
			}
		
		
		}
		

		str_format(aaBuf, sizeof(aaBuf), "%4d", pInfo->m_Score);
		TextRender()->Text(0, x+60-TextRender()->TextWidth(0, FontSize,aaBuf,-1), y, FontSize, aaBuf, -1);
		
		
		/** NAME // spec **/
		if(Team == -1)
		TextRender()->Text(0, x+128, y, FontSize, m_pClient->m_aClients[pInfo->m_ClientId].m_aName, -1);
		
		
		//TextRender()->Text(0, x+128, y, FontSize, m_pClient->m_aClients[pInfo->m_ClientId].m_aName, -1); default name render

		str_format(aaBuf, sizeof(aaBuf), "%4d", pInfo->m_Latency);
		
		float tw = TextRender()->TextWidth(0, FontSize, aaBuf, -1);
		
		/** NAME // nonpec **/
		if(Team != -1)
		TextRender()->Text(0, x+w-tw-55, y, FontSize, m_pClient->m_aClients[pInfo->m_ClientId].m_aName, -1);
		
		
		if(Team != -1){
		TextRender()->Text(0, x+90, y, FontSize, aaBuf, -1);
		}
		if(Team == -1){
		TextRender()->Text(0, x+w-tw-20, y-10, FontSize-11.0f, aaBuf, -1);
		}
		if(Team == -1){
		TextRender()->TextColor(0,1,0,1.0f);
		/** TIME spec **/
		//str_format(aBuf, sizeof(aBuf), "%i:%02i", ((-1*(gameclient.clients[info->cid].time-base_time())/60)), ((-1*(gameclient.clients[info->cid].time-base_time()))%60));
		str_format(aaBuf, sizeof(aaBuf), "%i:%02i", ((-1*(m_pClient->m_aClients[pInfo->m_ClientId].m_JoinTime-base_time())/60)), ((-1*(m_pClient->m_aClients[pInfo->m_ClientId].m_JoinTime-base_time()))%60));
		TextRender()->Text(0, x+w-tw-20, y+10, FontSize-11.0f, aaBuf, -1);
		TextRender()->TextColor(1,1,1,1.0f);
		}
		if(Team != -1){
		/** TIME  nonspec **/
		str_format(aaBuf, sizeof(aaBuf), "%i:%02i", ((-1*(m_pClient->m_aClients[pInfo->m_ClientId].m_JoinTime-base_time())/60)), ((-1*(m_pClient->m_aClients[pInfo->m_ClientId].m_JoinTime-base_time()))%60));
		TextRender()->Text(0, x+175, y, FontSize, aaBuf, -1);
		}
		if(Team != -1){
		/** ID  nonspec **/
		str_format(aaBuf, sizeof(aaBuf), "%i", pInfo->m_ClientId);
		TextRender()->Text(0, x+255, y, FontSize, aaBuf, -1);
		
		// TODO: FPM, Ratio, Flags
		
		TextRender()->Text(0, x+255, y, FontSize, aaBuf, -1);
		
		str_format(aaBuf, sizeof(aaBuf), "%.1f", (float)(m_pClient->m_aClients[pInfo->m_ClientId].m_Frags*60)/((float)(base_time()-m_pClient->m_aClients[pInfo->m_ClientId].m_JoinTime)));
		TextRender()->Text(0, x+300, y, FontSize, aaBuf, -1);
		
		if(m_pClient->m_aClients[pInfo->m_ClientId].m_Deaths != 0){
		str_format(aaBuf, sizeof(aaBuf), "%.1f", (float)(m_pClient->m_aClients[pInfo->m_ClientId].m_Frags/m_pClient->m_aClients[pInfo->m_ClientId].m_Deaths));}
		if(m_pClient->m_aClients[pInfo->m_ClientId].m_Deaths == 0){
		str_format(aaBuf, sizeof(aaBuf), "---");
		}
		TextRender()->Text(0, x+370, y, FontSize, aaBuf, -1);
		
		str_format(aaBuf, sizeof(aaBuf), "%i", m_pClient->m_aClients[pInfo->m_ClientId].m_FlagCaptures);
		TextRender()->Text(0, x+450, y, FontSize, aaBuf, -1);
		}
		
		
		// render avatar
		if((m_pClient->m_Snap.m_paFlags[0] && m_pClient->m_Snap.m_paFlags[0]->m_CarriedBy == pInfo->m_ClientId) ||
			(m_pClient->m_Snap.m_paFlags[1] && m_pClient->m_Snap.m_paFlags[1]->m_CarriedBy == pInfo->m_ClientId))
		{
			Graphics()->BlendNormal();
			if(g_Config.m_tc_colored_flags)
			Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME_GRAY].m_Id);
			else
			Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
			Graphics()->QuadsBegin();

			if(pInfo->m_Team == 0) RenderTools()->SelectSprite(SPRITE_FLAG_BLUE, SPRITE_FLAG_FLIP_X);
			else RenderTools()->SelectSprite(SPRITE_FLAG_RED, SPRITE_FLAG_FLIP_X);
			if(g_Config.m_tc_colored_flags)
			{
				vec3 col = TeecompUtils::getTeamColor(1-pInfo->m_Team, m_pClient->m_Snap.m_paPlayerInfos[m_pClient->m_Snap.m_LocalCid]->m_Team, 
					g_Config.m_tc_colored_tees_team1, g_Config.m_tc_colored_tees_team2, g_Config.m_tc_colored_tees_method);
				Graphics()->SetColor(col.r, col.g, col.b, 1.0f);
			}
			
			
			float size = 64.0f;
			IGraphics::CQuadItem QuadItem(x-25, y-15, size/2, size);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
		}
		
		CTeeRenderInfo TeeInfo = m_pClient->m_aClients[pInfo->m_ClientId].m_RenderInfo;
		TeeInfo.m_Size *= TeeSizeMod;
		RenderTools()->RenderTee(CAnimState::GetIdle(), &TeeInfo, EMOTE_NORMAL, vec2(1,0), vec2(x+10, y+28+TeeOffset));

		
		y += LineHeight;
	}
}

void CStatboard::OnRender()
{
	bool DoScoreBoard = false;

	// if we activly wanna look on the scoreboard	
	if(m_Active)
		DoScoreBoard = true;
		
	if(m_pClient->m_Snap.m_pLocalInfo && m_pClient->m_Snap.m_pLocalInfo->m_Team != -1)
	{
		// we are not a spectator, check if we are ead
		if((!m_pClient->m_Snap.m_pLocalCharacter || m_pClient->m_Snap.m_pLocalCharacter->m_Health < 0) && g_Config.m_cl_new_scoreboard)
			DoScoreBoard = true;
	}

	// if we the game is over
	if((m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver) && g_Config.m_cl_new_scoreboard){
		DoScoreBoard = true;}
	if(m_pClient->m_Snap.m_pGameobj && !m_pClient->m_Snap.m_pGameobj->m_GameOver)
		m_DoScreenShot = true;
		
	if(!DoScoreBoard)
		return;
		
	// if the score board is active, then we should clear the motd message aswell
	if(m_Active)
		m_pClient->m_pMotd->Clear();
	

	float Width = 400*3.0f*Graphics()->ScreenAspect();
	float Height = 400*3.0f;
	
	Graphics()->MapScreen(0, 0, Width, Height);

	float w = 650.0f;

	if(m_pClient->m_Snap.m_pGameobj && !(m_pClient->m_Snap.m_pGameobj->m_Flags&GAMEFLAG_TEAMS))
	{
		RenderScoreboard(Width/2-w/2, 150.0f, w, 0, 0);
		//render_scoreboard(gameobj, 0, 0, -1, 0);
	}
	else
	{
			
		if(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver)
		{
			const char *pText = Localize("Draw!");
			if(m_pClient->m_Snap.m_pGameobj->m_TeamscoreRed > m_pClient->m_Snap.m_pGameobj->m_TeamscoreBlue)
				pText = Localize("Red team wins!");
			else if(m_pClient->m_Snap.m_pGameobj->m_TeamscoreBlue > m_pClient->m_Snap.m_pGameobj->m_TeamscoreRed)
				pText = Localize("Blue team wins!");
				
			float w = TextRender()->TextWidth(0, 92.0f, pText, -1);
			TextRender()->Text(0, Width/2-w/2, 45, 92.0f, pText, -1);
		}
		
		RenderScoreboard((Width/2-w-140)-60, 150.0f, w, 0, Localize("Red team"));
		RenderScoreboard((Width/2 + 140)-60, 150.0f, w, 1, Localize("Blue team"));
	}

	RenderGoals(Width/2-w/2, 150+750+25, w);
	RenderSpectators(Width/2-w/2, 150+750+25+50+25, w);
}
