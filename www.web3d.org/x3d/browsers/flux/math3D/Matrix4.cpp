/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: matrix4.cpp
 *    Desc: Definition for a 4x4 homogenous matrix class
 *          
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"
#include "point3.h"
#include "matrix4.h"
#include "matrix4D.h"

const matrix4 matrix4::Identity = matrix4(
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1);

const matrix4 &matrix4::GetIdentity()
{
	return Identity;
}

//returns matrix4*matrix4
matrix4 operator*(matrix4 const &a, matrix4 const &b)
{
    matrix4 out;      // temporary matrix4 for storing result
    for (int j = 0; j < 4; j++)         // transform by columns first
        for (int i = 0; i < 4; i++)     // then by rows
            out.m[i][j] = (float)(
    				(double)a.m[i][0] * (double)b.m[0][j]
    			+	(double)a.m[i][1] * (double)b.m[1][j]
    			+	(double)a.m[i][2] * (double)b.m[2][j]
    			+	(double)a.m[i][3] * (double)b.m[3][j]
    		);
	return out;
};

// Multiplies two matrices together
void MatMult( const matrix4 &a, const matrix4& b, matrix4* result )
{
    for (int j = 0; j < 4; j++)         // transform by columns first
        for (int i = 0; i < 4; i++)     // then by rows
            result->m[i][j] = (float)(
            		(double)a.m[i][0] * (double)b.m[0][j]
            	+	(double)a.m[i][1] * (double)b.m[1][j]
            	+	(double)a.m[i][2] * (double)b.m[2][j]
            	+	(double)a.m[i][3] * (double)b.m[3][j]
            );
}



matrix4::matrix4( const matrix4D& matf )
{
	_11 = ( float )matf._11;
	_12 = ( float )matf._12;
	_13 = ( float )matf._13;
	_14 = ( float )matf._14;

	_21 = ( float )matf._21;
	_22 = ( float )matf._22;
	_23 = ( float )matf._23;
	_24 = ( float )matf._24;

	_31 = ( float )matf._31;
	_32 = ( float )matf._32;
	_33 = ( float )matf._33;
	_34 = ( float )matf._34;
	
	_41 = ( float )matf._41;
	_42 = ( float )matf._42;
	_43 = ( float )matf._43;
	_44 = ( float )matf._44;
}



void matrix4::Rotate( float yaw, float pitch, float roll )
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

void matrix4::Translate( const point3& amt)
{
    _41 += amt.x;
    _42 += amt.y;
    _43 += amt.z;
}

void matrix4::Place( const point3& loc)
{
    _41 = loc.x;
    _42 = loc.y;
    _43 = loc.z;
}

void matrix4::MakeIdent()
{
	_11 = 1.f;	_22 = 1.f;	_33 = 1.f;	_44 = 1.f;
	_12 = 0.f;  _13 = 0.f;  _14 = 0.f;	_21 = 0.f;
	_23 = 0.f;	_24 = 0.f;	_31 = 0.f;	_32 = 0.f;	
	_34 = 0.f;	_41 = 0.f;	_42 = 0.f;	_43 = 0.f;	
}

//==========--------------------------  

