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

import math
from openwns.geometry.position import Position
from openwns.interface import Interface, abstractmethod
import rise.scenario.Nodes

class IDeploymentStrategy(Interface):

    @abstractmethod
    def getPositions(self):
        """ Here you need to implement your deployment """

    @abstractmethod
    def getRelayEnhancedCells(self):
        """ Return the list of RECs """

    @abstractmethod
    def setScenario(self, scenario):
        """ Dependency injection """

    @abstractmethod
    def buildScenario(self, IScenarioBuilder):
        """ Start to build the scenario by calling the IScenarioBuilder implementation.
        see also: ScenarioBuilder.IScenarioBuilder"""

    @abstractmethod
    def finalizeScenario(self, IScenarioBuilder):
        """ Finalize the scenario. After the scenario has been built by buildScenarion
        final polishing may take place here."""

class DeploymentStrategyBase(IDeploymentStrategy):

    def __init__(self):
        super(DeploymentStrategyBase, self).__init__()
        self.scenario = None

    def setScenario(self, scenario):
        self.scenario = scenario

class BackwardCompatibility(DeploymentStrategyBase):

    def __init__(self, name):
        super(BackwardCompatibility, self).__init__()
        self.name = name

    def getPositions(self):
        print "WARNING: BackwardCompatibility deployment strategy used!"

    def getRelayEnhancedCells(self):
        print "WARNING: BackwardCompatibility deployment strategy used!"

    def buildScenario(self):
        print "WARNING: BackwardCompatibility deployment strategy used!"

    def finalizeScenario(self):
        print "WARNING: BackwardCompatibility deployment strategy used!"

class Crossings(DeploymentStrategyBase):

    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "crossings"

    def getPositions(self):
        bspos = [ crossing.position for crossing in self.scenario.crossings if not crossing in self.scenario.getEdgeCrossings() ]
        return { 'BS' : bspos, 'RN' : [] }

class UMTS30_03Relay(DeploymentStrategyBase):

    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "30.03Relay"

    def getPositions(self):
        hrec = rise.scenario.Nodes.RelayEnhancedCell()
        hrec.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 1)
        hrec.addRelayNode(position = Position( - self.scenario.xStepSize , 0, self.scenario.nodeHeight), group = 2)
        hrec.addRelayNode(position = Position(   self.scenario.xStepSize , 0, self.scenario.nodeHeight), group = 3)

        vrec = rise.scenario.Nodes.RelayEnhancedCell()
        vrec.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 2)
        vrec.addRelayNode(position = Position(0,    self.scenario.yStepSize, self.scenario.nodeHeight), group = 3)
        vrec.addRelayNode(position = Position(0, -  self.scenario.yStepSize, self.scenario.nodeHeight), group = 2)

        # Will contain all relay enhanced cells
        recs = []

        reuseDistance = 4 # measured in blocks
        # first calculate positions on the horizontal streets
        firstDx = 1
        bsPerRow = int(math.ceil(float(self.scenario.columns) / float(reuseDistance)))
        for rowNum in xrange(self.scenario.rows+1):
            for bsNum in xrange(bsPerRow):
                x = firstDx + bsNum * reuseDistance # bs positioned at the middle of the xth block
                y = rowNum                          # bs positioned on the yth horizontal street

                if x < self.scenario.columns-1 and x>0:
                    rec = hrec.copy()
                    rec.setOffset(Position((x+1) * self.scenario.xStepSize   - 0.5 * self.scenario.blockWidth,
                                           y    * self.scenario.yStepSize   + self.scenario.yBegin,
                                           0))
                    recs.append(rec)

            firstDx -= 1
            if (firstDx == -1): firstDx = reuseDistance-1

        # second calculate positions on the vertical streets
        firstDy = 3
        bsPerCol = int(math.ceil(float(self.scenario.rows) / float(reuseDistance)))
        for colNum in xrange(self.scenario.columns+1):
            for bsNum in xrange(bsPerCol):
                x = colNum                          # bs positioned on the xth vertical street
                y = firstDy + bsNum * reuseDistance # bs positioned at the middle of the yth block
                if y < self.scenario.rows-1 and y>0:
                    rec = vrec.copy()
                    rec.setOffset(Position(x   * self.scenario.xStepSize  + self.scenario.xBegin,
                                           (y+1)* self.scenario.yStepSize - 0.5*self.scenario.blockHeight,
                                           0))
                    recs.append(rec)

            firstDy -= 1
            if (firstDy == -1): firstDy = reuseDistance-1

        bsPositions = [(rec.baseStation.position, rec.baseStation.group) for rec in recs]
        rnPositions = []
        for rec in recs:
            rnPositions += [ (rn.position, rn.group) for rn in rec.relayNodes ]
        return { 'BS' : bsPositions, 'RN' : rnPositions }


