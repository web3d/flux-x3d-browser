/********************************************************************************
 * Flux
 *
 * File: npx3dbrowser.h
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

#ifndef _npx3dbrowser_h
#define _npx3dbrowser_h

#include "scriptablepluginobject.h"
#include "anmbrowser.h"

struct sAnmBrowserListenerCallback {

	NPObject *m_listener;

	sAnmBrowserListenerCallback(NPObject *pListener)
	{
		m_listener = pListener;
	}
};

struct sAnmThreadedBrowserCallback {
	class CNPX3DBrowser *pBrowser;
	eAnmBrowserEvent event;
};

 
class CNPX3DBrowser : public ScriptablePluginObject
{
protected :

	class CAnmBrowser		*m_browser;
	std::list<sAnmBrowserListenerCallback> m_listeners;

	static void browserEventCallback(eAnmBrowserEvent event, void *userData);
	void handleBrowserEvent(eAnmBrowserEvent event);
	void callThreadedCallbacks(eAnmBrowserEvent event);

	// NP Glue
	static bool m_classInitialized;
	DECLARE_NPMETHODID(getName);
	DECLARE_NPMETHODID(getVersion);
	DECLARE_NPMETHODID(getExecutionContext);
	DECLARE_NPMETHODID(addBrowserListener);
	DECLARE_NPMETHODID(loadUrlS);
	DECLARE_NPMETHODID(createX3DFromString);
	DECLARE_NPMETHODID(createX3DFromURL);
	DECLARE_NPMETHODID(firstViewpoint);
	DECLARE_NPMETHODID(lastViewpoint);
	DECLARE_NPMETHODID(nextViewpoint);
	DECLARE_NPMETHODID(previousViewpoint);
	DECLARE_NPPROPID(viewpoints);

	void initClass();

public:
	CNPX3DBrowser(NPP npp)
	: ScriptablePluginObject(npp)
	{
		m_browser = NULL;

		initClass();
	}

	virtual ~CNPX3DBrowser();

	virtual bool getViewpoints(/*[out,retval]*/ class NPArray **viewpoints);
	virtual bool previousViewpoint();
	virtual bool nextViewpoint();
	virtual bool lastViewpoint();
	virtual bool firstViewpoint();

	// old-style VRML; keep for reference
	// bool createX3DFromURL(/*[in]*/ char * url, /*[in]*/ X3DNode *node, /*[in]*/ char * event);
	virtual bool createX3DFromURL(/*[in]*/ char * url, /*[out, retval]*/ class CNPX3DScene **scenereturn);
	virtual bool createX3DFromString(/*[in]*/ char * x3dSource, /*[out, retval]*/ class CNPX3DScene **scenereturn);
	virtual bool setDescription(/*[in]*/ char * description);
	virtual bool loadURL(/*[in]*/ int nUrls, /*[in, size_is(nUrls)]*/ char **url, /*[in]*/ int nParams, /*[in,size_is(nParams)]*/ char **params);
	virtual bool replaceWorld(/*[in]*/ class CNPX3DScene *value);
	virtual bool getWorldURL(/*[out, retval]*/ const char **value);
	virtual bool getCurrentFrameRate(/*[out, retval]*/ float *value);
	virtual bool getCurrentSpeed(/*[out, retval]*/ float *value);
	virtual bool getVersion(/*[out, retval]*/ const char **value);
	virtual bool getName(/*[out, retval]*/ const char **value);
	virtual bool getClosure(/*[out, retval]*/ void *closure);
	virtual bool setClosure(/*[in]*/ void *closure);
	virtual bool print(/*[in]*/ const char * str);
	virtual bool loadUrlS(/*[in]*/ const char * url);
	virtual bool getExecutionContext(/*[out, retval]*/ class CNPX3DExecutionContext **ppContext);
	virtual bool addBrowserListener(/*[in]*/ NPObject  *pListener);
	virtual bool removeBrowserListener(/*[in]*/ NPObject  *pListener);

	static void CallX3DCallback(sAnmThreadedBrowserCallback *pTCB);

	// NP Glue
	static CNPX3DBrowser *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

#endif // _npx3dbrowser_h
