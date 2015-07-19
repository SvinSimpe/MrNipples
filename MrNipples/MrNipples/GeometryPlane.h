#ifndef _GEOMETRYPLANE_H_
#define _GEOMETRYPLANE_H_

#include "3DLibs.h"
#include "Vertex48.h"

const unsigned int PLANE_VERTEX_COUNT = 6;

class GeometryPlane
{
	private:
		const unsigned int	NUM_VERTICES = 36;
		Vertex48*			mVertexFaces;
		
	public:
		GeometryPlane();
		GeometryPlane( XMFLOAT3 origin, float width, float depth );
		~GeometryPlane();

		Vertex48*		VertexFaces();
		void			Release();
};
#endif