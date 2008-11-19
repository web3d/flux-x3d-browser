/********************************************************************************
 * Flux
 *
 * File: anmescompiler.cpp
 * Description: ECMAScript language compiler
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
#include "anmescompiler.h"
#include "anmbrowser.h"
#include "anmesmessage.h"
#include "anmesbrowser.h"
#include "anmesexpression.h"
#include "anmesfields.h"
#include "anmeslanguagebinding.h"
#include "anmeslexer.h"
#include "anmesmath.h"
#include "anmesmatrix.h"
#include "anmesobject.h"
#include "anmesstatement.h"
#include "anmesstring.h"
#include "anmessymbol.h"

#define ANMESBROWSER_VARNAME "Browser"
#define ANMESMATH_VARNAME "Math"

CAnmECMAScriptCompiler::CAnmECMAScriptCompiler(cApplication *pApp,
											   CAnmECMAScriptLanguageBinding *pOwner,
											   BOOL consoleOutput) :
m_app(pApp),
m_owner(pOwner),
m_consoleOutput(consoleOutput)
{
	m_lexer = NULL;
	m_script = pOwner->GetScriptImplementation();

	AddGlobals(pOwner);
	AddFunctions(pOwner);
	AddTypeSymbols(pOwner);

	m_functionDeclaring = NULL;
	m_functionCalling = NULL;
	m_variablesDeclaring = NULL;
	m_warnUndeclared = TRUE;
	m_inCase = FALSE;
}

CAnmECMAScriptCompiler::~CAnmECMAScriptCompiler()
{
}

void CAnmECMAScriptCompiler::AddGlobals(CAnmECMAScriptLanguageBinding *pOwner)
{
	CAnmESBrowserObject *pESBrowser = new CAnmESBrowserObject(pOwner->GetBrowser(), pOwner);
	AnmESVar *pBrowserVar = new AnmESVar(pESBrowser);

	CAnmESMathObject *pESMath = new CAnmESMathObject(pOwner);
	AnmESVar *pMathVar = new AnmESVar(pESMath);

	pOwner->AddGlobal(pBrowserVar);
	pOwner->AddGlobal(pMathVar);

	CAnmESVarSymbol *pBrowserSym = new CAnmESVarSymbol(new CAnmString(ANMESBROWSER_VARNAME), eGlobalVariable);
	pBrowserSym->SetGlobalAddress(pBrowserVar);
	m_script->AddSymbol(pBrowserSym);

	CAnmESVarSymbol *pMathSym = new CAnmESVarSymbol(new CAnmString(ANMESMATH_VARNAME), eGlobalVariable);
	pMathSym->SetGlobalAddress(pMathVar);
	m_script->AddSymbol(pMathSym);
}

void CAnmECMAScriptCompiler::AddFunctions(CAnmECMAScriptLanguageBinding *pOwner)
{
	// N.B.: add parseInt, parseFloat here
}

void CAnmECMAScriptCompiler::AddTypeSymbols(CAnmECMAScriptLanguageBinding *pOwner)
{
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFColor"), CAnmESSFColor::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFImage"), CAnmESSFImage::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFNode"), CAnmESSFNode::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFRotation"), CAnmESSFRotation::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFTime"), CAnmESSFTime::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFVec2f"), CAnmESSFVec2f::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("SFVec3f"), CAnmESSFVec3f::Create));

	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFColor"), CAnmESMFColor::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFDouble"), CAnmESMFDouble::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFFloat"), CAnmESMFFloat::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFImage"), CAnmESMFImage::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFInt32"), CAnmESMFInt32::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFNode"), CAnmESMFNode::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFRotation"), CAnmESMFRotation::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFString"), CAnmESMFString::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFTime"), CAnmESMFTime::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFVec2f"), CAnmESMFVec2f::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("MFVec3f"), CAnmESMFVec3f::Create));

	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("String"), CAnmESString::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("Object"), CAnmESUserObject::Create));

	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("VrmlMatrix"), CAnmESMatrix4::Create));
	m_script->AddSymbol(new CAnmESTypeSymbol(new CAnmString("Matrix4"), CAnmESMatrix4::Create));

}

eAnmReturnStatus CAnmECMAScriptCompiler::Compile(const char *filename, BOOL fromFile)
{
	eAnmReturnStatus retstatus = eAnmReturnAllGood;
		
	m_lexer = new CAnmECMAScriptLexer(filename, this, fromFile);

	try {
		Yyparse();
	}
	catch( CAnmError err ) // ... )
	{		
		WriteToConsole((char *) err.GetText());
		// ANMMSG_ESERRORSTATUS : "Flux: ECMAScript error"
		if (m_app)
			m_app->SetStatusText(ANMMSG_ESERRORSTATUS);

		SafeDelete(m_lexer);

		return(eAnmReturnAllBad);
	}

	SafeDelete(m_lexer);

	return retstatus;
}

int CAnmECMAScriptCompiler::Yylex(void)
{
	return m_lexer->Lex();
}

void CAnmECMAScriptCompiler::Yyerror(char *msg)
{
	// ANMMSG_ECMASCRIPTCOMPILEERROR : "ECMAScript compile error: [%s] at line %d"
	char *errmsg = (char *) malloc(strlen(ANMMSG_ESCOMPILEERROR) + 
		strlen(msg) + 
		ANMMAXDECIMALLENGTH +
		1);
	sprintf(errmsg, ANMMSG_ESCOMPILEERROR, msg, m_lexer->GetLineNumber());

	CAnmError err(errmsg);

	delete errmsg;

	throw err;
}

void CAnmECMAScriptCompiler::Yywarning(char *msg)
{
	// ANMMSG_ECMASCRIPTCOMPILEWARNING : "ECMAScript compile warning: [%s] at line %d"
	char *warningmsg = (char *) malloc(strlen(ANMMSG_ESCOMPILEWARNING) + 
		strlen(msg) + 
		ANMMAXDECIMALLENGTH +
		1);
	sprintf(warningmsg, ANMMSG_ESCOMPILEWARNING, msg, m_lexer->GetLineNumber());

	WriteToConsole(warningmsg);

	delete warningmsg;
}

// symbol handling
void CAnmECMAScriptCompiler::AddSymbol(CAnmESSymbol *pSymbol)
{
	m_script->AddSymbol(pSymbol);
}

CAnmESSymbol *CAnmECMAScriptCompiler::FindSymbol(char* str)
{
	CAnmESSymbol *pSym = NULL;

	// try local vars first
	if (m_functionDeclaring)
	{
		pSym = m_functionDeclaring->FindLocal(str);
		if (pSym != NULL)
			return pSym;
	}

	return m_script->FindSymbol(str);
}

// expressions
CAnmESExpression *CAnmECMAScriptCompiler::ThisExpression()
{
	CAnmESVarSymbol *pVarSym = m_functionDeclaring->FindLocal(THIS_NAME);


	return new CAnmESLValueExpression(pVarSym);
}

CAnmESExpression *CAnmECMAScriptCompiler::IdentifierExpression(String name)
{
	CAnmESSymbol *pSym = FindSymbol(name->GetBuf());
	CAnmESVarSymbol *pVarSym = NULL;

	if (pSym == NULL)
	{
		// implicit var declaration
		if (m_functionDeclaring)
		{
			pVarSym = m_functionDeclaring->AddLocal(name);
			return new CAnmESLValueExpression(pVarSym);
		}
		else
		{
			AnmESVar *pGlobalVar = new AnmESVar(0.f);
			m_owner->AddGlobal(pGlobalVar);

			pVarSym = new CAnmESVarSymbol(name, eGlobalVariable);
			pVarSym->SetGlobalAddress(pGlobalVar);
			m_script->AddSymbol(pVarSym);

			return new CAnmESLValueExpression(pVarSym);
		}
	}
	else if (pSym->GetSymType() == eVarSymbol)
	{
		pVarSym = (CAnmESVarSymbol *) pSym;

		if (pVarSym->GetVarType() == eMemberVariable)
		{
			if (m_functionDeclaring == NULL)
				Yyerror("attempt to refer to Script member outside of function");

			return new CAnmESLValueExpression(ThisExpression(), name);
		}
		else
			return new CAnmESLValueExpression(pVarSym);
	}
	else if (pSym->GetSymType() == eFunctionSymbol)
	{
		// N.B.: this really didn't work well, caused re-entrancy problems ->
		// Better to treat method calls to 'this' as subroutines TP 08/29/05
		// if (m_owner->LookupMember(name->GetBuf()))
		//	return new CAnmESLValueExpression(ThisExpression(), name);
		// else
		{
			CAnmESFunctionSymbol *pFuncSym = (CAnmESFunctionSymbol *) pSym;
			return new CAnmESLValueExpression(pFuncSym);
		}
	}

	Yyerror("Unknown identifier expression");
	return NULL;
}

CAnmESExpression *CAnmECMAScriptCompiler::UnaryExpression(eESOperator op,
															CAnmESExpression *pExpr)
{
	CAnmESConstExpression *pConstExpr = NULL;
	AnmESVar val;

	// first, let's try to reduce
	if (pExpr->GetExprType() == eConstExpression)
	{
		pConstExpr = (CAnmESConstExpression *) pExpr;
		val = pConstExpr->GetValue();

		switch (op)
		{
			case eOpINCREMENT :
			case eOpDECREMENT :
				// no dice; these must work on variables
				Yyerror("Attempt to auto-increment or auto-decrement constant");
				break;

			case eOpUNARYPLUS :
				// no-op
				return pExpr;
				break;

			case eOpUNARYMINUS :
				val = - val;
				break;

			case eOpONES_COMPLIMENT :
				val = ~ val;
				break;

			case eOpLOGICAL_NOT :
				val = ! val;
				break;

			default :
				assert(FALSE);			// what did we miss?
				break;
		}

		return new CAnmESConstExpression(val);
	}

	// no reduction? just make a new Unary expr with op and base
	return new CAnmESUnaryExpression(pExpr, op);
}

CAnmESExpression *CAnmECMAScriptCompiler::BinaryExpression(CAnmESExpression *pExpr1,
															eESOperator op,
															CAnmESExpression *pExpr2)
{
	CAnmESConstExpression *pConstExpr1 = NULL;
	CAnmESConstExpression *pConstExpr2 = NULL;
	AnmESVar val1, val2, newval;

	// first, let's try to reduce
	if (pExpr1->GetExprType() == eConstExpression && pExpr2->GetExprType() == eConstExpression )
	{
		pConstExpr1 = (CAnmESConstExpression *) pExpr1;
		pConstExpr2 = (CAnmESConstExpression *) pExpr2;
		val1 = pConstExpr1->GetValue();
		val2 = pConstExpr2->GetValue();

		switch (op)
		{
			case eOpMULTIPLY :
				newval = val1 * val2;
				break;

			case eOpDIV :
				newval = val1 / val2;
				break;

			case eOpMOD :
				newval = val1 % val2;
				break;

			case eOpPLUS :
				newval = val1 + val2;
				break;

			case eOpMINUS :
				newval = val1 - val2;
				break;

			case eOpBITWISE_SHIFT_LEFT :
				newval = val1 << val2;
				break;

			case eOpBITWISE_SHIFT_RIGHT :
				newval = val1 >> val2;
				break;

			case eOpBITWISE_SHIFT_RIGHT_ZERO_FILL :
				assert(FALSE);							// I don't even know what this is! --TP	
				break;

			case eOpLESS_THAN :
				newval = val1 < val2;	
				break;

			case eOpGREATER_THAN :
				newval = val1 > val2;	
				break;

			case eOpLS_EQUAL :
				newval = val1 <= val2;	
				break;

			case eOpGT_EQUAL :
				newval = val1 >= val2;	
				break;

			case eOpEQUALS :
				newval = val1 == val2;	
				break;

			case eOpNOT_EQUAL :
				newval = val1 != val2;	
				break;

			case eOpBITWISE_AND :
				newval = val1 & val2;	
				break;

			case eOpBITWISE_EXCLUSIVE_OR :
				newval = val1 ^ val2;	
				break;

			case eOpBITWISE_OR :
				newval = val1 | val2;	
				break;

			case eOpLOGICAL_AND :
				newval = val1 && val2;	
				break;

			case eOpLOGICAL_OR :
				newval = val1 || val2;	
				break;

			default :
				assert(FALSE);			// what did we miss?
				break;
		}

		if (newval.valueType == eString)
		{
			AnmESVar *pStrVar = new AnmESVar(newval.strVal);
			m_owner->AddGlobal(pStrVar);
		}

		return new CAnmESConstExpression(newval);
	}

	// no reduction? just make a new Binary expr with op and base exprs
	return new CAnmESBinaryExpression(pExpr1, pExpr2, op);
}

CAnmESExpression *CAnmECMAScriptCompiler::AssignmentExpression(CAnmESExpression *pLHS,
															eESOperator op,
															CAnmESExpression *pRHS)
{
	// no reduction? just make a new Binary expr with op and base exprs
	return new CAnmESAssignmentExpression(pLHS, pRHS, op);
}

void CAnmECMAScriptCompiler::BeginFunctionCall(class CAnmESExpression *pExpr)
{
	if (pExpr->GetExprType() != eLValueExpression)
		Yyerror("Attempt to use non-object for method call");

	m_callLevels.push(m_functionCalling);
	m_functionCalling = new CAnmESFunctionExpression(pExpr, NULL);
}

CAnmESFunctionExpression *CAnmECMAScriptCompiler::EndFunctionCall()
{
	assert(!m_callLevels.empty());

	CAnmESFunctionExpression *pFuncExpr = m_functionCalling;
	m_functionCalling = m_callLevels.top();
	m_callLevels.pop();

	return pFuncExpr;
}

CAnmECMAScriptCompiler::BeginNew(String name)
{
	CAnmESSymbol *pSym = m_script->FindSymbol(name->GetBuf());
	if (pSym == NULL || pSym->GetSymType() != eTypeSymbol)
		Yyerror("Unknown object type (operator new)");

	CAnmESTypeSymbol *pTypeSym = (CAnmESTypeSymbol *) pSym;

	m_callLevels.push(m_functionCalling);
	m_functionCalling = new CAnmESFunctionExpression(NULL, pTypeSym);
}

CAnmESFunctionExpression *CAnmECMAScriptCompiler::EndNew()
{
	return EndFunctionCall();
}

void CAnmECMAScriptCompiler::AddArgument(class CAnmESExpression *pExpr)
{
	assert (m_functionCalling != NULL);

	m_functionCalling->AddArg(pExpr);

}


// declarations

void CAnmECMAScriptCompiler::BeginDeclareFunction(String name)
{
	// do all the checks here;
	// add function symbol to script's symbol table

	CAnmESSymbol *pSym = FindSymbol(name->GetBuf());
	CAnmESFunctionSymbol *pFuncSym = NULL;

	if (pSym == NULL)
	{
		pFuncSym = new CAnmESFunctionSymbol(name);
		AddSymbol(pFuncSym);
	}
	else if (pSym->GetSymType() != eFunctionSymbol)
	{
		Yyerror("Attempt to use variable or type name as function");
	}
	else
	{
		pFuncSym = (CAnmESFunctionSymbol *) pSym;
		if (pFuncSym->GetProcAddress() != ESBADLOCALADDR)
			Yyerror("Attempt to redefine function");
	}

	pFuncSym->SetProcAddress(m_script->GetCode()->GetCurrentCodeAddress());

	m_functionDeclaring = pFuncSym;
	m_functionDeclaring->AddParam(new CAnmString(THIS_NAME));

	// I'm sure we need to generate function startup code with args, etc.

	// we'll come in and backfill this later
	m_script->GetCode()->GenAllocNV(0);
}

void CAnmECMAScriptCompiler::DeclareParam(String name)
{
	m_functionDeclaring->AddParam(name);
}


void CAnmECMAScriptCompiler::EndDeclareFunction(CAnmESStatement *pStatement)
{
	// Generate all the code for the function body
	GenStatement(pStatement);

	// I'm sure we need to generate function return code with args, etc.

	// first, backfill number of non-param locals
	int nlocals = m_functionDeclaring->GetNLocals() - m_functionDeclaring->GetNArgs();

	int funcaddr = m_functionDeclaring->GetProcAddress();

	if (nlocals > 0)
		m_script->GetCode()->WriteAt(funcaddr + 1,
			(ESCODE) nlocals);

	// generate a return with 0 value (in case there was no explicit one)
	GenReturn(NULL);

	// now backfill any calls made to me by other functions
	std::vector<int> *pCallRefs = 
		m_script->GetCallRefs(m_functionDeclaring->GetName());

	if (pCallRefs)
	{
		for (int i = 0; i < pCallRefs->size(); i++)
		{
			int jumpaddr = (*pCallRefs)[i];
			m_script->GetCode()->WriteAt(jumpaddr, funcaddr);
		}
	}

	m_functionDeclaring = NULL;
}

void CAnmECMAScriptCompiler::BeginBlock()
{
	CAnmESStatementBlock *pBlock = new CAnmESStatementBlock();

	m_blockLevels.push(pBlock);
}

CAnmESStatementBlock *CAnmECMAScriptCompiler::EndBlock()
{
	assert(!m_blockLevels.empty());

	CAnmESStatementBlock *pBlock = m_blockLevels.top();

	m_blockLevels.pop();

	return pBlock;
}

void CAnmECMAScriptCompiler::AddStatement(class CAnmESStatement *pStatement)
{
	if (m_blockLevels.empty())
	{
		// some kind of global statement; just generate code
	}
	else
	{
		CAnmESStatementBlock *pBlock = m_blockLevels.top();

		pBlock->AddStatement(pStatement);
	}
}

void CAnmECMAScriptCompiler::BeginVariableDeclarationList()
{
	if (m_variablesDeclaring)
	{
		Yyerror("Nested VAR statements not allowed.");
	}
	else
	{
		m_variablesDeclaring = new CAnmESVarStatementList();
	}
}

CAnmESVarStatementList *CAnmECMAScriptCompiler::EndVariableDeclarationList()
{
	assert(m_variablesDeclaring);

	CAnmESVarStatementList *pVarStatementList = m_variablesDeclaring;

	m_variablesDeclaring = NULL;

	return pVarStatementList;
}

void CAnmECMAScriptCompiler::AddVariableDeclaration(class CAnmESStatement *pStatement)
{
	assert(m_variablesDeclaring);

	m_variablesDeclaring->AddStatement(pStatement);
}

void CAnmECMAScriptCompiler::BeginCaseBlock(class CAnmESExpression *pExpr, BOOL isDefault)
{
	CAnmESCaseStatement *pCaseStatement = new CAnmESCaseStatement(pExpr, isDefault);

	m_blockLevels.push(pCaseStatement);

}

CAnmESStatement *CAnmECMAScriptCompiler::EndCaseBlock()
{
	CAnmESStatement *pEndBlock = EndBlock();

	assert(pEndBlock->GetType() == eCaseStatement);

	return pEndBlock;
}

static CAnmESEmptyStatement emptyStatement;
CAnmESEmptyStatement *CAnmECMAScriptCompiler::EmptyStatement()
{
	return &emptyStatement;
}


static CAnmESBreakStatement breakStatement;
CAnmESBreakStatement *CAnmECMAScriptCompiler::BreakStatement()
{
	return &breakStatement;
}

static CAnmESContinueStatement continueStatement;
CAnmESContinueStatement *CAnmECMAScriptCompiler::ContinueStatement()
{
	return &continueStatement;
}

class CAnmESStatement *CAnmECMAScriptCompiler::NIYStatement(char *niyName)
{
	char warnbuf[ANMMAXSTRBUF];
	
	sprintf(warnbuf, "Not yet implemented: %s statement", niyName);
	Yywarning(warnbuf);
	return &emptyStatement;
}



// code generation
void CAnmECMAScriptCompiler::GenStatement(CAnmESStatement *pStatement)
{
	std::vector<CAnmESStatement *> *pStatementList = NULL;
	std::vector<class CAnmESVarStatement *> *pVarStatementList = NULL;
	int i;

	switch (pStatement->GetType())
	{
		case eEmptyStatement :
			break;

		case eTopLevelStatement :
			GenTopLevelStatement(((CAnmESTopLevelStatement *)pStatement)->GetExpression());
			break;

		case eVarStatement :
			GenVarStatement((CAnmESVarStatement *)pStatement);
			break;

		case eVarStatementList :
			pVarStatementList = ((CAnmESVarStatementList *)pStatement)->GetStatements();
			assert(pVarStatementList);
			for (i = 0; i < pVarStatementList->size(); i++)
			{
				GenStatement((*pVarStatementList)[i]);
			}
			break;

		case eStatementBlock :
			pStatementList = ((CAnmESStatementBlock *)pStatement)->GetStatements();
			assert(pStatementList);
			for (i = 0; i < pStatementList->size(); i++)
			{
				GenStatement((*pStatementList)[i]);
			}
			break;

		case eIfStatement :
			GenIfStatement((CAnmESIfStatement *)pStatement);
			break;

		case eForStatement :
			GenForStatement((CAnmESForStatement *)pStatement);
			break;

		case eWhileStatement :
			GenWhileStatement((CAnmESWhileStatement *)pStatement);
			break;

		case eSwitchStatement :
			GenSwitchStatement((CAnmESSwitchStatement *)pStatement);
			break;

		case eBreakStatement :
			GenBreak();
			break;

		case eContinueStatement :
			GenContinue();
			break;

		case eWithStatement :
			break;

		case eReturnStatement :
			GenReturn(((CAnmESReturnStatement *)pStatement)->GetExpression());
			break;

		case eCaseStatement :
		default :
			assert(FALSE);
			break;
	}
}

void CAnmECMAScriptCompiler::GenTopLevelStatement(CAnmESExpression *pExpr)
{
	CAnmESAssignmentExpression *pAssignmentExpression = NULL;
	CAnmESExpression *pExpr1 = NULL;
	CAnmESExpression *pExpr2 = NULL;
	eESOperator op;

	// Top-level statement generation; once the code is generated we
	// can ditch the expression object

	switch (pExpr->GetExprType())
	{
		case eAssignmentExpression :

			// call our helper
			pAssignmentExpression = (CAnmESAssignmentExpression *) pExpr;
			pExpr1 = pAssignmentExpression->GetLHS();
			pExpr2 = pAssignmentExpression->GetRHS();
			op = pAssignmentExpression->GetOp();
			GenAssignment(pExpr1, op, pExpr2);
			break;

		case eConstExpression :
			// I think we can just ignore this
			break;

		case eLValueExpression :
		case eUnaryExpression :
		case eBinaryExpression :
		case eFunctionExpression :

			// Generate it (even though results don't go anywhere),
			// just for any potential side effects
			GenRValue(pExpr);
			break;

		default :
			assert (FALSE);			// shouldn't be here
			break;
	}
}

void CAnmECMAScriptCompiler::GenAssignment(CAnmESExpression *pLValueExpr,
										   eESOperator op,
										   CAnmESExpression *pRValueExpr)
{
	if (pLValueExpr->GetExprType() != eLValueExpression)
	{
		Yyerror("Attempt to use constant on left-hand side of assignment");
		return;
	}

	// generate LHS of assignment
	CAnmESLValueExpression *pLV = (CAnmESLValueExpression *) pLValueExpr;
	GenLValue(pLV);

	// for simple assignment, just get the RHS on the stack
	if (op == eOpASSIGNMENT)
		GenRValue(pRValueExpr);
	else
	{
		// if it's not simple assignment (e.g. *=), get the value of the LHS on the stack first
		GenRValue(pLV);

		if (op == eOpPREINCREMENT || op == eOpPREDECREMENT ||
			op == eOpPOSTINCREMENT || op == eOpPOSTDECREMENT)
		{
			// special-case for increment operators
			// R-value will be replaced with the op'ed value
			if (op == eOpPREDECREMENT || op == eOpPOSTDECREMENT)
				op = eOpDECREMENT;
			else
				op = eOpINCREMENT;
		}
		else
		{
			// now get the RHS on the stack
			GenRValue(pRValueExpr);
		}

		// generate the op here (e.g. *=)
		// it will replace the R-value with the op'ed value
		m_script->GetCode()->GenPush((ESSTACKVALUE) op);
		m_script->GetCode()->GenOpCall();
	}

	// now store it
	if (pLV->GetLValueType() == eLValueVar)
	{
		CAnmESVarSymbol *pVarSymbol = pLV->GetVarSymbol();
		if (pVarSymbol->GetVarType() == eLocalVariable)
			m_script->GetCode()->GenFStoreV();
		else if (pVarSymbol->GetVarType() == eGlobalVariable)
			m_script->GetCode()->GenStoreV();
		else
			assert(FALSE);
	}
	else if (pLV->GetLValueType() == eLValueMember)
		m_script->GetCode()->GenPutPropV();
	else // (pLV->GetLValueType() == eLValueArrayMember)
		m_script->GetCode()->GenPutArrayPropV();

}

void CAnmECMAScriptCompiler::GenFunctionCall(CAnmESFunctionExpression *pFnExpr)
{

	CAnmESTypeSymbol *pTypeSym = pFnExpr->GetTypeForNew();
	CAnmESLValueExpression *pLV = NULL;
	eESLValueType lvType;

	if (pTypeSym != NULL)
		m_script->GetCode()->GenPush((ESCODE) pTypeSym);
	else
	{
		pLV = (CAnmESLValueExpression *) pFnExpr->GetCallExpression();
		lvType = pLV->GetLValueType();

		if (lvType == eLValueMember || lvType == eLValueArrayMember)
			GenLValue(pLV);
		else
		{
			// if the LValue type says it's a variable, it's not really a variable;
			// it's a global subroutine in this context
			// set up for subroutine return
			m_script->GetCode()->GenSubReturnLinkage();

			// get 'this' on the stack; all procedures need it
			m_script->GetCode()->GenPush((ESSTACKVALUE) 0);
			m_script->GetCode()->GenFLoadV();
		}
	}

	ESExprArgs *pArgs = pFnExpr->GetArgs();
	int sz = pArgs->size();
	for (int nargs = 0; nargs < sz; nargs++)
	{
		GenRValue((*pArgs)[nargs]);
	}

	if (pTypeSym != NULL)
		m_script->GetCode()->GenNewOpNV(nargs);
	else
	{
		if (lvType == eLValueMember || lvType == eLValueArrayMember)
			m_script->GetCode()->GenCallMethodNV(nargs);
		else
		{
			int jumptoaddr = 0;

			if (lvType == eLValueFunction)
			{
				CAnmESFunctionSymbol *pFuncSymbol = pLV->GetFunctionSymbol();
				assert(pFuncSymbol);

				jumptoaddr = pFuncSymbol->GetProcAddress();
			}

			// generate jumpsub (maybe with a placeholder to fill in later)
			m_script->GetCode()->GenJumpSub();
			int jumpfromaddr = m_script->GetCode()->GetCurrentCodeAddress();
			m_script->GetCode()->Gen(jumptoaddr);

			// generate argument count
			m_script->GetCode()->Gen(nargs + 1);	// +1 for 'this'

			if (jumptoaddr == 0)
			{
				// add the call reference to list for later backfill
				String subName = pLV->GetVarSymbol()->GetName();
				m_script->AddCallRef(subName, jumpfromaddr);
			}
		}
	}
}

void CAnmECMAScriptCompiler::GenReturn(class CAnmESExpression *pReturnExpr)
{
	if (pReturnExpr)
		GenRValue(pReturnExpr);
	else
		m_script->GetCode()->GenPushV((AnmESVar)(double)0.f);

	m_script->GetCode()->GenReturn();
}

void CAnmECMAScriptCompiler::GenVarStatement(class CAnmESVarStatement *pVarStatement)
{
	CAnmESLValueExpression *pIDExpr = 
		(CAnmESLValueExpression *) pVarStatement->GetIDExpression();
	CAnmESExpression *pInitExpr = pVarStatement->GetInitExpression();
	
	if (pInitExpr)
	{
		// N.B.: Do we need to do anything if there's no initializer?!
		GenLValue(pIDExpr);
		GenRValue(pInitExpr);
	}
}

void CAnmECMAScriptCompiler::GenIfStatement(CAnmESIfStatement *pIfStatement)
{
	assert(pIfStatement);

	// Begin the IF: if (condition)
	CAnmESExpression *pCondition = pIfStatement->Condition();
	GenRValue(pCondition);

	// save this address for later backfill and generate a jump to the else
	// put placeholder in for now
	m_script->GetCode()->GenJumpFalse();
	int jumptoelse = m_script->GetCode()->GetCurrentCodeAddress();
	m_script->GetCode()->Gen(0L);

	// Generate statements for the IF block
	CAnmESStatement *pIfBranch = pIfStatement->IfBranch();
	if (pIfBranch)
		GenStatement(pIfBranch);

	// End of the IF block
	// save this address for later and generate a jump out
	// put 0 jump in for now; if there's no else it will be OK; if there is,
	// we'll backfill jump addr
	m_script->GetCode()->GenJump();
	int jumpfromif = m_script->GetCode()->GetCurrentCodeAddress();
	m_script->GetCode()->Gen(0L);

	// now backfill IF jump
	int jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumptoelse - 1;
	m_script->GetCode()->WriteAt(jumptoelse, jumpdist);

	// Generate statements for the ELSE block
	CAnmESStatement *pElseBranch = pIfStatement->ElseBranch();
	if (pElseBranch)
		GenStatement(pElseBranch);

	// calc distance from end of previous if statement
	jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumpfromif - 1;
	m_script->GetCode()->WriteAt(jumpfromif, jumpdist);
}

void CAnmECMAScriptCompiler::GenForStatement(class CAnmESForStatement *pForStatement)
{
	CAnmESExpression *pInitExpr = pForStatement->InitExpression();
	CAnmESExpression *pTestExpr = pForStatement->TestExpression();
	CAnmESExpression *pFinalExpr = pForStatement->FinalExpression();
	CAnmESStatement *pBody = pForStatement->GetBody();

	// Step 1: generate the initialization expression
	GenRValue(pInitExpr);

	// Step 2: Generate code to test the expression every time through
	int loopstart = m_script->GetCode()->GetCurrentCodeAddress();
	
	// Get the expression value onto the stack
	GenRValue(pTestExpr);
	// Generate a jump-false with placeholder 0 offset
	m_script->GetCode()->GenJumpFalse();
	int jumpfromtest = m_script->GetCode()->GetCurrentCodeAddress();
	m_script->GetCode()->Gen(0L);

	// Step 3: Generate the statement body
	// First, mark loop start for break/continue
	EnterLoop(loopstart, TRUE);
	GenStatement(pBody);

	// Step 4: Generate the final code
	int finalstart = m_script->GetCode()->GetCurrentCodeAddress();
	SetLoopFinalAddr(finalstart);
	GenRValue(pFinalExpr);

	// Step 5: Go back to loop start
	m_script->GetCode()->GenJump();
	int jumpdist = - (m_script->GetCode()->GetCurrentCodeAddress() - loopstart) - 1;
	m_script->GetCode()->Gen((UINT) jumpdist);

	// Step 6: Backfill the jump-false from initial test
	jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumpfromtest - 1;
	m_script->GetCode()->WriteAt(jumpfromtest, jumpdist);

	// Finally, mark loop end for break/continue
	LeaveLoop();
}

void CAnmECMAScriptCompiler::GenWhileStatement(class CAnmESWhileStatement *pWhileStatement)
{
	CAnmESExpression *pTestExpr = pWhileStatement->TestExpression();
	CAnmESStatement *pBody = pWhileStatement->GetBody();

	// Step 1: Generate code to test the expression every time through
	int loopstart = m_script->GetCode()->GetCurrentCodeAddress();
	
	// Get the expression value onto the stack
	GenRValue(pTestExpr);
	// Generate a jump-false with placeholder 0 offset
	m_script->GetCode()->GenJumpFalse();
	int jumpfromtest = m_script->GetCode()->GetCurrentCodeAddress();
	m_script->GetCode()->Gen(0L);

	// Step 2: Generate the statement body
	// First, mark loop start for break/continue
	EnterLoop(loopstart, FALSE);
	GenStatement(pBody);

	// Step 3: Go back to loop start
	m_script->GetCode()->GenJump();
	int jumpdist = - (m_script->GetCode()->GetCurrentCodeAddress() - loopstart) - 1;
	m_script->GetCode()->Gen((UINT) jumpdist);

	// Step 4: Backfill the jump-false from initial test
	jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumpfromtest - 1;
	m_script->GetCode()->WriteAt(jumpfromtest, jumpdist);

	// Finally, mark loop end for break/continue
	LeaveLoop();
}

void CAnmECMAScriptCompiler::GenBreak()
{
	if (!m_inCase)		// this is handled elsewhere, see GenCaseBlock()
	{
		if (!m_loopLevels.empty())
		{
			sAnmLoopBlock *pLoopBlock = m_loopLevels.top();

			// Generate a jump out, with 0 placeholder that will be backfilled at the end of the loop
			m_script->GetCode()->GenJump();
			int jumpfromaddr = m_script->GetCode()->GetCurrentCodeAddress();
			m_script->GetCode()->Gen(0L);

			pLoopBlock->breakJumps.push_back(jumpfromaddr);
		}
		else
		{
			// N.B.: warning here
		}
	}
}

void CAnmECMAScriptCompiler::GenContinue()
{
	if (!m_loopLevels.empty())
	{
		sAnmLoopBlock *pLoopBlock = m_loopLevels.top();

		if (pLoopBlock->forLoop)
		{
			// generate a jump forward to the loop final code, with placeholder
			m_script->GetCode()->GenJump();
			int jumpfromaddr = m_script->GetCode()->GetCurrentCodeAddress();
			m_script->GetCode()->Gen(0L);

			pLoopBlock->continueJumps.push_back(jumpfromaddr);
		}
		else
		{
			// generate a jump back to the loop start
			m_script->GetCode()->GenJump();
			int jumpdist = - (m_script->GetCode()->GetCurrentCodeAddress() - pLoopBlock->loopStartAddr) - 1;
			m_script->GetCode()->Gen((UINT) jumpdist);
		}
	}
	else
	{
		// N.B.: warning here
	}
}

void CAnmECMAScriptCompiler::EnterLoop(int loopStartAddr, BOOL forLoop)
{
	sAnmLoopBlock *pLoopBlock = new sAnmLoopBlock;
	pLoopBlock->forLoop = forLoop;
	pLoopBlock->loopStartAddr = loopStartAddr;
	m_loopLevels.push(pLoopBlock);
}

void CAnmECMAScriptCompiler::LeaveLoop()
{
	assert(!m_loopLevels.empty());

	sAnmLoopBlock *pLoopBlock = m_loopLevels.top();

	for (int i = 0; i < pLoopBlock->breakJumps.size(); i++)
	{
		int jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - pLoopBlock->breakJumps[i] - 1;
		m_script->GetCode()->WriteAt(pLoopBlock->breakJumps[i], jumpdist);
	}

	for (i = 0; i < pLoopBlock->continueJumps.size(); i++)
	{
		int jumpdist =  pLoopBlock->loopFinalAddr - pLoopBlock->continueJumps[i] - 1;
		m_script->GetCode()->WriteAt(pLoopBlock->continueJumps[i], jumpdist);
	}

	delete pLoopBlock;

	m_loopLevels.pop();
}

void CAnmECMAScriptCompiler::SetLoopFinalAddr(int loopFinalAddr)
{
	assert(!m_loopLevels.empty());

	sAnmLoopBlock *pLoopBlock = m_loopLevels.top();
	assert(pLoopBlock->forLoop);
	pLoopBlock->loopFinalAddr = loopFinalAddr;
}

void CAnmECMAScriptCompiler::GenSwitchStatement(class CAnmESSwitchStatement *pSwitchStatement)
{
	assert(m_functionDeclaring);

	CAnmESExpression *pTestExpr = pSwitchStatement->GetTestExpression();

	// Step 1: store test expression into a temp for retesting
	// Alloc a temp
	int tempOffset = m_functionDeclaring->AddTemp();
	m_script->GetCode()->GenPush((ESSTACKVALUE) tempOffset);

	// get the expression valueonto the stack
	GenRValue(pTestExpr);

	// store it
	m_script->GetCode()->GenFStoreV();

	CAnmESStatementBlock *pCaseBlock = pSwitchStatement->GetCaseBlock();
	std::vector<CAnmESStatement *> *pCaseStatements = 
		pCaseBlock ? pCaseBlock->GetStatements() : NULL;

	// Step 2: generate all the tests and jump-froms
	int jumpdist;

	std::vector<int> jumpins;
	int jumpinaddr;
	for (int i = 0; pCaseStatements && (i < pCaseStatements->size()); i++)
	{
		CAnmESStatement *pStatement = (*pCaseStatements)[i];

		assert(pStatement->GetType() == eCaseStatement);

		CAnmESCaseStatement *pCaseStatement = (CAnmESCaseStatement *) pStatement;

		if (pCaseStatement->IsDefaultCase())
		{
			// for default case, generate unconditional jump (with placeholder)
			// generate the jump, with 0 offset as a placeholder
			m_script->GetCode()->GenJump();
		}
		else
		{
			// otherwise do a case-by-case compare

			CAnmESExpression *pMatchExpr = pCaseStatement->GetMatchExpression();

			// get the test expression on the stack
			m_script->GetCode()->GenPush((ESSTACKVALUE) tempOffset);
			m_script->GetCode()->GenFLoadV();

			// get the match expression on the stack
			GenRValue(pMatchExpr);

			// compare the values by generating an == op
			// Load the op
			m_script->GetCode()->GenPush((ESSTACKVALUE) eOpEQUALS);
			// Call the op
			m_script->GetCode()->GenOpCall();

			// generate the jump, with 0 offset as a placeholder
			m_script->GetCode()->GenJumpTrue();
		}

		jumpinaddr = m_script->GetCode()->GetCurrentCodeAddress();
		m_script->GetCode()->Gen(0L);

		jumpins.push_back(jumpinaddr);
	}

	// Step 3: after all that, we need a skip-around case, too
	m_script->GetCode()->GenJump();
	int jumpskipcases = m_script->GetCode()->GetCurrentCodeAddress();
	m_script->GetCode()->Gen(0L);

	// Step 4: now generate all the statements, jump-tos and jump-outs
	std::vector<int> jumpouts;
	int jumpoutaddr;
	for (i = 0; pCaseStatements && (i < pCaseStatements->size()); i++)
	{
		CAnmESStatement *pStatement = (*pCaseStatements)[i];

		assert(pStatement->GetType() == eCaseStatement);

		CAnmESCaseStatement *pCaseStatement = (CAnmESCaseStatement *) pStatement;

		// Backfill the jump from our test (== TRUE) into the start of the code
		jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumpins[i] - 1;
		m_script->GetCode()->WriteAt(jumpins[i], jumpdist);

		// Get our generic helper to generate code for the block
		BOOL gotBreak = GenCaseBlock(pCaseStatement);

		// If there was a break statement, we need to generate a jump-out
		if (gotBreak || pCaseStatement->IsDefaultCase())
		{
			// generate an unconditional jump, with 0 offset as a placeholder
			m_script->GetCode()->GenJump();
			jumpoutaddr = m_script->GetCode()->GetCurrentCodeAddress();
			m_script->GetCode()->Gen(0L);

			jumpouts.push_back(jumpoutaddr);
		}

	}

	// Step 5: Backfill the jumpouts
	for (i = 0; i < jumpouts.size(); i++)
	{
		jumpoutaddr = jumpouts[i];
		jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumpoutaddr - 1;
		m_script->GetCode()->WriteAt(jumpoutaddr, jumpdist);
		
	}

	// Step 6: Backfill the skip-around case
	jumpdist = m_script->GetCode()->GetCurrentCodeAddress() - jumpskipcases - 1;
	m_script->GetCode()->WriteAt(jumpskipcases, jumpdist);

}

BOOL CAnmECMAScriptCompiler::GenCaseBlock(CAnmESCaseStatement *pCaseStatement)
{
	std::vector<CAnmESStatement *> *pStatementList = 
		pCaseStatement->GetStatements();

	BOOL gotBreak = FALSE;
	for (int i = 0; i < pStatementList->size(); i++)
	{
		CAnmESStatement *pStatement = (*pStatementList)[i];
		
		if (pStatement->GetType() == eBreakStatement)
		{
			gotBreak = TRUE;
			break;		// heh
		}

		GenStatement(pStatement);
	}

	if (i < pStatementList->size())
	{
		// warning here about skipped statements
	}

	return gotBreak;
}


void CAnmECMAScriptCompiler::EnterCase()
{
	m_inCase = TRUE;
}

void CAnmECMAScriptCompiler::LeaveCase()
{
	m_inCase = FALSE;
}

void CAnmECMAScriptCompiler::GenLValue(class CAnmESLValueExpression *pLValueExpr)
{
	CAnmESVarSymbol *pVarSymbol = NULL;
	CAnmESExpression *pObjectExpression = NULL;
	CAnmESExpression *pArrayMemberExpression = NULL;
	char *memberName = NULL;
	ESLOCALOFFSET localOffset = ESBADLOCALADDR;
	void *globaladdr = (void *) ESBADLOCALADDR;

	switch (pLValueExpr->GetLValueType())
	{
		case eLValueVar :
			pVarSymbol = pLValueExpr->GetVarSymbol();
			
			if (pVarSymbol->GetVarType() == eLocalVariable)
			{
				localOffset = pVarSymbol->GetLocalOffset();
				assert(localOffset != ESBADLOCALADDR);

				m_script->GetCode()->GenPush((ESSTACKVALUE) localOffset);
			}
			else if (pVarSymbol->GetVarType() == eGlobalVariable)
			{
				globaladdr = pVarSymbol->GetGlobalAddress();
				assert(globaladdr != (void *) ESBADLOCALADDR &&
					globaladdr != NULL);
				m_script->GetCode()->GenPush((ESSTACKVALUE) globaladdr);
			}
			else
				assert(FALSE);

			break;

		case eLValueMember :
			pObjectExpression = pLValueExpr->GetObjectExpression();
			// Load the object
			GenRValue(pObjectExpression);
			// Load the member name
			memberName = pLValueExpr->GetMemberName()->GetBuf();
			m_script->GetCode()->GenPush((ESSTACKVALUE) memberName);			// N.B.: heap?
			break;

		case eLValueArrayMember :
			pObjectExpression = pLValueExpr->GetObjectExpression();
			// Load the object
			GenRValue(pObjectExpression);
			// Load the array subscript
			pArrayMemberExpression = pLValueExpr->GetArrayMemberExpression();
			GenRValue(pArrayMemberExpression);
			break;

		default :
			assert (FALSE);			// shouldn't be here
			break;
	}
}

void CAnmECMAScriptCompiler::GenRValue(class CAnmESExpression *pExpr)
{
	AnmESVar val;
	CAnmESLValueExpression *pLValueExpr = NULL;
	CAnmESExpression *pObjectExpression = NULL;
	CAnmESExpression *pArrayMemberExpression = NULL;
	char *memberName = NULL;
	CAnmESVarSymbol *pVarSymbol = NULL;
	CAnmESUnaryExpression *pUnaryExpression = NULL;
	CAnmESBinaryExpression *pBinaryExpression = NULL;
	CAnmESAssignmentExpression *pAssignmentExpression = NULL;
	CAnmESExpression *pExpr1 = NULL;
	CAnmESExpression *pExpr2 = NULL;
	eESOperator op;

	switch ( ( pExpr ) ? pExpr->GetExprType() : -1 )
	{
		case eConstExpression :
			val = ((CAnmESConstExpression *) pExpr)->GetValue();
			m_script->GetCode()->GenPushV(val);
			break;

		case eLValueExpression :
			pLValueExpr = (CAnmESLValueExpression *) pExpr;
			if (pLValueExpr->GetLValueType() == eLValueVar)
			{
				// Load local variable from stack frame
				pVarSymbol = pLValueExpr->GetVarSymbol();
				if (pVarSymbol->GetVarType() == eLocalVariable)
				{
					// Push address
					m_script->GetCode()->GenPush((ESSTACKVALUE) pVarSymbol->GetLocalOffset());
					// Get the value
					m_script->GetCode()->GenFLoadV();
				}
				else if (pVarSymbol->GetVarType() == eGlobalVariable)
				{
					// Push address
					m_script->GetCode()->GenPush((ESSTACKVALUE) pVarSymbol->GetGlobalAddress());
					// Get the value
					m_script->GetCode()->GenLoadV();
				}
				else
					assert(FALSE);
			}
			else if (pLValueExpr->GetLValueType() == eLValueMember)
			{
				// Load member property

				// Load the object
				pObjectExpression = pLValueExpr->GetObjectExpression();
				GenRValue(pObjectExpression);

				// Load the member name
				memberName = pLValueExpr->GetMemberName()->GetBuf();
				m_script->GetCode()->GenPush((ESSTACKVALUE) memberName);			// N.B.: heap?

				// Get the property value
				m_script->GetCode()->GenGetPropV();
			}
			else // (pLV->GetLValueType() == eLValueArrayMember)
			{
				// load member array property

				// Load the object
				pObjectExpression = pLValueExpr->GetObjectExpression();
				if( pObjectExpression == NULL ) {
					Yyerror("Null Object Expression");
				}
				GenRValue(pObjectExpression);

				// Load the array subscript
				pArrayMemberExpression = pLValueExpr->GetArrayMemberExpression();
				GenRValue(pArrayMemberExpression);

				// Get the array property value
				m_script->GetCode()->GenGetArrayPropV();
			}
			break;

		case eUnaryExpression :
			pUnaryExpression = (CAnmESUnaryExpression *) pExpr;
			pExpr1 = pUnaryExpression->GetBaseExpression();
			op = pUnaryExpression->GetOp();
			
			if (op == eOpPREINCREMENT || op == eOpPREDECREMENT ||
				op == eOpPOSTINCREMENT || op == eOpPOSTDECREMENT)
			{
				// special-case increment and decrement operators; they are more
				// than just R-Values. Use assignment helper to do it

				GenAssignment(pExpr1, op, NULL);

				// N.B.: that really doesn't handle post-increment properly; we
				// need to think about that


				if (op == eOpPREDECREMENT || op == eOpPOSTDECREMENT)
					op = eOpDECREMENT;
				else
					op = eOpINCREMENT;
			}

			// Load the value
			GenRValue(pExpr1);
			// Load the op
			m_script->GetCode()->GenPush((ESSTACKVALUE) op);
			// Call it
			m_script->GetCode()->GenOpCall();

			break;

		case eBinaryExpression :
			pBinaryExpression = (CAnmESBinaryExpression *) pExpr;
			pExpr1 = pBinaryExpression->GetBaseExpression1();
			pExpr2 = pBinaryExpression->GetBaseExpression2();
			op = pBinaryExpression->GetOp();
			// Load the values
			GenRValue(pExpr1);
			GenRValue(pExpr2);
			// Load the op
			m_script->GetCode()->GenPush((ESSTACKVALUE) op);
			// Call it
			m_script->GetCode()->GenOpCall();
			break;

		case eAssignmentExpression :
			pAssignmentExpression = (CAnmESAssignmentExpression *) pExpr;
			pExpr1 = pAssignmentExpression->GetLHS();
			pExpr2 = pAssignmentExpression->GetRHS();
			op = pAssignmentExpression->GetOp();
			GenAssignment(pExpr1, op, pExpr2);
			// now put our RValue on the stack again, for that a = b = c nonsense
			GenRValue(pExpr1);
			break;

		case eFunctionExpression :
			GenFunctionCall((CAnmESFunctionExpression *) pExpr);
			break;

		default :
			assert (FALSE);			// shouldn't be here
			break;
	}

}


// Utility methods
void CAnmECMAScriptCompiler::WriteToConsole(char *str)
{
	if (m_app && m_consoleOutput)
		m_app->WriteToConsole(str);
}
