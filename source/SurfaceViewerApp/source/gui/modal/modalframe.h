/////////////////////////////////////////////////////////////////////////
//
// CModalFrame					Version 1.2
//
// Created: May 28, 2003		Last modified: Aug 5, 2005
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 2003-2005 by Alexey Nikitin. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// nick@complex-a5.ru.
/////////////////////////////////////////////////////////////////////////

#if !defined(MODALFRAME_H_1F53AB30_F611_4DF8_B694_7089307224AD__INCLUDED_)
#define MODALFRAME_H_1F53AB30_F611_4DF8_B694_7089307224AD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

struct CModalFrame
{
	static void End(CFrameWnd* pModalFrameWnd, int nResult);

	static bool Run(CRuntimeClass& FrameWndClass,
		bool bTopLevelParentPlacement,
		UINT nIDFrameResource,
		DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
		CWnd* pParentWnd = NULL,
		CCreateContext* pContext = NULL);

	//	if (bParentPlacement == true && pParentWnd != NULL)
	//		frame placement on the screen will be the same as
	//		top level parent window of pParentWnd.
	// else
	//		frame placement on the screen will be selected by
	//		the system.
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(MODALFRAME_H_1F53AB30_F611_4DF8_B694_7089307224AD__INCLUDED_)
