#include <string.h>

#include <engine/client.h>

#include <game/generated/protocol.h>
#include <game/generated/client_data.h>

#include <engine/graphics.h>

#include <engine/shared/config.h>

#include <engine/textrender.h>

#include <game/version.h>
#include <game/client/ui.h>
#include <game/client/render.h>
#include <game/client/gameclient.h>
#include <game/client/animstate.h>

#include "binds.h"
#include "menus.h"
#include "maplayers.h"
#include "skins.h"
#include "items.h"

#include <game/client/teecomp.h>

void CMenus::render_rgb_sliders(CUIRect* main_view, CUIRect* button, int &r, int &g, int &b, bool enabled)
{
	const char *labels[] = {"R.", "G.", "B."};
	int *color_slider[3] = {&r, &g, &b};
	for(int i=0; i<3; i++)
	{
		CUIRect text;
		main_view->HSplitTop(19.0f, button, main_view);
		button->VMargin(15.0f, button);
		button->VSplitLeft(30.0f, &text, button);
		button->VSplitRight(5.0f, button, 0);
		button->HSplitTop(4.0f, 0, button);

		if(enabled)
		{
			float k = (*color_slider[i]) / 255.0f;
			k = DoScrollbarH(color_slider[i], button, k);
			*color_slider[i] = (int)(k*255.0f);
		}
		else
			DoScrollbarH(color_slider[i], button, 0);
		UI()->DoLabel(&text, labels[i], 15.0f, -1);
	}
}

void CMenus::ui_do_keybinder(KEYINFO& key, CUIRect* r)
{
	CUIRect label, button;
	r->HSplitTop(20.0f, &button, r);
	button.VSplitRight(5.0f, &button, 0);
	button.VSplitLeft(130.0f, &label, &button);

	UI()->DoLabel(&label, key.name, 14.0f, -1);
	int oldid = key.keyid;
	int newid = DoKeyReader((void *)key.name, &button, oldid);
	if(newid != oldid)
	{
		m_pClient->m_pBinds->Bind(oldid, "");
		m_pClient->m_pBinds->Bind(newid, key.command);
	}
	r->HSplitTop(5.0f, 0, r);
}

void CMenus::render_settings_teecomp(CUIRect main_view)
{
	CUIRect button;
	static int settings_page = 0;

	if(settings_page != 3)
	{
		main_view.HSplitBottom(20.0f, 0, &button);
		button.VSplitLeft(main_view.w/3, &button, 0);
		static int default_button = 0;
		if(DoButton_Menu((void*)&default_button, "Reset to defaults", 0, &button))
			TeecompUtils::reset_config();

		main_view.HSplitBottom(10.0f, &main_view, &button);
		UI()->DoLabel(&button, "http://spl0k.unreal-design.com/teeworlds/", 10.0f, 1);
		main_view.HSplitBottom(10.0f, &main_view, &button);
		char buf[64];
		str_format(buf, sizeof(buf), "Teeworlds %s with TeeComp %s by spl0k", GAME_VERSION, "0.7-beta1");
		UI()->DoLabel(&button, buf, 10.0f, 1);
		main_view.HSplitBottom(10.0f, &main_view, 0);
	}

	// render background
	CUIRect tabbar;
	main_view.HSplitTop(24.0f, &tabbar, &main_view);
	//ui_draw_rect(&main_view, color_tabbar_active, CORNER_ALL, 10.0f);

	const char *tabs[] = {"Skins", "Stats", "Misc", "About", "HUD"};
	int num_tabs = (int)(sizeof(tabs)/sizeof(*tabs));

	for(int i=0; i<num_tabs; i++)
	{
		tabbar.VSplitLeft(10.0f, &button, &tabbar);
		tabbar.VSplitLeft(80.0f, &button, &tabbar);
		int Corners = CUI::CORNER_ALL;
		if(DoButton_MenuTab(tabs[i], tabs[i], settings_page == i, &button, Corners))
			settings_page = i;
	}

	if(settings_page != 3)
		main_view.Margin(10.0f, &main_view);
	
	if(settings_page == 0)
		render_settings_teecomp_skins(main_view);
	else if(settings_page == 1)
		render_settings_teecomp_stats(main_view);
	else if(settings_page == 2)
		render_settings_teecomp_misc(main_view);
	else if(settings_page == 3)
		render_settings_teecomp_about(main_view);
	else if(settings_page == 4)
		render_settings_teecomp_hud(main_view);
}

