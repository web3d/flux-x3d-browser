/********************************************************************************
 * Flux
 *
 * File: anmesstackmachine.cpp
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

#include "stdafx.h"
#include "anmesstackmachine.h"
#include "anmeslanguagebinding.h"
#include "anmesmessage.h"
#include "anmesobject.h"
#include "anmessymbol.h"

CAnmESCode::CAnmESCode()
{
	m_code.reserve(ESCODEBLOCKSIZE);
	// need a place to come back to
	Gen(eHalt);
	m_codeptr = 1;
	m_codesize = 1;

	m_stack.reserve(ESSTACKBLOCKSIZE);
	m_stackptr = 0;
	m_frameptr = 0;

	// push return linkage - this should get us back to Halt
	Push(0L);
	Push(0L);
	m_frameptr = 2;

}

CAnmESCode::~CAnmESCode()
{
}

eAnmReturnStatus CAnmESCode::Exec(class CAnmECMAScriptLanguageBinding *pContext)
{
	m_codesize = m_code.size();
	ESSTACKVALUE sval;
	AnmESVar v;
	AnmESVar obj;
	AnmESVar arrayindex;
	AnmESVar *vp;
	eESOperator op;
	int jumpoffset;
	int jumpaddr;
	int frameoffset;
	int linkstart;
	ESSTACKVALUE *ep;
	ESSTACKVALUE ev[ESVAR_NSTACKSLOTS];
	int i;
	int nVars;
	char *name;
	CAnmESObject *pObject = NULL;
	CAnmESTypeSymbol *pTypeSym = NULL;
	int nargs;

	BOOL isrunning = TRUE;

	while (isrunning && m_codeptr < m_codesize)
	{
		eESOpCode opcode = (eESOpCode) ReadCode();

		switch (opcode)
		{
			case ePush :
				sval = (ESSTACKVALUE) ReadCode();
				Push(sval);
				break;

			case ePop :
				sval = Pop();
				break;

			case eDup :
				Dup();
				break;

			case eLoad :
				break;

			case eStore :
				break;

			case eFLoad :
				break;

			case eFStore :
				break;

			case ePushV :
				v = ReadValue();
				PushV(v);
				break;

			case ePopV :
				v = PopV();
				break;

			case eDupV :
				DupV();
				break;

			case eLoadV :
				vp = (AnmESVar *) Pop();
				PushV(*vp);
				break;

			case eStoreV :
				v = PopV();
				vp = (AnmESVar *) Pop();
				*vp = v;
				break;

			case eFLoadV :
				frameoffset = Pop();
				ep = &m_stack[m_frameptr + frameoffset * ESVAR_NSTACKSLOTS];
				
				for (i = 0; i < ESVAR_NSTACKSLOTS; i++)
					ev[i] = ep[i];

				v = * (AnmESVar *) ep;
				PushV(v);
				break;

			case eFStoreV :
				v = PopV();
				frameoffset = Pop();
				ep = &m_stack[m_frameptr + frameoffset * ESVAR_NSTACKSLOTS];
				* (AnmESVar *) ep = v;
				break;

			case ePutPropV :
				v = PopV();
				name = (char *) Pop();
				obj = PopV();
				pObject = (CAnmESObject *) obj;
				if (pObject)
				{
					pObject->PutProperty(name, v);
					PushV(v);
				}
				else
				{
					if (!name)
						name = "(null)";

					// ANMMSG_ESINVALIDOBJECTPUTPROP : "ECMAScript intepreter: invalid object in PutPropV call (property '%s') "
					char *errmsg = (char *) malloc(strlen(ANMMSG_ESINVALIDOBJECTPUTPROP) + 
						strlen(name) +
						1);

					sprintf(errmsg, ANMMSG_ESINVALIDOBJECTPUTPROP, name);

					CAnmError err(errmsg);

					delete errmsg;

					throw err;
				}
				break;

			case eGetPropV :
				name = (char *) Pop();
				obj = PopV();
				pObject = (CAnmESObject *) obj;
				if (pObject)
				{
					v = pObject->GetProperty(name);
					PushV(v);
				}
				else
				{
					if (!name)
						name = "(null)";

					// ANMMSG_ESINVALIDOBJECTGETPROP : "ECMAScript intepreter: invalid object in GetPropV call (property '%s') "
					char *errmsg = (char *) malloc(strlen(ANMMSG_ESINVALIDOBJECTGETPROP) + 
						strlen(name) +
						1);

					sprintf(errmsg, ANMMSG_ESINVALIDOBJECTGETPROP, name);
					
					CAnmError err(errmsg);

					delete errmsg;

					throw err;

				}
				break;

			case ePutArrayPropV :
				v = PopV();
				arrayindex = PopV();
				obj = PopV();
				pObject = (CAnmESObject *) obj;
				if (pObject)
				{
					pObject->PutArrayProperty(arrayindex, v);
					PushV(v);
				}
				else
				{
					// ANMMSG_ESINVALIDOBJECTPUTPROPA : "ECMAScript intepreter: invalid object in PutArrayPropV call"
					throw CAnmError(ANMMSG_ESINVALIDOBJECTPUTPROPA);
				}
				break;

			case eGetArrayPropV :
				arrayindex = PopV();
				obj = PopV();
				pObject = (CAnmESObject *) obj;
				if (pObject)
				{
					v = pObject->GetArrayProperty(arrayindex);
					PushV(v);
				}
				else
				{
					// ANMMSG_ESINVALIDOBJECTGETPROPA : "ECMAScript intepreter: invalid object in GetArrayPropV call"
					throw CAnmError(ANMMSG_ESINVALIDOBJECTGETPROPA);
				}
				break;

			case eCallMethodNV :

				nargs = (int) ReadCode();
				for (i = nargs - 1; i >= 0; i--)
				{
					m_args[i] = PopV();
				}
				name = (char *) Pop();
				obj = PopV();
				if (obj.ptrVal)
				{
					pObject = (CAnmESObject *) obj.ptrVal;
					v = pObject->CallMethod(name, nargs, m_args);
					PushV(v);
				}
				else
				{
					// ANMMSG_ESINVALIDOBJECTCALLMETHOD : "ECMAScript intepreter: invalid object in CallMethodNV call"
					throw CAnmError(ANMMSG_ESINVALIDOBJECTCALLMETHOD);
				}
				break;

			case eNewOpNV:

				nargs = (int) ReadCode();
				for (i = nargs - 1; i >= 0; i--)
				{
					m_args[i] = PopV();
				}

				pTypeSym = (CAnmESTypeSymbol *) Pop();

				if (pTypeSym != NULL)
				{
					v = pTypeSym->CreateObject(pContext, nargs, m_args);
					PushV(v);
				}
				else
				{
					// ANMMSG_ESINVALIDOBJECTNEWOP : "ECMAScript intepreter: invalid type code in NewOpNV call"
					throw CAnmError(ANMMSG_ESINVALIDOBJECTNEWOP);
				}
				break;

			case eAllocNV :
				nVars = ReadCode();
				for (i = 0; i < nVars; i++)
					PushV(0.0);
				break;

			case eOpCall :
				op = (eESOperator) Pop();
				OperatorCall(op);
				break;

			case eJumpFalse :
				v = PopV();
				jumpoffset = ReadCode();
				if (!(BOOL) v)
				{
					m_codeptr += jumpoffset;
				}
				break;

			case eJumpTrue :
				v = PopV();
				jumpoffset = ReadCode();
				if ((BOOL) v)
				{
					m_codeptr += jumpoffset;
				}
				break;

			case eJump :
				jumpoffset = (int) ReadCode();
				m_codeptr += jumpoffset;
				break;

			case eJumpSub :
				jumpaddr = (int) ReadCode();
				nargs = (int) ReadCode();
				if (jumpaddr)
				{
					// save return linkage:
					//		stackptr just below args and frame linkage;
					//		frameptr at current
					//		codeptr at current
					linkstart = m_stackptr - (nargs * ESVAR_NSTACKSLOTS) - 2;
													// 2 for saved frame and return address

					m_stack[linkstart] = m_frameptr;
					m_stack[linkstart + 1] = m_codeptr;

					// do the jump
					//		stackptr is ok
					//		move codeptr by offset
					//		move frameptr to start of frame

					m_codeptr = jumpaddr;
					m_frameptr = m_stackptr - (nargs * ESVAR_NSTACKSLOTS);
				}
				else
				{
					// N.B.: should warn here about nonexistent call
					// push a bogus return value on stack to keep sanity
					PushV((AnmESVar) 0L);
				}
				break;

			case eReturn :
				m_retval = PopV();

				linkstart = m_frameptr - 2;
				m_stackptr = linkstart;
				m_frameptr = m_stack[linkstart];
				m_codeptr = m_stack[linkstart + 1];
				PushV(m_retval);
				break;

			case eHalt :
				isrunning = FALSE;
				break;

			default :
				// ANMMSG_ESBADOPCODE : "[CAnmESCode::Execute] bad opcode"
				throw CAnmError(ANMMSG_ESBADOPCODE);
				break;
		}
	}
	
	return eAnmReturnAllGood;
}

void CAnmESCode::OperatorCall(eESOperator op)
{
	AnmESVar val, val1, val2;

	switch (op)
	{
		// Unary operators
			case eOpINCREMENT :
				val = (double) PopV() + 1;
				PushV(val);
				break;

			case eOpDECREMENT :
				val = (double) PopV() - 1;
				PushV(val);				
				break;

			case eOpUNARYPLUS :
				// no-op; shouldn't even be here?
				assert(FALSE);
				break;

			case eOpUNARYMINUS :
				val = - PopV();
				PushV(val);
				break;

			case eOpONES_COMPLIMENT :
				val = ~ PopV();
				PushV(val);
				break;

			case eOpLOGICAL_NOT :
				val = ! PopV();
				PushV(val);
				break;

		// Binary operators

			case eOpMULTIPLY :
			case eOpMULTIPLY_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 * val2);
				break;

			case eOpDIV :
			case eOpDIV_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 / val2);
				break;

			case eOpMOD :
			case eOpMOD_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 % val2);
				break;

			case eOpPLUS :
			case eOpPLUS_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 + val2);
				break;

			case eOpMINUS :
			case eOpMINUS_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 - val2);
				break;

			case eOpBITWISE_SHIFT_LEFT :
			case eOpBITWISE_SHIFT_LEFT_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 << val2);
				break;

			case eOpBITWISE_SHIFT_RIGHT :
			case eOpBITWISE_SHIFT_RIGHT_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 >> val2);
				break;

			case eOpBITWISE_SHIFT_RIGHT_ZERO_FILL :
			case eOpBITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS :
				assert(FALSE);							// I don't even know what this is! --TP	
				break;

			case eOpLESS_THAN :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 < val2);
				break;

			case eOpGREATER_THAN :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 > val2);
				break;

			case eOpLS_EQUAL :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 <= val2);
				break;

			case eOpGT_EQUAL :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 >= val2);
				break;

			case eOpEQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 == val2);
				break;

			case eOpNOT_EQUAL :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 != val2);
				break;

			case eOpBITWISE_AND :
			case eOpBITWISE_AND_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 & val2);
				break;

			case eOpBITWISE_EXCLUSIVE_OR :
			case eOpBITWISE_EXCLUSIVE_OR_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 ^ val2);
				break;

			case eOpBITWISE_OR :
			case eOpBITWISE_OR_EQUALS :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 | val2);
				break;

			case eOpLOGICAL_AND :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 && val2);
				break;

			case eOpLOGICAL_OR :
				val2 = PopV();
				val1 = PopV();
				PushV (val1 || val2);
				break;

			default :
				// ANMMSG_ESBADOPERATOR : "[CAnmESCode::OperatorCall] bad operator"
				throw CAnmError(ANMMSG_ESBADOPERATOR);
				break;

	}
}

AnmESVar CAnmESCode::CallFunction(class CAnmECMAScriptLanguageBinding *pContext,
								  int addr, int nArgs, AnmESVar *args)
{
#if 0

	// N.B.: PRE-REENTRANCY FIX; JUST IN CASE -- TP 06-05-19 

	m_stackptr = 0;

	// push return linkage - this should get us back to Halt
	Push(0L);
	Push(0L);
	m_frameptr = 2;

	m_codeptr = addr;

	// Push args
	for (int i = 0; i < nArgs; i++)
		PushV(args[i]);

	// Run it
	Exec(pContext);

	// Pop one off stack; this is the return value and
	// we don't need it/causes trouble on re-entrant Exec
	PopV();

#else

	// save return linkage:
	//		stackptr just below args and frame linkage;
	//		frameptr at current
	Push(m_frameptr);

	//		want to get to the Halt instruction, so push a Zero
	Push(0L);

	// Push args
	for (int i = 0; i < nArgs; i++)
		PushV(args[i]);

	// do the equivalent of a jumpsub
	//		stackptr is ok
	//		move codeptr by start address
	//		move frameptr to start of frame

	int savedcodeptr = m_codeptr;

	m_codeptr = addr;
	m_frameptr = m_stackptr - (nArgs * ESVAR_NSTACKSLOTS);

	// Run it
	Exec(pContext);

	// Pop one off stack; this is the return value and
	// we don't need it/causes trouble on re-entrant Exec
	PopV();

	// Restore the old code pointer (real one)
	m_codeptr = savedcodeptr;

#endif


	return m_retval;
}