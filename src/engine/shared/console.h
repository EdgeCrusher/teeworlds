#ifndef ENGINE_SHARED_CONSOLE_H
#define ENGINE_SHARED_CONSOLE_H

#include <engine/console.h>

class CConsole : public IConsole
{
	class CCommand : public CCommandInfo
	{
	public:
		CCommand *m_pNext;
		int m_Flags;
		FCommandCallback m_pfnCallback;
		void *m_pUserData;
	};
		

	class CChain
	{
	public:
		FChainCommandCallback m_pfnChainCallback;
		FCommandCallback m_pfnCallback;
		void *m_pCallbackUserData;
		void *m_pUserData;
	};	
	
	CCommand *m_pFirstCommand;

	class CExecFile
	{
	public:
		const char *m_pFilename;
		struct CExecFile *m_pPrev;
	};
	
	CExecFile *m_pFirstExec;
	class IStorage *m_pStorage;

	static void Con_Chain(IResult *pResult, void *pUserData);
	static void Con_Echo(IResult *pResult, void *pUserData);
	static void Con_Exec(IResult *pResult, void *pUserData);

	void ExecuteFileRecurse(const char *pFilename);
	void ExecuteLineStroked(int Stroke, const char *pStr);
	
	FPrintCallback m_pfnPrintCallback;
	void *m_pPrintCallbackUserdata;

	enum
	{
		CONSOLE_MAX_STR_LENGTH  = 1024,
		MAX_PARTS = (CONSOLE_MAX_STR_LENGTH+1)/2
	};
	
	class CResult : public IResult
	{
	public:
		char m_aStringStorage[CONSOLE_MAX_STR_LENGTH+1];
		char *m_pArgsStart;
		
		const char *m_pCommand;
		const char *m_apArgs[MAX_PARTS];
		
		void AddArgument(const char *pArg)
		{
			m_apArgs[m_NumArgs++] = pArg;
		}

		virtual const char *GetString(unsigned Index);
		virtual int GetInteger(unsigned Index);
		virtual float GetFloat(unsigned Index);
	};
	
	int ParseStart(CResult *pResult, const char *pString, int Length);
	int ParseArgs(CResult *pResult, const char *pFormat);

	CCommand *FindCommand(const char *pName);

public:
	CConsole();

	virtual CCommandInfo *GetCommandInfo(const char *pName);
	virtual void PossibleCommands(const char *pStr, int FlagMask, FPossibleCallback pfnCallback, void *pUser) ;

	virtual void ParseArguments(int NumArgs, const char **ppArguments);
	virtual void Register(const char *pName, const char *pParams, int Flags, FCommandCallback pfnFunc, void *pUser, const char *pHelp);
	virtual void Chain(const char *pName, FChainCommandCallback pfnChainFunc, void *pUser);
	
	virtual void ExecuteLine(const char *pStr);
	virtual void ExecuteFile(const char *pFilename);

	virtual void RegisterPrintCallback(FPrintCallback pfnPrintCallback, void *pUserData);
	virtual void Print(const char *pStr);
};

#endif
