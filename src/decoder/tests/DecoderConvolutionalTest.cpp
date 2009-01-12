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

#include <RISE/decoder/tests/DecoderConvolutionalTest.hpp>

using namespace rise;

CPPUNIT_TEST_SUITE_REGISTRATION( DecoderConvolutionalTest );

void DecoderConvolutionalTest::setUp()
{
		// These are TETRA convolutional coders
	    tetra1 = new DecoderConvolutional(4,4,15);
		tetra2 = new DecoderConvolutional(4,4,15,2.666);
		// These are UMTS convolutional coders
		umtsthirdrate = new DecoderConvolutional(1,11,18,1.0);
		umtshalfrate = new DecoderConvolutional(1,33,12,1.0);
}

void DecoderConvolutionalTest::tearDown()
{
	if(tetra1)
		delete tetra1;
	if(tetra2)
		delete tetra2;
	if(umtsthirdrate)
		delete umtsthirdrate;
	if(umtshalfrate)
		delete umtshalfrate;
}

void DecoderConvolutionalTest::testFunctions()
{
	/** \todo (all): we have to do some testing here. At least first tests
	 * for TETRA specific coders seem to show error rates more than 100
	 * times to low. I would appreciate some UMTS/WLAN tests here.;-) */
	// cout << d->getBER(0.0275) << "should be around 3E-6" << endl;
	CPPUNIT_ASSERT( tetra2->getBER(0.1) == tetra1->getBER(0.1, 2.666) );
	// For UMTS the values seem to be ok!
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.00155, umtshalfrate->getBER(0.05), 2E-5 );
	// CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1E-3, umtsthirdrate->getBER(0.1), 2E-5 );
	// CPPUNIT_ASSERT_DOUBLES_EQUAL( 3E-2, umtsthirdrate->getBER(0.15), 2E-5 );
	// CPPUNIT_ASSERT_DOUBLES_EQUAL( 3E-6, tetra1->getBER(0.0275), 0.0 );
}

void DecoderConvolutionalTest::createDecoderConvolutionalKTooSmall()
{
	if(tetra1)
		delete tetra1;
	tetra1 = NULL;
	tetra1 = new DecoderConvolutional(-4,4,15);
}

void DecoderConvolutionalTest::createDecoderConvolutionalBDFREETooSmall()
{
	if(tetra1)
		delete tetra1;
	tetra1 = NULL;
	tetra1 = new DecoderConvolutional(4,-4,15);
}

void DecoderConvolutionalTest::createDecoderConvolutionalDFREETooSmall()
{
	if(tetra1)
		delete tetra1;
	tetra1 = NULL;
	tetra1 = new DecoderConvolutional(4,4,-15);
}

void DecoderConvolutionalTest::createDecoderConvolutionalPunctuationTooSmall1()
{
	if(tetra1)
		delete tetra1;
	tetra1 = NULL;
	tetra1 = new DecoderConvolutional(4,4,15,-0.1);
}

void DecoderConvolutionalTest::createDecoderConvolutionalPunctuationTooSmall2()
{
	if(tetra1)
		delete tetra1;
	tetra1 = NULL;
	tetra1 = new DecoderConvolutional(4,4,15,0.9);
}


