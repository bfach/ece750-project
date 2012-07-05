#pragma once
#include "WirelessProvider.h"
#include "MobileSystem.h"

class CWorld
{
public:
	CWorld();
	virtual ~CWorld();

	virtual void Draw( CDC & dc );
	void Tick();

protected:
	void DrawTower( CDC & dc, int x, int y, int width, int height );

protected:
	CArray<CWirelessProvider> wirelessProviders;
	CMobileSystem mobileSystem;

};

