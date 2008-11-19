/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmaudiosensor.cpp
 *		Description:	Anima AudioSensor node
 *
 *
 * (C) 2001, 2002 by Media Machines
 *
 *		Revision history:
 *			$Log: anmaudiosensor.cpp,v $
 *			Revision 1.2  2003/09/14 19:00:19  Tony
 *			Reference-counted strings
 *			
 *			Revision 1.1.1.1  2003/09/10 02:18:12  turk
 *			build466
 *			
 *
 ******************************************************************/
 
#include "stdafx.h"

#include <streams.h>

#include "anmaudiosensor.h"
#include "anmmovietexture.h"
#include "anmaudioclip.h"
#include "anmaudioSensorRenderer.h"


CAnmAudioSensor::CAnmAudioSensor() : 
				CAnmSensor(ANMAUDIOSENSOR_DEFAULT_ENABLED),
	m_frequencyDataSize(ANMAUDIOSENSOR_DEFAULT_FREQ_SIZE),
	m_frequencyStart(ANMAUDIOSENSOR_DEFAULT_FREQSTART),
	m_frequencyEnd(ANMAUDIOSENSOR_DEFAULT_FREQEND),
	m_source(NULL),
	m_pRenererFilter( NULL )
{
				
				
}

CAnmAudioSensor::~CAnmAudioSensor()
{
	if( m_pRenererFilter ) {
		m_pRenererFilter->SetRenderObject( NULL );
	}
}

void CAnmAudioSensor::Realize()
{
	CAnmSensor::Realize();
}



void CAnmAudioSensor::Update()
{
	if (StateDirty())
	{
		if (m_source)
		{
			CAnmAudioClip *pAudioClip = GETIMPLEMENTATION(CAnmAudioClip, m_source);
			if (pAudioClip) {
//				pAudioClip->SetIntensity(m_intensity);
			}
			else {
				CAnmMovieTexture *pMovie = GETIMPLEMENTATION(CAnmMovieTexture, m_source);
				if (pMovie) {
//					pMovie->SetIntensity(m_intensity);
				}
			}
		}

		ClearStateDirty();
	}
}


//	m_frequencyDataSize(ANMAUDIOSENSOR_DEFAULT_FREQ_SIZE),
//	m_frequencyStart(ANMAUDIOSENSOR_DEFAULT_FREQSTART),
//	m_frequencyEnd(ANMAUDIOSENSOR_DEFAULT_FREQEND),
/**
			Float newSoundData = 0.0;
			FloatArray* pMyFloatArrayOut = new FloatArray;

			if( m_frequencyDataSize > 0 ) {
				pMyFloatArrayOut = new FloatArray;
				pMyFloatArrayOut->resize( m_frequencyDataSize );
			}

			Boolean bBeat = m_pSoundDirShow->GetAudioData(newSoundData, *pMyFloatArrayOut);
			CallCallbacks(CALLBACKID(CAnmAudioSensor, audioData_changed), &newSoundData);
			if( bBeat ) {
				Boolean bBeatTrue = TRUE;
				CallCallbacks(CALLBACKID(CAnmAudioSensor, audioBeat_changed), &bBeatTrue);
			}
			CallCallbacks(CALLBACKID(CAnmAudioSensor, audioFreq_changed), &pMyFloatArrayOut);


**/


void CAnmAudioSensor::Poll()
{
	// Get the audio data from the AudioSensorRenderer that this wired into the DIrect SHow Graph.
	//
	if( m_enabled ) {
		if( m_pRenererFilter ) {

			// Make sure we can recieve the data.
			//
			Float audioMagnitude = 0.0;
			Boolean bBeat = false;
			FloatArray*	pFreqArrayOut = &m_FreqArrayOut;

			if( m_frequencyDataSize > 0 &&
				m_frequencyDataSize != pFreqArrayOut->size() ) {
				pFreqArrayOut->resize( m_frequencyDataSize );
			}

			// Get the audio data
			//
			if( m_pRenererFilter->GetAudioData( 
				m_frequencyDataSize,
				m_frequencyStart,
				m_frequencyEnd,
				bBeat, audioMagnitude, m_FreqArrayOut) ) {

				// Distrubute the audio data via callbacks.
				//
				CallCallbacks(CALLBACKID(CAnmAudioSensor, magnitude_changed), &audioMagnitude);
				if( bBeat ) {
					Boolean bBeatTrue = TRUE;
					CallCallbacks(CALLBACKID(CAnmAudioSensor, beat_changed), &bBeatTrue);
				}

				CallCallbacks(CALLBACKID(CAnmAudioSensor, frequency_changed), &pFreqArrayOut);
			}
		}
	}
}


// N.B.: We use render traverse to get local matrix; only time to do it
void CAnmAudioSensor::Traverse(CAnmUpdateVisitor *pVisitor)
{
	assert(pVisitor);

	// krv: do we need to traverse the "source" node????

	CAnmSensor::Traverse(pVisitor);
}





