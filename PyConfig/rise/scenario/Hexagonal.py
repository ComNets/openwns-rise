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
import os
#from wns import Position
from openwns.geometry.position import Position, Vector, Line, BoundingBox
from wns.Distribution import Fixed
from wns.PyConfig import attrsetter
from rise.Scenario import ScenarioBase

# Hexagonal Scenarios:
# R=radius=distance from center to farest edge
# H=height=distance from center to nearest edge
# H=R/2*math.srqt(3.0)
def getHexagonHeight(radius):
    return radius*math.srqt(3.0)*0.5

# Wiederverwendungsabstand (Abstand zwischen Basisstationen) im hexagonalen Szenario
def getInterfererDistance(clusterSize, radius=1.0):
    """ Wiederverwendungsabstand """
    # from Walke, "Mobile Radio Networks"
    assert clusterSize > 0
    assert clusterSize in [1,3,4,7,12] # only these numbers give regular patterns
    return math.sqrt(3.0*clusterSize)*radius

def getInterfererAngle(clusterSize):
    """ Correction angle for some clusterSizes """
    # [rs] calculated with some geometrics
    assert clusterSize > 0
    assert clusterSize in [1,3,4,7,12] # only these numbers give regular patterns
    corrAngle = {1:0, 3:math.pi/2, 4:0, 7:0.3334731722518320951, 12:math.pi/6}
    # a(7)=arcsin(sin(120deg)/sqrt(7))
    return corrAngle[clusterSize]

def numberOfBaseStationsForHexagonalScenario(nCircles):
    """ Calculates the number of base stations that are needed to form
        a scenario of hexagonal layout with nCircles circles of cells.
        1 circle means 7 BSs; 2 circles mean 19 BSs
    """
    return (nCircles*(nCircles+1)/2)*6+1

def aequiAngular(n, radius, center, corrAngle = 0.0):
    """ returns n Positions(x,y,z) in a list.
        The positions start with one that is distance=radius away to the North.
        The following positions are ordered counterclockwise.
        corrAngle is in radiant [0..2pi]
    """
    assert n >= 0
    assert 0 <= corrAngle <= 2.0*math.pi # rotates the final result by corrAngle

    res = []
    for i in xrange(n): # 0..n-1
        angle = 2.0  * math.pi / n
        res.append(Position(round(center.x+radius*math.sin(i*angle+corrAngle), 6),
                    round(center.y+radius*math.cos(i*angle+corrAngle), 6),
                    center.z))
    return res

def solveDiophantine(coreCells):
    maxIndex = int(math.floor(math.sqrt(coreCells)))
    for j in xrange(0,maxIndex+1):
        for i in xrange(1,maxIndex+1):
            if (i*i+j*j+i*j == coreCells):
                return [i,j]

def transformHexCoordinates(i,j,gridDistance):
    return Vector(math.sqrt(3)/2.0*j,float(i)+0.5*float(j))*gridDistance

def getFirstPhantomPosition(coreCells,gridDistance):
    """ HexCoordinates: i goes up, j goes 60deg clockwise of i """
    # solve Diophantine equation for positive i,j, wrt i,j<sqrt(3*coreCells)
    # i^2+j^2+i*j == coreCells
    ijResult = solveDiophantine(coreCells)
    i = ijResult[0]
    j = ijResult[1]
    xyVector = transformHexCoordinates(i,j,gridDistance)
    #print "getFirstPhantomPosition(): cc=%d, (i,j)=(%d,%d), (x,y)=(%.3f,%.3f) l=%.3f"%(coreCells,i,j,xyVector.x,xyVector.y,xyVector.length())
    return xyVector

def OLD_hexagonalGrid(nCircles, radius, center, corrAngle = 0):
    """ return list of BS positions
        given the number of nCircles around the center cell.
        radius is in Meters [m]
        center is a Position(x,y,z)
        corrAngle is in radiant [0..2pi]
        corrAngle can be 0 (default) or e.g. math.pi/6.0 ( = 30 degrees)
    """
    assert nCircles >= 0
    assert radius > 0
    assert 0 <= corrAngle <= 2.0*math.pi # rotates the final result by corrAngle

    posList = [center] # central BS position

    # TODO: instead of this "repeat-and-search-if-double" approach
    # we can do it faster
    for i in xrange(nCircles):
        posListLen = len(posList)
        for n in xrange(posListLen):
            for newPos in aequiAngular(6, radius, posList[n], corrAngle):
                alreadyInList = False
                for pos in posList:
                    #if (pos == newPos): # does not work with doubles
                    v = pos-newPos # distance
                    if (v.length2D()<1.0):
                        alreadyInList = True
                        break
                if not alreadyInList:
                    posList.append(newPos)
    #print "Created %d Positions on Grid"%len(posList)
    return posList

