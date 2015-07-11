#ifndef _GEOMETRYBOX_H_
#define _GEOMETRYBOX_H_

#include "3DLibs.h"

struct Vertex32
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
};

class GeometryBox
{
	

	private:
		bool	 mIsVisible;

	public: 
		static unsigned int sID;
		unsigned int		mID;
		Vertex32			mVertexFaces[36];

	public:
		GeometryBox();
		GeometryBox( XMFLOAT3 origin, float width, float height, float depth );
		~GeometryBox();
		void IsVisible( bool isVisible );
		bool IsVisible() const;
};
#endif