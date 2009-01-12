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

#ifndef _RISE_SCENARIO_PATHLOSS_ONESLOPE_HPP
#define _RISE_SCENARIO_PATHLOSS_ONESLOPE_HPP

#include <RISE/stations/base/base.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/scenario/pathloss/RangeChecked.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/Enumerator.hpp>
#include <WNS/isClass.hpp>

namespace rise { namespace scenario { namespace pathloss {

    /**
     * @brief Frequency dependent attenuation term, that ignores base height.
     */
    class NoHeightFreqAttFormula
    {
    public:
		static const bool needsHeight = false;

		NoHeightFreqAttFormula(const double& freqFactor,
							   const double&)
			: freqFactor(freqFactor)
			{}

		inline wns::Ratio freqAtt(const wns::Frequency& frequency,
								  const wns::Distance&) const
			{
				return wns::Ratio::from_dB(freqFactor * std::log10(frequency));
			}

    private:
		const double freqFactor;
    };

    /**
     * @brief Frequency dependent attenuation term taking the base height in account.
     */
    class DefaultFreqAttFormula
    {
    public:
		static const bool needsHeight = true;

		DefaultFreqAttFormula(const double& freqFactor,
							  const double& heightFactor)
			: freqFactor(freqFactor),
			  heightFactor(heightFactor)
			{}

		inline wns::Ratio freqAtt(const wns::Frequency& frequency,
								  wns::Distance baseHeight) const
			{
				if (baseHeight < 1) baseHeight = 1;;
				return wns::Ratio::from_dB(freqFactor * std::log10(frequency) -
										   heightFactor * std::log10(baseHeight));
			}

    private:
		const double freqFactor;
		const double heightFactor;
    };

    /**
     * @brief Distance dependent attenuation term, that ignores base height.
     */
    class NoHeightDistAttFormula
    {
    public:
		static const bool needsHeight = false;

		NoHeightDistAttFormula(const double& distFactor,
							   const double&)
			: distFactor(distFactor)
			{}

		inline wns::Ratio distAtt(wns::Distance distance,
								  const wns::Distance&) const
			{
				if (distance == 0) distance = ++wns::makeEnumerable(distance);
				return wns::Ratio::from_dB(distFactor * std::log10(distance));
			}

    private:
		const double distFactor;
    };

    /**
     * @brief Distance dependent attenuation term taking the base height in account.
     */
    class DefaultDistAttFormula
    {
    public:

		static const bool needsHeight = true;

		DefaultDistAttFormula(const double& distFactor,
							  const double& heightFactor)
			: distFactor(distFactor),
			  heightFactor(heightFactor)
			{}

		inline wns::Ratio distAtt(wns::Distance distance,
								  wns::Distance baseHeight) const
			{
				if (distance == 0) distance = ++wns::makeEnumerable(distance);
				if (baseHeight < 1) baseHeight = 1;;
				return wns::Ratio::from_dB((distFactor - heightFactor * std::log10(baseHeight))
										   * std::log10(distance));
			}

    private:
		const double distFactor;
		const double heightFactor;
    };

    /**
     * @brief Distance dependent attenuation term used by UMTS 30.03 models.
     */
    class Umts3003DistAttFormula
    {
    public:
		static const bool needsHeight = true;

		Umts3003DistAttFormula(const double& distFactor,
							   const double& heightFactor)
			: distFactor(distFactor),
			  heightFactor(heightFactor)
			{}

		inline wns::Ratio distAtt(wns::Distance distance,
								  wns::Distance baseHeight) const
			{
				if (distance == 0) distance = ++wns::makeEnumerable(distance);
				if (baseHeight < 1) baseHeight = 1;;
				return wns::Ratio::from_dB((distFactor - heightFactor * baseHeight)
										   * std::log10(distance));
			}
    private:
		const double distFactor;
		const double heightFactor;
    };

    /**
     * @brief One slope pathloss model.
     *
     * This model calculates the pathloss by using one FreqAttFormula and DistAttFormula.
     */
    template<class FreqAttFormula = DefaultFreqAttFormula,
			 class DistAttFormula = DefaultDistAttFormula>
    class OneSlope :
	public RangeChecked
    {
		friend class MultiSlope;
    protected:
		OneSlope(const wns::pyconfig::View& config,
				 const wns::Ratio& offset,
				 const double& freqFactor,
				 const double& distFactor,
				 const double& distNormFactor,
				 const double& heightFactor = 0,
				 const double& distXheightFactor = 0)
			: RangeChecked(config),
			  freqTerm(freqFactor, heightFactor),
			  distTerm(distFactor, distXheightFactor),
			  distNormFactor(distNormFactor),
			  offset(offset)
			{}


		virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
											 const antenna::Antenna& target,
											 const wns::Frequency& frequency,
											 const wns::Distance& distance) const
			{
				wns::Distance baseHeight = 0;
				if (FreqAttFormula::needsHeight || DistAttFormula::needsHeight)
					baseHeight = fetchBaseHeight(source, target);
				return offset + freqTerm.freqAtt(frequency, baseHeight)
					+ distTerm.distAtt(distance * distNormFactor, baseHeight);
			}

    private:
		FreqAttFormula freqTerm;
		DistAttFormula distTerm;
		double distNormFactor;
		wns::Ratio offset;

		static inline wns::Distance fetchBaseHeight(const antenna::Antenna& source,
													const antenna::Antenna& target)
			{
				bool sourceIsBase = wns::isClass<Base>(*(source.getStation()));
				const antenna::Antenna& baseAntenna(sourceIsBase ? source : target);
				return baseAntenna.getPosition().getZ();
			}
    };

}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_ONESLOPE_HPP
