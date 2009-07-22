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

import rise.scenario.Pathloss as Pathloss
from openwns.logger import Logger

class PathlossChooser(object):
	__plugin__ = 'PathlossChooser'
	plLOS = None
	plNLOS = None
	plModelName = None

	def __init__(self, plLOS, plNLOS, plModelName,parentLogger = None):
		self.plLOS = plLOS
		self.plNLOS = plNLOS
		self.plModelName = plModelName
		self.minPathloss = min(self.plLOS.minPathloss,self.plNLOS.minPathloss)
		self.logger = Logger("RISE", "Scenario.PathlossChooser", True, parentLogger)

class PathlossChooserTestUrbanMicro(PathlossChooser):
	def __init__(self):
		plLOS = Pathloss.LOSUrbanMicro(minDistance=10, maxDistance=5000, hBS=10, hUT=10, f=2500)
		plNLOS = Pathloss.NLOSUrbanMicroHexagonal(minDistance=10, maxDistance=5000, hBS=10, hUT=10, f=2500)
		super(PathlossChooserTestUrbanMicro, self).__init__(plLOS,plNLOS,plModelName = "UrbanMicro")
		
class PathlossChooserTestUrbanMacro(PathlossChooser):
	def __init__(self):
		plLOS = Pathloss.LOSUrbanMacro(minDistance=10, 
										maxDistance=5000,
										hBS=10,
										hUT=10,
										f=2000
													)
		
		plNLOS = Pathloss.NLOSUrbanMacro(minDistance=10,
										maxDistance=5000,
										hBS=10,
										hUT=10,
										f=2000
													)
		super(PathlossChooserTestUrbanMacro, self).__init__(plLOS,plNLOS,plModelName = "UrbanMacro")
		
class PathlossChooserTestSubUrbanMacro(PathlossChooser):
	def __init__(self):
		plLOS = Pathloss.LOSSubUrbanMacro(minDistance=10, 
										maxDistance=5000,
										hBS=10,
										hUT=10,
										f=2000
													)
		
		plNLOS = Pathloss.NLOSSubUrbanMacro(minDistance=10,
										maxDistance=5000,
										hBS=10,
										hUT=10,
										f=2000
													)
		super(PathlossChooserTestSubUrbanMacro, self).__init__(plLOS,plNLOS,plModelName = "SuburbanMacro")
		
class PathlossChooserTestRuralMacro(PathlossChooser):
	def __init__(self):
		plLOS = Pathloss.LOSRuralMacro(minDistance=10, 
										maxDistance=10000,
										hBS=10,
										hUT=10,
										f=800
													)
		
		plNLOS = Pathloss.NLOSRuralMacro(minDistance=10,
										maxDistance=5000,
										hBS=10,
										hUT=10,
										f=800
													)
		super(PathlossChooserTestRuralMacro, self).__init__(plLOS,plNLOS,plModelName = "RuralMacro")
	
class UrbanMicroPathlossChooser(PathlossChooser):
	hBS = None # m
	hUT = None # m
	f = None # MHz
	minDistance = None # m
	maxDistance = None # m
	plModelName = None
	def __init__(self,plModelName, minDistance, maxDistance, hBS=10, hUT=1.5, f=2500):
		self.hBS = hBS
		self.hUT = hUT
		self.f = f
		self.minDistance = minDistance
		self.maxDistance = maxDistance
		self.plModelName = plModelName
		plLOS = Pathloss.LOSUrbanMicro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		
		plNLOS = Pathloss.NLOSUrbanMicroHexagonal(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		super(UrbanMicroPathlossChooser, self).__init__(plLOS,plNLOS,plModelName)
	
class UrbanMacroPathlossChooser(PathlossChooser):
	hBS = None # m
	hUT = None # m
	f = None # MHz
	minDistance = None # m
	maxDistance = None # m
	plModelName = None
	def __init__(self,plModelName, minDistance, maxDistance, hBS=25, hUT=1.5, f=2000):
		self.hBS = hBS
		self.hUT = hUT
		self.f = f
		self.minDistance = minDistance
		self.maxDistance = maxDistance
		self.plModelName = plModelName
		plLOS = Pathloss.LOSUrbanMacro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		
		plNLOS = Pathloss.NLOSUrbanMacro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		super(UrbanMacroPathlossChooser, self).__init__(plLOS,plNLOS,plModelName)
		
class SubUrbanMacroPathlossChooser(PathlossChooser):
	hBS = None # m
	hUT = None # m
	f = None # MHz
	minDistance = None # m
	maxDistance = None # m
	plModelName = None
	def __init__(self,plModelName, minDistance, maxDistance, hBS=35, hUT=1.5, f=2000):
		self.hBS = hBS
		self.hUT = hUT
		self.f = f
		self.minDistance = minDistance
		self.maxDistance = maxDistance
		self.plModelName = plModelName
		plLOS = Pathloss.LOSSubUrbanMacro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		
		plNLOS = Pathloss.NLOSSubUrbanMacro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		super(SubUrbanMacroPathlossChooser, self).__init__(plLOS,plNLOS,plModelName)
		
class RuralMacroPathlossChooser(PathlossChooser):
	hBS = None # m
	hUT = None # m
	f = None # MHz
	minDistance = None # m
	maxDistance = None # m
	plModelName = None
	def __init__(self,plModelName, minDistance, maxDistance, hBS=35, hUT=1.5, f=800):
		self.hBS = hBS
		self.hUT = hUT
		self.f = f
		self.minDistance = minDistance
		self.maxDistance = maxDistance
		self.plModelName = plModelName
		plLOS = Pathloss.LOSRuralMacro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		
		plNLOS = Pathloss.NLOSRuralMacro(minDistance=self.minDistance, 
								maxDistance=self.maxDistance,
								hBS=self.hBS,
								hUT=self.hUT,
								f=self.f)
		super(RuralMacroPathlossChooser, self).__init__(plLOS,plNLOS,plModelName)
