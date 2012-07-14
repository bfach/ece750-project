#pragma once
#include "WirelessProvider.h"
#include "MobileSystem.h"

class CHandoverAlgorithm;

class CWorld
{
public:
	CWorld( CHandoverAlgorithm * pAlg );
	virtual ~CWorld();

	virtual void Draw( CDC & dc );
	void Tick( int nTickCount );

	CArray<CWirelessProvider> wirelessProviders;
	CMobileSystem mobileSystem;

	int nProviderConnectedTo;

	double fTotalCost;
	double fTotalBandwidth;
	double fAverageSignalQuality;
	double fTotalPowerUsed;

protected:
	void DrawTower( CDC & dc, int x, int y, int width, int height );
	CHandoverAlgorithm * m_pAlg;
};

