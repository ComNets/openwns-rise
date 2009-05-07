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

from openwns.distribution import Normal
from openwns.logger import Logger
from openwns import Position
import openwns.node
import rise.Roadmap
import random
import math


class MobilityBase(object):
    nameInMobilityFactory = None
    coords       = None
    moveTimeStep = None
    logger = None
    obstructionList = None # list of no-go areas :-)

    def __init__(self, coords=Position()):
        self.coords = coords
        self.moveTimeStep = 0.01
        self.obstructionList = []

    def setCoords(self, coords):
        self.coords = coords
        self.checkBounds()

    def getCoords(self):
        return self.coords


class Mobility(MobilityBase):
     userVelocityDist = None

     def __init__(self, coords=Position()):
          super(Mobility, self).__init__(coords)
          # Mobility is to be specified in m/s
          self.userVelocityDist = Normal(mean = 3.0, variance = 1.0)


class No(MobilityBase):
     nameInMobilityFactory = "rise.mobility.None"
     def __init__(self, coords):
          super(No, self).__init__(coords)
          self.logger=Logger("RISE Mobility","None",True)
     def checkBounds(self):
               """do nothing, here you could check if the  coords are inside the scenario"""

class MobilityDropin(No):
     def __init__(self):
          super(MobilityDropin,self).__init__(Position(0,0))

class EventListEntry:
     position = None
     time = None
     def __init__(self, position, time):
        self.position = position
        self.time = time

class EventList(MobilityBase):
     nameInMobilityFactory = "rise.mobility.EventList"

     events = None

     def __init__(self, coords):
        super(EventList, self).__init__(coords)
        self.logger=Logger("RISE Mobility","EventList",True)
        self.events = []

     def addWaypoint(self, time, position):
        self.events.append(EventListEntry(position, time))

     def checkBounds(self):
            """do nothing, here you could check if the  coords are inside the scenario"""

class BrownianRect(Mobility):
     nameInMobilityFactory = "rise.mobility.BrownianRect"
     xmin = None
     ymin = None
     xmax = None
     ymax = None

     def __init__(self, boundaries, obstructions = []):
        super(BrownianRect, self).__init__()
        """The boundaries have to be a set with the following entries:
        boundaries[0] = xmin
        boundaries[1] = ymin
        boundaries[2] = xmax
        boundaries[3] = ymax
        """
        assert (len(boundaries) == 4),"please specify 4 coordinates for scenario corners: xmin,ymin,xmax,ymax"
        self.logger = Logger("RISE Mobility","BrownianRect",True)
        self.xmin = boundaries[0]
        self.ymin = boundaries[1]
        self.xmax = boundaries[2]
        self.ymax = boundaries[3]
        self.obstructionList = obstructions
        while True:
            self.setCoords(self.getRandomPosition())
            if self.checkBounds():
                break
        

     def getRandomPosition(self):
         return Position(round(self.xmin + random.random()*(self.xmax-self.xmin) , 6),
                         round(self.ymin + random.random()*(self.ymax-self.ymin) , 6),
                         1.5)

     def checkBounds(self):
         if not (self.xmin < self.coords.x < self.xmax):
             raise("x-Coordinate "+ str(self.coords.x) +" out of bounds ["+str(self.xmin)+","+str(self.xmax)+"]")
         if not (self.ymin < self.coords.y < self.ymax):
             raise("y-Coordinate "+ str(self.coords.y) +" out of bounds ["+str(self.ymin)+","+str(self.ymax)+"]")

         positionObstructed = False
         for obstruction in self.obstructionList:
             if obstruction.containsPoint([ self.coords.x, self.coords.y, self.coords.z ]):
                 positionObstructed = True

         return not positionObstructed

class BrownianCirc(Mobility):
     nameInMobilityFactory = "rise.mobility.BrownianCirc"
     center = None # center of the permitted circular area
     maxDistance = None # radius of the permitted circular area

     def __init__(self, center, maxDistance, obstructions = []):
        super(BrownianCirc, self).__init__()
        self.center = center
        self.maxDistance = maxDistance
        self.obstructionList = obstructions
        while True:
            self.setCoords(self.getRandomPosition())
            if self.checkBounds():
                break
        self.logger = Logger("RISE Mobility","BrownianCirc",True)

     def getRandomPosition(self):
        angle = 2*math.pi * random.random()
        radius = self.maxDistance * math.sqrt(random.random())
        return Position(round(self.center.x+radius*math.sin(angle), 6),
                round(self.center.y+radius*math.cos(angle), 6),
                1.5)

     def checkBounds(self):
        myCoords = self.getCoords()
        distance  = math.sqrt((self.center.x-myCoords.x)**2 +
                (self.center.y-myCoords.y)**2 )
        assert (distance <= self.maxDistance), "Coordinate out of maxDistance! (actual = %.2f m, target = %.2f m)" % (distance, self.maxDistance)

        if myCoords.x <= 0 or myCoords.y <=0:
            return False

            positionObstructed = False
            for obstruction in self.obstructionList:
                if obstruction.containsPoint([ self.coords.x, self.coords.y, self.coords.z ]):
                    positionObstructed = True

            return not positionObstructed


