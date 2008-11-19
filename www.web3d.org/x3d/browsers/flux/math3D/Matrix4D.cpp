/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: matrix4D.cpp
 *    Desc: Definition for a 4x4 homogenous matrix class
 *          
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"
#include "point3D.h"
#include "matrix4D.h"

const matrix4D matrix4D::Identity = matrix4D(
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1);

//returns matrix4D*matrix4D
matrix4D operator*(matrix4D const &a, matrix4D const &b)
{
    matrix4D out;      // temporary matrix4D for storing result
    for (int j = 0; j < 4; j++)         // transform by columns first
        for (int i = 0; i < 4; i++)     // then by rows
            out.m[i][j] = (valType)(
    				a.m[i][0] * b.m[0][j]
    			+	a.m[i][1] * b.m[1][j]
    			+	a.m[i][2] * b.m[2][j]
    			+	a.m[i][3] * b.m[3][j]
    		);
	return out;
};

// Multiplies two matrices together
void MatMult( const matrix4D &a, const matrix4D& b, matrix4D* result )
{
    for (int j = 0; j < 4; j++)         // transform by columns first
        for (int i = 0; i < 4; i++)     // then by rows
            result->m[i][j] = (valType)(
            		a.m[i][0] * b.m[0][j]
            	+	a.m[i][1] * b.m[1][j]
            	+	a.m[i][2] * b.m[2][j]
            	+	a.m[i][3] * b.m[3][j]
            );
}


void matrix4D::Rotate( valType yaw, valType pitch, valType roll )
{
    int i;
    double sinangx,cosangx,sinangy,cosangy,sinangz,cosangz;
    double xold,yold,zold,xnew,ynew,znew;//temporary rotation results to prevent round off error
    sinangx=sin(yaw);
    cosangx=cos(yaw);
    sinangy=sin(pitch);
    cosangy=cos(pitch);
    sinangz=sin(roll);
    cosangz=cos(roll);
    for(i=0; i<3; i++)
    {
////////init points/////////////////////
        xold=m[i][0];
        yold=m[i][1];
        zold=m[i][2];
////////rotate point locally////////////        
        //xrot
        xnew=xold;
        ynew=(yold*cosangx)-(zold*sinangx);
        znew=(yold*sinangx)+(zold*cosangx);
        xold=xnew;
        yold=ynew;
        zold=znew;
        //yrot            
        xnew=((xold*cosangy)+(zold*sinangy));
        ynew=yold;
        znew=((-1*(xold*sinangy))+(zold*cosangy));
        xold=xnew;
        yold=ynew;
        zold=znew;
        //zrot
        xnew=((xold*cosangz)-(yold*sinangz));
        ynew=((yold*cosangz)+(xold*sinangz));
        znew=zold;
        //change object location
        m[i][0]=xnew;
        m[i][1]=ynew;
        m[i][2]=znew;
    }
}

void matrix4D::Translate( const point3D& amt)
{
    _41 += amt.x;
    _42 += amt.y;
    _43 += amt.z;
}

void matrix4D::Place( const point3D& loc)
{
    _41 = loc.x;
    _42 = loc.y;
    _43 = loc.z;
}

void matrix4D::MakeIdent()
{
	_11 = 1.0;	_22 = 1.0;	_33 = 1.0;	_44 = 1.0;
	_12 = 0.0;  _13 = 0.0;  _14 = 0.0;	_21 = 0.0;
	_23 = 0.0;	_24 = 0.0;	_31 = 0.0;	_32 = 0.0;	
	_34 = 0.0;	_41 = 0.0;	_42 = 0.0;	_43 = 0.0;	
}

//==========--------------------------  

void matrix4D::ToObjectLookAt( const point3D& loc, const point3D& lookAt, const point3D& inUp )
{

    point3D viewVec = lookAt - loc;
 
    valType mag = viewVec.Mag();
 
    viewVec /= mag;
 
    valType fDot = inUp * viewVec;
 
    point3D upVec = inUp - fDot * viewVec;
 
	upVec.Normalize();
	
    point3D rightVec = upVec ^ viewVec;


    // Start building the matrix. The first three rows contains the basis
    // vectors used to rotate the view to point at the lookat point
    _11 = rightVec.x;    _21 = upVec.x;    _31 = viewVec.x;
    _12 = rightVec.y;    _22 = upVec.y;    _32 = viewVec.y;
    _13 = rightVec.z;    _23 = upVec.z;    _33 = viewVec.z;
 
    // Do the translation values (rotations are still about the eyepoint)
    _41 = loc.x;
    _42 = loc.y;
    _43 = loc.z;

    _14 = 0;
    _24 = 0;
    _34 = 0;
    _44 = 1;


}

matrix4D matrix4D::ObjectLookAt( 
	const point3D& loc, 
	const point3D& lookAt, 
	const point3D& inUp )
{
	matrix4D out;
	out.ToObjectLookAt( loc, lookAt, inUp );
	return out;
}

//==========--------------------------  

