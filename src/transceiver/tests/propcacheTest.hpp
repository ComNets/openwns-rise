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

#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transceiver/tests/ReceiverDropIn.hpp>
#include <RISE/transceiver/tests/TransmitterDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/pyconfig/Parser.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

#ifndef RISE_TESTS_PROPCACHETESTTRANSMITTER_HPP
#define RISE_TESTS_PROPCACHETESTTRANSMITTER_HPP

namespace rise { namespace tests {
	class PropCacheTestTransmitter :
		public TransmitterDropIn
	{
	public:
		PropCacheTestTransmitter(unsigned long int index, SystemManagerDropIn* systemManager) :
			TransmitterDropIn(new StationDropIn(systemManager),NULL),
			tId(index)
		{}

		~PropCacheTestTransmitter()
		{
			delete getStation();
		}

		long int getTransmitterId() const
		{
			return tId;
		}
	private:
		unsigned long int tId;
	};

	class PropCacheTestReceiver :
		public ReceiverDropIn
	{
	public:
		PropCacheTestReceiver(SystemManagerDropIn* systemManager) :
			ReceiverDropIn(new StationDropIn(systemManager), NULL),
			numUpdates(1)
		{}

		virtual ~PropCacheTestReceiver()
		{
		    delete getStation();
		}

		/* Calculates a fake loss which is the Ratio of index*numUpdates
		   to compare it with the cacheEntry of the Transmitter which has
		   this index and at the given update level.
		*/
		wns::Ratio calculateLoss(unsigned long int index, unsigned long int numUp)
		{
			int p = index*numUp;
			int s = index*numUp;
			int g = index*numUp;

			wns::Ratio pathloss = wns::Ratio::from_factor(p);
			wns::Ratio shadowing = wns::Ratio::from_factor(s);
			wns::Ratio antennaGain = wns::Ratio::from_factor(g);
			wns::Ratio currLoss =  pathloss + shadowing - antennaGain;
			return currLoss;
		}

		void setNumUpdates(unsigned long int num)
		{
			numUpdates = num;
		}

		unsigned long int getNumUpdates()
		{
			return numUpdates;
		}

		void setPropCache(PropagationCache* _propCache)
		{
			propCache = _propCache;
		}

		// ReceiverInterface
		virtual void notify(const TransmissionObjectPtr&)
		{}

	protected:
		/* Writes fake values for pathloss, shadowing and antennaGain into the
		   cache. The loss computes in class PropCacheEntry is equal the Ratio
		   of TransmitterId * numUpdates
		*/
		void writeCacheEntry(PropCacheEntry& cacheEntry,
							 Transmitter* t,
							 double)
		{
			unsigned long int tId = t->getTransmitterId();
			unsigned long int pathloss = tId * numUpdates;
			unsigned long int shadowing = tId * numUpdates;
			unsigned long int antennaGain = tId * numUpdates;
			cacheEntry.setPathloss(wns::Ratio::from_factor(pathloss));
			cacheEntry.setShadowing(wns::Ratio::from_factor(shadowing));
			cacheEntry.setAntennaGain(wns::Ratio::from_factor(antennaGain));
			cacheEntry.setValid(true);
		}
		unsigned long int numUpdates;
	}; // class PropCacheTestReceiver

