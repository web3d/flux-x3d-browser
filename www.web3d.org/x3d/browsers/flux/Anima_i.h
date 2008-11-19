/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun May 06 19:46:32 2007
 */
/* Compiler settings for C:\DEV\flux634os\Anima.odl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __Anima_i_h__
#define __Anima_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __X3DNode_FWD_DEFINED__
#define __X3DNode_FWD_DEFINED__
typedef interface X3DNode X3DNode;
#endif 	/* __X3DNode_FWD_DEFINED__ */


#ifndef __X3DBrowser_FWD_DEFINED__
#define __X3DBrowser_FWD_DEFINED__
typedef interface X3DBrowser X3DBrowser;
#endif 	/* __X3DBrowser_FWD_DEFINED__ */


#ifndef __X3DField_FWD_DEFINED__
#define __X3DField_FWD_DEFINED__
typedef interface X3DField X3DField;
#endif 	/* __X3DField_FWD_DEFINED__ */


#ifndef __X3DEventOutObserver_FWD_DEFINED__
#define __X3DEventOutObserver_FWD_DEFINED__
typedef interface X3DEventOutObserver X3DEventOutObserver;
#endif 	/* __X3DEventOutObserver_FWD_DEFINED__ */


#ifndef __X3DExecutionContext_FWD_DEFINED__
#define __X3DExecutionContext_FWD_DEFINED__
typedef interface X3DExecutionContext X3DExecutionContext;
#endif 	/* __X3DExecutionContext_FWD_DEFINED__ */


#ifndef __X3DScene_FWD_DEFINED__
#define __X3DScene_FWD_DEFINED__
typedef interface X3DScene X3DScene;
#endif 	/* __X3DScene_FWD_DEFINED__ */


#ifndef __X3DBrowserListener_FWD_DEFINED__
#define __X3DBrowserListener_FWD_DEFINED__
typedef interface X3DBrowserListener X3DBrowserListener;
#endif 	/* __X3DBrowserListener_FWD_DEFINED__ */


#ifndef __X3DFieldEventListener_FWD_DEFINED__
#define __X3DFieldEventListener_FWD_DEFINED__
typedef interface X3DFieldEventListener X3DFieldEventListener;
#endif 	/* __X3DFieldEventListener_FWD_DEFINED__ */


#ifndef __X3DRoute_FWD_DEFINED__
#define __X3DRoute_FWD_DEFINED__
typedef interface X3DRoute X3DRoute;
#endif 	/* __X3DRoute_FWD_DEFINED__ */


#ifndef __X3DSFBool_FWD_DEFINED__
#define __X3DSFBool_FWD_DEFINED__
typedef interface X3DSFBool X3DSFBool;
#endif 	/* __X3DSFBool_FWD_DEFINED__ */


#ifndef __X3DSFColor_FWD_DEFINED__
#define __X3DSFColor_FWD_DEFINED__
typedef interface X3DSFColor X3DSFColor;
#endif 	/* __X3DSFColor_FWD_DEFINED__ */


#ifndef __X3DSFFloat_FWD_DEFINED__
#define __X3DSFFloat_FWD_DEFINED__
typedef interface X3DSFFloat X3DSFFloat;
#endif 	/* __X3DSFFloat_FWD_DEFINED__ */


#ifndef __X3DSFInt32_FWD_DEFINED__
#define __X3DSFInt32_FWD_DEFINED__
typedef interface X3DSFInt32 X3DSFInt32;
#endif 	/* __X3DSFInt32_FWD_DEFINED__ */


#ifndef __X3DSFNode_FWD_DEFINED__
#define __X3DSFNode_FWD_DEFINED__
typedef interface X3DSFNode X3DSFNode;
#endif 	/* __X3DSFNode_FWD_DEFINED__ */


#ifndef __X3DSFRotation_FWD_DEFINED__
#define __X3DSFRotation_FWD_DEFINED__
typedef interface X3DSFRotation X3DSFRotation;
#endif 	/* __X3DSFRotation_FWD_DEFINED__ */


#ifndef __X3DSFString_FWD_DEFINED__
#define __X3DSFString_FWD_DEFINED__
typedef interface X3DSFString X3DSFString;
#endif 	/* __X3DSFString_FWD_DEFINED__ */


#ifndef __X3DSFVec2f_FWD_DEFINED__
#define __X3DSFVec2f_FWD_DEFINED__
typedef interface X3DSFVec2f X3DSFVec2f;
#endif 	/* __X3DSFVec2f_FWD_DEFINED__ */


#ifndef __X3DSFVec3f_FWD_DEFINED__
#define __X3DSFVec3f_FWD_DEFINED__
typedef interface X3DSFVec3f X3DSFVec3f;
#endif 	/* __X3DSFVec3f_FWD_DEFINED__ */


#ifndef __X3DNodeArray_FWD_DEFINED__
#define __X3DNodeArray_FWD_DEFINED__
typedef interface X3DNodeArray X3DNodeArray;
#endif 	/* __X3DNodeArray_FWD_DEFINED__ */


#ifndef __X3DFieldDefinition_FWD_DEFINED__
#define __X3DFieldDefinition_FWD_DEFINED__
typedef interface X3DFieldDefinition X3DFieldDefinition;
#endif 	/* __X3DFieldDefinition_FWD_DEFINED__ */


#ifndef __X3DFieldDefinitionArray_FWD_DEFINED__
#define __X3DFieldDefinitionArray_FWD_DEFINED__
typedef interface X3DFieldDefinitionArray X3DFieldDefinitionArray;
#endif 	/* __X3DFieldDefinitionArray_FWD_DEFINED__ */


#ifndef __X3DProtoDeclaration_FWD_DEFINED__
#define __X3DProtoDeclaration_FWD_DEFINED__
typedef interface X3DProtoDeclaration X3DProtoDeclaration;
#endif 	/* __X3DProtoDeclaration_FWD_DEFINED__ */


#ifndef __X3DProtoDeclarationArray_FWD_DEFINED__
#define __X3DProtoDeclarationArray_FWD_DEFINED__
typedef interface X3DProtoDeclarationArray X3DProtoDeclarationArray;
#endif 	/* __X3DProtoDeclarationArray_FWD_DEFINED__ */


#ifndef ___DAnima_FWD_DEFINED__
#define ___DAnima_FWD_DEFINED__
typedef interface _DAnima _DAnima;
#endif 	/* ___DAnima_FWD_DEFINED__ */


#ifndef ___DAnimaEvents_FWD_DEFINED__
#define ___DAnimaEvents_FWD_DEFINED__
typedef interface _DAnimaEvents _DAnimaEvents;
#endif 	/* ___DAnimaEvents_FWD_DEFINED__ */


#ifndef __X3DMField_FWD_DEFINED__
#define __X3DMField_FWD_DEFINED__
typedef interface X3DMField X3DMField;
#endif 	/* __X3DMField_FWD_DEFINED__ */


#ifndef __X3DMFColor_FWD_DEFINED__
#define __X3DMFColor_FWD_DEFINED__
typedef interface X3DMFColor X3DMFColor;
#endif 	/* __X3DMFColor_FWD_DEFINED__ */


#ifndef __X3DMFNode_FWD_DEFINED__
#define __X3DMFNode_FWD_DEFINED__
typedef interface X3DMFNode X3DMFNode;
#endif 	/* __X3DMFNode_FWD_DEFINED__ */


#ifndef __X3DSFTime_FWD_DEFINED__
#define __X3DSFTime_FWD_DEFINED__
typedef interface X3DSFTime X3DSFTime;
#endif 	/* __X3DSFTime_FWD_DEFINED__ */


#ifndef __Anima_FWD_DEFINED__
#define __Anima_FWD_DEFINED__

#ifdef __cplusplus
typedef class Anima Anima;
#else
typedef struct Anima Anima;
#endif /* __cplusplus */

#endif 	/* __Anima_FWD_DEFINED__ */


#ifndef __IDashboard1_FWD_DEFINED__
#define __IDashboard1_FWD_DEFINED__
typedef interface IDashboard1 IDashboard1;
#endif 	/* __IDashboard1_FWD_DEFINED__ */


#ifndef __Dashboard_FWD_DEFINED__
#define __Dashboard_FWD_DEFINED__

#ifdef __cplusplus
typedef class Dashboard Dashboard;
#else
typedef struct Dashboard Dashboard;
#endif /* __cplusplus */

#endif 	/* __Dashboard_FWD_DEFINED__ */


#ifndef __CoX3DNode_FWD_DEFINED__
#define __CoX3DNode_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DNode CoX3DNode;
#else
typedef struct CoX3DNode CoX3DNode;
#endif /* __cplusplus */

#endif 	/* __CoX3DNode_FWD_DEFINED__ */


#ifndef __CoX3DField_FWD_DEFINED__
#define __CoX3DField_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DField CoX3DField;
#else
typedef struct CoX3DField CoX3DField;
#endif /* __cplusplus */

#endif 	/* __CoX3DField_FWD_DEFINED__ */


#ifndef __CoX3DBrowser_FWD_DEFINED__
#define __CoX3DBrowser_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DBrowser CoX3DBrowser;
#else
typedef struct CoX3DBrowser CoX3DBrowser;
#endif /* __cplusplus */

#endif 	/* __CoX3DBrowser_FWD_DEFINED__ */


#ifndef __CoX3DSFBool_FWD_DEFINED__
#define __CoX3DSFBool_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFBool CoX3DSFBool;
#else
typedef struct CoX3DSFBool CoX3DSFBool;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFBool_FWD_DEFINED__ */


#ifndef __CoX3DMField_FWD_DEFINED__
#define __CoX3DMField_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMField CoX3DMField;
#else
typedef struct CoX3DMField CoX3DMField;
#endif /* __cplusplus */

#endif 	/* __CoX3DMField_FWD_DEFINED__ */


#ifndef __CoX3DMFColor_FWD_DEFINED__
#define __CoX3DMFColor_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFColor CoX3DMFColor;
#else
typedef struct CoX3DMFColor CoX3DMFColor;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFColor_FWD_DEFINED__ */


#ifndef __CoX3DMFNode_FWD_DEFINED__
#define __CoX3DMFNode_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFNode CoX3DMFNode;
#else
typedef struct CoX3DMFNode CoX3DMFNode;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFNode_FWD_DEFINED__ */


#ifndef ___X3DSFTimeEvents_FWD_DEFINED__
#define ___X3DSFTimeEvents_FWD_DEFINED__
typedef interface _X3DSFTimeEvents _X3DSFTimeEvents;
#endif 	/* ___X3DSFTimeEvents_FWD_DEFINED__ */


#ifndef __X3DMFFloat_FWD_DEFINED__
#define __X3DMFFloat_FWD_DEFINED__
typedef interface X3DMFFloat X3DMFFloat;
#endif 	/* __X3DMFFloat_FWD_DEFINED__ */


#ifndef __X3DMFInt32_FWD_DEFINED__
#define __X3DMFInt32_FWD_DEFINED__
typedef interface X3DMFInt32 X3DMFInt32;
#endif 	/* __X3DMFInt32_FWD_DEFINED__ */


#ifndef __X3DMFRotation_FWD_DEFINED__
#define __X3DMFRotation_FWD_DEFINED__
typedef interface X3DMFRotation X3DMFRotation;
#endif 	/* __X3DMFRotation_FWD_DEFINED__ */


#ifndef __X3DMFString_FWD_DEFINED__
#define __X3DMFString_FWD_DEFINED__
typedef interface X3DMFString X3DMFString;
#endif 	/* __X3DMFString_FWD_DEFINED__ */


#ifndef __X3DMFVec2f_FWD_DEFINED__
#define __X3DMFVec2f_FWD_DEFINED__
typedef interface X3DMFVec2f X3DMFVec2f;
#endif 	/* __X3DMFVec2f_FWD_DEFINED__ */


#ifndef __X3DMFVec3f_FWD_DEFINED__
#define __X3DMFVec3f_FWD_DEFINED__
typedef interface X3DMFVec3f X3DMFVec3f;
#endif 	/* __X3DMFVec3f_FWD_DEFINED__ */


#ifndef __X3DSFDouble_FWD_DEFINED__
#define __X3DSFDouble_FWD_DEFINED__
typedef interface X3DSFDouble X3DSFDouble;
#endif 	/* __X3DSFDouble_FWD_DEFINED__ */


#ifndef __IFluxPlayer_FWD_DEFINED__
#define __IFluxPlayer_FWD_DEFINED__
typedef interface IFluxPlayer IFluxPlayer;
#endif 	/* __IFluxPlayer_FWD_DEFINED__ */


#ifndef __X3DExternalBrowser_FWD_DEFINED__
#define __X3DExternalBrowser_FWD_DEFINED__
typedef interface X3DExternalBrowser X3DExternalBrowser;
#endif 	/* __X3DExternalBrowser_FWD_DEFINED__ */


#ifndef __CoX3DSFTime_FWD_DEFINED__
#define __CoX3DSFTime_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFTime CoX3DSFTime;
#else
typedef struct CoX3DSFTime CoX3DSFTime;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFTime_FWD_DEFINED__ */


#ifndef __CoX3DEventOutObserver_FWD_DEFINED__
#define __CoX3DEventOutObserver_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DEventOutObserver CoX3DEventOutObserver;
#else
typedef struct CoX3DEventOutObserver CoX3DEventOutObserver;
#endif /* __cplusplus */

#endif 	/* __CoX3DEventOutObserver_FWD_DEFINED__ */


#ifndef __CoX3DSFColor_FWD_DEFINED__
#define __CoX3DSFColor_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFColor CoX3DSFColor;
#else
typedef struct CoX3DSFColor CoX3DSFColor;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFColor_FWD_DEFINED__ */


#ifndef __CoX3DSFFloat_FWD_DEFINED__
#define __CoX3DSFFloat_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFFloat CoX3DSFFloat;
#else
typedef struct CoX3DSFFloat CoX3DSFFloat;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFFloat_FWD_DEFINED__ */


#ifndef __CoX3DSFInt32_FWD_DEFINED__
#define __CoX3DSFInt32_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFInt32 CoX3DSFInt32;
#else
typedef struct CoX3DSFInt32 CoX3DSFInt32;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFInt32_FWD_DEFINED__ */


#ifndef __CoX3DSFNode_FWD_DEFINED__
#define __CoX3DSFNode_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFNode CoX3DSFNode;
#else
typedef struct CoX3DSFNode CoX3DSFNode;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFNode_FWD_DEFINED__ */


#ifndef __CoX3DSFRotation_FWD_DEFINED__
#define __CoX3DSFRotation_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFRotation CoX3DSFRotation;
#else
typedef struct CoX3DSFRotation CoX3DSFRotation;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFRotation_FWD_DEFINED__ */


#ifndef __CoX3DSFString_FWD_DEFINED__
#define __CoX3DSFString_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFString CoX3DSFString;
#else
typedef struct CoX3DSFString CoX3DSFString;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFString_FWD_DEFINED__ */


