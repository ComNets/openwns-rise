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

#ifndef __PROPCACHEENTRY_HPP
#define __PROPCACHEENTRY_HPP

#include <WNS/PowerRatio.hpp>

namespace rise{
    /**
		@brief PropCacheEntry is an object that stores the values of pathloss, 
		shadowing and antennaGain to compute the total loss
	*/
	class PropCacheEntry{

	public:
		//! Default Constructor
		PropCacheEntry();
		//! Defeult Destructor
		~PropCacheEntry();
		//! Returns the total loss of this PSG
		wns::Ratio getTotalLoss() const;
		//! Returns the strored value for the pathloss
		wns::Ratio getPathloss() const;
		//! Returns the stored value for the shadowing
		wns::Ratio getShadowing() const;
		//! Returns the stored value for the antennaGain
		wns::Ratio getAntennaGain() const;
		//! Returns true if stored values are valid
		bool  isValid() const;

		//! Stores the given value for pathloss
		void setPathloss(wns::Ratio loss);
		//! Stores the given value for shadowing
		void setShadowing(wns::Ratio shadow);
		//! Stores the given value for antennaGain
		void setAntennaGain(wns::Ratio gain);
		//! Set true for guilty values and false to set the object dirty
		void setValid(bool valid);
	private:
		//! Computes total loss from the currently stored values
		void recomputeTotalLoss() {
			totalLoss = pathloss;
			totalLoss += shadowing;
			totalLoss -= antennaGain;
		}
		wns::Ratio totalLoss;
		wns::Ratio pathloss;
		wns::Ratio shadowing;
		wns::Ratio antennaGain;
		bool valid;
	};
}
#endif


