/********************************************************************************
 * Flux
 *
 * File: anmcontextmenu.h
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

#ifndef _anmcontextmenu_h
#define _anmcontextmenu_h

#include "anmmenucommands.h"

class EXPORT CAnmContextMenu
{
protected :

	class cAnimaApp			*m_app;
	HMENU					 m_hMenu;

	int IDFromCommand(eAnmContextMenuCommand cmd);
	eAnmContextMenuCommand CommandFromID(int id);

public :

	CAnmContextMenu(class cAnimaApp *pApp, void *handle);
	virtual ~CAnmContextMenu();

	virtual eAnmContextMenuCommand TrackMenu(int x, int y);

	virtual CAnmContextMenu *GetSubMenu(eAnmContextMenuPopup subMenuID);

	void UpdateContextMenu();
	static void UpdateViewMenu(cAnimaApp *pApp, CAnmContextMenu *pViewMenu);
	static void UpdateSpeedMenu(cAnimaApp *pApp, CAnmContextMenu *pSpeedMenu);
	
	void *GetMenuHandle()
	{
		return m_hMenu;
	}
};

#endif // _anmcontextmenu_h
