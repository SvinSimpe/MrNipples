#ifndef _GEOMETRYBOX_H_
#define _GEOMETRYBOX_H_

#include "3DLibs.h"
#include "VertexLayout.h"

const unsigned int BOX_VERTEX_COUNT = 36;

class GeometryBox
{
	private:
		Vertex48*			mVertexFaces;
		
	public:
		GeometryBox();
		GeometryBox( XMFLOAT3 origin, float width, float height, float depth );
		~GeometryBox();

		Vertex48*		VertexFaces();
		void			Release();
};
#endif