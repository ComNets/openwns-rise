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

#ifndef RISE_ANTENNA_ANTENNA_HPP
#define RISE_ANTENNA_ANTENNA_HPP

#include <RISE/misc/RISELogger.hpp>
#include <WNS/SmartPtr.hpp>
#include <WNS/Position.hpp>
#include <WNS/PositionObserver.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/service/phy/ofdma/Pattern.hpp>
#include <WNS/Observer.hpp>
#include <set>

namespace rise {
	class Station;
}

namespace rise { namespace antenna {

	class Beamforming;

 	/**
	 * @brief Class to hold the beampattern calculated by the Beamforming
	 * Antenna. The class is RefCountable to allow the PatternPtr typedef.
	 *
	 */
	typedef wns::service::phy::ofdma::PatternPtr PatternPtr;
	typedef wns::service::phy::ofdma::Pattern Pattern;


	/**
	 * @brief Base class of all antennas
	 *
	 */
	class Antenna :
		public wns::Observer<wns::PositionObserver>
	{
	public:

		/**
		 * @brief Antenna class for base and mobile stations.
		 *
		 * Omnidirectional and directional antenna patterns with different
		 * beamwidth are provided internally. There is also support for reading
		 * antenna patterns from files in 3d and planet format.
		 */
		Antenna();

		/**
		 * @brief Constructor setting characteristicType, position, and rotation
		 * axis.
		 */
 		Antenna(const wns::pyconfig::View& pyConfigView,
 				Station* const station);

		virtual ~Antenna();

		virtual wns::Ratio getGain(const wns::Position& pos,
								   const PatternPtr pattern) const = 0;

		// inlined for performance
		const wns::Position& getPosition() const
		{
				return position;
		}

		Station* getStation() const;

		void positionWillChange()
		{};

		void positionChanged();

	protected:

		/**
		 * @brief Local Logger
		 */
		RISELogger log;

		double radius;

		/** @brief flag that stores whether to draw antenna pattern */
		bool drawPattern;

		/** @brief method that writes the pattern to a file */
		virtual void drawRadiationPattern() const;

		/**
		 * @brief Pattern files are written here
		 */
		std::string patternOutputFile;
	private:

		/**
		 * @brief Truncate/create the pattern files
		 */
		void preparePatternOutputFile();

		/**
		 * @brief The Station the Antenna is mounted to
		 */
		Station* pd_station;

		/**
		 * @brief The offset from the base point of the Station (pd_station)
		 */
		wns::PositionOffset pd_positionOffset;

		/**
		 * @brief The Position current position of the antenna
		 *
		 * recomputed when the station moves
		 */
		wns::Position position;

		/**
		 * @brief Antenna ID of this antenna
		 */
		unsigned long int id;

		/**
		 * @brief Counter of antenna ids
		 */
		static unsigned long int counter;

		static std::set<std::string> patternOutputFilePrepared;
	};
}
}
#endif  // _ANTENNA_HPP


