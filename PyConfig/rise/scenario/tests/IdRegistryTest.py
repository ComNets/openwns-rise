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

import rise.scenario.IdRegistry
import unittest

class IdRegistryTest(unittest.TestCase):

    def testInit(self):
        foo = rise.scenario.IdRegistry.IdRegistry()
        self.assertEqual(0, foo.len())
        self.assert_(not foo.knowsId(0))
        self.assertRaises(rise.scenario.IdRegistry.IdNotFoundError, foo.findName, 0)

    def testRegister(self):
        foo = rise.scenario.IdRegistry.IdRegistry()
        bar = rise.scenario.IdRegistry.IdRegistry()
        self.assert_(foo.registerName("test"))
        self.assert_(foo.knowsName("test"))
        self.assertEqual(1, foo.len())
        self.assertEqual(0, bar.len())
        self.assert_(not foo.registerName("test"))
        foo.registerName("foo")
        l = foo.len()
        foo.registerName("foo")
        self.assertEqual(l, foo.len())
        foo.registerName("bar")
        self.assertEqual(l + 1, foo.len())

    def testFindIdKnowsId(self):
        foo = rise.scenario.IdRegistry.IdRegistry()
        self.assertRaises(rise.scenario.IdRegistry.NameNotFoundError, foo.findId, "foo")
        foo.registerName("foo")
        fooId = foo.findId("foo")
        self.assert_(foo.knowsId(fooId))
        foo.registerName("bar")
        self.assertNotEqual(fooId, foo.findId("bar"))

    def testFindName(self):
        foo = rise.scenario.IdRegistry.IdRegistry()
        self.assertRaises(rise.scenario.IdRegistry.IdNotFoundError, foo.findName, 0)
        fooId = foo.getId("foo")
        self.assertEqual("foo", foo.findName(fooId))

    def testGetId(self):
        foo = rise.scenario.IdRegistry.IdRegistry()
        fooId = foo.getId("foo")
        self.assertEqual(fooId + 1, foo.getId("bar"))
        
