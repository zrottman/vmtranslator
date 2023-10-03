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
    TEST_ASSERT_EQUAL(C_PUSH, get_command_type("push"));
    TEST_ASSERT_EQUAL(C_IF, get_command_type("if-goto"));
}

void test_get_command_for_invalid_strings(void)
{
    TEST_ASSERT_EQUAL(C_UNKNOWN, get_command_type("fake command"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_command_type);
    RUN_TEST(test_get_command_for_invalid_strings);


    return UNITY_END();
}