void CMenus::render_settings_teecomp_skins(CUIRect main_view)
{
	CUIRect button, left_view, right_view;
	main_view.VSplitLeft(main_view.w/2, &left_view, &right_view);

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_colored_tees_method, "Enemy based colors", g_Config.m_tc_colored_tees_method, &button))
		g_Config.m_tc_colored_tees_method ^= 1;

	// Colors team 1

	left_view.HSplitTop(20.0f, &button, &left_view);
	char buf[128];
	str_format(buf, sizeof(buf), "Use DM colors for team %s", (g_Config.m_tc_colored_tees_method)?"mates":"1");
	if(DoButton_CheckBox(&g_Config.m_tc_dm_colors_team1, buf, g_Config.m_tc_dm_colors_team1, &button))
		g_Config.m_tc_dm_colors_team1 ^= 1;

	left_view.HSplitTop(20.0f, &button, &left_view);
	UI()->DoLabel(&button, (g_Config.m_tc_colored_tees_method)?"Team mates":"Team 1", 14.0f, -1);
	int r1, g1, b1, r2, g2, b2;
	r1 = g_Config.m_tc_colored_tees_team1>>16;
	g1 = (g_Config.m_tc_colored_tees_team1>>8)&0xff;
	b1 = g_Config.m_tc_colored_tees_team1&0xff;
	render_rgb_sliders(&left_view, &button, r1, g1, b1, !g_Config.m_tc_dm_colors_team1);
	g_Config.m_tc_colored_tees_team1 = (r1<<16) + (g1<<8) + b1;

	const CSkins::CSkin *s = m_pClient->m_pSkins->Get(max(0, m_pClient->m_pSkins->Find(g_Config.m_tc_forced_skin1)));
	CTeeRenderInfo info;
	if(!g_Config.m_tc_dm_colors_team1)
	{
		info.m_Texture = s->m_ColorTexture;
		info.m_ColorBody = vec4(r1/255.0f, g1/255.0f, b1/255.0f, 1.0f);
		info.m_ColorFeet = vec4(r1/255.0f, g1/255.0f, b1/255.0f, 1.0f);
	}
	else
	{
		info.m_Texture = s->m_OrgTexture;
		info.m_ColorBody = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		info.m_ColorFeet = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	info.m_Size = UI()->Scale()*50.f;

	button.HSplitTop(70.0f, 0, &button);
	RenderTools()->RenderTee(CAnimState::GetIdle(), &info, 0, vec2(1, 0), vec2(button.x, button.y+button.h/2));
	left_view.HSplitTop(50.0f, 0, &left_view);

	// Colors team 2

	right_view.HSplitTop(20.0f, 0, &right_view);
	right_view.HSplitTop(20.0f, &button, &right_view);
	str_format(buf, sizeof(buf), "Use DM colors for %s", (g_Config.m_tc_colored_tees_method)?"enemies":"team 2");
	if(DoButton_CheckBox(&g_Config.m_tc_dm_colors_team2, buf, g_Config.m_tc_dm_colors_team2, &button))
		g_Config.m_tc_dm_colors_team2 ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
	UI()->DoLabel(&button, (g_Config.m_tc_colored_tees_method)?"Enemies":"Team 2", 14.0f, -1);
	r2 = g_Config.m_tc_colored_tees_team2>>16;
	g2 = (g_Config.m_tc_colored_tees_team2>>8)&0xff;
	b2 = g_Config.m_tc_colored_tees_team2&0xff;
	render_rgb_sliders(&right_view, &button, r2, g2, b2, !g_Config.m_tc_dm_colors_team2);
	g_Config.m_tc_colored_tees_team2 = (r2<<16) + (g2<<8) + b2;

	s = m_pClient->m_pSkins->Get(max(0, m_pClient->m_pSkins->Find(g_Config.m_tc_forced_skin2)));
	if(!g_Config.m_tc_dm_colors_team2)
	{
	info.m_Texture = s->m_ColorTexture;
		info.m_ColorBody = vec4(r2/255.0f, g2/255.0f, b2/255.0f, 1.0f);
		info.m_ColorFeet = vec4(r2/255.0f, g2/255.0f, b2/255.0f, 1.0f);
	}
	else
	{
		info.m_Texture = s->m_OrgTexture;
		info.m_ColorBody = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		info.m_ColorFeet = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	button.HSplitTop(70.0f, 0, &button);
	RenderTools()->RenderTee(CAnimState::GetIdle(), &info, 0, vec2(1, 0), vec2(button.x, button.y+button.h/2));
	right_view.HSplitTop(50.0f, 0, &right_view);

	// Force skins team 1

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_forced_skins_method, "Enemy based skins", g_Config.m_tc_forced_skins_method, &button))
		g_Config.m_tc_forced_skins_method ^= 1;

	left_view.HSplitTop(20.0f, &button, &left_view);
	str_format(buf, sizeof(buf), "Force team %s/FFA skins", (g_Config.m_tc_forced_skins_method)?"mates":"1");
	if(DoButton_CheckBox(&g_Config.m_tc_force_skin_team1, buf, g_Config.m_tc_force_skin_team1, &button))
		g_Config.m_tc_force_skin_team1 ^= 1;

	CUIRect skinselection, scroll;
	left_view.Margin(10.0f, &skinselection);

	skinselection.HSplitTop(20, &button, &skinselection);
	RenderTools()->DrawUIRect(&button, vec4(1,1,1,0.25f), CUI::CORNER_T, 5.0f); 
	UI()->DoLabel(&button, "Forced skin", 14.0f, 0);

	RenderTools()->DrawUIRect(&skinselection, vec4(0,0,0,0.15f), 0, 0);
	skinselection.VSplitRight(15, &skinselection, &scroll);

	CUIRect list = skinselection;
	list.HSplitTop(20, &button, &list);
	
	int num = (int)(skinselection.h/button.h);
	static float scrollvalue = 0;
	static int scrollbar = 0;
	scroll.HMargin(5.0f, &scroll);
	scrollvalue = DoScrollbarV(&scrollbar, &scroll, scrollvalue);

	int start = (int)((m_pClient->m_pSkins->Num()-num)*scrollvalue);
	if(start < 0)
		start = 0;

	for(int i=start; i<start+num && i<m_pClient->m_pSkins->Num(); i++)
	{
		const CSkins::CSkin *s = m_pClient->m_pSkins->Get(i);

		str_format(buf, sizeof(buf), "%s", s->m_aName);
		int selected = 0;
		if(strcmp(s->m_aName, g_Config.m_tc_forced_skin1) == 0)
			selected = 1;

		if(DoButton_ListRow(s, "", selected, &button))
			str_copy(g_Config.m_tc_forced_skin1, s->m_aName, sizeof(g_Config.m_tc_forced_skin1));

		button.VMargin(5.0f, &button);
		button.HSplitTop(1.0f, 0, &button);
		UI()->DoLabel(&button, buf, 14.0f, -1);

		list.HSplitTop(20.0f, &button, &list);
	}

	// Forced skin team 2

	right_view.HSplitTop(20.0f, 0, &right_view);
	right_view.HSplitTop(20.0f, &button, &right_view);
	str_format(buf, sizeof(buf), "Force %s skins", (g_Config.m_tc_forced_skins_method)?"enemies":"team 2");
	if(DoButton_CheckBox(&g_Config.m_tc_force_skin_team2, buf, g_Config.m_tc_force_skin_team2, &button))
		g_Config.m_tc_force_skin_team2 ^= 1;

	right_view.Margin(10.0f, &skinselection);

	skinselection.HSplitTop(20, &button, &skinselection);
	RenderTools()->DrawUIRect(&button, vec4(1,1,1,0.25f), CUI::CORNER_T, 5.0f); 
	UI()->DoLabel(&button, "Forced skin", 14.0f, 0);

	RenderTools()->DrawUIRect(&skinselection, vec4(0,0,0,0.15f), 0, 0);
	skinselection.VSplitRight(15, &skinselection, &scroll);

	list = skinselection;
	list.HSplitTop(20, &button, &list);
	
	num = (int)(skinselection.h/button.h);
	static float scrollvalue2 = 0;
	static int scrollbar2 = 0;
	scroll.HMargin(5.0f, &scroll);
	scrollvalue2 = DoScrollbarV(&scrollbar2, &scroll, scrollvalue2);

	start = (int)((m_pClient->m_pSkins->Num()-num)*scrollvalue2);
	if(start < 0)
		start = 0;

	for(int i=start; i<start+num && i<m_pClient->m_pSkins->Num(); i++)
	{
		const CSkins::CSkin *s = m_pClient->m_pSkins->Get(i);

		str_format(buf, sizeof(buf), "%s", s->m_aName);
		int selected = 0;
		if(strcmp(s->m_aName, g_Config.m_tc_forced_skin2) == 0)
			selected = 1;

		if(DoButton_ListRow(s+m_pClient->m_pSkins->Num(), "", selected, &button))
			str_copy(g_Config.m_tc_forced_skin2, s->m_aName, sizeof(g_Config.m_tc_forced_skin2));

		button.VMargin(5.0f, &button);
		button.HSplitTop(1.0f, 0, &button);
		UI()->DoLabel(&button, buf, 14.0f, -1);

		list.HSplitTop(20.0f, &button, &list);
	}
}

