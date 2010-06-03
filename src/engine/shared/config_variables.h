#ifndef ENGINE_SHARED_E_CONFIG_VARIABLES_H
#define ENGINE_SHARED_E_CONFIG_VARIABLES_H
#undef ENGINE_SHARED_E_CONFIG_VARIABLES_H // this file will be included several times

// TODO: remove this
#include "././game/variables.h"


MACRO_CONFIG_STR(PlayerName, player_name, 24, "nameless tee", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Name of the player")
MACRO_CONFIG_STR(ClanName, clan_name, 32, "", CFGFLAG_SAVE|CFGFLAG_CLIENT, "(not used)")
MACRO_CONFIG_STR(Password, password, 32, "", CFGFLAG_CLIENT|CFGFLAG_SERVER, "Password to the server")
MACRO_CONFIG_STR(Logfile, logfile, 128, "", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filename to log all output to")

MACRO_CONFIG_INT(ClCpuThrottle, cl_cpu_throttle, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(ClEditor, cl_editor, 0, 0, 1, CFGFLAG_CLIENT, "")

MACRO_CONFIG_INT(ClEventthread, cl_eventthread, 0, 0, 1, CFGFLAG_CLIENT, "Enables the usage of a thread to pump the events")

MACRO_CONFIG_INT(InpGrab, inp_grab, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Use forceful input grabbing method")

MACRO_CONFIG_STR(BrFilterString, br_filter_string, 25, "", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")

MACRO_CONFIG_STR(cl_dyncam_key, cl_dyncam_key, 64, "e", CFGFLAG_SAVE|CFGFLAG_CLIENT, "DynCam switchbutton")

/** EDGE MOD STUFF **/

MACRO_CONFIG_INT(cl_warmup_size, cl_warmup_size, 24, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(cl_jointeam, cl_jointeam, -2, -2, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(cl_mouse_deadzone, cl_mouse_deadzone, 300, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_deadzone_dc_on, cl_mouse_deadzone_dc_on, 300, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_deadzone_dc_off, cl_mouse_deadzone_dc_off, 0, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_followfactor, cl_mouse_followfactor, 60, 0, 200, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_followfactor_dc_on, cl_mouse_followfactor_dc_on, 60, 0, 200, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_followfactor_dc_off, cl_mouse_followfactor_dc_off, 0, 0, 200, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_max_distance, cl_mouse_max_distance, 800, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_max_distance_dc_on, cl_mouse_max_distance_dc_on, 1000, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_mouse_max_distance_dc_off, cl_mouse_max_distance_dc_off, 400, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")


MACRO_CONFIG_INT(cl_connect_tries, cl_connect_tries, 16, -1, 10000, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(cl_broadcast_size, cl_broadcast_size, 14, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(cl_dc, cl_dc, 100, 0, 1000, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_dc_state, cl_dc_state, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_hud, cl_hud, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(cl_menusize, cl_menusize, 600, 0, 1000, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(cl_chatsize, cl_chatsize, 7, 0, 500, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

/* TeeComp default Variables */

MACRO_CONFIG_INT(tc_nameplate_shadow,tc_nameplate_shadow, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Enable name plate shadow")
MACRO_CONFIG_INT(tc_nameplate_score,tc_nameplate_score, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Display score on name plates")
MACRO_CONFIG_INT(tc_colored_nameplates,tc_colored_nameplates, 0, 0, 3, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Enable colored name plates")
MACRO_CONFIG_INT(tc_colored_nameplates_team1,tc_colored_nameplates_team1, 16739179, 0, 16777215, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Red team/team mates name plate color")
MACRO_CONFIG_INT(tc_colored_nameplates_team2,tc_colored_nameplates_team2, 7053311, 0, 16777215, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Blue team/enemies name plate color")

MACRO_CONFIG_INT(tc_colored_tees_method,tc_colored_tees_method, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Enable enemy based skin colors")
MACRO_CONFIG_INT(tc_dm_colors_team1,tc_dm_colors_team1, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Use DM colors for red team/team mates")
MACRO_CONFIG_INT(tc_dm_colors_team2,tc_dm_colors_team2, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Use DM colors for blue team/enemies")
MACRO_CONFIG_INT(tc_colored_tees_team1,tc_colored_tees_team1, 16739179, 0, 16777215, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Red team/team mates color")
MACRO_CONFIG_INT(tc_colored_tees_team2,tc_colored_tees_team2, 7053311, 0, 16777215, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Blue team/enemies color")

MACRO_CONFIG_INT(tc_forced_skins_method,tc_forced_skins_method, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Enable enemy based forced skins")
MACRO_CONFIG_INT(tc_force_skin_team1,tc_force_skin_team1, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Force a skin for red team/your team/DM matchs")
MACRO_CONFIG_INT(tc_force_skin_team2,tc_force_skin_team2, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Force a skin for blue team/opponents")
MACRO_CONFIG_STR(tc_forced_skin1,tc_forced_skin1, 64, "default", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Forced skin for red/mates/DM matchs")
MACRO_CONFIG_STR(tc_forced_skin2,tc_forced_skin2, 64, "default", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Forced skin for blue/opponents")

MACRO_CONFIG_INT(tc_hud_match,tc_hud_match, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Make HUD match tees' colors")
MACRO_CONFIG_INT(tc_speedmeter,tc_speedmeter, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Display speed meter")
MACRO_CONFIG_INT(tc_speedmeter_accel,tc_speedmeter_accel, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Speed meter shows acceleration")

MACRO_CONFIG_INT(tc_autodemo,tc_autodemo, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Enable auto demo recording")
MACRO_CONFIG_INT(tc_autoscreen,tc_autoscreen, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Enable auto screenshot")

MACRO_CONFIG_INT(tc_colored_flags,tc_colored_flags, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Make flags colors match tees colors")
MACRO_CONFIG_INT(tc_hide_carrying,tc_hide_carrying, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Hide the flag if you're carrying it")

MACRO_CONFIG_INT(tc_statboard_infos,tc_statboard_infos, 235, 1, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Mask of infos to display on the global statboard")
MACRO_CONFIG_INT(tc_stat_id,tc_stat_id, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show player id in statboards")
MACRO_CONFIG_INT(tc_disable_chat,tc_disable_chat, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Disable chat")
MACRO_CONFIG_INT(tc_laser_color_inner,tc_laser_color_inner, 8355839, 0, 16777215, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Inner color of laser")
MACRO_CONFIG_INT(tc_laser_color_outer,tc_laser_color_outer, 1250112, 0, 16777215, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Outer color of laser")



/* HUD settings */

MACRO_CONFIG_INT(debug_crap, debug_crap, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Teecomp++ debugging messages")


MACRO_CONFIG_INT(hud_cursor, hud_cursor, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")
MACRO_CONFIG_INT(hud_hp_ammo, hud_hp_ammo, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")
MACRO_CONFIG_INT(hud_goals, hud_goals, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")
MACRO_CONFIG_INT(hud_voting, hud_voting, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")
MACRO_CONFIG_INT(hud_teambalance, hud_teambalance, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")
MACRO_CONFIG_INT(hud_killmsg, hud_killmsg, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")
MACRO_CONFIG_INT(hud_chat, hud_chat, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")


MACRO_CONFIG_INT(hud_speczoom, hud_speczoom, 1, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")

MACRO_CONFIG_INT(hud_show_own_nameplate, hud_show_own_nameplate, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "HUD Settings part")





MACRO_CONFIG_INT(cl_dyncam_switch, cl_dyncam_switch, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Mouse sensitivity")

MACRO_CONFIG_STR(cl_rcon1, cl_rcon1, 64, "empty", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")
MACRO_CONFIG_STR(cl_rcon2, cl_rcon2, 64, "empty", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")
MACRO_CONFIG_STR(cl_rcon3, cl_rcon3, 64, "empty", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")
MACRO_CONFIG_STR(cl_rcon4, cl_rcon4, 64, "empty", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")
MACRO_CONFIG_STR(cl_rcon5, cl_rcon5, 64, "empty", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")
MACRO_CONFIG_STR(cl_rcon6, cl_rcon6, 64, "empty", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Server browser filtering string")

/** EDGE STUFF END **/

MACRO_CONFIG_INT(BrFilterFull, br_filter_full, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filter out full server in browser")
MACRO_CONFIG_INT(BrFilterEmpty, br_filter_empty, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filter out empty server in browser")
MACRO_CONFIG_INT(BrFilterPw, br_filter_pw, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filter out password protected servers in browser")
MACRO_CONFIG_INT(BrFilterPing, br_filter_ping, 999, 0, 999, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Ping to filter by in the server browser")
MACRO_CONFIG_STR(BrFilterGametype, br_filter_gametype, 128, "", CFGFLAG_SAVE|CFGFLAG_CLIENT, "Game types to filter")
MACRO_CONFIG_INT(BrFilterPure, br_filter_pure, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filter out non-standard servers in browser")
MACRO_CONFIG_INT(BrFilterPureMap, br_filter_pure_map, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filter out non-standard maps in browser")
MACRO_CONFIG_INT(BrFilterCompatversion, br_filter_compatversion, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Filter out non-compatible servers in browser")

MACRO_CONFIG_INT(BrSort, br_sort, 0, 0, 256, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(BrSortOrder, br_sort_order, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(BrMaxRequests, br_max_requests, 10, 0, 1000, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Number of requests to use when refreshing server browser")

MACRO_CONFIG_INT(SndBufferSize, snd_buffer_size, 512, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Sound buffer size")
MACRO_CONFIG_INT(SndRate, snd_rate, 48000, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Sound mixing rate")
MACRO_CONFIG_INT(SndEnable, snd_enable, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Sound enable")
MACRO_CONFIG_INT(SndVolume, snd_volume, 100, 0, 100, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Sound volume")
MACRO_CONFIG_INT(SndDevice, snd_device, -1, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "(deprecated) Sound device to use")

MACRO_CONFIG_INT(SndNonactiveMute, snd_nonactive_mute, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")

MACRO_CONFIG_INT(GfxScreenWidth, gfx_screen_width, 800, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Screen resolution width")
MACRO_CONFIG_INT(GfxScreenHeight, gfx_screen_height, 600, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Screen resolution height")
MACRO_CONFIG_INT(GfxFullscreen, gfx_fullscreen, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Fullscreen")
MACRO_CONFIG_INT(GfxAlphabits, gfx_alphabits, 0, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Alpha bits for framebuffer  (fullscreen only)")
MACRO_CONFIG_INT(GfxColorDepth, gfx_color_depth, 24, 16, 24, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Colors bits for framebuffer (fullscreen only)")
MACRO_CONFIG_INT(GfxClear, gfx_clear, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Clear screen before rendering")
MACRO_CONFIG_INT(GfxVsync, gfx_vsync, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Vertical sync")
MACRO_CONFIG_INT(GfxDisplayAllModes, gfx_display_all_modes, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(GfxTextureCompression, gfx_texture_compression, 0, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Use texture compression")
MACRO_CONFIG_INT(GfxHighDetail, gfx_high_detail, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "High detail")
MACRO_CONFIG_INT(GfxTextureQuality, gfx_texture_quality, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(GfxFsaaSamples, gfx_fsaa_samples, 0, 0, 16, CFGFLAG_SAVE|CFGFLAG_CLIENT, "FSAA Samples")
MACRO_CONFIG_INT(GfxRefreshRate, gfx_refresh_rate, 0, 0, 0, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Screen refresh rate")
MACRO_CONFIG_INT(GfxFinish, gfx_finish, 1, 0, 1, CFGFLAG_SAVE|CFGFLAG_CLIENT, "")

MACRO_CONFIG_INT(InpMousesens, inp_mousesens, 100, 5, 100000, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Mouse sensitivity")




MACRO_CONFIG_STR(SvName, sv_name, 128, "unnamed server", CFGFLAG_SERVER, "Server name")
MACRO_CONFIG_STR(SvBindaddr, sv_bindaddr, 128, "", CFGFLAG_SERVER, "Address to bind the server to")
MACRO_CONFIG_INT(SvPort, sv_port, 8303, 0, 0, CFGFLAG_SERVER, "Port to use for the server")
MACRO_CONFIG_INT(SvExternalPort, sv_external_port, 0, 0, 0, CFGFLAG_SERVER, "External port to report to the master servers")
MACRO_CONFIG_STR(SvMap, sv_map, 128, "dm1", CFGFLAG_SERVER, "Map to use on the server")
MACRO_CONFIG_INT(SvMaxClients, sv_max_clients, 8, 1, MAX_CLIENTS, CFGFLAG_SERVER, "Maximum number of clients that are allowed on a server")
MACRO_CONFIG_INT(SvMaxClientsPerIP, sv_max_clients_per_ip, 8, 1, MAX_CLIENTS, CFGFLAG_SERVER, "Maximum number of clients with the same IP that can connect to the server")
MACRO_CONFIG_INT(SvHighBandwidth, sv_high_bandwidth, 0, 0, 1, CFGFLAG_SERVER, "Use high bandwidth mode. Doubles the bandwidth required for the server. LAN use only")
MACRO_CONFIG_INT(SvRegister, sv_register, 1, 0, 1, CFGFLAG_SERVER, "Register server with master server for public listing")
MACRO_CONFIG_STR(SvRconPassword, sv_rcon_password, 32, "", CFGFLAG_SERVER, "Remote console password")

MACRO_CONFIG_INT(Debug, debug, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SERVER, "Debug mode")
MACRO_CONFIG_INT(DbgStress, dbg_stress, 0, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SERVER, "Stress systems")
MACRO_CONFIG_INT(DbgStressNetwork, dbg_stress_network, 0, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SERVER, "Stress network")
MACRO_CONFIG_INT(DbgPref, dbg_pref, 0, 0, 1, CFGFLAG_SERVER, "Performance outputs")
MACRO_CONFIG_INT(DbgGraphs, dbg_graphs, 0, 0, 1, CFGFLAG_CLIENT, "Performance graphs")
MACRO_CONFIG_INT(DbgHitch, dbg_hitch, 0, 0, 0, CFGFLAG_SERVER, "Hitch warnings")
MACRO_CONFIG_STR(DbgStressServer, dbg_stress_server, 32, "localhost", CFGFLAG_CLIENT, "Server to stress")
MACRO_CONFIG_INT(DbgResizable, dbg_resizable, 0, 0, 0, CFGFLAG_CLIENT, "Enables window resizing")
#endif
