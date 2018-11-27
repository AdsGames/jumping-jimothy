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
      TEST_ADD(ToolsUnitTest::stringToInt_ut)
      TEST_ADD(ToolsUnitTest::stringToFloat_ut)
      TEST_ADD(ToolsUnitTest::toString_ut)
    }

  private:
    void stringToInt_ut();
    void stringToFloat_ut();
    void toString_ut();
};
