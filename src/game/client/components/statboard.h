#ifndef GAME_CLIENT_COMPONENTS_SCOREBOARD_H
#define GAME_CLIENT_COMPONENTS_SCOREBOARD_H
#include <game/client/component.h>

class CStatboard : public CComponent
{
	void RenderGoals(float x, float y, float w);
	void RenderSpectators(float x, float y, float w);
	void RenderScoreboard(float x, float y, float w, int Team, const char *pTitle);

	static void ConKeyStatboard(IConsole::IResult *pResult, void *pUserData);
	
	bool m_Active;
	bool m_DoScreenShot;
	

public:
	CStatboard();
	virtual void OnReset();
	virtual void OnConsoleInit();
	virtual void OnRender();
};

#endif
