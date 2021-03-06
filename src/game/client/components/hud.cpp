#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/shared/config.h>

#include <game/generated/protocol.h>
#include <game/generated/client_data.h>
#include <game/layers.h>
#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/client/render.h>

#include "controls.h"
#include "camera.h"
#include "hud.h"
#include "voting.h"
#include "binds.h"

#include <game/client/teecomp.h>

CHud::CHud()
{
	// won't work if zero
	m_AverageFPS = 1.0f;
}
	
void CHud::OnReset()
{
}

void CHud::RenderGameTimer()
{
if(g_Config.m_cl_hud){                                                       /** HUD Part: Timer **/
	float Half = 300.0f*Graphics()->ScreenAspect()/2.0f;
	Graphics()->MapScreen(0, 0, 300.0f*Graphics()->ScreenAspect(), 300.0f);
	
	if(!m_pClient->m_Snap.m_pGameobj->m_SuddenDeath)
	{
		char Buf[32];
		int Time = 0;
		if(m_pClient->m_Snap.m_pGameobj->m_TimeLimit)
		{
			Time = m_pClient->m_Snap.m_pGameobj->m_TimeLimit*60 - ((Client()->GameTick()-m_pClient->m_Snap.m_pGameobj->m_RoundStartTick)/Client()->GameTickSpeed());

			if(m_pClient->m_Snap.m_pGameobj->m_GameOver)
				Time  = 0;
		}
		else
			Time = (Client()->GameTick()-m_pClient->m_Snap.m_pGameobj->m_RoundStartTick)/Client()->GameTickSpeed();

		str_format(Buf, sizeof(Buf), "%d:%02d", Time/60, Time%60);
		float FontSize = 10.0f;
		float w = TextRender()->TextWidth(0, FontSize, Buf, -1);
		TextRender()->Text(0, Half-w/2, 2, FontSize, Buf, -1);
	}
}
}

void CHud::RenderSuddenDeath()
{
if(g_Config.m_cl_hud){                                                       /** HUD Part: Sudden Death ( Timer ) **/
	if(m_pClient->m_Snap.m_pGameobj->m_SuddenDeath)
	{
		float Half = 300.0f*Graphics()->ScreenAspect()/2.0f;
		const char *pText = Localize("Sudden Death");
		float FontSize = 12.0f;
		float w = TextRender()->TextWidth(0, FontSize, pText, -1);
		TextRender()->Text(0, Half-w/2, 2, FontSize, pText, -1);
	}
}
}

