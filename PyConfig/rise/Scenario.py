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


from wns.Sealed import Sealed
from wns import Interface, abstractmethod
from openwns.geometry.position import Position, Vector, Line, BoundingBox
from wns.PyConfig import attrsetter

class IScenario(Interface):

    @abstractmethod
    def getPositions():
        """ Return the BS and RN positions """

    @abstractmethod
    def getMobilityObstructions():
        """ Return the walls, etc. """

    @abstractmethod
    def getCellRadius():
        """ Return the cell radius """

    @abstractmethod
    def buildScenario():
        """ Start to build the scenario by calling the associated ScenarioBuilder.
        see also: ScenarioBuilder.IScenarioBuilder"""

    @abstractmethod
    def finalizeScenario():
        """ Finalize the scenario. After the scenario has been built by buildScenarion
        final polishing may take place here."""

class ScenarioBase(Sealed, IScenario):

    sizeX = None # obsolete, only for backward compatibility [rs]
    sizeY = None # obsolete, only for backward compatibility [rs]
    boundingBox = None

    #def __init__(self, sizeX, sizeY):
    def __init__(self, xmin=None, xmax=None, ymin=None, ymax=None):
        Sealed.__init__(self)
        IScenario.__init__(self)
        if ymax!=None:
            self.boundingBox = BoundingBox(xmin, xmax, ymin, ymax, 0.0, 0.0)
        else:
            self.boundingBox = BoundingBox() # auto resize
	self.sizeX = (self.boundingBox.xmax-self.boundingBox.xmin)
	self.sizeY = (self.boundingBox.ymax-self.boundingBox.ymin)

    def setBoundingBox(self, xmin, xmax, ymin, ymax, zmin=0.0, zmax=0.0):
        self.boundingBox = BoundingBox(xmin, xmax, ymin, ymax, zmin, zmax)
 	self.sizeX = (self.boundingBox.xmax-self.boundingBox.xmin)
	self.sizeY = (self.boundingBox.ymax-self.boundingBox.ymin)       
    def getXMax(self):
        return self.boundingBox.xmax
    def getYMax(self):
        return self.boundingBox.ymax
    def getXMin(self):
        return self.boundingBox.xmin
    def getYMin(self):
        return self.boundingBox.ymin

class Scenario(ScenarioBase):
    #def __init__(self, sizeX, sizeY):
    def __init__(self, **kw):
        ScenarioBase.__init__(self, **kw)

    def getPositions(self): pass
    def getMobilityObstructions(self): pass
    def getCellRadius(self): pass
    def buildScenario(self): pass
    def finalizeScenario(self): pass
