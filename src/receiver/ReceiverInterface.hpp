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

#ifndef __RECEIVERINTERFACE_HPP
#define __RECEIVERINTERFACE_HPP

#include <RISE/misc/pointer.hpp>
#include <RISE/receiver/SignalAveragingStrategy.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/receiver/LossCalculation.hpp>
#include <RISE/medium/PhysicalResourceObserver.hpp>

#include <WNS/PowerRatio.hpp>
//#include <WNS/PositionObserver.hpp>

namespace rise {
	class Station;
}

namespace rise { namespace receiver {
	/**
	 * @brief Base of every Receiver
	 */
	class ReceiverInterface :
		public wns::Observer<wns::PositionObserver>,
		virtual public PhysicalResourceObserver,
		virtual public TransmissionAveragingStrategy,
		virtual public SignalCalculationInterface,
		virtual public InterferenceCalculationInterface,
		virtual protected LossCalculationInterface
	{
	public:
		virtual ~ReceiverInterface()
		{}

		// FIXME: for PropagationCache - will be removed (msg)
		virtual void writeCacheEntry(PropCacheEntry& cacheEntry, Transmitter* t, double freq) = 0;
	protected:
		virtual Station* getStation() const = 0;

		bool transmissionForMe(const TransmissionObjectPtr& t) const
		{
			return t->isForMe(this);
		}
	};
}}

#endif // NOT defined RECEIVERINTERFACE_HPP




