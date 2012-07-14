#pragma once
class CLocatableObject
{
public:
	CLocatableObject();
	virtual ~CLocatableObject();

	double getDistanceFrom( long sx, long sy );

	long x;
	long y;
};