class BrownianEquiangularPolygon(BrownianCirc):
     nameInMobilityFactory = "rise.mobility.BrownianEquiangularPolygon"
     corners = None
     angle = None

     def __init__(self, center, maxDistance, corners, angle, obstructions = []):
        self.corners = corners
        self.angle = angle
        super(BrownianEquiangularPolygon, self).__init__(center, maxDistance)
        self.logger = Logger("RISE Mobility","BrownianEquiangularPolygon",True)

     def getRandomPosition(self):
        angle = 2*math.pi * random.random()
        radius = self.maxDistance * math.sqrt(random.random()) * math.cos(math.pi/self.corners)
        return Position(round(self.center.x+radius*math.sin(angle), 6),
              round(self.center.y+radius*math.cos(angle), 6),
              1.5)

class Roadmap(Mobility):
     nameInMobilityFactory = "rise.mobility.Roadmap"
     roadMap = None

     def __init__(self, name, streets, crossings):
        super(Roadmap, self).__init__()
        self.roadMap = rise.Roadmap.Roadmap(name, streets, crossings)
        self.logger = Logger("RISE Mobility","Roadmap",True)

class Component( openwns.node.Component ):
     mobility = None

     def __init__(self, node, name, mobility):
        super(Component, self).__init__(node,name)
        self.nameInComponentFactory = "rise.mobility.Component"
        self.mobility = mobility
class ThereAndBack(EventList):
    # waitTimeAtEndPoint timeperiod to wait at an endpoint in secoonds.
    def __init__(self, firstPoint, secondPoint, velocity, endTime, waitTimeAtEndPoint=0, startToMoveTime = 0.0,  stepsPerSecond=1000):
        rise.Mobility.EventList.__init__(self, firstPoint)
        # Velocity is in km/h
        v = float( float(velocity) / 3.6)
        distance = self._distance(firstPoint, secondPoint)

        timeNow = startToMoveTime + waitTimeAtEndPoint
        stepTime = float( 1.0 / float(stepsPerSecond))
        stepBack = self._vectorScalMul(self._vectorScalDiv(self._vectorSub(firstPoint, secondPoint), distance), v)
        stepThere = self._vectorScalMul(self._vectorScalDiv(self._vectorSub(secondPoint, firstPoint), distance), v)
        start = firstPoint
        end = secondPoint
        pos = start
        while timeNow < endTime:
            pos = self._vectorAdd(pos, self._vectorScalMul(stepThere, stepTime))
            travelledDistance = self._distance(start, pos)
            if travelledDistance > distance:
                # Turn around
                (stepThere, stepBack) = self._swap(stepThere, stepBack)
                (start, end) = self._swap(start, end)

                # Go back (now go to because we swapped) the distance you travelled to much
                travelledToMuch = travelledDistance - distance
                remainingTravelTime = float(travelledToMuch / v)
                pos = self._vectorAdd(pos, self._vectorScalMul(stepThere, remainingTravelTime))
                timeNow += waitTimeAtEndPoint
            timeNow += stepTime
            self.addWaypoint(timeNow, pos)

    def _swap(self, a, b):
        tmpa = Position(a.x, a.y, a.z)
        tmpb = Position(b.x, b.y, b.z)
        a = tmpb
        b = tmpa
        return (a,b)
    def _distance(self, p1, p2):
        sqrDistance = pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2)
        return pow(sqrDistance, 0.5)

    def _vectorAdd(self, p1, p2):
        return Position(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z)

    def _vectorSub(self, p1, p2):
        return Position(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z)

    def _vectorScalDiv(self, vector, scal):
        return Position(vector.x / scal, vector.y / scal, vector.z / scal)

    def _vectorScalMul(self, vector, scal):
        return Position(vector.x * scal, vector.y * scal, vector.z * scal)
