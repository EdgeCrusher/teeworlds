#include <engine/client.h>
#include <game/generated/protocol.h>
#include <game/generated/client_data.h>
#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/client/teecomp.h>
#include "teecomp_stats.h"

CTeecompStats::CTeecompStats()
{
	OnReset();
}

void CTeecompStats::OnReset()
{
	//for(int i=0; i<MAX_CLIENTS; i++)
		//done by the gameclient component
//
}
/*
void CTeecompStats::con_key_stats(void *result, void *user_data)
{
	//((CTeecompStats *)user_data)->active = console_arg_int(result, 0) != 0;
	if(console_arg_int(result, 0) != 0)
		((CTeecompStats *)user_data)->mode = console_arg_int(result, 1);
	else
		((CTeecompStats *)user_data)->mode = 0;
}

void CTeecompStats::con_key_next(void *result, void *user_data)
{
	//((CTeecompStats *)user_data)->active = console_arg_int(result, 0) != 0;
	CTeecompStats *stats = (CTeecompStats *)user_data;
	if(stats->mode != 2)
		return;

	if(console_arg_int(result, 0) == 0)
	{
		stats->stats_cid++;
		stats->stats_cid %= MAX_CLIENTS;
		stats->check_stats_cid();
	}
}

void CTeecompStats::on_console_init()
{
// not used
}

bool CTeecompStats::is_active() {
	return (mode > 0);
}

void CTeecompStats::check_stats_cid()
{
	if(stats_cid == -1)
		stats_cid = gameclient.snap.local_cid;

	int prev = stats_cid;
	while(!m_pClient->stats[stats_cid].active)
	{
		stats_cid++;
		stats_cid %= MAX_CLIENTS;
		if(stats_cid == prev)
		{
			stats_cid = -1;
			mode = 0;
			break;
		}
	}
}*/

void CTeecompStats::OnMessage(int msgtype, void *rawmsg)
{
	if(msgtype == NETMSGTYPE_SV_KILLMSG)
	{
		CNetMsg_Sv_KillMsg *msg = (CNetMsg_Sv_KillMsg *)rawmsg;

		m_pClient->m_aClients[msg->m_Victim].m_Deaths++;
		if(msg->m_Weapon >= 0)
			m_pClient->m_aClients[msg->m_Victim].m_DeathsFrom[msg->m_Weapon]++;
		if(msg->m_ModeSpecial & 1)
			m_pClient->m_aClients[msg->m_Victim].m_DeathsCarrying++;
		if(msg->m_Victim != msg->m_Killer)
		{
			m_pClient->m_aClients[msg->m_Killer].m_Frags++;
			if(msg->m_Weapon >= 0)
				m_pClient->m_aClients[msg->m_Killer].m_FragsWith[msg->m_Weapon]++;
			if(msg->m_ModeSpecial & 1)
				m_pClient->m_aClients[msg->m_Killer].m_CarriersKilled++;
			if(msg->m_ModeSpecial & 2)
				m_pClient->m_aClients[msg->m_Killer].m_KillsCarrying++;
		}
		else
			m_pClient->m_aClients[msg->m_Victim].m_Suicides++;
	}
	else if(msgtype == NETMSGTYPE_SV_CHAT)
	{
		CNetMsg_Sv_Chat *msg = (CNetMsg_Sv_Chat *)rawmsg;
		if(msg->m_Cid < 0)
		{
			const char *p;
			const char *look_for = "flag was captured by ";
			if(p = str_find_nocase(msg->m_pMessage, look_for))
			{
				char name[64];
				p += str_length(look_for);
				str_copy(name, p, sizeof(name));
				if(str_comp_nocase(name+str_length(name)-9, " seconds)") == 0)
				{
					char *c = name+str_length(name)-10;
					while(c > name)
					{
						c--;
						if(*c == '(')
						{
							*(c-1) = 0;
							break;
						}
					}
				}
				for(int i=0; i<MAX_CLIENTS; i++)
				{
					if(!m_pClient->m_aClients[i].m_Active)
						continue;

					if(str_comp_nocase(m_pClient->m_aClients[i].m_aName, name) == 0)
					{
						m_pClient->m_aClients[i].m_FlagCaptures++;
						break;
					}
				}
			}
		}
	}
}

