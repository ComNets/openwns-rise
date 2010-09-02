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

#include <RISE/antenna/ITUAntennaWithWrap.hpp>
#include <RISE/stations/station.hpp>
#include <cmath>

using namespace rise::antenna;

ITUAntennaWithWrap::ITUAntennaWithWrap(const wns::pyconfig::View& pyConfigView, Station* const station):
    ITUAntenna(pyConfigView, station),
    sizeX(pyConfigView.get<double>("sizeX")),
    sizeY(pyConfigView.get<double>("sizeX"))
{
}

ITUAntennaWithWrap::~ ITUAntennaWithWrap()
{
}

wns::Ratio
ITUAntennaWithWrap::getGain(const wns::Position& pos, const PatternPtr pattern) const
{
    double x1 = getPosition().getX();
    double y1 = getPosition().getY();
    double x2 = pos.getX();
    double y2 = pos.getY();
    double newX2 = 0.0;
    double newY2 = 0.0;

    if((abs(x1 - x2) > (sizeX - abs(x1 - x2))) or (abs(y1 - y2) > (sizeY - abs(y1 - y2))))
    {
        // draw over edge required
        if (abs(x1 - x2) < abs(y1 - y2))
        {
            // draw over x-edge
            if (y1 < y2)
            {
                // [x1, y1] is the lower point
                double dy1 = y1;
                double dy2 = sizeY - y2;
                double dy = dy1 + dy2;
                double dx = x2 - x1;
                newX2 = x1+dx*dy1/dy;
                newY2 = 0.0;
            }
            else
            {
                // [x2, y2] is the lower point -> reverse operation
                double dy1 = sizeY - y1;
                double dy2 = y2;
                double dy = dy1 + dy2;
                double dx = x2 - x1;
                newX2 = x1+dx*dy1/dy;
                newY2 = sizeX;
            }
        }
        else
        {
            // draw over y-edge
            if (x1 < x2)
            {
                // [x1, y1] is the left point
                double dy = y2 - y1;
                double dx1 = x1;
                double dx2 = sizeX - x2;
                double dx = dx1 + dx2;
                newX2 = 0.0;
                newY2 = y1+dy*dx1/dx;
            }
            else
            {
                // [x2, y2] is the left point -> reverse operation
                double dy = y2 - y1;
                double dx1 = sizeX - x1;
                double dx2 = x2;
                double dx = dx1 + dx2;
                newX2 = sizeY;
                newY2 = y1+dy*dx1/dx;
            }
        }
    }
    else
    {
        newX2 = x2;
        newY2 = y2;
    }
    wns::Position newPos(newX2, newY2, pos.getZ());

    MESSAGE_BEGIN(NORMAL, log, m,"Moving point");
    m << "(" << pos.getX() << "," << pos.getY() <<  "," << pos.getZ() << ") to ";
    m << "(" << newPos.getX() << "," << newPos.getY() <<  "," << newPos.getZ() << ")";
    MESSAGE_END();

    return(ITUAntenna::getGain(newPos, pattern));
}
