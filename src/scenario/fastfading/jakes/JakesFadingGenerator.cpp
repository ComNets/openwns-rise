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

#include <RISE/scenario/fastfading/jakes/JakesFadingGenerator.hpp>
#include <WNS/module/Base.hpp>
#include <WNS/distribution/Uniform.hpp>
#include <WNS/rng/RNGen.hpp>

using namespace rise;
using namespace rise::scenario;
using namespace rise::scenario::fastfading;
using namespace rise::scenario::fastfading::jakes;

JakesFadingGenerator::JakesFadingGenerator()
{
}

JakesFadingGenerator::~JakesFadingGenerator()
{}

void
JakesFadingGenerator::initJakes(const double _maximumDopplerFrequency, const double _samplingTime, const unsigned int _numberOfWaves)
{
	currentSampleIndex = 0;

	//maximumDopplerFrequency = _maximumDopplerFrequency;
	samplingTime  = _samplingTime;
	//numberOfWaves = _numberOfWaves;
	numberOfWavesQuarter = _numberOfWaves/4;

	wns::distribution::Uniform randomAngle = 
        wns::distribution::Uniform(0.0, 2.0 * M_PI); // between 0..2pi

	double scaleFactor = 2.0/std::sqrt(static_cast<double>(numberOfWavesQuarter));

	complexTurningFactorVector.resize(numberOfWavesQuarter, 0.0);
	for (unsigned int i=0; i<numberOfWavesQuarter; ++i)
		complexTurningFactorVector[i] = std::polar<double>(1,randomAngle() ) * scaleFactor;

	omega.resize(numberOfWavesQuarter, 0.0);
	for (unsigned int i=0; i<numberOfWavesQuarter; ++i)
		omega[i] = 2.0*M_PI*_maximumDopplerFrequency * samplingTime * std::cos(static_cast<double>((2.0*M_PI*(i+0.5) + randomAngle()) / _numberOfWaves));

	thetaInitial = randomAngle(); // from random number generator
}

// TODO?? std::complex<double> JakesFadingGenerator::getComplexFadingValue(unsigned int sample)

std::complex<double>
JakesFadingGenerator::getNextComplexFadingValue()
{
	std::complex<double> complexSum = 0.0;
	for (unsigned int i=0; i<numberOfWavesQuarter; ++i)
		complexSum += complexTurningFactorVector[i] * std::cos(omega[i]*currentSampleIndex/*now*/ + thetaInitial);
	++currentSampleIndex; // advance timestep
	return complexSum; // * scaleFactor is now already included in complexTurningFactorVector[i]
}

wns::Ratio
JakesFadingGenerator::getNextFadingRatio()
{
	return wns::Ratio::from_factor(std::abs(getNextComplexFadingValue()));
}

double
JakesFadingGenerator::getNextFadingValue()
{
	return std::abs(getNextComplexFadingValue());
}
