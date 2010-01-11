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

from openwns import dB
from openwns.geometry import Position

import rise.Scenario
import rise.Antenna

import random

from numpy import *

class No:
    __plugin__ = 'None'
    symmetric = True

    def getShadowing(self, x, y, u, v):
        return dB(0.0)

    def getShadowSigma(self):
        return 0.0

class Map:
    def __init__(self, sceneryMapName, interpolationType = 'NearestNeighbour'):
        self.sceneryMapName = sceneryMapName
        self.interpolationType = interpolationType
    __plugin__ = 'Map'
    sceneryMapName = None
    interpolationType = None

class SpatialCorrelated:
    """ Spatial (4D-) correlated log-Normal shadowing, for details see
        [1] Wang, Zhenyu and Tameh, Eustace K. and Nix, Andrew R.,
            'A Sum-of-Sinusoids based Simulation Model for the Joint Shadowing
            Process in Urban Peer-to-Peer Radio Channels,' VTC 2005
        [2] Cai, Xiaodong and Giannakis, Georgios B.
            'A Two-Dimensional Channel Simulation Model for Shadowing Processes,'
            VTC 2003
    """
    
    __plugin__ = 'SpatialCorrelated'
    correlationDistance = None
    nSamples = None
    shadowSigma = None

    f = None
    theta = None
    c = None
    symmetric = None

    def __init__(self, shadowSigma, correlationDistance, nSamples = 500, symmetric = True, seed = None):
        self.correlationDistance = correlationDistance
        self.shadowSigma = shadowSigma
        self.nSamples = nSamples
        self.symmetric = symmetric

        self.c = sqrt(2.0/nSamples)

        # random numbers for later on
        if seed is not None:
            random.seed(seed)
        rnd = []
        for i in xrange(5*nSamples):
            rnd.append(random.random())
        rnd = array(rnd)
        self.f = zeros((4, nSamples))

        fr = (log(2)/correlationDistance)/(2*pi) * sqrt(1/power(rnd[0:nSamples], 2) - 1)
        t = -pi/2+rnd[nSamples:nSamples*2]*pi
        self.f[0] = fr * cos(t)
        self.f[1] = fr * sin(t)
 
        fr = (log(2)/correlationDistance)/(2*pi) * sqrt(1/power(rnd[nSamples*2:nSamples*3], 2) - 1)
        t = -pi/2+rnd[nSamples*3:nSamples*4]*pi
        self.f[2] = fr * cos(t)
        self.f[3] = fr * sin(t)

        if(self.symmetric):
            self.f[0] = self.f[2]
            self.f[3] = self.f[1]
            self.f = transpose(self.f)

        if(self.symmetric):
            self.theta = rnd[nSamples*4:nSamples*5] * 2 * pi
            self.theta[nSamples/2:nSamples] = self.theta[0:(nSamples/2)]
        else:
            self.theta = rnd[nSamples*4:nSamples*5] * 2 * pi

    def getShadowing(self, x, y, u, v):
        shdw = dB(self.shadowSigma * (sum(self.c * cos(transpose(2*pi*dot(self.f, array([[x], [y], [u], [v]]))) + self.theta),1))[0])
        return(shdw)

    def getShadowSigma(self):
        return(self.shadowSigma)

class Objects(object):
    __plugin__ = 'Objects'
    obstructionList = None

    def __init__(self,
                 obstructionList):
        self.obstructionList = obstructionList

class Shape2D(object):
    pointA  = None
    pointB  = None
    attenuation = None

    def __init__(self, pointA, pointB, attenuation):
        self.pointA  = pointA
        self.pointB  = pointB
        self.attenuation = attenuation

class AxisParallelRectangle(Shape2D):
    __plugin__ = 'rise.shadowing.obstruction.AxisParallelRectangle'

    def __init__(self, pointA, pointB, attenuation):
        super(AxisParallelRectangle, self).__init__(pointA, pointB, attenuation)


    def containsPoint(self, point):
        return (point.x >= self.pointA.x and
                point.y >= self.pointA.y and
                point.x <= self.pointB.x and
                point.y <= self.pointB.y)

class LineSegment(Shape2D):
    __plugin__ = 'rise.shadowing.obstruction.LineSegment'

    def __init__(self, pointA, pointB, attenuation):
        super(LineSegment, self).__init__(pointA, pointB, attenuation)



class ObjectsTest(Objects):
    scenario = None
    antennas = None
    def __init__(self):
        super(ObjectsTest, self).__init__(obstructionList = [])

        self.obstructionList = [
            AxisParallelRectangle( Position(1.0,1.0,0.0), Position(5.0,2.0,0.0), attenuation = "3 dB"),
            AxisParallelRectangle( Position(3.0,3.0,0.0), Position(8.0,4.0,0.0), attenuation = "4 dB"),
            AxisParallelRectangle( Position(2.0,5.0,0.0), Position(4.0,7.0,0.0), attenuation = "5 dB"),
            ]
        self.scenario = rise.Scenario.Scenario()
        self.antennas = [
            rise.Antenna.AntennaPosDropIn( [2,0,0] ),
            rise.Antenna.AntennaPosDropIn( [2,3,0] ),
            rise.Antenna.AntennaPosDropIn( [4,0,0] ),
            rise.Antenna.AntennaPosDropIn( [4,4.5,0] ),
            rise.Antenna.AntennaPosDropIn( [2,8,0] ),
            rise.Antenna.AntennaPosDropIn( [8,1,0] ),
            ]

