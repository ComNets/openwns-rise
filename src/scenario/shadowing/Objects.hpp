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

#ifndef RISE_SCENARIO_SHADOWING_OBJECTS_HPP
#define RISE_SCENARIO_SHADOWING_OBJECTS_HPP

#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/Obstruction.hpp>
#include <RISE/scenario/Scenario.hpp>

#include <WNS/geometry/AxisParallelRectangle.hpp>
#include <WNS/geometry/LineSegment.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/container/Matrix.hpp>
#include <WNS/SmartPtr.hpp>

#include <list>

namespace rise { namespace scenario { namespace shadowing {

	/**
	 * @brief Obstruction based shadowing model.
	 */
	class Objects :
		public Shadowing
	{
	public:
		explicit
		Objects(const wns::pyconfig::View& config);

		virtual
		~Objects()
		{};

		virtual wns::Ratio
		getShadowing(const antenna::Antenna& source,
			     const antenna::Antenna& target) const;


	private:
		typedef wns::container::Matrix<ObstructionList, 2> BlockObstructionLists;
		typedef Obstructing<wns::geometry::AxisParallelRectangle> Object;
		typedef wns::SmartPtr<Object> ObjectPtr;

		static ObstructionList
		fetchObstructionList(const wns::pyconfig::View& config);

		static ObstructionList
		fetchObstructionList(Scenario& scenario,
				     const wns::pyconfig::View& config);

		void
		createBlockObstructionLists();

		Scenario* scenario;

		const unsigned int xGridBlocks;
		const unsigned int yGridBlocks;
		const double xGridBlockSize;
		const double yGridBlockSize;
		ObstructionList obstructionList;
		BlockObstructionLists blockObstructionLists;
	};

} // shadowing
} // scenario
} // rise

#endif // RISE_SHADOWING_SHADOWING_OBJECTS_HPP