#ifndef __CoX3DSFVec2f_FWD_DEFINED__
#define __CoX3DSFVec2f_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFVec2f CoX3DSFVec2f;
#else
typedef struct CoX3DSFVec2f CoX3DSFVec2f;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFVec2f_FWD_DEFINED__ */


#ifndef __CoX3DSFVec3f_FWD_DEFINED__
#define __CoX3DSFVec3f_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFVec3f CoX3DSFVec3f;
#else
typedef struct CoX3DSFVec3f CoX3DSFVec3f;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFVec3f_FWD_DEFINED__ */


#ifndef __CoX3DMFFloat_FWD_DEFINED__
#define __CoX3DMFFloat_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFFloat CoX3DMFFloat;
#else
typedef struct CoX3DMFFloat CoX3DMFFloat;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFFloat_FWD_DEFINED__ */


#ifndef __CoX3DMFInt32_FWD_DEFINED__
#define __CoX3DMFInt32_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFInt32 CoX3DMFInt32;
#else
typedef struct CoX3DMFInt32 CoX3DMFInt32;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFInt32_FWD_DEFINED__ */


#ifndef __CoX3DMFRotation_FWD_DEFINED__
#define __CoX3DMFRotation_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFRotation CoX3DMFRotation;
#else
typedef struct CoX3DMFRotation CoX3DMFRotation;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFRotation_FWD_DEFINED__ */


#ifndef __CoX3DMFString_FWD_DEFINED__
#define __CoX3DMFString_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFString CoX3DMFString;
#else
typedef struct CoX3DMFString CoX3DMFString;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFString_FWD_DEFINED__ */


#ifndef __CoX3DMFVec2f_FWD_DEFINED__
#define __CoX3DMFVec2f_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFVec2f CoX3DMFVec2f;
#else
typedef struct CoX3DMFVec2f CoX3DMFVec2f;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFVec2f_FWD_DEFINED__ */


#ifndef __CoX3DMFVec3f_FWD_DEFINED__
#define __CoX3DMFVec3f_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DMFVec3f CoX3DMFVec3f;
#else
typedef struct CoX3DMFVec3f CoX3DMFVec3f;
#endif /* __cplusplus */

#endif 	/* __CoX3DMFVec3f_FWD_DEFINED__ */


#ifndef __CoX3DSFDouble_FWD_DEFINED__
#define __CoX3DSFDouble_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DSFDouble CoX3DSFDouble;
#else
typedef struct CoX3DSFDouble CoX3DSFDouble;
#endif /* __cplusplus */

#endif 	/* __CoX3DSFDouble_FWD_DEFINED__ */


#ifndef __FluxPlayer_FWD_DEFINED__
#define __FluxPlayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class FluxPlayer FluxPlayer;
#else
typedef struct FluxPlayer FluxPlayer;
#endif /* __cplusplus */

#endif 	/* __FluxPlayer_FWD_DEFINED__ */


#ifndef __CoX3DExecutionContext_FWD_DEFINED__
#define __CoX3DExecutionContext_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DExecutionContext CoX3DExecutionContext;
#else
typedef struct CoX3DExecutionContext CoX3DExecutionContext;
#endif /* __cplusplus */

#endif 	/* __CoX3DExecutionContext_FWD_DEFINED__ */


#ifndef __CoX3DScene_FWD_DEFINED__
#define __CoX3DScene_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DScene CoX3DScene;
#else
typedef struct CoX3DScene CoX3DScene;
#endif /* __cplusplus */

#endif 	/* __CoX3DScene_FWD_DEFINED__ */


#ifndef __CoX3DBrowserListener_FWD_DEFINED__
#define __CoX3DBrowserListener_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DBrowserListener CoX3DBrowserListener;
#else
typedef struct CoX3DBrowserListener CoX3DBrowserListener;
#endif /* __cplusplus */

#endif 	/* __CoX3DBrowserListener_FWD_DEFINED__ */


#ifndef __CoX3DExternalBrowser_FWD_DEFINED__
#define __CoX3DExternalBrowser_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DExternalBrowser CoX3DExternalBrowser;
#else
typedef struct CoX3DExternalBrowser CoX3DExternalBrowser;
#endif /* __cplusplus */

#endif 	/* __CoX3DExternalBrowser_FWD_DEFINED__ */


#ifndef __CoX3DFieldEventListener_FWD_DEFINED__
#define __CoX3DFieldEventListener_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DFieldEventListener CoX3DFieldEventListener;
#else
typedef struct CoX3DFieldEventListener CoX3DFieldEventListener;
#endif /* __cplusplus */

#endif 	/* __CoX3DFieldEventListener_FWD_DEFINED__ */


#ifndef __CoX3DRoute_FWD_DEFINED__
#define __CoX3DRoute_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DRoute CoX3DRoute;
#else
typedef struct CoX3DRoute CoX3DRoute;
#endif /* __cplusplus */

#endif 	/* __CoX3DRoute_FWD_DEFINED__ */


#ifndef __CoX3DNodeArray_FWD_DEFINED__
#define __CoX3DNodeArray_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DNodeArray CoX3DNodeArray;
#else
typedef struct CoX3DNodeArray CoX3DNodeArray;
#endif /* __cplusplus */

#endif 	/* __CoX3DNodeArray_FWD_DEFINED__ */


#ifndef __CoX3DFieldDefinition_FWD_DEFINED__
#define __CoX3DFieldDefinition_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DFieldDefinition CoX3DFieldDefinition;
#else
typedef struct CoX3DFieldDefinition CoX3DFieldDefinition;
#endif /* __cplusplus */

#endif 	/* __CoX3DFieldDefinition_FWD_DEFINED__ */


#ifndef __CoX3DFieldDefinitionArray_FWD_DEFINED__
#define __CoX3DFieldDefinitionArray_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DFieldDefinitionArray CoX3DFieldDefinitionArray;
#else
typedef struct CoX3DFieldDefinitionArray CoX3DFieldDefinitionArray;
#endif /* __cplusplus */

#endif 	/* __CoX3DFieldDefinitionArray_FWD_DEFINED__ */


#ifndef __CoX3DProtoDeclaration_FWD_DEFINED__
#define __CoX3DProtoDeclaration_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DProtoDeclaration CoX3DProtoDeclaration;
#else
typedef struct CoX3DProtoDeclaration CoX3DProtoDeclaration;
#endif /* __cplusplus */

#endif 	/* __CoX3DProtoDeclaration_FWD_DEFINED__ */


#ifndef __CoX3DProtoDeclarationArray_FWD_DEFINED__
#define __CoX3DProtoDeclarationArray_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoX3DProtoDeclarationArray CoX3DProtoDeclarationArray;
#else
typedef struct CoX3DProtoDeclarationArray CoX3DProtoDeclarationArray;
#endif /* __cplusplus */

#endif 	/* __CoX3DProtoDeclarationArray_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __ANIMALib_LIBRARY_DEFINED__
#define __ANIMALib_LIBRARY_DEFINED__

/* library ANIMALib */
/* [control][helpstring][helpfile][version][uuid] */ 
























typedef 
enum X3DFieldAccess
    {	X3DInitializeOnly	= 0,
	X3DInputOnly	= X3DInitializeOnly + 1,
	X3DOutputOnly	= X3DInputOnly + 1,
	X3DInputOutput	= X3DOutputOnly + 1
    }	X3DFieldAccess;

typedef 
enum X3DFieldType
    {	X3DUnknownFieldType	= 0,
	X3DSFBoolType	= X3DUnknownFieldType + 1,
	X3DMFBoolType	= X3DSFBoolType + 1,
	X3DSFColorType	= X3DMFBoolType + 1,
	X3DMFColorType	= X3DSFColorType + 1,
	X3DSFColorRGBAType	= X3DMFColorType + 1,
	X3DMFColorRGBAType	= X3DSFColorRGBAType + 1,
	X3DSFDoubleType	= X3DMFColorRGBAType + 1,
	X3DMFDoubleType	= X3DSFDoubleType + 1,
	X3DSFFloatType	= X3DMFDoubleType + 1,
	X3DMFFloatType	= X3DSFFloatType + 1,
	X3DSFImageType	= X3DMFFloatType + 1,
	X3DMFImageType	= X3DSFImageType + 1,
	X3DSFInt32Type	= X3DMFImageType + 1,
	X3DMFInt32Type	= X3DSFInt32Type + 1,
	X3DSFNodeType	= X3DMFInt32Type + 1,
	X3DMFNodeType	= X3DSFNodeType + 1,
	X3DSFRotationType	= X3DMFNodeType + 1,
	X3DMFRotationType	= X3DSFRotationType + 1,
	X3DSFStringType	= X3DMFRotationType + 1,
	X3DMFStringType	= X3DSFStringType + 1,
	X3DSFTimeType	= X3DMFStringType + 1,
	X3DMFTimeType	= X3DSFTimeType + 1,
	X3DSFVec2fType	= X3DMFTimeType + 1,
	X3DMFVec2fType	= X3DSFVec2fType + 1,
	X3DSFVec2dType	= X3DMFVec2fType + 1,
	X3DMFVec2dType	= X3DSFVec2dType + 1,
	X3DSFVec3fType	= X3DMFVec2dType + 1,
	X3DMFVec3fType	= X3DSFVec3fType + 1,
	X3DSFVec3dType	= X3DMFVec3fType + 1,
	X3DMFVec3dType	= X3DSFVec3dType + 1
    }	X3DFieldType;

typedef 
enum X3DEncodingType
    {	X3DNO_SCENE	= 0,
	X3DSCRIPTED_ENCODING	= 1,
	X3DASCII_ENCODING	= 2,
	X3DCLASSIC_VRML_ENCODING	= 3,
	X3DXML_ENCODING	= 4,
	X3DBINARY_ENCODING	= 5,
	X3DBIFS_ENCODING	= 6,
	X3DLAST_STD_ENCODING	= 100
    }	X3DEncodingType;

typedef 
enum X3DBrowserEvent
    {	X3DBROWSER_INITIALIZED	= 0,
	X3DBROWSER_SHUTDOWN	= 1,
	X3DBROWSER_CONNECTIONERROR	= 2,
	X3DBROWSER_URLERROR	= 3
    }	X3DBrowserEvent;


EXTERN_C const IID LIBID_ANIMALib;

#ifndef __X3DNode_INTERFACE_DEFINED__
#define __X3DNode_INTERFACE_DEFINED__

/* interface X3DNode */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("33A5C345-9C42-430E-A208-3FE942999486")
    X3DNode : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setNativeNode( 
            /* [in] */ void __RPC_FAR *pNativeNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getNativeNode( 
            /* [retval][out] */ void __RPC_FAR *ppNativeNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getTypeName( 
            /* [retval][out] */ BSTR __RPC_FAR *typenm) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getField( 
            /* [in] */ BSTR name,
            /* [retval][out] */ X3DField __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setContext( 
            /* [in] */ X3DExecutionContext __RPC_FAR *context) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getContext( 
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getName( 
            /* [retval][out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getNumFields( 
            /* [retval][out] */ int __RPC_FAR *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getFieldInfo( 
            /* [in] */ int fieldIndex,
            /* [out] */ X3DFieldAccess __RPC_FAR *accessType,
            /* [out] */ X3DFieldType __RPC_FAR *fieldType,
            /* [out] */ BSTR __RPC_FAR *fieldName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getFieldDefinitions( 
            /* [retval][out] */ X3DFieldDefinitionArray __RPC_FAR *__RPC_FAR *fieldDefinitions) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setNativeNode )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pNativeNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNativeNode )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ void __RPC_FAR *ppNativeNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getTypeName )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *typenm);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getField )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ X3DField __RPC_FAR *__RPC_FAR *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setContext )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ X3DExecutionContext __RPC_FAR *context);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getContext )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNumFields )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getFieldInfo )( 
            X3DNode __RPC_FAR * This,
            /* [in] */ int fieldIndex,
            /* [out] */ X3DFieldAccess __RPC_FAR *accessType,
            /* [out] */ X3DFieldType __RPC_FAR *fieldType,
            /* [out] */ BSTR __RPC_FAR *fieldName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Text )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getFieldDefinitions )( 
            X3DNode __RPC_FAR * This,
            /* [retval][out] */ X3DFieldDefinitionArray __RPC_FAR *__RPC_FAR *fieldDefinitions);
        
        END_INTERFACE
    } X3DNodeVtbl;

    interface X3DNode
    {
        CONST_VTBL struct X3DNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DNode_setNativeNode(This,pNativeNode)	\
    (This)->lpVtbl -> setNativeNode(This,pNativeNode)

#define X3DNode_getNativeNode(This,ppNativeNode)	\
    (This)->lpVtbl -> getNativeNode(This,ppNativeNode)

#define X3DNode_getTypeName(This,typenm)	\
    (This)->lpVtbl -> getTypeName(This,typenm)

#define X3DNode_getField(This,name,result)	\
    (This)->lpVtbl -> getField(This,name,result)

#define X3DNode_setContext(This,context)	\
    (This)->lpVtbl -> setContext(This,context)

#define X3DNode_getContext(This,result)	\
    (This)->lpVtbl -> getContext(This,result)

#define X3DNode_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DNode_getNumFields(This,result)	\
    (This)->lpVtbl -> getNumFields(This,result)

#define X3DNode_getFieldInfo(This,fieldIndex,accessType,fieldType,fieldName)	\
    (This)->lpVtbl -> getFieldInfo(This,fieldIndex,accessType,fieldType,fieldName)

#define X3DNode_get_Text(This,pVal)	\
    (This)->lpVtbl -> get_Text(This,pVal)

#define X3DNode_getFieldDefinitions(This,fieldDefinitions)	\
    (This)->lpVtbl -> getFieldDefinitions(This,fieldDefinitions)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_setNativeNode_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *pNativeNode);


void __RPC_STUB X3DNode_setNativeNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getNativeNode_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ void __RPC_FAR *ppNativeNode);


void __RPC_STUB X3DNode_getNativeNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getTypeName_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *typenm);


void __RPC_STUB X3DNode_getTypeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getField_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ X3DField __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB X3DNode_getField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_setContext_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [in] */ X3DExecutionContext __RPC_FAR *context);


void __RPC_STUB X3DNode_setContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getContext_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB X3DNode_getContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getName_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *name);


void __RPC_STUB X3DNode_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getNumFields_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *result);


void __RPC_STUB X3DNode_getNumFields_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getFieldInfo_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [in] */ int fieldIndex,
    /* [out] */ X3DFieldAccess __RPC_FAR *accessType,
    /* [out] */ X3DFieldType __RPC_FAR *fieldType,
    /* [out] */ BSTR __RPC_FAR *fieldName);


void __RPC_STUB X3DNode_getFieldInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DNode_get_Text_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB X3DNode_get_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNode_getFieldDefinitions_Proxy( 
    X3DNode __RPC_FAR * This,
    /* [retval][out] */ X3DFieldDefinitionArray __RPC_FAR *__RPC_FAR *fieldDefinitions);


void __RPC_STUB X3DNode_getFieldDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DNode_INTERFACE_DEFINED__ */


