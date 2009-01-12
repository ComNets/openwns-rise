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

#ifndef _SC_HPP
#define _SC_HPP

#include <vector>
#include <fstream>

#include <RISE/scenario/sceneryfile/SceneryFile.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>
#include <RISE/scenario/Scenario.hpp>

namespace rise
{
	namespace scenario
	{
		namespace sceneryfile
		{
			//! This class reads the scenery maps from a GOOSE style scenery file.
			class SC :
				public SceneryFile
			{
			public:

				SC();
				SC(const std::string filename, const scenario::Scenario& scenario);
				virtual ~SC() {}

				virtual bool hasMorphologyMap() const { return true; }
				virtual bool hasPathlossMap() const { return true; }
				virtual bool hasShadowingMap() const { return true; }
				virtual bool hasObstructionList() const { return false; }

				virtual const scenerymap::MorphologyMap2D& getMorphologyMap() const
				{
					assert(initialized);
					return morphologyMap;
				}

				virtual const scenerymap::PathlossMaps2D& getPathlossMap() const
				{
					assert(initialized);
					return pathlossMaps;
				}

				virtual const scenerymap::ShadowingMap2D& getShadowingMap() const
				{
					assert(initialized);
					return shadowingMap;
				}

			private:
				bool initialized;

				std::ifstream SCfile;

				scenerymap::MorphologyMap2D morphologyMap;
				scenerymap::PathlossMaps2D pathlossMaps;
				scenerymap::ShadowingMap2D shadowingMap;

			};
		}
	}
}

#endif // _SC_HPP
