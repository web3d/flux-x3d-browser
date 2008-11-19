/********************************************************************************
 * Flux
 *
 * File: FluxPlayer.h
 * Description: Declaration of the CFluxPlayer
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

// FluxPlayer.h : Declaration of the CFluxPlayer

#ifndef __FLUXPLAYER_H_
#define __FLUXPLAYER_H_

#include "resource.h"       // main symbols
#include "anmdisphelper.h"

/////////////////////////////////////////////////////////////////////////////
// CFluxPlayer
class ATL_NO_VTABLE CFluxPlayer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFluxPlayer, &CLSID_FluxPlayer>,
	public IDispatchImpl<IFluxPlayer, &IID_IFluxPlayer, &LIBID_ANIMALib>,
	public CAnmDispHelper
{
public:
	CFluxPlayer()
	{
		m_fluxcontrol = NULL;
		m_fpdisp = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FLUXPLAYER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFluxPlayer)
	COM_INTERFACE_ENTRY(IFluxPlayer)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IFluxPlayer
public:
	STDMETHOD(setFluxControl)(void *pFluxCtrl);
	STDMETHOD(get_src)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_src)(/*[in]*/ BSTR newVal);
	STDMETHOD(AboutBox)();


protected :
	class CAnimaCtrl *m_fluxcontrol;
	IDispatch *m_fpdisp;
};

#endif //__FLUXPLAYER_H_
