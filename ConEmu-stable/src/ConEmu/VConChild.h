
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

#pragma once

#define SKIP_HIDE_TIMER
//#define SKIP_ALL_FILLRECT
class CVirtualConsole;
class CRealConsole;

class CConEmuChild
{
	public:
		CConEmuChild();
	protected:
		virtual ~CConEmuChild();
	public:
		static LRESULT WINAPI ChildWndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	public:
		LRESULT OnPaint();
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		HWND CreateView();
		HWND GetView();
		BOOL ShowView(int nShowCmd);
		void Invalidate();
		void Validate();
		void Redraw();

		void SetRedraw(BOOL abRedrawEnabled);

		void SetScroll(BOOL abEnabled, int anTop, int anVisible, int anHeight);
		bool InScroll();
		
		void CheckPostRedraw();

		BOOL TrackMouse();
		void OnAlwaysShowScrollbar();
		
		int IsDcLocked(RECT* CurrentConLockedRect);
		void LockDcRect(bool bLock, RECT* Rect = NULL);

		void SetAutoCopyTimer(bool bEnabled);

		void PostDetach(bool bSendCloseConsole /*= false*/);

#ifdef _DEBUG
	public:
		UINT    mn_MsgCreateDbgDlg;
		HWND    hDlgTest;
		static  INT_PTR CALLBACK DbgChildDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void    CreateDbgDlg();
#endif

	protected:
		virtual void OnDestroy() = 0; // WM_DESTROY

		HWND mh_WndDC;
		HWND mh_LastGuiChild;
		UINT mn_MsgTabChanged;
		UINT mn_MsgPostFullPaint;
		UINT mn_MsgSavePaneSnapshoot;
		UINT mn_MsgDetachPosted;
		BOOL mb_PostFullPaint;
		BOOL mb_DisableRedraw;
#ifdef _DEBUG
		friend class CVirtualConsole;
		friend class CRealConsole;
#endif
		struct tag_Caret
		{
			UINT  X, Y;
			UINT  nWidth, nHeight;
			BOOL  bVisible;
			BOOL  bCreated;
		} Caret;
		DWORD mn_LastPostRedrawTick;
		BOOL  mb_IsPendingRedraw, mb_RedrawPosted;

		CTimer m_TAutoCopy;

		BOOL mb_ScrollDisabled, mb_ScrollVisible, mb_Scroll2Visible, mb_ScrollAutoPopup, mb_VTracking;
		CTimer m_TScrollShow;
		CTimer m_TScrollHide;
		#ifndef SKIP_HIDE_TIMER
		CTimer m_TScrollCheck;
		#endif

		BYTE m_LastAlwaysShowScrollbar;
		SCROLLINFO m_si;
		BOOL CheckMouseOverScroll();
		BOOL CheckScrollAutoPopup();
		void ShowScroll(BOOL abImmediate);
		void HideScroll(BOOL abImmediate);
		void MySetScrollInfo(BOOL abSetEnabled, BOOL abEnableValue);
		
		struct LockDcInfo
		{
			BOOL  bLocked;
			DWORD nLockTick;
			RECT  rcScreen;
			RECT  rcCon;
		} m_LockDc;
};