void matrix4D::ToCameraLookAt( const point3D& loc, const point3D& lookAt, const point3D& inUp )
{

    point3D viewVec = lookAt - loc;
 
    valType mag = viewVec.Mag();
 
    viewVec /= mag;
 
    valType fDot = inUp * viewVec;
 
    point3D upVec = inUp - fDot * viewVec;
 
	upVec.Normalize();
	
    point3D rightVec = upVec ^ viewVec;


    // Start building the matrix. The first three rows contains the basis
    // vectors used to rotate the view to point at the lookat point
    _11 = rightVec.x;    _12 = upVec.x;    _13 = viewVec.x;
    _21 = rightVec.y;    _22 = upVec.y;    _23 = viewVec.y;
    _31 = rightVec.z;    _32 = upVec.z;    _33 = viewVec.z;
 
    // Do the translation values (rotations are still about the eyepoint)
    _41 = - (loc * rightVec);
    _42 = - (loc * upVec);
    _43 = - (loc * viewVec);

    _14 = 0;
    _24 = 0;
    _34 = 0;
    _44 = 1;
}


matrix4D matrix4D::CameraLookAt( 
	const point3D& loc, 
	const point3D& lookAt, 
	const point3D& inUp )
{
	matrix4D out;
	out.ToCameraLookAt( loc, lookAt, inUp );
	return out;
}

//==========--------------------------  

void matrix4D::ToXRot( valType theta )
{
	valType c = (valType) cos(theta);
	valType s = (valType) sin(theta);
	MakeIdent();
	_22 = c;
	_23 = s;
	_32 = -s;
	_33 = c;
}

matrix4D matrix4D::XRot( valType theta )
{
	matrix4D out;
	out.ToXRot( theta );
	return out;
}

//==========--------------------------  

void matrix4D::ToYRot( valType theta )
{
	valType c = (valType) cos(theta);
	valType s = (valType) sin(theta);
	MakeIdent();
	_11 = c;
	_13 = -s;
	_31 = s;
	_33 = c;
}

matrix4D matrix4D::YRot( valType theta )
{
	matrix4D out;
	out.ToYRot( theta );
	return out;
}

//==========--------------------------  

void matrix4D::ToZRot( valType theta )
{
	valType c = (valType) cos(theta);
	valType s = (valType) sin(theta);
	MakeIdent();
	_11 = c;
	_12 = s;
	_21 = -s;
	_22 = c;
}

matrix4D matrix4D::ZRot( valType theta )
{
	matrix4D out;
	out.ToZRot( theta );
	return out;
}

//==========--------------------------  

void matrix4D::ToAxisAngle( const point3D& inAxis, valType angle )
{
	point3D axis = inAxis.Normalized();
	double s = sin( angle );
	double c = cos( angle );
	double t = 1.0 - c;
	double q1, q2;
	double x = axis.x, y = axis.y, z = axis.z;

	_11 = x * x * t + c;
	_22 = y * y * t + c;
	_33 = z * z * t + c;
	
	q1 = x * y * t;
	q2 = z * s;
	_21 = (valType)(q1 - q2);
	_12 = (valType)(q1 + q2);
	
	q1 = x * z * t;
	q2 = y * s;
	_31 = (valType)(q1 + q2);
	_13 = (valType)(q1 - q2);
	
	q1 = y * z * t;
	q2 = x * s;
	_32 = (valType)(q1 - q2);
	_23 = (valType)(q1 + q2);

	_41 = 0;
	_42 = 0;
	_43 = 0;
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;
}

matrix4D matrix4D::AxisAngle( const point3D& axis, valType angle )
{
	matrix4D out;
	out.ToAxisAngle( axis, angle );
	return out;
}


//==========--------------------------  

void matrix4D::ToTranslation( const point3D& p )
{
	MakeIdent();
	_41 = p.x;
	_42 = p.y;
	_43 = p.z;
}

matrix4D matrix4D::Translation( const point3D& p )
{
	matrix4D out;
	out.ToTranslation( p );
	return out;
}


//==========--------------------------  

void matrix4D::ToScale( const point3D& scale )
{
	MakeIdent();
	_11 = scale.x;
	_22 = scale.y;
	_33 = scale.z;
}

matrix4D matrix4D::Scale( const point3D& scale )
{
	matrix4D out;
	out.ToScale( scale );
	return out;
}

//==========--------------------------  

void matrix4D::ToInverse( const matrix4D& in )
{

	// first transpose the rotation matrix
	_11 = in._11;
	_12 = in._21;
	_13 = in._31;
	_21 = in._12;
	_22 = in._22;
	_23 = in._32;
	_31 = in._13;
	_32 = in._23;
	_33 = in._33;

	// fix right column
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;

	// now get the new translation vector
	point3D temp = in.GetLoc();

	_41 = -(valType)(temp.x * in._11 + temp.y * in._12 + temp.z * in._13);
	_42 = -(valType)(temp.x * in._21 + temp.y * in._22 + temp.z * in._23);
	_43 = -(valType)(temp.x * in._31 + temp.y * in._32 + temp.z * in._33);

}

matrix4D matrix4D::Inverse( const matrix4D& in )
{
	matrix4D out;
	out.ToInverse( in );
	return out;
}

//==========--------------------------  

const point3D matrix4D::GetLoc() const
{
	return point3D( _41, _42, _43 );
}

point3D matrix4D::GetAxis( int axis ) const
{
	return point3D
	(
		m[axis][0],
		m[axis][1],
		m[axis][2]
	);
}