def hexagonalGrid(nCircles, radius, center, corrAngle = 0):
    """ return list of BS positions
        given the number of nCircles around the center cell.
        radius is in Meters [m]
        center is a Position(x,y,z)
        corrAngle is in radiant [0..2pi]
        corrAngle can be 0 (default) or e.g. math.pi/6.0 ( = 30 degrees)
    """
    assert nCircles >= 0
    assert radius > 0
    assert 0 <= corrAngle <= 2.0*math.pi # rotates the final result by corrAngle

    posList = [center] # central BS position

    numInnerCells = numberOfBaseStationsForHexagonalScenario(nCircles)
    #print "hexagonalGrid(%d,%d,%d): %d cells"%(nCircles, radius, corrAngle*180.0/math.pi, numInnerCells)
    for circle in xrange(1,nCircles+1): #  xrange(nCircles)=0..nCircles-1
        i=circle; j=0 # start vector
        di=-1; dj=1 # delta vector
        for edge in xrange(6): # 0..5
            for way in xrange(circle):
                v = transformHexCoordinates(i,j,radius)
                p = v.turn2D(corrAngle).toPosition() + center
                posList.append(p)
                #print "c=%d,e=%d,w=%d: ij=[%d,%d] -> xy=(%s) -> (%s)"%(circle,edge,way,i,j,v.toString(),p.toString())
                i=i+di; j=j+dj
            # turn delta vector by 60 degrees right
            odi=di; odj=dj
            di=-odj
            dj=odi+odj

    #print "Created %d Positions on Grid"%len(posList)
    return posList

def hexagonalGridBoundaries(nCircles, radius, center, corrAngle = 0):
    """ return list of boundary lines
        given the number of nCircles around the center cell.
        radius is the cell radius in Meters [m]
        center is a Position(x,y,z)
        corrAngle can be 0 (default) or e.g. 30 [degrees]
    """
    assert 0 <= corrAngle <= 2.0*math.pi # rotates the final result by corrAngle
    lines = []
    posList = hexagonalGrid(nCircles, math.sqrt(3)*radius, center, corrAngle) # BS locations
    smallHexagonCorrAngle = corrAngle + math.pi/6.0 # 30 degree turn
    for pos in posList: # foreach cell
        endPoints = aequiAngular(6, radius, pos, smallHexagonCorrAngle)
        for side in xrange(6):
            line = Line(endPoints[side-1].x,endPoints[side-1].y,endPoints[side].x,endPoints[side].y)
            lines.append(line)
    return lines

def isInHexagon(position, radius, center, corrAngle = 0.0):
    """ returns true if position is located within hexagon boundaries.
        Can be used to correct random placement of UTs within circle!=hexagon
    """
    assert 0 <= corrAngle <= 2.0*math.pi   # rotates hexagon by corrAngle. 0=radius to the right, flat top
    #print "isInHexagon([%s],%d,[%s],%f) ?"%(position.toString(),radius,center.toString(),corrAngle)
    vector = (position-center)/radius      # from center to position; normalized
    length = vector.length2D()
    cos30deg = 0.86602540378443865         # =cos(30deg)=sin(60deg)
    if length>1.0:
        return False
    if length<cos30deg:
        return True
    angle  = vector.angle2D()-corrAngle    # 0=right; pi/2=up; pi=left; -pi/2=down
    angleReduced = angle % (math.pi/3.0)   # in [0..60deg]
    x = length*math.cos(angleReduced)
    y = length*math.sin(angleReduced)
    maxy = (1.0-x)*2.0*cos30deg
    isIn = (y<=maxy)
    #print "isInHexagon([%s],%d,[%s],%f): v=%s, l=%.3f a=%.3fdeg =>%s"%(position.toString(),radius,center.toString(),corrAngle,vector.toString(3),length,angleReduced*180/math.pi,isIn)
    return isIn

def min(a,b):
    if (a<b):
        return a
    else:
        return b

def max(a,b):
    if (a>b):
        return a
    else:
        return b

def minPosition(p1,p2):
    box = BoundingBox()
    if (p1.x<p2.x):
        box.xmin=p1.x
    else:
        return b

