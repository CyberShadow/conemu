
/*
Copyright (c) 2011-2012 Maximus5
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


#pragma once

struct ConEmuUpdateSettings
{
public:
	wchar_t *szUpdateVerLocation; // ConEmu latest version location info
	LPCWSTR UpdateVerLocation();
	LPCWSTR UpdateVerLocationDefault();
	
	bool isUpdateCheckOnStartup;
	bool isUpdateCheckHourly;
	bool isUpdateConfirmDownload; // true-Show MessageBox, false-notify via TSA only
	BYTE isUpdateUseBuilds; // 0-�������� ������������ ��� ������ �������, 1-stable only, 2-latest
	BYTE isUpdateDownloadSetup; // 0-Auto, 1-Installer (ConEmuSetup.exe), 2-7z archieve (ConEmu.7z), WinRar or 7z required
	BYTE isSetupDetected; // 0-���� �� ����������, 1-����������� ����� Installer, ���� �������, 2-Installer �� ����������
	BYTE UpdateDownloadSetup();
	
	bool isUpdateUseProxy;
	wchar_t *szUpdateProxy; // "Server:port"
	wchar_t *szUpdateProxyUser;
	wchar_t *szUpdateProxyPassword;
	
	// "%1"-archive or setup file, "%2"-ConEmu.exe folder, "%3"-x86/x64, "%4"-ConEmu PID
	wchar_t *szUpdateExeCmdLine, *szUpdateExeCmdLineDef; // isUpdateDownloadSetup==1
	LPCWSTR UpdateExeCmdLine();
	wchar_t *szUpdateArcCmdLine, *szUpdateArcCmdLineDef; // isUpdateDownloadSetup==2
	LPCWSTR UpdateArcCmdLine();

	wchar_t *szUpdatePostUpdateCmd; // ���� ����� ����-�� ���� ������ � �������������� �������

	wchar_t *szUpdateDownloadPath; // "%TEMP%\\ConEmu"
	bool isUpdateLeavePackages;

	DWORD dwLastUpdateCheck;

public:
	ConEmuUpdateSettings();
	~ConEmuUpdateSettings();
	
	void ResetToDefaults();
	void FreePointers();
	void LoadFrom(ConEmuUpdateSettings* apFrom);
	bool UpdatesAllowed(wchar_t (&szReason)[128]);
	void CheckHourlyUpdate();
};
