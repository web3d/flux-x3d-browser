%{
#include "stdafx.h"
#include "anmvrml97reader.h"
#include "anmsymbol.h"
%}

%{
#define yyerror(message) Yyerror(message)
#define yylex() Yylex()
#define yyparse() CAnmVRML97Reader::Yyparse()
#define yylval m_Yylval
#define yyval m_Yyval
#define pcyytoken m_Pcyytoken
#define pcyyerrfl m_Pcyyerrfl
#define pcyyerrct m_Pcyyerrct
#define yyv m_Yyv
%}

%token IS
%token DEF
%token USE
%token ROUTE
%token TO
%token TRUE_TOKEN
%token FALSE_TOKEN
%token NULL_TOKEN
%token PROTO
%token EXTERNPROTO
%token PROFILE
%token COMPONENT
%token IMPORT_TOKEN
%token EXPORT_TOKEN
%token AS
%token META

%token Script
%token Field
%token ExposedField
%token EventIn
%token EventOut
%token ShaderProgram

%token '{'
%token '}'
%token '['
%token ']'
%token '.'
%token ':'

%token <intVal>		INTEGER_TOKEN
%token <doubleVal>	DOUBLE_TOKEN
%token <strVal>		STRING_TOKEN
%token <strVal>	IDENTIFIER


%token <pAttrSymbol> BOOLEAN_ATTRIBUTE
%token <pAttrSymbol> COLOR_ATTRIBUTE
%token <pAttrSymbol> COLORARRAY_ATTRIBUTE
%token <pAttrSymbol> DOUBLE_ATTRIBUTE
%token <pAttrSymbol> DOUBLEARRAY_ATTRIBUTE
%token <pAttrSymbol> FLOAT_ATTRIBUTE
%token <pAttrSymbol> FLOATARRAY_ATTRIBUTE
%token <pAttrSymbol> IMAGE_ATTRIBUTE
%token <pAttrSymbol> INTEGER_ATTRIBUTE
%token <pAttrSymbol> INTEGERARRAY_ATTRIBUTE
%token <pAttrSymbol> NODE_ATTRIBUTE
%token <pAttrSymbol> NODEARRAY_ATTRIBUTE
%token <pAttrSymbol> ROTATION_ATTRIBUTE
%token <pAttrSymbol> ROTATIONARRAY_ATTRIBUTE
%token <pAttrSymbol> STRING_ATTRIBUTE
%token <pAttrSymbol> STRINGARRAY_ATTRIBUTE
%token <pAttrSymbol> TIME_ATTRIBUTE
%token <pAttrSymbol> TIMEARRAY_ATTRIBUTE
%token <pAttrSymbol> POINT2_ATTRIBUTE
%token <pAttrSymbol> POINT2ARRAY_ATTRIBUTE
%token <pAttrSymbol> POINT3_ATTRIBUTE
%token <pAttrSymbol> POINT3ARRAY_ATTRIBUTE


%token <attrType> Boolean_Type
%token <attrType> BooleanArray_Type
%token <attrType> Color_Type
%token <attrType> ColorArray_Type
%token <attrType> Double_Type
%token <attrType> DoubleArray_Type
%token <attrType> Float_Type
%token <attrType> FloatArray_Type
%token <attrType> Image_Type
%token <attrType> ImageArray_Type
%token <attrType> Integer_Type
%token <attrType> IntegerArray_Type
%token <attrType> Node_Type
%token <attrType> NodeArray_Type
%token <attrType> Rotation_Type
%token <attrType> RotationArray_Type
%token <attrType> String_Type
%token <attrType> StringArray_Type
%token <attrType> Time_Type
%token <attrType> TimeArray_Type
%token <attrType> Point2_Type
%token <attrType> Point2Array_Type
%token <attrType> Point3_Type
%token <attrType> Point3Array_Type


%type <strVal>		identifier_or_field
%type <pAttrSymbol>	any_field_type
%type <doubleVal>	numeric_value
%type <attrType>	field_type
%type <memberType>	field_access

%start world

%%

