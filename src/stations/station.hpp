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

#ifndef RISE_STATION_HPP
#define RISE_STATION_HPP

#include <RISE/misc/pointer.hpp>
#include <RISE/scenario/Scenario.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>
#include <RISE/scenario/mobility/Mobility.hpp>
#include <RISE/manager/systemmanager.hpp>

#include <WNS/Positionable.hpp>
#include <WNS/PositionObserver.hpp>
#include <WNS/Position.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/Subject.hpp>
#include <WNS/Observer.hpp>
#include <WNS/service/phy/power/PowerMeasurement.hpp>

#include <vector>
#include <list>

namespace rise { namespace antenna {
	class Antenna;
}}

namespace rise {
	class SystemManager;
	class MobilityInterface;
	/**
	 * @brief The base class for all stations.
	 * @ingroup STATION
	 */
	class Station :
		virtual public wns::PositionableInterface,
		public wns::Subject<wns::PositionObserver>,
		public wns::Observer<wns::PositionObserver>
	{
	public:

		/**
		 * @brief Constructor
		 */
		Station(const wns::pyconfig::View& _pyConfigView);

		/**
		 * @brief Destructor
		 */
		virtual ~Station();

		/**
		 * @brief to inject a pointer to the mobility service
		 */
		virtual void
		setMobility(rise::scenario::mobility::MobilityInterface* _mobility);

		/**
		 * @brief Implement wns::PositionObserver
		 */
		virtual void
		positionWillChange();

		virtual void
		positionChanged();

		/**
		 * @brief Implement wns::PositionableInterface
		 */
		virtual const wns::Position&
		getPosition() const;

		virtual double
		getDistance(PositionableInterface* p);

		virtual double
		getDistance(const PositionableInterface& p);

        virtual double
        getAngle(const PositionableInterface& p);

		/**
		 * @brief Initialize object
		 *
		 * Must be called after object initialization. Classes deriving from Station that
		 * want to provide their own Initialize() code must call this function.
		 */
		virtual void initialize();

		/**
		 * @brief Return the ID of this Station
		 *
		 * The ID is set by the SystemManager
		 */
		int32_t getStationId() const;

		/**
		 * @brief Set the ID of this Station
		 *
		 * The ID is set by the SystemManager
		 */
		void setStationId(int32_t id);

		/**
		 * @brief Returns a pointer to the Antenna of this Station
		 */
		antenna::Antenna* getAntenna() const;

		/**
		 * @brief Returns the SystemManager of this Station
		 *
		 * Each Station belongs to a special system (UMTS, H2, 802.11 ...). The
		 * SystemManager of a system keeps track of all Stations, the Scenario
		 * and other stuff belonging to this system. Further it provides a nice
		 * way to things for each Station of this system by using
		 * SystemManager::forEachStation().
		 */
		virtual SystemManager* getSystemManager() const = 0;

		//! Move to a given position
		virtual void moveTo(const wns::Position& p);
		//! Move by a given positionOffset
		virtual void moveTo(const wns::PositionOffset& p);

		/**
		 * @brief Check if a pathloss map has been assigned to this station.
		 */
		bool hasPathlossMap() const;

		/**
		 * @brief Return reference to the pathloss map.
		 */
		const scenario::scenerymap::PathlossMap2D& getPathlossMap() const;

		/**
		 * @brief Return reference to the interpolated pathloss map.
		 */
		const scenario::scenerymap::Interpolation2D& getInterpolatedPathlossMap() const;

		/**
		 * @brief Check if a shadowing map has been assigned to this station.
		 */
		bool hasShadowingMap() const;

		/**
		 * @brief Return reference to the shadowing map.
		 */
		const scenario::scenerymap::ShadowingMap2D& getShadowingMap() const;

		/**
		 * @brief Return reference to the interpolated shadowing map.
		 */
		const scenario::scenerymap::Interpolation2D& getInterpolatedShadowingMap() const;

		/**
		 * @brief  receive data and forward back to higher layers
		 * receiveData(wns::osi::PDUPtr sdu, wns::Ratio sinr);
		 */
		virtual void receiveData(wns::osi::PDUPtr, wns::Ratio)
		{
		}

		/**
		 * @brief  receive data and forward back to higher layers
		 * receiveData(wns::osi::PDUPtr sdu, wns::Power rxPower, wns::Power interference, wns::Ratio omniAttenuation);
		 * old obsolete interface
		 */
		virtual void receiveData(wns::osi::PDUPtr, wns::Power, wns::Power, wns::Ratio)
		{
		}

		/**
		 * @brief  receive data and forward back to higher layers
		 * receiveData(wns::osi::PDUPtr sdu, wns::service::phy::power::PowerMeasurementPtr rxPowerMeasurementPtr);
		 * new interface [rs]
		 */
		virtual void receiveData(wns::osi::PDUPtr, wns::service::phy::power::PowerMeasurementPtr)
		{
			assure(false,"rise::station::receiveData(wns::osi::PDUPtr, wns::service::phy::power::PowerMeasurementPtr) called accidentially");
		}

	protected:

		/**
		 * @brief Check if the object has been initialized().
		 */
		virtual bool isInitialized() const;

		/**
		 * @brief Vector with pointers to Antennas
		 */
		antenna::Antenna* pd_antenna;

		/**
		 * @brief return pointer to mobility object
		 */
		scenario::mobility::MobilityInterface* getMobility() const;

		//! pyconfig::View of this Station
		wns::pyconfig::View pyConfigView;


		/**
		 * @brief Local Logger
		 */
		//RISELogger log;
		wns::logger::Logger log;

	private:

		/**
		 * @brief True, if station has been initialized().
		 */
		bool initialized;

		/**
		 * @brief Station ID
		 */
		int32_t stationId;

		/**
		 * @brief Disallow copy constructor
		 */
		Station(const Station&);

		/**
		 * @brief Disallow assignment
		 */
		Station& operator=(const Station&);

		void initializePathlossMap();
		void initializeShadowingMap();
		void createAntenna();

		const scenario::scenerymap::PathlossMap2D* pathlossMap;
		const scenario::scenerymap::Interpolation2D* interpolatedPathlossMap;

		const scenario::scenerymap::ShadowingMap2D* shadowingMap;
		const scenario::scenerymap::Interpolation2D* interpolatedShadowingMap;

		scenario::mobility::MobilityInterface* mobility;
	};

}
#endif  // RISE_STATION_HPP