void CMenus::render_settings_teecomp_stats(CUIRect main_view)
{
	CUIRect button, left_view;

	main_view.VSplitLeft(main_view.w/2, &left_view, &main_view);

	left_view.HSplitTop(20.0f, &button, &left_view);
	UI()->DoLabel(&button, "Show in global statboard:", 16.0f, -1);

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos, "Frags", g_Config.m_tc_statboard_infos & TC_STATS_FRAGS, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_FRAGS;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+1, "Deaths", g_Config.m_tc_statboard_infos & TC_STATS_DEATHS, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_DEATHS;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+2, "Suicides", g_Config.m_tc_statboard_infos & TC_STATS_SUICIDES, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_SUICIDES;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+3, "Ratio", g_Config.m_tc_statboard_infos & TC_STATS_RATIO, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_RATIO;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+4, "Net score", g_Config.m_tc_statboard_infos & TC_STATS_NET, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_NET;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+5, "Frags per minute", g_Config.m_tc_statboard_infos & TC_STATS_FPM, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_FPM;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+6, "Weapons stats", g_Config.m_tc_statboard_infos & TC_STATS_WEAPS, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_WEAPS;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_statboard_infos+7, "Flag captures", g_Config.m_tc_statboard_infos & TC_STATS_FLAGS, &button))
		g_Config.m_tc_statboard_infos ^= TC_STATS_FLAGS;

	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "Key bindings:", 16.0f, -1);
	KEYINFO keys[] = {{ "Global statboard:", "+stats 1", 0},
		{ "Individual statboard:", "+stats 2", 0},
		{ "Next individual stats:", "+next_stats", 0}};

	for(int keyid=0; keyid<KEY_LAST; keyid++)
	{
		const char *bind = m_pClient->m_pBinds->Get(keyid);
		if(!bind[0])
			continue;

		for(int i=0; i<sizeof(keys)/sizeof(KEYINFO); i++)
			if(strcmp(bind, keys[i].command) == 0)
			{
				keys[i].keyid = keyid;
				break;
			}
	}

	for(int i=0; i<sizeof(keys)/sizeof(KEYINFO); i++)
		ui_do_keybinder(keys[i], &main_view);
}

