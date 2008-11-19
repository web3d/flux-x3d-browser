/********************************************************************************
 * Flux
 *
 * File: anmesexpression.h
 * Description: ECMAScript compiler expressions
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

#ifndef _anmesexpression_h
#define _anmesexpression_h

#include "anmecmascript.h"

typedef enum eESExpressionType
{
	eConstExpression,
	eLValueExpression,
	eUnaryExpression,
	eBinaryExpression,
	eAssignmentExpression,
	eFunctionExpression,
} eESExpressionType ;

class CAnmESExpression
{
protected :

	eESExpressionType		m_exprType;
	eESValueType			m_valueType;

public :

	CAnmESExpression(eESExpressionType exprType, eESValueType valueType)
	{
		m_exprType = exprType;
		m_valueType = valueType;
	}

	~CAnmESExpression()
	{
	}

	// Accessors
	void SetExprType(eESExpressionType exprType)
	{
		m_exprType = exprType;
	}

	eESExpressionType GetExprType()
	{
		return m_exprType;
	}

	void SetValueType(eESValueType valueType)
	{
		m_valueType = valueType;
	}

	eESValueType GetValueType()
	{
		return m_valueType;
	}
};

class CAnmESConstExpression : public CAnmESExpression
{
	AnmESVar	m_value;

public :

	CAnmESConstExpression(AnmESVar val) :
	   CAnmESExpression(eConstExpression, val.valueType)
	   {
		   m_value = val;
	   }

	// Accessors
	void SetValue(AnmESVar val)
	{
		m_value = val;
		m_valueType = val.valueType;
	}

	AnmESVar GetValue()
	{
		return m_value;
	}

};

typedef enum eESLValueType {
	eLValueVar,
	eLValueMember,
	eLValueArrayMember,
	eLValueFunction,
} eESLValueType ;

class CAnmESLValueExpression : public CAnmESExpression
{
	class CAnmESVarSymbol		*m_varSymbol;
	CAnmESExpression			*m_objectExpr;
	String						m_memberName;
	CAnmESExpression			*m_arrayMemberExpr;
	class CAnmESFunctionSymbol	*m_funcSymbol;
	eESLValueType				 m_LValueType;

public :

	CAnmESLValueExpression(class CAnmESVarSymbol *pVarSymbol) :
	   CAnmESExpression(eLValueExpression, eUndefined)
	   {
		   m_varSymbol = pVarSymbol;
		   m_objectExpr = NULL;
		   m_memberName = NULL;
		   m_arrayMemberExpr = NULL;
		   m_funcSymbol = NULL;
		   m_LValueType = eLValueVar;
	   }

	CAnmESLValueExpression(CAnmESExpression *pObjectExpr, String memberName) :
	   CAnmESExpression(eLValueExpression, eUndefined)
	   {
		   m_objectExpr = pObjectExpr;
		   m_memberName = memberName;
		   m_memberName->Ref();
		   m_varSymbol = NULL;
		   m_arrayMemberExpr = NULL;
		   m_funcSymbol = NULL;
		   m_LValueType = eLValueMember;
	   }

	CAnmESLValueExpression(CAnmESExpression *pObjectExpr, CAnmESExpression *pArrayMemberExpr) :
	   CAnmESExpression(eLValueExpression, eUndefined)
	   {
		   m_objectExpr = pObjectExpr;
		   m_arrayMemberExpr = pArrayMemberExpr;
		   m_memberName = NULL;
		   m_varSymbol = NULL;
		   m_funcSymbol = NULL;
		   m_LValueType = eLValueArrayMember;
	   }

	CAnmESLValueExpression(class CAnmESFunctionSymbol *pFuncSymbol) :
	   CAnmESExpression(eLValueExpression, eUndefined)
	   {
		   m_varSymbol = NULL;
		   m_objectExpr = NULL;
		   m_memberName = NULL;
		   m_arrayMemberExpr = NULL;
		   m_funcSymbol = pFuncSymbol;
		   m_LValueType = eLValueFunction;
	   }

	~CAnmESLValueExpression()
	{
		SafeUnRef(m_memberName);
	}

	// Accessors
	eESLValueType GetLValueType()
	{
		return m_LValueType;
	}

	class CAnmESVarSymbol *GetVarSymbol()
	{
		return m_varSymbol;
	}

	CAnmESExpression *GetObjectExpression()
	{
		return m_objectExpr;
	}

	CAnmESExpression *GetArrayMemberExpression()
	{
		return m_arrayMemberExpr;
	}

	String GetMemberName()
	{
		return m_memberName;
	}

	class CAnmESFunctionSymbol *GetFunctionSymbol()
	{
		return m_funcSymbol;
	}

};


class CAnmESUnaryExpression : public CAnmESExpression
{
	CAnmESExpression			*m_baseExpr;
	eESOperator					 m_operator;

public :

	CAnmESUnaryExpression(CAnmESExpression *pBaseExpr, eESOperator op) :
	   CAnmESExpression(eUnaryExpression, eUndefined)
	   {
		   m_baseExpr = pBaseExpr;
		   m_operator = op;
	   }

	// Accessors
	eESOperator GetOp()
	{
		return m_operator;
	}

	CAnmESExpression *GetBaseExpression()
	{
		return m_baseExpr;
	}
};

class CAnmESBinaryExpression : public CAnmESExpression
{
	CAnmESExpression			*m_expr1;
	CAnmESExpression			*m_expr2;
	eESOperator					 m_operator;

public :

	CAnmESBinaryExpression(CAnmESExpression *pExpr1, CAnmESExpression *pExpr2, eESOperator op) :
	   CAnmESExpression(eBinaryExpression, eUndefined)
	   {
		   m_expr1 = pExpr1;
		   m_expr2 = pExpr2;
		   m_operator = op;
	   }

	// Accessors
	eESOperator GetOp()
	{
		return m_operator;
	}

	CAnmESExpression *GetBaseExpression1()
	{
		return m_expr1;
	}

	CAnmESExpression *GetBaseExpression2()
	{
		return m_expr2;
	}

};

class CAnmESAssignmentExpression : public CAnmESExpression
{
	CAnmESExpression			*m_lhs;
	CAnmESExpression			*m_rhs;
	eESOperator					 m_operator;

public :

	CAnmESAssignmentExpression(CAnmESExpression *pLHS, CAnmESExpression *pRHS, eESOperator op) :
	   CAnmESExpression(eAssignmentExpression, eUndefined)
	   {
		   m_lhs = pLHS;
		   m_rhs = pRHS;
		   m_operator = op;
	   }

	// Accessors
	CAnmESExpression *GetLHS()
	{
		return m_lhs;
	}

	CAnmESExpression *GetRHS()
	{
		return m_rhs;
	}

	eESOperator GetOp()
	{
		return m_operator;
	}

};

typedef std::vector<CAnmESExpression *> ESExprArgs;

class CAnmESFunctionExpression : public CAnmESExpression
{
	CAnmESExpression				*m_callExpression;
	class CAnmESTypeSymbol			*m_typeForNew;

	ESExprArgs						 m_argExprs;

public :

	CAnmESFunctionExpression(CAnmESExpression *pCallExpression,
		class CAnmESTypeSymbol *pTypeForNew) :
	   CAnmESExpression(eFunctionExpression, eUndefined)
	{
		   m_callExpression = pCallExpression;
		   m_typeForNew = pTypeForNew;
		   if (m_typeForNew != NULL)
			   m_valueType = eObject;
	}

	void AddArg(CAnmESExpression *pArgExpr)
	{
		m_argExprs.push_back(pArgExpr);
	}

	// Accessors
	CAnmESExpression *GetCallExpression()
	{
		return m_callExpression;
	}

	ESExprArgs *GetArgs()
	{
		return &m_argExprs;
	}

	class CAnmESTypeSymbol *GetTypeForNew()
	{
		return m_typeForNew;
	}
};


#endif // _anmesexpression_h
