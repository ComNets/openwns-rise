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

#ifndef _FTFADING_STRATEGY_HPP
#define _FTFADING_STRATEGY_HPP

#include <RISE/scenario/ftfading/JakesFadingGenerator.hpp>

#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/simulator/Time.hpp>

namespace rise { namespace scenario { namespace ftfading {
	namespace tests { class FTFadingTest; } // declaration for friend
	/** @brief abstract base class to support fading strategy (Python: rise.scenario.FTFading) */
	class FTFading
	{
		friend class rise::scenario::ftfading::tests::FTFadingTest;
	public:
		FTFading(const wns::pyconfig::View& config);
		virtual ~FTFading(){};

		/** @brief function prototype: return the fading value at a certain subCarrier */
		virtual wns::Ratio getFTFading(int _subCarrier) = 0;

		typedef wns::PyConfigViewCreator<FTFading> FTFadingCreator;
		typedef wns::StaticFactory<FTFadingCreator> FTFadingFactory;
		double getSamplingTime() { return samplingTime; }
		int getNumberOfSubChannels() { return numberOfSubChannels; }
	protected:
		wns::logger::Logger logger;
		double samplingTime;
		//simTimeType samplingTime;
		//double samplingFrequency; // = 1.0/samplingTime
		int numberOfSubChannels;
	};

	/** @brief no FTFading strategy is applied */
	class FTFadingOff :
		public FTFading
	{
	public:
		FTFadingOff(const wns::pyconfig::View& config);
		virtual ~FTFadingOff(){};
		virtual wns::Ratio getFTFading(int);
	};

	/** @brief Frequency and Time dependent Fading; Time correlation based on the Jakes model */
	class FTFadingJakes :
		public FTFading
	{
		friend class rise::scenario::ftfading::tests::FTFadingTest;
	public:
		FTFadingJakes(const wns::pyconfig::View& config);
		virtual ~FTFadingJakes(){};
		virtual wns::Ratio getFTFading(int _subCarrier) = 0;
		virtual wns::simulator::Time Now() { return scheduler->getTime(); };
	protected:
		double _dopFreq;
		int _numWaves;
		int _maxSamples;
		std::vector<JakesFadingGenerator*> jakesVector;
		wns::events::scheduler::Interface* scheduler;
	};

	/** @brief FTFading corralated in the time domain and flat in the
	 * frequency domain; All subcarriers suffer the same fading according
	 * the Jakes model **/
	class FTFadingFflat :
		public FTFadingJakes
	{
	public:
		FTFadingFflat(const wns::pyconfig::View& config);
		virtual ~FTFadingFflat();
		virtual wns::Ratio getFTFading(int _subCarrier);
	protected:
		double currentPeriodTime;
		double subCarrierFading;
		wns::Ratio CurrentFading;
	};

	/** @brief FTFading correlated in the time domain and uncorrelated in the
	 * frequency domain; there's a correlation in time according the Jakes Model
	 * fading which is different (uncorrelated) for every frequency applied  */
	class FTFadingFuncorrelated :
		public FTFadingJakes
	{
	public:
		FTFadingFuncorrelated(const wns::pyconfig::View& config);
		virtual ~FTFadingFuncorrelated(){};

		virtual wns::Ratio getFTFading(int _subCarrier);

	protected:
		std::vector<double> sampleIndex;
		std::vector<double> subCarrierFading;
		std::vector<wns::Ratio> currentFading;
	};

	/** @brief FTFading correlated in the time domain and neighbour
	 * correlated in the frequency domain;  there's a correlation in time
	 * according the Jakes Model fading which is correlated in frequency
	 * according  the FTFadingFneighbourCorrelation strategy */
	class FTFadingFneighbourCorrelation :
		public FTFadingJakes
	{
	public:
		FTFadingFneighbourCorrelation(const wns::pyconfig::View& config);
		virtual ~FTFadingFneighbourCorrelation(){};

		wns::Ratio getFTFading(int _subCarrier);

	protected:
		std::vector<double> sampleIndex;
		double currentSampleIndex;
		/** @brief Vector used to calculate the FTFading of every subcarrier */
		std::vector<double> subCarrierFading;
		std::vector<wns::Ratio> currentFading;

		/** @brief Factor which will define the amount of each subcarrier
		 * used to build the neighbour frequency correlation */
		double _neighbourCorrelationFactor;
	};
}}}

#endif


