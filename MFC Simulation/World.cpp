#include "StdAfx.h"
#include "World.h"
#include "WirelessProvider.h"

CWorld::CWorld(void)
{
	// semi-randomly locate the wireless location 
	// TODO: Make this work from the Ui
	CWirelessProvider p1( L"WiFi at Home" );
	p1.strength = 60;
	p1.x = 100;
	p1.y = 100;

	CWirelessProvider p2( L"Rogers" );
	p2.strength = 220;
	p2.x = 230;
	p2.y = 120;

	CWirelessProvider p3( L"WiFi at Work" );
	p3.strength = 80;
	p3.x = 310;
	p3.y = 140;

	wirelessProviders.Add( p1 );
	wirelessProviders.Add( p2 );
	wirelessProviders.Add( p3 );
}


CWorld::~CWorld(void)
{
}


void CWorld::Draw( CPaintDC & dc )
{
	for ( int i=0; i<wirelessProviders.GetCount(); ++i )
	{
		CWirelessProvider & p = wirelessProviders.GetAt( i );

		COLORREF qCircleColor = RGB(255,0,0);
		CPen qCirclePen(PS_SOLID, 2, qCircleColor);
		dc.SelectObject( qCirclePen );

		dc.Ellipse( p.x - p.strength/2, p.y - p.strength/2, p.x + p.strength/2, p.y + p.strength/2);
	}

	COLORREF qCircleColor = RGB(255,255,0);
	CPen qCirclePen(PS_SOLID, 2, qCircleColor);
	dc.SelectObject( qCirclePen );

	dc.Ellipse( 0,0, 10, 10);
}
