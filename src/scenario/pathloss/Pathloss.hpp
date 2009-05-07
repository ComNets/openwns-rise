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

#ifndef _RISE_SCENARIO_PATHLOSS_PATHLOSS_HPP
#define _RISE_SCENARIO_PATHLOSS_PATHLOSS_HPP

#include <RISE/antenna/Antenna.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/Types.hpp>
#include <WNS/Functor.hpp>

namespace rise { namespace scenario { namespace pathloss {

      static const wns::Ratio OutOfRangePathloss = wns::Ratio::from_dB(500);

    /**
     * @brief Base class for all pathloss models.
     *
     * Pathloss models are expected to either derive from
     * DistanceDependent or DistanceIndependent.
     */
    class Pathloss
    {
	friend class RangeChecked;
    public:

	/**
	 * @brief Interface for return value transformations.
	 *
	 * Classes deriving from pathloss must provide a return
	 * value transformation. The value calculated by the
	 * pathloss model can be further transformed. E.g. a minimal
	 * pathloss can be guaranteed, but return value transformations
	 * are of course not limited to such simple implementations.
	 */
	typedef wns::ArgumentFunctor<const wns::Ratio&, wns::Ratio> ReturnValueTransformation;

	/**
	 * @brief Return the pathloss between source and target at frequency.
	 */
	wns::Ratio getPathloss(const antenna::Antenna& source,
			       const antenna::Antenna& target,
			       const wns::Frequency& frequency) const;

	virtual ~Pathloss();

    protected:
		Pathloss(const ReturnValueTransformation* rvt);

	/**
	 * @brief Pathloss calculation function for models not dependent on the
	 * distance of source and target.
	 *
	 * For models dependent on the distance DistanceDepedent will provide
	 * an implementation of this method.
	 */
	virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
					     const antenna::Antenna& target,
					     const wns::Frequency& frequency) const = 0;

	/**
	 * @brief Pathloss calculation function for models dependent on the
	 * distance of source and target.
	 *
	 * For models not dependent on the distance DistanceIndependent will provide
	 * an implementation of this method.
	 */
	virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
					     const antenna::Antenna& target,
					     const wns::Frequency& frequency,
					     const wns::Distance& distance) const = 0;

    private:
	const ReturnValueTransformation* transform;
    };

}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_PATHLOSS_HPP
