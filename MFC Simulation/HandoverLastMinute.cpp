#include "StdAfx.h"
#include "HandoverLastMinute.h"
#include "HandoverSignalStrength.h"
#include "World.h"

CHandoverLastMinute::CHandoverLastMinute(void)
{
}


CHandoverLastMinute::~CHandoverLastMinute(void)
{
}


int CHandoverLastMinute::ChooseProvider( const CWorld & world )
{
	// Can we stay connected?
	if ( (world.nProviderConnectedTo != -1 ) && ( world.wirelessProviders[world.nProviderConnectedTo].signal > 0 ) ) return world.nProviderConnectedTo;

	// If not, let's use the best signal
	CHandoverSignalStrength h;
	return h.ChooseProvider( world );
}
