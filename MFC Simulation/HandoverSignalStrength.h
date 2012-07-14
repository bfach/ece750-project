#pragma once

#include "HandoverAlgorithm.h"

class CHandoverSignalStrength : public CHandoverAlgorithm
{
public:
	CHandoverSignalStrength();
	virtual ~CHandoverSignalStrength();

	virtual int ChooseProvider( const CWorld & world );
};