/* For now, a world is a scene */
world
	:
 	scene
	;

/* A scene is some number of prototypes, nodes and routes, in that order */	
scene
	:
	profile
	components
	metas
	scene_contents
	;

profile
	: PROFILE IDENTIFIER
	|
	;

components
	: 
	| components component
	;

component
	: COMPONENT IDENTIFIER ':' INTEGER_TOKEN
	| COMPONENT IDENTIFIER 
	;

metas
	: 
	| metas meta
	;

meta
	: META STRING_TOKEN STRING_TOKEN
		{ AddMetaData($<strVal>2, $<strVal>3); }
	;

scene_contents
	: 
	  declarations /* nodes routes node_instances scripts routes */
	;

proto_body
	: prototypes node declarations
	;

prototypes
	:
	| prototypes prototype
	;

/* Declarations */
declarations
	:
	| declarations declaration
	;

declaration
	: prototype
	| node
	| route
	| import
	| export
	;
	
/* Prototypes.  Contain interface definition and nested scene */

prototype
	:
	PROTO 
	{
	}
	IDENTIFIER
	{
		// id is $3
		BeginProtoDeclaration($<strVal>3);
	}
	  '[' proto_declarations ']'
	{
	}
	  '{' proto_body '}'
	{
		EndProtoDeclaration();
	}
	|
	EXTERNPROTO 
	{
	}
	IDENTIFIER
	{
		// id is $3
		BeginProtoDeclaration($<strVal>3, TRUE);
	}
	'[' extern_proto_declarations ']' string_values
	{
		EndProtoDeclaration(StringArrayFromStrings());
	}
	;

proto_declarations
	:
	| proto_declarations proto_declaration
	;

extern_proto_declarations
	:
	| extern_proto_declarations extern_proto_declaration
	;

proto_declaration
	: event_declaration
	| field_declaration
	;

extern_proto_declaration
	: event_declaration
	| extern_field_declaration
	;

			 	  
/* Nodes */
nodes
	:
	| nodes node
	;

node
	: node_instance
	| script
	| shader
	| route
	;

node_instance
	: use_clause
	| def_clause node_definition
	| node_definition
	;

use_clause
	: USE 
	{
	}
	IDENTIFIER
	{
		UseNode($<strVal>3);
	}
	;

def_clause
	: DEF 
	{
	}
	IDENTIFIER 
	{
		DefNode($<strVal>3);
	}
	;

node_definition
	: IDENTIFIER
	{
		BeginMakeNode($<strVal>1);
	}
	  '{' node_definition_contents '}'
	{
		EndMakeNode();
	}
	;


node_definition_contents
	:
	| node_definition_contents node_definition_onemember
	;

node_definition_onemember
	: field
	| route
	| prototype
	;

/* Scripts */
script
	: def_clause script_definition
	| script_definition
	;

script_definition
	: Script
	{
		BeginMakeScript();
	}
	  '{' script_declarations  '}'
	{
		EndMakeScript();
	}
	;

script_declarations
	:
	| script_declarations script_declaration
	;

script_declaration
	: script_field_declaration
	| event_declaration
	| field
	;


script_field_declaration
	: field_access
	{
		SetFieldAccess($<memberType>1);
	}
	script_field_defaults
	;


script_field_defaults
	: boolean_defaults
	| color_defaults
	| double_defaults
	| float_defaults
	| image_defaults
	| integer_defaults
	| script_node_defaults
	| rotation_defaults
	| string_defaults
	| time_defaults
	| point2_defaults
	| point3_defaults
	;

/* Shaders */

shader
	: def_clause shader_definition
	| shader_definition
	;

shader_definition
	: ShaderProgram
	{
		BeginMakeShader();
	}
	  '{' shader_declarations  '}'
	{
		EndMakeShader();
	}
	;

shader_declarations
	:
	| shader_declarations shader_declaration
	;

shader_declaration
	: shader_field_declaration
	| event_declaration
	| field
	;

shader_field_declaration
	: Field
	{
		SetFieldAccess(eAnmValueMember);
	}
	shader_field_defaults
	;

