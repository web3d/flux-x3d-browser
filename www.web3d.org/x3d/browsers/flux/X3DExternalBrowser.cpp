/********************************************************************************
 * Flux
 *
 * File: X3DExternalBrowser.cpp
 * Description: Implementation of CX3DExternalBrowser
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

// X3DExternalBrowser.cpp : Implementation of CX3DExternalBrowser
#include "stdafx.h"
#include "Anima.h"
#include "X3DExternalBrowser.h"
#include "anmexternalbrowser.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DExternalBrowser


STDMETHODIMP CX3DExternalBrowser::beginUpdate()
{
	CAnmExternalBrowser *pExtBrowser =
		(CAnmExternalBrowser *) m_browser;

	pExtBrowser->beginUpdate();

	return S_OK;
}

STDMETHODIMP CX3DExternalBrowser::endUpdate()
{
	CAnmExternalBrowser *pExtBrowser =
		(CAnmExternalBrowser *) m_browser;

	pExtBrowser->endUpdate();

	return S_OK;
}


