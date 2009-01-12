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

#include <RISE/antenna/Antenna.hpp>
#include <RISE/RISE.hpp>
#include <RISE/stations/station.hpp>
#include <WNS/pyconfig/View.hpp>

#include <fstream>

using namespace rise::antenna;

Antenna::Antenna():
 	  log("Antenna"),
	  radius(0),
	  drawPattern(false),
 	  patternOutputFile(),
	  pd_station(0),
 	  pd_positionOffset(),
	  position(),
 	  id(counter++)
{}

/*! Constructor setting antenna characteristic, position offset
  relative to the station it belongs to, and rotation axis direction. */
Antenna::Antenna(const wns::pyconfig::View& pyConfigView,
				 Station* const station) :
	log("Antenna"),
	radius(0),
	drawPattern(pyConfigView.get<bool>("drawAntennaPattern")),
	patternOutputFile(),
	pd_station(station),
	pd_positionOffset(),
	position(),
	id(counter++)
{
	if(RISE::getPyConfigView().get<bool>("debug.antennas"))
	{
		log.switchOn();
	} else {
		log.switchOff();
	}

	typedef std::vector<double> container;
	container coords = pyConfigView.getSequence<container>("coordOffset");
	assure( coords.size() == 3,
			"Wrong declaration of CoordOffset = [x, y, z] in ConfigFile");
	pd_positionOffset = wns::PositionOffset(coords.at(0),
											coords.at(1),
											coords.at(2));
	radius = pyConfigView.get<double>("radius");
	MESSAGE_BEGIN(NORMAL, log, m,"New Antenna created (");
	m << pyConfigView.get<std::string>("__plugin__") << ")";
	MESSAGE_END();

	this->startObserving(this->pd_station);

	if (drawPattern == true)
	{
		std::string outputDir = wns::simulator::getConfiguration().get<std::string>("outputDir");
		patternOutputFile = outputDir + "/" + pyConfigView.get<std::string>("patternOutputFile");
		preparePatternOutputFile();
		MESSAGE_SINGLE(NORMAL,log,"Writing Antenna Pattern into "<<patternOutputFile);
	}
}

Antenna::~Antenna()
{}

void Antenna::drawRadiationPattern() const
{
	const uint32_t resolution = 360;
	double step = 2 * M_PI / resolution;
	double zero_dB_length = radius, length = 0, angle = 0;
	wns::Ratio gain = wns::Ratio::from_dB(0),
		gain_min = wns::Ratio::from_dB(-10);
	std::fstream outfile;
	outfile.open(patternOutputFile.c_str(), std::ios::out | std::ios::app);
	if (!outfile.is_open())
	{
		MESSAGE_BEGIN(NORMAL, log, m,"File ");
		m << patternOutputFile + " could not be opened." + " Simulation will continue.\n";
		MESSAGE_END();
		return;
	}
	wns::PositionOffset pOffset;
	wns::Position position, myPosition = getPosition();
	for (uint32_t k=0; k<resolution; k++)
	{
		angle = step * k;
		pOffset.setPolar(1, angle, M_PI/2);
		position = myPosition + pOffset;
		gain = getGain(position, PatternPtr());
		if (gain<gain_min)
			length = 0;
		else
			length = (zero_dB_length /(-gain_min.get_dB())) * gain.get_dB()
				+ zero_dB_length;
		outfile << myPosition.getX() + length * cos(angle) <<  " "
				<< myPosition.getY() + length * sin(angle) << std::endl;
	}
	outfile << "\n";
	outfile.close();
}

rise::Station* Antenna::getStation() const
{
	return pd_station;
}

void Antenna::positionChanged()
{
	position = pd_station->getPosition() + pd_positionOffset;
}

void Antenna::preparePatternOutputFile()
{
	if(patternOutputFilePrepared.find(patternOutputFile) == patternOutputFilePrepared.end()) {
		std::fstream outfile;
		outfile.open(patternOutputFile.c_str(), std::ios::out|std::ios::trunc);
		outfile.close();
		patternOutputFilePrepared.insert(patternOutputFile);

		MESSAGE_BEGIN(NORMAL, log, m,"Antenna pattern will be written to: ");
		m << patternOutputFile;
		MESSAGE_END();
	}
}


uint32_t Antenna::counter = 0;
std::set<std::string> Antenna::patternOutputFilePrepared = std::set<std::string>();


