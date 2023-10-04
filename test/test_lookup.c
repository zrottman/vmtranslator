#include "../src/lookup.h"
#include "../unity/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_lookup_vm_command(void)
{
    TEST_ASSERT_EQUAL(C_ARITHMETIC, lookup_vm_command("add"));
    TEST_ASSERT_EQUAL(C_PUSH, lookup_vm_command("push"));
    TEST_ASSERT_EQUAL(C_IF, lookup_vm_command("if-goto"));
    TEST_ASSERT_EQUAL(C_UNKNOWN, lookup_vm_command("non-existent command"));
}

void test_lookup_vm_segment(void)
{
    TEST_ASSERT_EQUAL(S_TEMP, lookup_vm_segment("temp"));
    TEST_ASSERT_EQUAL(S_ARGUMENT, lookup_vm_segment("argument"));
    TEST_ASSERT_EQUAL(S_THIS, lookup_vm_segment("this"));
    TEST_ASSERT_EQUAL(S_UNKNOWN, lookup_vm_segment("non-existent command"));
}

void test_lookup_seg_type(void)
{
    TEST_ASSERT_EQUAL_STRING("@ARG", lookup_seg_type(S_ARGUMENT));
    TEST_ASSERT_EQUAL_STRING(NULL, lookup_seg_type(S_CONSTANT));
    TEST_ASSERT_EQUAL_STRING("@3", lookup_seg_type(S_POINTER));
    TEST_ASSERT_EQUAL_STRING(NULL, lookup_seg_type(S_UNKNOWN));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_lookup_vm_command);
    RUN_TEST(test_lookup_vm_segment);
    RUN_TEST(test_lookup_seg_type);

    return UNITY_END();
}
