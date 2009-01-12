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

from openwns.logger import Logger
from openwns import Position

class Roadmap(object):
    name = None
    streets = None
    crossings = None
    logger = None
    
    def __init__(self, name, streets, crossings):
        """Reads the .roadmap-style description of the roadmap"""
        self.name = name
        self.streets = streets
        self.crossings = crossings
        self.logger = Logger("RISE Mobility", "Roadmap Class", True)


def readFromFile(name):
    retVal = {}
    execfile(name,retVal)
    streets = []
    for obj in retVal['streets']:
        streets.append(Street(ID = obj[0],
                              endPoints = [Position(x=obj[1], # Position1
                                                    y=obj[2],
                                                    z=0.0),
                                           Position(x=obj[3], # Position2
                                                    y=obj[4],
                                                    z=0.0)],
                              vMax = obj[5], # connections
                              connections = obj[6] # vMax
                              ))

    crossings = []
    for obj in retVal['crossings']:
        crossings.append(Crossing(ID = obj[0],
                                  position = Position(x=obj[1],
                                                      y=obj[2],
                                                      z=0.0),
                                  connections = obj[3],
                                  probMatrix = obj[4]
                                  ))
    return streets, crossings
        

class MapObject(object):
    ID = None
    connections = None
    
    def __init__(self, ID, connections):
        self.ID = ID
        assert isinstance(connections, list)
        self.connections = connections

class Street(MapObject):
    endPoints = None
    vMax = None

    def __init__(self, ID, endPoints, vMax, connections):
        super(Street, self).__init__(ID, connections)
        self.endPoints = endPoints
        assert len(self.endPoints)==2, "You must specify two endpoints!"
        assert len(self.connections)==2, "A street can only be connected to 2 objects!"
        assert isinstance(self.endPoints[0], Position), "Invalid Position"
        assert isinstance(self.endPoints[1], Position), "Invalid Position"
        self.vMax = vMax

class Crossing(MapObject):
    position = None
    probMatrix = None

    def __init__(self, ID, position, connections, probMatrix):
        super(Crossing, self).__init__(ID, connections)
        self.position = position
        assert isinstance(self.position, Position), "Ivalid Position"
        self.probMatrix = probMatrix
        assert len(self.probMatrix)==len(self.connections),"improper probability matrix for crossing "+str(self.ID)
        for i in xrange(len(connections)):
            assert len(self.probMatrix[i])==len(connections),"improper probability matrix for crossing "+str(self.ID)

