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

from openwns import Position

class RAP:

    def __init__(self):
        self.node = None
        self.group = None
        self.position = None
        self.azimuth = None

    def setOffset(self, offset):
        self.position.x += offset.x
        self.position.y += offset.y
        self.position.z += offset.z

class RelayEnhancedCell:

    def __init__(self):
        self.baseStation = None
        self.relayNodes = []
        self.offset = Position(0,0,0)

    def setBaseStation(self, position, group, azimuth):
        assert self.baseStation == None, "You can only set the Base Station once."
        # Ask the Scenario Builder for a Base Station Node
        # The Scenario Builder is implemented by the user specific for each simulation campaign
        self.baseStation = RAP()
        self.baseStation.position = Position(position.x, position.y, position.z)
        self.baseStation.group = group
        self.baseStation.azimuth = azimuth

    def addRelayNode(self, position, group):
        assert self.baseStation, "You must set the base station first."
        relayNode = RAP()
        relayNode.position = Position(self.baseStation.position.x,self.baseStation.position.y,self.baseStation.position.z)
        relayNode.position.x += position.x
        relayNode.position.y += position.y
        relayNode.position.z += position.z
        relayNode.group = group
        self.relayNodes.append(relayNode)

    def buildScenario(self, scenarioBuilder):
        scenarioBuilder.createRelayEnhancedCell(self)
#        node = scenarioBuilder.createBaseStation(self.baseStation.position, self.baseStation.group)
#        self.baseStation.node = node
#        for rn in self.relayNodes:
#            node = scenarioBuilder.createRelayNode(rn.position, self.baseStation.position, rn.group, self.baseStation.group)
#            rn.node = node

    def setOffset(self, offset):
        self.baseStation.setOffset(offset)
        for rn in self.relayNodes:
            rn.setOffset(offset)

    def copy(self):
        theCopy = RelayEnhancedCell()
        theCopy.offset = self.offset
        theCopy.setBaseStation(self.baseStation.position, self.baseStation.group, self.baseStation.azimuth)
        for rn in self.relayNodes:
            theCopy.addRelayNode(rn.position, rn.group)
        return theCopy