void CHud::RenderScoreHud()
{
if(g_Config.m_cl_hud && g_Config.m_hud_goals){                                                       /** HUD Part: Goals **/
	int GameFlags = m_pClient->m_Snap.m_pGameobj->m_Flags;
	float Whole = 300*Graphics()->ScreenAspect();
	
	// render small score hud
	if(!(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver) && (GameFlags&GAMEFLAG_TEAMS))
	{
		for(int t = 0; t < 2; t++)
		{
			Graphics()->BlendNormal();
			Graphics()->TextureSet(-1);
			Graphics()->QuadsBegin();
			
			if(!g_Config.m_tc_hud_match)
			{
				if(t == 0)
					Graphics()->SetColor(1,0,0,0.25f);
				else
					Graphics()->SetColor(0,0,1,0.25f);
			}
			else
			{
				vec3 col = TeecompUtils::getTeamColor(t, m_pClient->m_Snap.m_pLocalInfo->m_Team,
					g_Config.m_tc_colored_tees_team1, g_Config.m_tc_colored_tees_team2, g_Config.m_tc_colored_tees_method);
				Graphics()->SetColor(col.r, col.g, col.b, 0.25f);
			}
			
			
			RenderTools()->DrawRoundRect(Whole-45, 300-40-15+t*20, 50, 18, 5.0f);
			Graphics()->QuadsEnd();

			char Buf[32];
			str_format(Buf, sizeof(Buf), "%d", t?m_pClient->m_Snap.m_pGameobj->m_TeamscoreBlue : m_pClient->m_Snap.m_pGameobj->m_TeamscoreRed);
			float w = TextRender()->TextWidth(0, 14, Buf, -1);
			
			if(GameFlags&GAMEFLAG_FLAGS)
			{
				TextRender()->Text(0, Whole-20-w/2+5, 300-40-15+t*20, 14, Buf, -1);
				if(m_pClient->m_Snap.m_paFlags[t])
				{
					if(m_pClient->m_Snap.m_paFlags[t]->m_CarriedBy == -2 || (m_pClient->m_Snap.m_paFlags[t]->m_CarriedBy == -1 && ((Client()->GameTick()/10)&1)))
					{
						Graphics()->BlendNormal();
						if(g_Config.m_tc_colored_flags)
							Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME_GRAY].m_Id);
						else
							Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
						Graphics()->QuadsBegin();

						if(t == 0) RenderTools()->SelectSprite(SPRITE_FLAG_RED);
						else RenderTools()->SelectSprite(SPRITE_FLAG_BLUE);
						
						if(g_Config.m_tc_colored_flags)
						{
							vec3 col = TeecompUtils::getTeamColor(t,
								m_pClient->m_Snap.m_pLocalInfo->m_Team,
								g_Config.m_tc_colored_tees_team1,
								g_Config.m_tc_colored_tees_team2,
								g_Config.m_tc_colored_tees_method);
							Graphics()->SetColor(col.r, col.g, col.b, 1.0f);
						}
						
						float Size = 16;					
						IGraphics::CQuadItem QuadItem(Whole-40+2, 300-40-15+t*20+1, Size/2, Size);
						Graphics()->QuadsDrawTL(&QuadItem, 1);
						Graphics()->QuadsEnd();
					}
					else if(m_pClient->m_Snap.m_paFlags[t]->m_CarriedBy >= 0)
					{
						int Id = m_pClient->m_Snap.m_paFlags[t]->m_CarriedBy%MAX_CLIENTS;
						const char *pName = m_pClient->m_aClients[Id].m_aName;
						float w = TextRender()->TextWidth(0, 10, pName, -1);
						TextRender()->Text(0, Whole-40-7-w, 300-40-15+t*20+2, 10, pName, -1);
						CTeeRenderInfo Info = m_pClient->m_aClients[Id].m_RenderInfo;
						Info.m_Size = 18.0f;
						
						RenderTools()->RenderTee(CAnimState::GetIdle(), &Info, EMOTE_NORMAL, vec2(1,0),
							vec2(Whole-40+5, 300-40-15+9+t*20+1));
					}
				}
			}
			else
				TextRender()->Text(0, Whole-20-w/2, 300-40-15+t*20, 14, Buf, -1);
		}
	}
}
}

void CHud::RenderWarmupTimer()
{
if(g_Config.m_cl_hud){                                                       /** HUD Part: Warmup Timer **/
	// render warmup timer
	if(m_pClient->m_Snap.m_pGameobj->m_Warmup)
	{
		char Buf[256];
		float FontSize = g_Config.m_cl_warmup_size;
		float w = TextRender()->TextWidth(0, FontSize, Localize("Warmup"), -1);
		TextRender()->Text(0, 150*Graphics()->ScreenAspect()+-w/2, 50, g_Config.m_cl_warmup_size, Localize("Warmup"), -1);

		int Seconds = m_pClient->m_Snap.m_pGameobj->m_Warmup/SERVER_TICK_SPEED;
		if(Seconds < 5)
			str_format(Buf, sizeof(Buf), "%d.%d", Seconds, (m_pClient->m_Snap.m_pGameobj->m_Warmup*10/SERVER_TICK_SPEED)%10);
		else
			str_format(Buf, sizeof(Buf), "%d", Seconds);
		w = TextRender()->TextWidth(0, FontSize, Buf, -1);
		TextRender()->Text(0, 150*Graphics()->ScreenAspect()+-w/2, 75, FontSize, Buf, -1);
	}}	
}

