#include "StdAfx.h"
#include "LocatableObject.h"
#include <math.h>

CLocatableObject::CLocatableObject()
	: x(0), y(0)
{
}


CLocatableObject::~CLocatableObject()
{
}


double CLocatableObject::getDistanceFrom( long sx, long sy )
{
	double distance_x = x-sx;
	double distance_y = y-sy; 

	double distance = sqrt( (distance_x * distance_x) + (distance_y * distance_y));

	return distance;
}

