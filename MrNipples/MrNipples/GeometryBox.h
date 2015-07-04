#ifndef _GEOMETRYBOX_H_
#define _GEOMETRYBOX_H_

#include "3DLibs.h"

struct Vertex32
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;
};

class GeometryBox
{
	private:
		Vertex32 mVertices[8];
		bool	 mIsVisible;
	public:
		GeometryBox();
		GeometryBox( XMFLOAT3 origin, float width, float height, float depth, float R, float G, float B );
		~GeometryBox();
		void IsVisible( bool isVisible );
		bool IsVisible() const;
};
#endif