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

#ifndef _MEDIUM_HPP
#define _MEDIUM_HPP

#include <RISE/misc/RISELogger.hpp>

#include <vector>

namespace rise { namespace medium {
	class PhysicalResource;

	/**
	 * @brief Creates and manages PhysicalResource
	 * @ingroup MEDIUM
	 *
	 * This class provides the physical resources (kind of a factory) and
	 * manages them in a list. It is a singleton, therefore providing the method
	 * getInstance(); deleteInstance() should be used to destroy the singleton
	 * at simulation shutdown.
	 */
	class Medium {
		typedef std::vector<PhysicalResource*> PhysicalResourceContainer;

		typedef PhysicalResourceContainer::iterator PhysicalResourceIterator;

	public:
		/**
		 * @brief Singleton's getInstance
		 */
		static Medium* getInstance();

		/**
		 * @brief Delete this singleton
		 */
		static void deleteInstance();

		/**
		 * @brief Removes all PhysicalResources
		 *
		 * @note This method is only for test purposes (will remove all
		 * PhysicalResources created by Medium)
		 */
		void reset();

		/**
		 * @brief Returns a PhysicalResource, with the specified frequency and
		 * bandwidth.
		 *
		 * @todo (msg) Rename this method into allocatePhysicalResource and add
		 * a method freePhysicalResource. This way the PhysicalResources can be
		 * freed if they are not needed by anyone any longer.
		 */
		PhysicalResource* getPhysicalResource(double f, double b);


        /**
         * @brief Returns the index of the Physical Resource containing the frequency
         * usefull for implementations using integer sub channel indices
         */
        unsigned int
        getPhysicalResourceIndex(double f);


	private:
		Medium();
		~Medium();
		static Medium* myself;
		PhysicalResourceContainer prs;
		RISELogger log;
		bool checkForOverlap(PhysicalResource* pr) const;

		/**
		 * @brief Disallow copy constructor
		 */
		Medium(const Medium&);

		/**
		 * @brief Disallow assignment operator
		 */
		Medium& operator=(const Medium&);
	};
}}
#endif


