/********************************************************************************
 * Flux
 *
 * File: anmfilereader.h
 * Description: Abstract file reader class
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

#ifndef _anmfilereader_h
#define _anmfilereader_h


#define VALUEARRAY_ALLOC	128		// for SF values
#define FLOATARRAY_ALLOC	256
#define INTARRAY_ALLOC		(FLOATARRAY_ALLOC * 4)
#define ROTARRAY_ALLOC		128
#define STRINGARRAY_ALLOC	128
#define DOUBLEARRAY_ALLOC	FLOATARRAY_ALLOC
#define NODEARRAY_ALLOC		128
#define BOOLEANARRAY_ALLOC	128

class CAnmFileReader
{
protected :

	class CAnmFileLoader		*m_loader;

	// Field type support
	// Single-fields storage allocation
	BooleanArray				 m_booleanvalues;
	ColorArray					 m_colorvalues;
	DoubleArray					 m_doublevalues;
	FloatArray					 m_floatvalues;
	IntegerArray				 m_integervalues;
	RotationArray				 m_rotationvalues;
	StringArray					 m_stringvalues;
	TimeArray					 m_timevalues;
	Point2Array					 m_point2values;
	Point3Array					 m_point3values;

	// Multi-fields storage allocation

	BooleanArray				*m_booleanarray;
	ColorArray					*m_colorarray;
	DoubleArray					*m_doublearray;
	FloatArray					*m_floatarray;
	Image						*m_image;
	IntegerArray				*m_intarray;
	RotationArray				*m_rotationarray;
	StringArray					*m_stringarray;
	TimeArray					*m_timearray;
	Point2Array					*m_point2array;
	Point3Array					*m_point3array;

	virtual void WriteToConsole(char *str);

public :

	CAnmFileReader(class CAnmFileLoader *pLoader);
	virtual ~CAnmFileReader();

	virtual eAnmReturnStatus Read(const char *source, bool fromFile)
		PURE ;

	virtual int GetLineNumber()
		PURE ;
};


#endif // _anmfilereader_h
