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
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp("aCb1Def", "abC1Def"));
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp(" abC1Def", " abC1De "));
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp("abC1Def ", "kbDj21 3"));
}

void test_mystrcmp_on_empty_strings(void)
{
    TEST_ASSERT_EQUAL(0, mystrcmp("", ""));
}

void test_mystrcmp_on_strings_of_different_length(void)
{
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp("abcdef", "abcdef "));
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp("abC1Def", " abC1De f"));
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp("abcdEFGHI ", "a"));
}

void test_mystrcmp_on_null(void)
{
    TEST_ASSERT_EQUAL(0, mystrcmp(NULL, NULL));
    TEST_ASSERT_NOT_EQUAL(0, mystrcmp("hello", NULL));
}

void test_rfind_on_simple_strings(void) {
    TEST_ASSERT_EQUAL_STRING("ld", rfind("hello, world", 'l'));
    TEST_ASSERT_EQUAL_STRING(".vm", rfind("myfile.vm", '.'));
    TEST_ASSERT_EQUAL_STRING(".vm", rfind("my.file.vm", '.'));
    TEST_ASSERT_EQUAL_STRING(NULL, rfind("myfilevm", '.'));
}

void test_rfind_on_empty_strings(void) {
    TEST_ASSERT_EQUAL_STRING(NULL, rfind("", 'a'));
    TEST_ASSERT_EQUAL_STRING(NULL, rfind(NULL, 'a'));
}

void test_rfind_on_repeated_chars(void) {
    TEST_ASSERT_EQUAL_STRING("a", rfind("aaaaaaaaa", 'a'));
}

void test_rfind_on_null_terminated_strings(void) {
    char str[] = "hello world";
    TEST_ASSERT_EQUAL_STRING("orld", rfind(str, 'o'));
}

void test_fileid(void) {
    TEST_ASSERT_EQUAL_STRING("myvmfile", fileid("path/to/myvmfile.vm"));
    TEST_ASSERT_EQUAL_STRING("myvmfile", fileid("myvmfile.vm"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_mystrcmp_on_same_length_strings);
    RUN_TEST(test_mystrcmp_on_empty_strings);
    RUN_TEST(test_mystrcmp_on_strings_of_different_length);
    RUN_TEST(test_mystrcmp_on_null);

    RUN_TEST(test_rfind_on_simple_strings);
    RUN_TEST(test_rfind_on_empty_strings);
    RUN_TEST(test_rfind_on_repeated_chars);
    RUN_TEST(test_rfind_on_null_terminated_strings);

    RUN_TEST(test_fileid);

    return UNITY_END();
}
