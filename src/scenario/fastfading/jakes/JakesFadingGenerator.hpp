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

#ifndef JAKES_FADING_GENERATOR
#define JAKES_FADING_GENERATOR

#include <WNS/PowerRatio.hpp>

#include <vector>
#include <complex>


namespace rise { namespace scenario { namespace fastfading { namespace jakes {

	class JakesFadingGenerator
	{
	public:
		// empty constructor needed for std::vector<JakesFadingGenerator>
		JakesFadingGenerator();
		~JakesFadingGenerator();

		/**@brief initialize Jakes generator with parameters
		 * @param[in] maximumDopplerFrequency frequency [Hz] of max Doppler shift; proportional to vehicle speed
		 * @param[in] samplingTime time [s] between discrete steps of the output (typ: 1ms)
		 * @param[in] numberOfWaves number of simulated incoming waves from all	directions (typ: 100)
		 */
		void initJakes(const double _maximumDopplerFrequency, const double _samplingTime, const unsigned int _numberOfWaves);

		std::complex<double>     getNextComplexFadingValue();
		wns::Ratio               getNextFadingRatio();
		double                   getNextFadingValue();

	private:
		/**@brief current time [slot] */
		unsigned int        currentSampleIndex;
		/**@brief maximum Doppler shift fd=f(velocity,centerfrequency) */
		//double              maximumDopplerFrequency;      // maximum Doppler shift
		/**@brief samplingTime==frameDuration */
		double              samplingTime;
		/**@brief should be 1/frameDuration */
		//double              samplingFrequency_;      // sampling frequency
		/**@brief numberOfWaves */
		//unsigned int        numberOfWaves;
		/**@brief numberOfWaves/4 */
		unsigned int        numberOfWavesQuarter; // No_ in formulas

		//double              scaleFactor; // std::sqrt(2.0/(static_cast<double>(numberOfWavesQuarter)))
		//UniDis randomAngle; // between 0..2pi

		/**@brief complex turning factors (e^-j*phase) */
		std::vector< std::complex<double> > complexTurningFactorVector;
		/**@brief doppler frequency (normalized to sampling frequency) for different angles of arrival */
		std::vector<double>                omega;
		/**@brief arbitrary initial phase shift of each wave */
		//std::vector<double>                theta;
		/**@brief arbitrary initial phase shift of each wave */
		double              thetaInitial;
	};

}}}}

#endif
