#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "3DLibs.h"

class Level
{
	private:
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;

	public:
		void	Update( float deltaTime );
		void	Render( float deltaTime );
		HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
				Level();
				~Level();
		void	Release();
};
#endif