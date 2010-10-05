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

#include <RISE/antenna/ITUAntenna.hpp>
#include <RISE/stations/station.hpp>

using namespace rise::antenna;

ITUAntenna::ITUAntenna(const wns::pyconfig::View& pyConfigView, Station* const station):
    Antenna(pyConfigView, station),
    gain_(pyConfigView.get<wns::Ratio>("antennaGain")),
    direction_(pyConfigView.get<double>("elevation"),
               pyConfigView.get<double>("azimuth")),
    theta_3dB(pyConfigView.get<double>("theta_3dB")),
    phi_3dB(pyConfigView.get<double>("phi_3dB")),
    gainCC_("rise.antenna.ITUAntenna.gain")
{
}

ITUAntenna::~ ITUAntenna()
{
}

wns::Ratio
ITUAntenna::getGain(const wns::Position& pos, const PatternPtr pattern) const
{
    wns::PositionOffset p = pos - getPosition();

    // Apply antenna orientaion
    wns::Direction d = direction_.calcAngles(p);

    double elevation = d.getElevation();

    double azimuth = d.getAzimuth();
    if (azimuth > 3.14)
    {
        azimuth = azimuth - 2 * 3.14159265;
    }
    // theta_3dB = 70 degree = 1.221730475 rad
    //double horiz = -1.0 * std::min(12.0 * pow(azimuth/1.221730475, 2), 20.0);
    double horiz = -1.0 * std::min(12.0 * pow(azimuth/theta_3dB, 2), 20.0);

    // Elevation of main lobe is 90 degree = 1.570796325 rad in rise!
    // phi_3dB = 15 degree = 0.261799387 rad
    //double vert = -1.0 * std::min(12.0 * pow(elevation/0.261799387, 2), 20.0);
    double vert = 0.0;
    if(phi_3dB > 0)
    {
        vert = -1.0 * std::min(12.0 * pow(elevation/phi_3dB, 2), 20.0);
    }

    double directivity = -1 * std::min( -1 * (horiz + vert), 20.0);

    MESSAGE_BEGIN(NORMAL, log, m,"Directivity of ITUAntenna in (");
    m << pos.getX() << "," << pos.getY() <<  "," << pos.getZ() << "): "
        << directivity;
    MESSAGE_END();

    gainCC_.put(directivity + gain_.get_dB(), boost::make_tuple(
        "rise.scenario.mobility.x", (int) pos.getX(),
        "rise.scenario.mobility.y", (int) pos.getY(),
        "rise.scenario.StationID", getStation()->getStationId()));

    return wns::Ratio::from_dB(directivity) + gain_;
}