def plotScenarioWithGnuplot(positions,directory='positions.junk',grid=[],showLabels=None,title=None):
    box = BoundingBox()
    setupCommands = ''
    labelCommands = ''
    lineCommands  = ''
    setupCommands = setupCommands+'set style line 1 linetype 1 linewidth 4 lc rgb "orange"\n' # for arrows/lines
    setupCommands = setupCommands+'set style line 2 linetype 1 linewidth 4 lc rgb "#101010"\n' # for text
    labelTag=1
    arrowTag=1
    #labelStyle='textcolor rgbcolor "#101010" font "Helvetica,6"'
    labelStyle='textcolor linestyle 2 font "Helvetica,6"'
    arrowStyle='nohead back nofilled linestyle 1'
    positionLines={};
    #stationTypes = ['BS','RN','UT','RUT']
    stationTypes = positions.keys()
    sortedStationTypes = stationTypes
    sortedStationTypes.sort()
    for stationType in sortedStationTypes:
        positionLines[stationType] = ''
        #print "stationType=%s"%stationType
        for pos in positions[stationType]:
            positionLines[stationType] = positionLines[stationType] + pos.toString()+'\n'
            #print "%s %s"%(stationType,pos.toLabeledString())
            if (pos.label != None) and (pos.label != ''):
                label=pos.label
            elif (pos.label == '-'):
                label=''
            else:
                label="%s_{%d}"%(stationType,labelTag)
            if showLabels and (label != '') and (label != '-'):
                labelCommands = labelCommands+'set label %d "%s" at %d,%d,%d'%(labelTag,label,pos.x,pos.y,pos.z)+' centre norotate back %s nopoint offset character 0, -0.7, 0\n'%labelStyle
                # front|back
                labelTag=labelTag+1
            box.extendToIncludePosition(pos)

    for line in grid:
        lineCommands = lineCommands+'set arrow %d from %d,%d,0 to %d,%d,0 %s\n'%(arrowTag,line.x1,line.y1,line.x2,line.y2,arrowStyle)
        box.extendToIncludeLine(line)
        arrowTag=arrowTag+1

    box.roundToNextNiceNumber(100.0)

    #posdir = "positions.junk"
    #if (os.access('positions', os.F_OK)):
    #    os.rename('positions', directory) # move/rename
    if (not os.access(directory, os.F_OK)):
        os.mkdir(directory)
    #for stationType in ['BS','RN','UT','RUT']:
    for stationType in positions.keys():
        posFile = file(directory+'/%s.positions'%stationType,'w');
        posFile.write(positionLines[stationType]);
        posFile.close()
    gnuplotFile = file(directory+'/gnuplot_positions','w')
    gnuplotFile.write('set term postscript enhanced color;set output "positions.ps"\n')
    #gnuplotFile.write('set termoption enhanced\n')

    if title != None:
        title = title.replace('_',' ') # "_" is interpreted as subscript
        gnuplotFile.write('set title "'+title+'"'+"\n")
    gnuplotFile.write("\n")

    gnuplotFile.write(setupCommands+"\n")
    gnuplotFile.write(lineCommands+"\n")
    gnuplotFile.write(labelCommands+"\n")
    gnuplotFile.write('set xrange [%d:%d]\n'%(box.xmin,box.xmax))
    gnuplotFile.write('set yrange [%d:%d]\n'%(box.ymin,box.ymax))
    gnuplotFile.write("\n")

    plotString = 'plot [%03d:%03d][%03d:%03d] '%(box.xmin,box.xmax,box.ymin,box.ymax)
    plotString = plotString+'"BS.positions" using 1:2 with points pt 5 title "BS"'
    if positions.has_key('RN') and len(positions['RN'])>0:
         plotString = plotString+', "RN.positions" using 1:2 with points pt 6 title "RN"'
    if positions.has_key('UT') and len(positions['UT'])>0:
         plotString = plotString+', "UT.positions" using 1:2 with points pt 1 title "UT"'
    if positions.has_key('RUT') and len(positions['RUT'])>0:
         plotString = plotString+', "RUT.positions" using 1:2 with points pt 2 title "RUT"'
    if positions.has_key('P') and len(positions['P'])>0:
         plotString = plotString+', "P.positions" using 1:2 with points pt 3 title "P"'
    if positions.has_key('WP') and len(positions['WP'])>0:
         plotString = plotString+', "WP.positions" using 1:2 with points pt 0 ps 1.0 title "WP"'
    gnuplotFile.write(plotString+"\n")
    gnuplotFile.write("\n")
    #gnuplotFile.write("\npause -1 \"Press a key to continue\"\n")
    gnuplotFile.close()
    # to plot: call:  cd positions.junk; gnuplot gnuplot_positions
    os.system("cd "+directory+"; gnuplot gnuplot_positions");
    # to view: call: kghostview  positions.junk/positions.ps
    print "Scenario plot: kghostview %s"%(directory+"/positions.ps")


