/********************************************************************************
 * Flux
 *
 * File: anmcontextmenu.cpp
 * Description: Context (i.e. right-mouse) menus
 *
 * (C) 2001-2006 by Media Machines, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *********************************************************************************/

#include "stdafx.h"
#include "anmcontextmenu.h"
#include "anmmenucommands.h"
#include "anmmessage.h"
#include "anmviewer.h"
#include "anmviewpoint.h"
#include "anmworld.h"
#include "../animaapp.h"
#include "../resource.h"

CAnmContextMenu::CAnmContextMenu(class cAnimaApp *pApp, void *handle)
{
	m_app = pApp;
	m_hMenu = (HMENU) handle;
}

CAnmContextMenu::~CAnmContextMenu()
{
}

eAnmContextMenuCommand CAnmContextMenu::TrackMenu(int x, int y)
{
	if (m_hMenu)
	{
		POINT point;
		point.x = x;
		point.y = y;

		CWnd *pCtlWin = m_app->GetControlWindow();
		pCtlWin->ClientToScreen(&point);

		int cmdID = ::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY,
			point.x, point.y, 0,
			pCtlWin->GetSafeHwnd(), NULL);

		if (cmdID)
		{
			eAnmContextMenuCommand cmd = CommandFromID(cmdID);
			return cmd;
		}
	}

	return eAnmCtxMenuBadCommandID;
}

// N.B.: these are totally dependent on the context menu layout!
// any changes to that *must* be reflected in these position constants!
#define VIEWPOSITION	9
#define SPEEDPOSITION	10
#define HELPPOSITION	14

CAnmContextMenu *CAnmContextMenu::GetSubMenu(eAnmContextMenuPopup subMenuID)
{
	int position = -1;
	
	switch (subMenuID)
	{
		case eAnmCtxMenuViewMenu :
			position = VIEWPOSITION;
			break;

		case eAnmCtxMenuSpeedMenu :
			position = SPEEDPOSITION;
			break;

		case eAnmCtxMenuHelpMenu :
			position = HELPPOSITION;
			break;

		default :
			break;
	}

	if (position == -1)
		return NULL;


	HMENU hSubMenu = ::GetSubMenu(m_hMenu, position);

	return new CAnmContextMenu(m_app, hSubMenu);
}

int CAnmContextMenu::IDFromCommand(eAnmContextMenuCommand cmd)
{
	int id = 0;
	int cmd_int = 0;

	switch (cmd)
	{
		case eAnmCtxMenuWalk :
			id = IDR_CTXWALK;
			break;

		case eAnmCtxMenuFly :
			id = IDR_CTXFLY;
			break;

		case eAnmCtxMenuExamine :
			id = IDR_CTXEXAMINE;
			break;

		case eAnmCtxMenuSeek :
			id = IDR_CTXSEEK;
			break;

		case eAnmCtxMenuLevel :
			id = IDR_CTXLEVEL;
			break;

		case eAnmCtxMenuBack :
			id = IDR_CTXBACK;
			break;

		case eAnmCtxMenuForward :
			id = IDR_CTXFORWARD;
			break;

		case eAnmCtxMenuViewFirst :
			id = IDR_CTXVIEWFIRST;
			break;

		case eAnmCtxMenuViewLast :
			id = IDR_CTXVIEWLAST;
			break;

		case eAnmCtxMenuViewNext :
			id = IDR_CTXVIEWNEXT;
			break;

		case eAnmCtxMenuViewPrevious :
			id = IDR_CTXVIEWPREVIOUS;
			break;

		case eAnmCtxMenuViewRestore :
			id = IDR_CTXVIEWRESTORE;
			break;

		case eAnmCtxMenuSpeedSlower :
			id = IDR_CTXSPEEDSLOWER;
			break;

		case eAnmCtxMenuSpeedSlow :
			id = IDR_CTXSPEEDSLOW;
			break;

		case eAnmCtxMenuSpeedMedium :
			id = IDR_CTXSPEEDMEDIUM;
			break;

		case eAnmCtxMenuSpeedFast :
			id = IDR_CTXSPEEDFAST;
			break;

		case eAnmCtxMenuSpeedFaster :
			id = IDR_CTXSPEEDFASTER;
			break;

		case eAnmCtxMenuInfoCenter :
			id = IDR_CTXINFOCENTER;
			break;

		case eAnmCtxMenuPreferences :
			id = IDR_CTXPREFERENCES;
			break;

		case eAnmCtxMenuHelpUserGuide :
			id = IDR_CTXHELPUSERSGUIDE;
			break;

		case eAnmCtxMenuHelpAbout :
			id = IDR_CTXHELPABOUT;
			break;

		case eAnmCtxMenuBadCommandID :
			break;

		default :
			cmd_int = (int) cmd;
			if (cmd_int >= (int) eAnmCtxMenuViewpointsBase)
			{
				int viewindex = cmd_int - (int) eAnmCtxMenuViewpointsBase;
				int viewID = IDR_CTXVIEWPOINTS_BASE + viewindex;
				if (viewID >= IDR_CTXVIEWPOINTS_BASE &&
					viewID < IDR_CTXVIEWPOINTS_MAX)

					id = viewID;
			}
			break;
	}

	return id;
}

