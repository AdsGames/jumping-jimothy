#include "util/ToolsUnitTest.h"

void ToolsUnitTest::first_test() {
  // Will succeed since the expression evaluates to true
  TEST_ASSERT(1 + 1 == 2)

  // Will fail since the expression evaluates to false
  TEST_ASSERT(0 == 1);
}

void ToolsUnitTest::second_test() {
  // Will succeed since the expression evaluates to true
  TEST_ASSERT_DELTA(0.5, 0.7, 0.3);

  // Will fail since the expression evaluates to false
  TEST_ASSERT_DELTA(0.5, 0.7, 0.1);
}
