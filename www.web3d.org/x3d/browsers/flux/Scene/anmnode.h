/********************************************************************************
 * Flux
 *
 * File: anmnode.h
 * Description: Base node class
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

#ifndef _anmnode_h
#define _anmnode_h

#include "anmrenderobject.h"
#include "anmvolumes.h"


// Dirty bit macros
#define ANMMAX_DIRTYBITS (sizeof(ULONG) / sizeof(BYTE) * 8)
#define ANMDIRTYBIT(_n_) (1 << _n_)

#define ANMNODE_STATEDIRTY						0
#define ANMNODE_CHILDSTATEDIRTY					1
#define ANMNODE_MATRIXDIRTY						2
#define ANMNODE_CHILDMATRIXDIRTY				3
#define ANMNODE_CHILDRENDIRTY					4
#define ANMNODE_CHILDCHILDRENDIRTY				5
#define ANMNODE_INSTANCEDIRTY					6
#define ANMNODE_CHILDINSTANCEDIRTY				7
#define ANMNODE_NDIRTYBITS						(ANMNODE_CHILDINSTANCEDIRTY + 1)

#define ABORT_IF_WORLD_IS_NULL						{ if( pWorld == NULL ) { return; } }
#define ABORT_IF_WORLD_IS_NULL_RTN_NULL				{ if( pWorld == NULL ) { return NULL; } }
#define ABORT_IF_WORLD_IS_NULL_UNLOCK				{ if( pWorld == NULL ) { Unlock(); return; } }

typedef enum eAnmNodeDirtyBits {
	eAnmNodeStateDirty			= ANMDIRTYBIT(ANMNODE_STATEDIRTY),
	eAnmNodeChildStateDirty		= ANMDIRTYBIT(ANMNODE_CHILDSTATEDIRTY),
	eAnmNodeMatrixDirty			= ANMDIRTYBIT(ANMNODE_MATRIXDIRTY),
	eAnmNodeChildMatrixDirty	= ANMDIRTYBIT(ANMNODE_CHILDMATRIXDIRTY),
	eAnmNodeChildrenDirty		= ANMDIRTYBIT(ANMNODE_CHILDRENDIRTY),
	eAnmNodeChildChildrenDirty	= ANMDIRTYBIT(ANMNODE_CHILDCHILDRENDIRTY),
	eAnmNodeInstanceDirty		= ANMDIRTYBIT(ANMNODE_INSTANCEDIRTY),
	eAnmNodeChildInstanceDirty	= ANMDIRTYBIT(ANMNODE_CHILDINSTANCEDIRTY),
} eAnmNodeDirtyBits;

typedef std::list<class CAnmGroup *> tAnmGroupList;

class CAnmNodePath
{
protected :

	std::vector<class CAnmGroup *>	m_path;

public :

	void Push(class CAnmGroup *pGroup)
	{
		m_path.push_back(pGroup);
	}

	void Pop()
	{
		int sz = m_path.size();

		if (sz <= 0)
			return;

		m_path.resize(sz - 1);
	}

	inline CAnmNodePath &operator=(const CAnmNodePath &p)
	{
		m_path.clear();

		int sz = p.m_path.size();

		for (int i = 0; i < sz; i++)
			m_path.push_back(p.m_path[i]);

		return *this;
	}

	inline bool operator== (const CAnmNodePath &p)
	{
		int sz = m_path.size();
		int psz = p.m_path.size();

		if (sz != psz)
			return false;

		for (int i = 0; i < sz; i++)
		{
			if (m_path[i] != p.m_path[i])
				return false;
		}

		return true;
	}
};

class CAnmNodeInstance
{
protected :

	CAnmRenderObject	*m_renderobject;
	CAnmNodePath		 m_nodepath;

public :

	CAnmNodeInstance(CAnmRenderObject *pRenderObject, const CAnmNodePath &p)
	{
		m_renderobject = pRenderObject;
		m_nodepath = p;
	}

	~CAnmNodeInstance()
	{
		SafeDelete(m_renderobject);
	}

	bool PathIsEqual(const CAnmNodePath &p)
	{
		return m_nodepath == p;
	}

	CAnmRenderObject *GetRenderObject()
	{
		return m_renderobject;
	}

	void SetPath(const CAnmNodePath &p)
	{
		m_nodepath = p;
	}
};


class CAnmNode : public CAnmObject
{

protected:

	tAnmGroupList			 m_parents;
	BOOL					 m_realized;
	bool					 m_active;
	String					 m_name;
	ULONG					 m_dirtyBits;
	std::deque<class CAnmEvent *>
							 m_eventQueue;

	std::vector<CAnmNodeInstance *> m_instances;
	int						 m_instanceIndex;

	CAnmNode				*m_metadata;

	virtual CAnmRenderObject *CurrentInstance()
	{
		if (m_instanceIndex <= 0)
			return NULL;

		int sz = m_instances.size();
		if (m_instanceIndex > sz)
			return NULL;

		CAnmNodeInstance *pInst = m_instances[m_instanceIndex-1];
		if (pInst)
			return pInst->GetRenderObject();
		else
			return NULL;
	}

	virtual CAnmRenderObject *GetInstance(int index)
	{
		assert(index >= 0);

		int sz = m_instances.size();
		if (index >= sz)
			return NULL;

		CAnmNodeInstance *pInst = m_instances[index];

		if (pInst)
			return pInst->GetRenderObject();
		else
			return NULL;
	}

	virtual CAnmRenderObject *FindInstance(CAnmNodePath &p)
	{
		int sz = m_instances.size();
		if (sz <= 0)
			return NULL;

		for (int index = 0; index < sz; index++)
		{
			CAnmNodeInstance *pInst = m_instances[index];

			if (pInst && pInst->PathIsEqual(p))
				return pInst->GetRenderObject();
		}
		
		return NULL;
	}

	virtual void AddInstance(CAnmRenderObject *pRenderObject, const CAnmNodePath &p)
	{
		CAnmNodeInstance *pInst = new CAnmNodeInstance(pRenderObject, p);

		m_instances.push_back(pInst);
	}

	template <class _ROBJTYPE_> _ROBJTYPE_ *AddOrCreateInstance(_ROBJTYPE_ *&ppInst, int index, const CAnmNodePath &p)
	{
		_ROBJTYPE_ *pInst = (_ROBJTYPE_ *) GetInstance(index);
		if (pInst)
		{
			m_instances[index]->SetPath(p);
			return pInst;
		}
		else
		{
			pInst = new _ROBJTYPE_;
			pInst->SetUserData(this);
			AddInstance(pInst, p);
			return pInst;
		}
	}

	template <class _ROBJTYPE_> _ROBJTYPE_ *NextInstance(_ROBJTYPE_ *&ppInst, const CAnmNodePath &p)
	{
		return AddOrCreateInstance(ppInst, m_instanceIndex++, p);
	}

	virtual void PropagateDirtyBitsUpward(ULONG dirtyBits);
	virtual void PropagateDirtyBitsDownward(ULONG dirtyBits);

	friend class CAnmGroup;

public:

	// constructor/destructor
	CAnmNode();
	virtual ~CAnmNode();

	// New methods

	// Building the scene graph
	virtual void Realize();
	virtual BOOL Realized()
	{
		return m_realized;
	}
	virtual void Update();				// re-render/reset rendering structures

	// Scene graph structure
	virtual class CAnmWorld *GetWorld();
	virtual class CAnmScene *GetContainingScene();
	virtual class CAnmProto *GetProtoContext();
	virtual class CAnmLayer *GetLayer();

	virtual Boolean IsGroup()
	{
		return FALSE;
	}

	virtual Boolean IsAncestor(CAnmNode *pTestChild)
	{
		return FALSE;
	}

	virtual Boolean IsDescendant(class CAnmGroup *pTestParent);

	virtual void AddParent(class CAnmGroup *pParent);
	virtual void RemoveParent(class CAnmGroup *pParent);

	virtual void ParentChanged(class CAnmGroup *pParent);
	virtual void NotifyParentsOfChange();

	virtual tAnmGroupList *GetParents() { return &m_parents; }
	virtual class CAnmGroup *GetFirstParent();
	virtual class CAnmTransform *GetFirstTransformParent();

	// Traversal and rendering
	virtual void PreTraverse();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PostTraverse();
	virtual void TestPick(class CAnmDevice *pDevice, Point3 from, Point3 to, 
		struct sAnmSceneGraphPickList *pPickList)
	{
	}

	virtual void SetActive(bool active);

	virtual bool GetActive()
	{
		return m_active;
	}

	// Event support
	virtual void PostEvent(class CAnmEvent *pEvent);
	virtual BOOL HandleEvents();
	virtual eAnmReturnStatus HandleOneEvent(class CAnmEvent *pEvent);

	// Proto support
	// Note SetImplementationNode not defined here, it's unnecessary
	virtual CAnmNode *GetImplementationNode()
	{
		return this;
	}

	// Dirty state management
	virtual void SetDirtyBits(ULONG bits)
	{
		m_dirtyBits |= bits;
	}
	
	virtual void ClearDirtyBits(ULONG bits)
	{
		m_dirtyBits &= ~bits;
	}

	virtual void ClearAllDirtyBits()
	{
		m_dirtyBits = 0L;
	}

#if 0
	virtual BOOL IsDirty()	// flag something changed
	{
		return m_dirtyBits != 0L;
	}
#endif

	virtual ULONG TestDirtyBits(ULONG bits)
	{
		return m_dirtyBits & bits;
	}

	virtual ULONG GetDirtyBits()
	{
		return m_dirtyBits;
	}

	virtual ULONG StateDirty()
	{
		return TestDirtyBits(eAnmNodeStateDirty);
	}

	virtual void ClearStateDirty()
	{
		ClearDirtyBits(eAnmNodeStateDirty);
	}

	virtual void SetMatrixDirty();
	virtual void SetStateDirty();
	virtual void SetStateAndMatrixDirty();
	virtual void SetInstanceDirty();
	
	// Bounding volumes etc.
	virtual float GetRadius()
	{
		return 0.f;
	}

	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		CAnmBoundingBox bbox;
		*pBoundingBox = bbox;
	}

	virtual void GetObjectCenter(Point3 *pCenter)
	{
		assert(FALSE);
	}

	// Poly counts
	virtual long GetPolyCount()
	{
		return 0L;
	}

	// Url support
	virtual void SetBaseUrl(String baseUrl)
	{
	}

	virtual String GetBaseUrl()
	{
		return new CAnmString("");
	}

	// Accessors

	virtual void SetName(String name)
	{
		SafeUnRef(m_name);
		m_name = name;
		m_name->Ref();
	}

	virtual String GetName()
	{
		return m_name;
	}

	virtual int NumInstances()
	{
		return m_instances.size();
	}

	void SetMetadata(CAnmNode *pMetadata);
	class CAnmNode *GetMetadata() { return m_metadata; }
	void GetMetadata(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_metadata;
	}

	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		return false;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmNode);

	DECLARE_VALUE(metadata);

	DECLARE_METHOD(set_metadata);

	DECLARE_CALLBACK(metadata_changed);
};

// N.B.: Whacky metadata stuff; EVERY node needs those fields, so let's make some
// macros that take the sting out of it

#define DECLARE_METADATA_MEMBERS \
	DECLARE_VALUE(metadata); \
	DECLARE_METHOD(set_metadata); \
	DECLARE_CALLBACK(metadata_changed);

#define DEFINE_METADATA_MEMBERS(_cls_) \
	DEFINE_VALUE(_cls_, metadata, eValueNode, CAnmNode*, GetMetadata, SetMetadata); \
	DEFINE_METHOD(_cls_, set_metadata, eValueNode, CAnmNode*, SetMetadata); \
	DEFINE_CALLBACK(_cls_, metadata_changed, eValueNode);

#define NODE_METADATA_IDS(_cls_) \
	VALUEID(_cls_, metadata), \
	METHODID(_cls_, set_metadata), \
	CALLBACKID(_cls_, metadata_changed), \

#define DECLARE_NODECLASS(_cls_) \
	DECLARE_CLASS(_cls_); \
	DECLARE_METADATA_MEMBERS

#define BEGIN_DEFINE_NODE_MEMBERS(_cls_) \
	DEFINE_METADATA_MEMBERS(_cls_); \
	BEGIN_DEFINE_MEMBERS(_cls_) \
	NODE_METADATA_IDS(_cls_)


#endif // _anmnode_h
