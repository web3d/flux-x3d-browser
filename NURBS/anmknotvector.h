/********************************************************************************
 * Flux
 *
 * File: anmknotvector.h
 * Description: Nurbs support class
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

#ifndef _anmknotvector_h
#define _anmknotvector_h

#define MAXBASIS 50
#define UVEPSILON  0.0001
#define NURBZERO	0.000001

class  CAnmKnotVector 
{

protected:

 
public:

	// constructor/destructor
	CAnmKnotVector();
	virtual ~CAnmKnotVector();


	int GetnKnots() const { return ( m_pKnot ) ? m_pKnot->size() : 0 ; }
	int GetOrder() const { return m_iOrder; }

	double GetKnotHead() const { return ( ( GetnKnots() ) ? ( (*m_pKnot)[0] ) : 0.0 ); }
	double GetKnotTail() const { return ( ( GetnKnots() ) ? ( (*m_pKnot)[GetnKnots()-1] ) : 1.0 ); }
	double GetKnotStart() const { return GetKnotValue( m_iOrder + 0 ); }
	double GetKnotEnd() const { return GetKnotValue( GetnKnots()-m_iOrder-1 ); }

	
	double GetKnotValue ( int i ) const { return ( ( i>=0 && i<GetnKnots() ) ? ( (*m_pKnot)[i] ) : 0.0 ); }

	bool IsValid( int nDimension, bool bDontGenerate = false );

	void MakeUniformKnots( int nuKnots );

	void SetOrder( int iorder ){ m_iOrder = iorder; }
	void SetKnotVector( DoubleArray* pKnot);

	int FindNurbsSpan( double u );
	void CalcNurbsBasis( int &i, double u, Double *dBasis );

protected:

	int m_iOrder;
	DoubleArray* m_pKnot;

	DoubleArray*	m_pGeneratedKnotVector;


};


#endif // _anmknotvector_h
