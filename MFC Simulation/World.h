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
	CArray<CWirelessProvider> wirelessProviders;
	CMobileSystem mobileSystem;

};

