/********************************************************************************
 * Flux
 *
 * File: anminterpolator.h
 * Description: Base Interpolator class
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

#ifndef _anminterpolator_h
#define _anminterpolator_h

#include "anmnodedefs.h"
#include "anmtween.h"
#include <vector>

#define DEFAULT_NUM_KEYS 3		// typical small animation

template <class _T> class CAnmInterpolator 
{
	
typedef CAnmArray<_T> _TArray;

protected:

	FloatArray					*m_key;
	_TArray						*m_keyValue;


	// Added for Spline Interpolators
	//
	_TArray						*m_keyVelocity;
	_TArray						*m_keyVelocityOut;
	_TArray						*m_keyVelocityIn;
	bool						m_closed;
	bool						m_bKeysIsDirty;
	_T							m_Zero;

public:

	// constructor/destructor

	CAnmInterpolator()
	{
		m_closed = false;
		m_bKeysIsDirty = true;
		m_keyVelocity = NULL;
		m_keyVelocityIn = NULL;
		m_keyVelocityOut = NULL;
		m_key = new FloatArray;
		m_keyValue = new _TArray;
		m_key->reserve(DEFAULT_NUM_KEYS);
		m_keyValue->reserve(DEFAULT_NUM_KEYS);
	}

	virtual ~CAnmInterpolator()
	{
		SafeUnRef(m_key);
		SafeUnRef(m_keyValue);
		SafeUnRef(m_keyVelocity);
		SafeUnRef(m_keyVelocityIn);
		SafeUnRef(m_keyVelocityOut);
	}

	// New methods
	void Interp(Float fraction, _T *pResult)
	{

		int i;
		Float delta, ratio;

		int numKeys = m_key->size();
		if (numKeys > m_keyValue->size())
			numKeys = m_keyValue->size();

		if (numKeys)
		{
			for(i = 0; i < numKeys; i++)
			{
				if( fraction < (*m_key)[i])
					break;
			}

			if( i==0 )
			{
				*pResult = (*m_keyValue)[0];
			}
			else if ( i == numKeys )
			{
				*pResult = (*m_keyValue)[numKeys - 1];
			}
			else
			{
				delta = (*m_key)[i] - (*m_key)[i-1];
				if( delta == 0.f)
					ratio = 0.f;
				else
					ratio = (fraction - (*m_key)[i-1]) / delta;

				Tween(ratio, &(*m_keyValue)[i-1], &(*m_keyValue)[i], pResult);

			}
		}
	}


	void RecalcSplineVelocityVectors()
	{
		SafeUnRef(m_keyVelocityIn);
		SafeUnRef(m_keyVelocityOut);

		int nKeys = m_key->size();
		if (nKeys > m_keyValue->size()) {
			nKeys = m_keyValue->size();
		}

		int iPrev, iNext;

		if( nKeys > 1 ) {


			m_keyVelocityIn = new _TArray;
			m_keyVelocityIn->reserve(nKeys);
			m_keyVelocityOut = new _TArray;
			m_keyVelocityOut->reserve(nKeys);

			_TArray* pKeyVelocityUse = NULL;

			bool bClosed = m_closed;

			if( nKeys < 3 ) {
				bClosed = false;
			}

			if( m_keyVelocity &&
				m_keyValue &&
				m_keyVelocity->size() == m_keyValue->size() ) {
				// the velocities are fully defined, so use them.
				//
				pKeyVelocityUse = m_keyVelocity;
				pKeyVelocityUse->Ref();
				bClosed = true;
			}
			else {

				_TArray* pEndVelocities = NULL;
				if( m_keyVelocity &&
					m_keyValue &&
					m_keyVelocity->size() == 2 ) {
					pEndVelocities = m_keyVelocity;
				}

				// Generate our new array.
				//
				pKeyVelocityUse = new _TArray;
				pKeyVelocityUse->reserve(nKeys);


				// get and verify closed flag.
				//
				if( bClosed &&
						!( (*m_keyValue)[0] == (*m_keyValue)[nKeys-1]  ) ) {
					bClosed = false;
				}
				if( pEndVelocities != NULL ) {
					bClosed = true;
				}

				for( int iKey=0; iKey<nKeys; iKey++ ) {
					if( iKey == 0 && pEndVelocities != NULL ) {
						pKeyVelocityUse->push_back( (*pEndVelocities)[0] );
					}
					else if( iKey == nKeys-1 && pEndVelocities != NULL ) {
						pKeyVelocityUse->push_back( (*pEndVelocities)[1] );
					}
					else {
						iPrev = iKey-1;
						iNext = iKey+1;
						if( iPrev < 0 ) { iPrev = nKeys-2; }
						if( iNext > nKeys-1 ) { iNext = 1; }
						pKeyVelocityUse->push_back( 0.5f * ( (*m_keyValue)[iNext] - (*m_keyValue)[iPrev] ) );
					}
				}
			}

			// Now generate the pair of Velocity Vectors with non-uniform intervals taken into account.
			//

			float fOut, fIn, dt, dtNext, dtPrev;
			for( int iKey=0; iKey<nKeys; iKey++ ) {
				fOut = fIn = 0.0f;
				if( !( ( iKey == 0 || iKey == nKeys-1 ) && !bClosed ) ) {

					iPrev = iKey-1;
					iNext = iKey+1;

					// If we wrap around from one back to zero, we need to offset our time spans.
					//
					float fPrevOff = 0.0f;
					float fNextOff = 0.0f;

					// Check wrap on Previous Key.
					//
					if( iPrev < 0 ) { 
						iPrev = nKeys-2; 
						fPrevOff = 1.0f;
					}
					// Check wrap on Next Key.
					//
					if( iNext > nKeys-1 ) { 
						iNext = 1; 
						fNextOff = 1.0f;
					}


					dt = fPrevOff + fNextOff + (*m_key)[iNext] - (*m_key)[iPrev];
					dtNext = fNextOff + (*m_key)[iNext] - (*m_key)[iKey ];
					dtPrev = fPrevOff + (*m_key)[iKey ] - (*m_key)[iPrev];
					if( dt > 1.0e-20 ) {
						fOut = 2.0f * dtPrev / dt;
						fIn  = 2.0f * dtPrev / dt;
					}
				}
				m_keyVelocityOut->push_back( fOut * ( *pKeyVelocityUse )[iKey] );
				m_keyVelocityIn->push_back ( fIn  * ( *pKeyVelocityUse )[iKey] );


			}


			SafeUnRef(pKeyVelocityUse);

		}
	}


	// New Spline methods
	void SplineInterp(Float fraction, _T *pResult)
	{
		*pResult = m_Zero;

		if( m_bKeysIsDirty ) {
			m_bKeysIsDirty = false;
			RecalcSplineVelocityVectors();
		}

		int i;
		Float delta, ratio;

		int numKeys = m_key->size();
		if (numKeys > m_keyValue->size()) {
			numKeys = m_keyValue->size();
		}

		if( m_keyVelocityOut == NULL ||
			m_keyVelocityOut->size() != numKeys ||
			m_keyVelocityIn == NULL ||
			m_keyVelocityIn->size() != numKeys ) {
			return;
		}

		if (numKeys)
		{
			for(i = 0; i < numKeys; i++)
			{
				if( fraction < (*m_key)[i])
					break;
			}

			if( i==0 )
			{
				*pResult = (*m_keyValue)[0];
			}
			else if ( i == numKeys )
			{
				*pResult = (*m_keyValue)[numKeys - 1];
			}
			else
			{
				delta = (*m_key)[i] - (*m_key)[i-1];
				if( delta == 0.f) {
					ratio = 0.f;
				}
				else {
					ratio = (fraction - (*m_key)[i-1]) / delta;
				}

				SplineTween( ratio, &(*m_keyValue)[i-1], &(*m_keyValue)[i], 
					&(*m_keyVelocityOut)[i-1], &(*m_keyVelocityIn)[i], pResult);

			}
		}
	}




	// New methods
	void NormalInterp(Float fraction, _T *pResult)
	{

		int i;
		Float delta, ratio;

		int numKeys = m_key->size();
		if (numKeys > m_keyValue->size())
			numKeys = m_keyValue->size();

		if (numKeys)
		{
			for(i = 0; i < numKeys; i++)
			{
				if( fraction < (*m_key)[i])
					break;
			}

			if( i==0 )
			{
				*pResult = (*m_keyValue)[0];
			}
			else if ( i == numKeys )
			{
				*pResult = (*m_keyValue)[numKeys - 1];
			}
			else
			{
				delta = (*m_key)[i] - (*m_key)[i-1];
				if( delta == 0.f)
					ratio = 0.f;
				else
					ratio = (fraction - (*m_key)[i-1]) / delta;

				NormalTween(ratio, &(*m_keyValue)[i-1], &(*m_keyValue)[i], pResult);

			}
		}
	}





	void RGBInterp(Float fraction, _T *pResult)
	{

		int i;
		Float delta, ratio;

		int numKeys = m_key->size();
		if (numKeys > m_keyValue->size())
			numKeys = m_keyValue->size();

		if (numKeys)
		{
			for(i = 0; i < numKeys; i++)
			{
				if( fraction < (*m_key)[i])
					break;
			}

			if( i==0 )
			{
				*pResult = (*m_keyValue)[0];
			}
			else if ( i == numKeys )
			{
				*pResult = (*m_keyValue)[numKeys - 1];
			}
			else
			{
				delta = (*m_key)[i] - (*m_key)[i-1];
				if( delta == 0.f)
					ratio = 0.f;
				else
					ratio = (fraction - (*m_key)[i-1]) / delta;

				RGBTween(ratio, &(*m_keyValue)[i-1], &(*m_keyValue)[i], pResult);

			}
		}
	}

	void Interp(Float fraction, _TArray *pResult)
	{

		int i, j;
		Float delta, ratio;

		int numKeys = m_key->size();
		if (numKeys > m_keyValue->size())
			numKeys = m_keyValue->size();

		int valuesPerKey = m_keyValue->size() / numKeys;
		pResult->clear();

		if (numKeys)
		{
			for(i = 0; i < numKeys; i++)
			{
				if( fraction < (*m_key)[i])
					break;
			}

			for (j = 0; j < valuesPerKey; j++)
			{
				if( i==0 )
				{
					pResult->push_back((*m_keyValue)[j]);
				}
				else if ( i == numKeys )
				{
					pResult->push_back((*m_keyValue)[(numKeys - 1) * valuesPerKey + j]);
				}
				else
				{
					delta = (*m_key)[i] - (*m_key)[i-1];
					if( delta == 0.f)
						ratio = 0.f;
					else
						ratio = (fraction - (*m_key)[i-1]) / delta;

					_T tmp;

					Tween(ratio, &(*m_keyValue)[(i - 1) * valuesPerKey + j],
							&(*m_keyValue)[i * valuesPerKey + j], &tmp);

					pResult->push_back(tmp);
				}
			}


		}
	
	}

	void Tween(Float ratio, Point3 *pp1, Point3 *pp2, Point3 *pResult)
	{
		CAnmTween::Tween(ratio, pp1, pp2, pResult);
	}

	void NormalTween(Float ratio, Point3 *pp1, Point3 *pp2, Point3 *pResult)
	{
		CAnmTween::NormalTween(ratio, pp1, pp2, pResult);
	}

	void Tween(Float ratio, Rotation *pr1, Rotation *pr2, Rotation *pResult)
	{
		CAnmTween::Tween(ratio, pr1, pr2, pResult);
	}

	void Tween(Float ratio, Color *pc1, Color *pc2, Color *pResult)
	{
		CAnmTween::Tween(ratio, pc1, pc2, pResult);
	}

	void RGBTween(Float ratio, Color *pc1, Color *pc2, Color *pResult)
	{
		CAnmTween::RGBTween(ratio, pc1, pc2, pResult);
	}

	void Tween(Float ratio, Float *pf1, Float *pf2, Float *pResult)
	{
		CAnmTween::Tween(ratio, pf1, pf2, pResult);
	}

	void Tween(Float ratio, Point3Array *ppa1, Point3Array *ppa2, Point3Array *pResult)
	{
		CAnmTween::Tween(ratio, ppa1, ppa2, pResult);
	}


	void Tween(Float ratio, Point2 *pp1, Point2 *pp2, Point2 *pResult)
	{
		CAnmTween::Tween(ratio, pp1, pp2, pResult);
	}


	void SplineTween( Float ratio, 
						_T*			pKeyValue1,
						_T*			pKeyValue2,
						_T*			pKeyVelocity1,
						_T*			pKeyVelocity2,
						_T*			pResult )
{
	static float CatmullRomMatrix[16] = { 
			2.0f, -3.0f, 0.0f, 1.0f,
			-2.0f, 3.0f, 0.0f, 0.0f,
			1.0f, -2.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 0.0f };

		
	_T* KeyVector[4] = { pKeyValue1, pKeyValue2, pKeyVelocity1, pKeyVelocity2 };
	Float fractionVector[4];

	fractionVector[3] = 1.0f;
	fractionVector[2] = ratio;
	fractionVector[1] = ratio*ratio;
	fractionVector[0] = fractionVector[1]*ratio;

	int j, k, jj;

	for( k=0; k<4; k++ ) {
		_T tmp = m_Zero;
		for(j=0, jj=0; j<4; j++, jj+=4) {
			tmp += CatmullRomMatrix[jj+k] * ( * ( KeyVector[j] ) );
		}
		*pResult += tmp * fractionVector[k];
	}
}



	




	// Accessors
	virtual void SetKey(FloatArray *pKey)
	{
		// N.B.: Need to generate _changed events in all my client classes; later - TP
		assert(pKey);

		SafeUnRef(m_key);
		m_key = pKey;
		m_key->Ref();
		m_bKeysIsDirty = true;

	}
	virtual FloatArray *GetKey() { return m_key; }
	virtual void GetKey(FloatArray **pKey)
	{
		if (pKey)
			*pKey = m_key;
	}

	virtual void SetKeyValue(_TArray *pKeyValue)
	{
		assert(pKeyValue);

		SafeUnRef(m_keyValue);
		m_keyValue = pKeyValue;
		m_keyValue->Ref();
		m_bKeysIsDirty = true;
	}

	virtual _TArray *GetKeyValue() { return m_keyValue; }
	virtual void GetKeyValue(_TArray **pKeyValue)
	{
		if (pKeyValue)
			*pKeyValue = m_keyValue;
	}

	virtual void SetKeyVelocity(_TArray *pKeyVelocity)
	{
		assert(pKeyVelocity);

		SafeUnRef(m_keyVelocity);
		m_keyVelocity = pKeyVelocity;
		m_keyVelocity->Ref();
		m_bKeysIsDirty = true;
	}

	virtual _TArray *GetKeyVelocity() { return m_keyVelocity; }
	virtual void GetKeyVelocity(_TArray **pKeyVelocity)
	{
		if (pKeyVelocity)
			*pKeyVelocity = m_keyVelocity;
	}


	virtual void SetClosed( bool bClosed )
	{
		m_closed = bClosed;
		m_bKeysIsDirty = true;
	}
	virtual Boolean GetClosed() { return m_closed; }
	virtual void GetClosed( Boolean* b ) { *b = m_closed; }

	// krv; Ugly Ugly Ugly.  I dont know any other way to get a templatized Zero.
	//
	virtual SetZero( _T& zero ){ m_Zero = zero; }
};



#endif // _anminterpolator_h

