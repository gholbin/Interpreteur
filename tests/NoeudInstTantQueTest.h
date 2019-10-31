#ifndef NOEUDINSTTANTQUETEST_H
#define NOEUDINSTTANTQUETEST_H

#include <cppunit/extensions/HelperMacros.h>

class NoeudInstTantQueTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(NoeudInstTantQueTest);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    NoeudInstTantQueTest();
    virtual ~NoeudInstTantQueTest();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif /* NOEUDINSTTANTQUETEST_H */

