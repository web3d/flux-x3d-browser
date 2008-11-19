/********************************************************************************
 * Flux
 *
 * File: anmfileloader.cpp
 * Description: File loader class
 *						Dispatches to specific types: X3D, VRML, Binary
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
#include "anmfileloader.h"
#include "anmvrml97reader.h"
#include "anmxmldomreader.h"
#include "anmxmlsaxreader.h"
#include "anmdecrypt.h"
#include "anmfilemanager.h"
#include "anmgunzip.h"
#include "anmmessage.h"
#include "anmnode.h"
#include "anmproto.h"
#include "anmprotobuilder.h"
#include "anmroute.h"
#include "anmscene.h"
#include "anmsymbol.h"
#include "anmworld.h"

// Includes for pretty much every built-in node here
#include "anmgroup.h"
#include "anmtransform.h"
#include "anmshape.h"
#include "anmanchor.h"
#include "anmappearance.h"
#include "anmbackground.h"
#include "anmbillboard.h"
#include "anmbooleanfilter.h"
#include "anmbooleansequencer.h"
#include "anmbooleantoggle.h"
#include "anmbooleantrigger.h"
#include "anmbox.h"
#include "anmcomposedcubemaptexture.h"
#include "anmaudioclip.h"
#include "anmaudiosensor.h"
#include "anmcollision.h"
#include "anmcolor.h"
#include "anmcolorinterpolator.h"
#include "anmcone.h"
#include "anmcoordinate.h"
#include "anmcoordinateinterpolator.h"
#include "anmcoordinateinterpolator2D.h"
#include "anmcylinder.h"
#include "anmcylindersensor.h"
#include "anmdirectionallight.h"
#include "anmeaseineaseout.h"
#include "anmelevationgrid.h"
#include "anmextrudedfontstyle.h"
#include "anmextrusion.h"
#include "anmfog.h"
#include "anmfontstyle.h"
#include "anmscreenfontstyle.h"
#include "anmlayout.h"
#include "anmhanimdisplacer.h"
#include "anmhanimhumanoid.h"
#include "anmhanimjoint.h"
#include "anmhanimsegment.h"
#include "anmhanimsite.h"
#include "anmimagetexture.h"
#include "anmindexedfaceset.h"
#include "anmindexedlineset.h"
#include "anmindexedtriangleset.h"
#include "anminline.h"
#include "anmintegersequencer.h"
#include "anmintegertrigger.h"
#include "anmkeysensor.h"
#include "anmlayoutgroup.h"
#include "anmlayer2D.h"
#include "anmlayer3D.h"
#include "anmlineset.h"
#include "anmlod.h"
#include "anmloadsensor.h"
#include "anmmaterial.h"
#include "anmmetadatadouble.h"
#include "anmmetadatafloat.h"
#include "anmmetadatainteger.h"
#include "anmmetadataset.h"
#include "anmmetadatastring.h"
#include "anmmousesensor.h"
#include "anmmovietexture.h"
#include "anmmultitexture.h"
#include "anmmultitexturecoordinate.h"
#include "anmmultitexturetransform.h"
#include "anmnormal.h"
#include "anmnormalinterpolator.h"
#include "anmnavigationinfo.h"
#include "anmnurbspatchsurface.h"
#include "anmnurbssurfaceinterpolator.h"
#include "anmnurbstexturecoordinate.h"
#include "anmparticleset.h"
#include "anmpixeltexture.h"
#include "anmplanesensor.h"
#include "anmpointlight.h"
#include "anmpointset.h"
#include "anmpositioninterpolator.h"
#include "anmpositioninterpolator2D.h"
#include "anmprogramshader.h"
#include "anmproximitysensor.h"
#include "anmorientationinterpolator.h"
#include "anmscalarinterpolator.h"
#include "anmscreengroup.h"
#include "anmscript.h"
#include "anmshaderprogram.h"
#include "anmsound.h"
#include "anmsphere.h"
#include "anmspheresensor.h"
#include "anmsplinepositioninterpolator.h"
#include "anmsplinepositioninterpolator2D.h"
#include "anmsplinescalarinterpolator.h"
#include "anmspotlight.h"
#include "anmstringsensor.h"
#include "anmswitch.h"
#include "anmtext.h"
#include "anmtexturecoordinate.h"
#include "anmtexturecoordinategenerator.h"
#include "anmtexturetransform.h"
#include "anmtimesensor.h"
#include "anmtimetrigger.h"
#include "anmtouchsensor.h"
#include "anmtrianglefanset.h"
#include "anmtriangleset.h"
#include "anmtrianglestripset.h"
#include "anmviewpoint.h"
#include "anmvisibilitysensor.h"
#include "anmworldinfo.h"

#include "anmniynodes.h"


#define TESTBUFFERLEN	200


CAnmProtoSymbol *CAnmFileLoader::m_globalNames = NULL;	// node class types, init once
// CAnmCriticalSection CAnmFileLoader::m_criticalSection;
std::vector<CAnmFileLoader *> CAnmFileLoader::m_loaders;

CAnmFileLoader::CAnmFileLoader(cApplication *pApp, String baseUrl, BOOL consoleOutput) :
m_app(pApp),
m_consoleOutput(consoleOutput)
{
	m_baseUrl = baseUrl;
	m_baseUrl->Ref();

	CreateClassSymbols();

	m_sceneNames = CreateNewTopLevelScene(pApp);
	m_protoDeclaring = m_sceneNames;

	m_nodeSymbolDefining = NULL;
	m_nodelevel = 0;
	m_scriptDefining = NULL;
	m_shaderDefining = NULL;

	m_fieldAccess = eAnmValueMember;

	m_skipLevel = 0;

	m_encodingType = eAnmX3DBadEncodingValue;
}

CAnmFileLoader::~CAnmFileLoader()
{
	SafeDelete(m_sceneNames);

	int sz = m_metadatakeys.size();
	for (int i = 0; i < sz; i++)
	{
		m_metadatakeys[i]->UnRef();
		m_metadatavalues[i]->UnRef();
	}

	SafeUnRef(m_baseUrl);
}

CAnmSceneSymbol *CAnmFileLoader::CreateNewTopLevelScene(cApplication *pApp)
{
	String s = new CAnmString(SCENE_PROTO_NAME);
	CAnmSceneSymbol *pSceneSym = new CAnmSceneSymbol(pApp, s, m_globalNames);
	s->UnRef();

	return pSceneSym;
}

void CAnmFileLoader::CreateClassSymbols()
{
	// m_criticalSection.Lock();

	if (m_globalNames == NULL)
	{
		String s = new CAnmString(BROWSER_PROTO_NAME);
		m_globalNames = new CAnmProtoSymbol(m_app, s, NULL);
		s->UnRef();
		InitClassSymbols(m_globalNames->GetClassSymbols());
	}

	// m_criticalSection.Unlock();
}

void CAnmFileLoader::UnloadAllClassSymbols()
{
	// m_criticalSection.Lock();

	SafeDelete(m_globalNames);

	int nloaders = m_loaders.size();

	for (int i = 0; i < nloaders; i++)
	{
		SafeDelete(m_loaders[i]);
	}

	// m_criticalSection.Unlock();
}

void CAnmFileLoader::AddClassSymbol(CAnmSymbolTable *pClassSymbols, const char *classname,
									CAnmClass *pClass, const char *defContainerField)
{
	
	assert(pClass);

	String s = new CAnmString(classname);
	CAnmClassSymbol *pClassSym = new CAnmClassSymbol(s, pClass);
	s->UnRef();

	CAnmClassMember **pMemberTable = pClass->GetMemberTable();

	for (int i = 0; i < pClass->GetNumMembers(); i++)
	{
		CAnmClassMember *pMember = pMemberTable[i];
		pClassSym->AddAttribute(pMember->name, pMember);
	}

	s = new CAnmString(defContainerField);
	pClassSym->SetDefaultContainerField(s);
	s->UnRef();

	pClassSymbols->AddSymbol(pClassSym);

	// N.B.: this needs to move eventually
	pClass->m_className = (char *) classname;
}


void CAnmFileLoader::InitClassSymbols(CAnmSymbolTable *pClassSymbols)
{
	AddClassSymbol(pClassSymbols, "Anchor", GETCLASS(CAnmAnchor));
	AddClassSymbol(pClassSymbols, "Appearance", GETCLASS(CAnmAppearance), "appearance");
	AddClassSymbol(pClassSymbols, "AudioClip", GETCLASS(CAnmAudioClip), "source");
	AddClassSymbol(pClassSymbols, "AudioSensor", GETCLASS(CAnmAudioSensor) );
	AddClassSymbol(pClassSymbols, "Background", GETCLASS(CAnmBackground));
	AddClassSymbol(pClassSymbols, "Billboard", GETCLASS(CAnmBillboard));
	AddClassSymbol(pClassSymbols, "BooleanFilter", GETCLASS(CAnmBooleanFilter));
	AddClassSymbol(pClassSymbols, "BooleanSequencer", GETCLASS(CAnmBooleanSequencer));
	AddClassSymbol(pClassSymbols, "BooleanToggle", GETCLASS(CAnmBooleanToggle));
	AddClassSymbol(pClassSymbols, "BooleanTrigger", GETCLASS(CAnmBooleanTrigger));
	AddClassSymbol(pClassSymbols, "Box", GETCLASS(CAnmBox), "geometry");
	AddClassSymbol(pClassSymbols, "ComposedCubeMapTexture", GETCLASS(CAnmComposedCubeMapTexture), "texture");
	AddClassSymbol(pClassSymbols, "Collision", GETCLASS(CAnmCollision));
	AddClassSymbol(pClassSymbols, "Color", GETCLASS(CAnmColor), "color");
	AddClassSymbol(pClassSymbols, "ColorInterpolator", GETCLASS(CAnmColorInterpolator));
	AddClassSymbol(pClassSymbols, "Cone", GETCLASS(CAnmCone), "geometry");
	AddClassSymbol(pClassSymbols, "Coordinate", GETCLASS(CAnmCoordinate), "coord");
	AddClassSymbol(pClassSymbols, "CoordinateInterpolator", GETCLASS(CAnmCoordinateInterpolator));
	AddClassSymbol(pClassSymbols, "CoordinateInterpolator2D", GETCLASS(CAnmCoordinateInterpolator2D));
	AddClassSymbol(pClassSymbols, "Cylinder", GETCLASS(CAnmCylinder), "geometry");
	AddClassSymbol(pClassSymbols, "CylinderSensor", GETCLASS(CAnmCylinderSensor));
	AddClassSymbol(pClassSymbols, "DirectionalLight", GETCLASS(CAnmDirectionalLight));
	AddClassSymbol(pClassSymbols, "ElevationGrid", GETCLASS(CAnmElevationGrid), "geometry");
	AddClassSymbol(pClassSymbols, "Extrusion", GETCLASS(CAnmExtrusion), "geometry");
	AddClassSymbol(pClassSymbols, "ExtrudedFontStyle", GETCLASS(CAnmExtrudedFontStyle), "fontStyle");
	AddClassSymbol(pClassSymbols, "EaseInEaseOut", GETCLASS(CAnmEaseInEaseOut) );
	AddClassSymbol(pClassSymbols, "Fog", GETCLASS(CAnmFog));
	AddClassSymbol(pClassSymbols, "FontStyle", GETCLASS(CAnmFontStyle), "fontStyle");
	AddClassSymbol(pClassSymbols, "Layout", GETCLASS(CAnmLayout), "layout");
	AddClassSymbol(pClassSymbols, "Group", GETCLASS(CAnmGroup));
	AddClassSymbol(pClassSymbols, "HAnimDisplacer", GETCLASS(CAnmHAnimDisplacer));
	AddClassSymbol(pClassSymbols, "HAnimHumanoid", GETCLASS(CAnmHAnimHumanoid));
	AddClassSymbol(pClassSymbols, "HAnimJoint", GETCLASS(CAnmHAnimJoint));
	AddClassSymbol(pClassSymbols, "HAnimSegment", GETCLASS(CAnmHAnimSegment));
	AddClassSymbol(pClassSymbols, "HAnimSite", GETCLASS(CAnmHAnimSite));
	AddClassSymbol(pClassSymbols, "ImageTexture", GETCLASS(CAnmImageTexture), "texture");
	AddClassSymbol(pClassSymbols, "IndexedFaceSet", GETCLASS(CAnmIndexedFaceSet), "geometry");
	AddClassSymbol(pClassSymbols, "IndexedLineSet", GETCLASS(CAnmIndexedLineSet), "geometry");
	AddClassSymbol(pClassSymbols, "IndexedTriangleSet", GETCLASS(CAnmIndexedTriangleSet), "geometry");
	AddClassSymbol(pClassSymbols, "Inline", GETCLASS(CAnmInline));
	AddClassSymbol(pClassSymbols, "IntegerSequencer", GETCLASS(CAnmIntegerSequencer));
	AddClassSymbol(pClassSymbols, "IntegerTrigger", GETCLASS(CAnmIntegerTrigger));
	AddClassSymbol(pClassSymbols, "KeySensor", GETCLASS(CAnmKeySensor));
	AddClassSymbol(pClassSymbols, "LayoutGroup", GETCLASS(CAnmLayoutGroup));
	AddClassSymbol(pClassSymbols, "Layer2D", GETCLASS(CAnmLayer2D));
	AddClassSymbol(pClassSymbols, "Layer3D", GETCLASS(CAnmLayer3D));
	AddClassSymbol(pClassSymbols, "LineSet", GETCLASS(CAnmLineSet), "geometry");
	AddClassSymbol(pClassSymbols, "LoadSensor", GETCLASS(CAnmLoadSensor));
	AddClassSymbol(pClassSymbols, "LOD", GETCLASS(CAnmLOD));
	AddClassSymbol(pClassSymbols, "Material", GETCLASS(CAnmMaterial), "material");
	AddClassSymbol(pClassSymbols, "MetadataDouble", GETCLASS(CAnmMetadataDouble), "metadata");
	AddClassSymbol(pClassSymbols, "MetadataFloat", GETCLASS(CAnmMetadataFloat), "metadata");
	AddClassSymbol(pClassSymbols, "MetadataInteger", GETCLASS(CAnmMetadataInteger), "metadata");
	AddClassSymbol(pClassSymbols, "MetadataSet", GETCLASS(CAnmMetadataSet), "metadata");
	AddClassSymbol(pClassSymbols, "MetadataString", GETCLASS(CAnmMetadataString), "metadata");
	AddClassSymbol(pClassSymbols, "MouseSensor", GETCLASS(CAnmMouseSensor));
	AddClassSymbol(pClassSymbols, "MovieTexture", GETCLASS(CAnmMovieTexture), "texture");
	AddClassSymbol(pClassSymbols, "MultiTexture", GETCLASS(CAnmMultiTexture), "texture");
	AddClassSymbol(pClassSymbols, "MultiTextureCoordinate", GETCLASS(CAnmMultiTextureCoordinate), "texCoord");
	AddClassSymbol(pClassSymbols, "MultiTextureTransform", GETCLASS(CAnmMultiTextureTransform), "textureTransform");
	AddClassSymbol(pClassSymbols, "NavigationInfo", GETCLASS(CAnmNavigationInfo));
	AddClassSymbol(pClassSymbols, "Normal", GETCLASS(CAnmNormal), "normal");
	AddClassSymbol(pClassSymbols, "NormalInterpolator", GETCLASS(CAnmNormalInterpolator));
	AddClassSymbol(pClassSymbols, "NurbsPatchSurface", GETCLASS(CAnmNurbsPatchSurface), "geometry");
	AddClassSymbol(pClassSymbols, "NurbsSurfaceInterpolator", GETCLASS(CAnmNurbsSurfaceInterpolator));
	AddClassSymbol(pClassSymbols, "NurbsTextureCoordinate", GETCLASS(CAnmNurbsTextureCoordinate), "texCoord");
	AddClassSymbol(pClassSymbols, "OrientationInterpolator", GETCLASS(CAnmOrientationInterpolator));
	AddClassSymbol(pClassSymbols, "ParticleSet", GETCLASS(CAnmParticleSet), "geometry");
	AddClassSymbol(pClassSymbols, "PixelTexture", GETCLASS(CAnmPixelTexture), "texture");
	AddClassSymbol(pClassSymbols, "PlaneSensor", GETCLASS(CAnmPlaneSensor));
	AddClassSymbol(pClassSymbols, "PointLight", GETCLASS(CAnmPointLight));
	AddClassSymbol(pClassSymbols, "PointSet", GETCLASS(CAnmPointSet), "geometry");
	AddClassSymbol(pClassSymbols, "PositionInterpolator", GETCLASS(CAnmPositionInterpolator));
	AddClassSymbol(pClassSymbols, "PositionInterpolator2D", GETCLASS(CAnmPositionInterpolator2D));
	AddClassSymbol(pClassSymbols, "ProgramShader", GETCLASS(CAnmProgramShader), "shader");
	AddClassSymbol(pClassSymbols, "ProximitySensor", GETCLASS(CAnmProximitySensor));
	AddClassSymbol(pClassSymbols, "ScreenGroup", GETCLASS(CAnmScreenGroup));
	AddClassSymbol(pClassSymbols, "Script", GETCLASS(CAnmScript));
	AddClassSymbol(pClassSymbols, "ScalarInterpolator", GETCLASS(CAnmScalarInterpolator));
	AddClassSymbol(pClassSymbols, "ScreenFontStyle", GETCLASS(CAnmScreenFontStyle), "fontStyle");
	AddClassSymbol(pClassSymbols, "ShaderProgram", GETCLASS(CAnmShaderProgram), "programs");
	AddClassSymbol(pClassSymbols, "Shape", GETCLASS(CAnmShape));
	AddClassSymbol(pClassSymbols, "Sound", GETCLASS(CAnmSound));
	AddClassSymbol(pClassSymbols, "Sphere", GETCLASS(CAnmSphere), "geometry");
	AddClassSymbol(pClassSymbols, "SphereSensor", GETCLASS(CAnmSphereSensor));
	AddClassSymbol(pClassSymbols, "SplinePositionInterpolator", GETCLASS(CAnmSplinePositionInterpolator));
	AddClassSymbol(pClassSymbols, "SplinePositionInterpolator2D", GETCLASS(CAnmSplinePositionInterpolator2D));
	AddClassSymbol(pClassSymbols, "SplineScalarInterpolator", GETCLASS(CAnmSplineScalarInterpolator));
	AddClassSymbol(pClassSymbols, "SpotLight", GETCLASS(CAnmSpotLight));
	AddClassSymbol(pClassSymbols, "StringSensor", GETCLASS(CAnmStringSensor));
	AddClassSymbol(pClassSymbols, "Switch", GETCLASS(CAnmSwitch));
	AddClassSymbol(pClassSymbols, "Text", GETCLASS(CAnmText), "geometry");
	AddClassSymbol(pClassSymbols, "TextureCoordinate", GETCLASS(CAnmTextureCoordinate), "texCoord");
	AddClassSymbol(pClassSymbols, "TextureCoordinateGenerator", GETCLASS(CAnmTextureCoordinateGenerator), "texCoord");
	AddClassSymbol(pClassSymbols, "TextureTransform", GETCLASS(CAnmTextureTransform), "textureTransform");
	AddClassSymbol(pClassSymbols, "TimeSensor", GETCLASS(CAnmTimeSensor));
	AddClassSymbol(pClassSymbols, "TimeTrigger", GETCLASS(CAnmTimeTrigger));
	AddClassSymbol(pClassSymbols, "TouchSensor", GETCLASS(CAnmTouchSensor));
	AddClassSymbol(pClassSymbols, "Transform", GETCLASS(CAnmTransform));
	AddClassSymbol(pClassSymbols, "TriangleFanSet", GETCLASS(CAnmTriangleFanSet), "geometry");
	AddClassSymbol(pClassSymbols, "TriangleSet", GETCLASS(CAnmTriangleSet), "geometry");
	AddClassSymbol(pClassSymbols, "TriangleStripSet", GETCLASS(CAnmTriangleStripSet), "geometry");
	AddClassSymbol(pClassSymbols, "Viewpoint", GETCLASS(CAnmViewpoint));
	AddClassSymbol(pClassSymbols, "VisibilitySensor", GETCLASS(CAnmVisibilitySensor));
	AddClassSymbol(pClassSymbols, "WorldInfo", GETCLASS(CAnmWorldInfo));

	// dummy nodes for not implemented features
	CAnmNIYNodes::InitNIYClassSymbols(pClassSymbols);

}

CAnmSymbolTable *CAnmFileLoader::GetClassSymbols()
{
	assert(m_globalNames != NULL);

	return m_globalNames->GetClassSymbols();
}

// Loader/reader methods
eAnmReturnStatus CAnmFileLoader::LoadFile(const char *filename,
										  String baseUrl,
										  CAnmScene **ppSceneReturn,
										  CAnmSceneSymbol **ppSceneSymbol,
										  cApplication *pApp, BOOL consoleOutput)
{

	eAnmReturnStatus retstatus = eAnmReturnAllBad;

	// N.B.: when to delete the loader?
	CAnmFileLoader *pLoader = new CAnmFileLoader(pApp, baseUrl, consoleOutput);

	m_loaders.push_back(pLoader);

	// N.B.: length - we could blow this buffer

	char gzbuf[ANMMAXSTRBUF];
	char encryptbuf[ANMMAXSTRBUF];

	char* fileThatMustBeDeleted = NULL;


	// krv
	// Encryption:  
	//
	if (CAnmDecrypt::TestForEncrypt(filename))
	{
		if (CAnmDecrypt::Decrypt(filename, encryptbuf, ANMMAXSTRBUF) )
		{
			filename = encryptbuf;
			fileThatMustBeDeleted = encryptbuf;
		}
		else
		{
			// ANMMSG_COULDNTDECRYPTFILE : "Couldn't decrypt file %s"
			char *errmsg = (char *) malloc(strlen(ANMMSG_COULDNTDECRYPTFILE) + 
				strlen(filename) +
				1);

			sprintf(errmsg, ANMMSG_COULDNTUNZIPFILE, filename);
			
			CAnmError err(errmsg);

			delete errmsg;

			throw err;

		}
	}



	if (CAnmGunzip::TestForGzip(filename))
	{
		if (ANMSUCCEEDED(CAnmGunzip::Gunzip(filename, gzbuf, ANMMAXSTRBUF)))
		{
			filename = gzbuf;
			if( fileThatMustBeDeleted ) {
				unlink( fileThatMustBeDeleted );
			}
			fileThatMustBeDeleted = gzbuf;
		}
		else
		{
			// ANMMSG_COULDNTUNZIPFILE : "Couldn't unzip file %s"
			char *errmsg = (char *) malloc(strlen(ANMMSG_COULDNTUNZIPFILE) + 
				strlen(filename) +
				1);

			sprintf(errmsg, ANMMSG_COULDNTUNZIPFILE, filename);
			
			CAnmError err(errmsg);

			delete errmsg;

			throw err;
		}
	}


	const char *ext = strrchr(filename, '.');


	bool bIsX3D = false;
	bool bIsVRML = false;
	char extOnStack[3] = "\0";

	// krv;
	// If ext is NULL, better peek into the file to determine the file type.
	// IE might blow away the extension is the file is gzipped.
	//
#if 1
	if (ext)
	{
		if (stricmp(ext, ".x3d") &&
			stricmp(ext, ".xml") &&
			stricmp(ext, ".wrl") && 
			stricmp(ext, ".x3dv"))

			ext = NULL;
	}
#endif

	if( !ext) 
	{
		// we dont want a NULL ext, EVER!
		//
		ext = extOnStack;
		char tmpBuffer[TESTBUFFERLEN];
		FILE* fpTest = fopen( filename, "r" );
		if( fpTest ) {
			int nRead = fread( tmpBuffer, TESTBUFFERLEN, 1, fpTest );
			// terminate the string, and make it lowercase to prepare for our search.
			//
			tmpBuffer[TESTBUFFERLEN-1] = '\0';
			for( int i=0; tmpBuffer[i]; i++ )
			{
				if( !islower( tmpBuffer[i] ) ) {
					tmpBuffer[i] = tolower( tmpBuffer[i] );
				}
			}
			// Search for evidence of VRML or X3D
			//
			char* pX3DFound = strstr( tmpBuffer, "x3d" );
			char* pVRMLFound = strstr( tmpBuffer, "vrml" );

			if (pX3DFound && tmpBuffer[0] == '#')
			{
				// actually VRML-encoded X3D
				pVRMLFound = pX3DFound;
				pX3DFound = NULL;
			}

			// In the off chance we found them both, use the first one found./
			//
			if( pX3DFound && pVRMLFound )
			{
				if( pX3DFound < pVRMLFound )
				{
					bIsX3D = true;
				}
				else 
				{
					bIsVRML = true;
				}
			}
			else if( pX3DFound ) 
			{
				bIsX3D = true;
			}
			else if( pVRMLFound ) 
			{
				bIsVRML = true;
			}
			fclose( fpTest );
		}
	}



	if ( bIsX3D || 
			!stricmp(ext, ".xml") || 
			!stricmp(ext, ".x3d") )
	{
		if (!ANMFAILED(pLoader->LoadXML(filename, true)))

		{
			retstatus = eAnmReturnAllGood;
		}
	}
	else if ( bIsVRML || !stricmp(ext, ".wrl") || !stricmp(ext, ".x3dv") )
	{
		
		if (!ANMFAILED(pLoader->LoadVRML97(filename, true)))

		{
			retstatus = eAnmReturnAllGood;
		}
	}
	else if (!stricmp(ext, ".x3b"))
	{
#ifdef BINARY_LOADER
		if (!ANMFAILED(pLoader->LoadBinary(filename, true)))

		{
			retstatus = eAnmReturnAllGood;
		}
#endif
	}

	if (retstatus == eAnmReturnAllGood)
	{
		CAnmSceneSymbol *pSceneSymbol = pLoader->m_sceneNames;
		
		assert(pSceneSymbol);

		// Save header values in scene symbol
		pSceneSymbol->SetEncodingType(pLoader->GetEncodingType());
		pSceneSymbol->SetSpecificationVersion(pLoader->GetSpecificationVersion());
		
		int sz = pLoader->m_metadatakeys.size();
		int sz2 = pLoader->m_metadatavalues.size();
		if (sz > sz2)
			sz = sz2;

		for (int i = 0; i < sz; i++)
			pSceneSymbol->AddMetaData(pLoader->m_metadatakeys[i], pLoader->m_metadatavalues[i]);

		// It's compiled; build the scene
		if (ppSceneReturn)
		{
			// Create and return the scene
			CAnmScene *pScene = pSceneSymbol->CreateScene(pApp, pLoader->GetBaseUrl());

			*ppSceneReturn = pScene;

		}

		// Return the class symbol table (PROTOs) if requested
		if (ppSceneSymbol)
			*ppSceneSymbol = pSceneSymbol;

	}

	if( fileThatMustBeDeleted ) {
		unlink( fileThatMustBeDeleted );
	}

	return retstatus;
}

eAnmReturnStatus CAnmFileLoader::LoadFile(String source,
										  String baseUrl,
										  CAnmScene **ppSceneReturn,
										  CAnmSceneSymbol **ppSceneSymbol,
										  cApplication *pApp, BOOL consoleOutput)
{

	eAnmReturnStatus retstatus = eAnmReturnAllBad;
	eAnmX3DEncoding encodingtype = eAnmX3DBadEncodingValue;

	// N.B.: when to delete the loader?
	CAnmFileLoader *pLoader = new CAnmFileLoader(pApp, baseUrl, consoleOutput);

	char *cp = source->GetBuf();

	// strip leading whitespace
	while (*cp && isspace(*cp))
		cp++;

	if (!*cp)
		return eAnmReturnAllBad;

	// sniff out the file type
	if (cp[0] == '<')
	{
		if (!ANMFAILED(pLoader->LoadXML(cp, false)))
		{
			retstatus = eAnmReturnAllGood;
		}
	}
	else // everything but XML?
	{		
		if (!ANMFAILED(pLoader->LoadVRML97(cp, false)))

		{
			retstatus = eAnmReturnAllGood;
		}
	}

	if (retstatus == eAnmReturnAllGood)
	{
		CAnmSceneSymbol *pSceneSymbol = pLoader->m_sceneNames;
		
		assert(pSceneSymbol);

		// Save header values in scene symbol
		pSceneSymbol->SetEncodingType(pLoader->GetEncodingType());
		pSceneSymbol->SetSpecificationVersion(pLoader->GetSpecificationVersion());
		
		int sz = pLoader->m_metadatakeys.size();
		int sz2 = pLoader->m_metadatavalues.size();
		if (sz > sz2)
			sz = sz2;

		for (int i = 0; i < sz; i++)
			pSceneSymbol->AddMetaData(pLoader->m_metadatakeys[i], pLoader->m_metadatavalues[i]);

		// It's compiled; build the scene
		if (ppSceneReturn)
		{
			// Create and return the scene
			CAnmScene *pScene = pSceneSymbol->CreateScene(pApp, pLoader->GetBaseUrl());

			*ppSceneReturn = pScene;

		}

		// Return the class symbol table (PROTOs) if requested
		if (ppSceneSymbol)
			*ppSceneSymbol = pSceneSymbol;

	}

	return retstatus;
}

CAnmScene *CAnmFileLoader::CreateScene(const char *filename, String baseUrl,
									 cApplication *pApp, BOOL consoleOutput)
{
	CAnmScene *pScene = NULL;

	eAnmReturnStatus retstatus = LoadFile(filename, baseUrl, &pScene,
		NULL, pApp, consoleOutput);

	if (retstatus == eAnmReturnAllGood)
		return pScene;
	else
		return NULL;
}

CAnmScene *CAnmFileLoader::CreateScene(String source, String baseUrl,
									 cApplication *pApp, BOOL consoleOutput)
{
	CAnmScene *pScene = NULL;

	eAnmReturnStatus retstatus = LoadFile(source, baseUrl, &pScene,
		NULL, pApp, consoleOutput);

	if (retstatus == eAnmReturnAllGood)
		return pScene;
	else
		return NULL;
}

CAnmSceneSymbol *CAnmFileLoader::LoadScene(const char *filename, String baseUrl,
										   cApplication *pApp, BOOL consoleOutput)
{
	CAnmSceneSymbol *pSceneSymbol = NULL;

	eAnmReturnStatus retstatus = LoadFile(filename, baseUrl, NULL, &pSceneSymbol, pApp, consoleOutput);

	if (retstatus == eAnmReturnAllGood)
	{
		assert(pSceneSymbol);
		return pSceneSymbol;
	}
	else
		return NULL;
}

eAnmReturnStatus CAnmFileLoader::LoadVRML97(const char *source, bool fromFile)
{
	m_reader = new CAnmVRML97Reader(this);

	eAnmReturnStatus retstatus = m_reader->Read(source, fromFile);
	
	SafeDelete(m_reader);

	return retstatus;
}

eAnmReturnStatus CAnmFileLoader::LoadXML(const char *source, bool fromFile)
{
	m_reader = new CAnmXMLSAXReader(this);

	eAnmReturnStatus retstatus = m_reader->Read(source, fromFile);
	
	SafeDelete(m_reader);

	return retstatus;
}

// Scene construction methods
static int nBegins = 0, nEnds = 0;

CAnmClassSymbol *CAnmFileLoader::BeginMakeNode(String nodetypename)
{
	char *errmsg = NULL;

	// search for the node type name in our table
	CAnmClassSymbol *pClassSymbol = (CAnmClassSymbol *) LookupClassSymbol(nodetypename->GetBuf());

	if (pClassSymbol == NULL)
	{
		// it's an unknown node; ignore it
		char *nodetypenamebuf = nodetypename->GetBuf();

		// ANMMSG_UNKNOWNNODETYPE : "Unknown node type '%s' at line %d; skipping"
		errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNNODETYPE) + 
			strlen(nodetypenamebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_UNKNOWNNODETYPE, nodetypenamebuf, m_reader->GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;

		m_skipLevel++;
	}
	else
	{
		CAnmClass *pClass = pClassSymbol->GetClass();
		assert(pClass);

		if (!pClass->GetCreateObject())
		{
			// it's an unimplemented node; ignore it
			char *nodetypenamebuf = nodetypename->GetBuf();

			// ANMMSG_UNIMPLEMENTEDNODETYPE	: "Unimplemented node type '%s' at line %d; skipping"
			errmsg = (char *) malloc(strlen(ANMMSG_UNIMPLEMENTEDNODETYPE) + 
				strlen(nodetypenamebuf) +
				ANMMAXDECIMALLENGTH +
				1);

			sprintf(errmsg, ANMMSG_UNIMPLEMENTEDNODETYPE, nodetypenamebuf, m_reader->GetLineNumber());

			WriteToConsole(errmsg);

			delete errmsg;
			m_skipLevel++;
		}
		else
		{
			// set up the DEF
			if (m_nodeSymbolDefining)
			{
				m_nodeSymbolDefining->SetClassSymbol(pClassSymbol);
				NODELOCALOFFSET offset = m_protoDeclaring->GetBuilder()->GenDefNode(
					m_nodeSymbolDefining->GetName()->GetBuf());
				m_nodeSymbolDefining->SetLocalOffset((int) offset);
			}

			// generate the node
			m_protoDeclaring->GetBuilder()->GenBeginMakeNode(pClass);
		}
	}

	m_nodeSymbolDefining = NULL;

	PushClass(pClassSymbol);

	return pClassSymbol;
}

void CAnmFileLoader::EndMakeNode(void)
{
	CAnmClassSymbol *pClassSymbol = m_classstack.top();
	PopClass();
	m_nodeSymbolDefining = NULL;

	nEnds++;

	if (pClassSymbol)
	{
		// Make sure there was a call generated to BeginMakeNode before pairing with End
		CAnmClass *pClass = pClassSymbol->GetClass();
		assert(pClass);

		if (pClass->GetCreateObject())
			m_protoDeclaring->GetBuilder()->GenEndMakeNode();
		else
		{
			--m_skipLevel;
			if (m_skipLevel == 0)
				m_protoDeclaring->GetBuilder()->GenNullNode();	// keep stack happy
		}
	}
	else
	{
		--m_skipLevel;
		if (m_skipLevel == 0)
			m_protoDeclaring->GetBuilder()->GenNullNode();	// keep stack happy
	}

	assert(m_skipLevel >= 0);
}

CAnmClassSymbol *CAnmFileLoader::BeginMakeScript()
{
	static int nScripts = 0;

	char namebuf[128];
	sprintf(namebuf, "ScriptClass_%d", nScripts++);
	String s = new CAnmString(namebuf);
	m_scriptDefining = new CAnmScriptSymbol(s);
	s->UnRef();
	AddClassSymbol(m_scriptDefining);

	return BeginMakeNode(m_scriptDefining->GetName());
}

void CAnmFileLoader::EndMakeScript()
{
	assert(m_scriptDefining != NULL);

	m_scriptDefining->EndDeclaration();
	EndMakeNode();
	m_scriptDefining = NULL;
}

CAnmClassSymbol *CAnmFileLoader::BeginMakeShader()
{
	static int nShaders = 0;

	char namebuf[128];
	sprintf(namebuf, "ShaderClass_%d", nShaders++);

	m_shaderDefining = new CAnmShaderSymbol(new CAnmString(namebuf));

	AddClassSymbol(m_shaderDefining);

	return BeginMakeNode(m_shaderDefining->GetName());
}

void CAnmFileLoader::EndMakeShader()
{
	assert(m_shaderDefining != NULL);

	m_shaderDefining->EndDeclaration();
	EndMakeNode();
	m_shaderDefining = NULL;
}


void CAnmFileLoader::BeginProtoDeclaration(String prototypename, BOOL isExternProto)
{
	CAnmProtoSymbol *pProtoSymbol = new CAnmProtoSymbol(m_app, prototypename, m_protoDeclaring,
		isExternProto);

	m_protoDeclaring = pProtoSymbol;
}

void CAnmFileLoader::EndProtoDeclaration(StringArray **ppExternProtoUrl)
{
	CAnmProtoSymbol *pProtoSymbol = m_protoDeclaring;
	assert(pProtoSymbol != NULL);
	assert(pProtoSymbol != m_sceneNames);

	pProtoSymbol->EndDeclaration();

	// pop name scope
	m_protoDeclaring = (CAnmProtoSymbol *) m_protoDeclaring->GetParentScope();

	// add newly declared Proto to the containing scope
	AddClassSymbol(pProtoSymbol);

	if (pProtoSymbol->IsExternProto() && ppExternProtoUrl)
	{
		pProtoSymbol->SetUrl(m_baseUrl, *ppExternProtoUrl);
	}
}

void CAnmFileLoader::AddClassSymbol(CAnmClassSymbol *pClassSymbol)
{
	m_protoDeclaring->AddClassSymbol(pClassSymbol);
}


CAnmClassSymbol *CAnmFileLoader::LookupClassSymbol(char *classname)
{
	return m_protoDeclaring->FindClassSymbol(classname);
}

void CAnmFileLoader::AddNodeSymbol(CAnmNodeSymbol *pNodeSymbol)
{
	m_protoDeclaring->AddNodeSymbol(pNodeSymbol);
}


CAnmNodeSymbol *CAnmFileLoader::LookupNodeSymbol(char *nodename)
{
	return m_protoDeclaring->FindNodeSymbol(nodename);
}

void CAnmFileLoader::DefNode(String nodename)
{
	m_nodeSymbolDefining = new CAnmNodeSymbol(nodename,
		NULL, NULL);
	AddNodeSymbol(m_nodeSymbolDefining);
}

CAnmClassSymbol *CAnmFileLoader::UseNode(String nodename)
{
	// null class symbol at top of stack means we're skipping an unknown/unimplemented node
	if (!m_classstack.empty())
	{
		CAnmClassSymbol *pClassSymbol = m_classstack.top();
		if (pClassSymbol == NULL)
			return NULL;
	}

	CAnmNodeSymbol *pNodeSymbol = LookupNodeSymbol(nodename->GetBuf());
	if (pNodeSymbol)
	{
		NODELOCALOFFSET offset = (NODELOCALOFFSET) pNodeSymbol->GetLocalOffset();
		if (offset >= 0)
		{
			if (!m_skipLevel)
				m_protoDeclaring->GetBuilder()->GenUseNode(offset);

			return pNodeSymbol->GetClassSymbol();
		}
		else
			return NULL;
	}
	else
	{
		char *errmsg = NULL;

		char *nodenamebuf = nodename->GetBuf();

		// ANMMSG_ERRORUSENODE : "Attempt to USE non-existent or unimplemented node '%s' at line %d; skipping"
		errmsg = (char *) malloc(strlen(ANMMSG_ERRORUSENODE) + 
			strlen(nodenamebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_ERRORUSENODE, nodenamebuf, m_reader->GetLineNumber());

		if (!m_skipLevel)
			m_protoDeclaring->GetBuilder()->GenNullNode();

		WriteToConsole(errmsg);

		delete errmsg;

		return NULL;
	}
}

void CAnmFileLoader::NullNode()
{
	if (!m_skipLevel)
		m_protoDeclaring->GetBuilder()->GenNullNode();
}

void CAnmFileLoader::SetValue(CAnmAttributeSymbol *pAttrSymbol,void *pData)
{
	assert(!m_classstack.empty());

	// null class symbol at top of stack means we're skipping an unknown/unimplemented node
	CAnmClassSymbol *pClassSymbol = m_classstack.top();
	if (pClassSymbol == NULL)
		return;

	// Unimplemented; don't do SetValue
	CAnmClass *pClass = pClassSymbol->GetClass();
	assert(pClass);

	if (!pClass->GetCreateObject())
		return;

	CLASSMEMBERID mid;

	mid = pAttrSymbol->GetClassMember();

	if (!m_skipLevel)
	{
		if (mid->valueType == eValueNode)
			m_protoDeclaring->GetBuilder()->GenSetSFNodeValue(mid);
		else if (mid->valueType == eValueNodeArray)
		{
			m_protoDeclaring->GetBuilder()->GenSetMFNodeValue(mid);
		}
		else
			m_protoDeclaring->GetBuilder()->GenSetValue(mid, pData, m_protoDeclaring != m_sceneNames);
	}
}

void CAnmFileLoader::AppendNode(CAnmAttributeSymbol *pAttrSymbol)
{
	assert(!m_classstack.empty());

	// null class symbol at top of stack means we're skipping an unknown/unimplemented node
	CAnmClassSymbol *pClassSymbol = m_classstack.top();
	if (pClassSymbol == NULL)
		return;

	// Unimplemented; don't do SetValue
	CAnmClass *pClass = pClassSymbol->GetClass();
	assert(pClass);

	if (!pClass->GetCreateObject())
		return;

	CLASSMEMBERID mid;

	mid = pAttrSymbol->GetClassMember();

	if (!m_skipLevel)
	{
		assert(mid->valueType == eValueNodeArray);
		m_protoDeclaring->GetBuilder()->GenAppendNode(mid);
	}
}

void CAnmFileLoader::MakeRoute(String fromnode, String fromfield,
								 String tonode, String tofield)
{
	char *errmsg = NULL;
	char *nodenamebuf = NULL;
	char *fieldnamebuf = NULL;

	CAnmNodeSymbol *pFromNodeSymbol, *pToNodeSymbol;
	CAnmAttributeSymbol *pAttrSymbol;
	CAnmRoute *pRoute = NULL;
	CLASSMEMBERID from_mid, to_mid;
	char buf[1024];

	// get the syms for from and to nodes
	pFromNodeSymbol = LookupNodeSymbol(fromnode->GetBuf());
	pToNodeSymbol = LookupNodeSymbol(tonode->GetBuf());

	if (pFromNodeSymbol == NULL || pFromNodeSymbol->GetClassSymbol() == NULL)
	{
		nodenamebuf = fromnode->GetBuf();

		// ANMMSG_ERRORROUTENODE : "Warning: non-existent or unimplemented node '%s' in ROUTE statement at line %d; skipping."
		errmsg = (char *) malloc(strlen(ANMMSG_ERRORROUTENODE) + 
			strlen(nodenamebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_ERRORROUTENODE, nodenamebuf, m_reader->GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;

		return;
	}

	if (pToNodeSymbol == NULL || pToNodeSymbol->GetClassSymbol() == NULL)
	{
		nodenamebuf = tonode->GetBuf();

		// ANMMSG_ERRORROUTENODE : "Warning: non-existent or unimplemented node '%s' in ROUTE statement at line %d; skipping."
		errmsg = (char *) malloc(strlen(ANMMSG_ERRORROUTENODE) + 
			strlen(nodenamebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_ERRORROUTENODE, nodenamebuf, m_reader->GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;
		return;
	}

	assert(strlen(fromfield->GetBuf()));
	assert(strlen(tofield->GetBuf()));

	from_mid = NULL;
	pAttrSymbol = pFromNodeSymbol->GetClassSymbol()->FindAttribute(fromfield->GetBuf(), eAnmCallbackMember);

	if (pAttrSymbol != NULL)
		from_mid = pAttrSymbol->GetClassMember();
	else
	{
		// try the automatic _changed thing
		sprintf(buf, "%s_changed", fromfield->GetBuf());
		pAttrSymbol = pFromNodeSymbol->GetClassSymbol()->FindAttribute(buf, eAnmCallbackMember);
		if (pAttrSymbol != NULL)
			from_mid = pAttrSymbol->GetClassMember();
	}

	to_mid = NULL;
	pAttrSymbol = pToNodeSymbol->GetClassSymbol()->FindAttribute(tofield->GetBuf(), eAnmMethodMember);

	if (pAttrSymbol != NULL)
		to_mid = pAttrSymbol->GetClassMember();
	else
	{
		// try the automatic set_ thing
		sprintf(buf, "set_%s", tofield->GetBuf());
		pAttrSymbol = pToNodeSymbol->GetClassSymbol()->FindAttribute(buf, eAnmMethodMember);
		if (pAttrSymbol != NULL)
			to_mid = pAttrSymbol->GetClassMember();
	}

	if (from_mid == NULL)
	{
		fieldnamebuf = fromfield->GetBuf();
		nodenamebuf = fromnode->GetBuf();

		// ANMMSG_ERRORROUTEFIELD :"Warning: unknown field '%s' of node '%s' in ROUTE statement at line %d; skipping."
		errmsg = (char *) malloc(strlen(ANMMSG_ERRORROUTEFIELD) + 
			strlen(fieldnamebuf) +
			strlen(nodenamebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_ERRORROUTEFIELD, fieldnamebuf, nodenamebuf, m_reader->GetLineNumber());
		WriteToConsole(errmsg);

		delete errmsg;

		return;
	}

	if (to_mid == NULL)
	{
		fieldnamebuf = tofield->GetBuf();
		nodenamebuf = tonode->GetBuf();

		// ANMMSG_ERRORROUTEFIELD :"Warning: unknown field '%s' of node '%s' in ROUTE statement at line %d; skipping."
		errmsg = (char *) malloc(strlen(ANMMSG_ERRORROUTEFIELD) + 
			strlen(fieldnamebuf) +
			strlen(nodenamebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_ERRORROUTEFIELD, fieldnamebuf, nodenamebuf, m_reader->GetLineNumber());
		WriteToConsole(errmsg);

		delete errmsg;

		return;
	}

	if (from_mid->valueType != to_mid->valueType)
	{
		// ANMMSG_ERRORROUTETYPES : "Warning: incompatible field types in ROUTE statement at line %d; skipping."
		errmsg = (char *) malloc(strlen(ANMMSG_ERRORROUTETYPES) + 
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_ERRORROUTETYPES, m_reader->GetLineNumber());
		WriteToConsole(errmsg);

		delete errmsg;

		return;
	}

	if (!m_skipLevel)
		m_protoDeclaring->GetBuilder()->GenMakeRoute(pFromNodeSymbol->GetLocalOffset(),
			from_mid, pToNodeSymbol->GetLocalOffset(), to_mid);
}


CAnmAttributeSymbol *CAnmFileLoader::LookupAttribute(char *name)
{
	if (m_classstack.empty())
		return NULL;

	CAnmClassSymbol *pClassSymbol = m_classstack.top();

	if (pClassSymbol)
	{
		CAnmAttributeSymbol *pAttrSymbol = 
			pClassSymbol->FindAttribute(name, (eAnmClassMemberType) (eAnmValueMember | eAnmInitMember));
		
		return pAttrSymbol;
	}
	else
		return NULL;
}

void CAnmFileLoader::PushClass(CAnmClassSymbol *pClassSymbol)
{
	m_classstack.push(pClassSymbol);
}

CAnmClassSymbol *CAnmFileLoader::PopClass()
{
	assert(!m_classstack.empty());

	CAnmClassSymbol *pClass = m_classstack.top();
	
	// assert(pClass); don't; it might be a dummy class for an unknown node

	m_classstack.pop();

	return pClass;
}



void CAnmFileLoader::SetFieldAccess(eAnmClassMemberType memberType)
{
	m_fieldAccess = memberType;
}

CAnmAttributeSymbol *CAnmFileLoader::AddField(eValueType fieldType, String fieldName, void *defaultValue)
{
	CAnmDynClassSymbol *pDynClassSym = NULL;
	CAnmAttributeSymbol *pAttrSym = NULL;

	if (m_shaderDefining)
		pDynClassSym = m_shaderDefining;
	else if (m_scriptDefining)
		pDynClassSym = m_scriptDefining;
	else
		pDynClassSym = m_protoDeclaring;

	// add the value member
	pAttrSym = pDynClassSym->AddAttribute(fieldName, fieldType, m_fieldAccess, defaultValue);

	// Default node values are dynamic; must generate code to save them to node
	CLASSMEMBERID mid = pAttrSym->GetClassMember();

	if (mid->valueType == eValueNode)
	{
#if 0
		// $$$ TP experiment 11-25-03
		if (!m_skipLevel && !m_protoDeclaring->IsExternProto())
			m_protoDeclaring->GetBuilder()->GenSetSFNodeValue(mid);
#else
		if (!m_skipLevel)
		{
			// For a script, this is not really a default value, but an initial value
			// and settable here. However, for a proto it's actually a thing to be
			// set as a default value in its class symbol.
			if (m_scriptDefining || m_shaderDefining)
				m_protoDeclaring->GetBuilder()->GenSetSFNodeValue(mid);
			else if (!m_protoDeclaring->IsExternProto())
				m_protoDeclaring->GetBuilder()->GenSetSFNodeDefault(mid);
		}
#endif
	}
	else if (mid->valueType == eValueNodeArray)
	{
#if 0
		// $$$ TP experiment 11-25-03
		if (!m_skipLevel && !m_protoDeclaring->IsExternProto())
			m_protoDeclaring->GetBuilder()->GenSetMFNodeValue(mid);
#else
		if (!m_skipLevel)
		{
			// For a script, this is not really a default value, but an initial value
			// and settable here. However, for a proto it's actually a thing to be
			// set as a default value in its class symbol.
			if (m_scriptDefining || m_shaderDefining)
				m_protoDeclaring->GetBuilder()->GenSetMFNodeValue(mid);
			else if (!m_protoDeclaring->IsExternProto())
				m_protoDeclaring->GetBuilder()->GenSetMFNodeDefault(mid);
		}
#endif
	}

	if (m_fieldAccess == eAnmInitMember)
		return pAttrSym;

	// For exposed fields, add the set_ and _changed friends
	char eventbuf[ANMMAXSTRBUF];		// N.B.: length
	
	sprintf(eventbuf, "set_%s", fieldName->GetBuf());
	String s = new CAnmString(eventbuf);
	CAnmAttributeSymbol *pSetAttrSym = pDynClassSym->AddAttribute(s, fieldType, eAnmMethodMember);
	s->UnRef();

	sprintf(eventbuf, "%s_changed", fieldName->GetBuf());
	s = new CAnmString(eventbuf);
	CAnmAttributeSymbol *pChangedAttrSym = pDynClassSym->AddAttribute(s, fieldType, eAnmCallbackMember);
	s->UnRef();

	return pAttrSym;
}

CAnmAttributeSymbol *CAnmFileLoader::AddField(eValueType fieldType, String fieldName, void *defaultValue, String ISName)
{
	CAnmAttributeSymbol *pAttrSym = AddField(fieldType, fieldName, defaultValue);
	AddIS(pAttrSym, ISName);
	return pAttrSym;
}

void CAnmFileLoader::AddEventIn(eValueType fieldType, String eventInName)
{
	CAnmDynClassSymbol *pDynClassSym = NULL;

	if (m_shaderDefining)
		pDynClassSym = m_shaderDefining;
	else if (m_scriptDefining)
		pDynClassSym = m_scriptDefining;
	else
		pDynClassSym = m_protoDeclaring;

	pDynClassSym->AddAttribute(eventInName, fieldType, eAnmMethodMember);
}

void CAnmFileLoader::AddEventOut(eValueType fieldType, String eventOutName)
{
	CAnmDynClassSymbol *pDynClassSym = NULL;

	if (m_shaderDefining)
		pDynClassSym = m_shaderDefining;
	else if (m_scriptDefining)
		pDynClassSym = m_scriptDefining;
	else
		pDynClassSym = m_protoDeclaring;

	pDynClassSym->AddAttribute(eventOutName, fieldType, eAnmCallbackMember);
}

void CAnmFileLoader::AddIS(String name, String ISName)
{
	CAnmClassSymbol *pClassSymbol = m_classstack.top();
	CAnmAttributeSymbol *pAttrSymbol = NULL;

	char *errmsg = NULL;

	if (pClassSymbol)
	{
		pAttrSymbol = pClassSymbol->FindAttribute(name->GetBuf());

		if (pAttrSymbol == NULL)
		{
			char *namebuf = name->GetBuf();

			// ANMMSG_UNKNOWNFIELD : "Warning: unknown field '%s' at line %d; skipping"
			errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELD) + 
				strlen(namebuf) +
				ANMMAXDECIMALLENGTH +
				1);

			sprintf(errmsg, ANMMSG_UNKNOWNFIELD, namebuf, m_reader->GetLineNumber());

			WriteToConsole(errmsg);

			delete errmsg;

			return;
		}

		CAnmAttributeSymbol *pISAttrSym = 
			m_protoDeclaring->FindAttribute(ISName->GetBuf());

		if (pISAttrSym == NULL)
		{
			char *namebuf = ISName->GetBuf();

			// ANMMSG_UNKNOWNPROTOFIELDINIS: "Warning: unknown PROTO field in IS statement: '%s' at line %d; skipping"
			errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNPROTOFIELDINIS) + 
				strlen(namebuf) +
				ANMMAXDECIMALLENGTH +
				1);

			sprintf(errmsg, ANMMSG_UNKNOWNPROTOFIELDINIS, namebuf, m_reader->GetLineNumber());

			WriteToConsole(errmsg);

			delete errmsg;

			return;
		}

		if( pAttrSymbol->GetValueType() != pISAttrSym->GetValueType() ) {
			char *Isnamebuf = ISName->GetBuf();
			char *namebuf = name->GetBuf();

			errmsg = (char *) malloc(strlen(ANMMSG_ERRORISCONNECTTYPES) + 
				strlen(Isnamebuf) +
				strlen(namebuf) +
				ANMMAXDECIMALLENGTH +
				1);

			sprintf(errmsg, ANMMSG_ERRORISCONNECTTYPES,  namebuf, Isnamebuf,m_reader->GetLineNumber());

			WriteToConsole(errmsg);

			delete errmsg;

			return;
			
			
		}

		DoAddIS(pAttrSymbol, pISAttrSym);
	}
}

void CAnmFileLoader::AddIS(class CAnmAttributeSymbol *pAttrSym, String ISName)
{
	CAnmAttributeSymbol *pISAttrSym = 
		m_protoDeclaring->FindAttribute(ISName->GetBuf());

	if (pISAttrSym == NULL)
	{
		char *errmsg = NULL;

		char *namebuf = ISName->GetBuf();

		// ANMMSG_UNKNOWNPROTOFIELDINIS: "Warning: unknown PROTO field in IS statement: '%s' at line %d; skipping"
		errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNPROTOFIELDINIS) + 
			strlen(namebuf) +
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_UNKNOWNPROTOFIELDINIS, namebuf, m_reader->GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;

		return;
	}

	DoAddIS(pAttrSym, pISAttrSym);
}

void CAnmFileLoader::AddIS(class CAnmAttributeSymbol *pAttrSym, class CAnmAttributeSymbol *pISAttrSym)
{
	// Gotta trick the IS attr symbol; if the names are the same in the
	// current instance's class as in the containing PROTO the attr sym will be set
	// up for the current instance's class instead... just the way the lexer works

	// eAnmClassMemberType memberType = pISAttrSym->GetClassMember()->memberType;
	String ProtoFieldName = pISAttrSym->GetName();
	if( ProtoFieldName ) {
		pISAttrSym = m_protoDeclaring->FindAttribute(ProtoFieldName->GetBuf()); // , memberType);
		if( pISAttrSym ) {
			DoAddIS(pAttrSym, pISAttrSym);
		}
		else {
			char errmsg[1000];
			sprintf( errmsg, "Warning: Cant find Proto Interface field for IS: %s",
						ProtoFieldName->GetBuf() );
			WriteToConsole(errmsg);
		}
	}
}

void CAnmFileLoader::AddISField(eValueType fieldType, String name, String ISName)
{
	CAnmAttributeSymbol *pAttrSym = NULL;

	AddField(fieldType, name, NULL, ISName);
}

void CAnmFileLoader::DoAddIS(class CAnmAttributeSymbol *pAttrSym, class CAnmAttributeSymbol *pISAttrSym)
{
	char eventbuf[ANMMAXSTRBUF];		// N.B.: length

	CAnmClassSymbol *pClassSym = m_classstack.top();
	assert(pClassSym != NULL);

	if (pISAttrSym->GetClassMember()->memberType == eAnmValueMember)
	{
		// Add the IS for the exposedField
		if (!m_skipLevel)
		{
			// Rules say: Has to be an exposedField in the definition
			if (pAttrSym->GetClassMember()->memberType == eAnmValueMember)
			{
				m_protoDeclaring->GetBuilder()->GenAddISField(pISAttrSym->GetClassMember(),
					pAttrSym->GetClassMember());

				// do the set_ and _changed friends for exposed fields

				// add the IS for the set_ attribute
				sprintf(eventbuf, "set_%s", pISAttrSym->GetName()->GetBuf());
				CAnmAttributeSymbol *pISEventSym = m_protoDeclaring->FindAttribute(eventbuf, eAnmMethodMember);

				sprintf(eventbuf, "set_%s", pAttrSym->GetName()->GetBuf());
				CAnmAttributeSymbol *pEventSym = pClassSym->FindAttribute(eventbuf, eAnmMethodMember);

				if (pISEventSym && pEventSym)
				{
					m_protoDeclaring->GetBuilder()->GenAddISField(pISEventSym->GetClassMember(),
						pEventSym->GetClassMember());
				}

				// add the IS for the _changed attribute
				sprintf(eventbuf, "%s_changed", pISAttrSym->GetName()->GetBuf());
				pISEventSym = m_protoDeclaring->FindAttribute(eventbuf, eAnmCallbackMember);

				sprintf(eventbuf, "%s_changed", pAttrSym->GetName()->GetBuf());
				pEventSym = pClassSym->FindAttribute(eventbuf, eAnmCallbackMember);

				if (pISEventSym && pEventSym)
				{
					m_protoDeclaring->GetBuilder()->GenAddISField(pISEventSym->GetClassMember(),
						pEventSym->GetClassMember());
				}
			}
			else
			{
				char *namebuf = pAttrSym->GetName()->GetBuf();

				// ANMMSG_INCOMPATIBLEFIELDACCESS : "Warning: incompatible field access for IS: field '%s' at line %d; skipping"

				char *errmsg = (char *) malloc(strlen(ANMMSG_INCOMPATIBLEFIELDACCESS) + 
					strlen(namebuf) +
					ANMMAXDECIMALLENGTH +
					1);

				sprintf(errmsg, ANMMSG_INCOMPATIBLEFIELDACCESS, namebuf, m_reader->GetLineNumber());
				WriteToConsole(errmsg);

				delete errmsg;

				return;
			}
		}
	}
	else if (pAttrSym->GetClassMember()->memberType == eAnmValueMember)
	{
		CAnmAttributeSymbol *pAttrSym2 = NULL;

		// we've got an exposedField ISed to an eventIn or event out in the PROTO
		if (pISAttrSym->GetClassMember()->memberType == eAnmMethodMember)
		{
			sprintf(eventbuf, "set_%s", pAttrSym->GetName()->GetBuf());
			pAttrSym2 = pClassSym->FindAttribute(eventbuf, eAnmMethodMember);

			if (pAttrSym2 == NULL)
			{
				char *namebuf = pISAttrSym->GetName()->GetBuf();

				// ANMMSG_UNKNOWNFIELD : "Warning: unknown field '%s' at line %d; skipping"
				char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELD) + 
					strlen(namebuf) +
					ANMMAXDECIMALLENGTH +
					1);

				sprintf(errmsg, ANMMSG_UNKNOWNFIELD, namebuf, m_reader->GetLineNumber());

				WriteToConsole(errmsg);

				delete errmsg;

				return;
			}
		}
		else if (pISAttrSym->GetClassMember()->memberType == eAnmCallbackMember)
		{
			sprintf(eventbuf, "%s_changed", pAttrSym->GetName()->GetBuf());
			pAttrSym2 = pClassSym->FindAttribute(eventbuf, eAnmCallbackMember);

			if (pAttrSym2 == NULL)
			{
				char *namebuf = pISAttrSym->GetName()->GetBuf();

				// ANMMSG_UNKNOWNFIELD : "Warning: unknown field '%s' at line %d; skipping"
				char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELD) + 
					strlen(namebuf) +
					ANMMAXDECIMALLENGTH +
					1);

				sprintf(errmsg, ANMMSG_UNKNOWNFIELD, namebuf, m_reader->GetLineNumber());

				WriteToConsole(errmsg);

				delete errmsg;

				return;
			}
		}
		else	// must be initializeOnly field
		{
			pAttrSym2 = pAttrSym;
		}

		// Now add the IS
		if (!m_skipLevel)
			m_protoDeclaring->GetBuilder()->GenAddISField(pISAttrSym->GetClassMember(),
				pAttrSym2->GetClassMember());

	}
	else
	{
		// otherwise just add the IS, ahem as is
		if (pAttrSym->GetClassMember()->memberType == pISAttrSym->GetClassMember()->memberType)
		{
			if (!m_skipLevel)
				m_protoDeclaring->GetBuilder()->GenAddISField(pISAttrSym->GetClassMember(),
					pAttrSym->GetClassMember());
		}
		else
		{
			char *namebuf = pAttrSym->GetName()->GetBuf();

			// ANMMSG_INCOMPATIBLEFIELDACCESS : "Warning: incompatible field access for IS: field '%s' at line %d; skipping"

			char *errmsg = (char *) malloc(strlen(ANMMSG_INCOMPATIBLEFIELDACCESS) + 
				strlen(namebuf) +
				ANMMAXDECIMALLENGTH +
				1);

			WriteToConsole(errmsg);

			delete errmsg;

			return;
		}

	}
}

void CAnmFileLoader::AddISEventIn(eValueType fieldType, String name, String ISName)
{
	CAnmAttributeSymbol *pAttrSym = NULL;

	AddEventIn(fieldType, name);

	if (m_shaderDefining)
		pAttrSym = m_shaderDefining->FindAttribute(name->GetBuf());
	else if (m_scriptDefining)
		pAttrSym = m_scriptDefining->FindAttribute(name->GetBuf());
	else
		pAttrSym = m_protoDeclaring->FindAttribute(name->GetBuf());

	assert(pAttrSym != NULL);

	AddIS(pAttrSym, ISName);
}

void CAnmFileLoader::AddISEventOut(eValueType fieldType, String name, String ISName)
{
	CAnmAttributeSymbol *pAttrSym = NULL;

	AddEventOut(fieldType, name);

	if (m_shaderDefining)
		pAttrSym = m_shaderDefining->FindAttribute(name->GetBuf());
	else if (m_scriptDefining)
		pAttrSym = m_scriptDefining->FindAttribute(name->GetBuf());
	else
		pAttrSym = m_protoDeclaring->FindAttribute(name->GetBuf());

	assert(pAttrSym != NULL);

	AddIS(pAttrSym, ISName);
}

void CAnmFileLoader::AddMetaData(String key, String value)
{
	m_metadatakeys.push_back(key);
	m_metadatavalues.push_back(value);

	key->Ref();
	value->Ref();
}

// Utility methods
void CAnmFileLoader::WriteToConsole(char *str)
{
	if (m_app && m_consoleOutput)
		m_app->WriteToConsole(str);
}