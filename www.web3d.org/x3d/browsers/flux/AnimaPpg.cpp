/********************************************************************************
 * Flux
 *
 * File: AnimaPpg.cpp
 * Description: Implementation of the CAnimaPropPage property page class
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

// AnimaPpg.cpp : Implementation of the CAnimaPropPage property page class.

#include "stdafx.h"
#include "Anima.h"
#include "AnimaPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAnimaPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CAnimaPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAnimaPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAnimaPropPage, "ANIMA.AnimaPropPage.1",
	0x84c26562, 0x217f, 0x4d0b, 0x87, 0xb, 0xd5, 0x1, 0xdf, 0xb4, 0xbd, 0x9a)


/////////////////////////////////////////////////////////////////////////////
// CAnimaPropPage::CAnimaPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CAnimaPropPage

BOOL CAnimaPropPage::CAnimaPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ANIMA_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaPropPage::CAnimaPropPage - Constructor

CAnimaPropPage::CAnimaPropPage() :
	COlePropertyPage(IDD, IDS_ANIMA_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CAnimaPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaPropPage::DoDataExchange - Moves data between page and properties

void CAnimaPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CAnimaPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaPropPage message handlers