shader_field_defaults
	: boolean_defaults
	| color_defaults
	| float_defaults
	| image_defaults
	| integer_defaults
	| script_node_defaults
	| rotation_defaults
	| string_defaults
	| time_defaults
	| point2_defaults
	| point3_defaults
	;


/* Fields */
/* Field declarations */

field_declaration
	: field_access
	{
		SetFieldAccess($<memberType>1);
	}
	  field_defaults
	{
	}
	;


field_access
: Field { $$ = eAnmInitMember; }
	| ExposedField { $$ = eAnmValueMember; }
	;


field_type
	: Boolean_Type {$$ = $1;}
	| Color_Type {$$ = $1;}
	| ColorArray_Type {$$ = $1;}
	| Float_Type {$$ = $1;}
	| FloatArray_Type {$$ = $1;}
	| Image_Type {$$ = $1;}
	| Integer_Type {$$ = $1;}
	| IntegerArray_Type {$$ = $1;}
	| Node_Type {$$ = $1;}
	| NodeArray_Type {$$ = $1;}
	| Rotation_Type {$$ = $1;}
	| RotationArray_Type {$$ = $1;}
	| String_Type {$$ = $1;}
	| StringArray_Type {$$ = $1;}
	| Time_Type {$$ = $1;}
	| TimeArray_Type {$$ = $1;}
	| Point2_Type {$$ = $1;}
	| Point2Array_Type {$$ = $1;}
	| Point3_Type {$$ = $1;}
	| Point3Array_Type {$$ = $1;}
	;

field_defaults
	: boolean_defaults
	| color_defaults
	| double_defaults
	| float_defaults
	| image_defaults
	| integer_defaults
	| node_defaults
	| rotation_defaults
	| string_defaults
	| time_defaults
	| point2_defaults
	| point3_defaults
	;

boolean_defaults
	: Boolean_Type IDENTIFIER boolean_value
	{
		AddField(eValueBoolean, $<strVal>2, (void*) GetCurrentBool());
	}
	| Boolean_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueBoolean, $<strVal>2, (void*) GetCurrentBool(), $4);
	}
	;

color_defaults
	: Color_Type IDENTIFIER color_value
	{
		AddField(eValueColor, $<strVal>2, (void*) ColorFromFloats());
	}
	| ColorArray_Type IDENTIFIER color_values
	{
		AddField(eValueColorArray, $<strVal>2, (void*) ColorArrayFromFloats());
	}
	| Color_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueColor, $<strVal>2, (void*) ColorFromFloats(), $4);
	}
	| ColorArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueColorArray, $<strVal>2, (void*) ColorArrayFromFloats(), $4);
	}
	;

double_defaults
	: Double_Type IDENTIFIER float_value
	{
		AddField(eValueDouble, $<strVal>2, (void*) DoubleFromFloats());
	}
	| DoubleArray_Type IDENTIFIER float_values
	{
		AddField(eValueDoubleArray, $<strVal>2, (void*) DoubleArrayFromFloats());
	}
	| Double_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueDouble, $<strVal>2, (void*) DoubleFromFloats(), $4);
	}
	| DoubleArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueDoubleArray, $<strVal>2, (void*) DoubleArrayFromFloats(), $4);
	}
	;

float_defaults
	: Float_Type IDENTIFIER float_value
	{
		AddField(eValueFloat, $<strVal>2, (void*) FloatFromFloats());
	}
	| FloatArray_Type IDENTIFIER float_values
	{
		AddField(eValueFloatArray, $<strVal>2, (void*) FloatArrayFromFloats());
	}
	| Float_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueFloat, $<strVal>2, (void*) FloatFromFloats(), $4);
	}
	| FloatArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueFloatArray, $<strVal>2, (void*) FloatArrayFromFloats(), $4);
	}
	;

image_defaults
	: Image_Type IDENTIFIER image_value
	{
		AddField(eValueImage, $<strVal>2, (void*) ImageFromInts());
	}
	| Image_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueImage, $<strVal>2, (void*) ImageFromInts(), $4);
	}
	;

