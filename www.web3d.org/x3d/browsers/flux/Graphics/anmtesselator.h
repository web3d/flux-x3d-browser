/********************************************************************************
 * Flux
 *
 * File: anmtesselator.h
 * Description: Tesselator class - tesselates non-
 *							convex polygons using GLU tesselator
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

#ifndef _anmtesselator_h
#define _anmtesselator_h

class EXPORT CAnmTesselator
{
protected:

	class GLUtesselator			*m_GLUtesselator;
	std::vector<int>			 m_indices;
	double						*m_coords;
	int							 m_primtype;
	int							 m_firstprimindex;
	int							 m_previous[2];
	BOOL						 m_combineFlag;
	BOOL						 m_errorFlag;
	int							 m_errnum;

	static void __stdcall TessBeginCallback(int type, void *polygon_data);
	static void __stdcall TessVertexCallback(void *vertex_data, void *polygon_data);
	static void __stdcall TessCombineCallback(double coords[3], void *vertex_data[4],
		float weight[4], void **outData, void *polygon_data);
	static void __stdcall TessErrorCallback(int errnum, void *polygon_data);

	void BeginPrimitive(int type);
	void AddIndex(double *coord);
	void Combine(double coords[3], double *vertex_data[4],
		float weight[4], double **outData);
	void Error(int errnum);

	BOOL PlanarTest(double *coords, int &numIndices);

public:

	CAnmTesselator();
	virtual  ~CAnmTesselator();
	int  *Tesselate(double *coords, int &numIndices);
	int  *Tesselate(double *coords, int &numIndices, int *countours, int countourcount);
};

#endif // _anmtesselator_h

