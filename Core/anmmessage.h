/********************************************************************************
 * Flux
 *
 * File: anmmessage.h
 * Description: Message string definitions
 *
 *		We *could* use Window string resources, but they're not
 *			portable and they're hardly better than #defines in 
 *			practice-- TP
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

#ifndef _anmmessage_h
#define _anmmessage_h

// Application
#define ANMMSG_WARNING					"Warning: "
#define ANMMSG_WARNINGSTATUSMESSAGE		"There were warnings; see FLUX console for details"

// File Manager
#define ANMMSG_LOADINGURL				"Loading %s..."
#define ANMMSG_STREAMINGURL				"Streaming %s..."
#define ANMMSG_DONELOADINGURL			"Done"
#define ANMMSG_URLNOTFOUND				"not found."
#define ANMMSG_URLCOULDNOTBELOCATED		"the URL could not be located."
#define ANMMSG_COULDNTDECRYPTFILE		"Couldn't decrypt file %s"
#define ANMMSG_COULDNTUNZIPFILE			"Couldn't unzip file %s"

// File Loader
#define ANMMSG_UNKNOWNNODETYPE			"Unknown node type '%s' at line %d; skipping"
#define ANMMSG_UNIMPLEMENTEDNODETYPE	"Unimplemented node type '%s' at line %d; skipping"
#define ANMMSG_ERRORUSENODE				"Attempt to USE non-existent or unimplemented node '%s' at line %d; skipping"
#define ANMMSG_ERRORROUTENODE			"Warning: non-existent or unimplemented node '%s' in ROUTE statement at line %d; skipping."
#define ANMMSG_ERRORROUTEFIELD			"Warning: unknown field '%s' of node '%s' in ROUTE statement at line %d; skipping."
#define ANMMSG_ERRORROUTETYPES			"Warning: incompatible field types in ROUTE statement at line %d; skipping."
#define ANMMSG_UNKNOWNFIELD				"Warning: unknown field '%s' at line %d; skipping"
#define ANMMSG_UNKNOWNPROTOFIELDINIS	"Warning: unknown PROTO field in IS statement: '%s' at line %d; skipping"
#define ANMMSG_INCOMPATIBLEFIELDACCESS	"Warning: incompatible field access for IS: field '%s' at line %d; skipping"
#define ANMMSG_ERRORISCONNECTTYPES		"Warning: different field types in IS connect ( %s IS %s ) at line %d; skipping."

// VRML97 Loader
#define ANMMSG_VRML97LOADERROR			"VRML97 Load error: [%s] at line %d"
#define ANMMSG_DONEPARSINGWITHERRORS	"Done parsing with errors."

// XML Loader
#define ANMMSG_DONEPARSINGXMLWITHERRORS	"Done parsing XML with errors."
#define ANMMSG_COULDNOTCREATESAXREADER	"Could not create SAX reader to load XML."
#define ANMMSG_CONNECTTAGERROR			"Connect tag requires nodeField and protoField values"
#define ANMMSG_BADXMLFIELDDECLARATION	"Error at line %d: field declarations allowed only in Script, Shader or ProtoInterface"
#define ANMMSG_UNKNOWNFIELDACCESSXML	"Error at line %d: Unknown field access type %s"
#define ANMMSG_UNKNOWNFIELDTYPEXML		"Error at line %d: Unknown field type %s"
#define ANMMSG_FIELDDECLMISSINGREQUIRED	"Error: field declaration is missing required values"
#define ANMMSG_UNKNOWNFIELDNAMEXML		"Unknown field name %s"
#define ANMMSG_FIELDVALUEREQNAMEXML		"Field value requires a name"
#define ANMMSG_UNKNOWNCONTAINERFIELD	"Warning: unknown container field: %s at line %d"
#define ANMMSG_UNEXPECTEDENDTAGXML		"Unexpected end tag (%s); expected to match %s."
#define ANMMSG_UNKNOWNSTARTTAGXML		"Unknown tag (%s); expected %s tag to start scene."
#define ANMMSG_CONNECTTAGREQUIRESIS		"Connect tags must appear within an IS element"
#define ANMMSG_CHILDELEMENTSUNDERUSE	"Warning: child elements under a USE"
#define ANMMSG_XMLPARSEERROR			"XML parse error at line %d: %s"
#define ANMMSG_XMLPARSEWARNING			"XML parse warning at line %d: %s"
#define ANMMSG_BADSFIMAGEXML			"Warning: malformed SFImage value at or near line %d"
#define ANMMSG_BADMFSTRINGXML			"Warning: malformed MFString value at or near line %d; missing interior quotes?"


// Runtime
#define ANMMSG_ERRORLOADINGTEXTURE		"Error loading texture '%s' : %s; exiting"
#define ANMMSG_PROTOBUILDERBADOPTOPLEVEL	"[ProtoBuilder] bad opcode at top PROTO level"
#define ANMMSG_PROTOBUILDERBADOPMAKENODE	"[ProtoBuilder] bad opcode in MakeNode"
#define ANMMSG_PROTOBUILDERENDOFCODE	"[ProtoBuilder] unexpected end of code stream in MakeNode"
#define ANMMSG_OCTREEALLOCFAILED		"Octree allocation failed."
#define ANMMSG_WAVCOULDNTOPENFILE		"Error: couldn't open WAV file"
#define ANMMSG_WAVCOULDNTCREATEBUFFER	"DirectSound::CreateSoundBuffer failed"
#define ANMMSG_WAVCOULDNTGETBUFFERSTATUS	"Couldn't get buffer status" 
#define ANMMSG_WAVOUTOFMEMORY			"Out of memory"
#define ANMMSG_WAVREADFAILED			"m_pWaveSoundRead->Read failed"
#define ANMMSG_WAVBUFLOCKFAILED			"m_pBuffer->Lock failed"
#define ANMMSG_WAVPLAYFAILED			"m_pBuffer->Play failed"
#define ANMMSG_WAVSTOPFAILED			"m_pBuffer->Stop failed"
#define ANMMSG_DSOUNDCREATEFAILED		"DirectSoundCreate failed" 
#define ANMMSG_DSOUNDSCLFAILED			"SetCooperativeLevel (DS) failed"
#define ANMMSG_DSOUNDCSBFAILED			"CreateSoundBuffer (DS) failed"
#define ANMMSG_DSOUNDSETFORMATFAILED	"SetFormat (DS) failed"
#define ANMMSG_COULDNTFINDGZIP			"Couldn't find gzip executable"
#define ANMMSG_OUTOFMEMORY				"Out of memory"
#define ANMMSG_SCRIPTCALLOVERFLOW		"Script method call stack overflow"
#define ANMMSG_SCRIPTERRORSTATUS		"Flux: Script error"
#define ANMMSG_UNKNOWNSHADERTYPE		"Unknown shader type: '%s'"
#define ANMMSG_DSOUND_FAILED			"Warning: Failed to create Direct Sound Device"
#define ANMMSG_DSOUND_BUFFER_FAILED		"Warning: Failed to create Direct Sound Buffer"

// Names
#define ANMMSG_CTXMENUVIEWPOINT			"Viewpoint %d"

#endif //_anmmessage_h