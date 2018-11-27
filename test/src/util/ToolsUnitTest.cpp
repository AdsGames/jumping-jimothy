#include "util/ToolsUnitTest.h"
#include "util/Tools.h"

// Unit test for stringToInt
void ToolsUnitTest::stringToInt_ut() {
  TEST_ASSERT(tools::stringToInt("0") == 0)
  TEST_ASSERT(tools::stringToInt("-0") == 0)
  TEST_ASSERT(tools::stringToInt("10") == 10)
  TEST_ASSERT(tools::stringToInt("100") == 100)
  TEST_ASSERT(tools::stringToInt("-1") == -1)
  TEST_ASSERT(tools::stringToInt("-100") == -100)
  TEST_ASSERT(tools::stringToInt("asdf") == 0)
  TEST_ASSERT(tools::stringToInt("0asdf0") == 0)
  TEST_ASSERT(tools::stringToInt("") == 0)
  TEST_ASSERT(tools::stringToInt("0000100") == 100)
  TEST_ASSERT(tools::stringToInt("000000") == 0)
}

// Unit test for stringToFloat
void ToolsUnitTest::stringToFloat_ut() {
  TEST_ASSERT(tools::stringToFloat("1.0") == 1.0f)
  TEST_ASSERT(tools::stringToFloat("10.0") == 10.0f)
  TEST_ASSERT(tools::stringToFloat("0.0") == 0.0f)
  TEST_ASSERT(tools::stringToFloat("-0.0") == 0.0f)
  TEST_ASSERT(tools::stringToFloat("-10.0") == -10.0f)
  TEST_ASSERT(tools::stringToFloat("-10.99") == -10.99f)
  TEST_ASSERT(tools::stringToFloat("") == 0.0f)
}

// Unit test for toString
void ToolsUnitTest::toString_ut() {
  // Int
  TEST_ASSERT(tools::toString(1) == "1")
  TEST_ASSERT(tools::toString(0) == "0")

  // Float
  TEST_ASSERT(tools::toString(1.0f) == "1")
  TEST_ASSERT(tools::toString(1.1f) == "1.1")
  TEST_ASSERT(tools::toString(111.111f) == "111.111")

  // Bool
  TEST_ASSERT(tools::toString(true) == "1")
  TEST_ASSERT(tools::toString(false) == "0")
}

