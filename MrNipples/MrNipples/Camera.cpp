#include "Camera.h"

Camera::Camera()
{
	mEyePosition		= XMFLOAT3( 30.0f, 120.0f, -250.0f); // TEST POSITION

	//mEyePosition		= XMFLOAT3( -20.0f, 10.0f, 150.0f); // RECORD POSITION
	//mEyePosition		= XMFLOAT3( 25.0f, 50.0f, -200.0f);	// OVERVIEW (FOCUS Z=100)
	//mEyePosition		= XMFLOAT3( -15.0f, 10.0f, 200.0f); // SCREENSHOT POSITION (FOCUS Z=190) | Lighting in Level.cpp == XMFLOAT4( -40.0f, 20.0f, 210.0f, 350.0f );
	//mEyePosition		= XMFLOAT3( -55.0f, 50.0f, 100.0f); // TOP VIEW (z same as focus.z)
	mFocusPosition		= XMFLOAT3( 0.0f, 0.0f, -80.0f );
	mUpDirection		= XMFLOAT3( 0.0f, 1.0f,  0.0f );
	mRightDirection		= XMFLOAT3( 1.0f, 0.0f,  0.0f );

	XMMATRIX tempViewMatrix = XMMatrixLookAtLH( XMLoadFloat3( &mEyePosition ), 
												XMLoadFloat3( &mFocusPosition ),
												XMLoadFloat3( &mUpDirection ) );
	XMStoreFloat4x4( &mViewMatrix, tempViewMatrix );


	XMMATRIX tempProjectionMatrix = XMMatrixPerspectiveFovLH( 0.75f, 1280.0f / 720.0f, 0.5f, 50000.0f );
	XMStoreFloat4x4( &mProjectionMatrix, tempProjectionMatrix );
}

Camera::~Camera()
{

}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return mViewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

XMFLOAT3 Camera::GetEyePosition() const
{
	return mEyePosition;
}

//void Camera::Walk( float distance )
//{
//	XMVECTOR s = XMVectorReplicate( distance );
//	XMVECTOR l = XMLoadFloat3( &m_focusPosition );
//	XMVECTOR p = XMLoadFloat3( &m_eyePosition );
//	XMStoreFloat3( &m_eyePosition, XMVectorMultiplyAdd( s, l, p ) );
//}
//
//void Camera::Strafe( float distance )
//{
//	XMVECTOR s = XMVectorReplicate( distance );
//	XMVECTOR r = XMLoadFloat3( &m_rightDirection );
//	XMVECTOR p = XMLoadFloat3( &m_eyePosition );
//	XMStoreFloat3( &m_eyePosition, XMVectorMultiplyAdd( s, r, p ) );
//}
//
//void Camera::SetRotate( float angle )
//{
//	// Rotate the basis vectors about the world y-axis
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY( angle );
//
//	DirectX::XMStoreFloat3( &m_rightDirection, 
//							DirectX::XMVector3TransformNormal( DirectX::XMLoadFloat3( &m_rightDirection), R ) );
//	DirectX::XMStoreFloat3( &m_upDirection, DirectX::XMVector3TransformNormal( DirectX::XMLoadFloat3( &m_upDirection), R ) );
//	DirectX::XMStoreFloat3( &m_focusPosition, DirectX::XMVector3TransformNormal( DirectX::XMLoadFloat3( &m_focusPosition), R ) );
//}
//
//void Camera::SetPitch( float angle )
//{
//	// Rotate Up och Look vectors about the Right vector
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis( DirectX::XMLoadFloat3( &m_rightDirection ), angle );
//
//	DirectX::XMStoreFloat3( &m_upDirection, DirectX::XMVector3TransformNormal( DirectX::XMLoadFloat3( &m_upDirection ), R ) );
//	DirectX::XMStoreFloat3( &m_focusPosition, DirectX::XMVector3TransformNormal( DirectX::XMLoadFloat3( &m_focusPosition ), R ) );
//}
//
//void Camera::AddEyePosition( float x = 0.0f, float y = 0.0f, float z = 0.0f )
//{
//	m_eyePosition.x += x;
//	m_eyePosition.y += y;
//	m_eyePosition.z += z;
//}

void Camera::UpdateViewMatrix()
{
	XMMATRIX tempViewMatrix = XMMatrixLookAtLH( XMLoadFloat3( &mEyePosition ), 
												XMLoadFloat3( &mFocusPosition ),
												XMLoadFloat3( &mUpDirection ) );
	XMStoreFloat4x4( &mViewMatrix, tempViewMatrix );
}