void CMenus::render_settings_teecomp_misc(CUIRect main_view)
{
	CUIRect left_view, right_view, button;

	main_view.VSplitLeft(main_view.w/2, &left_view, &right_view);

	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "Name plates", 16.0f, 0);
	left_view.HSplitTop(20.0f, 0, &left_view);
	right_view.HSplitTop(20.0f, 0, &right_view);

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_colored_nameplates, "Colored name plates", g_Config.m_tc_colored_nameplates&1, &button))
		g_Config.m_tc_colored_nameplates ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
	if(DoButton_CheckBox(&g_Config.m_tc_colored_nameplates+1, "Enemy based colors", g_Config.m_tc_colored_nameplates&2, &button))
		g_Config.m_tc_colored_nameplates ^= 2;

	left_view.HSplitTop(20.0f, &button, &left_view);
	UI()->DoLabel(&button, (g_Config.m_tc_colored_nameplates&2)?"Team mates":"Team 1", 14.0f, -1);
	int r1, g1, b1, r2, g2, b2;
	r1 = g_Config.m_tc_colored_nameplates_team1>>16;
	g1 = (g_Config.m_tc_colored_nameplates_team1>>8)&0xff;
	b1 = g_Config.m_tc_colored_nameplates_team1&0xff;
	render_rgb_sliders(&left_view, &button, r1, g1, b1, true);
	g_Config.m_tc_colored_nameplates_team1 = (r1<<16) + (g1<<8) + b1;

	left_view.HSplitTop(25.0f, &button, &left_view);
	button.VSplitLeft(30.0f, 0, &button);
	TextRender()->TextColor(r1/255.0f, g1/255.0f, b1/255.0f, 1);
	UI()->DoLabel(&button, (g_Config.m_tc_colored_nameplates&2)?"Team mates":"Team 1", 15.0f, -1);
	TextRender()->TextColor(1,1,1,1);

	right_view.HSplitTop(20.0f, &button, &right_view);
	UI()->DoLabel(&button, (g_Config.m_tc_colored_nameplates&2)?"Enemies":"Team 2", 14.0f, -1);
	r2 = g_Config.m_tc_colored_nameplates_team2>>16;
	g2 = (g_Config.m_tc_colored_nameplates_team2>>8)&0xff;
	b2 = g_Config.m_tc_colored_nameplates_team2&0xff;
	render_rgb_sliders(&right_view, &button, r2, g2, b2, true);
	g_Config.m_tc_colored_nameplates_team2 = (r2<<16) + (g2<<8) + b2;

	right_view.HSplitTop(25.0f, &button, &right_view);
	button.VSplitLeft(30.0f, 0, &button);
	TextRender()->TextColor(r2/255.0f, g2/255.0f, b2/255.0f, 1);
	UI()->DoLabel(&button, (g_Config.m_tc_colored_nameplates&2)?"Enemies":"Team 2", 15.0f, -1);
	TextRender()->TextColor(1,1,1,1);

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_nameplate_shadow, "Show name plate shadow", g_Config.m_tc_nameplate_shadow, &button))
		g_Config.m_tc_nameplate_shadow ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
	if(DoButton_CheckBox(&g_Config.m_tc_nameplate_score, "Show score in name plate", g_Config.m_tc_nameplate_score, &button))
		g_Config.m_tc_nameplate_score ^= 1;

	main_view.HSplitTop(160.0f, 0, &main_view);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "Laser", 16.0f, 0);
	left_view.HSplitTop(40.0f, 0, &left_view);
	right_view.HSplitTop(40.0f, 0, &right_view);

	int lri, lro, lgi, lgo, lbi, lbo;
	lri = g_Config.m_tc_laser_color_inner>>16;
	lgi = (g_Config.m_tc_laser_color_inner>>8)&0xff;
	lbi = g_Config.m_tc_laser_color_inner&0xff;
	left_view.HSplitTop(20.0f, &button, &left_view);
	UI()->DoLabel(&button, "Laser inner color", 14.0f, -1);
	render_rgb_sliders(&left_view, &button, lri, lgi, lbi, true);
	g_Config.m_tc_laser_color_inner = (lri<<16) + (lgi<<8) + lbi;

	lro = g_Config.m_tc_laser_color_outer>>16;
	lgo = (g_Config.m_tc_laser_color_outer>>8)&0xff;
	lbo = g_Config.m_tc_laser_color_outer&0xff;
	right_view.HSplitTop(20.0f, &button, &right_view);
	UI()->DoLabel(&button, "Laser outer color", 14.0f, -1);
	render_rgb_sliders(&right_view, &button, lro, lgo, lbo, true);
	g_Config.m_tc_laser_color_outer = (lro<<16) + (lgo<<8) + lbo;

	{ CUIRect lbut, rbut;
	CUIRect screen = *UI()->Screen();
	left_view.HSplitTop(20.0f, &lbut, &left_view);
	right_view.HSplitTop(20.0f, &rbut, &right_view);
	// calculate world screen mapping
	float points[4];
	//mapscreen_to_world(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Graphics()->ScreenAspect(), 1.0f, points);
	RenderTools()->MapscreenToWorld(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Graphics()->ScreenAspect(), 1.0f, points);
	points[2] = points[2] - points[0];
	points[3] = points[3] - points[1];
	points[0] = 0.0f;
	points[1] = 0.0f;
	// factor between world and menu screen mappings
	float fact_x, fact_y;
	fact_x = (points[2]-points[0])/screen.w;
	fact_y = (points[3]-points[1])/screen.h;
	struct CNetObj_Laser laser;
	
	// we want to draw a beam from under the center of one sliders section to the center of the other sliders section
	laser.m_FromX = (lbut.x + lbut.w/2)*fact_x;
	laser.m_FromY = (lbut.y + lbut.h/2)*fact_y;
	laser.m_X = (rbut.x + rbut.w/2)*fact_x;
	laser.m_Y = (rbut.y + rbut.h/2)*fact_y;
	laser.m_StartTick = Client()->GameTick() + Client()->IntraGameTick();
	// apply world screen mapping (beam is bigger in menu mapping)
	Graphics()->MapScreen(points[0], points[1], points[2], points[3]);
	// draw laser
	//CItems::RenderLaser(&laser);
	// restore menu screen mapping
	Graphics()->MapScreen(screen.x, screen.y, screen.w, screen.h); }

	left_view.HSplitTop(20.0f, 0, &left_view);
	left_view.HSplitTop(20.0f, &button, &left_view);
	UI()->DoLabel(&button, "HUD/Flag", 16.0f, -1);

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_hud_match, "Make HUD match tees colors", g_Config.m_tc_hud_match, &button))
		g_Config.m_tc_hud_match ^= 1;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_colored_flags, "Make flags match tees colors", g_Config.m_tc_colored_flags, &button))
		g_Config.m_tc_colored_flags ^= 1;

	left_view.HSplitTop(20.0f, &button, &left_view);
	if(DoButton_CheckBox(&g_Config.m_tc_speedmeter, "Display speed meter", g_Config.m_tc_speedmeter, &button))
		g_Config.m_tc_speedmeter ^= 1;

	left_view.HSplitTop(20.0f, &button, &left_view);
	button.VSplitLeft(15.0f, 0, &button);
	if(DoButton_CheckBox(&g_Config.m_tc_speedmeter_accel, "Speed meter show acceleration", g_Config.m_tc_speedmeter_accel, &button))
		g_Config.m_tc_speedmeter_accel ^= 1;

	right_view.HSplitTop(20.0f, 0, &right_view);
	right_view.HSplitTop(20.0f, &button, &right_view);
	UI()->DoLabel(&button, "Others", 16.0f, -1);

	right_view.HSplitTop(20.0f, &button, &right_view);
	if(DoButton_CheckBox(&g_Config.m_tc_disable_chat, "Disable chat", g_Config.m_tc_disable_chat, &button))
		g_Config.m_tc_disable_chat ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
	if(DoButton_CheckBox(&g_Config.m_tc_hide_carrying, "Hide flag while carrying it", g_Config.m_tc_hide_carrying, &button))
		g_Config.m_tc_hide_carrying ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
	if(DoButton_CheckBox(&g_Config.m_tc_autodemo, "Enable automatic demo recording", g_Config.m_tc_autodemo, &button))
		g_Config.m_tc_autodemo ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
	if(DoButton_CheckBox(&g_Config.m_tc_autoscreen, "Enable end game automatic screenshot", g_Config.m_tc_autoscreen, &button))
		g_Config.m_tc_autoscreen ^= 1;
		
		
		


	
}

