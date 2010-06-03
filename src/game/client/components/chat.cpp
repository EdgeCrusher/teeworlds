
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/keys.h>
#include <engine/shared/config.h>

#include <game/generated/protocol.h>
#include <game/generated/client_data.h>

#include <game/client/gameclient.h>

#include <game/client/components/sounds.h>
#include <game/localization.h>

#include "chat.h"


CChat::CChat()
{
	OnReset();
}

void CChat::OnReset()
{
	for(int i = 0; i < MAX_LINES; i++)
	{
		m_aLines[i].m_Time = 0;
		m_aLines[i].m_aText[0] = 0;
		m_aLines[i].m_aName[0] = 0;
	}
	
	m_Show = false;
}

void CChat::OnStateChange(int NewState, int OldState)
{
	if(OldState <= IClient::STATE_CONNECTING)
	{
		m_Mode = MODE_NONE;
		for(int i = 0; i < MAX_LINES; i++)
			m_aLines[i].m_Time = 0;
		m_CurrentLine = 0;
	}
}

void CChat::ConSay(IConsole::IResult *pResult, void *pUserData)
{
	((CChat*)pUserData)->Say(0, pResult->GetString(0));
}

void CChat::ConSayTeam(IConsole::IResult *pResult, void *pUserData)
{
	((CChat*)pUserData)->Say(1, pResult->GetString(0));
}

void CChat::ConChat(IConsole::IResult *pResult, void *pUserData)
{
	const char *pMode = pResult->GetString(0);
	if(str_comp(pMode, "all") == 0)
		((CChat*)pUserData)->EnableMode(0);
	else if(str_comp(pMode, "team") == 0)
		((CChat*)pUserData)->EnableMode(1);
	else
		dbg_msg("console", "expected all or team as mode");
}

void CChat::ConShowChat(IConsole::IResult *pResult, void *pUserData)
{
	((CChat *)pUserData)->m_Show = pResult->GetInteger(0) != 0;
}

void CChat::OnConsoleInit()
{
	Console()->Register("say", "r", CFGFLAG_CLIENT, ConSay, this, "Say in chat");
	Console()->Register("say_team", "r", CFGFLAG_CLIENT, ConSayTeam, this, "Say in team chat");
	Console()->Register("chat", "s", CFGFLAG_CLIENT, ConChat, this, "Enable chat with all/team mode");
	Console()->Register("+show_chat", "", CFGFLAG_CLIENT, ConShowChat, this, "Show chat");
}

bool CChat::OnInput(IInput::CEvent e)
{

	
	
	
	
	
		if(m_Mode == MODE_NONE)
	{
		curr_history_line = -1;
		return false;
	}

	if(e.m_Flags&IInput::FLAG_PRESS && e.m_Key == KEY_ESCAPE)
		m_Mode = MODE_NONE;
	else if(e.m_Flags&IInput::FLAG_PRESS && (e.m_Key == KEY_RETURN || e.m_Key == KEY_KP_ENTER))
	{
		if(m_Input.GetString()[0])
		{
			Say(m_Mode == MODE_ALL ? 0 : 1, m_Input.GetString());
			if (history_count == 0 || str_comp_nocase(m_Input.GetString(), history[history_count - 1].text) != 0)
			{
				if (history_count == MAX_HISTORY_LINES)
				{
					for (int i = 1; i < MAX_HISTORY_LINES; i++)
						str_copy(history[i - 1].text, history[i].text, sizeof(history[0].text));
					str_copy(history[history_count - 1].text, m_Input.GetString(), sizeof(history[0].text));
				} else {
					str_copy(history[history_count].text, m_Input.GetString(), sizeof(history[0].text));
					history_count++;
				}
			}
		}
		m_Mode = MODE_NONE;
	}
	else if (e.m_Flags&IInput::FLAG_PRESS && e.m_Key == KEY_UP)
	{
		if (curr_history_line < 0) curr_history_line = history_count;
		curr_history_line--;
		//if (last_msg_index < 0) last_msg_index = last_msg_count;
		if (curr_history_line < 0) curr_history_line = 0;
		if (curr_history_line >= history_count || history_count == 0)
			m_Input.Set("");
		else
		{
			m_Input.Set(history[curr_history_line].text);
		}
	}
	else if (e.m_Flags&IInput::FLAG_PRESS && e.m_Key == KEY_DOWN)
	{
		if (curr_history_line <= history_count)
		{
			curr_history_line++;
			m_Input.Set(history[curr_history_line].text);
		}
		else
		{
			curr_history_line = -1;
			m_Input.Set("");
		}
		/*curr_history_line = -1;
		input.set("");*/

		/*//last_msg_index = (last_msg_index + 1)%(last_msg_count + 1);
		if (curr_history_line < history_count) curr_history_line = history_count;
		if (curr_history_line >= history_count || history_count == 0)
			input.set("");
		else
		{
			input.set(history[curr_history_line].text);
		}*/
	}
	else
		m_Input.ProcessInput(e);

	return true;
}


