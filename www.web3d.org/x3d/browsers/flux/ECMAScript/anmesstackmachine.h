/********************************************************************************
 * Flux
 *
 * File: anmesstackmachine.h
 * Description: ECMAScript stack machine
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

#ifndef _anmesstackmachine_h
#define _anmesstackmachine_h

#include "anmecmascript.h"
#include <stack>

// Code/opcodes for ECMAScript stack machine
typedef ULONG ESCODE;
typedef std::vector<ESCODE> ESCodeBuffer;
#define ESCODEBLOCKSIZE 256

typedef enum eESOpCode {
	ePush,				// 4-byte stack commands
	ePop,
	eDup,
	eLoad,				// load/store 4-byte address
	eStore,
	eFLoad,				// load/store frame-relative
	eFStore,
	ePushV,				// AnmESVar stack commands
	ePopV,
	eDupV,
	eLoadV,
	eStoreV,
	eFLoadV,
	eFStoreV,
	ePutPropV,			// member set/get value
	eGetPropV, 
	ePutArrayPropV,
	eGetArrayPropV, 
	eCallMethodNV,		// object method (varargs)
	eNewOpNV,			// new operator (varargs)
	eAllocNV,			// alloc space for N locals
	eOpCall,			// operators
	eJumpTrue,			// control flow
	eJumpFalse,
	eJump,
	eJumpSub,
	eReturn,
	eHalt,
} eESOpCode;

// Stack machine operators
typedef enum eESOperator
{
	eOpASSIGNMENT, 
	eOpBITWISE_AND ,
	eOpBITWISE_AND_EQUALS ,
	eOpBITWISE_EXCLUSIVE_OR ,
	eOpBITWISE_EXCLUSIVE_OR_EQUALS ,
	eOpBITWISE_OR ,
	eOpBITWISE_OR_EQUALS ,
	eOpBITWISE_SHIFT_LEFT ,
	eOpBITWISE_SHIFT_LEFT_EQUALS ,
	eOpBITWISE_SHIFT_RIGHT ,
	eOpBITWISE_SHIFT_RIGHT_EQUALS ,
	eOpBITWISE_SHIFT_RIGHT_ZERO_FILL ,
	eOpBITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS ,
	eOpDECREMENT ,
	eOpPREDECREMENT ,
	eOpPOSTDECREMENT ,
	eOpDIV ,
	eOpDIV_EQUALS ,
	eOpEQUALS ,
	eOpGREATER_THAN, 
	eOpGT_EQUAL ,
	eOpINCREMENT ,
	eOpPREINCREMENT ,
	eOpPOSTINCREMENT ,
	eOpLESS_THAN ,
	eOpLOGICAL_AND, 
	eOpLOGICAL_OR ,
	eOpLOGICAL_NOT ,
	eOpLS_EQUAL ,
	eOpUNARYMINUS ,
	eOpMINUS ,
	eOpMINUS_EQUALS ,
	eOpMOD ,
	eOpMOD_EQUALS ,
	eOpMULTIPLY ,
	eOpMULTIPLY_EQUALS ,
	eOpNOT_EQUAL ,
	eOpONES_COMPLIMENT ,
	eOpUNARYPLUS ,
	eOpPLUS ,
	eOpPLUS_EQUALS ,
	eOpTYPEOF ,
	eOpCOMMA ,
} eESOperator ;

typedef ULONG ESLOCALOFFSET;
#define ESBADLOCALADDR ((ESLOCALOFFSET) -1)

// Definitions for stack machine runtime
typedef ULONG ESSTACKVALUE;
typedef std::vector<ESSTACKVALUE> ESStack;

#define ESVAR_NSTACKSLOTS (sizeof(AnmESVar) / sizeof(ESCODE))
#define ESSTACKBLOCKSIZE 256

#define ESMAX_METHODARGS 32

#define ESSTACK_MAXSTACKSIZE 65536

class CAnmESStackMachineError : public CAnmError
{
public :
	
	CAnmESStackMachineError( const char* errorText ) : CAnmError(errorText)
	{
	}
};

// The stack machine
class CAnmESCode
{
protected :

	ESCodeBuffer				 m_code;
	int							 m_codeptr;
	int							 m_savedcodeptr;
	int							 m_codesize;

	ESStack						 m_stack;
	int							 m_stackptr;
	int							 m_frameptr;

	AnmESVar					 m_retval;
	AnmESVar					 m_args[ESMAX_METHODARGS];

	virtual eAnmReturnStatus Exec(class CAnmECMAScriptLanguageBinding *pContext);

public :

	CAnmESCode();
	virtual ~CAnmESCode();

	// Accessors
	virtual int GetCurrentCodeAddress()
	{
		return m_code.size();
	}

	// Execution
	virtual void OperatorCall(eESOperator op);
	virtual AnmESVar CallFunction(class CAnmECMAScriptLanguageBinding *pContext,
		int addr, int nArgs, AnmESVar *args);

	// Execution helpers
	virtual ESCODE ReadCode()
	{
		return m_code[m_codeptr++];
	}

	virtual AnmESVar ReadValue()
	{
		// the value is actually inline in the code stream
		AnmESVar *vp = (AnmESVar *) & m_code[m_codeptr];

		// move code pointer past the value
		m_codeptr += ESVAR_NSTACKSLOTS;

		return *vp;
	}

	virtual void Push(ESSTACKVALUE val)
	{
#if 0
		if (m_stackptr > ESSTACK_MAXSTACKSIZE)
			throw CAnmESStackMachineError("ECMAScript stack overflow");
#endif

		if (m_stackptr >= m_stack.size())
		{
			m_stack.push_back(val);
			m_stackptr++;
		}
		else
			m_stack[m_stackptr++] = val;
	}

	virtual ESSTACKVALUE Pop()
	{
		assert (m_stackptr > 0);

		return m_stack[--m_stackptr];
	}

	virtual ESSTACKVALUE Top()
	{
		assert (m_stackptr > 0);

		return m_stack[m_stackptr - 1];
	}

	virtual BOOL Empty()
	{
		return m_stackptr <= 0;
	}

	virtual void Dup()
	{
		Push(Top());
	}

	virtual void PushV(AnmESVar v)
	{

		ESSTACKVALUE ev[ESVAR_NSTACKSLOTS];
		AnmESVar *vp = (AnmESVar *) &ev[0];

		*vp = v;
		for (int i = 0; i < ESVAR_NSTACKSLOTS; i++)
			Push(ev[i]);

	}

	virtual AnmESVar PopV()
	{
		ESSTACKVALUE ev[ESVAR_NSTACKSLOTS];

		for (int i = ESVAR_NSTACKSLOTS - 1; i >= 0; i--)
		{
			assert (!Empty());
			ev[i] = Pop();
		}

		AnmESVar retval = * (AnmESVar *) ev;
		return retval;
	}

	virtual AnmESVar TopV()
	{
		ESSTACKVALUE ev[ESVAR_NSTACKSLOTS];

		assert ((m_stackptr - ESVAR_NSTACKSLOTS) > 0);

		for (int i = 0; i < ESVAR_NSTACKSLOTS; i++)
		{
			ev[i] = m_stack[m_stackptr - i - 1];
		}

		AnmESVar retval = * (AnmESVar *) ev;
		return retval;
	}

	virtual void DupV()
	{
		PushV(TopV());
	}


	// Code generation
	virtual void Gen(ESCODE code)
	{
		m_code.push_back(code);
	}

	virtual void GenValue(AnmESVar v)
	{
		ESCODE *ep = (ESCODE *) &v;

		for (int i = 0; i < ESVAR_NSTACKSLOTS; i++)
		{
			Gen(*ep++);
		}
	}

	virtual void GenPush(ESSTACKVALUE v)
	{
		Gen((ESCODE) ePush);
		Gen((ESCODE) v);
	}

	virtual void GenPop()
	{
		Gen((ESCODE) ePop);
	}

	virtual void GenDup()
	{
		Gen((ESCODE) eDup);
	}

	virtual void GenLoad()
	{
		Gen((ESCODE) eLoad);
	}

	virtual void GenStore()
	{
		Gen((ESCODE) eStore);
	}

	virtual void GenFLoad()
	{
		Gen((ESCODE) eFLoad);
	}

	virtual void GenFStore()
	{
		Gen((ESCODE) eFStore);
	}

	virtual void GenPushV(AnmESVar v)
	{
		Gen((ESCODE) ePushV);
		GenValue(v);
	}

	virtual void GenPopV()
	{
		Gen((ESCODE) ePopV);
	}

	virtual void GenDupV()
	{
		Gen((ESCODE) eDupV);
	}

	virtual void GenLoadV()
	{
		Gen((ESCODE) eLoadV);
	}

	virtual void GenStoreV()
	{
		Gen((ESCODE) eStoreV);
	}

	virtual void GenFLoadV()
	{
		Gen((ESCODE) eFLoadV);
	}

	virtual void GenFStoreV()
	{
		Gen((ESCODE) eFStoreV);
	}

	virtual void GenPutPropV()
	{
		Gen((ESCODE) ePutPropV);
	}

	virtual void GenGetPropV()
	{
		Gen((ESCODE) eGetPropV);
	}
	
	virtual void GenPutArrayPropV()
	{
		Gen((ESCODE) ePutArrayPropV);
	}

	virtual void GenGetArrayPropV()
	{
		Gen((ESCODE) eGetArrayPropV);
	}
	
	virtual void GenCallMethodNV(int nargs)
	{
		Gen((ESCODE) eCallMethodNV);
		Gen((ESCODE) nargs);
	}

	virtual void GenNewOpNV(int nargs)
	{
		Gen((ESCODE) eNewOpNV);
		Gen((ESCODE) nargs);
	}

	virtual void GenAllocNV(int nVars)
	{
		Gen((ESCODE) eAllocNV);
		Gen((ESCODE) nVars);
	}

	virtual void GenOpCall()
	{
		Gen((ESCODE) eOpCall);
	}

	virtual void GenJumpTrue()
	{
		Gen((ESCODE) eJumpTrue);
	}

	virtual void GenJumpFalse()
	{
		Gen((ESCODE) eJumpFalse);
	}

	virtual void GenJump()
	{
		Gen((ESCODE) eJump);
	}

	virtual void GenJumpSub()
	{
		Gen((ESCODE) eJumpSub);
	}

	virtual void GenReturn()
	{
		Gen((ESCODE) eReturn);
	}

	virtual void GenSubReturnLinkage()
	{
		// gen placeholders for saved frameptr and return address
		GenPush(0L);
		GenPush(0L);
	}

	virtual void WriteAt(int loc, ESCODE code)
	{
		assert(loc >= 0 && loc < m_code.size());
		m_code[loc] = code;
	}
};

#endif // _anmesstackmachine_h
