import os
import CNBuildSupport
from CNBuildSupport import CNBSEnvironment
import wnsbase.RCS as RCS

commonEnv = CNBSEnvironment(PROJNAME       = 'rise',
                            PROJMODULES    = ['TEST', 'ANTENNA-TEST', 'SCENARIO-TEST', 'BASE', 'ANTENNA', 'SCENARIO', 'MODULE'],
                            AUTODEPS       = ['wns'],
                            INCEXTENSIONS  = ['hpp'],
                            SHORTCUTS      = True,
                            LIBRARY        = True,
                            DEFAULTVERSION = True,
                            LIBS           = ['gsl', 'gslcblas'],
			    REVISIONCONTROL = RCS.Bazaar('../', 'rise', 'unstable', '1.3'), 
                            )

commonEnv['CXXFLAGS'].remove('-Woverloaded-virtual')
Return('commonEnv')
