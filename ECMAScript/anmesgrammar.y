%{ 
#include "stdafx.h"

#include "anmescompiler.h"
#include "anmessymbol.h"
#include "anmesexpression.h"
#include "anmesstatement.h"

#define yyerror(message) Yyerror(message)
#define yylex() Yylex()
#define yyparse() CAnmECMAScriptCompiler::Yyparse()
#define yylval m_Yylval
#define yyval m_Yyval
#define pcyytoken m_Pcyytoken
#define pcyyerrfl m_Pcyyerrfl
#define pcyyerrct m_Pcyyerrct
#define yyv m_Yyv
%} 


%token ASSIGN_SYMBOL 
%token BITWISE_AND 
%token BITWISE_AND_EQUALS 
%token BITWISE_EXCLUSIVE_OR 
%token BITWISE_EXCLUSIVE_OR_EQUALS 
%token BITWISE_OR 
%token BITWISE_OR_EQUALS 
%token BITWISE_SHIFT_LEFT 
%token BITWISE_SHIFT_LEFT_EQUALS 
%token BITWISE_SHIFT_RIGHT 
%token BITWISE_SHIFT_RIGHT_EQUALS 
%token BITWISE_SHIFT_RIGHT_ZERO_FILL 
%token BITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS 
%token BREAK 
%token CASE 
%token CLOSE_PARENTHESIS 
%token CLOSE_SQ_BRACKETS 
%token COLON 
%token COMMA  
%token CONTINUE 
%token DECREMENT 
%token DEFAULT 
%token DELETE_TOKEN 
%token DIV 
%token DIV_EQUALS 
%token DOT 
%token ELSE 
%token END_BLOCK 
%token EQUALS 
%token FALSE_TOKEN
%token FOR 
%token FUNCTION  
%token GREATER_THAN 
%token GT_EQUAL 
%token IF 
%token INCREMENT 
%token INFINITY
%token IN_TOKEN 
%token LESS_THAN 
%token LINE_TERMINATOR
%token LOGICAL_AND 
%token LOGICAL_OR 
%token LOGICAL_NOT 
%token LS_EQUAL 
%token MINUS 
%token MINUS_EQUALS 
%token MOD 
%token MOD_EQUALS 
%token MULTIPLY 
%token MULTIPLY_EQUALS 
%token NEW 
%token NOT_EQUAL 
%token NULL_TOKEN
%token ONES_COMPLIMENT 
%token OPEN_PARENTHESIS 
%token OPEN_SQ_BRACKETS 
%token PLUS 
%token PLUS_EQUALS 
%token QUERY 
%token RETURN 
%token SWITCH 
%token SEMICOLON 
%token START_BLOCK 
%token THIS_TOKEN 
%token TRUE_TOKEN 
%token TYPEOF 
%token UNDEFINED_TOKEN
%token VAR 
%token VOID_SYMBOL
%token WHILE 
%token WITH  

%token <idVal> IDENTIFIER
%token <doubleVal> NUMERIC
%token <strVal> STRING

%type <pExpr> PrimaryExpression
%type <pExpr> OptionalExpression
%type <pExpr> Expression
%type <pExpr> MemberExpression
%type <pExpr> NewExpression
%type <pExpr> CallExpression
%type <pExpr> LeftHandSideExpression
%type <pExpr> PostfixExpression
%type <pExpr> UnaryExpression
%type <pExpr> MultiplicativeExpression
%type <pExpr> AdditiveExpression
%type <pExpr> ShiftExpression
%type <pExpr> RelationalExpression
%type <pExpr> EqualityExpression
%type <pExpr> BitwiseANDExpression
%type <pExpr> BitwiseXORExpression
%type <pExpr> BitwiseORExpression
%type <pExpr> LogicalANDExpression
%type <pExpr> LogicalORExpression
%type <pExpr> ConditionalExpression
%type <pExpr> AssignmentExpression
%type <pExpr> Initializer
%type <pExpr> OptionalInitializer

