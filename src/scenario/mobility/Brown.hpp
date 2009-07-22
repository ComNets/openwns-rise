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

#ifndef RISE_SCENARIO_MOBILITY_BROWN_HPP
#define RISE_SCENARIO_MOBILITY_BROWN_HPP

#include <RISE/scenario/mobility/Mobility.hpp>
#include <WNS/geometry/AxisParallelRectangle.hpp>

#include <WNS/distribution/Uniform.hpp>

namespace wns {	namespace distribution {
	class Distribution;
} // distribution
} // wns

namespace rise { namespace scenario { namespace mobility {

	/**
	 * @brief Mobility Model according to Brownian Molecular Movement.
	 *
	 * The user moves with a randomly determined speed with a provided mean
	 * and variance. The user is only allowed to move inside a certain area
	 * determined the abstract checkBounds method. Overload this in your
	 * Brownian Implementation
	 */
	class BrownianBase :
		public Mobility
	{
	public:
		/**
		 * @brief pyconfig::Parser Constructor
		 */
		explicit
		BrownianBase(const wns::pyconfig::View& mobilityView);

		/** @brief Destructor */
		virtual
		~BrownianBase();

		/**
		 * @brief This method Implements the MobilityInterface
		 *
		 * It performs a location update of the mobile user depending on
		 * his previous coordinates and his previous speed.
		 */
		virtual void
		move();

		/**
		 * @brief Implement PeriodicTimeout
		 */
		virtual void
		periodically();

	protected:
		/**
		 * @brief This method draws a new velocity vector
		 */
		void
		newVelocityVector();

		/**
		 * @brief checks whether the given position is inside the
		 * allowed bounds
		 */
		virtual bool
		checkBounds(const wns::Position& pos) const = 0;

		/**
		 * @brief Normal distribution of velocity among mobile users
		 */
		wns::distribution::Distribution* velocityDistribution;

		/**
		 * @brief Uniform distribution of angles used for location
		 * updates
		 */
		wns::distribution::Uniform* angleDistribution;

		/**
		 * @brief Uniform distributin for update probabilities
		 */
		wns::distribution::StandardUniform* updateProbability;

		/**
		 * @brief The velocityVector is intended to contain the product
		 * of the moveTimeStep and the directed velocity. i.e., if added
		 * to the position you immediately obtain the new position.
		 */
		wns::PositionOffset velocityVector;

		/** @brief helper method to load the list of obstructions
		 * (i.e. no-go areas) 
		 */
		void
		fetchObstructionList(const wns::pyconfig::View& config);

		/** @brief list of no-go areas */
		std::list<wns::geometry::AxisParallelRectangle> obstructionList;

		/** @brief check whether point is inside one of the obstructions */
		virtual bool
		isObstructed(const wns::Position& pos) const;
	private:
		/** @brief helper function to retrieve the RNG */
		void
		initRNGs();
	};

	/**
	 * @brief Mobility Model according to Brownian Molecular Movement inside
	 * circular bounds
	 *
	 * The user moves with a randomly determined speed with a provided mean
	 * and variance. The user is only allowed to move inside a certain
	 * rectangular area determined by xmin,ymin and xmax,ymax. You may set
	 * these to be the scenario borders or something else.
	 */
	class BrownianRect :
		public BrownianBase
	{
	public:
		explicit
		BrownianRect(const wns::pyconfig::View& mobilityView);

	protected:
		virtual bool
		checkBounds(const wns::Position& pos) const;

	private:
		/**
		 * @brief store the rectangular bounds
		 */
		double xmin, ymin, xmax, ymax;
	};

	/**
	 * @brief Mobility Model according to Brownian Molecular Movement inside
	 * rectengular bounds.
	 *
	 * The user moves with a randomly determined speed with a provided mean
	 * and variance. The user is only allowed to move inside a certain
	 * circular area determined by a center Position and a maximum radius.
	 * You may set these at your leisure.
	 */
	class BrownianCirc :
		public BrownianBase
	{
	public:
		explicit
		BrownianCirc(const wns::pyconfig::View& mobilityView);

	protected:
		virtual bool
		checkBounds(const wns::Position& pos) const;

	private:
		/**
		 * @brief Center of the circular area
		 */
		wns::Position center;

		/**
		 * @brief Maximum distance from center
		 */
		double maxDistance;
		
		/**
		 * @brief Number of sectors
		 */
		int nSectors;
	}; // BrownianCirc

	/** @brief Mobility Model according to Brownian Molecular Movement inside
	 * a hexagonal area.*/
	class BrownianEquiangularPolygon :
		public BrownianBase
	{
	public:
		explicit
		BrownianEquiangularPolygon(const wns::pyconfig::View& mobilityView);

	protected:
		virtual bool
		checkBounds(const wns::Position& pos) const;

	private:
		/**
		 * @brief Center of the area
		 */
		wns::Position center;

		/**
		 * @brief Maximum distance from center
		 */
		double maxDistance;

		/**
		 * @brief Number of polygon corners
		 */
		int corners;

		/**
		 * @brief Angle to rotate polygon
		 */
		double angle;
	}; // BrownianEquiangularPolygon

} // mobility
} // scenario
} // rise

#endif  // RISE_SCENARIO_MOBILITY_BROWN_HPP


