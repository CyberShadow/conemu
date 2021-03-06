#pragma once

/*
macro.hpp

�������
*/
/*
Copyright (c) 1996 Eugene Roshal
Copyright (c) 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "macrocompiler.hpp"
#include "tvar.hpp"
#include "macroopcode.hpp"

enum MACRODISABLEONLOAD
{
	MDOL_ALL            = 0x80000000, // �������� ��� ������� ��� ��������
	MDOL_AUTOSTART      = 0x00000001, // �������� �������������� �������
};

// ������� �������� �������� (������ ����������) -  �� ����� 0xFF ��������!
enum MACROMODEAREA
{
	MACRO_FUNCS                =  -3,
	MACRO_CONSTS               =  -2,
	MACRO_VARS                 =  -1,

	// see also plugin.hpp # FARMACROAREA
	MACRO_OTHER                =   0, // ����� ����������� ������ � ������, ������������ ����
	MACRO_SHELL                =   1, // �������� ������
	MACRO_VIEWER               =   2, // ���������� ��������� ���������
	MACRO_EDITOR               =   3, // ��������
	MACRO_DIALOG               =   4, // �������
	MACRO_SEARCH               =   5, // ������� ����� � �������
	MACRO_DISKS                =   6, // ���� ������ ������
	MACRO_MAINMENU             =   7, // �������� ����
	MACRO_MENU                 =   8, // ������ ����
	MACRO_HELP                 =   9, // ������� ������
	MACRO_INFOPANEL            =  10, // �������������� ������
	MACRO_QVIEWPANEL           =  11, // ������ �������� ���������
	MACRO_TREEPANEL            =  12, // ������ ������ �����
	MACRO_FINDFOLDER           =  13, // ����� �����
	MACRO_USERMENU             =  14, // ���� ������������
	MACRO_AUTOCOMPLETION       =  15, // ������ ��������������

	MACRO_COMMON,                     // �����! - ������ ���� �������������, �.�. ��������� ����� ������ !!!
	MACRO_LAST                        // ������ ���� ������ ���������! ������������ � ������
};

enum MACROFLAGS_MFLAGS
{
	MFLAGS_MODEMASK            =0x000000FF, // ����� ��� ��������� ������� �������� (������� ������ ����������) �������

	MFLAGS_DISABLEOUTPUT       =0x00000100, // �������� ���������� ������ �� ����� ���������� �������
	MFLAGS_NOSENDKEYSTOPLUGINS =0x00000200, // �� ���������� �������� ������� �� ����� ������/��������������� �������
	MFLAGS_RUNAFTERFARSTARTED  =0x00000400, // ���� ������ ��� ���������� ��� ������ ����
	MFLAGS_RUNAFTERFARSTART    =0x00000800, // ���� ������ ����������� ��� ������ ����

	MFLAGS_EMPTYCOMMANDLINE    =0x00001000, // ���������, ���� ��������� ����� �����
	MFLAGS_NOTEMPTYCOMMANDLINE =0x00002000, // ���������, ���� ��������� ����� �� �����
	MFLAGS_EDITSELECTION       =0x00004000, // ���������, ���� ���� ��������� � ���������
	MFLAGS_EDITNOSELECTION     =0x00008000, // ���������, ���� ���� ��� ��������� � ���������

	MFLAGS_SELECTION           =0x00010000, // ��������:  ���������, ���� ���� ���������
	MFLAGS_PSELECTION          =0x00020000, // ���������: ���������, ���� ���� ���������
	MFLAGS_NOSELECTION         =0x00040000, // ��������:  ���������, ���� ���� ��� ���������
	MFLAGS_PNOSELECTION        =0x00080000, // ���������: ���������, ���� ���� ��� ���������
	MFLAGS_NOFILEPANELS        =0x00100000, // ��������:  ���������, ���� ��� ���������� ������
	MFLAGS_PNOFILEPANELS       =0x00200000, // ���������: ���������, ���� ��� ���������� ������
	MFLAGS_NOPLUGINPANELS      =0x00400000, // ��������:  ���������, ���� ��� �������� ������
	MFLAGS_PNOPLUGINPANELS     =0x00800000, // ���������: ���������, ���� ��� �������� ������
	MFLAGS_NOFOLDERS           =0x01000000, // ��������:  ���������, ���� ������� ������ "����"
	MFLAGS_PNOFOLDERS          =0x02000000, // ���������: ���������, ���� ������� ������ "����"
	MFLAGS_NOFILES             =0x04000000, // ��������:  ���������, ���� ������� ������ "�����"
	MFLAGS_PNOFILES            =0x08000000, // ���������: ���������, ���� ������� ������ "�����"

	MFLAGS_REG_MULTI_SZ        =0x10000000, // ����� ������� ������������� (REG_MULTI_SZ)
	MFLAGS_POSTFROMPLUGIN      =0x20000000, // ������������������ ������ �� ���
	MFLAGS_NEEDSAVEMACRO       =0x40000000, // ���������� ���� ������ ���������
	MFLAGS_DISABLEMACRO        =0x80000000, // ���� ������ ��������
};


// ���� �������� ��� KeyMacro::GetCurRecord()
enum MACRORECORDANDEXECUTETYPE
{
	MACROMODE_NOMACRO          =0,  // �� � ������ �����
	MACROMODE_EXECUTING        =1,  // ����������: ��� �������� ������� ����
	MACROMODE_EXECUTING_COMMON =2,  // ����������: � ��������� ������� ����
	MACROMODE_RECORDING        =3,  // ������: ��� �������� ������� ����
	MACROMODE_RECORDING_COMMON =4,  // ������: � ��������� ������� ����
};

class Panel;

struct TMacroFunction;
typedef bool (*INTMACROFUNC)(const TMacroFunction*);

enum INTMF_FLAGS{
	IMFF_UNLOCKSCREEN               =0x00000001,
	IMFF_DISABLEINTINPUT            =0x00000002,
};

struct TMacroFunction
{
	const wchar_t *Name;             // ��� �������
	int nParam;                      // ���������� ����������
	int oParam;                      // �������������� ���������
	TMacroOpCode Code;               // ������� �������
	const wchar_t *fnGUID;           // GUID ����������� �������

	int    BufferSize;               // ������ ������ ��������������� ������������������
	DWORD *Buffer;                   // ��������������� ������������������ (OpCode) �������
	//wchar_t  *Src;                   // ������������ "�����" �������
	//wchar_t  *Description;           // �������� �������

	const wchar_t *Syntax;           // ��������� �������

	DWORD IntFlags;                  // ����� �� INTMF_FLAGS (� �������� ���������� "��� �������� �������")
	INTMACROFUNC Func;               // �������
};

struct MacroRecord
{
	DWORD  Flags;         // ����� �����������������������
	int    Key;           // ����������� �������
	int    BufferSize;    // ������ ������ ��������������� ������������������
	DWORD *Buffer;        // ��������������� ������������������ (OpCode) �������
	wchar_t  *Src;           // ������������ "�����" �������
	wchar_t  *Description;   // �������� �������
	DWORD  Reserved[2];   // ���������������
};

#define STACKLEVEL      32

struct MacroState
{
	int KeyProcess;
	int Executing;
	int MacroPC;
	int ExecLIBPos;
	int MacroWORKCount;
	DWORD HistroyEnable;
	bool UseInternalClipboard;
	struct MacroRecord *MacroWORK; // �.�. ������� ����������
	INPUT_RECORD cRec; // "�������� ������� ������� �������"

	bool AllocVarTable;
	TVarTable *locVarTable;

	void Init(TVarTable *tbl);
};


struct MacroPanelSelect {
	int     Action;
	DWORD   ActionFlags;
	int     Mode;
	__int64 Index;
	TVar    *Item;
};

/* $TODO:
    1. ������� IndexMode[], Sort()
    2. �� MacroLIB �������
       struct MacroRecord *MacroLIB[MACRO_LAST];
*/
class KeyMacro
{
	private:
		DWORD MacroVersion;

