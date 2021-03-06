
/*
Copyright (c) 2009-2012 Maximus5
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

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
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

#define HIDE_USE_EXCEPTION_INFO
#include <windows.h>
#include "defines.h"
#include "MAssert.h"
#include "Memory.h"
#include "MStrSafe.h"
#include "RConStartArgs.h"
#include "common.hpp"
#include "WinObjects.h"

RConStartArgs::RConStartArgs()
{
	bDetached = bRunAsAdministrator = bRunAsRestricted = FALSE;
	bForceUserDialog = bBackgroundTab = bForceDosBox = FALSE;
	#if 0
	eSplit = eSplitNone; nSplitValue = DefaultSplitValue; nSplitPane = 0;
	#endif
	aRecreate = cra_CreateTab;
	pszSpecialCmd = pszStartupDir = pszUserName = pszDomain = /*pszUserPassword =*/ NULL;
	bBufHeight = FALSE; nBufHeight = 0;
	eConfirmation = eConfDefault;
	szUserPassword[0] = 0;
	//hLogonToken = NULL;
}

RConStartArgs::~RConStartArgs()
{
	SafeFree(pszSpecialCmd); // ������ SafeFree
	SafeFree(pszStartupDir); // ������ SafeFree
	SafeFree(pszUserName);
	SafeFree(pszDomain);

	//SafeFree(pszUserPassword);
	if (szUserPassword[0]) SecureZeroMemory(szUserPassword, sizeof(szUserPassword));

	//if (hLogonToken) { CloseHandle(hLogonToken); hLogonToken = NULL; }
}

