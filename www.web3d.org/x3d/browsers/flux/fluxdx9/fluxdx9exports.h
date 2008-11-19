/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			fluxdx9exports.h
 *		Description:	DLL Export help for Flux
 *
 * (C) 2001 by Tony Parisi
 *
 *		Revision history:
 *			$Log: fluxdx8exports.h,v $
 *			Revision 1.2  2004/05/20 17:26:09  Tony
 *			View/render model
 *			
 *			Revision 1.1.1.1  2003/09/10 02:18:11  turk
 *			build466
 *			
 *
 *******************************************************************/

#ifndef _fluxdx9exports_h
#define _fluxdx9exports_h

#ifdef EXPORT_FLUXDX9
#define EXPORT DLL_EXPORT
#else
#define EXPORT DLL_IMPORT
#endif // EXPORT_FLUXDX9

#endif // _fluxdx9exports_h