integer_defaults
	: Integer_Type IDENTIFIER integer_value
	{
		AddField(eValueInteger, $<strVal>2, (void*) IntegerFromInts());
	}
	| IntegerArray_Type IDENTIFIER integer_values
	{
		AddField(eValueIntegerArray, $<strVal>2, (void*) IntegerArrayFromInts());
	}
	| Integer_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueInteger, $<strVal>2, (void*) IntegerFromInts(), $4);
	}
	| IntegerArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueIntegerArray, $<strVal>2, (void*) IntegerArrayFromInts(), $4);
	}
	;

node_defaults
	: Node_Type IDENTIFIER node_value
	{
		AddField(eValueNode, $<strVal>2, NULL);
	}
	| NodeArray_Type IDENTIFIER node_values
	{
		AddField(eValueNodeArray, $<strVal>2, NULL);
	}
	| Node_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueNode, $<strVal>2, NULL, $4);
	}
	| NodeArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueNodeArray, $<strVal>2, NULL, $4);
	}
	;

rotation_defaults
	: Rotation_Type IDENTIFIER rotation_value
	{
		AddField(eValueRotation, $<strVal>2, (void*) RotationFromFloats());
	}
	| RotationArray_Type IDENTIFIER rotation_values
	{
		AddField(eValueRotationArray, $<strVal>2, (void*) RotationArrayFromFloats());
	}
	| Rotation_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueRotation, $<strVal>2, (void*) RotationFromFloats(), $4);
	}
	| RotationArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueRotationArray, $<strVal>2, (void*) RotationArrayFromFloats(), $4);
	}
	;

string_defaults
	: String_Type IDENTIFIER string_value
	{
		AddField(eValueString, $<strVal>2, (void*) StringFromStrings());
	}
	| StringArray_Type IDENTIFIER string_values
	{
		AddField(eValueStringArray, $<strVal>2, (void*) StringArrayFromStrings());
	}
	| String_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueString, $<strVal>2, (void*) StringFromStrings(), $4);
	}
	| StringArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueStringArray, $<strVal>2, (void*) StringArrayFromStrings(), $4);
	}
	;

time_defaults
	: Time_Type IDENTIFIER float_value
	{
		AddField(eValueTime, $<strVal>2, (void*) TimeFromFloats());
	}
	| TimeArray_Type IDENTIFIER float_values
	{
		AddField(eValueTimeArray, $<strVal>2, (void*) TimeArrayFromFloats());
	}
	| Time_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueTime, $<strVal>2, (void*) TimeFromFloats(), $4);
	}
	| TimeArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueTimeArray, $<strVal>2, (void*) TimeArrayFromFloats(), $4);
	}
	;

point2_defaults
	: Point2_Type IDENTIFIER point2_value
	{
		AddField(eValuePoint2, $<strVal>2, (void*) Point2FromFloats());
	}
	| Point2Array_Type IDENTIFIER point2_values
	{
		AddField(eValuePoint2Array, $<strVal>2, (void*) Point2ArrayFromFloats());
	}
	| Point2_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValuePoint2, $<strVal>2, (void*) Point2FromFloats(), $4);
	}
	| Point2Array_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValuePoint2Array, $<strVal>2, (void*) Point2ArrayFromFloats(), $4);
	}
	;

point3_defaults
	: Point3_Type IDENTIFIER point3_value
	{
		AddField(eValuePoint3, $<strVal>2, (void*) Point3FromFloats());
	}
	| Point3Array_Type IDENTIFIER point3_values
	{
		AddField(eValuePoint3Array, $<strVal>2, (void*) Point3ArrayFromFloats());
	}
	| Point3_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValuePoint3, $<strVal>2, (void*) Point3FromFloats(), $4);
	}
	| Point3Array_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValuePoint3Array, $<strVal>2, (void*) Point3ArrayFromFloats(), $4);
	}
	;