eAnmContextMenuCommand CAnmContextMenu::CommandFromID(int id)
{
	eAnmContextMenuCommand cmd = eAnmCtxMenuBadCommandID;

	switch (id)
	{
		case IDR_CTXWALK :
			cmd = eAnmCtxMenuWalk ;
			break;

		case IDR_CTXFLY :
			cmd = eAnmCtxMenuFly ;
			break;

		case IDR_CTXEXAMINE :
			cmd = eAnmCtxMenuExamine ;
			break;

		case IDR_CTXSEEK :
			cmd = eAnmCtxMenuSeek ;
			break;

		case IDR_CTXLEVEL :
			cmd = eAnmCtxMenuLevel ;
			break;

		case IDR_CTXBACK :
			cmd = eAnmCtxMenuBack ;
			break;

		case IDR_CTXFORWARD :
			cmd = eAnmCtxMenuForward ;
			break;

		case IDR_CTXVIEWFIRST :
			cmd = eAnmCtxMenuViewFirst ;
			break;

		case IDR_CTXVIEWLAST :
			cmd = eAnmCtxMenuViewLast ;
			break;

		case IDR_CTXVIEWNEXT :
			cmd = eAnmCtxMenuViewNext ;
			break;

		case IDR_CTXVIEWPREVIOUS :
			cmd = eAnmCtxMenuViewPrevious ;
			break;

		case IDR_CTXVIEWRESTORE :
			cmd = eAnmCtxMenuViewRestore ;
			break;

		case IDR_CTXSPEEDSLOWER :
			cmd = eAnmCtxMenuSpeedSlower ;
			break;

		case IDR_CTXSPEEDSLOW :
			cmd = eAnmCtxMenuSpeedSlow ;
			break;

		case IDR_CTXSPEEDMEDIUM :
			cmd = eAnmCtxMenuSpeedMedium ;
			break;

		case IDR_CTXSPEEDFAST :
			cmd = eAnmCtxMenuSpeedFast ;
			break;

		case IDR_CTXSPEEDFASTER :
			cmd = eAnmCtxMenuSpeedFaster ;
			break;

		case IDR_CTXINFOCENTER :
			cmd = eAnmCtxMenuInfoCenter ;
			break;

		case IDR_CTXPREFERENCES :
			cmd = eAnmCtxMenuPreferences ;
			break;

		case IDR_CTXHELPUSERSGUIDE :
			cmd = eAnmCtxMenuHelpUserGuide ;
			break;

		case IDR_CTXHELPABOUT :
			cmd = eAnmCtxMenuHelpAbout ;
			break;

		default :
			if (id >= IDR_CTXVIEWPOINTS_BASE && id < IDR_CTXVIEWPOINTS_MAX)
			{
				int viewindex = id - IDR_CTXVIEWPOINTS_BASE;
				cmd = (eAnmContextMenuCommand) 
					((int) eAnmCtxMenuViewpointsBase + viewindex);
			}
			break;
	}

	return cmd;
}

// N.B.: this is totally position-dependent for current menu!
#define NAVMODEBASE 0
#define NNAVMODES 3

