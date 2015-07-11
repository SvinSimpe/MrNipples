#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "3DLibs.h"
#include "GeometryBox.h"

		const unsigned int			MAX_NUM_OF_OBJECTS			= 200;
		const unsigned int			NUM_VERTICES_PER_OBJECT		= 36;

class Level
{
	private:
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		std::vector<GeometryBox*>	mLevelGeometry;
		ID3D11Buffer*				mObjectVertexBuffer;
		ID3D11Buffer*				mPerObjectCBuffer; // mObjectCBuffer ??

		///TEST
		GeometryBox* mBox;
		float		 mRotation;

	private:
		HRESULT CreateVertexBuffer( unsigned int nrOfObjects );
		HRESULT CreatePerObjectCBuffer();

		HRESULT UpdateObjectVertexBuffer();
		HRESULT UpdatePerObjectCBuffer();

	public:
		void	AddBox( XMFLOAT3 origin, float width, float height, float depth, float R, float G, float B );
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