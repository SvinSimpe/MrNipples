#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "3DLibs.h"
#include "GeometryBox.h"

class Level
{
	private:
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		std::vector<GeometryBox>	mLevelGeometry;

	public:
		void	AddGeometry( GeometryBox newBox );
		void	SetObjectToRender( int index, bool renderObject );
		void	CheckCollision();

		void	Update( float deltaTime );
		void	Render( float deltaTime );
		HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
				Level();
				~Level();
		void	Release();
};
#endif