#ifndef __X3DBrowser_INTERFACE_DEFINED__
#define __X3DBrowser_INTERFACE_DEFINED__

/* interface X3DBrowser */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DBrowser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1DDF9D8D-6931-4B5D-A2F6-8A341A1AD0CF")
    X3DBrowser : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getName( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getVersion( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getCurrentSpeed( 
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getCurrentFrameRate( 
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getWorldURL( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE replaceWorld( 
            /* [in] */ X3DScene __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE loadURL( 
            /* [in] */ int nUrls,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int nParams,
            /* [size_is][in] */ BSTR __RPC_FAR *params) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setDescription( 
            /* [in] */ BSTR description) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE createX3DFromString( 
            /* [in] */ BSTR x3dSource,
            /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE createX3DFromURL( 
            /* [in] */ BSTR url,
            /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setClosure( 
            /* [in] */ void __RPC_FAR *closure) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getClosure( 
            /* [retval][out] */ void __RPC_FAR *closure) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE print( 
            /* [in] */ BSTR str) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE loadUrlS( 
            BSTR url) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getExecutionContext( 
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *ppContext) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE addBrowserListener( 
            /* [in] */ IDispatch __RPC_FAR *pListener) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE removeBrowserListener( 
            /* [in] */ IDispatch __RPC_FAR *pListener) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE firstViewpoint( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lastViewpoint( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE nextViewpoint( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE previousViewpoint( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_viewpoints( 
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DBrowser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DBrowser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DBrowser __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getVersion )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentSpeed )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentFrameRate )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWorldURL )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *replaceWorld )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ X3DScene __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadURL )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ int nUrls,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int nParams,
            /* [size_is][in] */ BSTR __RPC_FAR *params);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setDescription )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ BSTR description);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createX3DFromString )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ BSTR x3dSource,
            /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createX3DFromURL )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ BSTR url,
            /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setClosure )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getClosure )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *print )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ BSTR str);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadUrlS )( 
            X3DBrowser __RPC_FAR * This,
            BSTR url);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExecutionContext )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *ppContext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBrowserListener )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pListener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBrowserListener )( 
            X3DBrowser __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pListener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *firstViewpoint )( 
            X3DBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lastViewpoint )( 
            X3DBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *nextViewpoint )( 
            X3DBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *previousViewpoint )( 
            X3DBrowser __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_viewpoints )( 
            X3DBrowser __RPC_FAR * This,
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } X3DBrowserVtbl;

    interface X3DBrowser
    {
        CONST_VTBL struct X3DBrowserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DBrowser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DBrowser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DBrowser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DBrowser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DBrowser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DBrowser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DBrowser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DBrowser_getName(This,value)	\
    (This)->lpVtbl -> getName(This,value)

#define X3DBrowser_getVersion(This,value)	\
    (This)->lpVtbl -> getVersion(This,value)

#define X3DBrowser_getCurrentSpeed(This,value)	\
    (This)->lpVtbl -> getCurrentSpeed(This,value)

#define X3DBrowser_getCurrentFrameRate(This,value)	\
    (This)->lpVtbl -> getCurrentFrameRate(This,value)

#define X3DBrowser_getWorldURL(This,value)	\
    (This)->lpVtbl -> getWorldURL(This,value)

#define X3DBrowser_replaceWorld(This,value)	\
    (This)->lpVtbl -> replaceWorld(This,value)

#define X3DBrowser_loadURL(This,nUrls,url,nParams,params)	\
    (This)->lpVtbl -> loadURL(This,nUrls,url,nParams,params)

#define X3DBrowser_setDescription(This,description)	\
    (This)->lpVtbl -> setDescription(This,description)

#define X3DBrowser_createX3DFromString(This,x3dSource,scenereturn)	\
    (This)->lpVtbl -> createX3DFromString(This,x3dSource,scenereturn)

#define X3DBrowser_createX3DFromURL(This,url,scenereturn)	\
    (This)->lpVtbl -> createX3DFromURL(This,url,scenereturn)

#define X3DBrowser_setClosure(This,closure)	\
    (This)->lpVtbl -> setClosure(This,closure)

#define X3DBrowser_getClosure(This,closure)	\
    (This)->lpVtbl -> getClosure(This,closure)

#define X3DBrowser_print(This,str)	\
    (This)->lpVtbl -> print(This,str)

#define X3DBrowser_loadUrlS(This,url)	\
    (This)->lpVtbl -> loadUrlS(This,url)

#define X3DBrowser_getExecutionContext(This,ppContext)	\
    (This)->lpVtbl -> getExecutionContext(This,ppContext)

#define X3DBrowser_addBrowserListener(This,pListener)	\
    (This)->lpVtbl -> addBrowserListener(This,pListener)

#define X3DBrowser_removeBrowserListener(This,pListener)	\
    (This)->lpVtbl -> removeBrowserListener(This,pListener)

#define X3DBrowser_firstViewpoint(This)	\
    (This)->lpVtbl -> firstViewpoint(This)

#define X3DBrowser_lastViewpoint(This)	\
    (This)->lpVtbl -> lastViewpoint(This)

#define X3DBrowser_nextViewpoint(This)	\
    (This)->lpVtbl -> nextViewpoint(This)

#define X3DBrowser_previousViewpoint(This)	\
    (This)->lpVtbl -> previousViewpoint(This)

#define X3DBrowser_get_viewpoints(This,pVal)	\
    (This)->lpVtbl -> get_viewpoints(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getName_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DBrowser_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getVersion_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DBrowser_getVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getCurrentSpeed_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB X3DBrowser_getCurrentSpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getCurrentFrameRate_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB X3DBrowser_getCurrentFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getWorldURL_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DBrowser_getWorldURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_replaceWorld_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ X3DScene __RPC_FAR *value);


void __RPC_STUB X3DBrowser_replaceWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_loadURL_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ int nUrls,
    /* [size_is][in] */ BSTR __RPC_FAR *url,
    /* [in] */ int nParams,
    /* [size_is][in] */ BSTR __RPC_FAR *params);


void __RPC_STUB X3DBrowser_loadURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_setDescription_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ BSTR description);


void __RPC_STUB X3DBrowser_setDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_createX3DFromString_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ BSTR x3dSource,
    /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn);


void __RPC_STUB X3DBrowser_createX3DFromString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_createX3DFromURL_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ BSTR url,
    /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn);


void __RPC_STUB X3DBrowser_createX3DFromURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_setClosure_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *closure);


void __RPC_STUB X3DBrowser_setClosure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getClosure_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ void __RPC_FAR *closure);


void __RPC_STUB X3DBrowser_getClosure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_print_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ BSTR str);


void __RPC_STUB X3DBrowser_print_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_loadUrlS_Proxy( 
    X3DBrowser __RPC_FAR * This,
    BSTR url);


void __RPC_STUB X3DBrowser_loadUrlS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_getExecutionContext_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *ppContext);


void __RPC_STUB X3DBrowser_getExecutionContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_addBrowserListener_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pListener);


void __RPC_STUB X3DBrowser_addBrowserListener_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_removeBrowserListener_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pListener);


void __RPC_STUB X3DBrowser_removeBrowserListener_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_firstViewpoint_Proxy( 
    X3DBrowser __RPC_FAR * This);


void __RPC_STUB X3DBrowser_firstViewpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_lastViewpoint_Proxy( 
    X3DBrowser __RPC_FAR * This);


void __RPC_STUB X3DBrowser_lastViewpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_nextViewpoint_Proxy( 
    X3DBrowser __RPC_FAR * This);


void __RPC_STUB X3DBrowser_nextViewpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowser_previousViewpoint_Proxy( 
    X3DBrowser __RPC_FAR * This);


void __RPC_STUB X3DBrowser_previousViewpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DBrowser_get_viewpoints_Proxy( 
    X3DBrowser __RPC_FAR * This,
    /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB X3DBrowser_get_viewpoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DBrowser_INTERFACE_DEFINED__ */


#ifndef __X3DField_INTERFACE_DEFINED__
#define __X3DField_INTERFACE_DEFINED__

/* interface X3DField */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DField;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77590C91-073D-428E-8940-7CD14B1897C8")
    X3DField : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE attach( 
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE addFieldEventListener( 
            /* [in] */ IDispatch __RPC_FAR *listener) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE removeFieldEventListener( 
            /* [in] */ IDispatch __RPC_FAR *listener) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAccessType( 
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getType( 
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getName( 
            /* [retval][out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE dispose( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DFieldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DField __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DField __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DField __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DField __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DField __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DField __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DField __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DField __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DField __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DField __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DField __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DField __RPC_FAR * This);
        
        END_INTERFACE
    } X3DFieldVtbl;

    interface X3DField
    {
        CONST_VTBL struct X3DFieldVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DField_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DField_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DField_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DField_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DField_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DField_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DField_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DField_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DField_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DField_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DField_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DField_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DField_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DField_dispose(This)	\
    (This)->lpVtbl -> dispose(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_attach_Proxy( 
    X3DField __RPC_FAR * This,
    X3DExecutionContext __RPC_FAR *context,
    void __RPC_FAR *pNode,
    void __RPC_FAR *pField,
    void __RPC_FAR *mid,
    long valueType);


void __RPC_STUB X3DField_attach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_addFieldEventListener_Proxy( 
    X3DField __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *listener);


void __RPC_STUB X3DField_addFieldEventListener_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_removeFieldEventListener_Proxy( 
    X3DField __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *listener);


void __RPC_STUB X3DField_removeFieldEventListener_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_getAccessType_Proxy( 
    X3DField __RPC_FAR * This,
    /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);


void __RPC_STUB X3DField_getAccessType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_getType_Proxy( 
    X3DField __RPC_FAR * This,
    /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);


void __RPC_STUB X3DField_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_getName_Proxy( 
    X3DField __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *name);


void __RPC_STUB X3DField_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DField_dispose_Proxy( 
    X3DField __RPC_FAR * This);


void __RPC_STUB X3DField_dispose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DField_INTERFACE_DEFINED__ */


#ifndef __X3DEventOutObserver_INTERFACE_DEFINED__
#define __X3DEventOutObserver_INTERFACE_DEFINED__

/* interface X3DEventOutObserver */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DEventOutObserver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3497632D-0CA7-4EC8-BCDA-09564868B9F2")
    X3DEventOutObserver : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE callbackFunction( 
            /* [in] */ X3DField __RPC_FAR *value,
            /* [in] */ double timeStamp,
            /* [in] */ IDispatch __RPC_FAR *userData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DEventOutObserverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DEventOutObserver __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DEventOutObserver __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DEventOutObserver __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DEventOutObserver __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DEventOutObserver __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DEventOutObserver __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DEventOutObserver __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *callbackFunction )( 
            X3DEventOutObserver __RPC_FAR * This,
            /* [in] */ X3DField __RPC_FAR *value,
            /* [in] */ double timeStamp,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        END_INTERFACE
    } X3DEventOutObserverVtbl;

    interface X3DEventOutObserver
    {
        CONST_VTBL struct X3DEventOutObserverVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DEventOutObserver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DEventOutObserver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DEventOutObserver_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DEventOutObserver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DEventOutObserver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DEventOutObserver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DEventOutObserver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DEventOutObserver_callbackFunction(This,value,timeStamp,userData)	\
    (This)->lpVtbl -> callbackFunction(This,value,timeStamp,userData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DEventOutObserver_callbackFunction_Proxy( 
    X3DEventOutObserver __RPC_FAR * This,
    /* [in] */ X3DField __RPC_FAR *value,
    /* [in] */ double timeStamp,
    /* [in] */ IDispatch __RPC_FAR *userData);


void __RPC_STUB X3DEventOutObserver_callbackFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DEventOutObserver_INTERFACE_DEFINED__ */


#ifndef __X3DExecutionContext_INTERFACE_DEFINED__
#define __X3DExecutionContext_INTERFACE_DEFINED__

/* interface X3DExecutionContext */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DExecutionContext;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4B456961-5D8C-4E07-8131-848A23753E04")
    X3DExecutionContext : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setClosure( 
            /* [in] */ void __RPC_FAR *closure) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getClosure( 
            /* [in] */ void __RPC_FAR *__RPC_FAR *closure) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getSpecificationVersion( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getEncoding( 
            /* [retval][out] */ X3DEncodingType __RPC_FAR *encodingType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getNode( 
            /* [in] */ BSTR name,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE createNode( 
            BSTR type,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *node) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE addRoute( 
            /* [in] */ X3DNode __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ X3DNode __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn,
            /* [retval][out] */ X3DRoute __RPC_FAR *__RPC_FAR *ppRoute) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE deleteRoute( 
            /* [in] */ X3DRoute __RPC_FAR *pRoute) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getRootNodes( 
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *rootnodes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getProtoDeclarations( 
            /* [retval][out] */ X3DProtoDeclarationArray __RPC_FAR *__RPC_FAR *protodeclarations) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DExecutionContextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DExecutionContext __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DExecutionContext __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setClosure )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getClosure )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *__RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSpecificationVersion )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getEncoding )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [retval][out] */ X3DEncodingType __RPC_FAR *encodingType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNode )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createNode )( 
            X3DExecutionContext __RPC_FAR * This,
            BSTR type,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *node);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addRoute )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ X3DNode __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ X3DNode __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn,
            /* [retval][out] */ X3DRoute __RPC_FAR *__RPC_FAR *ppRoute);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *deleteRoute )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [in] */ X3DRoute __RPC_FAR *pRoute);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getRootNodes )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *rootnodes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getProtoDeclarations )( 
            X3DExecutionContext __RPC_FAR * This,
            /* [retval][out] */ X3DProtoDeclarationArray __RPC_FAR *__RPC_FAR *protodeclarations);
        
        END_INTERFACE
    } X3DExecutionContextVtbl;

    interface X3DExecutionContext
    {
        CONST_VTBL struct X3DExecutionContextVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DExecutionContext_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DExecutionContext_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DExecutionContext_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DExecutionContext_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DExecutionContext_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DExecutionContext_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DExecutionContext_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DExecutionContext_setClosure(This,closure)	\
    (This)->lpVtbl -> setClosure(This,closure)

#define X3DExecutionContext_getClosure(This,closure)	\
    (This)->lpVtbl -> getClosure(This,closure)

#define X3DExecutionContext_getSpecificationVersion(This,value)	\
    (This)->lpVtbl -> getSpecificationVersion(This,value)

#define X3DExecutionContext_getEncoding(This,encodingType)	\
    (This)->lpVtbl -> getEncoding(This,encodingType)

#define X3DExecutionContext_getNode(This,name,value)	\
    (This)->lpVtbl -> getNode(This,name,value)

#define X3DExecutionContext_createNode(This,type,node)	\
    (This)->lpVtbl -> createNode(This,type,node)

#define X3DExecutionContext_addRoute(This,fromNode,fromEventOut,toNode,toEventIn,ppRoute)	\
    (This)->lpVtbl -> addRoute(This,fromNode,fromEventOut,toNode,toEventIn,ppRoute)

#define X3DExecutionContext_deleteRoute(This,pRoute)	\
    (This)->lpVtbl -> deleteRoute(This,pRoute)

#define X3DExecutionContext_getRootNodes(This,rootnodes)	\
    (This)->lpVtbl -> getRootNodes(This,rootnodes)

#define X3DExecutionContext_getProtoDeclarations(This,protodeclarations)	\
    (This)->lpVtbl -> getProtoDeclarations(This,protodeclarations)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_setClosure_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *closure);