script_node_defaults
	: Node_Type IDENTIFIER node_value
	{
		AddField(eValueNode, $<strVal>2, NULL);
	}
	| NodeArray_Type IDENTIFIER node_values
	{
		AddField(eValueNodeArray, $<strVal>2, NULL);
	}
	| Node_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueNode, $<strVal>2, NULL, $4);
	}
	| NodeArray_Type IDENTIFIER IS identifier_or_field
	{
		AddField(eValueNodeArray, $<strVal>2, NULL, $4);
	}
	;


identifier_or_field :
	IDENTIFIER
	{
		$$ = $1;
	}
	|
	any_field_type
	{
		$$ = $1->GetName()->Clone();
	}
	;

/* Field IDENTIFIER */
field
	: field_value
	| IDENTIFIER IS IDENTIFIER
	{
		AddIS($<strVal>1, $<strVal>3);
	}
	| any_field_type IS IDENTIFIER
	{
		AddIS($<pAttrSymbol>1, $<strVal>3);
	}
	| any_field_type IS any_field_type
	{
		AddIS($<pAttrSymbol>1, $<pAttrSymbol>3);
	}
	;

/* Field values */
field_value
	: BOOLEAN_ATTRIBUTE boolean_value
	{
		SetValue($<pAttrSymbol>1, (void*) GetCurrentBool());
	}
	| COLOR_ATTRIBUTE color_value
	{
		SetValue($<pAttrSymbol>1, (void*) ColorFromFloats());
	}
	| COLORARRAY_ATTRIBUTE color_values
	{
		SetValue($<pAttrSymbol>1, (void*) ColorArrayFromFloats());
	}
	| DOUBLE_ATTRIBUTE float_value
	{
		SetValue($<pAttrSymbol>1, (void*) DoubleFromFloats());
	}
	| DOUBLEARRAY_ATTRIBUTE float_values
	{
		SetValue($<pAttrSymbol>1, (void*) DoubleArrayFromFloats());
	}
	| FLOAT_ATTRIBUTE float_value
	{
		SetValue($<pAttrSymbol>1, (void*) FloatFromFloats());
	}
	| FLOATARRAY_ATTRIBUTE float_values
	{
		SetValue($<pAttrSymbol>1, (void*) FloatArrayFromFloats());
	}
	| IMAGE_ATTRIBUTE image_value
	{
		SetValue($<pAttrSymbol>1, (void*) ImageFromInts());
	}
	| INTEGER_ATTRIBUTE integer_value
	{
		SetValue($<pAttrSymbol>1, (void*) IntegerFromInts());
	}
	| INTEGERARRAY_ATTRIBUTE integer_values
	{
		SetValue($<pAttrSymbol>1, (void*) IntegerArrayFromInts());
	}
	| NODE_ATTRIBUTE node_value
	{
		SetValue($<pAttrSymbol>1, NULL);
	}
	| NODEARRAY_ATTRIBUTE node_values
	{
		SetValue($<pAttrSymbol>1, NULL);
	}
	| ROTATION_ATTRIBUTE rotation_value
	{
		SetValue($<pAttrSymbol>1, (void*) RotationFromFloats());
	}
	| ROTATIONARRAY_ATTRIBUTE rotation_values
	{
		SetValue($<pAttrSymbol>1, (void*) RotationArrayFromFloats());
	}
	| STRING_ATTRIBUTE string_value
	{
		SetValue($<pAttrSymbol>1, (void*) StringFromStrings());
	}
	| STRINGARRAY_ATTRIBUTE string_values
	{
		SetValue($<pAttrSymbol>1, (void*) StringArrayFromStrings());
	}
	| TIME_ATTRIBUTE float_value
	{
		SetValue($<pAttrSymbol>1, (void*) TimeFromFloats());
	}
	| TIMEARRAY_ATTRIBUTE float_values
	{
		SetValue($<pAttrSymbol>1, (void*) TimeArrayFromFloats());
	}
	| POINT2_ATTRIBUTE point2_value
	{
		SetValue($<pAttrSymbol>1, (void*) Point2FromFloats());
	}
	| POINT2ARRAY_ATTRIBUTE point2_values
	{
		SetValue($<pAttrSymbol>1, (void*) Point2ArrayFromFloats());
	}
	| POINT3_ATTRIBUTE point3_value
	{
		SetValue($<pAttrSymbol>1, (void*) Point3FromFloats());
	}
	| POINT3ARRAY_ATTRIBUTE point3_values
	{
		SetValue($<pAttrSymbol>1, (void*) Point3ArrayFromFloats());
	}
	| IDENTIFIER any_value
	{
		IgnoreValue($<strVal>1, $<attrType>2);
	}
	;

