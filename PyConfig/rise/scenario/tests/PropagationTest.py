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

import rise.scenario.Propagation
import rise.scenario.Pathloss
import rise.Transmitter
import rise.Receiver
import unittest
import copy

class PropagationTest(unittest.TestCase):

    def setUp(self):
        rise.scenario.Propagation.Propagation.testRun = True

    def tearDown(self):
        rise.scenario.Propagation.Propagation.testRun = False

    def test(self):
        propagation = copy.deepcopy(rise.scenario.Propagation.DropInPropagation.getInstance())
        transmitter = rise.Transmitter.TransmitterDropIn(propagation = propagation)
        receiver = rise.Receiver.ReceiverDropIn(propagation = propagation)

        id = propagation.findId("DropIn")
        self.assertEqual("FreeSpace", propagation.getPair(id, id).pathloss.__plugin__)

        maxId = propagation.maxId()
        transmitter2 = rise.Transmitter.Transmitter(propagation, "Test")
        self.assertNotEqual(maxId, propagation.maxId())

        id2 = propagation.findId("Test")
        propagation.setPair("DropIn", "Test").pathloss = rise.scenario.Pathloss.Deny()
        self.assertEqual("Deny", propagation.getPair(id, id2).pathloss.__plugin__)
        self.assertRaises(rise.scenario.Propagation.NotConfigured, propagation.getPair, id2, id)
