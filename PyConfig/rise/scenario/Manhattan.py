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

import os
import math

from openwns.geometry.position import Position
from rise.Roadmap import Roadmap, Street, Crossing
import rise.scenario.Shadowing
from rise.Scenario import ScenarioBase
from rise.Mobility import EventListEntry
from rise.scenario.Deployments import IDeploymentStrategy, BackwardCompatibility
from rise.scenario.Nodes import *

#  ID order for crossings and streets:
#  for each row:
#     - 1st crossings
#     - 2nd horizontal streets
#     - 3rd vertical streets
#  are consecutively numbered
#
#   +:crossing --:street
#
#   1       2       3       4       5
#   +-------+-------+-------+-------+
#   |   6   |   7   |   8   |   9   |
#   |       |       |       |       |
#   |10     |11     |12     |13     |14
#   |       |       |       |       |
# 15|     16|     17|     18|     19|
#   +-------+-------+-------+-------+
#   |  20   |  21   |  22   |  23   |
#   |       |       |       |       |
#   |24     |25     |26     |27     |28
#   |       |       |       |       |
# 29|     30|     31|     32|     33|
#   +-------+-------+-------+-------+
#      34      35      36      37
#
# for n*m blocks
#

class Manhattan(ScenarioBase):
    rows = None
    columns = None
    blockWidth = None
    blockHeight = None
    streetWidth = None
    velocity = None
    streets = None
    __edgeCrossings = None
    crossings = None

    idDiffLowerCross = None
    idDiffUpperCross = None
    idDiffRightCross = None
    idDiffLeftCross  = None

    xBegin = None
    yBegin = None
    xStepSize = None
    yStepSize = None

    cellRadius = None

    nodeHeight = None

    deploymentStrategy = None

    scenarioBuilder = None

    mode = None

    sizeX = None
    sizeY = None


    def __init__(self, rows, columns, blockWidth=100, blockHeight=100,
                 streetWidth=10, velocity=30, scenarioBuilder=None, deploymentStrategy=None):
        assert (rows>0 and
                columns>0),"Number of rows and columns have to be > 0"

        ScenarioBase.__init__(self)

        self.rows = rows
        self.columns = columns
        self.blockWidth = blockWidth
        self.blockHeight = blockHeight
        self.streetWidth = streetWidth
        self.velocity = velocity
        self.streets = []
        self.__edgeCrossings = []
        self.crossings = []

        self.idDiffLowerCross = self.columns+1
        self.idDiffUpperCross = -(2*self.columns+1)
        self.idDiffRightCross = -(self.columns)
        self.idDiffLeftCross  = -(self.columns+1)

        streetMid = 0.5 * self.streetWidth
        self.xBegin = streetMid
        self.yBegin = streetMid
        self.xStepSize = self.blockWidth + self.streetWidth
        self.yStepSize = self.blockHeight + self.streetWidth

        self.cellRadius = self.blockWidth
        self.nodeHeight = 10

        # Be backward compatible
        if deploymentStrategy.__class__.__name__ == "str":
            self.deploymentStrategy = BackwardCompatibility(deploymentStrategy)
        else:
            self.deploymentStrategy = deploymentStrategy

        self.__createRoadMap()

        self.deploymentStrategy.setScenario(self)
        self.scenarioBuilder = scenarioBuilder
        if self.scenarioBuilder:
            self.scenarioBuilder.setScenario(self)

        self.sizeX = (self.columns + 1) * self.streetWidth + self.columns * self.blockWidth
        self.sizeY = (self.rows + 1) * self.streetWidth + self.rows * self.blockHeight

        #  upper or lower edge crossings of Manhattan scenario
        #
        #  +---+---+---+---+
        #  |   |   |   |   |
    def __createEdgeCrossings(self, idCounter, positions, idDiffHorizStreet):

        crossings = []

        crossings.append(Crossing(idCounter, positions[0],
                                       [idCounter-self.idDiffLeftCross,
                                        idCounter-idDiffHorizStreet],
                                       [[0.0, 1.0],
                                        [1.0, 0.0]]))
        idCounter = idCounter+1
        for i in range(1,self.columns):
            crossings.append(
                Crossing(idCounter, positions[i],
                         [idCounter-self.idDiffRightCross,
                          idCounter-self.idDiffLeftCross,
                          idCounter-idDiffHorizStreet],
                         [[0.0, 0.5, 0.5],
                          [0.5, 0.0, 0.5],
                          [0.5, 0.5, 0.0]]))
            idCounter = idCounter+1
        crossings.append(Crossing(idCounter, positions[self.columns],
                                       [idCounter-self.idDiffRightCross,
                                        idCounter-idDiffHorizStreet],
                                       [[0.0, 1.0],
                                        [1.0, 0.0]]))

        self.crossings += crossings
        self.__edgeCrossings += crossings

        return idCounter+1


     #  middle part of scenario
     #
     #  |   |   |   |   |
     #  +---+---+---+---+
     #  |   |   |   |   |
     #  +---+---+---+---+
     #  |   |   |   |   |
     #  +---+---+---+---+
     #  |   |   |   |   |

    def __createCrossings(self, idCounter, positions):
        # create left edge
        anEdgeCrossing = Crossing(idCounter, positions[0],
                                  [idCounter-self.idDiffLowerCross,
                                   idCounter-self.idDiffLeftCross,
                                   idCounter-self.idDiffUpperCross],
                                  [[0.0, 0.5, 0.5],
                                   [0.5, 0.0, 0.5],
                                   [0.5, 0.5, 0.0]])
        self.crossings.append(anEdgeCrossing)
        self.__edgeCrossings.append(anEdgeCrossing)

        idCounter = idCounter+1
        for i in range(1,self.columns):
            self.crossings.append(Crossing(idCounter, positions[i],
                                           [idCounter-self.idDiffLowerCross,
                                            idCounter-self.idDiffRightCross,
                                            idCounter-self.idDiffLeftCross,
                                            idCounter-self.idDiffUpperCross],
                                           [[0.0, 0.25, 0.25, 0.5],
                                            [0.25, 0.0, 0.5, 0.25],
                                            [0.25, 0.5, 0.0, 0.25],
                                            [0.5, 0.25, 0.25, 0.0]]))
            idCounter = idCounter+1
        # create right edge
        anEdgeCrossing = Crossing(idCounter, positions[self.columns],
                                  [idCounter-self.idDiffLowerCross,
                                   idCounter-self.idDiffRightCross,
                                   idCounter-self.idDiffUpperCross],
                                  [[0.0, 0.5, 0.5],
                                   [0.5, 0.0, 0.5],
                                   [0.5, 0.5, 0.0]])

        self.crossings.append(anEdgeCrossing)
        self.__edgeCrossings.append(anEdgeCrossing)

        return idCounter+1

    def __createHorizStreets(self, idCounter, positions):
        for i in range(self.columns):
            self.streets.append(Street(idCounter,
                                       [positions[i], positions[i+1]],
                                       self.velocity,
                                       [idCounter+self.idDiffLeftCross ,
                                        idCounter+self.idDiffRightCross]))
            idCounter = idCounter+1
        return idCounter



    def __createVertStreets(self, idCounter, upperPos, lowerPos):
        for i in range(self.columns+1):
            self.streets.append(Street(idCounter,
                                       [upperPos[i], lowerPos[i]],
                                       self.velocity,
                                       [idCounter+self.idDiffUpperCross ,
                                        idCounter+self.idDiffLowerCross]))
            idCounter = idCounter+1
        return idCounter


    # compute coordinates of crossings of row rowNum
    def __createRowCoordinates(self, rowNum):
        positions = []
        yCoord = rowNum*self.yStepSize + self.yBegin
        for i in range(self.columns+1):
            positions.append(Position(x = self.xBegin + i * self.xStepSize,
                                      y = yCoord,
                                      z = 0.0))
        return positions



    def __createRoadMap(self):
        idCounter = 1
        # upper edge

        upperPos = self.__createRowCoordinates(0)
        lowerPos = self.__createRowCoordinates(1)

        #create upper edge
        idCounter = self.__createEdgeCrossings(idCounter,
                                             upperPos,
                                             self.idDiffUpperCross)
        idCounter = self.__createHorizStreets(idCounter, upperPos)

        #create middle rows
        idCounter = self.__createVertStreets(idCounter, upperPos, lowerPos)
        upperPos = lowerPos
        for i in range(1,self.rows):
            lowerPos = self.__createRowCoordinates(i+1)
            idCounter = self.__createCrossings(idCounter, upperPos)
            idCounter = self.__createHorizStreets(idCounter, upperPos)
            idCounter = self.__createVertStreets(idCounter,upperPos,lowerPos)
            upperPos = lowerPos
        #create lower edge
        idCounter = self.__createEdgeCrossings(idCounter,
                                             upperPos,
                                             self.idDiffLowerCross)
        idCounter = self.__createHorizStreets(idCounter, upperPos)
        return  self.streets,self.crossings



    def getEdgeCrossings(self):
        return self.__edgeCrossings

    def createXFigFile(self):
        filename = ("manhattan"
                    +str(self.rows)+"x"+str(self.columns)
                    +".fig")
        if (os.path.exists(filename)):
            os.remove(filename)
        file = open(filename,"a")
        header = "#FIG 3.2\n" + "Landscape\n" + "Center\n" + "Metric\n" + "A4      \n" + "100.00\n" +        "Single\n"+        "-2\n"+        "1200 2\n"
        file.write(header)

        # We are using the Metric system. 1cm corresponds
	# to 450 Points in the XFig Coordinates.
	# Since we need the output in m, there is the additional
	# factor of 0.1 because 1m = 0.1 x 10m
	scale=45.000;

        for i in self.streets:
            x1 = int(i.endPoints[0].x * scale)
            x2 = int(i.endPoints[1].x * scale)
            y1 = int(i.endPoints[0].y * scale)
            y2 = int(i.endPoints[1].y * scale)

            file.write("2 1 0 3 0 7 50 -1 -1 0.000 0 0 7 0 0 2\n")
            file.write("\t" + str(x1) + " " + str(y1) + " " + str(x2) + " " + str(y2) + "\n")

        file.close()

    def createRoadmapFile(self):
        filename = ("manhattan"
                    +str(self.rows)+"x"+str(self.columns)
                    +".roadmap")
        os.remove(filename)
        file = open(filename,"a")
        file.write("crossings = [\n")
        for i in self.crossings:
            file.write("["
                       +str(i.ID)+", "
                       +str(i.position.x)+", "
                       +str(i.position.y)+",  "
                       +str(i.connections)+", "
                       +str(i.probMatrix)+"], \n")
        file.write( "]\n \n")
        file.write("streets = [\n")
        for i in self.streets:
            file.write("["+str(i.ID)+",   "
                       +str(i.endPoints[0].x)+",  "
                       +str(i.endPoints[0].y)+",   "
                       +str(i.endPoints[1].x)+",   "
                       +str(i.endPoints[1].y)+",   "
                       +str(i.vMax)+", "
                       +str(i.connections)+"],\n")
        file.write("] \n")
        file.close()
        print self.idDiffUpperCross
        print self.idDiffLeftCross
        print self.idDiffRightCross
        print self.idDiffLowerCross

    def getMobilityObstructions(self):
        tmp = self.getShadowing(0,1)
        return tmp.obstructionList

    def getShadowing(self, wallAttenuation, smoothingSteps):
        objs = []
        sizeX = (self.columns + 1) * self.streetWidth + self.columns * self.blockWidth
        sizeY = (self.rows + 1) * self.streetWidth + self.rows * self.blockHeight

        for yy in xrange(self.rows):
            uly = self.streetWidth * (1+yy) + yy * self.blockHeight
            lry = uly + self.blockHeight
            for xx in xrange(self.columns):
                ulx = self.streetWidth * (1+xx ) + xx * self.blockWidth
                lrx = ulx + self.blockWidth

                nSteps = smoothingSteps
                offset = 0.0
                for step in xrange(nSteps):
                    offset_old = 0.0
                    offset = offset_old + float(step)/(2*nSteps - step) * (nSteps - offset_old)
                    delta_offset_percent = (offset - offset_old) / nSteps
                    attenuation = delta_offset_percent * wallAttenuation

                    objs.append(
                        rise.scenario.Shadowing.AxisParallelRectangle(Position(ulx+offset, uly+offset, 0.0),
                                                                      Position(lrx-offset, lry-offset, 0.0),
                                                                      attenuation = "%d dB" % attenuation))

        return rise.scenario.Shadowing.Objects(obstructionList = objs)

    def getRelayEnhancedCells(self):
        return self.deploymentStrategy.getRelayEnhancedCells()

    def buildScenario(self):
        return self.deploymentStrategy.buildScenario(self.scenarioBuilder)

    def finalizeScenario(self):
        return self.deploymentStrategy.finalizeScenario(self.scenarioBuilder)

    def getPositions(self):
        return self.deploymentStrategy.getPositions()

        if self.deploymentStrategy.name == "crossings":
            bspos = [ crossing.position for crossing in self.crossings if not crossing in self.__edgeCrossings ]
            return { 'BS' : bspos, 'RN' : [] }
        elif self.deploymentStrategy.name == "30.03":
            tmp = self.__getRelayScenarioPositions(deployment='30.03')
            tmp.pop('RN')
            tmp['RN'] = []
            return tmp
        elif self.deploymentStrategy.name == "30.03Relay":
            return self.__getRelayScenarioPositions(deployment='30.03')
        elif self.deploymentStrategy.name == "N=1":
            return self.__getRelayScenarioPositions(clusterReuse = 1)
        elif self.deploymentStrategy.name == "N=2":
            return self.__getRelayScenarioPositions(clusterReuse = 2)
        elif self.deploymentStrategy.name == "N=3":
            return self.__getRelayScenarioPositions(clusterReuse = 3)
        elif self.deploymentStrategy.name == "Pabst":
            return self.__getRelayScenarioPositions(deployment="Pabst")
        elif self.deploymentStrategy.name == "Schultz":
            return self.__getRelayScenarioPositions(clusterReuse = 3, deployment="Schultz")
        elif self.deploymentStrategy.name == "SchultzBS":
            tmp = self.__getRelayScenarioPositions(clusterReuse = 3, deployment="Schultz")
            tmp.pop('RN')
            tmp['RN'] = []
            return tmp
        elif self.deploymentStrategy.name == "D6.13.7":
            return self.__getRelayScenarioPositions(deployment="D6.13.7")
        else:
            raise "Unknown positioning option '"+self.deploymentStrategy+"' specified."

    def __getRelayScenarioPositions(self, clusterReuse = 1, deployment="Esseling"):

        class BaseElement:
            bsPos = None
            rnPos = None

            def __init__(self):
                self.bsPos = []
                self.rnPos = []

            def offset(self, xOffset, yOffset):
                bsCopy = [ (Position(bsPos.x + xOffset, bsPos.y + yOffset, bsPos.z), group) for bsPos,group in self.bsPos ]
                rnCopy = [ (Position(rnPos.x + xOffset, rnPos.y + yOffset, rnPos.z), group) for rnPos,group in self.rnPos ]
                return { 'BS' : bsCopy, 'RN' : rnCopy }

        bsPos = []
        rnPos = []

        # Prepare the BaseElement ( a 4-Relay REC )
        be = BaseElement()
        be.bsPos = [ ( Position(0, 0, self.nodeHeight), 1 )]
        be.rnPos = [ (Position(   self.xStepSize , 0, self.nodeHeight),2),
                     (Position( - self.xStepSize , 0, self.nodeHeight),3),
                     (Position( 0,  self.yStepSize , self.nodeHeight), 4),
                     (Position( 0,- self.yStepSize , self.nodeHeight), 5)
                     ]

        if deployment == '30.03':
            hrec = BaseElement()
            group = 1
            hrec.bsPos = [(Position(0, 0, self.nodeHeight),group) ]
            hrec.rnPos = [(Position( - self.xStepSize , 0, self.nodeHeight),2),
                          (Position(   self.xStepSize , 0, self.nodeHeight),3),
                         ]
            vrec = BaseElement()
            group+=1
            vrec.bsPos = [(Position(0, 0, self.nodeHeight),group) ]
            vrec.rnPos = [( Position( 0,  self.yStepSize , self.nodeHeight), 3),
                          ( Position( 0,- self.yStepSize , self.nodeHeight), 2),
                         ]
            reuseDistance = 4 # measured in blocks
            # first calculate positions on the horizontal streets
            firstDx = 1
            bsPerRow = int(math.ceil(float(self.columns) / float(reuseDistance)))
            for rowNum in xrange(self.rows+1):
                for bsNum in xrange(bsPerRow):
                    x = firstDx + bsNum * reuseDistance # bs positioned at the middle of the xth block
                    y = rowNum                          # bs positioned on the yth horizontal street
                    tmp = hrec.offset(xOffset = (x+1) * self.xStepSize   - 0.5 * self.blockWidth,
                                      yOffset =  y    * self.yStepSize   + self.yBegin)
                    if x < self.columns-1 and x>0:
                        bsPos += tmp['BS']
                        rnPos += tmp['RN']
                firstDx -= 1
                if (firstDx == -1): firstDx = reuseDistance-1

            # second calculate positions on the vertical streets
            firstDy = 3
            bsPerCol = int(math.ceil(float(self.rows) / float(reuseDistance)))
            for colNum in xrange(self.columns+1):
                for bsNum in xrange(bsPerCol):
                    x = colNum                          # bs positioned on the xth vertical street
                    y = firstDy + bsNum * reuseDistance # bs positioned at the middle of the yth block
                    tmp = vrec.offset(xOffset =  x   * self.xStepSize  + self.xBegin,
                                      yOffset = (y+1)* self.yStepSize - 0.5*self.blockHeight)
                    if y < self.rows-1 and y>0:
                        bsPos += tmp['BS']
                        rnPos += tmp['RN']
                firstDy -= 1
                if (firstDy == -1): firstDy = reuseDistance-1
            return { 'BS' : bsPos, 'RN' : rnPos }
                 
        if deployment == 'D6.13.7':
            hrec = BaseElement()
            group = 1
            hrec.bsPos = [(Position(0, 0, self.nodeHeight),group) ]
            hrec.rnPos = [(Position( - self.xStepSize , 0, self.nodeHeight),3),
                          (Position(   self.xStepSize , 0, self.nodeHeight),4),
                         ]
            vrec = BaseElement()
            group=2
            vrec.bsPos = [(Position(0, 0, self.nodeHeight),group) ]
            vrec.rnPos = [( Position( 0,  self.yStepSize , self.nodeHeight), 4),
                          ( Position( 0,- self.yStepSize , self.nodeHeight), 3),
                         ]
            reuseDistance = 4 # measured in blocks
            # first calculate positions on the horizontal streets
            firstDx = 1
            bsPerRow = int(math.ceil(float(self.columns) / float(reuseDistance)))
            for rowNum in xrange(self.rows, -1, -1):
                #print rowNum
                for bsNum in xrange(bsPerRow):
                    x = firstDx + bsNum * reuseDistance # bs positioned at the middle of the xth block
                    y = rowNum                          # bs positioned on the yth horizontal street
                    tmp = hrec.offset(xOffset = (x+1) * self.xStepSize   - 0.5 * self.blockWidth,
                                      yOffset =  y    * self.yStepSize   + self.yBegin)
                    if x < self.columns:
                        bsPos += tmp['BS']
                        rnPos += tmp['RN']
                firstDx -= 1
                if (firstDx == -1): firstDx = reuseDistance-1

            # second calculate positions on the vertical streets
            firstDy = 3
            bsPerCol = int(math.ceil(float(self.rows) / float(reuseDistance)))
            for colNum in xrange(self.columns, -1 , -1):
                #print colNum
                for bsNum in xrange(bsPerCol):
                    x = colNum                          # bs positioned on the xth vertical street
                    y = firstDy + bsNum * reuseDistance # bs positioned at the middle of the yth block
                    tmp = vrec.offset(xOffset =  x   * self.xStepSize  + self.xBegin,
                                      yOffset = (y+1)* self.yStepSize - 0.5*self.blockHeight)
                    if y < self.rows:
                        bsPos += tmp['BS']
                        rnPos += tmp['RN']
                firstDy -= 1
                if (firstDy == -1): firstDy = reuseDistance-1
            return { 'BS' : bsPos, 'RN' : rnPos }

        if deployment == 'Esseling':
            # compose cluster from baseElements
            cluster = BaseElement()
            for c in xrange(clusterReuse):
                tmp = be.offset( c * 3 * self.xStepSize,
                                 c * 3 * self.yStepSize )
                cluster.bsPos += tmp['BS']
                cluster.rnPos += tmp['RN']

                # Now calculate the offsets and place copies of the baseElement
                numxRECs = self.columns / (3 * clusterReuse)
                numyRECs = self.rows    / (3 * clusterReuse)

                for x in xrange(numxRECs):
                    for y in xrange(numyRECs):

                        vroad = (x*(3*clusterReuse) + 2) # 2 is the initial Offset
                        hroad = (y*(3*clusterReuse) + 2) # 2 is the initial Offset
                        xOffset = vroad * self.xStepSize + self.xBegin
                        yOffset = hroad * self.yStepSize + self.yBegin

                        copy = cluster.offset(xOffset,yOffset)
                        bsPos += copy['BS']
                        rnPos += copy['RN']
        elif deployment == 'Pabst':
            reuseDistance = 3 # measured in blocks
            # first calculate positions on the horizontal streets
            firstDx = 1
            bsPerRow = int(math.ceil( float(self.rows) / float(reuseDistance) ))
            for rowNum in xrange(self.rows+1):
                for bsNum in xrange(bsPerRow):
                    x = firstDx + bsNum * reuseDistance # bs positioned at the xth vertical crossing
                    y = rowNum                          # bs positioned on the yth horizontal street
                    if x<= self.rows:
                        pos = (Position(x =  x   * self.xStepSize + self.xBegin,
                                       y =  y   * self.yStepSize + self.yBegin,
                                       z = self.nodeHeight),1)
                        rnPos.append(pos)
                firstDx -= 1
                if (firstDx == -1): firstDx = reuseDistance-1
        elif deployment == 'Schultz':
            rec1 = BaseElement()
            rec2 = BaseElement()
            rec3 = BaseElement()

            rec = [rec1, rec2, rec3]
            rec[2].bsPos = [(Position(0, 0, self.nodeHeight),2) ]
            rec[2].rnPos = [(Position( - self.xStepSize , 0, self.nodeHeight),3),
                         (Position( 0,  self.yStepSize , self.nodeHeight),1),
                         (Position(   self.xStepSize , 0, self.nodeHeight),1),
                         (Position( 0,- self.yStepSize , self.nodeHeight),3)
                         ]
            rec[1].bsPos = [(Position(0, 0, self.nodeHeight),1) ]
            rec[1].rnPos = [(Position( - self.xStepSize , 0, self.nodeHeight),2),
                         (Position( 0,  self.yStepSize , self.nodeHeight),3),
                         (Position(   self.xStepSize , 0, self.nodeHeight),3),
                         (Position( 0,- self.yStepSize , self.nodeHeight),2)
                         ]
            rec[0].bsPos = [(Position(0, 0, self.nodeHeight),3) ]
            rec[0].rnPos = [(Position( - self.xStepSize , 0, self.nodeHeight),1),
                         (Position( 0,  self.yStepSize , self.nodeHeight),2),
                         (Position(   self.xStepSize , 0, self.nodeHeight),2),
                         (Position( 0,- self.yStepSize , self.nodeHeight),1)
                         ]

            recBsDistance = 5
            firstDy = 3
            firstRecType = 0
            bsPerCol = int(math.ceil( float(self.columns) / float(recBsDistance) ))
            for colNum in xrange(1, self.columns):
                dy = firstDy
                recType = firstRecType
                for nREC in xrange(bsPerCol):
                    if dy <= self.rows-1:
                        tmp = rec[recType].offset(colNum * self.xStepSize  + 0.5*self.streetWidth,
                                             dy * self.yStepSize  + 0.5*self.streetWidth)
                        bsPos += tmp['BS']
                        rnPos += tmp['RN']
                        recType = (recType+1)%3
                        dy += recBsDistance
                firstDy = firstDy - 2
                firstRecType = (firstRecType+1)%2
                if colNum%5 == 0:
                    rec.append(rec[0])
                    rec.pop(0)
                    firstRecType = (firstRecType+1)%2
                if (firstDy < 1):
                    firstDy += recBsDistance
        else:
            raise "Unknown deployment option "+deployment+" specified."



        return { 'BS' : bsPos, 'RN' : rnPos }

    def getCellRadius(self):
        return self.cellRadius


    def getEventList(self, resolution, timestep):
        evList = []
        obstructions = self.getMobilityObstructions()

        time = 0.0
        for yy in xrange(self.sizeY / resolution):
            for xx in xrange(self.sizeX / resolution):
                position = Position(x= xx*resolution + 0.5*resolution,
                                    y= yy*resolution + 0.5*resolution,
                                    z= 1.5)
                obstructed = False
                for object in obstructions:
                    if object.containsPoint(position):
                        obstructed = True
                        break
                if not obstructed:
                    evList.append( EventListEntry(position, time) )
                    time += timestep

        return evList

class Manhattan_D6_13_7(Manhattan):
    #
    # Creating a scenario after the document D6.13.7
    #
    def __init__(self, rows=12, columns=11, blockWidth=200, blockHeight=200,
                 streetWidth=30, velocity=30, scenarioBuilder=None, deploymentStrategy=None):
        super(Manhattan_D6_13_7,self).__init__(rows, columns, blockWidth, blockHeight,
                                               streetWidth, velocity, scenarioBuilder, deploymentStrategy)
        #print "Creating Manhattan_D6_13_7"

