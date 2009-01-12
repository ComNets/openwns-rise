/*******************************************************************************
 * This file is part of openWNS (open Wireless Network Simulator)
 * _____________________________________________________________________________
 *
 * Copyright (C) 2004-2007
 * Chair of Communication Networks (ComNets)
 * Kopernikusstr. 5, D-52074 Aachen, Germany
 * phone: ++49-241-80-27910,
 * fax: ++49-241-80-22242
 * email: info@openwns.org
 * www: http://www.openwns.org
 * _____________________________________________________________________________
 *
 * openWNS is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 2 as published by the
 * Free Software Foundation;
 *
 * openWNS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include <RISE/scenario/Obstruction.hpp>

#include <WNS/geometry/Point.hpp>
#include <WNS/geometry/LineSegment.hpp>
#include <WNS/geometry/AxisParallelRectangle.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/CppUnit.hpp>

namespace rise { namespace scenario { namespace tests {

    class ObstructionTest :
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( ObstructionTest );
	CPPUNIT_TEST( testGetAttenuationFactor );
	CPPUNIT_TEST( testGetAttenuation );
	CPPUNIT_TEST_SUITE_END();
    public:

	void prepare();
	void cleanup();
	
	void testGetAttenuationFactor();
	void testGetAttenuation();
    private:	
	typedef Obstructing<wns::geometry::AxisParallelRectangle> Obstruct;
	typedef wns::SmartPtr<Obstruct> ObstructPtr;
	
	Obstruct* obstruct;
	ObstructPtr obstructPtr;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( ObstructionTest );

    void ObstructionTest::prepare()
    {
	const wns::Ratio& attenuation = wns::Ratio::from_dB(3.0);
	wns::geometry::AxisParallelRectangle rect(wns::geometry::Point(1,1,0),
						  wns::geometry::Point(4,4,0));
	obstruct = new Obstruct(rect, attenuation);
	obstructPtr = ObstructPtr(obstruct);

    }
    

    void ObstructionTest::cleanup()
    {
    }
    
    void ObstructionTest::testGetAttenuationFactor()
    {
	CPPUNIT_ASSERT(obstructPtr->getAttenuationFactor() == wns::Ratio::from_dB(3.0));
	
    }
    
    void ObstructionTest::testGetAttenuation()
    {

	wns::geometry::LineSegment line1(wns::geometry::Point(0,0,0), wns::geometry::Point(5,5,0));
	WNS_ASSERT_MAX_REL_ERROR(obstructPtr->getAttenuation(line1),
				 wns::Ratio::from_dB(6.0),
				 1E-5);

	wns::geometry::LineSegment line2(wns::geometry::Point(0,0,0), wns::geometry::Point(3,3,0));
	WNS_ASSERT_MAX_REL_ERROR(obstructPtr->getAttenuation(line2),
				 wns::Ratio::from_dB(3.0),
				 1E-5);

	wns::geometry::LineSegment line3(wns::geometry::Point(1,0,0), wns::geometry::Point(1,5,0));
	WNS_ASSERT_MAX_REL_ERROR(obstructPtr->getAttenuation(line3),
				 wns::Ratio::from_dB(6.0),
				 1E-5);

	wns::geometry::LineSegment line4(wns::geometry::Point(0,0,0), wns::geometry::Point(5,0,0));
	WNS_ASSERT_MAX_REL_ERROR(obstructPtr->getAttenuation(line4),
				 wns::Ratio::from_dB(0.0),
				 1E-5);
    }
    
}}}
