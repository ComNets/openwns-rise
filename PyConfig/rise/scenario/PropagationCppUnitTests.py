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
from rise.Station import Station
from rise.scenario.Propagation import Propagation
from rise.Antenna import AntennaDropIn
from rise.Receiver import ReceiverDropIn
from rise.Transmitter import TransmitterDropIn
import rise.scenario.Pathloss as Pathloss
import rise.scenario.Shadowing as Shadowing
import rise.scenario.FastFading as FastFading

class PropagationForUnitTest:
    __instance = None

    @staticmethod
    def getInstance():
        if (PropagationForUnitTest.__instance == None):
            PropagationForUnitTest.__instance = Propagation()
            PropagationForUnitTest.__instance.setPair("Test1", "Test1").pathloss = Pathloss.Constant("3 dB")
            PropagationForUnitTest.__instance.setPair("Test1", "Test1").shadowing = Shadowing.No()
            PropagationForUnitTest.__instance.setPair("Test1", "Test1").fastFading = FastFading.No()
            PropagationForUnitTest.__instance.setPair("Test2", "Test2").pathloss = Pathloss.Constant("4 dB")
            PropagationForUnitTest.__instance.setPair("Test2", "Test2").shadowing = Shadowing.No()
            PropagationForUnitTest.__instance.setPair("Test2", "Test2").fastFading = FastFading.No()
            PropagationForUnitTest.__instance.setPair("Test1", "Test2").pathloss = Pathloss.Constant("5 dB")
            PropagationForUnitTest.__instance.setPair("Test1", "Test2").shadowing = Shadowing.No()
            PropagationForUnitTest.__instance.setPair("Test1", "Test2").fastFading = FastFading.No()
        return PropagationForUnitTest.__instance

class TestStation(Station):
    def __init__(self, name):
        super(TestStation, self).__init__(_antennas = [AntennaDropIn()],
                                           _receiver = [ReceiverDropIn(PropagationForUnitTest.getInstance(), name)],
                                           _transmitter = [TransmitterDropIn(PropagationForUnitTest.getInstance(), name)])
        self.logger = Logger("RISE", "TestStation_" + name, False)


class TestStation1(TestStation):
    def __init__(self):
        super(TestStation1, self).__init__("Test1")

class TestStation2(TestStation):
    def __init__(self):
        super(TestStation2, self).__init__("Test2")
