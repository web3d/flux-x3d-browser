/********************************************************************************
 * Flux
 *
 * File: anmesstatement.h
 * Description: ECMAScript compiler statements
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

#ifndef _anmesstatement_h
#define _anmesstatement_h

#include "anmecmascript.h"

typedef enum eESStatementType
{
	eEmptyStatement,
	eTopLevelStatement,
	eVarStatement,
	eVarStatementList,
	eStatementBlock,
	eIfStatement,
	eForStatement,
	eWhileStatement,
	eBreakStatement,
	eContinueStatement,
	eSwitchStatement,
	eCaseStatement,
	eWithStatement,
	eReturnStatement,
} eESStatementType ;

class CAnmESStatement
{
protected :

	eESStatementType	m_type;

public :

	CAnmESStatement(eESStatementType type)
	{
		m_type = type;
	}

	eESStatementType GetType()
	{
		return m_type;
	}
};

class CAnmESEmptyStatement : public CAnmESStatement
{
protected :

public :

	CAnmESEmptyStatement() : CAnmESStatement(eEmptyStatement)
	{
	}
};

class CAnmESTopLevelStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression *m_expr;

public :
	CAnmESTopLevelStatement(class CAnmESExpression *pExpr)
		: CAnmESStatement(eTopLevelStatement),
		m_expr(pExpr)
	{
	}

	// Accessors
	class CAnmESExpression *GetExpression()
	{
		return m_expr;
	}
};

class CAnmESVarStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression	*m_idExpression;
	class CAnmESExpression	*m_initExpression;

public :
	CAnmESVarStatement(class CAnmESExpression *pIdExpr, class CAnmESExpression *pInitExpr = NULL)
		: CAnmESStatement(eVarStatement),
		m_idExpression(pIdExpr),
		m_initExpression(pInitExpr)
	{
	}

	// Accessors
	class CAnmESExpression *GetIDExpression()
	{
		return m_idExpression;
	}

	class CAnmESExpression *GetInitExpression()
	{
		return m_initExpression;
	}

};

class CAnmESVarStatementList : public CAnmESStatement
{
protected :

	std::vector<class CAnmESVarStatement *>		m_statements;

public :
	CAnmESVarStatementList(class CAnmESStatement *pStatement = NULL)
		: CAnmESStatement(eVarStatementList)
	{
		if (pStatement)
		{
			assert(pStatement->GetType() == eVarStatement);
			m_statements.push_back((CAnmESVarStatement *) pStatement);
		}
	}

	void AddStatement(class CAnmESStatement *pStatement)
	{
		assert(pStatement);
		assert(pStatement->GetType() == eVarStatement);
		m_statements.push_back((CAnmESVarStatement *) pStatement);
	}

	// Accessors
	std::vector<class CAnmESVarStatement *> *GetStatements()
	{
		return &m_statements;
	}
};

class CAnmESStatementBlock : public CAnmESStatement
{
protected :

	std::vector<class CAnmESStatement *>		m_statements;

public :
	CAnmESStatementBlock()
		: CAnmESStatement(eStatementBlock)
	{
	}

	void AddStatement(class CAnmESStatement *pStatement)
	{
		assert(pStatement);
		m_statements.push_back(pStatement);
	}

	// Accessors
	std::vector<class CAnmESStatement *> *GetStatements()
	{
		return &m_statements;
	}
};

class CAnmESIfStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression		*m_condition;
	CAnmESStatement				*m_ifbranch;
	CAnmESStatement				*m_elsebranch;

	int							 m_jumpFromIf;
	int							 m_jumpToElse;

public :

	CAnmESIfStatement(class CAnmESExpression *pCondition, CAnmESStatement *pIfBranch,
		CAnmESStatement *pElseBranch)
		: CAnmESStatement(eIfStatement),
		m_condition(pCondition),
		m_ifbranch(pIfBranch),
		m_elsebranch(pElseBranch)
	{
	}

	// Accessors
	class CAnmESExpression *Condition()
	{
		return m_condition;
	}

	CAnmESStatement *IfBranch()
	{
		return m_ifbranch;
	}

	CAnmESStatement *ElseBranch()
	{
		return m_elsebranch;
	}

	void SetJumpFromIf(int jumpFromIf)
	{
		m_jumpFromIf = jumpFromIf;
	}

	int GetJumpFromIf()
	{
		return m_jumpFromIf;
	}

	void SetJumpToElse(int jumpToElse)
	{
		m_jumpToElse = jumpToElse;
	}


	int GetJumpToElse()
	{
		return m_jumpToElse;
	}

};

class CAnmESForStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression			*m_initExpr;
	class CAnmESExpression			*m_testExpr;
	class CAnmESExpression			*m_finalExpr;
	CAnmESStatement					*m_body;

public :

	CAnmESForStatement(class CAnmESExpression *pInitExpr, class CAnmESExpression *pTestExpr,
		class CAnmESExpression *pFinalExpr, CAnmESStatement *pBody)
		: CAnmESStatement(eForStatement),
		m_initExpr(pInitExpr),
		m_testExpr(pTestExpr),
		m_finalExpr(pFinalExpr),
		m_body(pBody)
	{
	}

	// Accessors
	class CAnmESExpression *InitExpression()
	{
		return m_initExpr;
	}

	class CAnmESExpression *TestExpression()
	{
		return m_testExpr;
	}

	class CAnmESExpression *FinalExpression()
	{
		return m_finalExpr;
	}

	CAnmESStatement *GetBody()
	{
		return m_body;
	}
};

class CAnmESWhileStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression			*m_testExpr;
	CAnmESStatement					*m_body;

public :

	CAnmESWhileStatement(class CAnmESExpression *pTestExpr, CAnmESStatement *pBody)
		: CAnmESStatement(eWhileStatement),
		m_testExpr(pTestExpr),
		m_body(pBody)
	{
	}

	// Accessors
	class CAnmESExpression *TestExpression()
	{
		return m_testExpr;
	}

	CAnmESStatement *GetBody()
	{
		return m_body;
	}
};

class CAnmESBreakStatement : public CAnmESStatement
{
protected :

public :

	CAnmESBreakStatement()
		: CAnmESStatement(eBreakStatement)
	{
	}
};

class CAnmESContinueStatement : public CAnmESStatement
{
protected :

public :

	CAnmESContinueStatement()
		: CAnmESStatement(eContinueStatement)
	{
	}
};

class CAnmESSwitchStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression				*m_testExpr;
	class CAnmESStatementBlock			*m_caseblock;

public :

	CAnmESSwitchStatement(class CAnmESExpression *pTestExpr, CAnmESStatement *pStatement)
		: CAnmESStatement(eSwitchStatement),
		m_testExpr(pTestExpr)
	{
		if (pStatement->GetType() == eEmptyStatement)
		{
			m_caseblock = NULL;
			return;
		}

		assert(pStatement->GetType() == eStatementBlock);
		m_caseblock = (class CAnmESStatementBlock *) pStatement;
	}

	// Accessors
	class CAnmESExpression *GetTestExpression()
	{
		return m_testExpr;
	}

	CAnmESStatementBlock *GetCaseBlock()
	{
		return m_caseblock;
	}
};


class CAnmESCaseStatement : public CAnmESStatementBlock
{
protected :

	class CAnmESExpression			*m_matchexpression;
	BOOL							 m_isDefault;

public :

	CAnmESCaseStatement(class CAnmESExpression *pMatchExpr, BOOL isDefault)
		: CAnmESStatementBlock(),
		m_matchexpression(pMatchExpr),
		m_isDefault(isDefault)
	{
		m_type = eCaseStatement;
		
		if (pMatchExpr == NULL)
			assert(isDefault);
		else
			assert(!isDefault);
	}

	// Accessors
	class CAnmESExpression *GetMatchExpression()
	{
		return m_matchexpression;
	}

	BOOL IsDefaultCase()
	{
		return m_isDefault;
	}
};

class CAnmESReturnStatement : public CAnmESStatement
{
protected :

	class CAnmESExpression *m_expr;

public :
	CAnmESReturnStatement(class CAnmESExpression *pExpr = NULL)
	   : CAnmESStatement(eReturnStatement),
		m_expr(pExpr)
	{
	}

	// Accessors
	class CAnmESExpression *GetExpression()
	{
		return m_expr;
	}
};

#endif // _anmesstatement_h