void CHud::MapscreenToGroup(float CenterX, float CenterY, CMapItemGroup *pGroup)
{
	float Points[4];
	RenderTools()->MapscreenToWorld(CenterX, CenterY, pGroup->m_ParallaxX/100.0f, pGroup->m_ParallaxY/100.0f,
		pGroup->m_OffsetX, pGroup->m_OffsetY, Graphics()->ScreenAspect(), 1.0f, Points);
	Graphics()->MapScreen(Points[0], Points[1], Points[2], Points[3]);
}

void CHud::RenderFps()
{
	if(g_Config.m_ClShowfps)
	{
		// calculate avg. fps
		float FPS = 1.0f / Client()->FrameTime();
		m_AverageFPS = (m_AverageFPS*(1.0f-(1.0f/m_AverageFPS))) + (FPS*(1.0f/m_AverageFPS));
		char Buf[512];
		str_format(Buf, sizeof(Buf), "%d", (int)m_AverageFPS);
		TextRender()->Text(0, m_Width-10-TextRender()->TextWidth(0,12,Buf,-1), 5, 12, Buf, -1);
	}
}



void CHud::RenderConnectionWarning()
{
if(g_Config.m_cl_hud){                                                       /** HUD Part: Connection Problems warning **/
	if(Client()->ConnectionProblems())
	{
		const char *pText = Localize("Connection Problems...");
		float w = TextRender()->TextWidth(0, 24, pText, -1);
		TextRender()->Text(0, 150*Graphics()->ScreenAspect()-w/2, 50, 24, pText, -1);
	}}
}

void CHud::RenderTeambalanceWarning()
{
if(g_Config.m_cl_hud && g_Config.m_hud_teambalance){                          /** HUD Part: Teambalance Warning **/
	// render prompt about team-balance
	bool Flash = time_get()/(time_freq()/2)%2 == 0;
	if (m_pClient->m_Snap.m_pGameobj && (m_pClient->m_Snap.m_pGameobj->m_Flags&GAMEFLAG_TEAMS) != 0)
	{	
		int TeamDiff = m_pClient->m_Snap.m_aTeamSize[0]-m_pClient->m_Snap.m_aTeamSize[1];
		if (g_Config.m_ClWarningTeambalance && (TeamDiff >= 2 || TeamDiff <= -2))
		{
			const char *pText = Localize("Please balance teams!");
			if(Flash)
				TextRender()->TextColor(1,1,0.5f,1);
			else
				TextRender()->TextColor(0.7f,0.7f,0.2f,1.0f);
			TextRender()->Text(0x0, 5, 50, 6, pText, -1);
			TextRender()->TextColor(1,1,1,1);
		}
	}
}
}


void CHud::RenderVoting()
{
if(g_Config.m_cl_hud && g_Config.m_hud_voting){                                 /** HUD Part: Running Votes **/
	if(!m_pClient->m_pVoting->IsVoting())
		return;
	
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.40f);
	RenderTools()->DrawRoundRect(-10, 60-2, 100+10+4+5, 28, 5.0f);
	Graphics()->QuadsEnd();

	TextRender()->TextColor(1,1,1,1);

	char Buf[512];
	TextRender()->Text(0x0, 5, 60, 6, m_pClient->m_pVoting->VoteDescription(), -1);

	str_format(Buf, sizeof(Buf), Localize("%ds left"), m_pClient->m_pVoting->SecondsLeft());
	float tw = TextRender()->TextWidth(0x0, 6, Buf, -1);
	TextRender()->Text(0x0, 5+100-tw, 60, 6, Buf, -1);
	

	CUIRect Base = {5, 70, 100, 4};
	m_pClient->m_pVoting->RenderBars(Base, false);
	
	const char *pYesKey = m_pClient->m_pBinds->GetKey("vote yes");
	const char *pNoKey = m_pClient->m_pBinds->GetKey("vote no");
	str_format(Buf, sizeof(Buf), "%s - Vote Yes", pYesKey);
	Base.y += Base.h+1;
	UI()->DoLabel(&Base, Buf, 6.0f, -1);

	str_format(Buf, sizeof(Buf), "Vote No - %s", pNoKey);
	UI()->DoLabel(&Base, Buf, 6.0f, 1);
}
	}