class UMTS30_03(UMTS30_03Relay):

    def __init__(self):
        UMTS30_03Relay.__init__(self)
        self.name = "30.03"

    def getPositions(self):
        positions = UMTS30_03Relay.getPositions(self)
        # Just strip the Relays
        return { 'BS' : positions['BS'], 'RN' : []}

class WINNER_D_6_13_7(DeploymentStrategyBase):

    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "D6.13.7"

    def getRelayEnhancedCells(self):
        leftREC = rise.scenario.Nodes.RelayEnhancedCell()
        leftREC.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 1, azimuth=180)
        leftREC.addRelayNode(position = Position( - self.scenario.xStepSize , 0, self.scenario.nodeHeight), group = 3)
        rightREC = rise.scenario.Nodes.RelayEnhancedCell()
        rightREC.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 1, azimuth=0)
        rightREC.addRelayNode(position = Position(   self.scenario.xStepSize , 0, self.scenario.nodeHeight), group = 4)

        upREC = rise.scenario.Nodes.RelayEnhancedCell()
        upREC.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 2, azimuth=90)
        upREC.addRelayNode(position = Position(0,    self.scenario.yStepSize, self.scenario.nodeHeight), group = 4)
        downREC = rise.scenario.Nodes.RelayEnhancedCell()
        downREC.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 2, azimuth=270)
        downREC.addRelayNode(position = Position(0, -  self.scenario.yStepSize, self.scenario.nodeHeight), group = 3)

        # Will contain all relay enhanced cells
        recs = []

        reuseDistance = 4 # measured in blocks
        # first calculate positions on the horizontal streets
        firstDx = 1
        bsPerRow = int(math.ceil(float(self.scenario.columns) / float(reuseDistance)))
        for rowNum in xrange(self.scenario.rows, -1, -1):
            for bsNum in xrange(bsPerRow):
                x = firstDx + bsNum * reuseDistance # bs positioned at the middle of the xth block
                y = rowNum                          # bs positioned on the yth horizontal street

                if x < self.scenario.columns:
                    rec = leftREC.copy()
                    rec.setOffset(Position((x+1) * self.scenario.xStepSize   - 0.5 * self.scenario.blockWidth,
                                           y    * self.scenario.yStepSize   + self.scenario.yBegin,
                                           0))
                    recs.append(rec)

                    rec = rightREC.copy()
                    rec.setOffset(Position((x+1) * self.scenario.xStepSize   - 0.5 * self.scenario.blockWidth,
                                           y    * self.scenario.yStepSize   + self.scenario.yBegin,
                                           0))
                    recs.append(rec)
            firstDx -= 1
            if (firstDx == -1): firstDx = reuseDistance-1

        # second calculate positions on the vertical streets
        firstDy = 3
        bsPerCol = int(math.ceil(float(self.scenario.rows) / float(reuseDistance)))
        for colNum in xrange(self.scenario.columns, -1 , -1):
            for bsNum in xrange(bsPerCol):
                x = colNum                          # bs positioned on the xth vertical street
                y = firstDy + bsNum * reuseDistance # bs positioned at the middle of the yth block

                if y < self.scenario.rows:
                    rec = upREC.copy()
                    rec.setOffset(Position(x   * self.scenario.xStepSize  + self.scenario.xBegin,
                                           (y+1)* self.scenario.yStepSize - 0.5*self.scenario.blockHeight,
                                           0))
                    recs.append(rec)

                    rec = downREC.copy()
                    rec.setOffset(Position(x   * self.scenario.xStepSize  + self.scenario.xBegin,
                                           (y+1)* self.scenario.yStepSize - 0.5*self.scenario.blockHeight,
                                           0))
                    recs.append(rec)

            firstDy -= 1
            if (firstDy == -1): firstDy = reuseDistance-1

        return recs

    def getPositions(self):
        recs = self.getRelayEnhancedCells()
        bsPositions = [(rec.baseStation.position, rec.baseStation.group) for rec in recs]
        rnPositions = []
        for rec in recs:
            rnPositions += [ (rn.position, rn.group) for rn in rec.relayNodes ]

        return { 'BS' : bsPositions, 'RN' : rnPositions }

    def buildScenario(self, scenarioBuilder):
        recs = self.getRelayEnhancedCells()
        for rec in recs:
            rec.buildScenario(scenarioBuilder)

    def finalizeScenario(self, scenarioBuilder):
        scenarioBuilder.finalizeScenario()

