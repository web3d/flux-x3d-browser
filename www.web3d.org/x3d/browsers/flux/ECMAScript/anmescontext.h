/********************************************************************************
 * Flux
 *
 * File: anmescontext.h
 * Description: ECMAScript execution context (and scene) object
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

#ifndef _anmescontext_h
#define _anmescontext_h

#include "anmesobject.h"

class CAnmESExecutionContext : public CAnmESBuiltinObject
{
protected :

public :

	// Constructor/destructor
	CAnmESExecutionContext(class CAnmExecutionContext *pContext, class CAnmECMAScriptLanguageBinding *pOwner);
	virtual ~CAnmESExecutionContext();

	// New Methods
	virtual AnmESVar getNode(int nArgs, AnmESVar *args);
	virtual AnmESVar getImportedNode(int nArgs, AnmESVar *args);
	virtual AnmESVar createNode(int nArgs, AnmESVar *args);
	virtual AnmESVar createProto(int nArgs, AnmESVar *args);
	virtual AnmESVar getProtoDeclaration(int nArgs, AnmESVar *args);
	virtual AnmESVar addRoute(int nArgs, AnmESVar *args);
	virtual AnmESVar deleteRoute(int nArgs, AnmESVar *args);
	virtual AnmESVar getSpecificationVersion();
	virtual AnmESVar getEncoding();
	virtual AnmESVar getProfile();
	virtual AnmESVar getComponents();
	virtual AnmESVar getWorldURL();
	virtual AnmESVar getRootNodes();
	virtual AnmESVar getRoutes();

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESExecutionContext);

	DECLARE_ESMETHOD(getNode);
	DECLARE_ESMETHOD(getImportedNode);
	DECLARE_ESMETHOD(createNode);
	DECLARE_ESMETHOD(createProto);
	DECLARE_ESMETHOD(getProtoDeclaration);
	DECLARE_ESMETHOD(getRootNodes);
	DECLARE_ESMETHOD(addRoute);
	DECLARE_ESMETHOD(deleteRoute);
	DECLARE_ESMETHOD(getRoutes);

	DECLARE_ESPROPERTY(specificationVersion);
	DECLARE_ESPROPERTY(encoding);
	DECLARE_ESPROPERTY(profile);
	DECLARE_ESPROPERTY(components);
	DECLARE_ESPROPERTY(worldURL);
	DECLARE_ESPROPERTY(rootNodes);
	DECLARE_ESPROPERTY(routes);

};

class CAnmESScene : public CAnmESExecutionContext
{
protected :

public :

	// Constructor/destructor
	CAnmESScene(class CAnmSAIScene *pScene, class CAnmECMAScriptLanguageBinding *pOwner);
	virtual ~CAnmESScene();

	// New Methods
	virtual AnmESVar getMetaData(int nArgs, AnmESVar *args);
	virtual AnmESVar setMetaData(int nArgs, AnmESVar *args);

	virtual AnmESVar getExportedNode(int nArgs, AnmESVar *args);

	virtual AnmESVar addRootNode(int nArgs, AnmESVar *args);
	virtual AnmESVar removeRootNode(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESScene);

	DECLARE_ESMETHOD(getNode);
	DECLARE_ESMETHOD(getImportedNode);
	DECLARE_ESMETHOD(createNode);
	DECLARE_ESMETHOD(createProto);
	DECLARE_ESMETHOD(getProtoDeclaration);
	DECLARE_ESMETHOD(getRootNodes);
	DECLARE_ESMETHOD(addRoute);
	DECLARE_ESMETHOD(deleteRoute);
	DECLARE_ESMETHOD(getRoutes);
	DECLARE_ESMETHOD(getMetaData);
	DECLARE_ESMETHOD(setMetaData);
	DECLARE_ESMETHOD(getExportedNode);
	DECLARE_ESMETHOD(addRootNode);
	DECLARE_ESMETHOD(removeRootNode);
};

#endif // _anmescontext_h
