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

#include <RISE/receiver/LossCalculation.hpp>
#include <RISE/scenario/Propagation.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/transceiver/transmitter.hpp>

#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/Singleton.hpp>

using namespace rise::receiver;

LossCalculation::LossCalculation(const wns::pyconfig::View& config)
    : PropagationCharacteristic(config),
      propagation(*(rise::scenario::GlobalPropagationBroker::getInstance()->procure(config.get<wns::pyconfig::View>("propagation"))))
{
}

LossCalculation::~LossCalculation()
{
}

wns::Ratio LossCalculation::getPathloss(const Transmitter& t, const wns::Frequency& frequency) const
{
    const scenario::pathloss::Pathloss& pathloss = propagation.getPathlossModel(t.getPropagationCharacteristicId(), getPropagationCharacteristicId());
    return pathloss.getPathloss(*(t.getAntenna()), *(getStation()->getAntenna()), frequency);
}

wns::Ratio LossCalculation::getShadowing(const Transmitter& t) const
{
    const scenario::shadowing::Shadowing& shadowing = propagation.getShadowingModel(t.getPropagationCharacteristicId(), getPropagationCharacteristicId());
    return shadowing.getShadowing(*(t.getAntenna()), *(getStation()->getAntenna()));
}

wns::Ratio LossCalculation::getFastFading(const Transmitter& t, const wns::Frequency& frequency, const Transmitter& tsig) const
{
    const scenario::fastfading::FastFading& fastFading = propagation.getFastFadingModel(t.getPropagationCharacteristicId(), getPropagationCharacteristicId());
    return fastFading.getFastFading(*(t.getAntenna()), *(getStation()->getAntenna()), frequency, *(tsig.getAntenna()));
}

void
LossCalculation::initPropagation()
{
    const_cast<rise::scenario::Propagation&>(propagation).onWorldCreated();
}