void CMenus::render_settings_teecomp_about(CUIRect main_view)
{
	CUIRect button;

	main_view.HSplitTop(52.0f, &button, &main_view);
	UI()->DoLabel(&button, "TeeComp", 48.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	button.VSplitRight(button.w/3, 0, &button);
	UI()->DoLabel(&button, "Version 0.7-beta1", 14.0f, -1);
	main_view.HSplitTop(20.0f, &button, &main_view);
	button.VSplitRight(button.w/3, 0, &button);
	UI()->DoLabel(&button, "For Teeworlds "GAME_VERSION, 14.0f, -1);
	main_view.HSplitTop(20.0f, &button, &main_view);
	button.VSplitRight(button.w/3, 0, &button);
	UI()->DoLabel(&button, "Compiled "__DATE__" "__TIME__, 14.0f, -1);

	main_view.HSplitTop(40.0f, 0, &main_view);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "By Alban 'spl0k' FERON", 14.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "http://spl0k.unreal-design.com/", 14.0f, 0);

	main_view.HSplitTop(20.0f, 0, &main_view);
	main_view.HSplitTop(22.0f, &button, &main_view);
	UI()->DoLabel(&button, "Special thanks to:", 16.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "Sd`", 14.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "Tho", 14.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "Eve", 14.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "some other MonkeyStyle members", 14.0f, 0);
	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "and the Teeworlds.com community", 14.0f, 0);

	main_view.HSplitBottom(10.0f, &main_view, &button);
	UI()->DoLabel(&button, "so you can set while u set while u set options", 10.0f, -1);
	main_view.HSplitBottom(10.0f, &main_view, &button);
	UI()->DoLabel(&button, "Yo dawg I herd you like tabs so we put tabs in yo tabs in yo tabs", 10.0f, -1);
}