void __RPC_STUB X3DExecutionContext_setClosure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_getClosure_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *__RPC_FAR *closure);


void __RPC_STUB X3DExecutionContext_getClosure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_getSpecificationVersion_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DExecutionContext_getSpecificationVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_getEncoding_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [retval][out] */ X3DEncodingType __RPC_FAR *encodingType);


void __RPC_STUB X3DExecutionContext_getEncoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_getNode_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DExecutionContext_getNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_createNode_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    BSTR type,
    /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *node);


void __RPC_STUB X3DExecutionContext_createNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_addRoute_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [in] */ X3DNode __RPC_FAR *fromNode,
    /* [in] */ BSTR fromEventOut,
    /* [in] */ X3DNode __RPC_FAR *toNode,
    /* [in] */ BSTR toEventIn,
    /* [retval][out] */ X3DRoute __RPC_FAR *__RPC_FAR *ppRoute);


void __RPC_STUB X3DExecutionContext_addRoute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_deleteRoute_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [in] */ X3DRoute __RPC_FAR *pRoute);


void __RPC_STUB X3DExecutionContext_deleteRoute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_getRootNodes_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *rootnodes);


void __RPC_STUB X3DExecutionContext_getRootNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExecutionContext_getProtoDeclarations_Proxy( 
    X3DExecutionContext __RPC_FAR * This,
    /* [retval][out] */ X3DProtoDeclarationArray __RPC_FAR *__RPC_FAR *protodeclarations);


void __RPC_STUB X3DExecutionContext_getProtoDeclarations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DExecutionContext_INTERFACE_DEFINED__ */


#ifndef __X3DScene_INTERFACE_DEFINED__
#define __X3DScene_INTERFACE_DEFINED__

/* interface X3DScene */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DScene;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("006FBCDF-28E2-4F47-A2F4-F493AD909844")
    X3DScene : public X3DExecutionContext
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMetaData( 
            /* [in] */ BSTR key,
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setMetaData( 
            /* [in] */ BSTR key,
            /* [in] */ BSTR value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE addRootNode( 
            /* [in] */ X3DNode __RPC_FAR *node) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE removeRootNode( 
            /* [in] */ X3DNode __RPC_FAR *node) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSceneVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DScene __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DScene __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DScene __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setClosure )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getClosure )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *__RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSpecificationVersion )( 
            X3DScene __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getEncoding )( 
            X3DScene __RPC_FAR * This,
            /* [retval][out] */ X3DEncodingType __RPC_FAR *encodingType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNode )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createNode )( 
            X3DScene __RPC_FAR * This,
            BSTR type,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *node);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addRoute )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ X3DNode __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ X3DNode __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn,
            /* [retval][out] */ X3DRoute __RPC_FAR *__RPC_FAR *ppRoute);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *deleteRoute )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ X3DRoute __RPC_FAR *pRoute);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getRootNodes )( 
            X3DScene __RPC_FAR * This,
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *rootnodes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getProtoDeclarations )( 
            X3DScene __RPC_FAR * This,
            /* [retval][out] */ X3DProtoDeclarationArray __RPC_FAR *__RPC_FAR *protodeclarations);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getMetaData )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ BSTR key,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setMetaData )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ BSTR key,
            /* [in] */ BSTR value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addRootNode )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ X3DNode __RPC_FAR *node);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeRootNode )( 
            X3DScene __RPC_FAR * This,
            /* [in] */ X3DNode __RPC_FAR *node);
        
        END_INTERFACE
    } X3DSceneVtbl;

    interface X3DScene
    {
        CONST_VTBL struct X3DSceneVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DScene_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DScene_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DScene_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DScene_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DScene_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DScene_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DScene_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DScene_setClosure(This,closure)	\
    (This)->lpVtbl -> setClosure(This,closure)

#define X3DScene_getClosure(This,closure)	\
    (This)->lpVtbl -> getClosure(This,closure)

#define X3DScene_getSpecificationVersion(This,value)	\
    (This)->lpVtbl -> getSpecificationVersion(This,value)

#define X3DScene_getEncoding(This,encodingType)	\
    (This)->lpVtbl -> getEncoding(This,encodingType)

#define X3DScene_getNode(This,name,value)	\
    (This)->lpVtbl -> getNode(This,name,value)

#define X3DScene_createNode(This,type,node)	\
    (This)->lpVtbl -> createNode(This,type,node)

#define X3DScene_addRoute(This,fromNode,fromEventOut,toNode,toEventIn,ppRoute)	\
    (This)->lpVtbl -> addRoute(This,fromNode,fromEventOut,toNode,toEventIn,ppRoute)

#define X3DScene_deleteRoute(This,pRoute)	\
    (This)->lpVtbl -> deleteRoute(This,pRoute)

#define X3DScene_getRootNodes(This,rootnodes)	\
    (This)->lpVtbl -> getRootNodes(This,rootnodes)

#define X3DScene_getProtoDeclarations(This,protodeclarations)	\
    (This)->lpVtbl -> getProtoDeclarations(This,protodeclarations)


#define X3DScene_getMetaData(This,key,value)	\
    (This)->lpVtbl -> getMetaData(This,key,value)

#define X3DScene_setMetaData(This,key,value)	\
    (This)->lpVtbl -> setMetaData(This,key,value)

#define X3DScene_addRootNode(This,node)	\
    (This)->lpVtbl -> addRootNode(This,node)

#define X3DScene_removeRootNode(This,node)	\
    (This)->lpVtbl -> removeRootNode(This,node)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DScene_getMetaData_Proxy( 
    X3DScene __RPC_FAR * This,
    /* [in] */ BSTR key,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DScene_getMetaData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DScene_setMetaData_Proxy( 
    X3DScene __RPC_FAR * This,
    /* [in] */ BSTR key,
    /* [in] */ BSTR value);


void __RPC_STUB X3DScene_setMetaData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DScene_addRootNode_Proxy( 
    X3DScene __RPC_FAR * This,
    /* [in] */ X3DNode __RPC_FAR *node);


void __RPC_STUB X3DScene_addRootNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DScene_removeRootNode_Proxy( 
    X3DScene __RPC_FAR * This,
    /* [in] */ X3DNode __RPC_FAR *node);


void __RPC_STUB X3DScene_removeRootNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DScene_INTERFACE_DEFINED__ */


#ifndef __X3DBrowserListener_INTERFACE_DEFINED__
#define __X3DBrowserListener_INTERFACE_DEFINED__

/* interface X3DBrowserListener */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DBrowserListener;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7E147096-E5B6-4C6C-A610-256DE53B92EC")
    X3DBrowserListener : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE browserChanged( 
            /* [in] */ X3DBrowserEvent event) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DBrowserListenerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DBrowserListener __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DBrowserListener __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DBrowserListener __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DBrowserListener __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DBrowserListener __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DBrowserListener __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DBrowserListener __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *browserChanged )( 
            X3DBrowserListener __RPC_FAR * This,
            /* [in] */ X3DBrowserEvent event);
        
        END_INTERFACE
    } X3DBrowserListenerVtbl;

    interface X3DBrowserListener
    {
        CONST_VTBL struct X3DBrowserListenerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DBrowserListener_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DBrowserListener_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DBrowserListener_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DBrowserListener_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DBrowserListener_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DBrowserListener_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DBrowserListener_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DBrowserListener_browserChanged(This,event)	\
    (This)->lpVtbl -> browserChanged(This,event)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DBrowserListener_browserChanged_Proxy( 
    X3DBrowserListener __RPC_FAR * This,
    /* [in] */ X3DBrowserEvent event);


void __RPC_STUB X3DBrowserListener_browserChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DBrowserListener_INTERFACE_DEFINED__ */


#ifndef __X3DFieldEventListener_INTERFACE_DEFINED__
#define __X3DFieldEventListener_INTERFACE_DEFINED__

/* interface X3DFieldEventListener */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DFieldEventListener;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2EE4BBCA-E98E-407D-8E37-90EB2FE2E856")
    X3DFieldEventListener : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readableFieldChanged( 
            /* [in] */ X3DField __RPC_FAR *pField,
            double timestamp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DFieldEventListenerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DFieldEventListener __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DFieldEventListener __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DFieldEventListener __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DFieldEventListener __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DFieldEventListener __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DFieldEventListener __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DFieldEventListener __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *readableFieldChanged )( 
            X3DFieldEventListener __RPC_FAR * This,
            /* [in] */ X3DField __RPC_FAR *pField,
            double timestamp);
        
        END_INTERFACE
    } X3DFieldEventListenerVtbl;

    interface X3DFieldEventListener
    {
        CONST_VTBL struct X3DFieldEventListenerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DFieldEventListener_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DFieldEventListener_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DFieldEventListener_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DFieldEventListener_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DFieldEventListener_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DFieldEventListener_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DFieldEventListener_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DFieldEventListener_readableFieldChanged(This,pField,timestamp)	\
    (This)->lpVtbl -> readableFieldChanged(This,pField,timestamp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DFieldEventListener_readableFieldChanged_Proxy( 
    X3DFieldEventListener __RPC_FAR * This,
    /* [in] */ X3DField __RPC_FAR *pField,
    double timestamp);


void __RPC_STUB X3DFieldEventListener_readableFieldChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DFieldEventListener_INTERFACE_DEFINED__ */


#ifndef __X3DRoute_INTERFACE_DEFINED__
#define __X3DRoute_INTERFACE_DEFINED__

/* interface X3DRoute */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DRoute;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8ACA7B53-957E-4432-8F40-2D8B105C856C")
    X3DRoute : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setClosure( 
            /* [in] */ void __RPC_FAR *closure) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getClosure( 
            /* [in] */ void __RPC_FAR *__RPC_FAR *closure) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setContext( 
            /* [in] */ X3DExecutionContext __RPC_FAR *context) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getContext( 
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getSourceNode( 
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *pSourceNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getSourceField( 
            /* [retval][out] */ BSTR __RPC_FAR *sourcefield) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getDestinationNode( 
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *pDestinationNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getDestinationField( 
            /* [retval][out] */ BSTR __RPC_FAR *destinationfield) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DRouteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DRoute __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DRoute __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DRoute __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setClosure )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getClosure )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *__RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setContext )( 
            X3DRoute __RPC_FAR * This,
            /* [in] */ X3DExecutionContext __RPC_FAR *context);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getContext )( 
            X3DRoute __RPC_FAR * This,
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSourceNode )( 
            X3DRoute __RPC_FAR * This,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *pSourceNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSourceField )( 
            X3DRoute __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *sourcefield);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getDestinationNode )( 
            X3DRoute __RPC_FAR * This,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *pDestinationNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getDestinationField )( 
            X3DRoute __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *destinationfield);
        
        END_INTERFACE
    } X3DRouteVtbl;

    interface X3DRoute
    {
        CONST_VTBL struct X3DRouteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DRoute_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DRoute_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DRoute_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DRoute_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DRoute_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DRoute_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DRoute_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DRoute_setClosure(This,closure)	\
    (This)->lpVtbl -> setClosure(This,closure)

#define X3DRoute_getClosure(This,closure)	\
    (This)->lpVtbl -> getClosure(This,closure)

#define X3DRoute_setContext(This,context)	\
    (This)->lpVtbl -> setContext(This,context)

#define X3DRoute_getContext(This,result)	\
    (This)->lpVtbl -> getContext(This,result)

#define X3DRoute_getSourceNode(This,pSourceNode)	\
    (This)->lpVtbl -> getSourceNode(This,pSourceNode)

#define X3DRoute_getSourceField(This,sourcefield)	\
    (This)->lpVtbl -> getSourceField(This,sourcefield)

#define X3DRoute_getDestinationNode(This,pDestinationNode)	\
    (This)->lpVtbl -> getDestinationNode(This,pDestinationNode)

#define X3DRoute_getDestinationField(This,destinationfield)	\
    (This)->lpVtbl -> getDestinationField(This,destinationfield)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_setClosure_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *closure);


void __RPC_STUB X3DRoute_setClosure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_getClosure_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *__RPC_FAR *closure);


void __RPC_STUB X3DRoute_getClosure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_setContext_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [in] */ X3DExecutionContext __RPC_FAR *context);


void __RPC_STUB X3DRoute_setContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_getContext_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB X3DRoute_getContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_getSourceNode_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *pSourceNode);


void __RPC_STUB X3DRoute_getSourceNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_getSourceField_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *sourcefield);


void __RPC_STUB X3DRoute_getSourceField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_getDestinationNode_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *pDestinationNode);


void __RPC_STUB X3DRoute_getDestinationNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DRoute_getDestinationField_Proxy( 
    X3DRoute __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *destinationfield);


void __RPC_STUB X3DRoute_getDestinationField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DRoute_INTERFACE_DEFINED__ */


#ifndef __X3DSFBool_INTERFACE_DEFINED__
#define __X3DSFBool_INTERFACE_DEFINED__