any_field_type
	: BOOLEAN_ATTRIBUTE {$$ = $1; }
	| COLOR_ATTRIBUTE {$$ = $1; }
	| COLORARRAY_ATTRIBUTE {$$ = $1; }
	| DOUBLE_ATTRIBUTE {$$ = $1; }
	| DOUBLEARRAY_ATTRIBUTE {$$ = $1; }
	| FLOAT_ATTRIBUTE {$$ = $1; }
	| FLOATARRAY_ATTRIBUTE {$$ = $1; }
	| IMAGE_ATTRIBUTE {$$ = $1; }
	| INTEGER_ATTRIBUTE {$$ = $1; }
	| INTEGERARRAY_ATTRIBUTE {$$ = $1; }
	| NODE_ATTRIBUTE {$$ = $1; }
	| NODEARRAY_ATTRIBUTE {$$ = $1; }
	| ROTATION_ATTRIBUTE {$$ = $1; }
	| ROTATIONARRAY_ATTRIBUTE {$$ = $1; }
	| STRING_ATTRIBUTE {$$ = $1; }
	| STRINGARRAY_ATTRIBUTE {$$ = $1; }
	| TIME_ATTRIBUTE {$$ = $1; }
	| TIMEARRAY_ATTRIBUTE {$$ = $1; }
	| POINT2_ATTRIBUTE {$$ = $1; }
	| POINT2ARRAY_ATTRIBUTE {$$ = $1; }
	| POINT3_ATTRIBUTE {$$ = $1; }
	| POINT3ARRAY_ATTRIBUTE {$$ = $1; }
	;

any_value
	: boolean_value {$<attrType>$ = eValueBoolean; }
	| float_values {$<attrType>$ = eValueFloatArray; }
	| image_value {$<attrType>$ = eValueImage; }
	| integer_values {$<attrType>$ = eValueIntegerArray; }
	| node_values {$<attrType>$ = eValueNodeArray; }
	| string_values {$<attrType>$ = eValueStringArray; }
	;

boolean_value
	: TRUE_TOKEN
	{
		SetCurrentBool(TRUE);
		$<intVal>$ = TRUE;
	}
	| FALSE_TOKEN
	{
		SetCurrentBool(FALSE);
		$<intVal>$ = FALSE;
	}
	;

color_value
	:
	numeric_value numeric_value numeric_value
	{
		AccumDouble($1);
		AccumDouble($2);
		AccumDouble($3);
	}
	;

color_values
	: color_value
	| empty_array
	| '[' many_color_values ']'
	;

many_color_values
	: numeric_value
	{
		AccumDouble($1);
	}
	| many_color_values numeric_value
	{
		AccumDouble($2);
	}
	;

float_value
	:
	numeric_value
	{
		AccumDouble($1);
	}
	;

float_values
	: float_value
	| empty_array
	| '[' many_float_values ']'
	;

many_float_values
	: numeric_value
	{
		AccumDouble($<doubleVal>1);
	}
	| many_float_values numeric_value
	{
		AccumDouble($<doubleVal>2);
	}
	;

image_value
	: integer_value integer_value integer_value integer_star
	;

integer_star
	:
	| integer_star integer_value
	;

integer_value
	:
	INTEGER_TOKEN
	{
		AccumInteger($1);
	}
	;

integer_values
	: integer_value
	| empty_array
	| '[' many_integer_values ']'
	;