void CHud::RenderCursor()
{
if(g_Config.m_cl_hud && g_Config.m_hud_cursor){                                 /** HUD Part: Cursor **/
	if(!m_pClient->m_Snap.m_pLocalCharacter)
		return;
		
	MapscreenToGroup(m_pClient->m_pCamera->m_Center.x, m_pClient->m_pCamera->m_Center.y, Layers()->GameGroup());
	Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
	Graphics()->QuadsBegin();

	// render cursor
	RenderTools()->SelectSprite(g_pData->m_Weapons.m_aId[m_pClient->m_Snap.m_pLocalCharacter->m_Weapon%NUM_WEAPONS].m_pSpriteCursor);
	float CursorSize = 64;
	RenderTools()->DrawSprite(m_pClient->m_pControls->m_TargetPos.x, m_pClient->m_pControls->m_TargetPos.y, CursorSize);
	Graphics()->QuadsEnd();
}
}

void CHud::RenderHealthAndAmmo()
{
if(g_Config.m_cl_hud && g_Config.m_hud_hp_ammo){                                 /** HUD Part: HP and Ammo **/
	//mapscreen_to_group(gacenter_x, center_y, layers_game_group());

	float x = 5;
	float y = 5;

	// render ammo count
	// render gui stuff

	Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
	Graphics()->MapScreen(0,0,m_Width,300);
	
	Graphics()->QuadsBegin();
	
	// if weaponstage is active, put a "glow" around the stage ammo
	RenderTools()->SelectSprite(g_pData->m_Weapons.m_aId[m_pClient->m_Snap.m_pLocalCharacter->m_Weapon%NUM_WEAPONS].m_pSpriteProj);
	IGraphics::CQuadItem Array[10];
	int i;
	for (i = 0; i < min(m_pClient->m_Snap.m_pLocalCharacter->m_AmmoCount, 10); i++)
		Array[i] = IGraphics::CQuadItem(x+i*12,y+24,10,10);
	Graphics()->QuadsDrawTL(Array, i);
	Graphics()->QuadsEnd();

	Graphics()->QuadsBegin();
	int h = 0;

	// render health
	RenderTools()->SelectSprite(SPRITE_HEALTH_FULL);
	for(; h < min(m_pClient->m_Snap.m_pLocalCharacter->m_Health, 10); h++)
		Array[h] = IGraphics::CQuadItem(x+h*12,y,10,10);
	Graphics()->QuadsDrawTL(Array, h);

	i = 0;
	RenderTools()->SelectSprite(SPRITE_HEALTH_EMPTY);
	for(; h < 10; h++)
		Array[i++] = IGraphics::CQuadItem(x+h*12,y,10,10);
	Graphics()->QuadsDrawTL(Array, i);

	// render armor meter
	h = 0;
	RenderTools()->SelectSprite(SPRITE_ARMOR_FULL);
	for(; h < min(m_pClient->m_Snap.m_pLocalCharacter->m_Armor, 10); h++)
		Array[h] = IGraphics::CQuadItem(x+h*12,y+12,10,10);
	Graphics()->QuadsDrawTL(Array, h);

	i = 0;
	RenderTools()->SelectSprite(SPRITE_ARMOR_EMPTY);
	for(; h < 10; h++)
		Array[i++] = IGraphics::CQuadItem(x+h*12,y+12,10,10);
	Graphics()->QuadsDrawTL(Array, i);
	Graphics()->QuadsEnd();
}
	}

