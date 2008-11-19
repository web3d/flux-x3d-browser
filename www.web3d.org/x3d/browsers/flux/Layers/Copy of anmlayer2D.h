/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmlayer2D.h
 *		Description:	Anima Layer2D node - grouping node for 2D layer
 *
 * (C) 2001-2005 by Media Machines
 *
 *		Revision history:
 *			$Log: anmlayer2D.h,v $
 *
 *******************************************************************/

#ifndef _anmlayer2D_h
#define _anmlayer2D_h


#define ANMLAYER2D_DEFAULT_TRANSPARENCY						(0.0f)
#define ANMLAYER2D_DEFAULT_TYPE								("FOREGROUND")
#define ANMLAYER2D_DEFAULT_LAYOUT							( new CAnmLayout )

#include "anmlayer.h"


class CAnmLayer2D : public CAnmLayer
{
protected : 

	float							m_scaleToPixelFactor;

	// Fields

	CAnmNode*						m_layout;
	Float							m_transparency;
	

	float m_WidthInRealUnits;
	float m_HeightInRealUnits;
	float m_WidthInPixels;
	float m_HeightInPixels;
	
	
	// helpers
	virtual void RecalculateCameraParams(class CAnmDevice *pDevice);

public :

	// Constructor/destructor
	CAnmLayer2D();
	virtual ~CAnmLayer2D();

	// CAnmTransform overrides
	virtual void Realize();

	virtual void ChildChanged(CAnmNode *pChild);
	virtual void ParentChanged(class CAnmGroup *pParent);

	// Accessors

	float GetWidthInRealUnits(){ return m_WidthInRealUnits; }
	float GetHeightInRealUnits(){ return m_HeightInRealUnits; }
	float GetWidthInPixels(){ return m_WidthInPixels; }
	float GetHeightInPixels(){ return m_HeightInPixels; }


	void SetTransparency(Float transparency);
	Float GetTransparency() { return m_transparency; }
	void GetTransparency(Float *pVal)
	{
		if (pVal)
			*pVal = m_transparency;
	}

	void SetLayout(CAnmNode* pLayout);
	CAnmNode* GetLayout() { return m_layout; }
	void GetLayout(CAnmNode**pVal)
	{
		if (pVal)
			*pVal = m_layout;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmLayer2D);

	DECLARE_VALUE(children);



	DECLARE_VALUE(layout);
	DECLARE_VALUE(transparency);
	DECLARE_VALUE(type);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_children);

	DECLARE_METHOD(set_layout);
	DECLARE_METHOD(set_transparency);
	DECLARE_METHOD(set_type);


	DECLARE_CALLBACK(children_changed);

	DECLARE_CALLBACK(layout_changed);
	DECLARE_CALLBACK(transparency_changed);
	DECLARE_CALLBACK(type_changed);
};

DECLARE_GETIMPLEMENTATION(CAnmLayer2D);


#endif // _anmlayer2D_h







/**
Layer2D : X3DLayerNode {
  MFNode   [in]     addChildren     []            [X3DChildNode]
  MFNode   [in]     removeChildren  []            [X3DChildNode]
  MFNode   [in,out] children        []            [X3DChildNode]
  SFNode   [in,out] metadata        NULL          [X3DMetadataObject]

  SFNode   [in,out] layout          NULL          [X3DLayoutNode]
  SFFloat   [in,out] transparency 0             [0..1]
  SFString [in,out] type            "FOREGROUND" ["BACKGROUND","FOREGROUND"]


}
**/