%type <op> AssignmentOperator

%type <pStatement> Statement
%type <pStatement> Block
%type <pStatement> VariableStatement
%type <pStatement> EmptyStatement
%type <pStatement> ExpressionStatement
%type <pStatement> IfStatement
%type <pStatement> OptionalElse
%type <pStatement> IterationExpression
%type <pStatement> ContinueStatement
%type <pStatement> BreakStatement
%type <pStatement> CaseStatement
%type <pStatement> ReturnStatement
%type <pStatement> SwitchStatement
%type <pStatement> SwitchBlock
%type <pStatement> WithStatement
%type <pStatement> VariableDeclaration
%type <pStatement> VariableDeclarationList

%left PLUS MINUS

%left MULTIPLY DIV MOD 


%nonassoc FALSE_TOKEN

%nonassoc HIGHER_THAN_FALSE

%nonassoc ELSE 


%nonassoc LOWER_THAN_CLOSE_PARENTHESIS

%nonassoc CLOSE_PARENTHESIS 


%% 


Program    : SourceElements

      ; 


SourceElements   : SourceElement

     | SourceElements SourceElement

     ; 


SourceElement

	: Statement
	{
		GenStatement($1);
	}

     | FunctionDeclaration

     ; 


Statement   : Block { $$ = $1; }

     | VariableStatement { $$ = $1; }

     | EmptyStatement { $$ = $1; }

     | ExpressionStatement { $$ = $1; }

     | IfStatement { $$ = $1; }

     | IterationExpression { $$ = $1; }

     | ContinueStatement { $$ = $1; }

     | BreakStatement { $$ = $1; }

     | ReturnStatement { $$ = $1; }

     | SwitchStatement { $$ = $1; }

     | WithStatement { $$ = $1; }

     ; 


FunctionDeclaration
	: FUNCTION IDENTIFIER 
	{
		BeginDeclareFunction($2);
	}
	OPEN_PARENTHESIS OptionalParameterList
	{
	}
	CLOSE_PARENTHESIS Block
	{
		EndDeclareFunction($8);
	}
    ; 

OptionalParameterList
	:

	| FormalParameterList

	;

FormalParameterList

	: IDENTIFIER
	{
		DeclareParam($1);
	}

    | FormalParameterList COMMA IDENTIFIER
	{
		DeclareParam($3);
	}

    ;


StatementList

	: Statement

	{
		AddStatement($1);
	}

    | StatementList Statement
	{
		AddStatement($2);
	}

    ; 


Block

	: START_BLOCK
	{
		BeginBlock();
	}
	
	StatementList
	
	END_BLOCK
	{
		$$ = EndBlock();
	}
	;

    | START_BLOCK END_BLOCK { $$ = EmptyStatement(); }

     ; 


VariableStatement  : VAR VariableDeclarationList SEMICOLON { $$ = $2; }

     ; 


EmptyStatement   : SEMICOLON { $$ = EmptyStatement(); }

     ; 


ExpressionStatement  : Expression SEMICOLON
	{
		$$ = new CAnmESTopLevelStatement($1);
	}

     ; 


IfStatement
	:
	
	IF OPEN_PARENTHESIS Expression CLOSE_PARENTHESIS	Statement /* %prec HIGHER_THAN_FALSE */
	OptionalElse
	{
		$$ = new CAnmESIfStatement($3, $5, $6);
	}

/*    |

	 IF OPEN_PARENTHESIS Expression CLOSE_PARENTHESIS
	{
		GenRValue($3);
		GenIf();
	}
	Statement
	{
		GenEndIf();
	}
	ELSE Statement
	{
		GenEndElse();
	}


     | IF OPEN_PARENTHESIS FALSE_TOKEN CLOSE_PARENTHESIS Statement

     {

               Yyerror("Unreachable code portion");

     }

     | IF OPEN_PARENTHESIS LeftHandSideExpression AssignmentOperator AssignmentExpression CLOSE_PARENTHESIS Statement

     {

               Yywarning("Possible error in IF Expression (e.g. a=b instead of a==b)");

     }
*/
     ; 

