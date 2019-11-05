#ifndef TANTQUETEST_H
#define TANTQUETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Portability.h>
#include <cppunit/Exception.h>
#include <cppunit/Asserter.h>
#include <cppunit/portability/Stream.h>

class TantQueTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TantQueTest);

    CPPUNIT_TEST(testExecution);
    CPPUNIT_TEST(testFailedExecution);
    CPPUNIT_TEST(testTranslation);

    CPPUNIT_TEST_SUITE_END();

public:
    TantQueTest();
    virtual ~TantQueTest();
    void setUp();
    void tearDown();

private:
    void testExecution();
    void testFailedExecution();
    void testTranslation();

};

#endif /* TANTQUETEST_H */

