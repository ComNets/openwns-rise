/*******************************************************************************
 * This file is part of openWNS (open Wireless Network Simulator)
 * _____________________________________________________________________________
 *
 * Copyright (C) 2004-2007
 * Chair of Communication Networks (ComNets)
 * Kopernikusstr. 5, D-52074 Aachen, Germany
 * phone: ++49-241-80-27910,
 * fax: ++49-241-80-22242
 * email: info@openwns.org
 * www: http://www.openwns.org
 * _____________________________________________________________________________
 *
 * openWNS is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 2 as published by the
 * Free Software Foundation;
 *
 * openWNS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include <RISE/transceiver/tests/propcacheTest.hpp>
#include <RISE/transceiver/cache/propagationcache.hpp>
#include <RISE/transceiver/cache/idvectorcache.hpp>
#include <RISE/transceiver/cache/vectorcache.hpp>
#include <RISE/transceiver/cache/hashcache.hpp>
#include <RISE/transceiver/cache/h2cache.hpp>
#include <RISE/transceiver/cache/nocache.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>

using namespace rise;
using namespace rise::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( PropCacheTest );

void PropCacheTest::setUp()
{
	systemManager = new SystemManagerDropIn();

	r1 = new PropCacheTestReceiver(systemManager);

	frequency = 100;

	lossT1U1 = r1->calculateLoss(1,1);
	lossT2U1 = r1->calculateLoss(2,1);
	lossT3U1 = r1->calculateLoss(3,1);

	lossT1U2 = r1->calculateLoss(1,2);
	lossT2U2 = r1->calculateLoss(2,2);
	lossT3U2 = r1->calculateLoss(3,2);

	lossT1U3 = r1->calculateLoss(1,3);
	lossT2U3 = r1->calculateLoss(2,3);
	lossT3U3 = r1->calculateLoss(3,3);

	lossT1U4 = r1->calculateLoss(1,4);
	lossT2U4 = r1->calculateLoss(2,4);
	lossT3U4 = r1->calculateLoss(3,4);

	t1 = new PropCacheTestTransmitter(1, systemManager);
	t2 = new PropCacheTestTransmitter(2, systemManager);
	t3 = new PropCacheTestTransmitter(3, systemManager);
}

void PropCacheTest::tearDown()
{
	delete r1;
	delete t1;
	delete t2;
	delete t3;
	delete systemManager;
}

// Testing getLoss without updating calls by the receiver
// getLoss calls sorted by the TransmitterIds
void PropCacheTest::testGetLossInVectorCacheSorted()
{
	VectorCache* propCache = new VectorCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(loss.get_dB(), lossT1U1.get_dB(), 0.0001);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}

// Testing getLoss without updating calls by the receiver
// getLoss calls not sorted by the TransmitterIds
void PropCacheTest::testGetLossInVectorCacheUnsorted()
{
	VectorCache* propCache = new VectorCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(loss.get_dB(), lossT2U1.get_dB(), 0.0001);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}

void PropCacheTest::testGetLossInHashCacheSorted()
{
	HashCache* propCache = new HashCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}
void PropCacheTest::testGetLossInHashCacheUnsorted()
{
	HashCache* propCache = new HashCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}

void PropCacheTest::testGetLossInH2CacheSorted()
{

	H2Cache* propCache = new H2Cache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}

void PropCacheTest::testGetLossInH2CacheUnsorted()
{
	H2Cache* propCache = new H2Cache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}

void PropCacheTest::testGetLossInIdVectorCacheSorted()
{
	IdVectorCache* propCache = new IdVectorCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}
void PropCacheTest::testGetLossInIdVectorCacheUnsorted()
{
	IdVectorCache* propCache = new IdVectorCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}


void PropCacheTest::testGetLossInNoCacheSorted()
{
	NoCache* propCache = new NoCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}
void PropCacheTest::testGetLossInNoCacheUnsorted()
{
	NoCache* propCache = new NoCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U1, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);
}

void PropCacheTest::testUpdateWholeCacheInVectorCache()
{

	VectorCache* propCache = new VectorCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);


	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->positionChanged();


	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->positionChanged();

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U4, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U4, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateWholeCacheInIdVectorCache()
{
	IdVectorCache* propCache = new IdVectorCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->positionChanged();

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->positionChanged();

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U4, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U4, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}


void PropCacheTest::testUpdateWholeCacheInHashCache()
{
	HashCache* propCache = new HashCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->positionChanged();


	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->positionChanged();

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U4, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U4, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateWholeCacheInH2Cache()
{
	H2Cache* propCache = new H2Cache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->positionChanged();


	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->positionChanged();

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U4, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U4, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

}

void PropCacheTest::testUpdateWholeCacheInNoCache()
{
	NoCache* propCache = new NoCache(r1);
	r1->setPropCache(propCache);

	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->positionChanged();


	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	r1->setNumUpdates(3);
	r1->positionChanged();

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U4, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U4, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateCacheForOneTransmitterInIdVectorCache()
{

	IdVectorCache* propCache = new IdVectorCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);


	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->mobilityUpdate(t2);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT(lossT3U1 != loss);

	r1->setNumUpdates(4);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

}

void PropCacheTest::testUpdateCacheForOneTransmitterInVectorCache()
{

	VectorCache* propCache = new VectorCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);


	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->mobilityUpdate(t2);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT(lossT3U1 != loss);

	r1->setNumUpdates(4);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateCacheForOneTransmitterInHashCache()
{

	HashCache* propCache = new HashCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);


	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->mobilityUpdate(t2);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT(lossT3U1 != loss);

	r1->setNumUpdates(4);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateCacheForOneTransmitterInH2Cache()
{

	H2Cache* propCache = new H2Cache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);

	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->mobilityUpdate(t2);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT(lossT3U1 != loss);

	r1->setNumUpdates(4);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateCacheForOneTransmitterInNoCache()
{
	NoCache* propCache = new NoCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);


	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);
	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);

	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->mobilityUpdate(t2);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(1);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U1, loss);

	r1->setNumUpdates(4);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(4);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
}

void PropCacheTest::testUpdateCacheBothWaysInIdVectorCache()
{
	IdVectorCache* propCache = new IdVectorCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	r1->positionChanged(),
		loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);


	r1->setNumUpdates(3);
	r1->positionChanged();
	r1->mobilityUpdate(t2);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->positionChanged();
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	//lazy cache, updates entry when getLoss is called
	r1->setNumUpdates(2);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);
}
void PropCacheTest::testUpdateCacheBothWaysInVectorCache()
{
	VectorCache* propCache = new VectorCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	r1->positionChanged(),
		loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);


	r1->setNumUpdates(3);
	r1->positionChanged();
	r1->mobilityUpdate(t2);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->positionChanged();
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	//no lazy cache; updates cache when mobilityUpdate is called
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U3, loss);
}
void PropCacheTest::testUpdateCacheBothWaysInHashCache()
{
	HashCache* propCache = new HashCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	r1->positionChanged(),
		loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);


	r1->setNumUpdates(3);
	r1->positionChanged();
	r1->mobilityUpdate(t2);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->positionChanged();
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	//lazy cache, updates entry when getLoss is called
	r1->setNumUpdates(2);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);
}
void PropCacheTest::testUpdateCacheBothWaysInH2Cache()
{
	H2Cache* propCache = new H2Cache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	r1->positionChanged(),
		loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);


	r1->setNumUpdates(3);
	r1->positionChanged();
	r1->mobilityUpdate(t2);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->positionChanged();
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	//lazy cache, updates entry when getLoss is called
	r1->setNumUpdates(2);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);
}
void PropCacheTest::testUpdateCacheBothWaysInNoCache()
{
	NoCache* propCache = new NoCache(r1);
	r1->setPropCache(propCache);
	wns::Ratio loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U1, loss);

	r1->setNumUpdates(2);
	r1->mobilityUpdate(t1);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U2, loss);

	r1->positionChanged(),
		loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);

	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U2, loss);


	r1->setNumUpdates(3);
	r1->positionChanged();
	r1->mobilityUpdate(t2);
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U3, loss);

	r1->setNumUpdates(4);
	r1->positionChanged();
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);
	r1->mobilityUpdate(t3);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U4, loss);

	r1->setNumUpdates(3);
	r1->mobilityUpdate(t1);
	r1->positionChanged();
	loss = propCache->getLoss(t1, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT1U3, loss);
	loss = propCache->getLoss(t2, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT2U3, loss);

	r1->setNumUpdates(2);
	loss = propCache->getLoss(t3, frequency);
	CPPUNIT_ASSERT_EQUAL(lossT3U2, loss);
}
