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

import rise.Scenario
import rise.Antenna

import random
#random.seed(22)

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
    xGridBlocks = None
    yGridBlocks = None
    sizeX = None
    sizeY = None

    def __init__(self,
                 obstructionList,
                 xGridBlocks,
                 yGridBlocks,
                 sizeX,
                 sizeY):
        self.obstructionList = obstructionList
        self.xGridBlocks = xGridBlocks
        self.yGridBlocks = yGridBlocks
        self.sizeX = sizeX
        self.sizeY = sizeY


class Point:
    x = None
    y = None
    z = None
    def __init__(self,x,y,z):
        self.x = x
        self.y = y
        self.z = z

class Shape2D:

    def __init__(self,
                 objType = "AxisParallelRectangle",
                 pointA = [0.0,0.0,0.0],
                 pointB = [0.0,0.0,0.0],
                 attenuation = "0 dB"):
        self.ObjType = objType
        self.pointA  = pointA
        self.pointB  = pointB
        self.attenuation = attenuation
    __plugin__='Shape2DObstruction'
    pointA  = None
    pointB  = None
    attenuation = None

    def containsPoint(self, point):
        return (point[0] >= self.pointA[0] and
                point[1] >= self.pointA[1] and
                point[0] <= self.pointB[0] and
                point[1] <= self.pointB[1])

class ObjectsTest(Objects):
    scenario = None
    antennas = None
    def __init__(self):
        super(ObjectsTest, self).__init__(obstructionList = [],
                                          xGridBlocks = 20,
                                          yGridBlocks = 30, 
                                          sizeX = 500,
                                          sizeY = 500)

        self.obstructionList = [
            Shape2D( pointA = [1.0,1.0,0.0], pointB = [5.0,2.0,0.0], attenuation = "3 dB"),
            Shape2D( pointA = [3.0,3.0,0.0], pointB = [8.0,4.0,0.0], attenuation = "4 dB"),
            Shape2D( pointA = [2.0,5.0,0.0], pointB = [4.0,7.0,0.0], attenuation = "5 dB"),
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

