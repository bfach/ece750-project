#pragma once
#include "handoveralgorithm.h"
class CHandoverLastMinute :
	public CHandoverAlgorithm
{
public:
	CHandoverLastMinute(void);
	virtual ~CHandoverLastMinute(void);

	virtual int ChooseProvider( const CWorld & world );

};