eAnmReturnStatus CAnmAudioSensor::AddChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();
	CAnmMediaSourceObject* pMSO = NULL;

	if( pImp && pImp->GetInterface(ANMINTERFACEID(CAnmMediaSourceObject), &pMSO) ) {
		m_source = pImp;
		pMSO->AddRenderer( this );
	}
	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmAudioSensor::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();
	CAnmMediaSourceObject* pMSO = NULL;
	
	if( pImp && 
		pImp == m_source &&
		pImp->GetInterface(ANMINTERFACEID(CAnmMediaSourceObject), &pMSO ) ) {
		pMSO->RemoveRenderer( this );
		m_source = NULL;
	}
	return eAnmReturnAllGood;
}



void CAnmAudioSensor::SetSource(CAnmNode *pSource)
{
	if (m_source == pSource)
		return;

	if (m_source)
		RemoveChild(m_source);

	if (pSource == NULL)
		return;

	AddChild(pSource);
	m_source = pSource;
}

void CAnmAudioSensor::SetFrequencyDataSize(int frequencyDataSize)
{
	m_frequencyDataSize = frequencyDataSize;

	SetStateDirty();
}

void CAnmAudioSensor::SetFrequencyStart(Float frequencyStart)
{
	m_frequencyStart = frequencyStart;

	SetStateDirty();
}

void CAnmAudioSensor::SetFrequencyEnd(Float frequencyEnd)
{
	m_frequencyEnd = frequencyEnd;

	SetStateDirty();
}







DEFINE_VALUE(CAnmAudioSensor, enabled, eValueBoolean, Boolean, GetEnabled, SetEnabled);
DEFINE_VALUE(CAnmAudioSensor, frequencyDataSize, eValueInteger, Integer, GetFrequencyDataSize, SetFrequencyDataSize);
DEFINE_VALUE(CAnmAudioSensor, frequencyStart, eValueFloat, Float, GetFrequencyStart, SetFrequencyStart);
DEFINE_VALUE(CAnmAudioSensor, frequencyEnd, eValueFloat, Float, GetFrequencyEnd, SetFrequencyEnd);
DEFINE_VALUE(CAnmAudioSensor, source, eValueNode, CAnmNode *, GetSource, SetSource);


DEFINE_METHOD(CAnmAudioSensor, set_enabled, eValueBoolean, Boolean, SetEnabled);
DEFINE_METHOD(CAnmAudioSensor, set_frequencyDataSize, eValueInteger, Integer, SetFrequencyDataSize);
DEFINE_METHOD(CAnmAudioSensor, set_frequencyStart, eValueFloat, Float, SetFrequencyStart);
DEFINE_METHOD(CAnmAudioSensor, set_frequencyEnd, eValueFloat, Float, SetFrequencyEnd);
DEFINE_METHOD(CAnmAudioSensor, set_source, eValueNode, CAnmNode *, SetSource);


DEFINE_CALLBACK(CAnmAudioSensor, enabled_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmAudioSensor, frequencyDataSize_changed, eValueInteger);
DEFINE_CALLBACK(CAnmAudioSensor, frequencyStart_changed, eValueFloat);
DEFINE_CALLBACK(CAnmAudioSensor, frequencyEnd_changed, eValueFloat);
DEFINE_CALLBACK(CAnmAudioSensor, source_changed, eValueNode);


DEFINE_CALLBACK(CAnmAudioSensor, magnitude_changed, eValueFloat);
DEFINE_CALLBACK(CAnmAudioSensor, beat_changed, eValueTime);
DEFINE_CALLBACK(CAnmAudioSensor, frequency_changed, eValueFloatArray);




BEGIN_DEFINE_NODE_MEMBERS(CAnmAudioSensor)



VALUEID(CAnmAudioSensor, enabled),
VALUEID(CAnmAudioSensor, source),
VALUEID(CAnmAudioSensor, frequencyDataSize),
VALUEID(CAnmAudioSensor, frequencyStart),
VALUEID(CAnmAudioSensor, frequencyEnd),


METHODID(CAnmAudioSensor, set_enabled),
METHODID(CAnmAudioSensor, set_source),
METHODID(CAnmAudioSensor, set_frequencyDataSize),
METHODID(CAnmAudioSensor, set_frequencyStart),
METHODID(CAnmAudioSensor, set_frequencyEnd),


CALLBACKID(CAnmAudioSensor, enabled_changed),
CALLBACKID(CAnmAudioSensor, source_changed),
CALLBACKID(CAnmAudioSensor, frequencyDataSize_changed),
CALLBACKID(CAnmAudioSensor, frequencyEnd_changed),

CALLBACKID(CAnmAudioSensor, magnitude_changed),
CALLBACKID(CAnmAudioSensor, beat_changed),
CALLBACKID(CAnmAudioSensor, frequency_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmAudioSensor, CAnmSensor);


