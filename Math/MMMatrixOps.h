/********************************************************************************
 * Flux
 *
 * File: MMMatrixOps.h
 * Description: Matrix Operations
 *      Both single and double precision are implemented in the same file. 
 *
 *	Written by:	Ken Turkowski, turkowski@siggraph.org
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


#ifndef __MMMATRIXOPS__


	#define MM_FLOAT	float



	#define MM_DOUBLE	double



	/********************************************************************************
	 ********************************************************************************
	 ********************************************************************************
	 ***		Function Prototypes
	 ********************************************************************************
	 ********************************************************************************
	 ********************************************************************************/


	/********************************************************************************
	 * Compute the L-infinite norm of a matrix, as induced by row vectors.
	 ********************************************************************************/
	MM_FLOAT	MMNormMatrix(
			const MM_FLOAT		*m,
			 long		height,
			 long		width
	);


	/********************************************************************************
	 * Linear transformations, for transforming vectors and matrices.
	 * This works for row vectors and column vectors alike.
	 *	L[nRows][lCol]	- input (left) matrix
	 *	rg[lCol][rCol]	- transformation (right) matrix
	 *	P[nRows][rCol]	- output (product) matrix
	 *
	 * Examples:
	 * v[3] * M[3][3] -> w[3] :			MMLinearTransform(&v[0], &M[0][0], &w[0], 1, 3, 3);
	 * M[3][3] * v[3] -> w[3] :			MMLinearTransform(&M[0][0], &v[0], &w[0], 3, 3, 1);
	 * M[4][4] * N[4][4] -> P[4][4]:	MMLinearTransform(&M[0][0], &N[0][0], &P[0][0], 4, 4, 4);
	 * v[4] * M[4][3] -> w[3]:			MMLinearTransform(&v[0], &M[0][0], &w[0], 1, 4, 3);
	 * v[3] tensor w[3] -> T[3][3]:		MMLinearTransform(&v[0], &w[0], T[3][3], 3, 1, 3);
	 * This can be used In Place, i.e., 
	 * to transform the left matrix
	 * by the right matrix, placing the result back in the left.  By its nature,
	 * then, this can only be used for transforming row vectors or concatenating
	 * matrices from the right.
	 ********************************************************************************/
	void		MMLinearTransform(
			const MM_FLOAT		*L,			/* The left matrix */
			const MM_FLOAT		*R,			/* The right matrix */
			MM_FLOAT			*P,			/* The resultant matrix */
			 long		nRows,		/* The number of rows of the left and resultant matrices */
			 long		lCol,		/* The number of columns in the left matrix */
			 long		rCol		/* The number of columns in the resultant matrix */
	);


	/********************************************************************************
	* MMLUDecompose() decomposes the coefficient matrix A into upper and lower
	* triangular matrices, the composite being the LU matrix.
	* This is then followed by multiple applications of MMLUSolve(),
	* to solve several problems with the same system matrix.
	*
	* 1 is returned if the matrix is non-singular and the decomposition was successful;
	* 0 is returned if the matrix is singular and the decomposition failed.
	********************************************************************************/
	long	MMLUDecompose(
			const MM_FLOAT		*a,			/* the (n x n) coefficient matrix */
			MM_DOUBLE			*lu, 		/* the (n x n) lu matrix augmented by an (n x 1) pivot sequence */
			 long		n			/* the order of the matrix */
	);


	/********************************************************************************
	* MMLUSolve() solves the linear equation (xA = b) after the matrix A has
	* been decomposed with MMLUDecompose() into the lower and upper triangular
	* matrices L and U, giving the equivalent equation (xUL = b).
	********************************************************************************/
	void	MMLUSolve(
			const MM_DOUBLE		*lu,		/* the decomposed LU matrix */
			const MM_FLOAT		*b,			/* the constant vector */
			MM_FLOAT			*x,			/* the solution vector */
			 long		n			/* the order of the equation */
	);


	/********************************************************************************
	* MMInvertMatrix()
	*	Inverts square matrices using row equilibration and partial pivoting.
	*
	* 1 is returned if the matrix was non-singular and the inversion was successful;
	* 0 is returned if the matrix was singular and the inversion failed.
	********************************************************************************/
	long	MMInvertMatrix(
			const MM_FLOAT		*M,			/* The given matrix */
			MM_FLOAT			*Minv,		/* The inverse of the given matrix -- may be the same */
			 long		nCols);




#endif	/* __MMMATRIXOPS__ */