void CChat::EnableMode(int Team)
{
	if(m_Mode == MODE_NONE)
	{
		if(Team)
			m_Mode = MODE_TEAM;
		else
			m_Mode = MODE_ALL;
		
		m_Input.Clear();
		Input()->ClearEvents();
	}
}

void CChat::OnMessage(int MsgType, void *pRawMsg)
{
	if(MsgType == NETMSGTYPE_SV_CHAT)
	{
		CNetMsg_Sv_Chat *pMsg = (CNetMsg_Sv_Chat *)pRawMsg;
		AddLine(pMsg->m_Cid, pMsg->m_Team, pMsg->m_pMessage);
	}
}

void CChat::AddLine(int ClientId, int Team, const char *pLine)
{
	char *p = const_cast<char*>(pLine);
	while(*p)
	{
		pLine = p;
		// find line seperator and strip multiline
		while(*p)
		{
			if(*p++ == '\n')
			{
				*(p-1) = 0;
				break;
			}
		}

		m_CurrentLine = (m_CurrentLine+1)%MAX_LINES;
		m_aLines[m_CurrentLine].m_Time = time_get();
		m_aLines[m_CurrentLine].m_ClientId = ClientId;
		m_aLines[m_CurrentLine].m_Team = Team;
		m_aLines[m_CurrentLine].m_NameColor = -2;

		if(ClientId == -1) // server message
		{
			str_copy(m_aLines[m_CurrentLine].m_aName, "*** ", sizeof(m_aLines[m_CurrentLine].m_aName));
			str_format(m_aLines[m_CurrentLine].m_aText, sizeof(m_aLines[m_CurrentLine].m_aText), "%s", pLine);
		}
		else
		{
			if(m_pClient->m_aClients[ClientId].m_Team == -1)
				m_aLines[m_CurrentLine].m_NameColor = -1;

			if(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_Flags&GAMEFLAG_TEAMS)
			{
				if(m_pClient->m_aClients[ClientId].m_Team == 0)
					m_aLines[m_CurrentLine].m_NameColor = 0;
				else if(m_pClient->m_aClients[ClientId].m_Team == 1)
					m_aLines[m_CurrentLine].m_NameColor = 1;
			}
			
			str_copy(m_aLines[m_CurrentLine].m_aName, m_pClient->m_aClients[ClientId].m_aName, sizeof(m_aLines[m_CurrentLine].m_aName));
			str_format(m_aLines[m_CurrentLine].m_aText, sizeof(m_aLines[m_CurrentLine].m_aText), ": %s", pLine);
		}
		
		char aBuf[1024];
		str_format(aBuf, sizeof(aBuf), "[chat]%s%s", m_aLines[m_CurrentLine].m_aName, m_aLines[m_CurrentLine].m_aText);
		Console()->Print(aBuf);
	}

	// play sound
	if(ClientId >= 0)
		m_pClient->m_pSounds->Play(CSounds::CHN_GUI, SOUND_CHAT_CLIENT, 0, vec2(0,0));
	else
		m_pClient->m_pSounds->Play(CSounds::CHN_GUI, SOUND_CHAT_SERVER, 0, vec2(0,0));
}