void matrix4::ToObjectLookAt( const point3& loc, const point3& lookAt, const point3& inUp )
{

    point3 viewVec = lookAt - loc;
 
    float mag = viewVec.Mag();
 
    viewVec /= mag;
 
    float fDot = inUp * viewVec;
 
    point3 upVec = inUp - fDot * viewVec;
 
	upVec.Normalize();
	
    point3 rightVec = upVec ^ viewVec;


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

matrix4 matrix4::ObjectLookAt( 
	const point3& loc, 
	const point3& lookAt, 
	const point3& inUp )
{
	matrix4 out;
	out.ToObjectLookAt( loc, lookAt, inUp );
	return out;
}

//==========--------------------------  

void matrix4::ToCameraLookAt( const point3& loc, const point3& lookAt, const point3& inUp )
{

    point3 viewVec = lookAt - loc;
 
    float mag = viewVec.Mag();
 
    viewVec /= mag;
 
    float fDot = inUp * viewVec;
 
    point3 upVec = inUp - fDot * viewVec;
 
	upVec.Normalize();
	
    point3 rightVec = upVec ^ viewVec;


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


matrix4 matrix4::CameraLookAt( 
	const point3& loc, 
	const point3& lookAt, 
	const point3& inUp )
{
	matrix4 out;
	out.ToCameraLookAt( loc, lookAt, inUp );
	return out;
}

//==========--------------------------  

void matrix4::ToXRot( float theta )
{
	float c = (float) cos(theta);
	float s = (float) sin(theta);
	MakeIdent();
	_22 = c;
	_23 = s;
	_32 = -s;
	_33 = c;
}

matrix4 matrix4::XRot( float theta )
{
	matrix4 out;
	out.ToXRot( theta );
	return out;
}

//==========--------------------------  

void matrix4::ToYRot( float theta )
{
	float c = (float) cos(theta);
	float s = (float) sin(theta);
	MakeIdent();
	_11 = c;
	_13 = -s;
	_31 = s;
	_33 = c;
}

matrix4 matrix4::YRot( float theta )
{
	matrix4 out;
	out.ToYRot( theta );
	return out;
}

//==========--------------------------  

void matrix4::ToZRot( float theta )
{
	float c = (float) cos(theta);
	float s = (float) sin(theta);
	MakeIdent();
	_11 = c;
	_12 = s;
	_21 = -s;
	_22 = c;
}

matrix4 matrix4::ZRot( float theta )
{
	matrix4 out;
	out.ToZRot( theta );
	return out;
}

//==========--------------------------  

void matrix4::ToAxisAngle( const point3& inAxis, float angle )
{
	point3 axis = inAxis.Normalized();
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
	_21 = (float)(q1 - q2);
	_12 = (float)(q1 + q2);
	
	q1 = x * z * t;
	q2 = y * s;
	_31 = (float)(q1 + q2);
	_13 = (float)(q1 - q2);
	
	q1 = y * z * t;
	q2 = x * s;
	_32 = (float)(q1 - q2);
	_23 = (float)(q1 + q2);

	_41 = 0;
	_42 = 0;
	_43 = 0;
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;
}

matrix4 matrix4::AxisAngle( const point3& axis, float angle )
{
	matrix4 out;
	out.ToAxisAngle( axis, angle );
	return out;
}


//==========--------------------------  

void matrix4::ToTranslation( const point3& p )
{
	MakeIdent();
	_41 = p.x;
	_42 = p.y;
	_43 = p.z;
}

matrix4 matrix4::Translation( const point3& p )
{
	matrix4 out;
	out.ToTranslation( p );
	return out;
}


//==========--------------------------  

void matrix4::ToScale( const point3& scale )
{
	MakeIdent();
	_11 = scale.x;
	_22 = scale.y;
	_33 = scale.z;
}

matrix4 matrix4::Scale( const point3& scale )
{
	matrix4 out;
	out.ToScale( scale );
	return out;
}

//==========--------------------------  

void matrix4::ToInverse( const matrix4& in )
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
	point3 temp = in.GetLoc();

	_41 = -(float)((double)temp.x * (double)in._11 + (double)temp.y * (double)in._12 + (double)temp.z * (double)in._13);
	_42 = -(float)((double)temp.x * (double)in._21 + (double)temp.y * (double)in._22 + (double)temp.z * (double)in._23);
	_43 = -(float)((double)temp.x * (double)in._31 + (double)temp.y * (double)in._32 + (double)temp.z * (double)in._33);

}

matrix4 matrix4::Inverse( const matrix4& in )
{
	matrix4 out;
	out.ToInverse( in );
	return out;
}

//==========--------------------------  

const point3 matrix4::GetLoc() const
{
	return point3( _41, _42, _43 );
}

point3 matrix4::GetAxis( int axis ) const
{
	return point3
	(
		m[axis][0],
		m[axis][1],
		m[axis][2]
	);
}


