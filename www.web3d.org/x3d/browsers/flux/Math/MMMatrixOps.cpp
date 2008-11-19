/********************************************************************************
 * Flux
 *
 * File: MMMatrixOps.cpp
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


/********************************************************************************
 ********************************************************************************
 ********************************************************************************
 ***	We implement single and double in the same file.
 ***	For every function, we need in this file:
 ***		- #define for double precision
 ***		- single precision implementation
 ***	we also need three (3) entries in the header:
 ***		- single precision function prototype
 ***		- #define for double precision
 ***		- #undef  for double precision
 ********************************************************************************
 ********************************************************************************
 ********************************************************************************/


#define DOUBLE								double

#define MAXDIM	32			/* The maximum dimension of a matrix */


/********************************************************************************
 * MMNormMatrix() computes the L-infinity norm (induced by ROW vectors)
 * of the largest square submatrix of the given matrix.
 ********************************************************************************/

FLOAT
MMNormMatrix(register const FLOAT *m0,  long height, register  long width)
{
	register  long	i;
	 long			j;
	register const FLOAT	*m;
	register DOUBLE			sum;
	register FLOAT			max		= 0;
	 long			n		= (width < height) ? width : height;
	
	for (j = n, m = m0; j--; m = ++m0) {
		for (i = n, sum = 0; i--; m += width)
			sum += fabs(*m);
		if (max < sum)
			max = sum;
	}
	
	return(max);
}


/********************************************************************************
 * MMLinearTransform
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

void
MMLinearTransform(
	const    FLOAT	*L,				/* The left matrix */
	const    FLOAT	*R,				/* The right matrix */
	register FLOAT	*P,				/* The resultant matrix */
	 long	nRows,			/* The number of rows of the left and resultant matrices */
	 long	lCol,			/* The number of columns in the left matrix */
	 long	rCol			/* The number of columns in the resultant matrix */
)
{
	register const FLOAT	*lp;	/* Left matrix pointer for dot product */
	register const char		*rp;	/* Right matrix pointer for dot product */
	register  long	k;		/* Loop counter */
	register DOUBLE			sum;	/* Extended precision for intermediate results */
	register  long	rrb;
	register  long	j, i;	/* Loop counters */
	register  long	lrb;
	const char				*lb;

	rrb = rCol * sizeof(FLOAT);
	lrb = lCol * sizeof(FLOAT);

	for (i = nRows, lb = (const char*)L; i--; lb += lrb) {	/* Each row in L */
		for (j = 0; j < rCol; j++) {						/* Each column in R */
			lp = (const FLOAT *)lb;							/* Left of ith row of L */
			rp = (const char *)(R + j);						/* Top of jth column of R */
			sum = 0;
			for (k = lCol; k--; rp += rrb)
				sum += *lp++ * (*((const FLOAT*)rp));		/* *P += L[i'][k'] * R[k'][j] */
			*P++ = sum;
		}
	}
}


#define luel(i, j)  lu[(i)*n+(j)]
#define ael(i, j)	a[(i)*n+(j)]


/********************************************************************************
 * MMLUDecompose() decomposes the coefficient matrix A into upper and lower
 * triangular matrices, the composite being the LU matrix.
 * This is then followed by multiple applications of MMLUSolve(),
 * to solve several problems with the same system matrix.
 *
 * 1 is returned if the matrix is non-singular and the decomposition was successful;
 * 0 is returned if the matrix is singular and the decomposition failed.
 ********************************************************************************/

