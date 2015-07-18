#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "3DLibs.h"
#include "GeometryBox.h"
#include "DDSTextureLoader.h"

		const unsigned int			MAX_NUM_OF_OBJECTS			= 200;
		const unsigned int			NUM_VERTICES_PER_OBJECT		= 36;

class Level
{
	private:
		ID3D11Device*					mDevice;
		ID3D11DeviceContext*			mDeviceContext;
		std::vector<PerInstanceData>	mInstanceData;
		std::vector<PointLightData>		mPointLightData;
		ID3D11Buffer*					mObjectVertexBuffer;
		ID3D11Buffer*					mInstanceBuffer;
		ID3D11Buffer*					mLightBuffer;

		TextureData						mBrickTextureData;
		ID3D11ShaderResourceView*		mShaderResourceView;
		ID3D11SamplerState*				mSamplerState;

		// TEMPORARY FOR MOVING LIGHT!
		int								mDirection;

		GeometryBox*					mBox;
		float							mRotation;

	private:
		HRESULT CreateVertexBuffer();
		HRESULT CreatePerInstanceBuffer();
		HRESULT CreateLightBuffer();

		HRESULT UpdateObjectVertexBuffer();
		HRESULT UpdatePerInstanceBuffer();
		HRESULT UpdateLightBuffer();

	public:
		void	AddBox( XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 translation, XMFLOAT3 color );
		void	CheckCollision();

		void	Update( float deltaTime );
		void	Render( float deltaTime );
		HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
				Level();
				~Level();
		void	Release();
};
#endif