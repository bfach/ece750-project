#pragma once

class CWorld;

class CHandoverAlgorithm
{
public:
	CHandoverAlgorithm();
	virtual ~CHandoverAlgorithm();

	virtual int ChooseProvider( const CWorld & world ) = 0;
};

