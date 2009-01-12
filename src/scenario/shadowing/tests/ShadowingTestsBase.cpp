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

#include <RISE/scenario/shadowing/tests/ShadowingTestsBase.hpp>

#include <WNS/Exception.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <limits>

using namespace rise::scenario::shadowing::tests;

const wns::Frequency ShadowingTestsBase::BaseTestDistance = 100;

ShadowingTestsBase::~ShadowingTestsBase()
{}

void ShadowingTestsBase::testInput(const Shadowing *const shadowing)
{
    if (shadowing != NULL) {
	station1->moveTo(wns::Position(0, 0, 0));
	station2->moveTo(wns::Position(BaseTestDistance, 0, 0));

	// test if the assertions work
	CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_THROW( shadowing->getShadowing(*antenna1, *antenna2), wns::Exception ) );
	CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_NO_THROW( shadowing->getShadowing(*antenna1, *antenna2) ) );

	// test zero distance
	station2->moveTo(wns::Position(0, 0, 0));
	CPPUNIT_ASSERT_NO_THROW( shadowing->getShadowing(*antenna1, *antenna2) );
    }
}

