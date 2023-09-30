#include "../src/utils.h"
#include "../unity/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mystrcmp_on_same_length_strings(void)
{
    TEST_ASSERT_EQUAL(0, mystrcmp("abC1Def", "abC1Def"));
    TEST_ASSERT_EQUAL(0, mystrcmp(" abC1Def", " abC1Def"));
    TEST_ASSERT_EQUAL(0, mystrcmp("abC1Def ", "abC1Def "));
    TEST_ASSERT_EQUAL(1, mystrcmp("aCb1Def", "abC1Def"));
    TEST_ASSERT_EQUAL(1, mystrcmp(" abC1Def", " abC1De "));
    TEST_ASSERT_EQUAL(1, mystrcmp("abC1Def ", "kbDj21 3"));
}

void test_mystrcmp_on_empty_strings(void)
{
    TEST_ASSERT_EQUAL(0, mystrcmp("", ""));
}

void test_mystrcmp_on_strings_of_different_length(void)
{
    TEST_ASSERT_EQUAL(1, mystrcmp("abcdef", "abcdef "));
    TEST_ASSERT_EQUAL(1, mystrcmp("abC1Def", " abC1De f"));
    TEST_ASSERT_EQUAL(1, mystrcmp("abcdEFGHI ", "a"));
}

void test_rfind_on_simple_strings(void) {
    TEST_ASSERT_EQUAL_STRING(".vm", rfind("myfile.vm", '.'));
    TEST_ASSERT_EQUAL_STRING(".vm", rfind("my.file.vm", '.'));
    TEST_ASSERT_EQUAL_STRING(NULL, rfind("myfilevm", '.'));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mystrcmp_on_same_length_strings);
    RUN_TEST(test_mystrcmp_on_empty_strings);
    RUN_TEST(test_mystrcmp_on_strings_of_different_length);
    RUN_TEST(test_rfind_on_simple_strings);
    return UNITY_END();
}
