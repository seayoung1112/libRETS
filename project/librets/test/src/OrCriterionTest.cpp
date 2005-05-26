#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/sql_forward.h"
#include "librets/OrCriterion.h"
#include "librets/LiteralCriterion.h"
#include "librets/DmqlExpression.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS OrCriterionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testToDmql);
    CPPUNIT_TEST(testDmqlExpression);
    CPPUNIT_TEST(testOrCoalescing);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testEquals();
    void testToDmql();
    void testDmqlExpression();
    void testOrCoalescing();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testEquals()
{
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    DmqlCriterionPtr field1 = literal("field1");
    DmqlCriterionPtr field2 = literal("field2");
    OrCriterion or1(field1, foo);
    OrCriterion or2(field1, foo);
    OrCriterion or3(field1, bar);
    OrCriterion or4(field2, foo);
    OrCriterion or5(field2, bar);

    ASSERT_EQUAL(or1, or2);
    ASSERT_NOT_EQUAL(or1, or3);
    ASSERT_NOT_EQUAL(or1, or4);
    ASSERT_NOT_EQUAL(or1, or5);
    ASSERT_NOT_EQUAL(or3, or4);
    ASSERT_NOT_EQUAL(or3, or5);
}

void CLASS::testToDmql()
{
    DmqlCriterionPtr criterion = logicOr(literal("foo"), literal("bar"));
    ASSERT_STRING_EQUAL("(foo|bar)", criterion->ToDmqlString());
}

void CLASS::testDmqlExpression()
{
    DmqlCriterionPtr c1 = logicOr(literal("field1"), literal("1"));
    DmqlCriterionPtr c2 = logicOr(literal("field1"), literal(1));
    DmqlCriterionPtr c3 = logicOr(literal("field1"), literal("2"));

    ASSERT_EQUAL(*c1, *c2);
    ASSERT_NOT_EQUAL(*c1, *c3);
}

void CLASS::testOrCoalescing()
{
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    DmqlCriterionPtr baz = literal("baz");

    // (foo + bar) + baz should equal (foo + bar + baz)
    OrCriterionPtr c1(new OrCriterion(foo, bar));
    c1->add(baz);
    DmqlCriterionPtr c2 = logicOr(foo, logicOr(bar, baz));
    ASSERT_EQUAL(*DmqlCriterionPtr(c1), *c2);

    // (foo + bar) * baz should not equal (foo + bar + baz)
    c2 = logicOr(foo, logicAnd(bar, baz));
    ASSERT_NOT_EQUAL(*DmqlCriterionPtr(c1), *c2);
}