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

class Error(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class NameNotFoundError(Error):
    def __init__(self, value):
        Error.__init__(self, value)

class IdNotFoundError(Error):
    def __init__(self, value):
        Error.__init__(self, value)

class IdRegistry:

    forward = None
    backward = None

    def __init__(self):
        self.forward = {}
        self.backward = []

    def registerName(self, name):
        if self.knowsName(name):
            return False
        self.forward[name] = len(self.backward)
        self.backward.append(name)
        return True

    def knowsName(self, name):
        return self.forward.has_key(name)

    def knowsId(self, id):
        return (id < len(self.backward))

    def len(self):
        return len(self.forward)

    def findId(self, name):
        if not self.knowsName(name):
            raise NameNotFoundError, "Name '" + name + "' was not found in IdRegistry"
        return self.forward[name]

    def findName(self, id):
        if not self.knowsId(id):
            raise IdNotFoundError, "Id '" + str(id) + "' was not found in IdRegistry"
        return self.backward[id]

    def getId(self, name):
        self.registerName(name)
        return self.findId(name)
