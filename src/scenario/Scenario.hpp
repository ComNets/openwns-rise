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

#ifndef RISE_SCENARIO_SCENARIO_HPP
#define RISE_SCENARIO_SCENARIO_HPP

#include <RISE/antenna/Antenna.hpp>

#include <WNS/StaticFactory.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/Position.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/Types.hpp>
#include <WNS/Broker.hpp>
#include <WNS/Singleton.hpp>

#include <WNS/rng/RNGen.hpp>

#include <map>
#include <string>


namespace rise
{
	class SystemManager;

	namespace scenario
	{
		namespace sceneryfile {
			class SceneryFile;
		}

		class Scenario
		{
		public:

			/**
			 * @brief List of diffenent types of morphology the user can choose
			 * from
			 */
			enum morphologyType
			{
				noMorphology = 0, sameMorphology, urban, subUrban, quasiOpen, open,
				forest, sea, vehicular, multiwall, outdoor
			};

			/**
			 * @brief List of diffenent types of topography the user can choose
			 * from
			 */
			enum topographyType
			{
				noTopography = 0, quasiSmooth, metropolitan
			};

			explicit
			Scenario(const wns::pyconfig::View& config);

			virtual
			~Scenario();

			wns::rng::RNGen&
			getRNG() const
			{
				return *wns::module::Base::getService<wns::rng::RNGen*>("SPEETCL-RNG");
			}

			void
			openSceneryFile(const std::string& filename, const std::string& filetype);

			const sceneryfile::SceneryFile&
			getSceneryFile(const std::string& filename) const;

		private:
			typedef std::map<std::string, sceneryfile::SceneryFile*> SceneryFilesMap;

			SceneryFilesMap sceneryFiles;

		};

		typedef wns::Broker<Scenario> ScenarioBroker;
		typedef wns::SingletonHolder<ScenarioBroker> GlobalScenarioBroker;
	}
}

#endif // NOT defined RISE_SCENARIO_SCENARIO_HPP
