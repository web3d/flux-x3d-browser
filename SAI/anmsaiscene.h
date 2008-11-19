/********************************************************************************
 * Flux
 *
 * File: anmsaiscene.h
 * Description: Scene object for SAI
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

#ifndef _anmsaiscene_h
#define _anmsaiscene_h

#include "anmexecutioncontext.h"

class EXPORT CAnmSAIScene : public CAnmExecutionContext
{
protected :

public :

	// Constructor/destructor
	CAnmSAIScene(class cApplication *pApp, class CAnmScene *pScene,
		class CAnmSAINodeEventSink *pSink);
	virtual ~CAnmSAIScene();

	// New methods
	virtual String GetMetaData(const char *key);
	virtual void SetMetaData(String key, String value);

	virtual class CAnmSAINode *GetExportedNode(const char *nodename);

	virtual void AddRootNode(class CAnmSAINode *pSAIRootNode);
	virtual void RemoveRootNode(class CAnmSAINode *pSAIRootNode);

	virtual class CAnmScene *GetScene();

	DECLARE_CLASS(CAnmSAIScene);

};

#endif // _anmsaiscene_h

