/********************************************************************************
 * Flux
 *
 * File: npx3dexternalbrowser.cpp
 * Description: Mozilla X3DBrowser 
 *							Scriptable Plugin Object
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
#include "npx3dexternalbrowser.h"

#include "plugin.h"

#include "anmexternalbrowser.h"
#include "animaapp.h"
#include "anmsymbol.h"
#include "anmroute.h"
#include "anmfileloader.h"
#include "anmproto.h"
#include "anmexecutioncontext.h"
#include "anmsainode.h"
#include "anmsaiscene.h"
#include "anmscene.h"
#include "anmworld.h"

CNPX3DExternalBrowser::~CNPX3DExternalBrowser()
{
	if (m_plugin)
		m_plugin->NotifyBrowserDestroyed();
}

bool CNPX3DExternalBrowser::beginUpdate()
{
	CAnmExternalBrowser *pExtBrowser =
		(CAnmExternalBrowser *) m_browser;

	pExtBrowser->beginUpdate();

	return true;
}

bool CNPX3DExternalBrowser::endUpdate()
{
	CAnmExternalBrowser *pExtBrowser =
		(CAnmExternalBrowser *) m_browser;

	pExtBrowser->endUpdate();

	return true;

}

// NP Glue 
DEFINE_NPMETHODID(CNPX3DExternalBrowser,beginUpdate);
DEFINE_NPMETHODID(CNPX3DExternalBrowser,endUpdate);
bool CNPX3DExternalBrowser::m_classInitialized = false;

static NPObject *
AllocateCNPX3DExternalBrowser(NPP npp, NPClass *aClass)
{
  return new CNPX3DExternalBrowser(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DExternalBrowser,
                                 AllocateCNPX3DExternalBrowser);

CNPX3DExternalBrowser *CNPX3DExternalBrowser::Create(NPP npp)
{
	return (CNPX3DExternalBrowser *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DExternalBrowser));
}


void CNPX3DExternalBrowser::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(beginUpdate);
	INIT_NPMETHODID(endUpdate);

	m_classInitialized = true;
}

bool
CNPX3DExternalBrowser::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name,beginUpdate) ||
	  TEST_NPMETHODID(name,endUpdate) ||		
	  CNPX3DBrowser::HasMethod(name);
	  ;

}

bool
CNPX3DExternalBrowser::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	if (TEST_NPMETHODID(name,beginUpdate)) {

		return beginUpdate();
	}
	else if (TEST_NPMETHODID(name,endUpdate)) {

		return endUpdate();
	}
	else
		return CNPX3DBrowser::Invoke(name, args, argCount, result);

  return PR_FALSE;
}

bool
CNPX3DExternalBrowser::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DExternalBrowser]"), *result);

  return PR_TRUE;
}

bool
CNPX3DExternalBrowser::HasProperty(NPIdentifier name)
{
	return CNPX3DBrowser::HasProperty(name);
}

bool
CNPX3DExternalBrowser::GetProperty(NPIdentifier name, NPVariant *result)
{
	return CNPX3DBrowser::GetProperty(name, result);
}

bool
CNPX3DExternalBrowser::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	return CNPX3DBrowser::SetProperty(name, value);
}
