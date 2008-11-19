/********************************************************************************
 * Flux
 *
 * File: X3DEventOutObserver.cpp
 * Description: Implementation of CX3DEventOutObserver
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

// X3DEventOutObserver.cpp : Implementation of CX3DEventOutObserver
#include "stdafx.h"
#include "Anima.h"
#include "X3DEventOutObserver.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DEventOutObserver



STDMETHODIMP CX3DEventOutObserver::callbackFunction(X3DField *value, double timeStamp, IDispatch *userData)
{
	// TODO: Add your implementation code here

	return S_OK;
}

