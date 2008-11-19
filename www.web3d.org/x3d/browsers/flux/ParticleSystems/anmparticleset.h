/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmparticleset.h
 *		Description:	Anima ParticleSet node
 *
 *
 *
 * (C) 2001-2004 by Media Machines
 *
 *		Revision history:
 *			$Log: anmparticleset.h,v $
 *			Revision 1.1.1.1  2003/09/10 02:18:12  turk
 *			build466
 *			
 *
 ******************************************************************/

#ifndef _anmparticleset_h
#define _anmparticleset_h

#include "anmgeometry.h"

class CAnmParticleSet  : public CAnmGeometry
{

	CAnmNode				*m_color;
	CAnmNode				*m_coord;
	FloatArray				*m_size;
	bool					 m_primsupdated;
	Point3					 m_prevcamerapos;
	Point3					 m_prevcameradir;
	Point3					 m_prevcameraup;
	class CAnmMesh			*m_noalphamesh;

	virtual void CreateMesh();

	virtual void UpdatePrimitives(class CAnmDevice *pDevice, class CAnmCamera *pCamera,
						  const matrix4 &worldmat, class CAnmAlphaBuffer *pAlphaBuffer = NULL);

	virtual void UpdatePrimitive(class CAnmTriangleList *pTriangleList, class CAnmTriangleList *pNoAlphaTriangleList,
		class CAnmDevice *pDevice, class CAnmCamera *pCamera, const matrix4 &bbmat,
		Point3 *pCoords, int nCoords, float *pSizes, int nSizes, Point3 &camerapos, const matrix4 &worldmat,
		float pixelSize, class CAnmAlphaBuffer *pAlphaBuffer = NULL);

	virtual void UpdatePrimitive(class CAnmTriangleList *pTriangleList, class CAnmTriangleList *pNoAlphaTriangleList,
		class CAnmDevice *pDevice, CAnmCamera *pCamera, class CAnmOctree *pOctree, Point3 *pCoords, Point3 *bbvec,
		Point3 &camerapos, const matrix4 &worldmat, float pixelSize, class CAnmAlphaBuffer *pAlphaBuffer = NULL);

public:

	CAnmParticleSet();
	~CAnmParticleSet();

	// CAnmGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);
	virtual void ChildChanged(CAnmNode *pChild);

	virtual eAnmRenderOptions GetRenderOptions();

	virtual void Render(class CAnmDevice *pDevice, int texturestage, class CAnmCamera *pCamera,
						  const matrix4 &worldmat);

	// New methods


	// Accessors
	void SetColor(CAnmNode *pColor);
	class CAnmNode *GetColor() { return m_color; }
	void GetColor(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_color;
	}

	void SetCoord(CAnmNode *pCoordinate);
	class CAnmNode *GetCoord() { return m_coord; }
	void GetCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_coord;
	}
	
	void SetSize(FloatArray *pSize);
	FloatArray *GetSize() { return m_size; }
	void GetSize(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_size;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmParticleSet);

	DECLARE_VALUE(color);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(size);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_size);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(size_changed);
};

#define ANMPARTICLESET_COLORDIRTY				ANMGEOMETRY_NDIRTYBITS
#define ANMPARTICLESET_COORDDIRTY				(ANMPARTICLESET_COLORDIRTY + 1)
#define ANMPARTICLESET_SIZEDIRTY				(ANMPARTICLESET_COLORDIRTY + 2)

typedef enum eAnmParticleSetDirtyFlags {
eAnmParticleSetColorDirty				= ANMDIRTYBIT(ANMPARTICLESET_COLORDIRTY),
eAnmParticleSetCoordDirty				= ANMDIRTYBIT(ANMPARTICLESET_COORDDIRTY),
eAnmParticleSetSizeDirty				= ANMDIRTYBIT(ANMPARTICLESET_SIZEDIRTY),
} eAnmParticleSetDirtyFlags;

#endif // _anmparticleset_h