/*void CTeecompStats::on_render()
{
	switch(mode)
	{
		case 1:
			render_global_stats();
			break;
		case 2:
			render_individual_stats();
			break;
	}
}

void CTeecompStats::render_global_stats()
{
	if(mode != 1)
		return;

	float width = 400*3.0f*gfx_screenaspect();
	float height = 400*3.0f;
	float w = 450.0f;
	float h = 750.0f;
	
	const NETOBJ_PLAYER_INFO *players[MAX_CLIENTS] = {0};
	int num_players = 0;
	int i;
	for(i=0; i<snap_num_items(SNAP_CURRENT); i++)
	{
		SNAP_ITEM item;
		const void *data = snap_get_item(SNAP_CURRENT, i, &item);

		if(item.type == NETOBJTYPE_PLAYER_INFO)
		{
			const NETOBJ_PLAYER_INFO *info = (const NETOBJ_PLAYER_INFO *)data;

			if(!m_pClient->stats[info->cid].active)
				continue;

			players[num_players] = info;
			num_players++;
		}
	}

	for(int i=0; i<6; i++)
		if(config.tc_statboard_infos & (1<<i))
			w += 100;
	if(gameclient.snap.gameobj && gameclient.snap.gameobj->flags&GAMEFLAG_FLAGS && config.tc_statboard_infos&TC_STATS_FLAGS)
		w += 100;

	bool display_m_Weapon[NUM_m_WeaponS] = {false};
	if(config.tc_statboard_infos & TC_STATS_WEAPS)
	{
		for(i=0; i<num_players; i++)
		{
			const GAMECLIENT::CLIENT_STATS stats = m_pClient->stats[players[i]->cid];
			for(int j=0; j<NUM_m_WeaponS; j++)
				display_m_Weapon[j] = display_m_Weapon[j] || stats.m_FragsWith[j] || stats.m_DeathsFrom[j];
		}
		for(i=0; i<NUM_m_WeaponS; i++)
			if(display_m_Weapon[i])
				w += 120;
	}

	float x = width/2-w/2;
	float y = 200.0f;

	gfx_mapscreen(0, 0, width, height);

	gfx_blend_normal();
	gfx_texture_set(-1);
	gfx_quads_begin();
	gfx_setcolor(0,0,0,0.5f);
	draw_round_rect(x-10.f, y-10.f, w, h, 17.0f);
	gfx_quads_end();

	float tw;
	int px = 525;

	gfx_text(0, x+10, y, 24.0f, "Name", -1);
	const char *headers[] = { "m_Frags", "Deaths", "m_Suicides", "Ratio", "Net", "FPM" };
	for(i=0; i<6; i++)
		if(config.tc_statboard_infos & (1<<i))
		{
			tw = gfx_text_width(0, 24.0f, headers[i], -1);
			gfx_text(0, x+px-tw, y, 24.0f, headers[i], -1);
			px += 100;
		}

	if(config.tc_statboard_infos & TC_STATS_WEAPS)
	{
		gfx_texture_set(data->images[IMAGE_GAME].id);
		gfx_quads_begin();
		for(i=0, px-=40; i<NUM_m_WeaponS; i++)
		{
			if(!display_m_Weapon[i])
				continue;

			select_sprite(data->m_Weapons.id[i].sprite_body, 0);
			if(i == 0)
				draw_sprite(x+px, y+8, data->m_Weapons.id[i].visual_size*0.8);
			else
				draw_sprite(x+px, y+8, data->m_Weapons.id[i].visual_size);
			px += 120;
		}
		gfx_quads_end();
		px += 40;
	}

	if(gameclient.snap.gameobj && gameclient.snap.gameobj->flags&GAMEFLAG_FLAGS && config.tc_statboard_infos&TC_STATS_FLAGS)
	{
		px -= 40;
		gfx_texture_set(data->images[IMAGE_GAME].id);
		gfx_quads_begin();
		gfx_quads_setrotation(0.78f);
		select_sprite(SPRITE_FLAG_RED);
		draw_sprite(x+px, y+10, 48);
		gfx_quads_end();
	}

	y += 29.0f;

	float font_size = 30.0f;
	float line_height = 50.0f;
	float tee_sizemod = 1.0f;
	float tee_offset = 0.0f;
	
	if(num_players > 14)
	{
		font_size = 30.0f;
		line_height = 40.0f;
		tee_sizemod = 0.8f;
		tee_offset = -5.0f;
	}

	for(int j=0; j<num_players; j++)
	{
		const NETOBJ_PLAYER_INFO *info = players[j];
		const GAMECLIENT::CLIENT_STATS stats = m_pClient->stats[info->cid];

		if(info->local)
		{
			// background so it's easy to find the local player
			gfx_texture_set(-1);
			gfx_quads_begin();
			gfx_setcolor(1,1,1,0.25f);
			draw_round_rect(x, y, w-20, line_height*0.95f, 17.0f);
			gfx_quads_end();
		}

		TEE_RENDER_INFO teeinfo = gameclient.clients[info->cid].render_info;
		teeinfo.size *= tee_sizemod;
		render_tee(ANIMSTATE::get_idle(), &teeinfo, EMOTE_NORMAL, vec2(1,0), vec2(x+28, y+28+tee_offset));

		char buf[128];
		if(config.tc_stat_id)
		{
			str_format(buf, sizeof(buf), "%d", info->cid);
			gfx_text(0, x, y, font_size, buf, -1);
		}

		gfx_text(0, x+64, y, font_size, gameclient.clients[info->cid].name, -1);

		px = 525;

		if(config.tc_statboard_infos & TC_STATS_m_Frags)
		{
			str_format(buf, sizeof(buf), "%d", stats.m_Frags);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		if(config.tc_statboard_infos & TC_STATS_DEATHS)
		{
			str_format(buf, sizeof(buf), "%d", stats.m_Deaths);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		if(config.tc_statboard_infos & TC_STATS_m_Suicides)
		{
			str_format(buf, sizeof(buf), "%d", stats.m_Suicides);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		if(config.tc_statboard_infos & TC_STATS_RATIO)
		{
			if(stats.m_Deaths == 0)
				str_format(buf, sizeof(buf), "--");
			else
				str_format(buf, sizeof(buf), "%.2f", (float)(stats.m_Frags)/stats.m_Deaths);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		if(config.tc_statboard_infos & TC_STATS_NET)
		{
			str_format(buf, sizeof(buf), "%+d", stats.m_Frags-stats.m_Deaths);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		if(config.tc_statboard_infos & TC_STATS_FPM)
		{
			float fpm = (float)(stats.m_Frags*60)/((float)(client_tick()-stats.join_date)/client_tickspeed());
			str_format(buf, sizeof(buf), "%.1f", fpm);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		for(i=0, px=px-40; i<NUM_m_WeaponS; i++)
		{
			if(!display_m_Weapon[i])
				continue;

			str_format(buf, sizeof(buf), "%d/%d", stats.m_FragsWith[i], stats.m_DeathsFrom[i]);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x+px-tw/2, y, font_size, buf, -1);
			px += 120;
		}
		if(gameclient.snap.gameobj && gameclient.snap.gameobj->flags&GAMEFLAG_FLAGS && config.tc_statboard_infos&TC_STATS_FLAGS)
		{
			str_format(buf, sizeof(buf), "%d", stats.flag_captures);
			tw = gfx_text_width(0, font_size, buf, -1);
			gfx_text(0, x-tw+px, y, font_size, buf, -1);
			px += 100;
		}
		y += line_height;
	}
}

void CTeecompStats::render_individual_stats()
{
	if(mode != 2)
		return;
	check_stats_cid();
	if(mode != 2)
		return;
	int cid = stats_cid;
	float width = 400*3.0f*gfx_screenaspect();
	float height = 400*3.0f;
	float w = 1200.0f;
	float x = width/2-w/2;
	float y = 100.0f;
	float xo = 200.0f;
	float font_size = 30.0f;
	float line_height = 40.0f;
	const GAMECLIENT::CLIENT_STATS stats = m_pClient->stats[cid];

	gfx_mapscreen(0, 0, width, height);

	// header with name and score
	gfx_blend_normal();
	gfx_texture_set(-1);
	gfx_quads_begin();
	gfx_setcolor(0,0,0,0.5f);
	draw_round_rect(x-10.f, y-10.f, w, 120.0f, 17.0f);
	gfx_quads_end();

	TEE_RENDER_INFO teeinfo = gameclient.clients[cid].render_info;
	teeinfo.size *= 1.5f;
	render_tee(ANIMSTATE::get_idle(), &teeinfo, EMOTE_NORMAL, vec2(1,0), vec2(x+xo+32, y+36));
	gfx_text(0, x+xo+128, y, 48.0f, gameclient.clients[cid].name, -1);

	char buf[64];
	if(config.tc_stat_id)
	{
		str_format(buf, sizeof(buf), "%d", cid);
		gfx_text(0, x+xo, y, font_size, buf, -1);
	}

	str_format(buf, sizeof(buf), "Score: %d", gameclient.snap.player_infos[cid]->score);
	gfx_text(0, x+xo, y+64, font_size, buf, -1);
	int seconds = (float)(client_tick()-stats.join_date)/client_tickspeed();
	str_format(buf, sizeof(buf), "Time played: %02d:%02d", seconds/60, seconds%60);
	gfx_text(0, x+xo+256, y+64, font_size, buf, -1);

	y += 150.0f;

	// m_Frags, etc. stats
	gfx_blend_normal();
	gfx_texture_set(-1);
	gfx_quads_begin();
	gfx_setcolor(0,0,0,0.5f);
	draw_round_rect(x-10.f, y-10.f, w, 100.0f, 17.0f);
	gfx_quads_end();

	gfx_texture_set(data->images[IMAGE_EMOTICONS].id);
	gfx_quads_begin();
	gfx_setcolor(1.0f,1.0f,1.0f,1.0f);
	select_sprite(SPRITE_DEADTEE);
	gfx_quads_draw(x+xo/2, y+40, 128, 128);
	gfx_quads_end();

	str_format(buf, sizeof(buf), "m_Frags: %d", stats.m_Frags);
	gfx_text(0, x+xo, y, font_size, buf, -1);
	str_format(buf, sizeof(buf), "Deaths: %d", stats.m_Deaths);
	gfx_text(0, x+xo+200.0f, y, font_size, buf, -1);
	str_format(buf, sizeof(buf), "m_Suicides: %d", stats.m_Suicides);
	gfx_text(0, x+xo+400.0f, y, font_size, buf, -1);
	y += line_height;

	if(stats.m_Deaths == 0)
		str_format(buf, sizeof(buf), "Ratio: --");
	else
		str_format(buf, sizeof(buf), "Ratio: %.2f", (float)(stats.m_Frags)/stats.m_Deaths);
	gfx_text(0, x+xo, y, font_size, buf, -1);
	str_format(buf, sizeof(buf), "Net: %d", stats.m_Frags-stats.m_Deaths);
	gfx_text(0, x+xo+200.0f, y, font_size, buf, -1);
	float fpm = (float)(stats.m_Frags*60)/((float)(client_tick()-stats.join_date)/client_tickspeed());
	str_format(buf, sizeof(buf), "FPM: %.1f", fpm);
	gfx_text(0, x+xo+400.0f, y, font_size, buf, -1);
	y+= line_height + 30.0f;

	// m_Weapon stats
	bool display_m_Weapon[NUM_m_WeaponS] = {false};
	int num_weaps = 0;
	for(int i=0; i<NUM_m_WeaponS; i++)
		if(stats.m_FragsWith[i] || stats.m_DeathsFrom[i])
		{
			display_m_Weapon[i] = true;
			num_weaps++;
		}

	if(num_weaps)
	{
		gfx_blend_normal();
		gfx_texture_set(-1);
		gfx_quads_begin();
		gfx_setcolor(0,0,0,0.5f);
		draw_round_rect(x-10.f, y-10.f, w, line_height*(1+num_weaps)+20.0f, 17.0f);
		gfx_quads_end();

		gfx_text(0, x+xo, y, font_size, "m_Frags", -1);
		gfx_text(0, x+xo+200.0f, y, font_size, "Deaths", -1);
		y += line_height;

		for(int i=0; i<NUM_m_WeaponS; i++)
		{
			if(!display_m_Weapon[i])
				continue;

			gfx_texture_set(data->images[IMAGE_GAME].id);
			gfx_quads_begin();
			select_sprite(data->m_Weapons.id[i].sprite_body, 0);
			draw_sprite(x+xo/2, y+24, data->m_Weapons.id[i].visual_size);
			gfx_quads_end();

			str_format(buf, sizeof(buf), "%d", stats.m_FragsWith[i]);
			gfx_text(0, x+xo, y, font_size, buf, -1);
			str_format(buf, sizeof(buf), "%d", stats.m_DeathsFrom[i]);
			gfx_text(0, x+xo+200.0f, y, font_size, buf, -1);
			y += line_height;
		}
		y += 30.0f;
	}

	// Flag stats
	if(gameclient.snap.gameobj && gameclient.snap.gameobj->flags&GAMEFLAG_FLAGS)
	{
		gfx_blend_normal();
		gfx_texture_set(-1);
		gfx_quads_begin();
		gfx_setcolor(0,0,0,0.5f);
		draw_round_rect(x-10.f, y-10.f, w, line_height*5+20.0f, 17.0f);
		gfx_quads_end();

		gfx_texture_set(data->images[IMAGE_GAME].id);
		gfx_quads_begin();
		select_sprite(SPRITE_FLAG_RED);
		draw_sprite(x+xo/2, y+100.0f, 192);
		gfx_quads_end();

		str_format(buf, sizeof(buf), "Grabs: %d", stats.flag_grabs);
		gfx_text(0, x+xo, y, font_size, buf, -1);
		y += line_height;
		str_format(buf, sizeof(buf), "Captures: %d", stats.flag_captures);
		gfx_text(0, x+xo, y, font_size, buf, -1);
		y += line_height;
		str_format(buf, sizeof(buf), "Kills holding flag: %d", stats.kills_carrying);
		gfx_text(0, x+xo, y, font_size, buf, -1);
		y += line_height;
		str_format(buf, sizeof(buf), "Deaths with flag: %d", stats.m_DeathsCarrying);
		gfx_text(0, x+xo, y, font_size, buf, -1);
		y += line_height;
		str_format(buf, sizeof(buf), "Carriers killed: %d", stats.carriers_killed);
		gfx_text(0, x+xo, y, font_size, buf, -1);
		y += line_height;
	}
}

*/