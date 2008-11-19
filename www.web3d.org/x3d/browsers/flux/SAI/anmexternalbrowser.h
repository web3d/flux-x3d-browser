/********************************************************************************
 * Flux
 *
 * File: anmexternalbrowser.h
 * Description: Browser object (for SAI external access)
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

#ifndef _anmexternalbrowser_h
#define _anmexternalbrowser_h

#include "anmbrowser.h"
#include "anmsainodeventsink.h"
#include "anmexternaleventsource.h"

class EXPORT CAnmExternalBrowser : public CAnmBrowser, public CAnmSAINodeEventSink,
	public CAnmExternalEventSource
{
protected :

public :

	// Constructor/destructor
	CAnmExternalBrowser(class cApplication *pApp);
	virtual ~CAnmExternalBrowser();

	// New methods
	virtual void beginUpdate();
	virtual void endUpdate();

	// CAnmBrowser overrides
	virtual class CAnmExecutionContext *getExecutionContext();

	// CAnmSAINodeEventSink overrides
	virtual void NotifyNodeChanged(class CAnmSAINode *pNode);

	// CAnmExternalEventSource overrides
	virtual void ProcessEvents();


	DECLARE_CLASS(CAnmExternalBrowser);

};

#endif // _anmexternalbrowser_h
