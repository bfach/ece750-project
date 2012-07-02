#pragma once
#include "WirelessProvider.h"
#include "MobileSystem.h"

class CWorld
{
public:
	CWorld();
	virtual ~CWorld();
	virtual void Draw( CPaintDC & dc );

protected:
	CArray<CWirelessProvider> wirelessProviders;
	CMobileSystem mobileSystem;
};

