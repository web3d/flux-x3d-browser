/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmaudioSensor.h
 *		Description:	Anima AudioSensor node
 *
 *
 * (C) 2001, 2002 by Media Machines
 *
 *		Revision history:
 *			$Log: anmaudioSensor.h,v $
 *			Revision 1.1.1.1  2003/09/10 02:18:12  turk
 *			build466
 *			
 *
 ******************************************************************/
 
#ifndef _anmaudioSensor_h
#define _anmaudioSensor_h

#include "anmnode.h"
#include "anmsensor.h"

#define ANMAUDIOSENSOR_DEFAULT_ENABLED						TRUE
#define ANMAUDIOSENSOR_DEFAULT_FREQ_SIZE					0
#define ANMAUDIOSENSOR_DEFAULT_FREQSTART					( 0.05 )
#define ANMAUDIOSENSOR_DEFAULT_FREQEND						( 0.40 )



class CAnmAudioSensor : public CAnmSensor
{
protected :

	Integer			m_frequencyDataSize;
	Float			m_frequencyStart;
	Float			m_frequencyEnd;
	CAnmNode		*m_source;

	FloatArray		m_FreqArrayOut;


	class CanmAudioSensorRenderer* m_pRenererFilter;

public:

	// constructor/destructor
	CAnmAudioSensor();
	virtual ~CAnmAudioSensor();

	// CAnmSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);


	// CAnmGroup overrides
	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);


	void SetFrequencyDataSize(int frequencyDataSize);
	int GetFrequencyDataSize() { return m_frequencyDataSize; }
	void GetFrequencyDataSize(int *pVal)
	{
		if (pVal)
			*pVal = m_frequencyDataSize;
	}

	void SetFrequencyStart(Float frequencyStart);
	Float GetFrequencyStart() { return m_frequencyStart; }
	void GetFrequencyStart(Float *pVal)
	{
		if (pVal)
			*pVal = m_frequencyStart;
	}

	void SetFrequencyEnd(Float frequencyEnd);
	Float GetFrequencyEnd() { return m_frequencyEnd; }
	void GetFrequencyEnd(Float *pVal)
	{
		if (pVal)
			*pVal = m_frequencyEnd;
	}

	void SetSource(CAnmNode *pSource);
	class CAnmNode *GetSource() { return m_source; }
	void GetSource(CAnmNode **pVal)
	{
		assert(pVal);
		*pVal = m_source;
	}


	// New methods
	void CAnmAudioSensor::SendAudioEvents( Float newSoundData, Boolean bBeat );

	// Accessors
	void SetRendererFilter( class CanmAudioSensorRenderer* pRenererFilter ){ m_pRenererFilter = pRenererFilter; }


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmAudioSensor);




	DECLARE_VALUE(enabled);
	DECLARE_VALUE(frequencyDataSize);
	DECLARE_VALUE(frequencyStart);
	DECLARE_VALUE(frequencyEnd);
	DECLARE_VALUE(source);

	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_frequencyDataSize);
	DECLARE_METHOD(set_frequencyStart);
	DECLARE_METHOD(set_frequencyEnd);
	DECLARE_METHOD(set_source);

	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(frequencyDataSize_changed);
	DECLARE_CALLBACK(frequencyStart_changed);
	DECLARE_CALLBACK(frequencyEnd_changed);
	DECLARE_CALLBACK(source_changed);

	DECLARE_CALLBACK(magnitude_changed);
	DECLARE_CALLBACK(beat_changed);
	DECLARE_CALLBACK(frequency_changed);


};

OKDECLARE_GETIMPLEMENTATION(CAnmAudioSensor);

#endif //_anmaudioSensor_h

