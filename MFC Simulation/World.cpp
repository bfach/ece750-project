#include "StdAfx.h"
#include "resource.h"
#include "World.h"
#include "WirelessProvider.h"
#include "HandoverAlgorithm.h"


static const int MS_X = 70;
static const int MS_Y = 115;


CWorld::CWorld(CHandoverAlgorithm * alg)
	: m_pAlg(alg)
	, nProviderConnectedTo( -1 )
	, fTotalCost(0)
	, fTotalBandwidth(0)
	, fAverageSignalQuality(0)
	, fTotalPowerUsed(0)
{
	// semi-randomly locate the wireless location 
	// TODO: Make this work from the Ui
	CWirelessProvider p1( L"WiFi at Home" );
	p1.strength = 40;
	p1.x = 100;
	p1.y = 100;

	CWirelessProvider p2( L"Rogers" );
	p2.strength = 130;
	p2.x = 230;
	p2.y = 140;

	CWirelessProvider p3( L"WiFi at Work" );
	p3.strength = 50;
	p3.x = 310;
	p3.y = 140;

	wirelessProviders.Add( p2 );
	wirelessProviders.Add( p1 );
	wirelessProviders.Add( p3 );

	mobileSystem.x = MS_X;
	mobileSystem.y = MS_Y;

	nProviderConnectedTo = m_pAlg->ChooseProvider( *this );
}


CWorld::~CWorld()
{
	if ( m_pAlg ) delete m_pAlg;
}


void CWorld::Draw( CDC & dc )
{
	for ( int i=0; i<wirelessProviders.GetCount(); ++i )
	{
		CWirelessProvider & p = wirelessProviders.GetAt( i );

		COLORREF qCircleColor = RGB(255,0,0);
		CPen qCirclePen(PS_SOLID, 2, qCircleColor);
		dc.SelectObject( qCirclePen );

		dc.Ellipse( p.x - p.strength, p.y - p.strength, p.x + p.strength, p.y + p.strength);
		DrawTower( dc, p.x, p.y, p.strength, p.strength );
	}

	COLORREF qCircleColor = RGB(255,255,0);
	CPen qCirclePen(PS_SOLID, 2, qCircleColor);
	dc.SelectObject( qCirclePen );

	dc.Ellipse( mobileSystem.x - 5, mobileSystem.y - 5, mobileSystem.x + 5, mobileSystem.y + 5 );
}


void CWorld::DrawTower( CDC & dc, int x, int y, int width, int height )
{
	CBitmap bmTower;
	if ( !bmTower.LoadBitmap( IDB_TOWER ) ) return;

	CDC mdc;
	mdc.CreateCompatibleDC( &dc );
	mdc.SelectObject( bmTower );

	dc.StretchBlt( x-width/2, y-width/2, width, height, &mdc, 0,0,114,114, SRCCOPY );
}


void CWorld::Tick( int nTick )
{
	// Move the mobile system
	mobileSystem.x = nTick + MS_X;

	// Calculate Signal Strengths
	for ( int i=0; i<wirelessProviders.GetCount(); ++i )
	{
		CWirelessProvider & p = wirelessProviders.GetAt( i );

		double distance = p.getDistanceFrom( mobileSystem.x, mobileSystem.y );
		double ds = ( distance / (double)p.strength );
		p.signal = 100.0 * ( 1.0 - (ds*ds) );

		if ( p.signal < 0 ) p.signal = 0.0;
	}

	// See if we switched to a new provider
	int newProvider = m_pAlg->ChooseProvider( *this );
	if ( newProvider != nProviderConnectedTo )
	{
		ATLTRACE( L"%s: Switching providers to %d\n", __FUNCTIONW__, newProvider );
		nProviderConnectedTo = newProvider;
	}
}


// signal str
// network connection time -- total time connected to this session
// network connection total
// handover latency
// available bandwidth
// power consumption
// cost per megabyte
// security 
// user preference