class WINNER_D_6_13_7_SDMA(DeploymentStrategyBase):

    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "D6.13.7_SDMA"

    def getRelayEnhancedCells(self):
        horREC = rise.scenario.Nodes.RelayEnhancedCell()
        horREC.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 1, azimuth=0)
        horREC.addRelayNode(position = Position( - self.scenario.xStepSize , 0, self.scenario.nodeHeight), group = 3)
        horREC.addRelayNode(position = Position(   self.scenario.xStepSize , 0, self.scenario.nodeHeight), group = 4)

        vertREC = rise.scenario.Nodes.RelayEnhancedCell()
        vertREC.setBaseStation(position = Position(0, 0, self.scenario.nodeHeight), group = 2, azimuth=0)
        vertREC.addRelayNode(position = Position(0,    self.scenario.yStepSize, self.scenario.nodeHeight), group = 4)
        vertREC.addRelayNode(position = Position(0, -  self.scenario.yStepSize, self.scenario.nodeHeight), group = 3)

        # Will contain all relay enhanced cells
        recs = []

        reuseDistance = 4 # measured in blocks
        # first calculate positions on the horizontal streets
        firstDx = 2
        bsPerRow = int(math.ceil(float(self.scenario.columns) / float(reuseDistance)))
        for rowNum in xrange(self.scenario.rows, -1, -1):
            for bsNum in xrange(bsPerRow):
                x = firstDx + bsNum * reuseDistance # bs positioned at the middle of the xth block
                y = rowNum                          # bs positioned on the yth horizontal street

                if x < self.scenario.columns:
                    rec = horREC.copy()
                    rec.setOffset(Position((x+1) * self.scenario.xStepSize   - 0.5 * self.scenario.blockWidth,
                                           y    * self.scenario.yStepSize   + self.scenario.yBegin,
                                           0))
                    recs.append(rec)
            firstDx -= 1
            if (firstDx == -1): firstDx = reuseDistance-1

        # second calculate positions on the vertical streets
        firstDy = 2
        bsPerCol = int(math.ceil(float(self.scenario.rows) / float(reuseDistance)))
        for colNum in xrange(self.scenario.columns, -1 , -1):
            for bsNum in xrange(bsPerCol):
                x = colNum                          # bs positioned on the xth vertical street
                y = firstDy + bsNum * reuseDistance # bs positioned at the middle of the yth block

                if y < self.scenario.rows:
                    rec = vertREC.copy()
                    rec.setOffset(Position(x   * self.scenario.xStepSize  + self.scenario.xBegin,
                                           (y+1)* self.scenario.yStepSize - 0.5*self.scenario.blockHeight,
                                           0))
                    recs.append(rec)

            firstDy -= 1
            if (firstDy == -1): firstDy = reuseDistance-1

        return recs

    def getPositions(self):
        recs = self.getRelayEnhancedCells()
        bsPositions = [(rec.baseStation.position, rec.baseStation.group) for rec in recs]
        rnPositions = []
        for rec in recs:
            rnPositions += [ (rn.position, rn.group) for rn in rec.relayNodes ]

        return { 'BS' : bsPositions, 'RN' : rnPositions }

    def buildScenario(self, scenarioBuilder):
        recs = self.getRelayEnhancedCells()
        for rec in recs:
            rec.buildScenario(scenarioBuilder)

    def finalizeScenario(self, scenarioBuilder):
        scenarioBuilder.finalizeScenario()


class Pabst(DeploymentStrategyBase):

    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "Pabst"

    def getPositions(self):
        return {'BS':[], 'RN':[]}

class Schultz(DeploymentStrategyBase):
    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "Schultz"

    def getPositions(self):
        return {'BS':[], 'RN':[]}

class SchultzBS(Schultz):
    def __init__(self):
        DeploymentStrategyBase.__init__(self)
        self.name = "SchultzBS"

    def getPositions(self):
        return {'BS':[], 'RN':[]}
