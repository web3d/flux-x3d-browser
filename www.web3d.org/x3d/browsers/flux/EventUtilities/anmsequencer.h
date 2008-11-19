/********************************************************************************
 * Flux
 *
 * File: anmsequencer.h
 * Description: Base Sequencer class
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

#ifndef _anmsequencer_h
#define _anmsequencer_h

#include <vector>

#define DEFAULT_NUM_KEYS 3		// typical small animation

template <class _T> class CAnmSequencer
{
	
typedef CAnmArray<_T> _TArray;

protected:

	FloatArray					*m_key;
	_TArray						*m_keyValue;
	Integer						m_iLastIndex;

public:

	// constructor/destructor
	CAnmSequencer()
	{
		m_key = new FloatArray;
		m_keyValue = new _TArray;
		m_key->reserve(DEFAULT_NUM_KEYS);
		m_keyValue->reserve(DEFAULT_NUM_KEYS);
		m_iLastIndex = 0;
	}

	virtual ~CAnmSequencer()
	{
		SafeUnRef(m_key);
		SafeUnRef(m_keyValue);
	}

	// New methods
	void Evaluate(Float fraction, _T *pResult)
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

			if (i == 0)
			{
				m_iLastIndex = 0;
			}
			else
			{
				m_iLastIndex = i-1;
			}
			*pResult = (*m_keyValue)[m_iLastIndex];
		}
	}


	bool Next(_T *pResult)
	{
		return NextPrev( 1, pResult );
	}

	bool Previous(_T *pResult)
	{
		return NextPrev( -1, pResult );
	}

	bool NextPrev( int iDir, _T *pResult)
	{
		bool bGotVal = false;
		int numKeys = m_key->size();
		if (numKeys > m_keyValue->size())
			numKeys = m_keyValue->size();

		int iTry = m_iLastIndex + iDir;
		if ( iTry < numKeys && iTry >= 0 )
		{
			m_iLastIndex = iTry;
			*pResult = (*m_keyValue)[m_iLastIndex];
			bGotVal = true;
		}
		return bGotVal;
	}


	// Accessors
	virtual void SetKey(FloatArray *pKey)
	{
		// N.B.: Need to generate _changed events in all my client classes; later - TP
		assert(pKey);

		SafeUnRef(m_key);
		m_key = pKey;
		m_key->Ref();

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
	}

	virtual _TArray *GetKeyValue() { return m_keyValue; }
	virtual void GetKeyValue(_TArray **pKeyValue)
	{
		if (pKeyValue)
			*pKeyValue = m_keyValue;
	}

};

#endif // _anmsequencer_h

