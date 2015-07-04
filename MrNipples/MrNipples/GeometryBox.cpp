#include "GeometryBox.h"

GeometryBox::GeometryBox()
{
	mIsVisible = true;
}

GeometryBox::GeometryBox( XMFLOAT3 origin, float width, float height, float depth, float R, float G, float B )
{
	mIsVisible = true;
}

GeometryBox::~GeometryBox()
{}

void GeometryBox::IsVisible( bool isVisible )
{
	if( mIsVisible != isVisible )
		mIsVisible = isVisible; 
}

bool GeometryBox::IsVisible() const
{
	return mIsVisible;
}