void CHud::RenderSpeed()
{
                  /** HUD Part: Teecomp Speedmeter **/

	if(!g_Config.m_tc_speedmeter || !g_Config.m_cl_hud)
		return;

	// We calculate the speed instead of getting it from character.velocity cause it's buggy when
	// walking in front of a wall or when using the ninja sword
	static float speed;
	static vec2 oldpos;
	static const int SMOOTH_TABLE_SIZE = 16;
	static const int ACCEL_THRESHOLD = 25;
	static float smooth_table[SMOOTH_TABLE_SIZE];
	static int smooth_index = 0;

	smooth_table[smooth_index] = distance(m_pClient->m_LocalCharacterPos, oldpos)/Client()->FrameTime();
	/*if(demorec_isplaying()) {
		float mult = client_demoplayer_getinfo()->speed;
		smooth_table[smooth_index] /= mult;
	}*/
	smooth_index = (smooth_index + 1) % SMOOTH_TABLE_SIZE;
	oldpos = m_pClient->m_LocalCharacterPos;
	speed = 0;
	for(int i=0; i<SMOOTH_TABLE_SIZE; i++)
		speed += smooth_table[i];
	speed /= SMOOTH_TABLE_SIZE;

	int t = (m_pClient->m_Snap.m_pGameobj->m_Flags & GAMEFLAG_TEAMS)? -1 : 1;
	int last_index = smooth_index - 1;
	if(last_index < 0)
		last_index = SMOOTH_TABLE_SIZE - 1;

	Graphics()->BlendNormal();;
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	if(g_Config.m_tc_speedmeter_accel && speed - smooth_table[last_index] > ACCEL_THRESHOLD)
		Graphics()->SetColor(0.6f, 0.1f, 0.1f, 0.25f);
	else if(g_Config.m_tc_speedmeter_accel && speed - smooth_table[last_index] < -ACCEL_THRESHOLD)
		Graphics()->SetColor(0.1f, 0.6f, 0.1f, 0.25f);
	else
		Graphics()->SetColor(0.1, 0.1, 0.1, 0.25);
	RenderTools()->DrawRoundRect(m_Width-40, 245+t*20, 50, 18, 5.0f);
	Graphics()->QuadsEnd();

	char buf[16];
	str_format(buf, sizeof(buf), "%.0f", speed);
	TextRender()->Text(0, m_Width-5-TextRender()->TextWidth(0,12,buf,-1), 246+t*20, 12, buf, -1);
}

void CHud::RenderSpectate()
{
if(g_Config.m_cl_hud){                                 /** HUD Part: Teecomp Spec View **/
	if(m_pClient->freeview)
		TextRender()->Text(0, 4*Graphics()->ScreenAspect(), 4, 8, "Freeview", -1);
	else
	{
		char buf[96];
		str_format(buf, sizeof(buf), "Following: %s", m_pClient->m_aClients[m_pClient->spectate_cid].m_aName);
		TextRender()->Text(0, 4*Graphics()->ScreenAspect(), 4, 8, buf, -1);
	}
}
	}

void CHud::OnRender()
{
	if(!m_pClient->m_Snap.m_pGameobj)
		return;
		
	m_Width = 300*Graphics()->ScreenAspect();

	bool Spectate = false;
	if(m_pClient->m_Snap.m_pLocalInfo && m_pClient->m_Snap.m_pLocalInfo->m_Team == -1)
		Spectate = true;
	
	if(m_pClient->m_Snap.m_pLocalCharacter && !Spectate && !(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver))
		RenderHealthAndAmmo();

	RenderGameTimer();
	RenderSuddenDeath();
	RenderScoreHud();
	RenderWarmupTimer();
	RenderFps();
	if(Client()->State() != IClient::STATE_DEMOPLAYBACK)
		RenderConnectionWarning();
	RenderTeambalanceWarning();
	RenderVoting();
	if(Spectate && !(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver))
		RenderSpectate();
	RenderSpeed();
	RenderCursor();
	
}