class PlotableScenario(ScenarioBase):
    #boundingBox    = None # already in ScenarioBase
    def __init__(self):
        #self.boundingBox = BoundingBox() # already in ScenarioBase
        pass

    #def plotScenarioWithGnuplot(positions,directory='positions.junk',grid=[],showLabels=None,title=None):

    def plotScenarioWithMatlab(positions,directory='positions.junk',grid=[],showLabels=None,title=None):
        """ TODO: implement this """
        assert False

class Wraparound:
    wraparoundClusterSize = None
    wraparoundCircles     = None
    wraparoundCellRadius  = None
    phantomShiftVectors   = None
    coreCells             = None
    gridDistance          = None
    wraparoundCorrAngle   = None
    wraparoundRadius      = None # calculated
    wraparoundStartAngle  = None # calculated

    def __init__(self, clusterSize, nCircles, cellRadius, corrAngle=0.0):
        assert 0 <= corrAngle <= 2.0*math.pi
        self.wraparoundClusterSize = clusterSize
        self.wraparoundCircles     = nCircles
        self.wraparoundCellRadius  = cellRadius
        self.wraparoundCorrAngle   = corrAngle
        self.phantomShiftVectors   = []
        self.gridDistance          = cellRadius*math.sqrt(3.0)
        self.coreCells             = self.wraparoundClusterSize*numberOfBaseStationsForHexagonalScenario(self.wraparoundCircles)
        self.wraparoundRadius      = self.wraparoundCellRadius*math.sqrt(3.0*self.coreCells)
        self.wraparoundStartAngle  = 0.0
        #print "Wraparound.__init__(cs=%d,r=%d,ci=%d): radius=%.3f angle=%.3f"%(self.wraparoundClusterSize,self.wraparoundCellRadius,self.wraparoundCircles,self.wraparoundRadius,self.wraparoundStartAngle)

    def test(self):
        print "Wraparound.test():"
        cellRadius = 1.0
        for nCircles in [0,1,2]:
            for clusterSize in [1,3,4,7,12]:
                coreCells        = clusterSize*numberOfBaseStationsForHexagonalScenario(nCircles)
                wraparoundRadius = cellRadius*math.sqrt(3.0*clusterSize*numberOfBaseStationsForHexagonalScenario(nCircles))
                print "  ci=%d cs=%2d => wr=%6.3f"%(nCircles,clusterSize,wraparoundRadius)
                xyVector = getFirstPhantomPosition(coreCells,1.0)

    def calculateWraparoundShiftVectors(self):
        # first vector:
        xyVector = getFirstPhantomPosition(self.coreCells,self.gridDistance)
        x = xyVector.x
        y = xyVector.y
        self.wraparoundStartAngle=xyVector.angle2D()
        for i in xrange(6):
            angle = i/3.0*math.pi + self.wraparoundStartAngle + self.wraparoundCorrAngle
            x = self.wraparoundRadius*math.cos(angle)
            y = self.wraparoundRadius*math.sin(angle)
            #print "calculateWraparoundShiftVectors(): v[%d] = (%.3f,%.3f), a=%.3f"%(i,x,y,angle)
            self.phantomShiftVectors.append(Vector(x,y,0))

    def getWraparoundShiftVectors(self):
        return self.phantomShiftVectors


