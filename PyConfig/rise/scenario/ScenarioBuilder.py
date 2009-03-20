###############################################################################
# This file is part of openWNS (open Wireless Network Simulator)
# _____________________________________________________________________________
#
# Copyright (C) 2004-2007
# Chair of Communication Networks (ComNets)
# Kopernikusstr. 5, D-52074 Aachen, Germany
# phone: ++49-241-80-27910,
# fax: ++49-241-80-22242
# email: info@openwns.org
# www: http://www.openwns.org
# _____________________________________________________________________________
#
# openWNS is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License version 2 as published by the
# Free Software Foundation;
#
# openWNS is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################

from openwns.interface import abstractmethod, Interface

class IScenarioBuilder(Interface):

    @abstractmethod
    def createBaseStation(RAP):
        """ Your builder will be asked to create a base station node at position.
        The groupID is used to distinguish certain groups of stations, e.g. horizontally
        and vertically oriented relay enhanced cells."""

    @abstractmethod
    def createRelayEnhancedCell(RelayEnhanceCell):
        """ Your builder will be asked to create a relay node at realyNodePosition.
        The relayNodeGroupID is used to distinguish certain groups of stations, e.g. horizontally
        and vertically oriented relay enhanced cells. In order to allow you to do
        association of relay node with the according base station the baseStationPosition and
        baseStationGroupID is passed to you, too"""

    @abstractmethod
    def createUserTerminal():
        """ Your builder will be asked to create a mobile terminal node. The scenario passes you
        the streets and crossings to allow you to properly setup the mobility. """


    @abstractmethod
    def finalizeScenario():
        """ Finalize the scenario. After the scenario has been built by buildScenarion
        final polishing may take place here."""