OptionalElse :
	{ $$ = EmptyStatement(); }
	|
	ELSE Statement
	{ $$ = $2; }
	;

IterationExpression

	: WHILE OPEN_PARENTHESIS Expression CLOSE_PARENTHESIS Statement %prec HIGHER_THAN_FALSE
	{
		$$ = new CAnmESWhileStatement($3, $5);
	}

    | WHILE OPEN_PARENTHESIS FALSE_TOKEN CLOSE_PARENTHESIS Statement

    {
         Yywarning("Unreachable code portion");
    }

    | WHILE OPEN_PARENTHESIS LeftHandSideExpression AssignmentOperator AssignmentExpression CLOSE_PARENTHESIS Statement

    {

        Yywarning("Possible error in WHILE Expression (e.g. a=b instead of a==b)");

		$$ = new CAnmESWhileStatement(AssignmentExpression($3, $4, $5), $7);

    }

    | FOR OPEN_PARENTHESIS OptionalExpression SEMICOLON OptionalExpression SEMICOLON OptionalExpression CLOSE_PARENTHESIS Statement
	{
		$$ = new CAnmESForStatement($3, $5, $7, $9);
	}

    | FOR OPEN_PARENTHESIS VAR VariableDeclarationList SEMICOLON OptionalExpression SEMICOLON OptionalExpression CLOSE_PARENTHESIS Statement
	{
		$$ = NIYStatement("for");
	}

    | FOR OPEN_PARENTHESIS LeftHandSideExpression IN_TOKEN Expression CLOSE_PARENTHESIS Statement
	{
		$$ = NIYStatement("for");
	}

    | FOR OPEN_PARENTHESIS VAR IDENTIFIER OptionalInitializer IN_TOKEN Expression CLOSE_PARENTHESIS Statement
	{
		$$ = NIYStatement("for");
	}

    ; 


ContinueStatement  : CONTINUE SEMICOLON
	{
		$$ = ContinueStatement();
	}

     ; 


BreakStatement   : BREAK SEMICOLON
	{
		$$ = BreakStatement();
	}

	; 


ReturnStatement

	: RETURN Expression SEMICOLON
	{
		$$ = new CAnmESReturnStatement($2);
	}

    | RETURN SEMICOLON
	{
		$$ = new CAnmESReturnStatement();
	}

     ; 

SwitchStatement

	: SWITCH OPEN_PARENTHESIS Expression CLOSE_PARENTHESIS SwitchBlock
	{
		$$ = new CAnmESSwitchStatement($3, $5);
	}
	;

SwitchBlock

	: START_BLOCK
	{
		BeginBlock();
	}
	
	CaseStatementList
	
	END_BLOCK
	{
		$$ = EndBlock();
	}
	;

    | START_BLOCK END_BLOCK { $$ = EmptyStatement(); }

    ; 


CaseStatementList

	: CaseStatement

	{
		AddStatement($1);
	}

    | CaseStatementList CaseStatement
	{
		AddStatement($2);
	}
    ;
	
CaseStatement
	: CASE Expression COLON 
	{
		BeginCaseBlock($2);
	}
	StatementList
	{
		$$ = EndCaseBlock();
	}
	|
	DEFAULT COLON
	{
		BeginCaseBlock(NULL, TRUE);
	}
	StatementList
	{
		$$ = EndCaseBlock();
	}
	;

WithStatement   : WITH OPEN_PARENTHESIS Expression CLOSE_PARENTHESIS Statement
	{
		$$ = NIYStatement("with");
	}

     ; 