	class PropCacheTest
		: public CppUnit::TestFixture  {
		CPPUNIT_TEST_SUITE( PropCacheTest );
		CPPUNIT_TEST( testGetLossInVectorCacheSorted );
		CPPUNIT_TEST( testGetLossInVectorCacheUnsorted );
		CPPUNIT_TEST( testGetLossInHashCacheSorted );
		CPPUNIT_TEST( testGetLossInHashCacheUnsorted );
		CPPUNIT_TEST( testGetLossInH2CacheSorted );
		CPPUNIT_TEST( testGetLossInH2CacheUnsorted );
		CPPUNIT_TEST( testGetLossInIdVectorCacheSorted );
		CPPUNIT_TEST( testGetLossInIdVectorCacheUnsorted );
		CPPUNIT_TEST( testGetLossInNoCacheSorted );
		CPPUNIT_TEST( testGetLossInNoCacheUnsorted );
		CPPUNIT_TEST( testUpdateWholeCacheInVectorCache );
		CPPUNIT_TEST( testUpdateWholeCacheInIdVectorCache );
		CPPUNIT_TEST( testUpdateWholeCacheInHashCache );
		CPPUNIT_TEST( testUpdateWholeCacheInH2Cache );
		CPPUNIT_TEST( testUpdateWholeCacheInNoCache );
		CPPUNIT_TEST( testUpdateCacheForOneTransmitterInNoCache );
		CPPUNIT_TEST( testUpdateCacheForOneTransmitterInIdVectorCache );
		CPPUNIT_TEST( testUpdateCacheForOneTransmitterInVectorCache );
		CPPUNIT_TEST( testUpdateCacheForOneTransmitterInHashCache );
		CPPUNIT_TEST( testUpdateCacheForOneTransmitterInH2Cache );
		CPPUNIT_TEST( testUpdateCacheBothWaysInNoCache );
		CPPUNIT_TEST( testUpdateCacheBothWaysInIdVectorCache );
		CPPUNIT_TEST( testUpdateCacheBothWaysInVectorCache );
		CPPUNIT_TEST( testUpdateCacheBothWaysInHashCache );
		CPPUNIT_TEST( testUpdateCacheBothWaysInH2Cache );
		CPPUNIT_TEST_SUITE_END();
    public:
		void setUp();
		void tearDown();

        // Testing getLoss without updating calls by the receiver
        //  getLoss calls sorted by the TransmitterIds
		void testGetLossInVectorCacheSorted();
		void testGetLossInHashCacheSorted();
		void testGetLossInH2CacheSorted();
		void testGetLossInIdVectorCacheSorted();
		void testGetLossInNoCacheSorted();
        // Testing getLoss without updating calls by the receiver
        // getLoss calls sorted by the TransmitterIds
		void testGetLossInVectorCacheUnsorted();
		void testGetLossInHashCacheUnsorted();
		void testGetLossInH2CacheUnsorted();
		void testGetLossInIdVectorCacheUnsorted();
		void testGetLossInNoCacheUnsorted();
        // Testing cache entries for updating when receiver 'moved'
		void testUpdateWholeCacheInIdVectorCache();
		void testUpdateWholeCacheInVectorCache();
		void testUpdateWholeCacheInHashCache();
		void testUpdateWholeCacheInH2Cache();
		void testUpdateWholeCacheInNoCache();
		// Testing cache entries for updating when transmitter 'moved'
		void testUpdateCacheForOneTransmitterInIdVectorCache();
		void testUpdateCacheForOneTransmitterInVectorCache();
		void testUpdateCacheForOneTransmitterInHashCache();
		void testUpdateCacheForOneTransmitterInH2Cache();
		void testUpdateCacheForOneTransmitterInNoCache();
		//Testing cache entries for updating when receiver and transmitter moved
		void testUpdateCacheBothWaysInIdVectorCache();
		void testUpdateCacheBothWaysInVectorCache();
		void testUpdateCacheBothWaysInHashCache();
		void testUpdateCacheBothWaysInH2Cache();
		void testUpdateCacheBothWaysInNoCache();

    private:
		SystemManagerDropIn* systemManager;
		PropCacheTestReceiver* r1;
		double frequency;
		PropCacheTestTransmitter* t1;
		PropCacheTestTransmitter* t2;
		PropCacheTestTransmitter* t3;

		/* Values to compare with the cache entries for a Transmitter TX and an
		   update level UX
		*/

		wns::Ratio lossT1U1;
		wns::Ratio lossT2U1;
		wns::Ratio lossT3U1;

		wns::Ratio lossT1U2;
		wns::Ratio lossT2U2;
		wns::Ratio lossT3U2;

		wns::Ratio lossT1U3;
		wns::Ratio lossT2U3;
		wns::Ratio lossT3U3;

		wns::Ratio lossT1U4;
		wns::Ratio lossT2U4;
		wns::Ratio lossT3U4;

	}; // class PropCacheTest
} // namespace tests
} // namespace rise

#endif // RISE_TESTS_PROPCACHETESTTRANSMITTER_HPP


