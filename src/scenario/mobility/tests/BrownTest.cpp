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

#include <RISE/scenario/mobility/Brown.hpp>
#include <WNS/evaluation/statistics/moments.hpp>
#include <WNS/evaluation/statistics/pdf.hpp>
#include <WNS/Positionable.hpp>
#include <WNS/pyconfig/Parser.hpp>
#include <WNS/TestFixture.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

namespace rise { namespace scenario {
	namespace mobility {
		namespace tests {

	class BrownTest :
		public wns::TestFixture
	{

		/** @brief class that collects movements.
		 *
		 * This class constructs a configurable grid and stores how many
		 * movements have been in which tile of the grid.
		 */
		class Observer
		{
			/** @brief total number of moves counted */
			int counter;
			/** @brief xsize of the scenario area */
			double xsize;
			/** @brief ysize of the scenario area */
			double ysize;
			/** @brief how many tiles to construct in both x and y direction */
			int resolution;

			/** @brief shortcut */
			typedef std::vector< std::vector<int> > IntMatrix;
                        typedef std::vector< std::vector<wns::evaluation::statistics::Moments> > MomentsMatrix;
			/** @brief storage container for the result */
			IntMatrix moveObservation;
			MomentsMatrix posObservation;
                    wns::evaluation::statistics::PDF pdf;

		public:
			Observer(double _xsize,
					 double _ysize,
					 int _resolution) :
				counter(0),
				xsize(_xsize),
				ysize(_ysize),
				resolution(_resolution),
				moveObservation( resolution, std::vector<int>(resolution, 0)),
				posObservation(),
				pdf( 0,  // double    aMinXValue        = defaultMinXValue, 
					 50, // double    aMaxXValue        = defaultMaxXValue,
					 50, // unsigned long int      aResolution       = defaultResolution,
                                     wns::evaluation::statistics::PDF::linear, // scaleType
                                     wns::evaluation::statistics::StatEval::fixed,  //formatType  aFormat           = defaultFormat,
					 "UsersPerElement", //const char* aName             = nil,
					 "Number of Mobility Objects Per Area Element") //const char* aDescription      = nil);)
			{
				posObservation.resize(resolution);
				for (size_t counter = 0; counter < posObservation.size(); ++counter)
				{
					posObservation[counter].resize(resolution);
				}
			}

			/** @brief logs pos*/
			void
			countMove(const wns::Position& pos)
			{
				++counter;

				int xbin = int( pos.getX() / ( xsize / resolution ));
				int ybin = int( pos.getY() / ( ysize / resolution ));

				moveObservation[xbin][ybin] = moveObservation[xbin][ybin] + 1;
			}

			void
			countNumbers()
			{
				for (int ybin = 0; ybin < resolution; ++ybin)
					for (int xbin = 0; xbin < resolution; ++xbin)
					{
						posObservation[xbin][ybin].put(moveObservation[xbin][ybin]);
						pdf.put(moveObservation[xbin][ybin]);
						moveObservation[xbin][ybin] = 0;
					}
			}

			/** @brief returns all bin entries */
			std::string
			print(){
				int decimalPlaces;
				decimalPlaces = 4;
				std::stringstream strm;

				strm << "# Counted " << counter << " moves in total." << std::endl;
// 				for (int ybin = 0; ybin < resolution; ++ybin){
// 					for (int xbin = 0; xbin < resolution; ++xbin){
//                                              strm.width(strmWidth);
// 						strm << moveObservation[xbin][ybin] << " ";
// 					}
// 					strm << std::endl;
// 				}

				strm << std::endl;
				strm << "# Mean number of users per cell:" << std::endl;
				for (int ybin = 0; ybin < resolution; ++ybin){
					for (int xbin = 0; xbin < resolution; ++xbin){
						float mean;
						mean = posObservation[xbin][ybin].mean();
                                                // round mean to decimal places 
						mean = floor((mean*(10*decimalPlaces)) + 0.5)/(10*decimalPlaces);
						strm.width(decimalPlaces+1);
						strm << mean << " ";
					}
					strm << std::endl;
				}

				strm << std::endl;
				strm << "# Variance per cell:" << std::endl;
				for (int ybin = 0; ybin < resolution; ++ybin){
					for (int xbin = 0; xbin < resolution; ++xbin){
						float variance;
						variance = posObservation[xbin][ybin].variance();
                                                // round variance to decimal places
						variance = floor((variance*(10*decimalPlaces)) + 0.5)/(10*decimalPlaces);
						strm.width(decimalPlaces+1);
						strm << variance << " ";
					}
					strm << std::endl;
				}

				strm << std::endl;
				strm << "# Trials per cell:" << std::endl;
				for (int ybin = 0; ybin < resolution; ++ybin){
					for (int xbin = 0; xbin < resolution; ++xbin){
						float trials;
						trials = posObservation[xbin][ybin].trials();
						strm.width(decimalPlaces+1);
						strm << trials << " ";
					}
					strm << std::endl;
				}
				return strm.str();
 			}

			std::string
			printPDF(){
				std::stringstream strm;
				pdf.print(strm);
				return strm.str();
			}
		};


		CPPUNIT_TEST_SUITE( BrownTest );
		CPPUNIT_TEST( testMovementRect );
		CPPUNIT_TEST( testMovementCirc );
		CPPUNIT_TEST( testMovementHex );
		CPPUNIT_TEST_SUITE_END();

		typedef std::vector<BrownianBase*> Testees;
		Testees testeesRect;
        Testees testeesCirc;
        Testees testeesHex;
		Observer* observerRect;
        Observer* observerCirc;
        Observer* observerHex;

	public:

		void
		prepare()
		{
			{ // prepare testMovementRect
				// here you can specify how many testees you would like to have
				for (int ii = 0; ii < 10000; ++ii){
					wns::pyconfig::Parser config;

					std::stringstream ss;
					ss << "from rise.Mobility import BrownianRect\n"
					   << "import wns\n"
					   << "mobility = BrownianRect([0,0,200,200])\n"
					   << "mobility.userVelocityDist.RNGENName='SPEETCL-RNG'\n";
					config.loadString(ss.str());

					BrownianBase* testee = new BrownianRect(config.get("mobility"));
					testeesRect.push_back(testee);
				}
				observerRect = new Observer( 200, 200, 20 );
			}

			{ // prepare testMovementCirc
				// here you can specify how many testees you would like to have
				for (int ii = 0; ii < 10000; ++ii){
					wns::pyconfig::Parser config;

					std::stringstream ss;
					ss << "from math import sqrt\n"
					   << "from rise.Mobility import BrownianCirc\n"
					   << "from wns import Position\n"
					   << "import wns\n"
					   << "mobility = BrownianCirc(Position(200/2,200/2,1.5),200/2)\n"
					   << "mobility.userVelocityDist.RNGENName='SPEETCL-RNG'\n";
					config.loadString(ss.str());

					BrownianBase* testee = new BrownianCirc(config.get("mobility"));
					testeesCirc.push_back(testee);
				}
				observerCirc = new Observer( 200, 200, 20 );
			}

			{ // prepare testMovementHex
				// here you can specify how many testees you would like to have
				for (int ii = 0; ii < 10000; ++ii){
					wns::pyconfig::Parser config;

					std::stringstream ss;
					ss << "from math import sqrt\n"
					   << "from rise.Mobility import BrownianEquiangularPolygon\n"
					   << "from wns import Position\n"
					   << "import wns\n"
					   << "mobility = BrownianEquiangularPolygon(Position(150,150,1.5),150*(sqrt(3.0)/2),6,30.0)\n"
					   << "mobility.userVelocityDist.RNGENName='SPEETCL-RNG'\n";
					config.loadString(ss.str());

					BrownianBase* testee = new BrownianEquiangularPolygon(config.get("mobility"));
					testeesHex.push_back(testee);
				}
				observerHex = new Observer( 300, 300, 30 );
			}
		}

		void
		cleanup()
		{
			delete observerRect;
			for (Testees::iterator iter = testeesRect.begin(); iter != testeesRect.end(); ++iter)
				delete *iter;
			testeesRect.clear();
			delete observerCirc;
			for (Testees::iterator iter = testeesCirc.begin(); iter != testeesCirc.end(); ++iter)
				delete *iter;
			testeesCirc.clear();		}

		void
		testMovementRect()
		{
			// Count 1e5 moves, distributed on the number of testees
			for (unsigned int steps = 0; steps < 100000 / testeesRect.size(); ++steps){
				for (Testees::iterator iter = testeesRect.begin(); iter != testeesRect.end(); ++iter){
					(*iter)->move();
					observerRect->countMove((*iter)->getPosition());
				}
				observerRect->countNumbers();
			}

			// write logged output to a file
			std::fstream outfile( "output/BrownMobRect.count", std::fstream::out );
			outfile << observerRect->print();
			outfile.close();

			std::fstream outfile2( "output/BrownMobRect.pdf", std::fstream::out );
			outfile2 << observerRect->printPDF();
			outfile2.close();

			// no assertions yet.
		}

		void
		testMovementCirc()
		{
			// Count 1e5 moves, distributed on the number of testees
			for (unsigned int steps = 0; steps < 100000 / testeesCirc.size(); ++steps){
				for (Testees::iterator iter = testeesCirc.begin(); iter != testeesCirc.end(); ++iter){
					(*iter)->move();
					observerCirc->countMove((*iter)->getPosition());
				}
				observerCirc->countNumbers();
			}

			// write logged output to a file
			std::fstream outfile( "output/BrownMobCirc.count", std::fstream::out );
			outfile << observerCirc->print();
			outfile.close();

			std::fstream outfile2( "output/BrownMobCirc.pdf", std::fstream::out );
			outfile2 << observerCirc->printPDF();
			outfile2.close();

			// no assertions yet.
		}

		void
		testMovementHex()
		{
			// Count 1e5 moves, distributed on the number of testees
			for (unsigned int steps = 0; steps < 100000 / testeesHex.size(); ++steps){
				for (Testees::iterator iter = testeesHex.begin(); iter != testeesHex.end(); ++iter){
					(*iter)->move();
					observerHex->countMove((*iter)->getPosition());
				}
				observerHex->countNumbers();
			}

			// write logged output to a file
			std::fstream outfile( "output/BrownMobHex.count", std::fstream::out );
			outfile << observerHex->print();
			outfile.close();

			std::fstream outfile2( "output/BrownMobHex.pdf", std::fstream::out );
			outfile2 << observerHex->printPDF();
			outfile2.close();

			// no assertions yet.
		}

	};

	CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( BrownTest, wns::testsuite::Performance() );
} // tests
} // mobility
} // scenario
} // rise
