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

#ifndef __RECEIVERBF_HPP
#define __RECEIVERBF_HPP

#include <RISE/transceiver/receiver.hpp>
#include <RISE/antenna/Beamforming.hpp>

namespace rise
{
	class ReceiverBF
		: public ReceiverBase
	{
		/**
		 * @brief Constructor
		 */
		ReceiverBF(Station* s,
			   antenna::Beamforming* a,
			   Demodulator *demodulator,
			   Decoder *decoder,
			   wns::Ratio rnf);

		/**
		 * @brief Destructor
		 */
		virtual ~ReceiverBF();

		//! Returns the interference for the TransmissionObject t.
		/** The interference is here defined as: the total received signal power
		    - the carrier, if using the beam for that TransmissionObject
		    (getTotalRxPower(t)-getRxPower(t, t)) */
		virtual wns::Power getInterference(const TransmissionObjectPtr& t);

		//! Returns the received signal power of the TransmissionObject t,
		//  when using the beam for the other TransmissionObject
		virtual wns::Power getRxPower(const TransmissionObjectPtr& t,
					      const TransmissionObjectPtr& whenListeningToTO);

		//! Returns the total received signal power of the PhysicalResource the
		//  Receiver is listening to, using the receive beam for
		//  the given TransmissionObject
		/** NOTE: This is a very simple implementation. It assumes your receiver
		    is working on the whole bandwidth of the PhysicalResource. If you
		    need something sophisticated please go ahead and implement something
		    on your own (that's why the method is virtual).*/
		virtual wns::Power getTotalRxPower(const TransmissionObjectPtr& whenListeningToTO);
		virtual void writeCacheEntry(PropCacheEntry& cacheEntry,
					     Transmitter* t,
					     double freq);
		virtual antenna::Beamforming* getAntenna() const
		{
			return antenna;
		}

		// from BFManager
		virtual std::map<rise::Station*, wns::Ratio> calculateSINRsRx(const std::vector<Station*>& stations,
														  wns::Power& IinterPlusNoise)
		{
			return antenna->calculateSINRsRx(stations, IinterPlusNoise);
		}

		// from BFManager
		virtual std::map<rise::Station*, wns::Ratio> calculateSINRsTx(std::map<Station*, wns::Power>& Station2NoisePlusIintercell,
														 wns::Power& x_safe,
														 wns::Power txPower)
			{
			return antenna->calculateSINRsTx(Station2NoisePlusIintercell, x_safe, txPower);
		}

		// from BFManager
		virtual antenna::PatternId calculateAndSetBeam(Station* id,
							       const std::vector<Station*>& undesired,
							       wns::Power& IinterPlusNoise)
		{
			return antenna->calculateAndSetBeam(id, undesired, IinterPlusNoise);
		}
		// from BFManager
		virtual void removeBeam(antenna::PatternId id)
		{
			antenna->removeBeam(id);
		}
		// from BFManager
		virtual void setPowerReceivedForStation(Station* const id,
						wns::Power power)
		{
			antenna->setPowerReceivedForStation(id, power);
		}
		// from BFManager
		virtual void startReceiving(TransmissionObjectPtr& TO,
					    antenna::PatternId patternId)
		{
			antenna->startReceiving(TO, patternId);
		}
		// from BFManager
		virtual void stopReceiving(TransmissionObjectPtr& TO)
		{
			antenna->stopReceiving(TO);
		}

	protected:
		antenna::Beamforming* antenna;
	};
}
#endif
