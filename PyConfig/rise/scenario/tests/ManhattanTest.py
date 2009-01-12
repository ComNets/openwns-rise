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

import unittest
import rise.scenario.Manhattan as Manhattan



# 2 cases are tests:
#-------------------
#  Manhattan 1x1:              Manhattan 2x3:

#    1      2            1       2       3       4
#    +-------+            +-------+-------+-------+
#    |   3   |            |   5   |   6   |   7   |
#    |       |            |       |       |       |
#    |4      |5           |8      |9      |10     |11
#    |       |            |       |       |       |
#   6|      7|          12|     13|     14|     15|
#    +-------+            +-------+-------+-------+
#       8                 |  16   |  17   |  18   |
#                         |       |       |       |
#                         |19     |20     |21     |22
#                         |       |       |       |
#                       23|     24|     25|     26|
#                         +-------+-------+-------+
#                            27      28      29    


class ManhattanTests(unittest.TestCase):


    def setUp(self):
        self.Manhattan11 = Manhattan.Manhattan(rows=1,
                                               columns=1,
                                               blockWidth=20,
                                               blockHeight=10,
                                               streetWidth=1,
                                               deploymentStrategy="UMTS30_03")

        self.Manhattan23 = Manhattan.Manhattan(rows=2,           
                                               columns=3,
                                               blockWidth=20,
                                               blockHeight=10,
                                               streetWidth=1,
                                               velocity = 30,
                                               deploymentStrategy="UMTS30_03") 
        

    def testDenyNonPermittedSize(self):    
        def provokeException():
            self.Manhattan0 = Manhattan(0,1) 
            self.Manhattan0 = Manhattan(1,-1) 
        self.assertRaises(Exception, provokeException)


    def testNumStreetsAndCrossings(self):
        # Manhattan 1x1:
        self.assertEqual(len(self.Manhattan11.crossings),4)
        self.assertEqual(len(self.Manhattan11.streets),4)
        # Manhattan 3x4:
        self.assertEqual(len(self.Manhattan23.crossings),12)
        self.assertEqual(len(self.Manhattan23.streets),17)


    def testIdDiffOfConnections(self):    
        # Manhattan 1x1:
        self.assertEqual(self.Manhattan11.idDiffUpperCross, -3)
        self.assertEqual(self.Manhattan11.idDiffLeftCross, -2)
        self.assertEqual(self.Manhattan11.idDiffRightCross, -1)
        self.assertEqual(self.Manhattan11.idDiffLowerCross, 2)
        # Manhattan 2x3:
        self.assertEqual(self.Manhattan23.idDiffUpperCross, -7)
        self.assertEqual(self.Manhattan23.idDiffLeftCross, -4)
        self.assertEqual(self.Manhattan23.idDiffRightCross, -3)
        self.assertEqual(self.Manhattan23.idDiffLowerCross, 4)
        

        #  +--
        #  |
        #
    def testCornerCrossings(self):
        # Manhattan 1x1:
        self.assertEqual(self.Manhattan11.crossings[0].ID, 1)
        self.assertEqual(self.Manhattan11.crossings[0].connections, [3, 4])
        self.assertEqual(self.Manhattan11.crossings[1].ID, 2)
        self.assertEqual(self.Manhattan11.crossings[1].connections, [3, 5])
        self.assertEqual(self.Manhattan11.crossings[2].ID, 6)
        self.assertEqual(self.Manhattan11.crossings[2].connections, [8, 4])
        self.assertEqual(self.Manhattan11.crossings[3].ID, 7)
        self.assertEqual(self.Manhattan11.crossings[3].connections, [8, 5])
        # Manhattan 2x3:
        self.assertEqual(self.Manhattan23.crossings[0].ID, 1)
        self.assertEqual(self.Manhattan23.crossings[0].connections, [5, 8])
        self.assertEqual(self.Manhattan23.crossings[3].ID, 4)
        self.assertEqual(self.Manhattan23.crossings[3].connections, [7, 11])
        self.assertEqual(self.Manhattan23.crossings[8].ID, 23)
        self.assertEqual(self.Manhattan23.crossings[8].connections, [27, 19])
        self.assertEqual(self.Manhattan23.crossings[11].ID, 26)
        self.assertEqual(self.Manhattan23.crossings[11].connections, [29, 22])

        # probabilities of corners
        self.assertEqual(self.Manhattan11.crossings[0].probMatrix, [[0.0, 1.0],
                                                                   [1.0, 0.0]])
        self.assertEqual(self.Manhattan23.crossings[3].probMatrix, [[0.0, 1.0],
                                                                   [1.0, 0.0]])
        # --+--
        #   |
    def testEdgeCrossings(self):
        probMatrix =   [[0.0, 0.5, 0.5],
                        [0.5, 0.0, 0.5],
                        [0.5, 0.5, 0.0]]
        
        self.assertEqual(self.Manhattan23.crossings[1].ID, 2)
        self.assertEqual(self.Manhattan23.crossings[1].connections,[5, 6, 9])
        self.assertEqual(self.Manhattan23.crossings[1].probMatrix, probMatrix)

        self.assertEqual(self.Manhattan23.crossings[4].ID, 12)
        self.assertEqual(self.Manhattan23.crossings[4].connections,[8, 16, 19])
        self.assertEqual(self.Manhattan23.crossings[4].probMatrix, probMatrix)

        self.assertEqual(self.Manhattan23.crossings[7].ID, 15)
        self.assertEqual(self.Manhattan23.crossings[7].connections,[11, 18, 22])
        self.assertEqual(self.Manhattan23.crossings[4].probMatrix, probMatrix)
        
        self.assertEqual(self.Manhattan23.crossings[10].ID, 25)
        self.assertEqual(self.Manhattan23.crossings[10].connections,[28,29,21])


        #     |
        #   --+--
        #     |
    def testMiddleCrossings(self):
        probMatrix = [[0.0, 0.25, 0.25, 0.5],
                      [0.25, 0.0, 0.5, 0.25],
                      [0.25, 0.5, 0.0, 0.25],
                      [0.5, 0.25, 0.25, 0.0]]
        self.assertEqual(self.Manhattan23.crossings[5].ID, 13)
        self.assertEqual(self.Manhattan23.crossings[5].connections,
                         [9 ,16, 17, 20])
        self.assertEqual(self.Manhattan23.crossings[5].probMatrix, probMatrix)

        self.assertEqual(self.Manhattan23.crossings[6].ID, 14)
        self.assertEqual(self.Manhattan23.crossings[6].connections,
                         [10, 17, 18, 21])
        self.assertEqual(self.Manhattan23.crossings[6].probMatrix, probMatrix)
        

        
    def testStreetsMeetingInSameCrossing(self):
        # Manhattan 1x1
        self.assertEqual(self.Manhattan11.streets[1].ID, 4)
        self.assertEqual(self.Manhattan11.streets[1].connections,[1, 6])

        self.assertEqual(self.Manhattan11.streets[3].ID, 8)
        self.assertEqual(self.Manhattan11.streets[3].connections,[6, 7])

        # Manhattan 2x3
        self.assertEqual(self.Manhattan23.streets[5].ID, 10)
        self.assertEqual(self.Manhattan23.streets[5].connections,[3, 14])

        self.assertEqual(self.Manhattan23.streets[8].ID, 17)
        self.assertEqual(self.Manhattan23.streets[8].connections,[13, 14])

        self.assertEqual(self.Manhattan23.streets[9].ID, 18)
        self.assertEqual(self.Manhattan23.streets[9].connections,[14, 15])

        self.assertEqual(self.Manhattan23.streets[12].ID, 21)
        self.assertEqual(self.Manhattan23.streets[12].connections,[14, 25])



    def testPositions(self):
        xPos1 = 42.5
        xPos2 = 63.5
        yPos1 = 0.5
        yPos2 = 11.5
        
        # horizontal street 10 connected to crossing 14
        self.assertEqual(self.Manhattan23.streets[5].endPoints[0].x, xPos1)
        self.assertEqual(self.Manhattan23.streets[5].endPoints[0].y, yPos1)
        self.assertEqual(self.Manhattan23.streets[5].endPoints[1].x, xPos1)
        self.assertEqual(self.Manhattan23.streets[5].endPoints[1].y, yPos2)

        # vertical street 18 connected to crossing 14
        self.assertEqual(self.Manhattan23.streets[9].endPoints[0].x, xPos1)
        self.assertEqual(self.Manhattan23.streets[9].endPoints[0].y, yPos2)
        self.assertEqual(self.Manhattan23.streets[9].endPoints[1].x, xPos2)
        self.assertEqual(self.Manhattan23.streets[9].endPoints[1].y, yPos2)
        
        # crossing 14
        self.assertEqual(self.Manhattan23.crossings[6].position.x, xPos1)
        self.assertEqual(self.Manhattan23.crossings[6].position.y, yPos2)



    def testVelocity(self):
        self.assertEqual(self.Manhattan23.streets[0].vMax,30)
