/********************************************************************************
 * Flux
 *
 * File: anmfields.h
 * Description: Field classes -
 *							polymorphic data for object fields
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

#ifndef _anmfields_h
#define _anmfields_h

#include "anmnode.h"


#define SAFEREF( ptr ) { if( ptr ){ ptr->Ref(); } }

class EXPORT CAnmField : public CAnmObject
{
protected :

	CAnmNode				*m_node;
	CLASSMEMBERID			 m_mid;
	eValueType				 m_valueType;

	void CallValueChanged()
	{
		Boolean b = false;
		CallCallbacks(CALLBACKID(CAnmField, value_changed), &b);
	}

public :

	CAnmField()
	{
		m_valueType = eValueBadType;
		m_node = NULL;
		m_mid = NULL;
	}

	CAnmField(eValueType valueType)
	{
		m_valueType = valueType;
		m_node = NULL;
		m_mid = NULL;
	}

	void SetType(eValueType valueType)
	{
		m_valueType = valueType;
	}

	eValueType GetType()
	{
		return m_valueType;
	}

	void SetNode(CAnmNode *pNode)
	{
		m_node = pNode;
	}
	CAnmNode *GetNode()
	{
		return m_node;
	}

	void SetMemberID(CLASSMEMBERID mid)
	{
		m_mid = mid;
	}

	CLASSMEMBERID GetMemberID()
	{
		return m_mid;
	}

	virtual void SetData(void *pData) PURE;
	virtual void *GetData() PURE;
	virtual void Copy(void *pData) PURE;
	virtual void UpdateFromNode() PURE;

	static CAnmField *CreateField(eValueType valueType, void *pData);
	static CAnmField *CreateField(CAnmNode *pNode, CLASSMEMBERID mid);

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmField);

	DECLARE_CALLBACK(value_changed);
};

class EXPORT CAnmMField : public CAnmField
{
protected :

public :

	CAnmMField() : CAnmField()
	{
	}

	CAnmMField(eValueType valueType) : CAnmField(valueType)
	{
	}

	virtual int GetSize() PURE;

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMField);
};

class EXPORT CAnmSFBool : public CAnmField
{
protected :

	Boolean				m_boolval;

public :

	CAnmSFBool() : CAnmField(eValueBoolean)
	{
		m_boolval = FALSE;
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Boolean *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Boolean *) pData = m_boolval;
	}

	virtual void SetValue(Boolean b)
	{
		m_boolval = b;
		CallValueChanged();
	}

	virtual Boolean *GetValue()
	{
		return &m_boolval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_boolval);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFBool);
};

class EXPORT CAnmMFBool : public CAnmMField
{
protected :

	BooleanArray			*m_boolarray;

public :

	CAnmMFBool() : CAnmMField(eValueBooleanArray)
	{
		m_boolarray = new BooleanArray;
	}

	virtual ~CAnmMFBool()
	{
		SafeUnRef(m_boolarray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(BooleanArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (BooleanArray **) pData = m_boolarray;
		SAFEREF( m_boolarray );
	}

	virtual void SetValue(BooleanArray *pBa)
	{
		SafeUnRef(m_boolarray);
		if (pBa)
		{
			m_boolarray = pBa;
			SAFEREF( m_boolarray );
		}
		else
		{
			pBa = new BooleanArray ;
		}

		CallValueChanged();
	}

	virtual BooleanArray **GetValue()
	{
		return &m_boolarray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_boolarray);
			m_node->GetValue(m_mid, (void **) &m_boolarray);
			SAFEREF( m_boolarray );
		}
	}

	virtual int GetSize()
	{
		assert(m_boolarray);
		return m_boolarray->size();
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFBool);
};

class EXPORT CAnmSFColor : public CAnmField
{
protected :

	Color				m_colorval;

public :

	CAnmSFColor() : CAnmField(eValueColor)
	{
		m_colorval = Color(0, 0, 0);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Color *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Color *) pData = m_colorval;
	}

	virtual void SetValue(Color c)
	{
		m_colorval = c;
		CallValueChanged();
	}

	virtual Color *GetValue()
	{
		return &m_colorval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_colorval);
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFColor);
};

class EXPORT CAnmMFColor : public CAnmMField
{
protected :

	ColorArray			*m_colorarray;

public :

	CAnmMFColor() : CAnmMField(eValueColorArray)
	{
		m_colorarray = new ColorArray;
	}

	virtual ~CAnmMFColor()
	{
		SafeUnRef(m_colorarray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(ColorArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (ColorArray **) pData = m_colorarray;
		SAFEREF( m_colorarray );
	}

	virtual void SetValue(ColorArray *pCa)
	{
		SafeUnRef(m_colorarray);
		if (pCa)
		{
			m_colorarray = pCa;
			SAFEREF( m_colorarray );
		}
		else
		{
			pCa = new ColorArray ;
		}

		CallValueChanged();
	}

	virtual ColorArray **GetValue()
	{
		return &m_colorarray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_colorarray);
			m_node->GetValue(m_mid, (void **) &m_colorarray);
			SAFEREF( m_colorarray );
		}
	}

	virtual int GetSize()
	{
		assert(m_colorarray);
		return m_colorarray->size();
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFColor);
};

class EXPORT CAnmSFDouble : public CAnmField
{
protected :

	Double				m_doubleval;

public :

	CAnmSFDouble() : CAnmField(eValueDouble)
	{
		m_doubleval = 0.f;
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Double *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Double *) pData = m_doubleval;
	}

	virtual void SetValue(Double d)
	{
		m_doubleval = d;
		CallValueChanged();
	}

	virtual Double *GetValue()
	{
		return &m_doubleval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_doubleval);
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFDouble);
};

class EXPORT CAnmMFDouble : public CAnmMField
{
protected :

	DoubleArray			*m_doublearray;

public :

	CAnmMFDouble() : CAnmMField(eValueDoubleArray)
	{
		m_doublearray = new DoubleArray;
	}

	virtual ~CAnmMFDouble()
	{
		SafeUnRef(m_doublearray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(DoubleArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (DoubleArray **) pData = m_doublearray;
		SAFEREF( m_doublearray );
	}

	virtual void SetValue(DoubleArray *pDa)
	{
		SafeUnRef(m_doublearray);
		if (pDa)
		{
			m_doublearray = pDa;
			SAFEREF( m_doublearray );
		}
		else
		{
			pDa = new DoubleArray ;
		}

		CallValueChanged();
	}

	virtual DoubleArray **GetValue()
	{
		return &m_doublearray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_doublearray);
			m_node->GetValue(m_mid, (void **) &m_doublearray);
			SAFEREF( m_doublearray );
		}
	}

	virtual int GetSize()
	{
		assert(m_doublearray);
		return m_doublearray->size();
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFDouble);
};


class EXPORT CAnmSFFloat : public CAnmField
{
protected :

	Float				m_floatval;

public :

	CAnmSFFloat() : CAnmField(eValueFloat)
	{
		m_floatval = 0.f;
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Float *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Float *) pData = m_floatval;
	}

	virtual void SetValue(Float f)
	{
		m_floatval = f;
		CallValueChanged();
	}

	virtual Float *GetValue()
	{
		return &m_floatval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_floatval);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFFloat);
};

class EXPORT CAnmMFFloat : public CAnmMField
{
protected :

	FloatArray			*m_floatarray;

public :

	CAnmMFFloat() : CAnmMField(eValueFloatArray)
	{
		m_floatarray = new FloatArray;
	}

	virtual ~CAnmMFFloat()
	{
		SafeUnRef(m_floatarray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(FloatArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (FloatArray **) pData = m_floatarray;
		SAFEREF( m_floatarray );
	}

	virtual void SetValue(FloatArray *pFa)
	{
		SafeUnRef(m_floatarray);
		if (pFa)
		{
			m_floatarray = pFa;
			SAFEREF( m_floatarray );
		}
		else
		{
			pFa = new FloatArray ;
		}

		CallValueChanged();
	}

	virtual FloatArray **GetValue()
	{
		return &m_floatarray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_floatarray);
			m_node->GetValue(m_mid, (void **) &m_floatarray);
			SAFEREF( m_floatarray );
		}
	}

	virtual int GetSize()
	{
		assert(m_floatarray);
		return m_floatarray->size();
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFFloat);
};

class EXPORT CAnmSFImage : public CAnmField
{
protected :

	Image				m_imageval;

public :

	CAnmSFImage() : CAnmField(eValueImage)
	{
		assert(FALSE);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Image *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Image *) pData = m_imageval;
	}

	virtual void SetValue(Image i)
	{
		m_imageval = i;
		CallValueChanged();
	}

	virtual Image *GetValue()
	{
		return &m_imageval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_imageval);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFImage);
};

class EXPORT CAnmMFImage : public CAnmMField
{
protected :

	ImageArray			*m_imagearray;

public :

	CAnmMFImage() : CAnmMField(eValueImageArray)
	{
		m_imagearray = new ImageArray;
	}

	virtual ~CAnmMFImage()
	{
		SafeUnRef(m_imagearray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(ImageArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (ImageArray **) pData = m_imagearray;
		SAFEREF( m_imagearray );
	}

	virtual void SetValue(ImageArray *pIa)
	{
		SafeUnRef(m_imagearray);
		if (pIa)
		{
			m_imagearray = pIa;
			SAFEREF( m_imagearray );
		}
		else
		{
			pIa = new ImageArray ;
		}

		CallValueChanged();
	}

	virtual ImageArray **GetValue()
	{
		return &m_imagearray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_imagearray);
			m_node->GetValue(m_mid, (void **) &m_imagearray);
			SAFEREF( m_imagearray );
		}
	}

	virtual int GetSize()
	{
		assert(m_imagearray);
		return m_imagearray->size();
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFImage);
};


class EXPORT CAnmSFInt32 : public CAnmField
{
protected :

	Integer				m_intval;

public :

	CAnmSFInt32() : CAnmField(eValueInteger)
	{
		m_intval = 0;
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Integer *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Integer *) pData = m_intval;
	}

	virtual void SetValue(Integer i)
	{
		m_intval = i;
		CallValueChanged();
	}

	virtual Integer *GetValue()
	{
		return &m_intval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_intval);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFInt32);
};

class EXPORT CAnmMFInt32 : public CAnmMField
{
protected :

	IntegerArray			*m_intarray;

public :

	CAnmMFInt32() : CAnmMField(eValueIntegerArray)
	{
		m_intarray = new IntegerArray;
	}

	virtual ~CAnmMFInt32()
	{
		SafeUnRef(m_intarray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(IntegerArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (IntegerArray **) pData = m_intarray;
		SAFEREF( m_intarray );
	}

	virtual void SetValue(IntegerArray *pIa)
	{
		SafeUnRef(m_intarray);
		if (pIa)
		{
			m_intarray = pIa;
			SAFEREF( m_intarray );
		}
		else
		{
			pIa = new IntegerArray ;
		}

		CallValueChanged();
	}

	virtual IntegerArray **GetValue()
	{
		return &m_intarray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_intarray);
			m_node->GetValue(m_mid, (void **) &m_intarray);
			SAFEREF( m_intarray );
		}
	}

	virtual int GetSize()
	{
		assert(m_intarray);
		return m_intarray->size();
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFInt32);
};

class EXPORT CAnmSFNode : public CAnmField
{
protected :

	Node				m_nodeval;

public :

	CAnmSFNode() : CAnmField(eValueNode)
	{
		m_nodeval = NULL;
	}

	~CAnmSFNode()
	{
		SafeUnRef(m_nodeval);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Node *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Node *) pData = m_nodeval;
	}

	virtual void SetValue(Node n)
	{
		SafeUnRef(m_nodeval);
		m_nodeval = n;
		if (n)
			SAFEREF( n );
		CallValueChanged();
	}

	virtual Node *GetValue()
	{
		return &m_nodeval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_nodeval);
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFNode);
};

class EXPORT CAnmMFNode : public CAnmMField
{
protected :

	NodeArray			*m_nodearray;

public :

	CAnmMFNode() : CAnmMField(eValueNodeArray)
	{
		m_nodearray = new NodeArray;
	}

	virtual ~CAnmMFNode()
	{
		SafeUnRef(m_nodearray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(NodeArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (NodeArray **) pData = m_nodearray;
		SAFEREF( m_nodearray );
	}

	virtual void SetValue(NodeArray *pNa)
	{
		SafeUnRef(m_nodearray);
		if (pNa)
		{
			m_nodearray = pNa;
			SAFEREF( m_nodearray );
		}
		else
		{
			pNa = new NodeArray ;
		}

		CallValueChanged();
	}

	virtual NodeArray **GetValue()
	{
		return &m_nodearray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_nodearray);
			m_node->GetValue(m_mid, (void **) &m_nodearray);
			SAFEREF( m_nodearray );
		}
	}

	virtual int GetSize()
	{
		assert(m_nodearray);
		return m_nodearray->size();
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFNode);
};

class EXPORT CAnmSFRotation : public CAnmField
{
protected :

	Rotation				m_rotval;

public :

	CAnmSFRotation() : CAnmField(eValueRotation)
	{
		m_rotval = Rotation(0, 0, 1, 0);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Rotation *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Rotation *) pData = m_rotval;
	}

	virtual void SetValue(Rotation r)
	{
		m_rotval = r;
		CallValueChanged();
	}

	virtual Rotation *GetValue()
	{
		return &m_rotval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_rotval);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFRotation);
};

class EXPORT CAnmMFRotation : public CAnmMField
{
protected :

	RotationArray			*m_rotarray;

public :

	CAnmMFRotation() : CAnmMField(eValueRotationArray)
	{
		m_rotarray = new RotationArray;
	}

	virtual ~CAnmMFRotation()
	{
		SafeUnRef(m_rotarray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(RotationArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (RotationArray **) pData = m_rotarray;
		SAFEREF( m_rotarray );
	}

	virtual void SetValue(RotationArray *pRa)
	{
		SafeUnRef(m_rotarray);
		if (pRa)
		{
			m_rotarray = pRa;
			SAFEREF( m_rotarray );
		}
		else
		{
			pRa = new RotationArray ;
		}

		CallValueChanged();
	}

	virtual RotationArray **GetValue()
	{
		return &m_rotarray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_rotarray);
			m_node->GetValue(m_mid, (void **) &m_rotarray);
			SAFEREF( m_rotarray );
		}
	}

	virtual int GetSize()
	{
		assert(m_rotarray);
		return m_rotarray->size();
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFRotation);
};

class EXPORT CAnmSFString : public CAnmField
{
protected :

	String				m_strval;

public :

	CAnmSFString() : CAnmField(eValueString)
	{
		m_strval = new CAnmString("");
	}

	~CAnmSFString()
	{
		SafeUnRef(m_strval);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(String *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (String *) pData = m_strval;
	}

	virtual void SetValue(String s)
	{
		SafeUnRef(m_strval);

		m_strval = s;
		SAFEREF( m_strval );

		CallValueChanged();
	}

	virtual String *GetValue()
	{
		return &m_strval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_strval);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFString);
};

class EXPORT CAnmMFString : public CAnmMField
{
protected :

	StringArray			*m_strarray;

public :

	CAnmMFString() : CAnmMField(eValueStringArray)
	{
		m_strarray = new StringArray;
	}

	virtual ~CAnmMFString()
	{
		SafeUnRef(m_strarray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(StringArray **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (StringArray **) pData = m_strarray;
		SAFEREF( m_strarray );
	}

	virtual void SetValue(StringArray *pSa)
	{
		SafeUnRef(m_strarray);
		if (pSa)
		{
			m_strarray = pSa;
			SAFEREF( m_strarray );
		}
		else
		{
			pSa = new StringArray ;
		}

		CallValueChanged();
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_strarray);
			m_node->GetValue(m_mid, (void **) &m_strarray);
			SAFEREF( m_strarray );
		}
	}

	virtual StringArray **GetValue()
	{
		return &m_strarray;
	}

	virtual int GetSize()
	{
		assert(m_strarray);
		return m_strarray->size();
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFString);
};

class EXPORT CAnmSFTime : public CAnmField
{
protected :

	Time				m_timeval;

public :

	CAnmSFTime() : CAnmField(eValueTime)
	{
		m_timeval = (Time) -1;
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Time *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Time *) pData = m_timeval;
	}

	virtual void SetValue(Time t)
	{
		m_timeval = t;
		CallValueChanged();
	}

	virtual Time *GetValue()
	{
		return &m_timeval;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_timeval);
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFTime);
};

class EXPORT CAnmMFTime : public CAnmMField
{
protected :

	TimeArray			*m_timearray;

public :

	CAnmMFTime() : CAnmMField(eValueTimeArray)
	{
		m_timearray = new TimeArray;
	}

	virtual ~CAnmMFTime()
	{
		SafeUnRef(m_timearray);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(TimeArray **) pData);
	}

	virtual void Copy(void *pData)
	{
		* (TimeArray **) pData = m_timearray;
		SAFEREF( m_timearray );
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void SetValue(TimeArray *pTa)
	{
		SafeUnRef(m_timearray);
		if (pTa)
		{
			m_timearray = pTa;
			SAFEREF( m_timearray );
		}
		else
		{
			pTa = new TimeArray ;
		}

		CallValueChanged();
	}

	virtual TimeArray **GetValue()
	{
		return &m_timearray;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_timearray);
			m_node->GetValue(m_mid, (void **) &m_timearray);
			SAFEREF( m_timearray );
		}
	}

	virtual int GetSize()
	{
		assert(m_timearray);
		return m_timearray->size();
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFTime);
};

class EXPORT CAnmSFVec2f : public CAnmField
{
protected :

	Point2				m_p2val;

public :

	CAnmSFVec2f() : CAnmField(eValuePoint2)
	{
		m_p2val = Point2(0, 0);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Point2 *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Point2 *) pData = m_p2val;
	}

	virtual void SetValue(Point2 p2)
	{
		m_p2val = p2;
		CallValueChanged();
	}

	virtual Point2 *GetValue()
	{
		return &m_p2val;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_p2val);
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFVec2f);
};

class EXPORT CAnmMFVec2f : public CAnmMField
{
protected :

	Point2Array			*m_p2array;

public :

	CAnmMFVec2f() : CAnmMField(eValuePoint2Array)
	{
		m_p2array = new Point2Array;
	}

	virtual ~CAnmMFVec2f()
	{
		SafeUnRef(m_p2array);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Point2Array **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Point2Array **) pData = m_p2array;
		SAFEREF( m_p2array );
	}

	virtual void SetValue(Point2Array *pP2a)
	{
		SafeUnRef(m_p2array);
		if (pP2a)
		{
			m_p2array = pP2a;
			SAFEREF( m_p2array );
		}
		else
		{
			pP2a = new Point2Array ;
		}

		CallValueChanged();
	}

	virtual Point2Array **GetValue()
	{
		return &m_p2array;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_p2array);
			m_node->GetValue(m_mid, (void **) &m_p2array);
			SAFEREF( m_p2array );
		}
	}

	virtual int GetSize()
	{
		assert(m_p2array);
		return m_p2array->size();
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFVec2f);
};


class EXPORT CAnmSFVec3f : public CAnmField
{
protected :

	Point3				m_p3val;

public :

	CAnmSFVec3f() : CAnmField(eValuePoint3)
	{
		m_p3val = Point3(0, 0, 0);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Point3 *) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Point3 *) pData = m_p3val;
	}

	virtual void SetValue(Point3 p3)
	{
		m_p3val = p3;
		CallValueChanged();
	}

	virtual Point3 *GetValue()
	{
		return &m_p3val;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
			m_node->GetValue(m_mid, (void **) &m_p3val);
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSFVec3f);
};

class EXPORT CAnmMFVec3f : public CAnmMField
{
protected :

	Point3Array			*m_p3array;

public :

	CAnmMFVec3f() : CAnmMField(eValuePoint3Array)
	{
		m_p3array = new Point3Array;
	}

	virtual ~CAnmMFVec3f()
	{
		SafeUnRef(m_p3array);
	}

	virtual void SetData(void *pData)
	{
		SetValue( *(Point3Array **) pData);
	}

	virtual void *GetData()
	{
		return (void *) GetValue();
	}

	virtual void Copy(void *pData)
	{
		* (Point3Array **) pData = m_p3array;
		SAFEREF( m_p3array );
	}

	virtual void SetValue(Point3Array *pP3a)
	{
		SafeUnRef(m_p3array);
		if (pP3a)
		{
			m_p3array = pP3a;
			SAFEREF( m_p3array );
		}
		else
		{
			pP3a = new Point3Array ;
		}

		CallValueChanged();
	}

	virtual Point3Array **GetValue()
	{
		return &m_p3array;
	}

	virtual void UpdateFromNode()
	{
		if (m_node)
		{
			SafeUnRef(m_p3array);
			m_node->GetValue(m_mid, (void **) &m_p3array);
			SAFEREF( m_p3array );
		}
	}

	virtual int GetSize()
	{
		assert(m_p3array);
		return m_p3array->size();
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMFVec3f);
};

#endif // _anmfields_h