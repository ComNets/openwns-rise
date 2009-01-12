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

#ifndef RISE_SCENARIO_OBSTRUCTION_HPP
#define RISE_SCENARIO_OBSTRUCTION_HPP

#include <WNS/geometry/LineSegment.hpp>
#include <WNS/SmartPtr.hpp>
#include <WNS/PowerRatio.hpp>

#include <list>

namespace rise { namespace scenario {

	class Obstruction :
		virtual public wns::RefCountable
	{
	public:
		virtual
		~Obstruction()
		{};

		const wns::Ratio&
		getAttenuationFactor() const
		{
			return attenuation;
		}

		void
		setAttenuationFactor(const wns::Ratio& _attenuation)
		{
			this->attenuation = _attenuation;
		}

		virtual wns::Ratio
		getAttenuation(const wns::geometry::LineSegment& signalPath) const = 0;

	protected:
		explicit
		Obstruction(const wns::Ratio& attenuation) :
			attenuation(attenuation)
		{}

		wns::Ratio attenuation;
	};

	typedef std::list< wns::SmartPtr<Obstruction> > ObstructionList;

	template<class Shape>
	class Obstructing :
		public Obstruction,
		public Shape
	{
	public:
		Obstructing(const Shape& shape, const wns::Ratio& attenuation) :
			Obstruction(attenuation),
			Shape(shape)
		{}

		virtual wns::Ratio
		getAttenuation(const wns::geometry::LineSegment& signalPath) const
		{
			return wns::Ratio::from_dB(
				Shape::countBorderIntersections(signalPath) * attenuation.get_dB());

		}
	};

} // scenario
} // rise

#endif // NOT defined RISE_SCENARIO_OBSTRUCTION_HPP