		static DWORD LastOpCodeUF; // ��������� �� ������� OpCode ��� UserFunction (������������ KEY_MACRO_U_BASE)
		// ��� �������
		static size_t CMacroFunction;
		static size_t AllocatedFuncCount;
		static TMacroFunction *AMacroFunction;

		// ��� ������ - � ������� ������� �������� ���...
		// 0 - ��� ������, 1 - ������� ������, 2 - ����� ������� ��������
		int Recording;
		int InternalInput;
		int IsRedrawEditor;

		int Mode;
		int StartMode;

		struct MacroState Work;
		struct MacroState PCStack[STACKLEVEL];
		int CurPCStack;

		bool StopMacro;

		// ���� "�����" ������ ������ ��� ������ �������� (��������� ������),
		// � ��������� ����� MacroWORK
		int MacroLIBCount;
		struct MacroRecord *MacroLIB;

		int IndexMode[MACRO_LAST][2];

		int RecBufferSize;
		DWORD *RecBuffer;
		wchar_t *RecSrc;

		class LockScreen *LockScr;

	private:
		int ReadVarsConst(int ReadMode, string &strBuffer);
		int ReadMacroFunction(int ReadMode, string &strBuffer);
		int WriteVarsConst(int WriteMode);
		int ReadMacros(int ReadMode, string &strBuffer);
		DWORD AssignMacroKey();
		int GetMacroSettings(int Key,DWORD &Flags);
		void InitInternalVars(BOOL InitedRAM=TRUE);
		void InitInternalLIBVars();
		void ReleaseWORKBuffer(BOOL All=FALSE); // ������� ��������� �����

		DWORD SwitchFlags(DWORD& Flags,DWORD Value);
		string &MkRegKeyName(int IdxMacro,string &strRegKeyName);