void CChat::OnRender()
{
	Graphics()->MapScreen(0,0,300*Graphics()->ScreenAspect(),300);
	float x = 10.0f;
	float y = 300.0f-20.0f;
	if(m_Mode != MODE_NONE)
	{
		// render chat input
		CTextCursor Cursor;
		TextRender()->SetCursor(&Cursor, x, y, 8.0f, TEXTFLAG_RENDER);
		Cursor.m_LineWidth = 200.0f;
		
		if(m_Mode == MODE_ALL)
			TextRender()->TextEx(&Cursor, Localize("All"), -1);
		else if(m_Mode == MODE_TEAM)
			TextRender()->TextEx(&Cursor, Localize("Team"), -1);
		else
			TextRender()->TextEx(&Cursor, Localize("Chat"), -1);

		TextRender()->TextEx(&Cursor, ": ", -1);
			
		TextRender()->TextEx(&Cursor, m_Input.GetString(), m_Input.GetCursorOffset());
		CTextCursor Marker = Cursor;
		TextRender()->TextEx(&Marker, "|", -1);
		TextRender()->TextEx(&Cursor, m_Input.GetString()+m_Input.GetCursorOffset(), -1);
	}

	y -= 8;

	int i;
	int64 Now = time_get();
	for(i = 0; i < MAX_LINES; i++)
	{
		int r = ((m_CurrentLine-i)+MAX_LINES)%MAX_LINES;
		if(Now > m_aLines[r].m_Time+15*time_freq() && !m_Show)
			break;

		float Begin = x;
		float FontSize = g_Config.m_cl_chatsize;
		
		// get the y offset
		CTextCursor Cursor;
		TextRender()->SetCursor(&Cursor, Begin, 0, FontSize, 0);
		Cursor.m_LineWidth = 200.0f;
		TextRender()->TextEx(&Cursor, m_aLines[r].m_aName, -1);
		TextRender()->TextEx(&Cursor, m_aLines[r].m_aText, -1);
		y -= Cursor.m_Y + Cursor.m_FontSize;

		// cut off if msgs waste too much space
		int HeightLimit = m_Show ? 0.0f : 200.0f;
		if(y < HeightLimit)
			break;
		
		// reset the cursor
		TextRender()->SetCursor(&Cursor, Begin, y, FontSize, TEXTFLAG_RENDER);
		Cursor.m_LineWidth = 200.0f;

		// render name
		TextRender()->TextColor(0.8f,0.8f,0.8f,1);
		if(m_aLines[r].m_ClientId == -1)
			TextRender()->TextColor(1,1,0.5f,1); // system
		else if(m_aLines[r].m_Team)
			TextRender()->TextColor(0.45f,0.9f,0.45f,1); // team message
		else if(m_aLines[r].m_NameColor == 0)
			TextRender()->TextColor(1.0f,0.5f,0.5f,1); // red
		else if(m_aLines[r].m_NameColor == 1)
			TextRender()->TextColor(0.7f,0.7f,1.0f,1); // blue
		else if(m_aLines[r].m_NameColor == -1)
			TextRender()->TextColor(0.75f,0.5f,0.75f, 1); // spectator
			
		// render name
		TextRender()->TextEx(&Cursor, m_aLines[r].m_aName, -1);

		// render line
		TextRender()->TextColor(1,1,1,1);
		if(m_aLines[r].m_ClientId == -1)
			TextRender()->TextColor(1,1,0.5f,1); // system
		else if(m_aLines[r].m_Team)
			TextRender()->TextColor(0.65f,1,0.65f,1); // team message

		TextRender()->TextEx(&Cursor, m_aLines[r].m_aText, -1);
	}

	TextRender()->TextColor(1,1,1,1);
}

void CChat::Say(int Team, const char *pLine)
{
	// send chat message
	CNetMsg_Cl_Say Msg;
	Msg.m_Team = Team;
	Msg.m_pMessage = pLine;
	Client()->SendPackMsg(&Msg, MSGFLAG_VITAL);
}