long
MMLUDecompose(
	register const FLOAT	*a,		/* the (n x n) coefficient matrix */
	register DOUBLE			*lu, 	/* the (n x n) lu matrix augmented by an (n x 1) pivot sequence */
	register long	n		/* the order of the matrix */
)
{
	register long	i, j, k;
	 long			pivotindex;
	register long	*ps;
	DOUBLE					pivot, biggest, mult, tempf;
	DOUBLE					scales[MAXDIM];

	ps = (long *)(&lu[n*n]); /* Memory for ps[] comes after LU[][] */

	for (i = 0; i < n; i++) {	/* For each row */
		/* Find the largest element in each row for row equilibration */
		biggest = 0.0;
		for (j = 0; j < n; j++)
			if (biggest < (tempf = fabs(luel(i,j) = ael(j,i)))) /* A transposed for row vectors */
				biggest = tempf;
		if (biggest != 0.0)
			scales[i] = 1.0 / biggest;
		else {
			scales[i] = 0.0;
			return(0);	/* Zero row: singular matrix */
		}

		ps[i] = i;		/* Initialize pivot sequence */
	}

	for (k = 0; k < n-1; k++) { /* For each column */
		/* Find the largest element in each column to pivot around */
		biggest = 0.0;
		for (i = k; i < n; i++) {
			if (biggest < (tempf = fabs(luel(ps[i],k)) * scales[ps[i]])) {
				biggest = tempf;
				pivotindex = i;
			}
		}
		if (biggest == 0.0)
			return(0);	/* Zero column: singular matrix */
		if (pivotindex != k) {	/* Update pivot sequence */
			j = ps[k];
			ps[k] = ps[pivotindex];
			ps[pivotindex] = j;
		}

		/* Pivot, eliminating an extra variable each time */
		pivot = luel(ps[k],k);
		for (i = k+1; i < n; i++) {
			luel(ps[i],k) = mult = luel(ps[i],k) / pivot;
			if (mult != 0.0) {
				for (j = k+1; j < n; j++)
					luel(ps[i],j) -= mult * luel(ps[k],j);
			}
		}
	}
	return(luel(ps[n-1],n-1) != 0.0);	/* 0 if singular, 1 if not */
}


/********************************************************************************
 * MMLUSolve() solves the linear equation (xA = b) after the matrix A has
 * been decomposed with MMLUDecompose() into the lower and upper triangular
 * matrices L and U, giving the equivalent equation (xUL = b).
 ********************************************************************************/

void
MMLUSolve(
	register const DOUBLE	*lu,	/* the decomposed LU matrix */
	register const FLOAT	*b,		/* the constant vector */
	register FLOAT			*x,		/* the solution vector */
	register long	n		/* the order of the equation */
)
{
	register long					i, j;
	DOUBLE							dot;
	register const long				*ps;
	
	ps = (const long *)(&lu[n*n]); /* Memory for ps[] comes after LU[][] */

	/* Vector reduction using U triangular matrix */
	for (i = 0; i < n; i++) {
		dot = 0.0;
		for (j = 0; j < i; j++)
			dot += luel(ps[i],j) * x[j];
		x[i] = b[ps[i]] - dot;
	}

	/* Back substitution, in L triangular matrix */
	for (i = n-1; i >= 0; i--) {
		dot = 0.0;
		for (j = i+1; j < n; j++)
			dot += luel(ps[i],j) * x[j];
		x[i] = (x[i] - dot) / luel(ps[i],i);
	}
}


/********************************************************************************
 * MMInvertMatrix()
 *	Inverts square matrices
 *	With tall matrices, invert upper part and transform the bottom
 *	rows as would be expected if embedded into a larger matrix.
 *	Undefined for wide matrices.
 * M^(-1) --> Minv
 * IN PLACE SUPPORTED, no performance difference
 *
 * 1 is returned if the matrix was non-singular and the inversion was successful;
 * 0 is returned if the matrix was singular and the inversion failed.
 ********************************************************************************/

long
MMInvertMatrix(const FLOAT *M, FLOAT *Minv, register long nCols)
{
	FLOAT			*m;
	FLOAT			b[MAXDIM];
	DOUBLE			lu[MAXDIM*MAXDIM+MAXDIM];
	register long	j, i;

	/* Decompose matrix into L and U triangular matrices */
	if (MMLUDecompose(M, lu, nCols) == 0) {
		return(0);		/* Singular */
	}

	/* Invert matrix by solving nCols simultaneous equations nCols times */
	for (i = 0, m = Minv; i < nCols; i++, m += nCols) {
		for(j = 0; j < nCols; j++)
			b[j] = 0;
		b[i] = 1;

		MMLUSolve(lu, b, m, nCols);	/* Into a row of m */
	}

	return(1);
}



