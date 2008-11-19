/********************************************************************************
 * Flux
 *
 * File: FluxPlayer.cpp
 * Description: Implementation of CFluxPlayer
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

// FluxPlayer.cpp : Implementation of CFluxPlayer
#include "stdafx.h"
#include "Anima.h"
#include "FluxPlayer.h"
#include "animactl.h"

/////////////////////////////////////////////////////////////////////////////
// CFluxPlayer

STDMETHODIMP CFluxPlayer::AboutBox()
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CFluxPlayer::get_src(BSTR *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CFluxPlayer::put_src(BSTR newVal)
{
	char buf[1024];
	wcstombs(buf, newVal, wcslen(newVal) + 1);

	// TODO: Add your implementation code here
	VARIANT vt;
	vt.bstrVal = newVal;
	vt.vt = VT_BSTR;
	PutProp(m_fpdisp, L"src", vt);
	return S_OK;
}

STDMETHODIMP CFluxPlayer::setFluxControl(void *pFluxCtrl)
{
	// TODO: Add your implementation code here
	m_fluxcontrol = (CAnimaCtrl *) pFluxCtrl;

	m_fpdisp = m_fluxcontrol->GetIDispatch(FALSE);

	return S_OK;
}
