#include "util/ToolsUnitTest.h"
#include "util/Tools.h"

void ToolsUnitTest::stringToInt_ut() {
  TEST_ASSERT(tools::stringToInt("0") == 0)
  TEST_ASSERT(tools::stringToInt("10") == 10)
  TEST_ASSERT(tools::stringToInt("100") == 100)
  TEST_ASSERT(tools::stringToInt("-1") == -1)
  TEST_ASSERT(tools::stringToInt("-100") == -100)
  TEST_ASSERT(tools::stringToInt("asdf") == 0)
  TEST_ASSERT(tools::stringToInt("0asdf0") == 0)
  TEST_ASSERT(tools::stringToInt("") == 0)
}

void ToolsUnitTest::stringToFloat_ut() {
  TEST_ASSERT(tools::stringToFloat("1.0") == 1.0f)
  TEST_ASSERT(tools::stringToFloat("10.0") == 10.0f)
}