		BOOL CheckEditSelected(DWORD CurFlags);
		BOOL CheckInsidePlugin(DWORD CurFlags);
		BOOL CheckPanel(int PanelMode,DWORD CurFlags, BOOL IsPassivePanel);
		BOOL CheckCmdLine(int CmdLength,DWORD Flags);
		BOOL CheckFileFolder(Panel *ActivePanel,DWORD CurFlags, BOOL IsPassivePanel);
		BOOL CheckAll(int CheckMode,DWORD CurFlags);
		void Sort();
		TVar FARPseudoVariable(DWORD Flags,DWORD Code,DWORD& Err);
		DWORD GetOpCode(struct MacroRecord *MR,int PC);
		DWORD SetOpCode(struct MacroRecord *MR,int PC,DWORD OpCode);

	private:
		static LONG_PTR WINAPI AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2);
		static LONG_PTR WINAPI ParamMacroDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2);

	public:
		KeyMacro();
		~KeyMacro();

	public:
		int ProcessKey(int Key);
		int GetKey();
		int PeekKey();
		bool IsOpCode(DWORD p);
		bool CheckWaitKeyFunc();

		int PushState(bool CopyLocalVars=FALSE);
		int PopState();
		int GetLevelState() {return CurPCStack;};

		int  IsRecording() {return(Recording);};
		int  IsExecuting() {return(Work.Executing);};
		int  IsExecutingLastKey();
		int  IsDsableOutput() {return CheckCurMacroFlags(MFLAGS_DISABLEOUTPUT);};
		void SetMode(int Mode) {KeyMacro::Mode=Mode;};
		int  GetMode() {return(Mode);};

		void DropProcess();

		// ������� ������ �� ���������� �������
		void SendDropProcess();

		void RunStartMacro();

		// ��������� ��������� ��������� ������������� �������
		int PostNewMacro(const wchar_t *PlainText,DWORD Flags=0,DWORD AKey=0,BOOL onlyCheck=FALSE);
		// ��������� ��������� ������ (�������� �������������)
		int PostNewMacro(struct MacroRecord *MRec,BOOL NeedAddSendFlag=0,BOOL IsPluginSend=FALSE);

		int  LoadMacros(BOOL InitedRAM=TRUE,BOOL LoadAll=TRUE);
		void SaveMacros(BOOL AllSaved=TRUE);

		int GetStartIndex(int Mode) {return IndexMode[Mode<MACRO_LAST-1?Mode:MACRO_LAST-1][0];}
		// ������� ��������� ������� ������� ������� � �������
		int GetIndex(int Key, int Mode, bool UseCommon=true);
		// ��������� �������, ����������� ��������� ��������
		int GetRecordSize(int Key, int Mode);

		bool GetPlainText(string& Dest);
		int  GetPlainTextSize();

		//void SetRedrawEditor(int Sets) {IsRedrawEditor=Sets;}

		void RestartAutoMacro(int Mode);

		// �������� ������ � ������� (���������� ������)
		int GetCurRecord(struct MacroRecord* RBuf=nullptr,int *KeyPos=nullptr);
		// ��������� ����� �������� ������������ �������.
		BOOL CheckCurMacroFlags(DWORD Flags);

		bool IsHistroyEnable(int TypeHistory);

		static const wchar_t* GetSubKey(int Mode);
		static int   GetSubKey(const wchar_t *Mode);
		static int   GetMacroKeyInfo(bool FromReg,int Mode,int Pos,string &strKeyName,string &strDescription);
		static wchar_t *MkTextSequence(DWORD *Buffer,int BufferSize,const wchar_t *Src=nullptr);
		// �� ���������� ������������� ������� ������� MacroRecord
		int ParseMacroString(struct MacroRecord *CurMacro,const wchar_t *BufPtr,BOOL onlyCheck=FALSE);
		BOOL GetMacroParseError(DWORD* ErrCode, COORD* ErrPos, string *ErrSrc);
		BOOL GetMacroParseError(string *Err1, string *Err2, string *Err3, string *Err4);

		static void SetMacroConst(const wchar_t *ConstName, const TVar& Value);
		static DWORD GetNewOpCode();

		static size_t GetCountMacroFunction();
		static const TMacroFunction *GetMacroFunction(size_t Index);
		static void RegisterMacroIntFunction();
		static TMacroFunction *RegisterMacroFunction(const TMacroFunction *tmfunc);
		static bool UnregMacroFunction(size_t Index);
};

BOOL WINAPI KeyMacroToText(int Key,string &strKeyText0);
int WINAPI KeyNameMacroToKey(const wchar_t *Name);
void initMacroVarTable(int global);
void doneMacroVarTable(int global);
bool checkMacroConst(const wchar_t *name);
const wchar_t *eStackAsString(int Pos=0);

inline bool IsMenuArea(int Area){return Area==MACRO_MAINMENU || Area==MACRO_MENU || Area==MACRO_DISKS || Area==MACRO_USERMENU || Area==MACRO_AUTOCOMPLETION;}
