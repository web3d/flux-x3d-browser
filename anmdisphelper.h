/********************************************************************************
 * Flux
 *
 * File: anmdisphelper.h
 * Description: Helpers for stupid IDispatch tricks
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



#ifndef _anmdisphelper_h
#define _anmdisphelper_h

class CAnmDispHelper
{
public :

	HRESULT PutProp(IDispatch *pDisp, BSTR name, VARIANT val)
	{
		HRESULT hr;

		DISPID putid = DISPID_PROPERTYPUT;

		DISPPARAMS dpParms;
		dpParms.rgvarg = &val;
		dpParms.cArgs = 1;
		dpParms.rgdispidNamedArgs = &putid;
		dpParms.cNamedArgs = 1;

		DISPID dpPropertyID;
		hr = GetID(pDisp, name, &dpPropertyID);
		if (FAILED(hr))
			return E_FAIL;

		EXCEPINFO excepInfo;
		VARIANT varRetVal;

		return pDisp->Invoke(dpPropertyID,
			IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT,
			&dpParms, &varRetVal, &excepInfo, NULL);
	}

	HRESULT GetID(IDispatch *pDisp, BSTR name, DISPID *pID)
	{
		return pDisp->GetIDsOfNames(IID_NULL, &name, 1, 
			LOCALE_SYSTEM_DEFAULT, pID);
	}
};

#endif // _anmdisphelper_h
