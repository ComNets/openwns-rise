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

#ifndef __LOSSCALCULATION_HPP
#define __LOSSCALCULATION_HPP

#include <RISE/receiver/LossCalculationInterface.hpp>
#include <RISE/scenario/PropagationCharacteristic.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/Types.hpp>
#include <WNS/pyconfig/View.hpp>

namespace rise {
    class Transmitter;
}

namespace rise { namespace receiver {

    class LossCalculation :
	virtual public LossCalculationInterface,
	public scenario::PropagationCharacteristic
    {
    public:
	LossCalculation(const wns::pyconfig::View& config);

	virtual ~LossCalculation();

	virtual wns::Ratio getPathloss(const Transmitter& t, 
        const wns::Frequency& frequency) const;

	virtual wns::Ratio getShadowing(const Transmitter& t) const;

	virtual wns::Ratio getFastFading(const Transmitter& t, 
        const wns::Frequency& frequency, const Transmitter& tsig) const;

    virtual void
    initPropagation();

    private:
	const scenario::Propagation& propagation;
    };

}}

#endif // NOT defined __LOSSCALCULATION_HPP
