/********************************************************************************
 * Flux
 *
 * File: npx3dexternalbrowser.h
 * Description: Mozilla X3DExternalBrowser 
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

#ifndef _npx3dexternalbrowser_h
#define _npx3dexternalbrowser_h

#include "npx3dbrowser.h"
 
class CNPX3DExternalBrowser : public CNPX3DBrowser
{
protected :

	class FluxPluginObject *m_plugin;

	static bool m_classInitialized;
	DECLARE_NPMETHODID(beginUpdate);
	DECLARE_NPMETHODID(endUpdate);

	void initClass();

public:
  CNPX3DExternalBrowser(NPP npp)
    : CNPX3DBrowser(npp)
  {
	  m_plugin = NULL;
	  initClass();
  }

  virtual ~CNPX3DExternalBrowser();

	static CNPX3DExternalBrowser *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);

	virtual bool beginUpdate();
	virtual bool endUpdate();

	void SetPlugin(class FluxPluginObject *pPlugin)
	{
		m_plugin = pPlugin;
	}
};

#endif // _npx3dexternalbrowser_h
