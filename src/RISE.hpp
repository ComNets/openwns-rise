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

#ifndef RISE_RISE_HPP
#define RISE_RISE_HPP

#include <WNS/module/Module.hpp>
#include <WNS/rng/RNGen.hpp>

/**
 * @brief This module is entirely in namespace "rise"
 */

namespace rise {
	/**
	 * @brief Enables the module functionality within the Simulation Framework
	 */
	class RISE :
		public wns::module::Module<RISE>
	{
	public:
		/**
		 * @brief Default c'tor
		 */
		RISE(const wns::pyconfig::View& _pyConfigView);
		/**
		 * @brief Default d'tor
		 */
		virtual ~RISE();
		/**
		 * @brief Configures this module before StartUp()
		 *
		 * Sets some pointers like the global EventScheduler or
		 * RandomNumberGenerator which are provided by the Simulation Framework.
		 */
		virtual void configure();
		/**
		 * @brief Adds all probes
		 */
		virtual void startUp();
		/**
		 * @brief Shut down the module after EventScheduler stopped
		 */
		virtual void shutDown();
		/**
		 * @brief Return Mobility RNG pointer
		 */
		wns::rng::RNGen* getMobilityRNG();

	private:
		wns::rng::RNGen* mobilityRNG;
	};
}

#endif
/**
 * @page RISE
 *
 * The following figure gives a rough overview over the concept of RISE. It may
 * be outdated since it is not automagically generated ...
 * @dot
 digraph overview {
   node [shape=record, fontname=Helvetica, fontsize=10, color=red, height=0.2,width=0.4];
   edge [style="solid", arrowhead="open", fontsize=10, fontname=Helvetica ]

   MetaSystemManager [ label="MetaSystemManager" URL="\ref rise::MetaSystemManager"];
   SystemManager [ label="SystemManager" URL="\ref rise::SystemManager"];
   ChannelInterface [ label="ChannelInterface" URL="\ref CHANNELINTERFACE" ];

   FastFading [ label="FastFading" URL="\ref rise::FastFading" ];
   Shadowing [ label="Shadowing" URL="\ref rise::Shadowing" ];
   Pathloss [ label="Pathloss" URL="\ref rise::Pathloss" ];
   Mobility [ label="Mobility" URL="\ref rise::Mobility" ];

   Scenario [ label="Scenario" URL="\ref rise::Scenario" ];
   Medium [ label="Medium" URL="\ref rise::Medium" ];

   PhysicalResource [ label="PhysicalResource" URL="\ref rise::PhysicalResource" ];

   TransmissionObject [ label="TransmissionObject" URL="\ref rise::TransmissionObject" ];

   Station [ label="Station" URL="\ref rise::Station" ];
   Antenna [ label="Antenna" URL="\ref rise::Antenna" ];
   AntennaPattern [ label="AntennaPattern" URL="\ref rise::AntennaPattern" ];

   Receiver [ label="Receiver" URL="\ref rise::Receiver" ];
   Transmitter [ label="Transmitter" URL="\ref rise::Transmitter" ];
   Receiver [ label="Receiver" URL="\ref rise::Receiver" ];

   Decoder [ label="Decoder" URL="\ref rise::Decoder" ];
   Demodulator [ label="Demodulator" URL="\ref rise::Demodulator" ];

   Station -> SystemManager [ label="1" ];
   Station -> Antenna [ label="1,*" ];
   Station -> Transmitter [ label="1,*" ];
   Station -> Receiver [ label="1,*" ];

   MetaSystemManager -> SystemManager [ label="1,* (manages)" ];

   SystemManager -> Station [ label="1,* (creates and manages)" ];
   SystemManager -> Scenario [ label="1" ];

   ChannelInterface -> SystemManager [ label="1" ]

   Medium -> PhysicalResource [ label="1,* (creates and manages)" ];

   PhysicalResource -> Transmitter [ label="0,*" ];
   PhysicalResource -> Receiver [ label="0,* (notifies)" ];
   PhysicalResource -> TransmissionObject [ label="0,* (active transmisssions)" ];

   Scenario -> Pathloss [ label="1" ];
   Scenario -> Shadowing [ label="1" ];
   Scenario -> FastFading [ label="1" ];
   Scenario -> Mobility [ label="1" ];
   Scenario -> Station [ label="0,* (stations to move)" ];

   TransmissionObject -> Transmitter [ label="1 (sender)" ];
   TransmissionObject -> Antenna [ label="1 (tx antenna)" ];

   Antenna -> Station [ label="1" ];
   Antenna -> AntennaPattern [ label="1" ];

   Receiver -> Decoder [ label="1" ];
   Receiver -> Demodulator [ label="1" ];
 }
 @enddot
 */


/**
 * @}
 */

/**
 * @defgroup PACKAGES Functional Packages
 * @brief Contains classes grouped by functionality
 * @{
 */

/** @defgroup STATION Station*/
/** @defgroup TRANSCEIVER Transceiver*/
/** @defgroup TRANSMISSIONOBJECT TransmissionObject*/
/** @defgroup SCENARIO Scenario*/
/** @defgroup MEDIUM Medium*/
/** @defgroup ANTENNA Antenna*/
/** @defgroup MISC Misc*/
/** @defgroup CHANNELINTERFACE Channel Interface*/
/** @defgroup DECODER Decoder*/
/** @defgroup DEMODULATOR Demodulator*/
/** @defgroup MANAGER Manager*/

/**
 * @}
 */


