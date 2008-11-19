/********************************************************************************
 * Flux
 *
 * File: anmlayout.h
 * Description: Anima Layout node
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


/**************
X3DLayoutNode : X3DNode
{
  SFVec2f   [in,out]  offset         0,0             [-inf..+inf]
  SFVec2f   [in,out]  size           0,0             [0..+inf]

  MFString  [in,out]  offsetUnits    "WORLD","WORLD" ["WORLD","FRACTION","PIXEL"]
  MFString  [in,out]  sizeUnits      "WORLD","WORLD" ["WORLD","FRACTION","PIXEL"]

  MFString  [in,out]  align          "MIDDLE" ["TOP","MIDDLE","BOTTOM"]
  MFString  [in,out]  scaleMode      "NONE" ["NONE" "PIXEL" "FRACTION" "STRETCH"]

	
	  
		
		  
  SFString  [in,out]  uAlign         "CENTER" ["LEFT","CENTER","RIGHT"]
  SFString  [in,out]  vAlign         "MIDDLE" ["TOP","MIDDLE","BOTTOM"]

  MFBool    [in,out]  scale        FALSE
  MFBool    [in,out]  stretch      FALSE
}
****************/


 
#ifndef _anmlayout_h
#define _anmlayout_h

#include "anmnode.h"


enum eUnitsMode {
	eUnitsWorld,
	eUnitsFraction,
	eUnitsPixel,
};

enum euAlignmentMode {
	euAlignmentLeft,
	euAlignmentCenter,
	euAlignmentRight,
};

enum evAlignmentMode {
	evAlignmentBottom,
	evAlignmentMiddle,
	evAlignmentTop,
};


enum eScaleMode {
	eScaleModeNone,
	eScaleModePixel,
	eScaleModeFraction,
	eScaleModeStretch,
};



#define ANMLAYOUT_DEFAULT_OFFSET							Point2( 0.0f, 0.0f )
#define ANMLAYOUT_DEFAULT_SIZE								Point2( 1.0f, 1.0f )
#define ANMLAYOUT_DEFAULT_OFFSETUNITS						("WORLD")
#define ANMLAYOUT_DEFAULT_SIZEUNITS							("WORLD")
#define ANMLAYOUT_DEFAULT_UALIGNMENT						("CENTER")
#define ANMLAYOUT_DEFAULT_VALIGNMENT						("MIDDLE")
#define ANMLAYOUT_DEFAULT_SCALE_MODE						("NONE")
#define ANMLAYOUT_DEFAULT_OFFSETUNITSMODE					eUnitsWorld
#define ANMLAYOUT_DEFAULT_SIZEUNITSMODE						eUnitsWorld
#define ANMLAYOUT_DEFAULT_UALIGNMENTMODE					euAlignmentCenter
#define ANMLAYOUT_DEFAULT_VALIGNMENTMODE					evAlignmentMiddle
#define ANMLAYOUT_DEFAULT_SCALE_MODEMODE					eScaleModeNone


class CAnmLayout : public CAnmNode
{
protected :


	// Fields
	Point2							m_offset;
	Point2							m_size;

	StringArray*					m_offsetUnits;
	StringArray*					m_sizeUnits;

	StringArray*					m_align;
	StringArray*					m_scaleMode;

public:

	// constructor/destructor
	CAnmLayout();
	virtual ~CAnmLayout();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures

	// New methods
	static euAlignmentMode GetuAlignmentMode( String str );
	static evAlignmentMode GetvAlignmentMode( String str );
	static eUnitsMode GetUnitsMode( String str );
	static eScaleMode GetScaleMode( String str );

	// Accessors

 	void SetOffset(Point2 pOffset);
	Point2 GetOffset() { return m_offset; }
	void GetOffset(Point2 *pVal)
	{ 
		if (pVal)
			*pVal = m_offset;
	}

 	void SetOffsetUnits(StringArray *pOffsetUnits);
	StringArray *GetOffsetUnits() { return m_offsetUnits; }
	void GetOffsetUnits(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_offsetUnits;
	}

 	void SetSize(Point2 pSize);
	Point2 GetSize() { return m_size; }
	void GetSize(Point2 *pVal)
	{ 
		if (pVal)
			*pVal = m_size;
	}

 	void SetSizeUnits(StringArray *pSizeUnits);
	StringArray *GetSizeUnits() { return m_sizeUnits; }
	void GetSizeUnits(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_sizeUnits;
	}




 	void SetAlign(StringArray * align);
	StringArray * GetAlign() { return m_align; }
	void GetAlign(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_align;
	}

 	void SetScaleMode(StringArray * scaleMode);
	StringArray * GetScaleMode() { return m_scaleMode; }
	void GetScaleMode(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_scaleMode;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmLayout);

	DECLARE_VALUE(offset);
	DECLARE_VALUE(size);
	DECLARE_VALUE(offsetUnits);
	DECLARE_VALUE(sizeUnits);

	DECLARE_VALUE(align);
	DECLARE_VALUE(scaleMode);

	DECLARE_CALLBACK(offset_changed);
	DECLARE_CALLBACK(size_changed);
	DECLARE_CALLBACK(offsetUnits_changed);
	DECLARE_CALLBACK(sizeUnits_changed);
	DECLARE_CALLBACK(align_changed);
	DECLARE_CALLBACK(scaleMode_changed);

	DECLARE_METHOD(set_offset);
	DECLARE_METHOD(set_size);
	DECLARE_METHOD(set_offsetUnits);
	DECLARE_METHOD(set_sizeUnits);
	DECLARE_METHOD(set_align);
	DECLARE_METHOD(set_scaleMode);
};

DECLARE_GETIMPLEMENTATION(CAnmLayout);

#endif //_anmlayout_h

