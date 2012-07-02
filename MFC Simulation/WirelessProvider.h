#pragma once
#include "locatableobject.h"
class CWirelessProvider :
	public CLocatableObject
{
public:
	CWirelessProvider();
	CWirelessProvider( const CString & providerName );
	virtual ~CWirelessProvider();

	long strength;
	CString name;
};

