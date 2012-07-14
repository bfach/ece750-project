#include "StdAfx.h"
#include "HandoverSignalStrength.h"
#include "World.h"

CHandoverSignalStrength::CHandoverSignalStrength(void)
{
}


CHandoverSignalStrength::~CHandoverSignalStrength(void)
{
}

int CHandoverSignalStrength::ChooseProvider( const CWorld & world )
{
	int maxSignalStrength = 0;
	int maxProvider = -1;

	for ( int i=0; i<world.wirelessProviders.GetCount(); ++i )
	{
		const CWirelessProvider & p = world.wirelessProviders.GetAt( i );
		if ( p.signal > maxSignalStrength )
		{
			maxSignalStrength = p.signal;
			maxProvider = i;
		}
	}

	return maxProvider;
}