VariableDeclarationList  : VariableDeclaration { $$ = new CAnmESVarStatementList($1); }

     | VariableDeclarationList COMMA VariableDeclaration
	 {
		BeginVariableDeclarationList();
		AddVariableDeclaration($3);
		$$ = EndVariableDeclarationList();
	 }
     ; 


VariableDeclaration  : IDENTIFIER

     {
		$$ = new CAnmESVarStatement(IdentifierExpression($1));
     }

     | IDENTIFIER Initializer

     {
		$$ = new CAnmESVarStatement(IdentifierExpression($1), AssignmentExpression(IdentifierExpression($1), eOpASSIGNMENT, $2));
     }

     ; 


Initializer   : ASSIGN_SYMBOL AssignmentExpression

	{ $$ = $2; }
     ; 


OptionalInitializer  : Initializer 	{ $$ = $1; }

     |
	 { $$ = NULL; }
     ; 


PrimaryExpression

	 : THIS_TOKEN
	 {
		$$ = ThisExpression();
	 }

     | IDENTIFIER

     {
			   $$ = IdentifierExpression($1);

     }

     | NUMERIC
	 {
			   $$ = new CAnmESConstExpression($1);
	 } 

     | STRING
	 {
			   $$ = new CAnmESConstExpression($1);
	 } 

     | NULL_TOKEN
	 {
			   $$ = new CAnmESConstExpression((CAnmESObject *) NULL);
	 } 

     | TRUE_TOKEN
	 {
			   $$ = new CAnmESConstExpression(AnmESBool(TRUE));
	 } 

     | FALSE_TOKEN
	 {
			   $$ = new CAnmESConstExpression(AnmESBool(FALSE));
	 } 

     | OPEN_PARENTHESIS Expression CLOSE_PARENTHESIS
	 {
		$$ = $2;
	 }
     ; 


MemberExpression

	 : PrimaryExpression
	 {
		$$ = $1;
	 }

     | MemberExpression OPEN_SQ_BRACKETS Expression CLOSE_SQ_BRACKETS
	 {
		$$ = new CAnmESLValueExpression($1, $3);
	 }

     | MemberExpression DOT IDENTIFIER
	 {
		$$ = new CAnmESLValueExpression($1, $3);
	 }

     | NEW IDENTIFIER
	 {
		BeginNew($2)
	 }
	 Arguments
	 {
		$$ = EndNew();
	 }
     ; 


NewExpression

	 : MemberExpression
	 {
		 $$ = $1;
	 }
/*
     | NEW NewExpression
	 {
		 $$ = $2;
	 }
*/   ; 


CallExpression

	 : MemberExpression
	 {
		BeginFunctionCall($1);
	 }
	 Arguments
	 {
		$$ = EndFunctionCall();
	 }

	 | CallExpression Arguments

	 | CallExpression OPEN_SQ_BRACKETS Expression CLOSE_SQ_BRACKETS

	 | CallExpression DOT IDENTIFIER

	 ; 


Arguments   : OPEN_PARENTHESIS CLOSE_PARENTHESIS

     | OPEN_PARENTHESIS ArgumentList CLOSE_PARENTHESIS

     ; 


ArgumentList

 	 : AssignmentExpression
	 {
		AddArgument($1);
	 }
     | ArgumentList COMMA AssignmentExpression
	 {
		AddArgument($3);
	 }

     ; 


LeftHandSideExpression
	 : NewExpression
	 {
		$$ = $1;
	 }

     | CallExpression
	 {
	 }

     ; 


PostfixExpression

	: LeftHandSideExpression
	{
		$$ = $1;
	}

     | LeftHandSideExpression INCREMENT
	 {
		$$ = UnaryExpression(eOpPOSTINCREMENT, $1);
	 }


     | LeftHandSideExpression DECREMENT
	 {
		$$ = UnaryExpression(eOpPOSTDECREMENT, $1);
	 }

     ; 


