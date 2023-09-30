#include "../src/lookup.h"
#include "../unity/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_get_command_type(void)
{
    TEST_ASSERT_EQUAL(C_ARITHMETIC, get_command_type("add"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_command_type);

    return UNITY_END();
}
