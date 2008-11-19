/********************************************************************************
 * Flux
 *
 * File: animadefs.h
 * Description: Type and other definitions
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


#ifndef _animadefs_h
#define _animadefs_h

#include <new>
#include <cstdlib> // for malloc() and free()

#include <list>
#include <string>
#include <vector>
#include <deque>

using std::string;
using std::list;
using std::vector;
using std::deque;

// Basic type defines
typedef unsigned char uChar;
typedef unsigned short uShort;
typedef unsigned long uLong;
typedef unsigned int uInt;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned int uint;

#define ANMMAXSTRBUF 2048

// Debugging help
#define DP( a )\
{\
	char buff[1024];\
	sprintf( buff, a );\
	OutputDebugString( buff );\
}

#define DP0 DP

#define DP1( a, b )\
{\
	char buff[1024];\
	sprintf( buff, a, b );\
	OutputDebugString( buff );\
}

#define DP2( a, b, c )\
{\
	char buff[1024];\
	sprintf( buff, a, b, c );\
	OutputDebugString( buff );\
}

#define DP3( a, b, c, d )\
{\
	char buff[1024];\
	sprintf( buff, a, b, c, d );\
	OutputDebugString( buff );\
}

#define DP4( a, b, c, d, e )\
{\
	char buff[1024];\
	sprintf( buff, a, b, c, d, e );\
	OutputDebugString( buff );\
}

// COM Interface management for Windows
// From Perez/Royer, cf. README

/**
 * The Right Way to release our COM interfaces.
 * If they're still valid, release them, then
 * invalidate them.
 */

template <class T>
inline void SafeRelease( T& iface )
{
	if( iface )
	{
		int refCount = iface->Release();
		iface = NULL;
	}
}

template <class T>
inline void SafeDelete( T& obj )
{
	if( obj )
	{
		delete obj;
		obj = NULL;
	}
}

template <class T>
inline void SafeUnRef( T& obj )
{
	if( obj )
	{
		obj->UnRef();
		obj = NULL;
	}
}

/**
 * Make sure the release brings our refcount to 0.
 */
template <class T>
inline bool ExtraSafeRelease( T& iface )
{
	long refCount = 0;
	bool err = false;

	if( iface )
	{
		if( 0 < ( refCount = iface->Release() ) )
		{
			DP0("[ExtraSafeRelease]: ERROR: Interface was not released enough!");
			err = true;
		}

		iface = NULL;

		return err;
	}

	return err;
}

// Direct X structure helpers
// Also from Perez/Royer - TP

/**
 * This class takes care of the annoying gruntwork
 * of having to zero-out and set the size parameter
 * of our Windows and DirectX structures.
 */
template <class T>
struct sAutoZero : public T
{
	sAutoZero()
	{
		memset( this, 0, sizeof(T) );
		dwSize = sizeof(T);
	}
};


template <class T>
inline void DXInit( T& dxstruct )
{
	memset(&dxstruct, 0, sizeof(T));
	dxstruct.dwSize = sizeof(T);
}

// COM string handling helper
inline void bstr2cstr(char *cstr, BSTR bstr, int blen)
{
	wcstombs(cstr, bstr, blen);
	cstr[blen] = '\0';
}

// Utility macros and inline functions
// make a value lie between upper and lower bounds
template <class T> inline T ClampValue(T lower, T value, T upper)
{
	if (value < lower)
		return lower;
	else if (value > upper)
		return upper;
	else
		return value;
}
// swap two values
#define SwapFloat(a, b) \
{ \
	float tmp = a; \
	a = b; \
	b = tmp; \
}

// Return status codes. N.B.: need to review return codes/exception discipline
enum eAnmReturnStatus {
	eAnmReturnAllGood = 0,
	eAnmReturnAllBad = 1,

};

#define ANMSUCCEEDED(x) (x == eAnmReturnAllGood)
#define ANMFAILED(x) (x != eAnmReturnAllGood)

enum eAnmOptions {
	eAnmOptionsCoordinateSystem,
	eAnmOptionsAntialiasing,
	eAnmOptionsPrimitiveQuality,
	eAnmOptionsMultiTexturing,
	eAnmOptionsWireframe,
	eAnmOptionsMaxTextureSize,
	eAnmOptionsDrawTextures,
	eAnmOptionsDrawBoundingBoxes,
	eAnmOptionsDrawOctreeBoxes,
	eAnmOptionsShowLights,
	eAnmOptionsFailureMode,
};

enum eAnmCoordinateSystemOptions {
	eAnmCoordinatesLeftHanded,
	eAnmCoordinatesRightHanded,
};

enum eAnmPrimitiveQualityOptions {
	eAnmPrimitiveGoodQuality,
	eAnmPrimitiveBetterQuality,
	eAnmPrimitiveBestQuality,
};

enum eAnmFailureModeOptions {
	eAnmFailureModeQuiet,
	eAnmFailureModeVerbose,
	eAnmFailureModeHalt,
};

#define ANM_DEFAULT_FAILUREMODE eAnmFailureModeQuiet

typedef enum eValueType {
	eValueBoolean,
	eValueBooleanArray,
	eValueColor,
	eValueColorArray,
	eValueDouble,
	eValueDoubleArray,
	eValueFloat,
	eValueFloatArray,
	eValueImage,
	eValueImageArray,
	eValueInteger,
	eValueIntegerArray,
	eValueNode,
	eValueNodeArray,
	eValueRotation,
	eValueRotationArray,
	eValueString,
	eValueStringArray,
	eValueTime,
	eValueTimeArray,
	eValuePoint2,
	eValuePoint2Array,
	eValuePoint3,
	eValuePoint3Array,
	eValueBadType,						// always leave this one for last
} eValueType;

typedef enum eAnmX3DEncoding {
	eAnmX3DNoSceneDefined = 0,
	eAnmX3DScriptedEncoding = 1,
	eAnmX3DAsciiVrmlEncoding = 2,
	eAnmX3DClassicVrmlEncoding = 3,
	eAnmX3DXMLEncoding = 4,
	eAnmX3DBinaryEncoding = 5,
	eAnmX3DBIFSEncoding = 6,
	eAnmX3DLastEncodingValue = 100,
	eAnmX3DBadEncodingValue = -1,
} eAnmX3DEncoding ;


// Poor man's profiling
#define BEGIN_PROFILE_BLOCK(_nm_) \
{ \
	char _buf[1024]; \
	double _curtime, _newtime, _deltatime; \
	_curtime = timeGetTime() * .001; \
	sprintf(_buf, "Begin %s...\n", _nm_); \
	TRACE(_buf);

#define END_PROFILE_BLOCK(_nm_) \
	_newtime = timeGetTime() * .001; \
	_deltatime = _newtime - _curtime; \
	sprintf(_buf, "Done %s, delta = %f sec.\n", _nm_, _deltatime); \
	TRACE(_buf); \
}


// Maximum length of a sprintf "%d" - will this ever overflow?
#define ANMMAXDECIMALLENGTH 64

// Visual C++ fix of operator new

extern void* ::operator new (size_t size);

extern void ::operator delete (void *p);

// N.B.: windows sutff
#define ANMWIN_ENTERCHAR	'\r'
#define ANMWIN_DELETECHAR	'\b'

#ifdef WIN32
#define DLL_EXPORT   __declspec( dllexport )
#define DLL_IMPORT   __declspec( dllimport )
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif // WIN32


#endif // _animadefs_h