BOOL RConStartArgs::CheckUserToken(HWND hPwd)
{
	//if (hLogonToken) { CloseHandle(hLogonToken); hLogonToken = NULL; }
	if (!pszUserName || !*pszUserName)
		return FALSE;

	//wchar_t szPwd[MAX_PATH]; szPwd[0] = 0;
	szUserPassword[0] = 0;

	if (!GetWindowText(hPwd, szUserPassword, MAX_PATH-1))
		return FALSE;

	SafeFree(pszDomain);
	wchar_t* pszSlash = wcschr(pszUserName, L'\\');
	if (pszSlash)
	{
		pszDomain = pszUserName;
		*pszSlash = 0;
		pszUserName = lstrdup(pszSlash+1);
	}

	HANDLE hLogonToken = NULL;
	BOOL lbRc = LogonUser(pszUserName, pszDomain, szUserPassword, LOGON32_LOGON_INTERACTIVE,
	                      LOGON32_PROVIDER_DEFAULT, &hLogonToken);
	//if (szUserPassword[0]) SecureZeroMemory(szUserPassword, sizeof(szUserPassword));

	if (!lbRc || !hLogonToken)
	{
		MessageBox(GetParent(hPwd), L"Invalid user name or password specified!", L"ConEmu", MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	CloseHandle(hLogonToken);
	//hLogonToken may be used for CreateProcessAsUser
	return TRUE;
}

// Returns ">0" - when changes was made
//  0 - no changes
// -1 - error
int RConStartArgs::ProcessNewConArg()
{
	if (!pszSpecialCmd || !*pszSpecialCmd)
	{
		_ASSERTE(pszSpecialCmd && *pszSpecialCmd);
		return -1;
	}

	int nChanges = 0;
	
	// 120115 - ���� ������ �������� - "ConEmu.exe" ��� "ConEmu64.exe" - �� ������������ "-cur_console" � "-new_console"
	{
		LPCWSTR pszTemp = pszSpecialCmd;
		wchar_t szExe[MAX_PATH+1];
		if (0 == NextArg(&pszTemp, szExe))
		{
			pszTemp = PointToName(szExe);
			if (lstrcmpi(pszTemp, L"ConEmu.exe") == 0
				|| lstrcmpi(pszTemp, L"ConEmu") == 0
				|| lstrcmpi(pszTemp, L"ConEmu64.exe") == 0
				|| lstrcmpi(pszTemp, L"ConEmu64") == 0)
			{
				return 0;
			}
		}
	}
	

	// 111211 - ����� ����� ���� ������� "-new_console:..."
	LPCWSTR pszNewCon = L"-new_console";
	// 120108 - ��� "-cur_console:..." ��� ��������� ���������� ������� ������ (�� ���� ��������)
	LPCWSTR pszCurCon = L"-cur_console";
	int nNewConLen = lstrlen(pszNewCon);
	_ASSERTE(lstrlen(pszCurCon)==nNewConLen);
	wchar_t* pszFind;
	bool bStop = false;

	while (!bStop
		&& ((pszFind = wcsstr(pszSpecialCmd, pszNewCon)) != NULL || (pszFind = wcsstr(pszSpecialCmd, pszCurCon)) != NULL)
		)
	{
		// �������� ����������
		_ASSERTE(pszFind > pszSpecialCmd);
		if (((pszFind == pszSpecialCmd) || (*(pszFind-1) == L'"') || (*(pszFind-1) == L' ')) // ������ ���������
			&& (pszFind[nNewConLen] == L' ' || pszFind[nNewConLen] == L':' 
				|| pszFind[nNewConLen] == L'"' || pszFind[nNewConLen] == 0))
		{
			// �� ���������, ������������� �������� "Press Enter or Esc to close console"
			eConfirmation = eConfAlways;
		
			bool lbQuot = (*(pszFind-1) == L'"');
			const wchar_t* pszEnd = pszFind+nNewConLen;
			//wchar_t szNewConArg[MAX_PATH+1];
			if (lbQuot)
				pszFind--;

			if (*pszEnd == L'"')
			{
				pszEnd++;
			}
			else if (*pszEnd != L':')
			{
				// �����
				_ASSERTE(*pszEnd == L' ' || *pszEnd == 0);
			}
			else
			{
				if (*pszEnd == L':')
				{
					pszEnd++;
				}
				else
				{
					_ASSERTE(*pszEnd == L':');
				}

				// ��������� ���.���������� -new_console:xxx
				bool lbReady = false;
				while (!lbReady && *pszEnd)
				{
					switch (*(pszEnd++))
					{
					//case L'-':
					//	bStop = true; // ��������� "-new_console" - �� �������!
					//	break;
					case L'"':
					case L' ':
					case 0:
						lbReady = true;
						break;
						
					case L'b':
						// b - background, �� ������������ ���
						bBackgroundTab = TRUE;
						break;
						
					case L'a':
						// a - RunAs shell verb (as admin on Vista+, login/password in WinXP-)
						bRunAsAdministrator = TRUE;
						break;
						
					case L'r':
						// r - run as restricted user
						bRunAsRestricted = TRUE;
						break;
						
					case L'h':
						// "h0" - ��������� �����, "h9999" - �������� ����� � 9999 �����
						{
							bBufHeight = TRUE;
							if (isDigit(*pszEnd))
							{
								wchar_t* pszDigits = NULL;
								nBufHeight = wcstoul(pszEnd, &pszDigits, 10);
								if (pszDigits)
									pszEnd = pszDigits;
							}
							else
							{
								nBufHeight = 0;
							}
						} // L'h':
						break;
						
					case L'n':
						// n - ��������� "Press Enter or Esc to close console"
						eConfirmation = eConfNever;
						break;
						
					case L'c':
						// c - ������������� �������� "Press Enter or Esc to close console"
						eConfirmation = eConfAlways;
						break;
						
					case L'x':
						// x - Force using dosbox for .bat files
						bForceDosBox = TRUE;
						break;
						
					// "Long" code blocks below: 'd', 'u', 's' and so on (in future)
					case L'd':
						// d:<StartupDir>. MUST be last options
						{
							if (*pszEnd == L':')
								pszEnd++;
							const wchar_t* pszDir = pszEnd;
							while ((*pszEnd) && (lbQuot || *pszEnd != L' ') && (*pszEnd != L'"'))
								pszEnd++;
							if (pszEnd > pszDir)
							{
								size_t cchLen = pszEnd - pszDir;
								SafeFree(pszStartupDir);
								pszStartupDir = (wchar_t*)malloc((cchLen+1)*sizeof(*pszStartupDir));
								if (pszStartupDir)
								{
									wmemmove(pszStartupDir, pszDir, cchLen);
									pszStartupDir[cchLen] = 0;
									// ��������, "%USERPROFILE%"
									if (wcschr(pszStartupDir, L'%'))
									{
										wchar_t* pszExpand = NULL;
										if (((pszExpand = ExpandEnvStr(pszStartupDir)) != NULL))
										{
											SafeFree(pszStartupDir);
											pszStartupDir = pszExpand;
										}
									}
								}
							}
						} // L'd':
						break;
					case L'u':
						{
							// u - ConEmu choose user dialog
							// u:<user>:<pwd> - specify user/pwd in args. MUST be last option
							
							lbReady = true; // ��������� �����

							SafeFree(pszUserName);
							SafeFree(pszDomain);
							if (szUserPassword[0]) SecureZeroMemory(szUserPassword, sizeof(szUserPassword));
							
							if (*pszEnd == L':')
							{
								pszEnd++;
								
								wchar_t szUser[MAX_PATH], *p = szUser, *p2 = szUser+countof(szUser)-1;
								while (*pszEnd && (p < p2))
								{
									if ((*pszEnd == 0) || (*pszEnd == L':') || (*pszEnd == L'"'))
									{
										break;
									}
									//else if (*pszEnd == L'"' && *(pszEnd+1) == L'"')
									//{
									//	*(p++) = L'"'; pszEnd += 2;
									//}
									else if (*pszEnd == L'^')
									{
										pszEnd++;
										*(p++) = *(pszEnd++);
									}
									else
									{
										*(p++) = *(pszEnd++);
									}
								}
								*p = 0;

								wchar_t* pszSlash = wcschr(szUser, L'\\');
								if (pszSlash)
								{
									*pszSlash = 0;
									pszDomain = lstrdup(szUser);
									pszUserName = lstrdup(pszSlash+1);
								}
								else
								{
									pszUserName = lstrdup(szUser);
								}
								
								if (*pszEnd == L':')
								{
									pszEnd++;
									//lstrcpyn(szUserPassword, pszPwd, countof(szUserPassword));

									p = szUserPassword; p2 = szUserPassword+countof(szUserPassword)-1;
									while (*pszEnd && (p < p2))
									{
										if ((*pszEnd == 0) || (*pszEnd == L':') || (*pszEnd == L'"'))
										{
											break;
										}
										else if (*pszEnd == L'^')
										{
											pszEnd++;
											*(p++) = *(pszEnd++);
										}
										else
										{
											*(p++) = *(pszEnd++);
										}
									}
									*p = 0;

								}
							}
							else
							{
								bForceUserDialog = TRUE;
							}
						} // L'u'
						break;
					}
				}
			}

			if (pszEnd > pszFind)
			{
				if (lbQuot)
				{
					if (*pszEnd == L'"' && *(pszEnd-1) != L'"')
						pszEnd++;
				}
				else
				{
					while (*(pszEnd-1) == L'"')
						pszEnd--;
				}

				while (((pszFind - 1) > pszSpecialCmd) && (*(pszFind-1) == L' ') && (*(pszFind-2) == L' '))
					pszFind--;
				//wmemset(pszFind, L' ', pszEnd - pszFind);
				wmemmove(pszFind, pszEnd, (lstrlen(pszEnd)+1));
				nChanges++;
			}
			else
			{
				_ASSERTE(pszEnd > pszFind);
				*pszFind = 0;
				nChanges++;
			}
		}
	}

	return nChanges;
}
