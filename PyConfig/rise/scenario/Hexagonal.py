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
import random
from openwns import Position
from openwns.distribution import Fixed
from rise.Scenario import ScenarioBase

def aequiAngular(n, radius, center, corrAngle = 0):
    assert n >= 0
    assert 0 <= corrAngle <=359 # rotates the final result by corrAngle

    res = []
    for i in xrange(n):
        angle = 2.0  * math.pi / n
        res.append(Position(round(center.x+radius*math.cos(i*angle+corrAngle*math.pi/180), 6),
                            round(center.y+radius*math.sin(i*angle+corrAngle*math.pi/180), 6),
                            center.z))
    return res

def hexagonalGrid(nCircles, radius, center, corrAngle = 0):
    assert nCircles >= 0
    assert radius > 0

    res = [center]

    for i in xrange(nCircles):
        resLen = len(res)
        for n in xrange(resLen):
            for newPos in aequiAngular(6, radius, res[n], corrAngle):
                alreadyInList = False
                for pos in res:
                    if (pos.x == newPos.x and pos.y == newPos.y and pos.z == newPos.z ):
                        alreadyInList = True
                        break
                if not alreadyInList:
                    res.append(newPos)
    #print "Created ",len(res)," Positions on Grid"
    return res

def numberOfAccessPointsForHexagonalScenario(nCircles):
    """ Calculates the number of access points that are needed to form
        a scenario of hexagonal layout with nCicles circles of cells.
    """
    return (nCircles*(nCircles+1)/2)*6+1



class Hexagonal(ScenarioBase):
    center       = None
    nCircles     = None
    dAP_AP       = None
    dAP_RN       = None
    cellRadius   = None
    nRN          = None
    corrAngle    = None
    rnShiftAngle = None
    cellRadiusRn = None

    bsPositions  = None
    rnPositions  = None
    nSectorsBS  = None
    sectorAngle = None
    nodeHeight  = 5
    def __init__(self,
                 center,
                 nCircles,
                 dAP_AP,
                 dAP_RN,
                 cellRadius,
                 nRN,
                 nSectorsBS = 1,
                 cellRadiusRN = cellRadius,
                 corrAngle = 0,
                 rnShiftAngle = 0
                 ):
        self.center       = center
        self.nCircles     = nCircles
        self.dAP_AP       = dAP_AP
        self.dAP_RN       = dAP_RN
        self.cellRadius   = cellRadius
        self.nRN          = nRN
        self.corrAngle    = corrAngle
        self.rnShiftAngle = rnShiftAngle
        self.nSectorsBS   = nSectorsBS
        self.sectorAngle  = math.pi/nSectorsBS

        self.__createHexagonalScenario()

        xValues = [ pos.x for pos,group in self.bsPositions+self.rnPositions ]
        yValues = [ pos.y for pos,group in self.bsPositions+self.rnPositions ]

        ScenarioBase.__init__(self,
                              sizeX = max(xValues) + self.cellRadius,
                              sizeY = max(yValues) + self.cellRadius)

    def __createHexagonalScenario(self):
        class baseElement:
            bsPos = None
            rnPos = None

            def __init__(self):
                self.bsPos = []
                self.rnPos = []

            def offset(self, xOffset, yOffset):
                bsCopy = [ (Position(bsPos.x + xOffset, bsPos.y + yOffset, bsPos.z), group) for bsPos,group in self.bsPos ]
                rnCopy = [ (Position(rnPos.x + xOffset, rnPos.y + yOffset, rnPos.z), group) for rnPos,group in self.rnPos ]
                return { 'BS' : bsCopy, 'RN' : rnCopy }


        be = baseElement()
        group = 1
        for i in xrange(self.nSectorsBS):
            be.bsPos.append( (Position(0, 0, self.nodeHeight), group))
            group+=1

        for i in xrange(self.nSectorsBS*self.nRN):
            rnAngle =  2*math.pi/(self.nRN * self.nSectorsBS) + self.rnShiftAngle * math.pi/180.0
            be.rnPos.append( (Position(self.dAP_RN*math.cos(rnAngle/2+i*rnAngle), self.dAP_RN*math.sin(rnAngle/2+i*rnAngle), self.nodeHeight), group))
            group+=1

        recPositions = hexagonalGrid(nCircles = self.nCircles,
                                         radius = self.dAP_AP,
                                         center = self.center,
                                         corrAngle = self.corrAngle)
        self.bsPositions = []
        self.rnPositions = []
        for rec in recPositions:
            tmp = be.offset(rec.x, rec.y)
            self.bsPositions += tmp['BS']
            self.rnPositions += tmp['RN']


    def getPositions(self):
        positions = {}
        positions['BS'] = self.bsPositions
        positions['RN'] = self.rnPositions
        return positions

    def getMobilityObstructions(self):
        return []

    def getCellRadius(self):
        return self.cellRadius

    def buildScenario(self):
        pass

    def finalizeScenario(self):
        pass

    def filterNodes(self, nodeType = 'RAP', groupList = [0]):
        """ Filters the nodes for limited simulations, by classifying the node types
        (BS, RN, RAP (both) and the group numbers (list)"""
        if nodeType == "BS":
            self.rnPositions = []
        elif nodeType == "RN":
            self.bsPositions = []
        print groupList
        bs = self.bsPositions
        self.bsPositions = []
        for bspos,group in bs:
            if group in groupList:
                self.bsPositions.append((bspos,group))

        rn = self.rnPositions
        self.rnPositions = []
        for rnpos,group in rn:
            if group in groupList:
                self.rnPositions.append((rnpos,group))

        
