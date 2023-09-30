#include "../src/parser.h"
#include "../unity/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_trim_on_simple_line(void)
{
    char a[] = "a string with no leading or trailing space";
    trim(a);
    TEST_ASSERT_EQUAL_STRING("a string with no leading or trailing space", a);

    char b[] = "a string with trailing space       ";
    trim(b);
    TEST_ASSERT_EQUAL_STRING("a string with trailing space", b);

    char c[] = "another string with trailing spaces  \t\n";
    trim(c);
    TEST_ASSERT_EQUAL_STRING("another string with trailing spaces", c);

    char d[] = "    a string with leading spaces";
    trim(d);
    TEST_ASSERT_EQUAL_STRING("a string with leading spaces", d);

    char e[] = "\t    another string with leading spaces";
    trim(e);
    TEST_ASSERT_EQUAL_STRING("another string with leading spaces", e);

    char f[] = "    a string with leading & trailing spaces    \n";
    trim(f);
    TEST_ASSERT_EQUAL_STRING("a string with leading & trailing spaces", f);
}

void test_cleanline_on_simple_line(void)
{
    char a[] = "a string with comments at the end // a comment";
    cleanline(a);
    TEST_ASSERT_EQUAL_STRING("a string with comments at the end", a);

    char b[] = " \t  a string with leading/trailing space and comments    // a comment";
    cleanline(b);
    TEST_ASSERT_EQUAL_STRING("a string with leading/trailing space and comments", b);

    char c[] = "// a line of all comments ";
    cleanline(c);
    TEST_ASSERT_EQUAL_STRING("", c);

    char d[] = "// a line with multiple // comments ";
    cleanline(d);
    TEST_ASSERT_EQUAL_STRING("", d);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_trim_on_simple_line);
    RUN_TEST(test_cleanline_on_simple_line);

    return UNITY_END();
}
