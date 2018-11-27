/**
 * ToolsUnitTest.cpp
 * Allan Legemaate
 * Unit test for Tools.cpp
 * 26/11/2018
 **/

// Include test lib
#include <cpptest.h>

// ToolsUnitTest
class ToolsUnitTest : public Test::Suite {
  public:
    ToolsUnitTest() {
      TEST_ADD(ToolsUnitTest::first_test)
      TEST_ADD(ToolsUnitTest::second_test)
    }

  private:
    void first_test();
    void second_test();
};
