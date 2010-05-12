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

#ifndef _SCENERYMAP_HPP
#define _SCENERYMAP_HPP

#include <WNS/container/Matrix.hpp>
#include <WNS/NearestNeighbour.hpp>
#include <WNS/NLinear.hpp>
#include <cstddef>
#include <stdint.h>

namespace rise
{
    namespace scenario
    {
		namespace scenerymap
		{
			typedef double resolution;
			typedef std::size_t dimension;

			// standard dimensions
			namespace Dim {
				enum { X = 0, Y = 1, Z = 2 };
			}

			//! Template class for scenery maps.
			template<typename t, std::size_t dims>
			class SceneryMap : public wns::container::Matrix<t, dims, t>
			{
			public:

				SceneryMap() :
					wns::container::Matrix<t, dims, t>(),
					resolutions()
				{}

				SceneryMap(const std::size_t sizes[dims],
						   const resolution res[dims],
						   const t defaultValue = t()) :
					wns::container::Matrix<t, dims, t>(sizes, defaultValue),
					resolutions()
				{
					// const resolution res[dims] is treated as
					// const resolution* res, which can't be
					// assigned directly to resolutions
					for (unsigned long int i = 0; i < dims; ++i) {
						resolutions[i] = res[i];
					}
				}


				void setResolution(const dimension dim, const resolution res)
				{ resolutions[dim] = res; }

				resolution getResolution(const dimension dim) const
				{ return resolutions[dim]; }

			private:
				resolution resolutions[dims];
			};

			//! Standard morphology map used by scenario.
			typedef SceneryMap<char, 2> MorphologyMap2D;

			//! Standard pathloss map used by scenario.
			typedef SceneryMap<double, 2> PathlossMap2D;
			typedef std::vector<PathlossMap2D> PathlossMaps2D;

			//! Standard shadowing map used by scenario.
			typedef SceneryMap<double, 2> ShadowingMap2D;

			// 2D interpolated map
			typedef wns::Interpolation<double, 2> Interpolation2D;

			// 2D nearest neighbour interpolation.
			typedef wns::NearestNeighbour<double, 2> NearestNeighbour2D;

			// Bilinear interpolation.
			typedef wns::NLinear<double, 2> Bilinear;
		}
	}
}

#endif // _SCENERYMAP_HPP