void CMenus::render_settings_teecomp_hud(CUIRect main_view)
{
CUIRect left_view, right_view;
CUIRect button;
CUIRect label;
		

	

	main_view.VSplitLeft(main_view.w/2, &left_view, &right_view);

	main_view.HSplitTop(20.0f, &button, &main_view);
	UI()->DoLabel(&button, "HUD Settings", 24.0f, 0);
	left_view.HSplitTop(20.0f, 0, &left_view);
	right_view.HSplitTop(20.0f, 0, &right_view);

	left_view.HSplitTop(22.0f, &button, &left_view);
		if (DoButton_CheckBox(&g_Config.m_cl_hud, "HUD overall", g_Config.m_cl_hud, &button))
			g_Config.m_cl_hud ^= 1;
	left_view.HSplitTop(20.0f, &button, &left_view);
		UI()->DoLabel(&button, "This will ignore any other settings!", 16.0f, 0);
		
	right_view.HSplitTop(20.0f, &button, &right_view);
		UI()->DoLabel(&button, "", 16.0f, 0);
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_hp_ammo, "HP/AMMO", g_Config.m_hud_hp_ammo, &button))
			g_Config.m_hud_hp_ammo ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_cursor, "Aimcursor", g_Config.m_hud_cursor, &button))
			g_Config.m_hud_cursor ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_tc_speedmeter, "Teecomp Speedmeter", g_Config.m_tc_speedmeter, &button))
			g_Config.m_tc_speedmeter ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_goals, "Goal view", g_Config.m_hud_goals, &button))
			g_Config.m_hud_goals ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_voting, "Show running votes", g_Config.m_hud_voting, &button))
			g_Config.m_hud_voting ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_teambalance, "Teambalance warning", g_Config.m_hud_teambalance, &button))
			g_Config.m_hud_teambalance ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_killmsg, "Killmessages", g_Config.m_hud_killmsg, &button))
			g_Config.m_hud_killmsg ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_tc_disable_chat, "Chat", g_Config.m_tc_disable_chat, &button))
			g_Config.m_tc_disable_chat ^= 1;
	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_newping, "Ping view with Time", g_Config.m_hud_newping, &button))
			g_Config.m_hud_newping ^= 1;

	right_view.HSplitTop(20.0f, &button, &right_view);
		if (DoButton_CheckBox(&g_Config.m_hud_show_own_nameplate, "Show own nameplate", g_Config.m_hud_show_own_nameplate, &button))
			g_Config.m_hud_show_own_nameplate ^= 1;	
		
			
			
			

		
		left_view.HSplitTop(19.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(4.0f, 0, &button);
		
		UI()->DoLabel(&label, "Broadcast message size", 14.0f, -1);
		
		
		left_view.HSplitTop(20.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(5.0f, 0, &button);
		g_Config.m_cl_broadcast_size = (int)(DoScrollbarH(&g_Config.m_cl_broadcast_size, &button, g_Config.m_cl_broadcast_size/100.0f)*100.0f);
		
		
		left_view.HSplitTop(25.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(7.0f, 0, &button);
		
		UI()->DoLabel(&label, "This is an example", g_Config.m_cl_broadcast_size*2, -1);
		
		
		
		left_view.HSplitTop(19.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(4.0f, 0, &button);
		
		UI()->DoLabel(&label, "Warmup message size", 14.0f, -1);
		
		
		left_view.HSplitTop(20.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(5.0f, 0, &button);
		g_Config.m_cl_warmup_size = (int)(DoScrollbarH(&g_Config.m_cl_warmup_size, &button, g_Config.m_cl_warmup_size/100.0f)*100.0f);
		
		
		left_view.HSplitTop(25.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(7.0f, 0, &button);
		
		UI()->DoLabel(&label, "Warmup", g_Config.m_cl_warmup_size*2, -1);
		
		
		left_view.HSplitTop(27.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(9.0f, 0, &button);
		
		UI()->DoLabel(&label, "10", g_Config.m_cl_warmup_size*2, -1);

	right_view.HSplitTop(20.0f, &button, &right_view);
       // ZWEITER BUTTON HIER

/*		main_view.HSplitTop(12.0f, &button, &main_view);
		button.VMargin(12.0f, &button);
		button.VSplitLeft(18.0f, &label, &button);
		button.VSplitRight(3.0f, &button, 0);
		button.HSplitTop(8.0f, 0, &button);*/
		
		
		
		left_view.HSplitTop(19.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(4.0f, 0, &button);
		
		UI()->DoLabel(&label, "Chat font size", 14.0f, -1);
		
		
		left_view.HSplitTop(20.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(5.0f, 0, &button);
		g_Config.m_cl_chatsize = (int)(DoScrollbarH(&g_Config.m_cl_chatsize, &button, g_Config.m_cl_chatsize/100.0f)*100.0f);
		
		
		left_view.HSplitTop(25.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(7.0f, 0, &button);
		
		UI()->DoLabel(&label, "[Krypton] EdgeCru$her: omg teh suck", g_Config.m_cl_chatsize*2, -1);
		
		
		
		
		

		left_view.HSplitTop(25.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(7.0f, 0, &button);
		UI()->DoLabel(&label, "Speczoom", 14.0f, -1);
		
		
		
	
		
		
		left_view.HSplitTop(25.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(7.0f, 0, &button);
		UI()->DoLabel(&label, "The higher the value, the bigger the sight range is on spectate", 10.0f, -1);
		
		
		
		left_view.HSplitTop(25.0f, &button, &left_view);
		button.VMargin(15.0f, &button);
		button.VSplitLeft(30.0f, &label, &button);
		button.VSplitRight(5.0f, &button, 0);
		button.HSplitTop(7.0f, 0, &button);
		
		g_Config.m_hud_speczoom = (int)(DoScrollbarH(&g_Config.m_hud_speczoom, &button, g_Config.m_hud_speczoom/100.0f)*100.0f);
		
		


	


}
