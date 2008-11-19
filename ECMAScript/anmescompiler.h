/********************************************************************************
 * Flux
 *
 * File: anmescompiler.h
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

#ifndef _anmescompiler_h
#define _anmescompiler_h

#include "anmecmascript.h"
#include "anmesstackmachine.h"

// YACC stuff
#define YYSTYPE ANMESYYSTYPE
typedef union  {
	class CAnmESExpression *pExpr;
	class CAnmESStatement *pStatement;
	class CAnmESString *strVal;
	String idVal;
	double doubleVal;
	eESOperator op;

} YYSTYPE;
#define YYMAXDEPTH 1024

#define yychar ANMESyychar
#define yynerrs ANMESnerrs

typedef struct sAnmLoopBlock
{
	BOOL					forLoop;
	int						loopStartAddr;
	int						loopFinalAddr;
	std::vector<int>		breakJumps;
	std::vector<int>		continueJumps;
} sAnmLoopBlock ;

class CAnmECMAScriptCompiler
{
protected :

	// YACC data type support
	YYSTYPE							m_Yylval;
	YYSTYPE							m_Yyval;
	int								m_Pcyytoken;
	int								m_Pcyyerrfl;
	int								m_Pcyyerrct;
	YYSTYPE							m_Yyv[YYMAXDEPTH];


	class cApplication				*m_app;
	class CAnmECMAScriptLanguageBinding	*m_owner;
	class CAnmECMAScriptLexer		*m_lexer;
	BOOL							m_consoleOutput;
	BOOL							m_warnUndeclared;

	// code generation variables

	FILE *fp;

	// the result of compilation: a new Script
	class CAnmESScript				*m_script;

	class CAnmESFunctionSymbol		*m_functionDeclaring;
	class CAnmESFunctionExpression	*m_functionCalling;

	class CAnmESVarStatementList	*m_variablesDeclaring;

	int								m_jumptoelse;
	int								m_jumpfromif;

	std::stack<class CAnmESFunctionExpression *> m_callLevels;
	std::stack<class CAnmESStatementBlock *>	m_blockLevels;
	std::stack<sAnmLoopBlock *>					m_loopLevels;

	int								m_inCase;

	int Yyparse();					// defined in yacc-generated file

	void Yyerror(char* message);
	void Yywarning(char* message);
	int Yylex();

	// expression support
	virtual class CAnmESExpression *ThisExpression();
	virtual class CAnmESExpression *IdentifierExpression(String name);
	virtual class CAnmESExpression *UnaryExpression(eESOperator op,
		 class CAnmESExpression *pExpr);
	virtual class CAnmESExpression *BinaryExpression(class CAnmESExpression *pExpr1,
		eESOperator op,
		class CAnmESExpression *pExpr2);
	virtual class CAnmESExpression *AssignmentExpression(class CAnmESExpression *pLValueExpr, 
		eESOperator op, class CAnmESExpression *pRValueExpr);
	virtual void BeginFunctionCall(class CAnmESExpression *pExpr);
	virtual class CAnmESFunctionExpression *EndFunctionCall();
	virtual BeginNew(String name);
	virtual class CAnmESFunctionExpression *EndNew();
	virtual void AddArgument(class CAnmESExpression *pExpr);


	// declarations
	virtual void BeginDeclareFunction(String name);
	virtual void EndDeclareFunction(class CAnmESStatement *pStatement);
	virtual void DeclareParam(String name);

	virtual void BeginBlock();
	virtual class CAnmESStatementBlock *EndBlock();
	virtual void AddStatement(class CAnmESStatement *pStatement);

	virtual void BeginVariableDeclarationList();
	virtual class CAnmESVarStatementList *EndVariableDeclarationList();
	virtual void AddVariableDeclaration(class CAnmESStatement *pStatement);

	virtual void BeginCaseBlock(class CAnmESExpression *pExpr, BOOL isDefault = FALSE);
	virtual class CAnmESStatement *EndCaseBlock();

	virtual class CAnmESEmptyStatement *EmptyStatement();
	virtual class CAnmESBreakStatement *BreakStatement();
	virtual class CAnmESContinueStatement *ContinueStatement();

	virtual class CAnmESStatement *NIYStatement(char *niyName);

	// code generation
	virtual void GenStatement(class CAnmESStatement *pStatement);
	virtual void GenTopLevelStatement(CAnmESExpression *pExpr);
	virtual void GenAssignment(class CAnmESExpression *pLValueExpr, 
		eESOperator op, class CAnmESExpression *pRValueExpr);

	virtual void GenFunctionCall(class CAnmESFunctionExpression *pFnExpr);

	virtual void GenReturn(class CAnmESExpression *pReturnExpr);

	virtual void GenIfStatement(class CAnmESIfStatement *pIfStatement);

	virtual void GenVarStatement(class CAnmESVarStatement *pVarStatement);
	virtual void GenForStatement(class CAnmESForStatement *pForStatement);
	virtual void GenWhileStatement(class CAnmESWhileStatement *pWhileStatement);
	virtual void GenBreak();
	virtual void GenContinue();
	virtual void EnterLoop(int loopStartAddr, BOOL forLoop);
	virtual void LeaveLoop();
	virtual void SetLoopFinalAddr(int loopFinalAddr);

	virtual void GenSwitchStatement(class CAnmESSwitchStatement *pSwitchStatement);
	virtual BOOL GenCaseBlock(class CAnmESCaseStatement *pCaseStatement);
	virtual void EnterCase();
	virtual void LeaveCase();

	virtual void GenLValue(class CAnmESLValueExpression *pLValueExpr);
	virtual void GenRValue(class CAnmESExpression *pExpr);


	virtual void AddGlobals(class CAnmECMAScriptLanguageBinding *pOwner);
	virtual void AddFunctions(class CAnmECMAScriptLanguageBinding *pOwner);
	virtual void AddTypeSymbols(class CAnmECMAScriptLanguageBinding *pOwner);

public :

	CAnmECMAScriptCompiler(cApplication *pApp, class CAnmECMAScriptLanguageBinding *pOwner,
		BOOL consoleOutput = TRUE);
	virtual ~CAnmECMAScriptCompiler();

	virtual eAnmReturnStatus Compile(const char *str, BOOL fromFile);
	virtual eAnmReturnStatus CompileString(const char *str)
	{
		return Compile(str, FALSE);
	}

	virtual eAnmReturnStatus CompileFile(const char *str)
	{
		return Compile(str, TRUE);
	}

	// symbol handling
	virtual void AddSymbol(class CAnmESSymbol *pSymbol);
	virtual class CAnmESSymbol *FindSymbol(char* str);

	// Accessors
	virtual class CAnmESScript *GetScript()
	{
		return m_script;
	}

	virtual class CAnmECMAScriptLanguageBinding *GetOwner()
	{
		return m_owner;
	}

	// Utility methods
	virtual void WriteToConsole(char *str);

	friend class CAnmECMAScriptLexer;
};



#endif // _anmescompiler_h
