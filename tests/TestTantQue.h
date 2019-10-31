#ifndef TESTTANTQUE_H
#define TESTTANTQUE_H

#include <cppunit/extensions/HelperMacros.h>
#include
class TestTantQue : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestTantQue);

    CPPUNIT_TEST(testExecution);
    CPPUNIT_TEST(testFailedExecution);
    CPPUNIT_TEST(testTranslation);

    CPPUNIT_TEST_SUITE_END();


private:
    void testExecution();
    void testFailedExecution();
    void testTranslation();
};

#endif /* TESTTANTQUE_H */