class Hexagonal(PlotableScenario,Wraparound):
    clusterSize    = None
    center         = None
    nCircles       = None
    distance_BS_BS = None
    distance_BS_RN = None
    cellRadius     = None
    nRN            = None
    nUT            = None
    totalCorrAngle = None
    rnShiftAngle   = None
    nSectorsBS     = None
    sectorAngle    = None
    nodeHeight     = 5
    positions      = None
    useWraparound  = False # switch on/off

    def __init__(self,
                 clusterSize,
                 center,
                 nCircles,
                 cellRadius,
                 nRN,
                 nUT,
                 distance_BS_BS = None,
                 distance_BS_RN = None,
                 nSectorsBS = 1,
                 corrAngle = 0.0,
                 rnShiftAngle = 0.0,
                 useWraparound = False):
        super(Hexagonal,self).__init__()
        self.clusterSize    = clusterSize
        self.center         = center
        self.nCircles       = nCircles
        self.cellRadius     = cellRadius
        if distance_BS_BS==None:
            distance_BS_BS=self.getInterfererDistance()
        if distance_BS_RN==None:
            distance_BS_RN=cellRadius/2.0
        self.distance_BS_BS = distance_BS_BS
        self.distance_BS_RN = distance_BS_RN
        self.nRN            = nRN
        self.nUT            = nUT
        self.totalCorrAngle = corrAngle
        self.rnShiftAngle   = rnShiftAngle
        self.nSectorsBS     = nSectorsBS
        self.sectorAngle    = math.pi/nSectorsBS
        self.positions      = {}
        self.useWraparound  = useWraparound
        Wraparound.__init__(self,clusterSize,nCircles,cellRadius,corrAngle)
        ScenarioBase.__init__(self)

        self.__createHexagonalScenario()
        labelTag=1
        for stationType in self.positions.keys():
            for pos in self.positions[stationType]:
                pos.label="%s_{%d}"%(stationType,labelTag)
                labelTag=labelTag+1
                self.boundingBox.extendToIncludePosition(pos)

        if self.useWraparound:
            #Wraparound.test(self)
            self.calculateWraparoundShiftVectors()
            phantomPositions = []
            for v in self.getWraparoundShiftVectors():
                phantomPositions.append(Position(v.x,v.y,label='P') + self.center)
            self.positions['P'] = phantomPositions

    # Hexagonal Scenarios:
    # R=radius=distance from center to farest edge
    # H=height=distance from center to nearest edge
    # H=R/2*math.srqt(3.0)
    def getHexagonHeight():
        return self.cellRadius*math.srqt(3.0)*0.5

    # Wiederverwendungsabstand (Abstand zwischen Basisstationen) im hexagonalen Szenario
    def getInterfererDistance():
        """ Reuse distance (Wiederverwendungsabstand) """
        # from Walke, "Mobile Radio Networks"
        assert self.clusterSize > 0
        assert self.clusterSize in [1,3,4,7,12] # only these numbers give regular patterns
        return math.sqrt(3.0*self.clusterSize)*self.cellRadius

    def getInterfererAngle():
        """ Correction angle for some clusterSizes """
        # [rs] calculated with some geometrics
        assert self.clusterSize > 0
        assert self.clusterSize in [1,3,4,7,12] # only these numbers give regular patterns
        corrAngle = {1:0, 3:math.pi/2, 4:0, 7:0.3334731722518320951, 12:math.pi/6}
        # a(7)=arcsin(sin(120deg)/sqrt(7))
        return corrAngle[self.clusterSize]

    def getFixedNodePosition(self, maxradius, center, corrAngle = 0.0):
        """ if we want to switch the mobility off, we always need the UT be be in the good position.
        this function offers such things.
        Returns a list of n Positions(x,y,z) around center,
        where the angles are distributed equidistantly,
        but the distance=radius(UTi) increases between maxradius/n and maxradius.
        Looks like a spiral or snake house.
        """	
        assert self.nUT >= 0
        assert 0 <= corrAngle <= 2.0*math.pi # rotates the final result by corrAngle
        if self.nUT==0:
            return []

        radiusUnit = maxradius/self.nUT
        res = []
        for i in xrange(self.nUT):
            angle = 2.0 * math.pi / self.nUT
            radius = radiusUnit * (i+1)
            res.append(Position(round(center.x+radius*math.sin(i*angle+corrAngle), 6),
                                round(center.y+radius*math.cos(i*angle+corrAngle), 6),
                                center.z))
        return res
    
    def __createHexagonalScenario(self):
        corrAngle = self.totalCorrAngle + getInterfererAngle(self.clusterSize)
        self.positions = {}
        self.positions['BS'] = hexagonalGrid(nCircles  = self.nCircles,
                                             radius    = self.distance_BS_BS,
                                             center    = self.center,
                                             corrAngle = corrAngle)
        self.positions['RN']  = []
        self.positions['UT']  = [] # to be specified externally if nUT=0

        for bsPosition in self.positions['BS']:
            # add UTs:
            limitBSRadiusToHeight = self.cellRadius*0.86602540378443865
            self.positions['UT'].extend(self.getFixedNodePosition(limitBSRadiusToHeight, bsPosition, corrAngle=0))
            # add RNs:
            for rnPosition in aequiAngular(self.nRN, self.distance_BS_RN, bsPosition, self.rnShiftAngle):
                self.positions['RN'].append(rnPosition)
                # add RUTs
                #limitRNRadiusToHeight = radiusRNcell*0.5*math.sqrt(3)
                #positions['RUT'].extend(getFixedNodePosition(nRUT, limitRNRadiusToHeight, rnPosition, corrAngle=0))


    def getPositions(self):
        return self.positions

    def getMobilityObstructions(self):
        return []

    def getCellRadius(self):
        return self.cellRadius

    def buildScenario(self):
        pass

    def finalizeScenario(self):
        pass

class OLD_Hexagonal(ScenarioBase):
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

        