UnaryExpression

	: PostfixExpression
	{
		$$ = $1;
	}

     | DELETE_TOKEN UnaryExpression
	 {
	 }

     | VOID_SYMBOL UnaryExpression
	 {
	 }

     | TYPEOF UnaryExpression
	 {
	 }

     | INCREMENT UnaryExpression
	 {
		$$ = UnaryExpression(eOpPREINCREMENT, $2);
	 }

     | DECREMENT UnaryExpression
	 {
		$$ = UnaryExpression(eOpPREDECREMENT, $2);
	 }

     | PLUS UnaryExpression
	 {
		$$ = UnaryExpression(eOpUNARYPLUS, $2);
	 }

     | MINUS UnaryExpression
	 {
		$$ = UnaryExpression(eOpUNARYMINUS, $2);
	 }

     | ONES_COMPLIMENT UnaryExpression
	 {
		$$ = UnaryExpression(eOpONES_COMPLIMENT, $2);
	 }

     | LOGICAL_NOT UnaryExpression
	 {
		$$ = UnaryExpression(eOpLOGICAL_NOT, $2);
	 }

     ; 


MultiplicativeExpression

	: UnaryExpression
	{
		$$ = $1;
	}

     | MultiplicativeExpression MULTIPLY UnaryExpression
	 {
		$$ = BinaryExpression($1, eOpMULTIPLY, $3);
	 }


     | MultiplicativeExpression DIV UnaryExpression
	 {
		$$ = BinaryExpression($1, eOpDIV, $3);
	 }

     | MultiplicativeExpression MOD UnaryExpression
	 {
		$$ = BinaryExpression($1, eOpMOD, $3);
	 }


     ; 


AdditiveExpression

	: MultiplicativeExpression
	{
		$$ = $1;
	}

     | AdditiveExpression PLUS MultiplicativeExpression
	 {
		$$ = BinaryExpression($1, eOpPLUS, $3);
	 }

     | AdditiveExpression MINUS MultiplicativeExpression
	 {
		$$ = BinaryExpression($1, eOpMINUS, $3);
	 }

     ; 


ShiftExpression

	: AdditiveExpression
	{
		$$ = $1;
	}

     | ShiftExpression BITWISE_SHIFT_LEFT AdditiveExpression
	 {
		$$ = BinaryExpression($1, eOpBITWISE_SHIFT_LEFT, $3);
	 }

     | ShiftExpression BITWISE_SHIFT_RIGHT AdditiveExpression
	 {
		$$ = BinaryExpression($1, eOpBITWISE_SHIFT_RIGHT, $3);
	 }


     | ShiftExpression BITWISE_SHIFT_RIGHT_ZERO_FILL AdditiveExpression
	 {
		$$ = BinaryExpression($1, eOpBITWISE_SHIFT_RIGHT_ZERO_FILL, $3);
	 }

     ; 


RelationalExpression

	: ShiftExpression
	{
		$$ = $1;
	}

     | RelationalExpression LESS_THAN ShiftExpression
	 {
		$$ = BinaryExpression($1, eOpLESS_THAN, $3);
	 }

     | RelationalExpression GREATER_THAN ShiftExpression
	 {
		$$ = BinaryExpression($1, eOpGREATER_THAN, $3);
	 }

     | RelationalExpression LS_EQUAL ShiftExpression
	 {
		$$ = BinaryExpression($1, eOpLS_EQUAL, $3);
	 }

     | RelationalExpression GT_EQUAL ShiftExpression
	 {
		$$ = BinaryExpression($1, eOpGT_EQUAL, $3);
	 }

     ; 


EqualityExpression
	
	: RelationalExpression
	{
		$$ = $1;
	}

     | EqualityExpression EQUALS RelationalExpression
	 {
		$$ = BinaryExpression($1, eOpEQUALS, $3);
	 }

     | EqualityExpression NOT_EQUAL RelationalExpression
	 {
		$$ = BinaryExpression($1, eOpNOT_EQUAL, $3);
	 }


     ; 


