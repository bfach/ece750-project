#include "StdAfx.h"
#include "WirelessProvider.h"


CWirelessProvider::CWirelessProvider()
	: strength(0), signal(0)
{
}


CWirelessProvider::CWirelessProvider( const CString & providerName )
	: name( providerName )
	, strength(0)
	, signal(0)
{
}


CWirelessProvider::~CWirelessProvider(void)
{
}