/* interface X3DSFBool */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFBool;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2DE0158A-0C8E-4667-820C-074953FB957A")
    X3DSFBool : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ VARIANT_BOOL value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFBoolVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFBool __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFBool __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFBool __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFBool __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFBool __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFBool __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFBool __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFBool __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFBool __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFBool __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL value);
        
        END_INTERFACE
    } X3DSFBoolVtbl;

    interface X3DSFBool
    {
        CONST_VTBL struct X3DSFBoolVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFBool_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFBool_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFBool_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFBool_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFBool_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFBool_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFBool_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFBool_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFBool_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFBool_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFBool_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFBool_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFBool_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFBool_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFBool_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFBool_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFBool_getValue_Proxy( 
    X3DSFBool __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB X3DSFBool_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFBool_setValue_Proxy( 
    X3DSFBool __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL value);


void __RPC_STUB X3DSFBool_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFBool_INTERFACE_DEFINED__ */


#ifndef __X3DSFColor_INTERFACE_DEFINED__
#define __X3DSFColor_INTERFACE_DEFINED__

/* interface X3DSFColor */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFColor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C7C5930C-186E-4B78-8040-BD1226828888")
    X3DSFColor : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_r( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_r( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_g( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_g( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_b( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_b( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFColorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFColor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFColor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFColor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFColor __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFColor __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFColor __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFColor __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFColor __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFColor __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFColor __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_r )( 
            X3DSFColor __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_r )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_g )( 
            X3DSFColor __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_g )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_b )( 
            X3DSFColor __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_b )( 
            X3DSFColor __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } X3DSFColorVtbl;

    interface X3DSFColor
    {
        CONST_VTBL struct X3DSFColorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFColor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFColor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFColor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFColor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFColor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFColor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFColor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFColor_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFColor_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFColor_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFColor_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFColor_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFColor_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFColor_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFColor_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFColor_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#define X3DSFColor_get_r(This,pVal)	\
    (This)->lpVtbl -> get_r(This,pVal)

#define X3DSFColor_put_r(This,newVal)	\
    (This)->lpVtbl -> put_r(This,newVal)

#define X3DSFColor_get_g(This,pVal)	\
    (This)->lpVtbl -> get_g(This,pVal)

#define X3DSFColor_put_g(This,newVal)	\
    (This)->lpVtbl -> put_g(This,newVal)

#define X3DSFColor_get_b(This,pVal)	\
    (This)->lpVtbl -> get_b(This,pVal)

#define X3DSFColor_put_b(This,newVal)	\
    (This)->lpVtbl -> put_b(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFColor_getValue_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFColor_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFColor_setValue_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFColor_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFColor_get_r_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFColor_get_r_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFColor_put_r_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFColor_put_r_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFColor_get_g_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFColor_get_g_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFColor_put_g_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFColor_put_g_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFColor_get_b_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFColor_get_b_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFColor_put_b_Proxy( 
    X3DSFColor __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFColor_put_b_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFColor_INTERFACE_DEFINED__ */


#ifndef __X3DSFFloat_INTERFACE_DEFINED__
#define __X3DSFFloat_INTERFACE_DEFINED__

/* interface X3DSFFloat */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFFloat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2D91D7E4-8A06-4626-8A41-6877657D1034")
    X3DSFFloat : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ float value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFFloatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFFloat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFFloat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFFloat __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFFloat __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFFloat __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFFloat __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFFloat __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFFloat __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFFloat __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFFloat __RPC_FAR * This,
            /* [in] */ float value);
        
        END_INTERFACE
    } X3DSFFloatVtbl;

    interface X3DSFFloat
    {
        CONST_VTBL struct X3DSFFloatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFFloat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFFloat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFFloat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFFloat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFFloat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFFloat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFFloat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFFloat_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFFloat_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFFloat_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFFloat_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFFloat_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFFloat_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFFloat_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFFloat_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFFloat_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFFloat_getValue_Proxy( 
    X3DSFFloat __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFFloat_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFFloat_setValue_Proxy( 
    X3DSFFloat __RPC_FAR * This,
    /* [in] */ float value);


void __RPC_STUB X3DSFFloat_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFFloat_INTERFACE_DEFINED__ */


#ifndef __X3DSFInt32_INTERFACE_DEFINED__
#define __X3DSFInt32_INTERFACE_DEFINED__

/* interface X3DSFInt32 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFInt32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1DB18114-E6A1-490E-A4C2-A29201999696")
    X3DSFInt32 : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ long __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ long value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFInt32Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFInt32 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFInt32 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFInt32 __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFInt32 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFInt32 __RPC_FAR * This,
            /* [in] */ long value);
        
        END_INTERFACE
    } X3DSFInt32Vtbl;

    interface X3DSFInt32
    {
        CONST_VTBL struct X3DSFInt32Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFInt32_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFInt32_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFInt32_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFInt32_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFInt32_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFInt32_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFInt32_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFInt32_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFInt32_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFInt32_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFInt32_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFInt32_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFInt32_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFInt32_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFInt32_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFInt32_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFInt32_getValue_Proxy( 
    X3DSFInt32 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *value);


void __RPC_STUB X3DSFInt32_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFInt32_setValue_Proxy( 
    X3DSFInt32 __RPC_FAR * This,
    /* [in] */ long value);


void __RPC_STUB X3DSFInt32_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFInt32_INTERFACE_DEFINED__ */


#ifndef __X3DSFNode_INTERFACE_DEFINED__
#define __X3DSFNode_INTERFACE_DEFINED__

/* interface X3DSFNode */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9410F6BF-BE28-48F8-97E8-7B040FC8E25C")
    X3DSFNode : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ X3DNode __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFNode __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFNode __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFNode __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFNode __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFNode __RPC_FAR * This,
            /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFNode __RPC_FAR * This,
            /* [in] */ X3DNode __RPC_FAR *value);
        
        END_INTERFACE
    } X3DSFNodeVtbl;

    interface X3DSFNode
    {
        CONST_VTBL struct X3DSFNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFNode_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFNode_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFNode_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFNode_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFNode_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFNode_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFNode_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFNode_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFNode_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFNode_getValue_Proxy( 
    X3DSFNode __RPC_FAR * This,
    /* [retval][out] */ X3DNode __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DSFNode_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFNode_setValue_Proxy( 
    X3DSFNode __RPC_FAR * This,
    /* [in] */ X3DNode __RPC_FAR *value);


void __RPC_STUB X3DSFNode_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFNode_INTERFACE_DEFINED__ */


#ifndef __X3DSFRotation_INTERFACE_DEFINED__
#define __X3DSFRotation_INTERFACE_DEFINED__

/* interface X3DSFRotation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B13C7843-EB3B-4D36-8C77-7C4DD37277F1")
    X3DSFRotation : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_x( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_x( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_y( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_y( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_z( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_z( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_angle( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_angle( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFRotation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFRotation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFRotation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFRotation __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFRotation __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFRotation __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFRotation __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_x )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_x )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_y )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_y )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_z )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_z )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_angle )( 
            X3DSFRotation __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_angle )( 
            X3DSFRotation __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } X3DSFRotationVtbl;

    interface X3DSFRotation
    {
        CONST_VTBL struct X3DSFRotationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFRotation_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFRotation_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFRotation_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFRotation_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFRotation_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFRotation_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFRotation_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFRotation_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFRotation_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#define X3DSFRotation_get_x(This,pVal)	\
    (This)->lpVtbl -> get_x(This,pVal)

#define X3DSFRotation_put_x(This,newVal)	\
    (This)->lpVtbl -> put_x(This,newVal)

#define X3DSFRotation_get_y(This,pVal)	\
    (This)->lpVtbl -> get_y(This,pVal)

#define X3DSFRotation_put_y(This,newVal)	\
    (This)->lpVtbl -> put_y(This,newVal)

#define X3DSFRotation_get_z(This,pVal)	\
    (This)->lpVtbl -> get_z(This,pVal)

#define X3DSFRotation_put_z(This,newVal)	\
    (This)->lpVtbl -> put_z(This,newVal)

#define X3DSFRotation_get_angle(This,pVal)	\
    (This)->lpVtbl -> get_angle(This,pVal)

#define X3DSFRotation_put_angle(This,newVal)	\
    (This)->lpVtbl -> put_angle(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_getValue_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFRotation_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_setValue_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFRotation_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_get_x_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFRotation_get_x_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_put_x_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFRotation_put_x_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_get_y_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFRotation_get_y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_put_y_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFRotation_put_y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_get_z_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFRotation_get_z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_put_z_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFRotation_put_z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_get_angle_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFRotation_get_angle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFRotation_put_angle_Proxy( 
    X3DSFRotation __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFRotation_put_angle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFRotation_INTERFACE_DEFINED__ */


#ifndef __X3DSFString_INTERFACE_DEFINED__
#define __X3DSFString_INTERFACE_DEFINED__

/* interface X3DSFString */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AD9E8B1D-84CB-498F-B050-FFD913A5D875")
    X3DSFString : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [out][in] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFString __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFString __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFString __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFString __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFString __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFString __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFString __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFString __RPC_FAR * This,
            /* [out][in] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFString __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        END_INTERFACE
    } X3DSFStringVtbl;

    interface X3DSFString
    {
        CONST_VTBL struct X3DSFStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFString_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFString_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFString_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFString_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFString_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFString_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFString_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFString_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFString_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFString_getValue_Proxy( 
    X3DSFString __RPC_FAR * This,
    /* [out][in] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DSFString_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFString_setValue_Proxy( 
    X3DSFString __RPC_FAR * This,
    /* [in] */ BSTR value);


void __RPC_STUB X3DSFString_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFString_INTERFACE_DEFINED__ */


#ifndef __X3DSFVec2f_INTERFACE_DEFINED__
#define __X3DSFVec2f_INTERFACE_DEFINED__

/* interface X3DSFVec2f */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFVec2f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8152C989-858A-470D-BC61-0532EC81A902")
    X3DSFVec2f : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_x( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_x( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_y( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_y( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFVec2fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFVec2f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFVec2f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFVec2f __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFVec2f __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_x )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_x )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_y )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_y )( 
            X3DSFVec2f __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } X3DSFVec2fVtbl;

    interface X3DSFVec2f
    {
        CONST_VTBL struct X3DSFVec2fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFVec2f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFVec2f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFVec2f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFVec2f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFVec2f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFVec2f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFVec2f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFVec2f_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFVec2f_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFVec2f_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFVec2f_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFVec2f_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFVec2f_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFVec2f_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFVec2f_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFVec2f_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#define X3DSFVec2f_get_x(This,pVal)	\
    (This)->lpVtbl -> get_x(This,pVal)

#define X3DSFVec2f_put_x(This,newVal)	\
    (This)->lpVtbl -> put_x(This,newVal)

#define X3DSFVec2f_get_y(This,pVal)	\
    (This)->lpVtbl -> get_y(This,pVal)

#define X3DSFVec2f_put_y(This,newVal)	\
    (This)->lpVtbl -> put_y(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFVec2f_getValue_Proxy( 
    X3DSFVec2f __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFVec2f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFVec2f_setValue_Proxy( 
    X3DSFVec2f __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFVec2f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFVec2f_get_x_Proxy( 
    X3DSFVec2f __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFVec2f_get_x_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFVec2f_put_x_Proxy( 
    X3DSFVec2f __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFVec2f_put_x_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFVec2f_get_y_Proxy( 
    X3DSFVec2f __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFVec2f_get_y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFVec2f_put_y_Proxy( 
    X3DSFVec2f __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFVec2f_put_y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFVec2f_INTERFACE_DEFINED__ */


#ifndef __X3DSFVec3f_INTERFACE_DEFINED__
#define __X3DSFVec3f_INTERFACE_DEFINED__

/* interface X3DSFVec3f */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFVec3f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("86771EAA-A5EB-4F9F-82C1-437160DAB4A5")
    X3DSFVec3f : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_x( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_x( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_y( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_y( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_z( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_z( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFVec3fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFVec3f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFVec3f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFVec3f __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFVec3f __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_x )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_x )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_y )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_y )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_z )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_z )( 
            X3DSFVec3f __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } X3DSFVec3fVtbl;

    interface X3DSFVec3f
    {
        CONST_VTBL struct X3DSFVec3fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFVec3f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFVec3f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFVec3f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFVec3f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFVec3f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFVec3f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFVec3f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFVec3f_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFVec3f_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFVec3f_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFVec3f_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFVec3f_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFVec3f_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFVec3f_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFVec3f_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFVec3f_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#define X3DSFVec3f_get_x(This,pVal)	\
    (This)->lpVtbl -> get_x(This,pVal)

#define X3DSFVec3f_put_x(This,newVal)	\
    (This)->lpVtbl -> put_x(This,newVal)

#define X3DSFVec3f_get_y(This,pVal)	\
    (This)->lpVtbl -> get_y(This,pVal)

#define X3DSFVec3f_put_y(This,newVal)	\
    (This)->lpVtbl -> put_y(This,newVal)

#define X3DSFVec3f_get_z(This,pVal)	\
    (This)->lpVtbl -> get_z(This,pVal)

#define X3DSFVec3f_put_z(This,newVal)	\
    (This)->lpVtbl -> put_z(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_getValue_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFVec3f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_setValue_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DSFVec3f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_get_x_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFVec3f_get_x_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_put_x_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFVec3f_put_x_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_get_y_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFVec3f_get_y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_put_y_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFVec3f_put_y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_get_z_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB X3DSFVec3f_get_z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DSFVec3f_put_z_Proxy( 
    X3DSFVec3f __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB X3DSFVec3f_put_z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFVec3f_INTERFACE_DEFINED__ */


#ifndef __X3DNodeArray_INTERFACE_DEFINED__
#define __X3DNodeArray_INTERFACE_DEFINED__

/* interface X3DNodeArray */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DNodeArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B9A89471-6092-439D-A453-D23A80D62616")
    X3DNodeArray : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Count( 
            /* [in] */ long val) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [in] */ X3DNode __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ X3DNode __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_length( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DNodeArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DNodeArray __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DNodeArray __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DNodeArray __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DNodeArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Count )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ long val);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DNode __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DNode __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DNodeArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DNodeArray __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } X3DNodeArrayVtbl;

    interface X3DNodeArray
    {
        CONST_VTBL struct X3DNodeArrayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DNodeArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DNodeArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DNodeArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DNodeArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DNodeArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DNodeArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DNodeArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DNodeArray_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)

#define X3DNodeArray_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DNodeArray_put_Count(This,val)	\
    (This)->lpVtbl -> put_Count(This,val)

#define X3DNodeArray_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#define X3DNodeArray_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DNodeArray_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DNodeArray_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_Item_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [in] */ VARIANT index,
    /* [retval][out] */ VARIANT __RPC_FAR *pItem);


void __RPC_STUB X3DNodeArray_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_get_Count_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DNodeArray_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_put_Count_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [in] */ long val);


void __RPC_STUB X3DNodeArray_put_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_get1Value_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DNode __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DNodeArray_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_set1Value_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DNode __RPC_FAR *value);


void __RPC_STUB X3DNodeArray_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_get_length_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DNodeArray_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DNodeArray_put_length_Proxy( 
    X3DNodeArray __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB X3DNodeArray_put_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DNodeArray_INTERFACE_DEFINED__ */


#ifndef __X3DFieldDefinition_INTERFACE_DEFINED__
#define __X3DFieldDefinition_INTERFACE_DEFINED__

/* interface X3DFieldDefinition */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DFieldDefinition;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B4665F92-C9C9-41B9-8EEF-A46FBA4E4615")
    X3DFieldDefinition : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_dataType( 
            /* [retval][out] */ X3DFieldType __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_dataType( 
            /* [in] */ X3DFieldType newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_accessType( 
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_accessType( 
            /* [in] */ X3DFieldAccess newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DFieldDefinitionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DFieldDefinition __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DFieldDefinition __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_name )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_dataType )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_dataType )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ X3DFieldType newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessType )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessType )( 
            X3DFieldDefinition __RPC_FAR * This,
            /* [in] */ X3DFieldAccess newVal);
        
        END_INTERFACE
    } X3DFieldDefinitionVtbl;

    interface X3DFieldDefinition
    {
        CONST_VTBL struct X3DFieldDefinitionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DFieldDefinition_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DFieldDefinition_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DFieldDefinition_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DFieldDefinition_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DFieldDefinition_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DFieldDefinition_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DFieldDefinition_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DFieldDefinition_get_name(This,pVal)	\
    (This)->lpVtbl -> get_name(This,pVal)

#define X3DFieldDefinition_put_name(This,newVal)	\
    (This)->lpVtbl -> put_name(This,newVal)

#define X3DFieldDefinition_get_dataType(This,pVal)	\
    (This)->lpVtbl -> get_dataType(This,pVal)

#define X3DFieldDefinition_put_dataType(This,newVal)	\
    (This)->lpVtbl -> put_dataType(This,newVal)

#define X3DFieldDefinition_get_accessType(This,pVal)	\
    (This)->lpVtbl -> get_accessType(This,pVal)

#define X3DFieldDefinition_put_accessType(This,newVal)	\
    (This)->lpVtbl -> put_accessType(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinition_get_name_Proxy( 
    X3DFieldDefinition __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB X3DFieldDefinition_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinition_put_name_Proxy( 
    X3DFieldDefinition __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB X3DFieldDefinition_put_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinition_get_dataType_Proxy( 
    X3DFieldDefinition __RPC_FAR * This,
    /* [retval][out] */ X3DFieldType __RPC_FAR *pVal);


void __RPC_STUB X3DFieldDefinition_get_dataType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinition_put_dataType_Proxy( 
    X3DFieldDefinition __RPC_FAR * This,
    /* [in] */ X3DFieldType newVal);


void __RPC_STUB X3DFieldDefinition_put_dataType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinition_get_accessType_Proxy( 
    X3DFieldDefinition __RPC_FAR * This,
    /* [retval][out] */ X3DFieldAccess __RPC_FAR *pVal);


void __RPC_STUB X3DFieldDefinition_get_accessType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinition_put_accessType_Proxy( 
    X3DFieldDefinition __RPC_FAR * This,
    /* [in] */ X3DFieldAccess newVal);


void __RPC_STUB X3DFieldDefinition_put_accessType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DFieldDefinition_INTERFACE_DEFINED__ */


#ifndef __X3DFieldDefinitionArray_INTERFACE_DEFINED__
#define __X3DFieldDefinitionArray_INTERFACE_DEFINED__

/* interface X3DFieldDefinitionArray */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DFieldDefinitionArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("49CCCA4A-56E1-42B1-8496-FCE99AAF605B")
    X3DFieldDefinitionArray : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Count( 
            /* [in] */ long val) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [in] */ X3DFieldDefinition __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            long index,
            X3DFieldDefinition __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_length( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DFieldDefinitionArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DFieldDefinitionArray __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DFieldDefinitionArray __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Count )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ long val);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DFieldDefinition __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            long index,
            X3DFieldDefinition __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DFieldDefinitionArray __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } X3DFieldDefinitionArrayVtbl;

    interface X3DFieldDefinitionArray
    {
        CONST_VTBL struct X3DFieldDefinitionArrayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DFieldDefinitionArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DFieldDefinitionArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DFieldDefinitionArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DFieldDefinitionArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DFieldDefinitionArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DFieldDefinitionArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DFieldDefinitionArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DFieldDefinitionArray_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)

#define X3DFieldDefinitionArray_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DFieldDefinitionArray_put_Count(This,val)	\
    (This)->lpVtbl -> put_Count(This,val)

#define X3DFieldDefinitionArray_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#define X3DFieldDefinitionArray_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DFieldDefinitionArray_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DFieldDefinitionArray_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_Item_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    /* [in] */ VARIANT index,
    /* [retval][out] */ VARIANT __RPC_FAR *pItem);


void __RPC_STUB X3DFieldDefinitionArray_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_get_Count_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DFieldDefinitionArray_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_put_Count_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    /* [in] */ long val);


void __RPC_STUB X3DFieldDefinitionArray_put_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_get1Value_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DFieldDefinition __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DFieldDefinitionArray_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_set1Value_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    long index,
    X3DFieldDefinition __RPC_FAR *value);


void __RPC_STUB X3DFieldDefinitionArray_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_get_length_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DFieldDefinitionArray_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DFieldDefinitionArray_put_length_Proxy( 
    X3DFieldDefinitionArray __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB X3DFieldDefinitionArray_put_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DFieldDefinitionArray_INTERFACE_DEFINED__ */


#ifndef __X3DProtoDeclaration_INTERFACE_DEFINED__
#define __X3DProtoDeclaration_INTERFACE_DEFINED__

/* interface X3DProtoDeclaration */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DProtoDeclaration;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B39E9F96-0CEA-4BA8-B8BE-796DA37A3B45")
    X3DProtoDeclaration : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_isExternProto( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_fields( 
            /* [retval][out] */ X3DFieldDefinitionArray __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setNativeProtoDeclaration( 
            /* [in] */ void __RPC_FAR *pNativeProtoDeclaration) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DProtoDeclarationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DProtoDeclaration __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DProtoDeclaration __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_isExternProto )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fields )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [retval][out] */ X3DFieldDefinitionArray __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setNativeProtoDeclaration )( 
            X3DProtoDeclaration __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pNativeProtoDeclaration);
        
        END_INTERFACE
    } X3DProtoDeclarationVtbl;

    interface X3DProtoDeclaration
    {
        CONST_VTBL struct X3DProtoDeclarationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DProtoDeclaration_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DProtoDeclaration_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DProtoDeclaration_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DProtoDeclaration_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DProtoDeclaration_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DProtoDeclaration_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DProtoDeclaration_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DProtoDeclaration_get_name(This,pVal)	\
    (This)->lpVtbl -> get_name(This,pVal)

#define X3DProtoDeclaration_get_isExternProto(This,pVal)	\
    (This)->lpVtbl -> get_isExternProto(This,pVal)

#define X3DProtoDeclaration_get_fields(This,pVal)	\
    (This)->lpVtbl -> get_fields(This,pVal)

#define X3DProtoDeclaration_setNativeProtoDeclaration(This,pNativeProtoDeclaration)	\
    (This)->lpVtbl -> setNativeProtoDeclaration(This,pNativeProtoDeclaration)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclaration_get_name_Proxy( 
    X3DProtoDeclaration __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB X3DProtoDeclaration_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclaration_get_isExternProto_Proxy( 
    X3DProtoDeclaration __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB X3DProtoDeclaration_get_isExternProto_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclaration_get_fields_Proxy( 
    X3DProtoDeclaration __RPC_FAR * This,
    /* [retval][out] */ X3DFieldDefinitionArray __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB X3DProtoDeclaration_get_fields_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclaration_setNativeProtoDeclaration_Proxy( 
    X3DProtoDeclaration __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *pNativeProtoDeclaration);


void __RPC_STUB X3DProtoDeclaration_setNativeProtoDeclaration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DProtoDeclaration_INTERFACE_DEFINED__ */


#ifndef __X3DProtoDeclarationArray_INTERFACE_DEFINED__
#define __X3DProtoDeclarationArray_INTERFACE_DEFINED__

/* interface X3DProtoDeclarationArray */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DProtoDeclarationArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1AACE5AB-5450-4B90-A151-2D87BB5E498F")
    X3DProtoDeclarationArray : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Count( 
            /* [in] */ long val) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [in] */ X3DProtoDeclaration __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            long index,
            X3DProtoDeclaration __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_length( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DProtoDeclarationArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DProtoDeclarationArray __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DProtoDeclarationArray __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Count )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ long val);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DProtoDeclaration __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            long index,
            X3DProtoDeclaration __RPC_FAR *value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DProtoDeclarationArray __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } X3DProtoDeclarationArrayVtbl;

    interface X3DProtoDeclarationArray
    {
        CONST_VTBL struct X3DProtoDeclarationArrayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DProtoDeclarationArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DProtoDeclarationArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DProtoDeclarationArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DProtoDeclarationArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DProtoDeclarationArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DProtoDeclarationArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DProtoDeclarationArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DProtoDeclarationArray_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)

#define X3DProtoDeclarationArray_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DProtoDeclarationArray_put_Count(This,val)	\
    (This)->lpVtbl -> put_Count(This,val)

#define X3DProtoDeclarationArray_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#define X3DProtoDeclarationArray_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DProtoDeclarationArray_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DProtoDeclarationArray_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_Item_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    /* [in] */ VARIANT index,
    /* [retval][out] */ VARIANT __RPC_FAR *pItem);


void __RPC_STUB X3DProtoDeclarationArray_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_get_Count_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DProtoDeclarationArray_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_put_Count_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    /* [in] */ long val);


void __RPC_STUB X3DProtoDeclarationArray_put_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_get1Value_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DProtoDeclaration __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DProtoDeclarationArray_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_set1Value_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    long index,
    X3DProtoDeclaration __RPC_FAR *value);


void __RPC_STUB X3DProtoDeclarationArray_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_get_length_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DProtoDeclarationArray_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DProtoDeclarationArray_put_length_Proxy( 
    X3DProtoDeclarationArray __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB X3DProtoDeclarationArray_put_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DProtoDeclarationArray_INTERFACE_DEFINED__ */


#ifndef ___DAnima_DISPINTERFACE_DEFINED__
#define ___DAnima_DISPINTERFACE_DEFINED__

/* dispinterface _DAnima */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__DAnima;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("44166B5D-CA84-4DFE-9ABB-5B03E72E9C05")
    _DAnima : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAnimaVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _DAnima __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _DAnima __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _DAnima __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _DAnima __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _DAnima __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _DAnima __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _DAnima __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _DAnimaVtbl;

    interface _DAnima
    {
        CONST_VTBL struct _DAnimaVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAnima_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DAnima_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DAnima_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DAnima_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DAnima_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DAnima_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DAnima_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAnima_DISPINTERFACE_DEFINED__ */


#ifndef ___DAnimaEvents_DISPINTERFACE_DEFINED__
#define ___DAnimaEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DAnimaEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAnimaEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CFB6910F-3849-417E-9217-A8532E7A1380")
    _DAnimaEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAnimaEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _DAnimaEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _DAnimaEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _DAnimaEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _DAnimaEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _DAnimaEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _DAnimaEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _DAnimaEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _DAnimaEventsVtbl;

    interface _DAnimaEvents
    {
        CONST_VTBL struct _DAnimaEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAnimaEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DAnimaEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DAnimaEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DAnimaEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DAnimaEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DAnimaEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DAnimaEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAnimaEvents_DISPINTERFACE_DEFINED__ */


#ifndef __X3DMField_INTERFACE_DEFINED__
#define __X3DMField_INTERFACE_DEFINED__

/* interface X3DMField */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMField;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4A564091-9A41-43DC-AE4C-4A1457D5A53E")
    X3DMField : public X3DField
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_length( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFieldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMField __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMField __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMField __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMField __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMField __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMField __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMField __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMField __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMField __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMField __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMField __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        END_INTERFACE
    } X3DMFieldVtbl;

    interface X3DMField
    {
        CONST_VTBL struct X3DMFieldVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMField_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMField_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMField_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMField_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMField_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMField_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMField_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMField_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMField_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMField_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMField_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMField_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMField_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMField_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMField_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMField_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMField_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMField_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DMField_get_Count_Proxy( 
    X3DMField __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DMField_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE X3DMField_get_length_Proxy( 
    X3DMField __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB X3DMField_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE X3DMField_put_length_Proxy( 
    X3DMField __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB X3DMField_put_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMField_Item_Proxy( 
    X3DMField __RPC_FAR * This,
    /* [in] */ VARIANT index,
    /* [retval][out] */ VARIANT __RPC_FAR *pItem);


void __RPC_STUB X3DMField_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMField_INTERFACE_DEFINED__ */


#ifndef __X3DMFColor_INTERFACE_DEFINED__
#define __X3DMFColor_INTERFACE_DEFINED__

/* interface X3DMFColor */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFColor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("16662D28-9E70-4161-A2AB-F0C65506DF3B")
    X3DMFColor : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ X3DSFColor __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ X3DSFColor __RPC_FAR *__RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFColorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFColor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFColor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFColor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFColor __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFColor __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFColor __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFColor __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFColor __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFColor __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFColor __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DSFColor __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFColor __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ X3DSFColor __RPC_FAR *__RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFColorVtbl;

    interface X3DMFColor
    {
        CONST_VTBL struct X3DMFColorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFColor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFColor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFColor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFColor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFColor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFColor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFColor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFColor_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFColor_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFColor_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFColor_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFColor_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFColor_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFColor_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFColor_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFColor_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFColor_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFColor_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFColor_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFColor_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFColor_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFColor_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFColor_getValue_Proxy( 
    X3DMFColor __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFColor_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFColor_setValue_Proxy( 
    X3DMFColor __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFColor_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFColor_set1Value_Proxy( 
    X3DMFColor __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DSFColor __RPC_FAR *value);


void __RPC_STUB X3DMFColor_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFColor_get1Value_Proxy( 
    X3DMFColor __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ X3DSFColor __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DMFColor_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFColor_INTERFACE_DEFINED__ */


#ifndef __X3DMFNode_INTERFACE_DEFINED__
#define __X3DMFNode_INTERFACE_DEFINED__

/* interface X3DMFNode */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("041BC54C-F94B-4829-A05E-8E7059423524")
    X3DMFNode : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ X3DNodeArray __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [out] */ X3DNode __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1ValueV( 
            /* [in] */ int index,
            /* [retval][out] */ VARIANT __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ X3DNode __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFNode __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFNode __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFNode __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFNode __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFNode __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFNode __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFNode __RPC_FAR * This,
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ X3DNodeArray __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ int index,
            /* [out] */ X3DNode __RPC_FAR *__RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1ValueV )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ VARIANT __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFNode __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DNode __RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFNodeVtbl;

    interface X3DMFNode
    {
        CONST_VTBL struct X3DMFNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFNode_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFNode_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFNode_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFNode_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFNode_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFNode_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFNode_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFNode_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFNode_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFNode_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFNode_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFNode_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DMFNode_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#define X3DMFNode_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#define X3DMFNode_get1ValueV(This,index,value)	\
    (This)->lpVtbl -> get1ValueV(This,index,value)

#define X3DMFNode_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFNode_getValue_Proxy( 
    X3DMFNode __RPC_FAR * This,
    /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DMFNode_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFNode_setValue_Proxy( 
    X3DMFNode __RPC_FAR * This,
    /* [in] */ X3DNodeArray __RPC_FAR *value);


void __RPC_STUB X3DMFNode_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFNode_get1Value_Proxy( 
    X3DMFNode __RPC_FAR * This,
    /* [in] */ int index,
    /* [out] */ X3DNode __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DMFNode_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFNode_get1ValueV_Proxy( 
    X3DMFNode __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ VARIANT __RPC_FAR *value);


void __RPC_STUB X3DMFNode_get1ValueV_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFNode_set1Value_Proxy( 
    X3DMFNode __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DNode __RPC_FAR *value);


void __RPC_STUB X3DMFNode_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFNode_INTERFACE_DEFINED__ */


#ifndef __X3DSFTime_INTERFACE_DEFINED__
#define __X3DSFTime_INTERFACE_DEFINED__

/* interface X3DSFTime */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFTime;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3172E8A3-97EB-4DD3-866A-3FBC0F6EA457")
    X3DSFTime : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ double __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ double __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFTimeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFTime __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFTime __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFTime __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFTime __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFTime __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFTime __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFTime __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFTime __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFTime __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFTime __RPC_FAR * This,
            /* [in] */ double __RPC_FAR *value);
        
        END_INTERFACE
    } X3DSFTimeVtbl;

    interface X3DSFTime
    {
        CONST_VTBL struct X3DSFTimeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFTime_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFTime_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFTime_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFTime_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFTime_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFTime_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFTime_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFTime_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFTime_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFTime_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFTime_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFTime_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFTime_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFTime_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFTime_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFTime_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFTime_getValue_Proxy( 
    X3DSFTime __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *value);


void __RPC_STUB X3DSFTime_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFTime_setValue_Proxy( 
    X3DSFTime __RPC_FAR * This,
    /* [in] */ double __RPC_FAR *value);


void __RPC_STUB X3DSFTime_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFTime_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Anima;

#ifdef __cplusplus

class DECLSPEC_UUID("918B202D-8E8F-4649-A70B-E9B178FEDC58")
Anima;
#endif

#ifndef __IDashboard1_DISPINTERFACE_DEFINED__
#define __IDashboard1_DISPINTERFACE_DEFINED__

/* dispinterface IDashboard1 */
/* [uuid] */ 


EXTERN_C const IID DIID_IDashboard1;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D181AEDA-E6CC-4556-8948-23D2C6222531")
    IDashboard1 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDashboard1Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDashboard1 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDashboard1 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDashboard1 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDashboard1 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDashboard1 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDashboard1 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDashboard1 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IDashboard1Vtbl;

    interface IDashboard1
    {
        CONST_VTBL struct IDashboard1Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDashboard1_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDashboard1_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDashboard1_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDashboard1_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDashboard1_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDashboard1_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDashboard1_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDashboard1_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Dashboard;

#ifdef __cplusplus

class DECLSPEC_UUID("25B54D15-EABB-4E27-B84C-2E2896368E89")
Dashboard;
#endif

EXTERN_C const CLSID CLSID_CoX3DNode;

#ifdef __cplusplus

class DECLSPEC_UUID("31042E47-EB83-4D36-BC6A-00D29006FEE5")
CoX3DNode;
#endif

EXTERN_C const CLSID CLSID_CoX3DField;

#ifdef __cplusplus

class DECLSPEC_UUID("36ECD09E-F20D-4897-959D-95761801B87D")
CoX3DField;
#endif

EXTERN_C const CLSID CLSID_CoX3DBrowser;

#ifdef __cplusplus

class DECLSPEC_UUID("F61F61CE-117D-4772-B2CB-388A2CFACA52")
CoX3DBrowser;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFBool;

#ifdef __cplusplus

class DECLSPEC_UUID("4F65CFBC-E8C1-4835-B2DB-4A59628FFBC2")
CoX3DSFBool;
#endif

EXTERN_C const CLSID CLSID_CoX3DMField;

#ifdef __cplusplus

class DECLSPEC_UUID("08F3F97A-01D7-4E0D-B4F4-46EA035E08E4")
CoX3DMField;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFColor;

#ifdef __cplusplus

class DECLSPEC_UUID("7E36E664-91FD-4702-B427-6D8083A75F05")
CoX3DMFColor;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFNode;

#ifdef __cplusplus

class DECLSPEC_UUID("6F11013E-CFA1-47A2-87F9-D491E6E108A8")
CoX3DMFNode;
#endif

#ifndef ___X3DSFTimeEvents_DISPINTERFACE_DEFINED__
#define ___X3DSFTimeEvents_DISPINTERFACE_DEFINED__

/* dispinterface _X3DSFTimeEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__X3DSFTimeEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7FCE58B1-F0BD-4696-A370-F9EF021FC3D5")
    _X3DSFTimeEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _X3DSFTimeEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _X3DSFTimeEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _X3DSFTimeEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _X3DSFTimeEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _X3DSFTimeEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _X3DSFTimeEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _X3DSFTimeEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _X3DSFTimeEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _X3DSFTimeEventsVtbl;

    interface _X3DSFTimeEvents
    {
        CONST_VTBL struct _X3DSFTimeEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _X3DSFTimeEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _X3DSFTimeEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _X3DSFTimeEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _X3DSFTimeEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _X3DSFTimeEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _X3DSFTimeEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _X3DSFTimeEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___X3DSFTimeEvents_DISPINTERFACE_DEFINED__ */


#ifndef __X3DMFFloat_INTERFACE_DEFINED__
#define __X3DMFFloat_INTERFACE_DEFINED__

/* interface X3DMFFloat */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFFloat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B4365441-8859-413F-8F16-EA0AD6E98D00")
    X3DMFFloat : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ float value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFFloatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFFloat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFFloat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFFloat __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFFloat __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFFloat __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFFloat __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFFloat __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFFloat __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFFloat __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFFloat __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ float value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFFloat __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFFloatVtbl;

    interface X3DMFFloat
    {
        CONST_VTBL struct X3DMFFloatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFFloat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFFloat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFFloat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFFloat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFFloat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFFloat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFFloat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFFloat_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFFloat_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFFloat_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFFloat_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFFloat_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFFloat_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFFloat_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFFloat_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFFloat_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFFloat_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFFloat_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFFloat_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFFloat_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFFloat_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFFloat_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFFloat_getValue_Proxy( 
    X3DMFFloat __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFFloat_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFFloat_setValue_Proxy( 
    X3DMFFloat __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFFloat_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFFloat_set1Value_Proxy( 
    X3DMFFloat __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ float value);


void __RPC_STUB X3DMFFloat_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFFloat_get1Value_Proxy( 
    X3DMFFloat __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFFloat_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFFloat_INTERFACE_DEFINED__ */


#ifndef __X3DMFInt32_INTERFACE_DEFINED__
#define __X3DMFInt32_INTERFACE_DEFINED__

/* interface X3DMFInt32 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFInt32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CE3B6FCD-8E08-4191-997A-B25DDA640AEF")
    X3DMFInt32 : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ long __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ long __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ long value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ long __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFInt32Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFInt32 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFInt32 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFInt32 __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFInt32 __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ long __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ long __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ long value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFInt32 __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ long __RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFInt32Vtbl;

    interface X3DMFInt32
    {
        CONST_VTBL struct X3DMFInt32Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFInt32_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFInt32_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFInt32_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFInt32_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFInt32_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFInt32_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFInt32_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFInt32_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFInt32_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFInt32_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFInt32_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFInt32_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFInt32_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFInt32_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFInt32_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFInt32_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFInt32_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFInt32_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFInt32_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFInt32_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFInt32_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFInt32_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFInt32_getValue_Proxy( 
    X3DMFInt32 __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ long __RPC_FAR *value);


void __RPC_STUB X3DMFInt32_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFInt32_setValue_Proxy( 
    X3DMFInt32 __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ long __RPC_FAR *value);


void __RPC_STUB X3DMFInt32_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFInt32_set1Value_Proxy( 
    X3DMFInt32 __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ long value);


void __RPC_STUB X3DMFInt32_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFInt32_get1Value_Proxy( 
    X3DMFInt32 __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ long __RPC_FAR *value);


void __RPC_STUB X3DMFInt32_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFInt32_INTERFACE_DEFINED__ */


#ifndef __X3DMFRotation_INTERFACE_DEFINED__
#define __X3DMFRotation_INTERFACE_DEFINED__

/* interface X3DMFRotation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("43D3E0EA-68EF-4923-85DA-D1D9BEF80212")
    X3DMFRotation : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ X3DSFRotation __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ X3DSFRotation __RPC_FAR *__RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFRotation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFRotation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFRotation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFRotation __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFRotation __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFRotation __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFRotation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFRotation __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFRotation __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFRotation __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DSFRotation __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFRotation __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ X3DSFRotation __RPC_FAR *__RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFRotationVtbl;

    interface X3DMFRotation
    {
        CONST_VTBL struct X3DMFRotationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFRotation_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFRotation_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFRotation_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFRotation_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFRotation_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFRotation_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFRotation_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFRotation_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFRotation_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFRotation_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFRotation_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFRotation_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFRotation_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFRotation_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFRotation_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFRotation_getValue_Proxy( 
    X3DMFRotation __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFRotation_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFRotation_setValue_Proxy( 
    X3DMFRotation __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFRotation_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFRotation_set1Value_Proxy( 
    X3DMFRotation __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DSFRotation __RPC_FAR *value);


void __RPC_STUB X3DMFRotation_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFRotation_get1Value_Proxy( 
    X3DMFRotation __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ X3DSFRotation __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DMFRotation_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFRotation_INTERFACE_DEFINED__ */


#ifndef __X3DMFString_INTERFACE_DEFINED__
#define __X3DMFString_INTERFACE_DEFINED__

/* interface X3DMFString */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("28FA5559-97BC-42CD-8E9A-B921F4B1A8CC")
    X3DMFString : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ BSTR __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ BSTR value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFString __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFString __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFString __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFString __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFString __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFString __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFString __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFString __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFString __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ BSTR value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFString __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFStringVtbl;

    interface X3DMFString
    {
        CONST_VTBL struct X3DMFStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFString_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFString_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFString_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFString_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFString_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFString_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFString_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFString_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFString_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFString_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFString_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFString_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFString_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFString_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFString_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFString_getValue_Proxy( 
    X3DMFString __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DMFString_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFString_setValue_Proxy( 
    X3DMFString __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DMFString_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFString_set1Value_Proxy( 
    X3DMFString __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ BSTR value);


void __RPC_STUB X3DMFString_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFString_get1Value_Proxy( 
    X3DMFString __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB X3DMFString_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFString_INTERFACE_DEFINED__ */


#ifndef __X3DMFVec2f_INTERFACE_DEFINED__
#define __X3DMFVec2f_INTERFACE_DEFINED__

/* interface X3DMFVec2f */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFVec2f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7C4602A2-F24C-40F0-BA32-92C9AECEB11E")
    X3DMFVec2f : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ X3DSFVec2f __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ X3DSFVec2f __RPC_FAR *__RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFVec2fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFVec2f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFVec2f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFVec2f __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFVec2f __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DSFVec2f __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFVec2f __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ X3DSFVec2f __RPC_FAR *__RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFVec2fVtbl;

    interface X3DMFVec2f
    {
        CONST_VTBL struct X3DMFVec2fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFVec2f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFVec2f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFVec2f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFVec2f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFVec2f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFVec2f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFVec2f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFVec2f_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFVec2f_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFVec2f_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFVec2f_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFVec2f_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFVec2f_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFVec2f_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFVec2f_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFVec2f_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFVec2f_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFVec2f_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFVec2f_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFVec2f_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFVec2f_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFVec2f_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec2f_getValue_Proxy( 
    X3DMFVec2f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFVec2f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec2f_setValue_Proxy( 
    X3DMFVec2f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFVec2f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec2f_set1Value_Proxy( 
    X3DMFVec2f __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DSFVec2f __RPC_FAR *value);


void __RPC_STUB X3DMFVec2f_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec2f_get1Value_Proxy( 
    X3DMFVec2f __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ X3DSFVec2f __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DMFVec2f_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFVec2f_INTERFACE_DEFINED__ */


#ifndef __X3DMFVec3f_INTERFACE_DEFINED__
#define __X3DMFVec3f_INTERFACE_DEFINED__

/* interface X3DMFVec3f */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DMFVec3f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42B550D1-A010-4DA8-9538-ECD651B35F02")
    X3DMFVec3f : public X3DMField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ X3DSFVec3f __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ X3DSFVec3f __RPC_FAR *__RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DMFVec3fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DMFVec3f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DMFVec3f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DMFVec3f __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DMFVec3f __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ VARIANT __RPC_FAR *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ X3DSFVec3f __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            X3DMFVec3f __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ X3DSFVec3f __RPC_FAR *__RPC_FAR *value);
        
        END_INTERFACE
    } X3DMFVec3fVtbl;

    interface X3DMFVec3f
    {
        CONST_VTBL struct X3DMFVec3fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DMFVec3f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DMFVec3f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DMFVec3f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DMFVec3f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DMFVec3f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DMFVec3f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DMFVec3f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DMFVec3f_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DMFVec3f_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DMFVec3f_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DMFVec3f_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DMFVec3f_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DMFVec3f_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DMFVec3f_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DMFVec3f_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define X3DMFVec3f_get_length(This,pVal)	\
    (This)->lpVtbl -> get_length(This,pVal)

#define X3DMFVec3f_put_length(This,newVal)	\
    (This)->lpVtbl -> put_length(This,newVal)

#define X3DMFVec3f_Item(This,index,pItem)	\
    (This)->lpVtbl -> Item(This,index,pItem)


#define X3DMFVec3f_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define X3DMFVec3f_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define X3DMFVec3f_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#define X3DMFVec3f_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec3f_getValue_Proxy( 
    X3DMFVec3f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFVec3f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec3f_setValue_Proxy( 
    X3DMFVec3f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB X3DMFVec3f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec3f_set1Value_Proxy( 
    X3DMFVec3f __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ X3DSFVec3f __RPC_FAR *value);


void __RPC_STUB X3DMFVec3f_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DMFVec3f_get1Value_Proxy( 
    X3DMFVec3f __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ X3DSFVec3f __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB X3DMFVec3f_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DMFVec3f_INTERFACE_DEFINED__ */


#ifndef __X3DSFDouble_INTERFACE_DEFINED__
#define __X3DSFDouble_INTERFACE_DEFINED__

/* interface X3DSFDouble */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DSFDouble;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2B0EB163-01B0-42A9-9979-F5E969306D32")
    X3DSFDouble : public X3DField
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getValue( 
            /* [out][in] */ double __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ double value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DSFDoubleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DSFDouble __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DSFDouble __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DSFDouble __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attach )( 
            X3DSFDouble __RPC_FAR * This,
            X3DExecutionContext __RPC_FAR *context,
            void __RPC_FAR *pNode,
            void __RPC_FAR *pField,
            void __RPC_FAR *mid,
            long valueType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addFieldEventListener )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeFieldEventListener )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *listener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAccessType )( 
            X3DSFDouble __RPC_FAR * This,
            /* [retval][out] */ X3DFieldAccess __RPC_FAR *accesstype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            X3DSFDouble __RPC_FAR * This,
            /* [retval][out] */ X3DFieldType __RPC_FAR *fieldtype);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DSFDouble __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dispose )( 
            X3DSFDouble __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            X3DSFDouble __RPC_FAR * This,
            /* [out][in] */ double __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            X3DSFDouble __RPC_FAR * This,
            /* [in] */ double value);
        
        END_INTERFACE
    } X3DSFDoubleVtbl;

    interface X3DSFDouble
    {
        CONST_VTBL struct X3DSFDoubleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DSFDouble_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DSFDouble_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DSFDouble_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DSFDouble_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DSFDouble_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DSFDouble_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DSFDouble_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DSFDouble_attach(This,context,pNode,pField,mid,valueType)	\
    (This)->lpVtbl -> attach(This,context,pNode,pField,mid,valueType)

#define X3DSFDouble_addFieldEventListener(This,listener)	\
    (This)->lpVtbl -> addFieldEventListener(This,listener)

#define X3DSFDouble_removeFieldEventListener(This,listener)	\
    (This)->lpVtbl -> removeFieldEventListener(This,listener)

#define X3DSFDouble_getAccessType(This,accesstype)	\
    (This)->lpVtbl -> getAccessType(This,accesstype)

#define X3DSFDouble_getType(This,fieldtype)	\
    (This)->lpVtbl -> getType(This,fieldtype)

#define X3DSFDouble_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define X3DSFDouble_dispose(This)	\
    (This)->lpVtbl -> dispose(This)


#define X3DSFDouble_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#define X3DSFDouble_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFDouble_getValue_Proxy( 
    X3DSFDouble __RPC_FAR * This,
    /* [out][in] */ double __RPC_FAR *value);


void __RPC_STUB X3DSFDouble_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DSFDouble_setValue_Proxy( 
    X3DSFDouble __RPC_FAR * This,
    /* [in] */ double value);


void __RPC_STUB X3DSFDouble_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DSFDouble_INTERFACE_DEFINED__ */


#ifndef __IFluxPlayer_INTERFACE_DEFINED__
#define __IFluxPlayer_INTERFACE_DEFINED__

/* interface IFluxPlayer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IFluxPlayer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("47DEE776-2909-45E7-BDBB-7E9EC092A290")
    IFluxPlayer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AboutBox( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_src( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_src( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setFluxControl( 
            void __RPC_FAR *pFluxCtrl) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFluxPlayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFluxPlayer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFluxPlayer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFluxPlayer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFluxPlayer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFluxPlayer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFluxPlayer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFluxPlayer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox )( 
            IFluxPlayer __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_src )( 
            IFluxPlayer __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_src )( 
            IFluxPlayer __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setFluxControl )( 
            IFluxPlayer __RPC_FAR * This,
            void __RPC_FAR *pFluxCtrl);
        
        END_INTERFACE
    } IFluxPlayerVtbl;

    interface IFluxPlayer
    {
        CONST_VTBL struct IFluxPlayerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFluxPlayer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFluxPlayer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFluxPlayer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFluxPlayer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFluxPlayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFluxPlayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFluxPlayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFluxPlayer_AboutBox(This)	\
    (This)->lpVtbl -> AboutBox(This)

#define IFluxPlayer_get_src(This,pVal)	\
    (This)->lpVtbl -> get_src(This,pVal)

#define IFluxPlayer_put_src(This,newVal)	\
    (This)->lpVtbl -> put_src(This,newVal)

#define IFluxPlayer_setFluxControl(This,pFluxCtrl)	\
    (This)->lpVtbl -> setFluxControl(This,pFluxCtrl)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFluxPlayer_AboutBox_Proxy( 
    IFluxPlayer __RPC_FAR * This);


void __RPC_STUB IFluxPlayer_AboutBox_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFluxPlayer_get_src_Proxy( 
    IFluxPlayer __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IFluxPlayer_get_src_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFluxPlayer_put_src_Proxy( 
    IFluxPlayer __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFluxPlayer_put_src_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFluxPlayer_setFluxControl_Proxy( 
    IFluxPlayer __RPC_FAR * This,
    void __RPC_FAR *pFluxCtrl);


void __RPC_STUB IFluxPlayer_setFluxControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFluxPlayer_INTERFACE_DEFINED__ */


#ifndef __X3DExternalBrowser_INTERFACE_DEFINED__
#define __X3DExternalBrowser_INTERFACE_DEFINED__

/* interface X3DExternalBrowser */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_X3DExternalBrowser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("62357248-742F-4BC6-84BF-BF622D5EAD02")
    X3DExternalBrowser : public X3DBrowser
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE beginUpdate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE endUpdate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct X3DExternalBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getVersion )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentSpeed )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentFrameRate )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWorldURL )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *replaceWorld )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ X3DScene __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadURL )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ int nUrls,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int nParams,
            /* [size_is][in] */ BSTR __RPC_FAR *params);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setDescription )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ BSTR description);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createX3DFromString )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ BSTR x3dSource,
            /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createX3DFromURL )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ BSTR url,
            /* [retval][out] */ X3DScene __RPC_FAR *__RPC_FAR *scenereturn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setClosure )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getClosure )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ void __RPC_FAR *closure);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *print )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ BSTR str);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadUrlS )( 
            X3DExternalBrowser __RPC_FAR * This,
            BSTR url);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExecutionContext )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ X3DExecutionContext __RPC_FAR *__RPC_FAR *ppContext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBrowserListener )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pListener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBrowserListener )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pListener);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *firstViewpoint )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lastViewpoint )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *nextViewpoint )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *previousViewpoint )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_viewpoints )( 
            X3DExternalBrowser __RPC_FAR * This,
            /* [retval][out] */ X3DNodeArray __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *beginUpdate )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *endUpdate )( 
            X3DExternalBrowser __RPC_FAR * This);
        
        END_INTERFACE
    } X3DExternalBrowserVtbl;

    interface X3DExternalBrowser
    {
        CONST_VTBL struct X3DExternalBrowserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define X3DExternalBrowser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define X3DExternalBrowser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define X3DExternalBrowser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define X3DExternalBrowser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define X3DExternalBrowser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define X3DExternalBrowser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define X3DExternalBrowser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define X3DExternalBrowser_getName(This,value)	\
    (This)->lpVtbl -> getName(This,value)

#define X3DExternalBrowser_getVersion(This,value)	\
    (This)->lpVtbl -> getVersion(This,value)

#define X3DExternalBrowser_getCurrentSpeed(This,value)	\
    (This)->lpVtbl -> getCurrentSpeed(This,value)

#define X3DExternalBrowser_getCurrentFrameRate(This,value)	\
    (This)->lpVtbl -> getCurrentFrameRate(This,value)

#define X3DExternalBrowser_getWorldURL(This,value)	\
    (This)->lpVtbl -> getWorldURL(This,value)

#define X3DExternalBrowser_replaceWorld(This,value)	\
    (This)->lpVtbl -> replaceWorld(This,value)

#define X3DExternalBrowser_loadURL(This,nUrls,url,nParams,params)	\
    (This)->lpVtbl -> loadURL(This,nUrls,url,nParams,params)

#define X3DExternalBrowser_setDescription(This,description)	\
    (This)->lpVtbl -> setDescription(This,description)

#define X3DExternalBrowser_createX3DFromString(This,x3dSource,scenereturn)	\
    (This)->lpVtbl -> createX3DFromString(This,x3dSource,scenereturn)

#define X3DExternalBrowser_createX3DFromURL(This,url,scenereturn)	\
    (This)->lpVtbl -> createX3DFromURL(This,url,scenereturn)

#define X3DExternalBrowser_setClosure(This,closure)	\
    (This)->lpVtbl -> setClosure(This,closure)

#define X3DExternalBrowser_getClosure(This,closure)	\
    (This)->lpVtbl -> getClosure(This,closure)

#define X3DExternalBrowser_print(This,str)	\
    (This)->lpVtbl -> print(This,str)

#define X3DExternalBrowser_loadUrlS(This,url)	\
    (This)->lpVtbl -> loadUrlS(This,url)

#define X3DExternalBrowser_getExecutionContext(This,ppContext)	\
    (This)->lpVtbl -> getExecutionContext(This,ppContext)

#define X3DExternalBrowser_addBrowserListener(This,pListener)	\
    (This)->lpVtbl -> addBrowserListener(This,pListener)

#define X3DExternalBrowser_removeBrowserListener(This,pListener)	\
    (This)->lpVtbl -> removeBrowserListener(This,pListener)

#define X3DExternalBrowser_firstViewpoint(This)	\
    (This)->lpVtbl -> firstViewpoint(This)

#define X3DExternalBrowser_lastViewpoint(This)	\
    (This)->lpVtbl -> lastViewpoint(This)

#define X3DExternalBrowser_nextViewpoint(This)	\
    (This)->lpVtbl -> nextViewpoint(This)

#define X3DExternalBrowser_previousViewpoint(This)	\
    (This)->lpVtbl -> previousViewpoint(This)

#define X3DExternalBrowser_get_viewpoints(This,pVal)	\
    (This)->lpVtbl -> get_viewpoints(This,pVal)


#define X3DExternalBrowser_beginUpdate(This)	\
    (This)->lpVtbl -> beginUpdate(This)

#define X3DExternalBrowser_endUpdate(This)	\
    (This)->lpVtbl -> endUpdate(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExternalBrowser_beginUpdate_Proxy( 
    X3DExternalBrowser __RPC_FAR * This);


void __RPC_STUB X3DExternalBrowser_beginUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE X3DExternalBrowser_endUpdate_Proxy( 
    X3DExternalBrowser __RPC_FAR * This);


void __RPC_STUB X3DExternalBrowser_endUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __X3DExternalBrowser_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CoX3DSFTime;

#ifdef __cplusplus

class DECLSPEC_UUID("8C7DB996-4067-4E0B-831A-380D1C3B22B0")
CoX3DSFTime;
#endif

EXTERN_C const CLSID CLSID_CoX3DEventOutObserver;

#ifdef __cplusplus

class DECLSPEC_UUID("5DCC9CD7-5A07-4E96-9406-8F08FCE28F1B")
CoX3DEventOutObserver;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFColor;

#ifdef __cplusplus

class DECLSPEC_UUID("288B5BD0-18B5-4DC0-A43A-9FF0BDF10FD8")
CoX3DSFColor;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFFloat;

#ifdef __cplusplus

class DECLSPEC_UUID("9A6FF18F-2F67-4D38-930C-57417472BD9B")
CoX3DSFFloat;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFInt32;

#ifdef __cplusplus

class DECLSPEC_UUID("682C3B9D-7462-4583-9AC7-1B8426EF7FC6")
CoX3DSFInt32;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFNode;

#ifdef __cplusplus

class DECLSPEC_UUID("31812207-62D7-42C0-96C1-A2CAF2FB89F6")
CoX3DSFNode;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFRotation;

#ifdef __cplusplus

class DECLSPEC_UUID("6FE68A4E-CDA6-4F66-BCF0-1532DE56CE89")
CoX3DSFRotation;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFString;

#ifdef __cplusplus

class DECLSPEC_UUID("84DBDFA8-9F78-44DB-AB52-66C60022B521")
CoX3DSFString;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFVec2f;

#ifdef __cplusplus

class DECLSPEC_UUID("56C77CB8-F7AC-47EA-83EF-A10E54F77747")
CoX3DSFVec2f;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFVec3f;

#ifdef __cplusplus

class DECLSPEC_UUID("8A40D936-57A8-4991-AAD8-BFCCB0877E9C")
CoX3DSFVec3f;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFFloat;

#ifdef __cplusplus

class DECLSPEC_UUID("71979F5B-1E40-495A-ADA9-D351F014B37B")
CoX3DMFFloat;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFInt32;

#ifdef __cplusplus

class DECLSPEC_UUID("90A72B6D-6161-4030-A491-76F3168A3E86")
CoX3DMFInt32;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFRotation;

#ifdef __cplusplus

class DECLSPEC_UUID("7AA35B65-2B21-4996-8C4B-F86871A8F7A4")
CoX3DMFRotation;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFString;

#ifdef __cplusplus

class DECLSPEC_UUID("11916947-30F6-4B01-B071-C7F9C99C6628")
CoX3DMFString;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFVec2f;

#ifdef __cplusplus

class DECLSPEC_UUID("0ACB238B-5F48-4CF9-953D-223B91311E00")
CoX3DMFVec2f;
#endif

EXTERN_C const CLSID CLSID_CoX3DMFVec3f;

#ifdef __cplusplus

class DECLSPEC_UUID("99C98869-477E-4EDD-A5DB-760797484E22")
CoX3DMFVec3f;
#endif

EXTERN_C const CLSID CLSID_CoX3DSFDouble;

#ifdef __cplusplus

class DECLSPEC_UUID("435362C0-FE67-4FF2-B2CB-0809349956C5")
CoX3DSFDouble;
#endif

EXTERN_C const CLSID CLSID_FluxPlayer;

#ifdef __cplusplus

class DECLSPEC_UUID("45ADF2D9-B311-4758-9220-F8D6D582D2D7")
FluxPlayer;
#endif

EXTERN_C const CLSID CLSID_CoX3DExecutionContext;

#ifdef __cplusplus

class DECLSPEC_UUID("563E697A-6954-422B-A82D-D61568522765")
CoX3DExecutionContext;
#endif

EXTERN_C const CLSID CLSID_CoX3DScene;

#ifdef __cplusplus

class DECLSPEC_UUID("03B74F4E-8BE5-453E-A870-92E1A3066B1F")
CoX3DScene;
#endif

EXTERN_C const CLSID CLSID_CoX3DBrowserListener;

#ifdef __cplusplus

class DECLSPEC_UUID("AF56D457-7DF6-4F5D-95E9-72CCC8F8A792")
CoX3DBrowserListener;
#endif

EXTERN_C const CLSID CLSID_CoX3DExternalBrowser;

#ifdef __cplusplus

class DECLSPEC_UUID("6A04E913-B549-4F17-98AE-8808D7C36E30")
CoX3DExternalBrowser;
#endif

EXTERN_C const CLSID CLSID_CoX3DFieldEventListener;

#ifdef __cplusplus

class DECLSPEC_UUID("DAB90B62-3B19-47ED-AC2D-F71063DAF907")
CoX3DFieldEventListener;
#endif

EXTERN_C const CLSID CLSID_CoX3DRoute;

#ifdef __cplusplus

class DECLSPEC_UUID("EE3BDCFD-4156-4D11-8227-F8BE60213837")
CoX3DRoute;
#endif

EXTERN_C const CLSID CLSID_CoX3DNodeArray;

#ifdef __cplusplus

class DECLSPEC_UUID("57C5C0F8-555E-4111-AA96-F9F747256B46")
CoX3DNodeArray;
#endif

EXTERN_C const CLSID CLSID_CoX3DFieldDefinition;

#ifdef __cplusplus

class DECLSPEC_UUID("61C1DA98-3B82-41F8-8327-6002920D2697")
CoX3DFieldDefinition;
#endif

EXTERN_C const CLSID CLSID_CoX3DFieldDefinitionArray;

#ifdef __cplusplus

class DECLSPEC_UUID("7C1F3AF8-C47D-4D55-9F25-64B9972C1126")
CoX3DFieldDefinitionArray;
#endif

EXTERN_C const CLSID CLSID_CoX3DProtoDeclaration;

#ifdef __cplusplus

class DECLSPEC_UUID("A8866F8B-7974-4109-A0E3-F1023F6C4BFA")
CoX3DProtoDeclaration;
#endif

EXTERN_C const CLSID CLSID_CoX3DProtoDeclarationArray;

#ifdef __cplusplus

class DECLSPEC_UUID("6EFD51B3-C0D4-4B54-B3E1-66DCA59404E9")
CoX3DProtoDeclarationArray;
#endif
#endif /* __ANIMALib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