many_integer_values
	: integer_value
	| many_integer_values integer_value
	;
			   
node_value
	: node
	{
		$$ = $1;
	}
	| NULL_TOKEN
	{
		NullNode();
	}
	;

node_values
	: node_value
	| empty_array
	| '[' many_node_values ']'
	;
			  
many_node_values
	: node_value
	| many_node_values  node_value
	;

point2_value
	:
	numeric_value numeric_value
	{
		AccumDouble($1);
		AccumDouble($2);
	}
	;

point2_values
	: point2_value
	| empty_array
	| '[' many_point2_values ']'
	;

many_point2_values
	: numeric_value
	{
		AccumDouble($1);
	}
	| many_point2_values numeric_value
	{
		AccumDouble($2);
	}
	;

point3_value
	:
	numeric_value numeric_value numeric_value
	{
		AccumDouble($1);
		AccumDouble($2);
		AccumDouble($3);
	}
	;

point3_values
	: point3_value
	| empty_array
	| '[' many_point3_values ']'
	;

many_point3_values
	: numeric_value
	{
		AccumDouble($1);
	}
	| many_point3_values numeric_value
	{
		AccumDouble($2);
	}
	;
	   
rotation_value
	:
	numeric_value numeric_value numeric_value numeric_value
	{
		AccumDouble($1);
		AccumDouble($2);
		AccumDouble($3);
		AccumDouble($4);
	}
	;

rotation_values
	: rotation_value
	| empty_array
	| '[' many_rotation_values ']'
	;

many_rotation_values
	: numeric_value
	{
		AccumDouble($1);
	}
	| many_rotation_values numeric_value
	{
		AccumDouble($2);
	}
	;
			  
string_value
	:
	STRING_TOKEN
	{
		AccumString($<strVal>1);
	}
	;

string_values
	: string_value
	| empty_array
	| '[' many_string_values ']'
	;

many_string_values
	: string_value
	| many_string_values string_value
	;

extern_field_declaration
	: field_access
	{
		SetFieldAccess($<memberType>1);
	}
	field_type IDENTIFIER
	{
		AddField($<attrType>3, $<strVal>4);
	}
	| field_access
	{
		SetFieldAccess($<memberType>1);
	}
	field_type IDENTIFIER IS IDENTIFIER
	{
		AddISField($<attrType>3, $<strVal>4, $<strVal>6);
	}
	;

/* Events */	
event_declaration
	: EventIn field_type IDENTIFIER
	{
		AddEventIn($<attrType>2, $<strVal>3);
	}
	| EventIn field_type IDENTIFIER IS IDENTIFIER
	{
		AddISEventIn($<attrType>2, $<strVal>3, $<strVal>5);
	}
	| EventOut field_type IDENTIFIER
	{
		AddEventOut($<attrType>2, $<strVal>3);
	}
	| EventOut field_type IDENTIFIER IS IDENTIFIER
	{
		AddISEventOut($<attrType>2, $<strVal>3, $<strVal>5);
	}
	;

/* Routes */
routes
	: 
	| routes route
	;

route
	: ROUTE 
	{
	}
	IDENTIFIER 
	{
	}
	'.' IDENTIFIER TO 
	{
	}
	IDENTIFIER 
	{
	}
	'.' IDENTIFIER
	{
		MakeRoute($<strVal>3, $<strVal>6, $<strVal>9, $<strVal>12);
	}
	;

/* Import/Export */

import
	: IMPORT_TOKEN IDENTIFIER '.' IDENTIFIER
	| IMPORT_TOKEN IDENTIFIER '.' IDENTIFIER AS IDENTIFIER
	;

export
	: EXPORT_TOKEN IDENTIFIER
	| EXPORT_TOKEN IDENTIFIER AS IDENTIFIER
	;

/* Helper nonterminals */

empty_array
	: '[' ']'
	;

numeric_value
	: INTEGER_TOKEN { $$ = $1; }
	| DOUBLE_TOKEN { $$ = $1; }
	;

%%
