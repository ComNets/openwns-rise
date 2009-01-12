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

#ifndef __METASYSTEMMANAGER_HPP
#define __METASYSTEMMANAGER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/misc/RISELogger.hpp>
#include <WNS/container/Registry.hpp>

namespace rise {
  	/**
	 * @brief Singleton that holds a list of all available SystemManager at run
	 * time. In addition it performs the setup of the scenario
	 *
	 * @ingroup MANAGER
	 */
	class MetaSystemManager {
	public:
  		/**
		 * @brief Provides global access to the singleton
		 * @return Pointer to the one and only instance
		 *
		 * If called for the first time the instance is created.
		 */
		static MetaSystemManager* getInstance() {
			if(!myself)	myself = new MetaSystemManager();
			return myself;
		};

  		/**
		 * @brief Explicitly delete the singleton before programm stops
		 *
		 * Use with great care
		 */
		static void deleteInstance() {
			if(myself) delete myself;
			myself = NULL;
		};

  		/**
		 * @brief Attach a SystemManager to the MetaSystemManager
		 *
		 * Called by the constructor of the SystemManager, hence there is no
		 * need to call this method explictly.
		 */
		void attach(SystemManager *sysmgr);

		/**
		 * @brief remove SystemManager from MetaSystemManager
		 */
		void detach(SystemManager *sysmgr);

		/**
		 * @brief Returns a SystemManager defined by its system name.
		 */
		SystemManager* getSystemManagerBySystemName(const std::string &aSystemName);

		/**
		 * @brief Returns the total number of stations
		 */
		int getTotalNumStations() {
			int sum = 0;
			for (SystemManagerIterator i=systemManagers.begin();
				 i!=systemManagers.end(); ++i) {
				sum += i->second->getNumStations();
			}
			return sum;
		};

  		/**
		 * @brief Functor or SystemManager member function is executed for each
		 * SystemManager available
		 *
		 * This takes any functor / SystemManager member function and executes the method
		 * for all SystemManager attached here. It returns the functor.
		 * Example:
		   @verbatim
		   MetaSystemManager::getInstance()->forEachSystemManager(mem_fun(&SystemManager::createStations));
		   @endverbatim
		 * @pre At least one SystemManager has to be available
		 */
 		template<class F> inline F forEachSystemManager(F f) {
			for(SystemManagerContainer::const_iterator itr = systemManagers.begin();
				itr != systemManagers.end();
				++itr) {
				f(itr->second);
			}
			return f;
 		}

		/**
		 * @brief Shut down all SystemManagers (at end of simulation)
		 */
		void shutDownSystemManagers();

	private:
		/**
		 * @brief Container with SystemManager
		 */
		typedef wns::container::Registry<std::string, SystemManager*> SystemManagerContainer;

		/**
		 * @brief Iterator to SystemManagerContainer
		 */
		typedef SystemManagerContainer::const_iterator SystemManagerIterator;

  		/**
		 * @brief Default constructor
		 */
		MetaSystemManager();

  		/**
		 * @brief Destructor
		 */
		~MetaSystemManager();

		/**
		 * @brief A pointer to the object itself to turn it into a singleton
		 */
		static MetaSystemManager *myself;

 		/**
		 * @brief Container for all SystemManager available at run time
		 *
		 * The MetaSystemManager holds a vector with all SystemManager available
		 * at run time.
		 */
		SystemManagerContainer systemManagers;

		/**
		 * @brief The logger
		 */
		RISELogger log;
	};
}
#endif