void CAnmContextMenu::UpdateContextMenu()
{
	CAnmViewer *pViewer = m_app->GetViewer();
	assert(pViewer);

	eNavigationMode navmode = pViewer->GetNavigationMode();

	int cmd = 0;
	switch (navmode)
	{
		case eNavigationModeFly :
			cmd = IDR_CTXFLY;
			break;

		case eNavigationModeExamine :
			cmd = IDR_CTXEXAMINE;
			break;

		case eNavigationModeWalk :
			cmd = IDR_CTXWALK;
		default :
			break;
	}

	for (int i = 0; i < NNAVMODES; i++)
	{
		CheckMenuItem(m_hMenu, NAVMODEBASE + i, MF_BYPOSITION | MF_UNCHECKED);
	}

	if (cmd)
	{
		CheckMenuItem(m_hMenu, cmd, MF_BYCOMMAND | MF_CHECKED);
	}

	bool canundo = m_app->CanUndoMove();

	if (canundo)
		EnableMenuItem(m_hMenu, IDR_CTXBACK, MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(m_hMenu, IDR_CTXBACK, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	bool canredo = m_app->CanRedoMove();

	if (canredo)
		EnableMenuItem(m_hMenu, IDR_CTXFORWARD, MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(m_hMenu, IDR_CTXFORWARD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	
}

// N.B.: this is totally dependent on the context menu layout!
// any changes to that *must* be reflected in this position constant!
#define VIEWPOINTLISTBASE 6

void CAnmContextMenu::UpdateViewMenu(cAnimaApp *pApp, CAnmContextMenu *pViewMenu)
{
	CAnmWorld *pWorld = pApp->GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	HMENU hViewMenu = (HMENU) pViewMenu->GetMenuHandle();

 	if (hViewMenu)
	{

		int i, cnt;

		cnt = GetMenuItemCount(hViewMenu);
		for (i = 0; i < cnt; i++)
			DeleteMenu(hViewMenu, VIEWPOINTLISTBASE, MF_BYPOSITION);

		std::list<CAnmViewpoint *> *pViewpoints =
			pWorld->GetViewpoints();
		std::list<class CAnmViewpoint *>::iterator iter;

		for (i = 0, iter = pViewpoints->begin(); i < pViewpoints->size(); i++, iter++)
		{
			String s;
			CAnmViewpoint *pViewpoint = *iter;
			pViewpoint->GetDescription(&s);
			bool alloced = false;
			if (s->length() == 0)
			{
				// ANMMSG_CTXMENUVIEWPOINT : "Viewpoint %d"
				char *vpname = (char *) malloc(strlen(ANMMSG_CTXMENUVIEWPOINT) + 
					ANMMAXDECIMALLENGTH +
					1);

				sprintf(vpname, ANMMSG_CTXMENUVIEWPOINT, i);
				s = new CAnmString(vpname);
				delete vpname;

				alloced = true;
			}
			AppendMenu(hViewMenu, MF_STRING, IDR_CTXVIEWPOINTS_BASE + i, s->GetBuf());
			if (pViewpoint == pWorld->GetViewpoint())
				CheckMenuItem(hViewMenu, IDR_CTXVIEWPOINTS_BASE + i, MF_BYCOMMAND | MF_CHECKED);

			if (alloced)
				delete s;

			if (IDR_CTXVIEWPOINTS_BASE + i >= IDR_CTXVIEWPOINTS_MAX)
				break;
		}

		if (i == 0)
		{
			AppendMenu(hViewMenu, MF_STRING, IDR_CTXVIEWPOINTS_BASE, "No defined viewpoints");
			EnableMenuItem(hViewMenu, IDR_CTXVIEWPOINTS_BASE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}
}

#define NSPEEDS 5

void CAnmContextMenu::UpdateSpeedMenu(cAnimaApp *pApp, CAnmContextMenu *pSpeedMenu)
{
	CAnmViewer *pViewer = pApp->GetViewer();
	assert(pViewer);

	HMENU hSpeedMenu = (HMENU) pSpeedMenu->GetMenuHandle();

	int cmd = 0;
	switch (pViewer->GetUserSpeed())
	{
		case eUserSpeedSlowest :
			cmd = IDR_CTXSPEEDSLOWER;
			break;

		case eUserSpeedSlow :
			cmd = IDR_CTXSPEEDSLOW;
			break;

		case eUserSpeedFast :
			cmd = IDR_CTXSPEEDFAST;
			break;

		case eUserSpeedFastest :
			cmd = IDR_CTXSPEEDFASTER;
			break;

		case eUserSpeedMedium :
		default :
			cmd = IDR_CTXSPEEDMEDIUM;
			break;
	}

	for (int i = 0; i < NSPEEDS; i++)
	{
		CheckMenuItem(hSpeedMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	}

	if (cmd)
	{
		CheckMenuItem(hSpeedMenu, cmd, MF_BYCOMMAND | MF_CHECKED);
	}
}