BitwiseANDExpression

	: EqualityExpression
	{
		$$ = $1;
	}

     | BitwiseANDExpression BITWISE_AND EqualityExpression
	 {
		$$ = BinaryExpression($1, eOpBITWISE_AND, $3);
	 }


     ; 


BitwiseXORExpression

	: BitwiseANDExpression
	{
		$$ = $1;
	}

     | BitwiseXORExpression BITWISE_EXCLUSIVE_OR BitwiseANDExpression
	 {
		$$ = BinaryExpression($1, eOpBITWISE_EXCLUSIVE_OR, $3);
	 }

     ; 


BitwiseORExpression

	: BitwiseXORExpression
	{
		$$ = $1;
	}

     | BitwiseORExpression BITWISE_OR BitwiseXORExpression
	 {
		$$ = BinaryExpression($1, eOpBITWISE_OR, $3);
	 }

     ; 


LogicalANDExpression

	: BitwiseORExpression
	{
		$$ = $1;
	}

     | LogicalANDExpression LOGICAL_AND BitwiseORExpression
	 {
		$$ = BinaryExpression($1, eOpLOGICAL_AND, $3);
	 }

     ; 


LogicalORExpression

	: LogicalANDExpression
	{
		$$ = $1;
	}

     | LogicalORExpression LOGICAL_OR LogicalANDExpression
	 {
		$$ = BinaryExpression($1, eOpLOGICAL_OR, $3);
	 }

     ; 


ConditionalExpression

	: LogicalORExpression
	{
		$$ = $1;
	}

     | LogicalORExpression QUERY AssignmentExpression COLON AssignmentExpression

     ; 


AssignmentExpression

	: ConditionalExpression
	{
		$$ = $1;
	}

     | LeftHandSideExpression AssignmentOperator AssignmentExpression %prec LOWER_THAN_CLOSE_PARENTHESIS
	{
		$$ = AssignmentExpression($1, $2, $3);
	}

     ; 


AssignmentOperator

	 : ASSIGN_SYMBOL
	 {
		$$ = eOpASSIGNMENT;
	 }

     | MULTIPLY_EQUALS
	 {
		$$ = eOpMULTIPLY_EQUALS;
	 }

     | DIV_EQUALS
	 {
		$$ = eOpDIV_EQUALS;
	 }

     | MOD_EQUALS
	 {
		$$ = eOpMOD_EQUALS;
	 }

     | PLUS_EQUALS
	 {
		$$ = eOpPLUS_EQUALS;
	 }

     | MINUS_EQUALS
	 {
		$$ = eOpMINUS_EQUALS;
	 }

     | BITWISE_SHIFT_LEFT_EQUALS
	 {
		$$ = eOpBITWISE_SHIFT_LEFT_EQUALS;
	 }

     | BITWISE_SHIFT_RIGHT_EQUALS
	 {
		$$ = eOpBITWISE_SHIFT_RIGHT_EQUALS;
	 }

     | BITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS
	 {
		$$ = eOpBITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS;
	 }

     | BITWISE_AND_EQUALS
	 {
		$$ = eOpBITWISE_AND_EQUALS;
	 }

     | BITWISE_EXCLUSIVE_OR_EQUALS
	 {
		$$ = eOpBITWISE_EXCLUSIVE_OR_EQUALS;
	 }

     | BITWISE_OR_EQUALS
	 {
		$$ = eOpBITWISE_OR_EQUALS;
	 }

     ; 


Expression  

	 : AssignmentExpression
	 {
		$$ = $1;
	 }

     | Expression COMMA AssignmentExpression
	 {
		$$ = BinaryExpression($1, eOpCOMMA, $3);
	 }

     ; 


OptionalExpression
	 : Expression
	 {
		$$ = $1;
	 }

     |
	 {
		$$ = NULL;
	 }

     ; 


%% 



 
