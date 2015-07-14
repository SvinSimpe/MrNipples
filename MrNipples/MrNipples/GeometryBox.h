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
		unsigned int	mID;
		bool			mIsVisible;
		Vertex32*		mVertexFaces;
		PerInstanceData	mPerInstanceData;

	public: 
		static unsigned int sID;
		
	public:
		GeometryBox();
		GeometryBox( XMFLOAT3 origin, float width, float height, float depth );
		~GeometryBox();

		unsigned int	ID() const;
		void			IsVisible( bool isVisible );
		bool			IsVisible() const;
		Vertex32*		VertexFaces();
		PerInstanceData	PerInstanceData();
		void			PerInstanceData( XMFLOAT4X4 data );

		void			Release();
};
#endif