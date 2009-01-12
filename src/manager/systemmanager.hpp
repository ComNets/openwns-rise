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

#ifndef RISE_MANAGER_SYSTEMMANAGER_HPP
#define RISE_MANAGER_SYSTEMMANAGER_HPP

#include <RISE/misc/RISELogger.hpp>
#include <RISE/stations/station.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/Position.hpp>

#include <string>
#include <vector>
#include <map>
#include <algorithm>

namespace rise {
	namespace scenario {
	    class Scenario;
	}
	class Mobile;
	class Station;
	/**
	 * @brief Manages the station, scenario, configuration of one specific
	 * system
	 *
	 * @ingroup MANAGER
	 *
	 * In order to support different systems this is only a base class and for
	 * the different systems we have derived class with extended functionality.
	 */
    class SystemManager {
	public:
		/**
		 * @brief A typedef for the container where the stations are stored
		 *
		 * @note Always use this typedef either inside this class or
		 * outside. Then it's easier to change the type of the container (if we
		 * need to do so)
		 */
		typedef std::vector<Station*> StationContainer;
		/**
		 * @brief A typedef for the iterator to the container where the stations
		 * are stored
		 *
		 * @note Always use this typedef either inside this class or
		 * outside. Then it's easier to change the type of the container (if we
		 * need to do so)
		 */
		typedef StationContainer::iterator StationIterator;

		/**
		 * @brief Constructor
		 *
		 * @param[in] aSystemName the name of the system (UMTS, TETRA, ...)
		 *
		 * @note Please consider using a system specific name for your
		 * configuration file!
		 */
		SystemManager(const std::string &aSystemName,
					  const wns::pyconfig::View& config);

		/**
		 * @brief Destructor (will shut down the Scenario)
		 *
		 * If the SystemManager gets destroyed it will shut down the scenario by
		 * calling scenario->shutDown()
		 */
		virtual ~SystemManager();

		/**
		 * @brief Returns the configfile for this system
		 *
		 * @return the configfile for this system
		 */
		wns::pyconfig::View getConfigFile() const;

		/**
		 * @brief Returns a pointer to a Station with Id i
		 *
		 * @param[in] i The Id of the Station (same Id as
		 * Station::getStationId())
		 *
		 * @return a pointer to a Station with Id i
		 */
		Station* getStation(uint32_t i) const {
			return stations.at(i);
		};

		/**
		 * @brief Returns the current number of Stations available in the system
		 *
		 * @return the current number of Stations available in the system
		 */
		uint32_t getNumStations() const {
			return stations.size();
		};

		/**
		 * @brief Return a container with all Stations
		 *
		 * @return a container with all Stations
		 *
		 * @note this is copy of the internal container and (depending on the
		 * size) this might be an expensive call. If you want to call a function
		 * for each station consider using forEachStation(F f)
		 */
		StationContainer getAllStations() const;

		/**
		 * @brief provide a member function (pointer) or a functor to do
		 * something for each station
		 *
		 * With this method you can do something for each station the
		 * SystemManager knows and you even don't have to know anything about
		 * the *data structure the stations are kept in.  Have a look at the
		 * following *example. Suppose you are inside the class Scenario and you
		 * want each *station to update it's position. Suppose there exists a
		 * method
		 *
		 * @verbatim
		 * Station::updatePosition() {
		 * ...
		 * }
		 * @endverbatim
		 * Then you would do:
		 * @verbatim
		 * systemManager->forEachStation(mem_fun(&Station::updatePosition));
		 * @endverbatim
		 * Easy, isn't it?
		 *
		 * @param[in] f object providing ()-operator (so-called functor), taking
		 * "Station*" as argument
		 *
		 * @return f, the object that has been used as functor
		 *
		 * @pre At least one Station has to be available
		 */
		template<class F> F forEachStation(F f) {
			assert(!stations.empty());
			return std::for_each(stations.begin(), stations.end(), f);
		}

		/**
		 * @brief Create Scenario as configured in the configuration file
		 */
		virtual void createScenario();

		/**
		 * @brief Returns the Scenario of this system
		 *
		 * @return the Scenario of this system
		 */
		scenario::Scenario* getScenario() const;

		/**
		 * @brief Returns the name of this system (UMTS, TETRA, ...)
		 *
		 * @return the name of this system (UMTS, TETRA, ...)
		 */
		std::string getSystemName() const;

		/**
		 * @brief Add a Station to the system
		 *
		 * @param[in] station A pointer to the Station that should be added to
		 * this system
		 */
		void addStation(Station* station);

	protected:
		/**
		 * @brief The name of the system (e.g. UMTS, TETRA, ...)
		 */
		std::string systemName;

		/**
		 * @brief A pointer to the scenario of this system 
		 *
		 * Unfortunately we need a scenario for each system we're currently
		 * simualting (since propagation conditions are different for each
		 * system). Be aware that this might get removed in the future if we can
		 * find a better solution here.
		 */
		scenario::Scenario* scenario;

		/**
		 * @brief A container that keeps all the stations available in this
		 * system
		 */
		StationContainer stations;

		/**
		 * @brief Pointer to an config file holding the system specific information
		 *
		 * It is very error prone to hold config-file data for RISE and the
		 * protocol stacks in different config files. This should be adressed with
		 * reading the protocol stack specific config file by the system manager
		 * and getting those informations the RISE needs. To avoid a mess like
		 * IoInform we should then read in every constructor of system specific
		 * classes the config file data we need and store it in private class
		 * variables.
		 */
		wns::pyconfig::View pyConfigView;

	private:
		/**
		 * @brief A counter for the station ID in a system
		 *
		 * Each station in a system has a unique ID: Station::stationId. The
		 * variable stationId is a simple counter to ensure that every station
		 * has a unique Id.
		 */
		int stationId;

		/**
		 * @brief The logger
		 */
		RISELogger log;

		/**
		 * @brief Disallow copy constructor
		 */
		SystemManager(const SystemManager&);

		/**
		 * @brief Disallow assignment operator
		 */
		SystemManager& operator=(const SystemManager&);
    };
}

#endif // NOT defined RISE_MANAGER_SYSTEMMANGER_HPP


