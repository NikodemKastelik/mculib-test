#include <unity.h>
#include <string.h>
#include <stdlib.h>

#include <mini_string.c>

#define STRING_FRAGMENT_LEN  3
#define STRING_FRAGMENT_LEN_NULL  4

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mini_strlen(void)
{
    char test_string[] = "123456789";

    TEST_ASSERT_EQUAL(9, mini_strlen(test_string));
}

void test_mini_strlen_string_empty(void)
{
    char test_string[] = "";

    TEST_ASSERT_EQUAL(0, mini_strlen(test_string));
}

void test_mini_strcmp_strings_equal(void)
{
    char test_string1[] = "123456789";
    char test_string2[] = "123456789";

    TEST_ASSERT_EQUAL(0, mini_strcmp(test_string1, test_string2));
}

void test_mini_strcmp_strings_not_equal(void)
{
    char test_string1[] = "12345";
    char test_string2[] = "abcdefg";

    int32_t strcmp_retval;
    int32_t mini_strcmp_retval;

    strcmp_retval = strcmp(test_string1, test_string2);
    mini_strcmp_retval = mini_strcmp(test_string1, test_string2);
    TEST_ASSERT(strcmp_retval != 0);
    TEST_ASSERT(mini_strcmp_retval != 0);
    TEST_ASSERT(strcmp_retval = mini_strcmp_retval)

    strcmp_retval = strcmp(test_string2, test_string1);
    mini_strcmp_retval = mini_strcmp(test_string2, test_string1);
    TEST_ASSERT(strcmp_retval != 0);
    TEST_ASSERT(mini_strcmp_retval != 0);
    TEST_ASSERT(strcmp_retval = mini_strcmp_retval)
}

void test_mini_strcmp_strings_partially_equal(void)
{
    char test_string1[] = "123456789";
    char test_string2[] = "12345";

    TEST_ASSERT(0 < strcmp(test_string1, test_string2));
    TEST_ASSERT(0 < mini_strcmp(test_string1, test_string2));

    TEST_ASSERT(0 > strcmp(test_string2, test_string1));
    TEST_ASSERT(0 > mini_strcmp(test_string2, test_string1));
}

void test_mini_strcmp_string_empty(void)
{
    char test_string1[] = "123456789";
    char test_string2[] = "123456789";

    char test_string1_empty[] = "";
    char test_string2_empty[] = "";

    TEST_ASSERT(0 < strcmp(test_string1, test_string2_empty));
    TEST_ASSERT(0 < mini_strcmp(test_string1, test_string2_empty));

    TEST_ASSERT(0 > strcmp(test_string1_empty, test_string2));
    TEST_ASSERT(0 > mini_strcmp(test_string1_empty, test_string2));

    TEST_ASSERT(0 == strcmp(test_string1_empty, test_string2_empty));
    TEST_ASSERT(0 == mini_strcmp(test_string1_empty, test_string2_empty));
}

void test_mini_strstr_strings_equal(void)
{
    char test_string1[] = "123456789";
    char test_string2[] = "123456789";

    TEST_ASSERT_EQUAL((uintptr_t)test_string1,
                      (uintptr_t)strstr(test_string1, test_string2));
    TEST_ASSERT_EQUAL((uintptr_t)test_string1,
                      (uintptr_t)mini_strstr(test_string1, test_string2));
}

void test_mini_strstr_string_not_contained(void)
{
    char test_string1[] = "01234";
    char test_string2[] = "56789";

    TEST_ASSERT_EQUAL(NULL, (uintptr_t)strstr(test_string1, test_string2));
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)mini_strstr(test_string1, test_string2));
}

void test_mini_strstr_string_contained_on_offset(void)
{
    const char test_string1[] = "0123456789";
    char test_string2[STRING_FRAGMENT_LEN_NULL];

    for (uint32_t i = 0; i < sizeof(test_string1) - STRING_FRAGMENT_LEN; i++)
    {
        memset(test_string2, 0, STRING_FRAGMENT_LEN_NULL);
        memcpy(test_string2, &test_string1[i], STRING_FRAGMENT_LEN);

        TEST_ASSERT_EQUAL((uintptr_t)&test_string1[i],
                          (uintptr_t)strstr(test_string1, test_string2));

        TEST_ASSERT_EQUAL((uintptr_t)&test_string1[i],
                          (uintptr_t)mini_strstr(test_string1, test_string2));

    }
}

void test_mini_strstr_string_contained_but_larger(void)
{
    char test_string1[] = "12345";
    char test_string2[] = "123456789";

    TEST_ASSERT_EQUAL(NULL, (uintptr_t)strstr(test_string1, test_string2));
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)mini_strstr(test_string1, test_string2));
}

void test_mini_strstr_string_empty(void)
{
    char test_string1[] = "123456789";
    char test_string2[] = "123456789";

    char test_string1_empty[] = "";
    char test_string2_empty[] = "";

    TEST_ASSERT_EQUAL(NULL, (uintptr_t)strstr(test_string1_empty, test_string2));
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)mini_strstr(test_string1_empty, test_string2));


    TEST_ASSERT_EQUAL((uintptr_t)test_string1_empty,
                      (uintptr_t)strstr(test_string1_empty, test_string2_empty));

    TEST_ASSERT_EQUAL((uintptr_t)test_string1_empty,
                      (uintptr_t)mini_strstr(test_string1_empty, test_string2_empty));


    TEST_ASSERT_EQUAL((uintptr_t)test_string1,
                      (uintptr_t)strstr(test_string1, test_string2_empty));

    TEST_ASSERT_EQUAL((uintptr_t)test_string1,
                      (uintptr_t)mini_strstr(test_string1, test_string2_empty));
}

void test_mini_atoi_positive_number(void)
{
    const char string[] = "1234";
    TEST_ASSERT_EQUAL_INT(1234, atoi(string));
    TEST_ASSERT_EQUAL_INT(1234, mini_atoi(string));
}

void test_mini_atoi_negative_number(void)
{
    const char string[] = "-1234";
    TEST_ASSERT_EQUAL_INT(-1234, atoi(string));
    TEST_ASSERT_EQUAL_INT(-1234, mini_atoi(string));
}

void test_mini_atoi_number_with_leading_whitespaces(void)
{
    const char string[] = "\n\t 1234";
    TEST_ASSERT_EQUAL_INT(1234, atoi(string));
    TEST_ASSERT_EQUAL_INT(1234, mini_atoi(string));
}

void test_mini_atoi_number_with_trailing_whitespaces(void)
{
    const char string[] = "1234 \n\t";
    TEST_ASSERT_EQUAL_INT(1234, atoi(string));
    TEST_ASSERT_EQUAL_INT(1234, mini_atoi(string));
}

void test_mini_atoi_number_with_whitespaces_inside(void)
{
    const char string[] = "12\n\t34";
    TEST_ASSERT_EQUAL_INT(12, atoi(string));
    TEST_ASSERT_EQUAL_INT(12, mini_atoi(string));
}

void test_mini_atoi_empty_string(void)
{
    const char string[] = "";
    TEST_ASSERT_EQUAL_INT(0, atoi(string));
    TEST_ASSERT_EQUAL_INT(0, mini_atoi(string));
}

void test_mini_atoi_string_without_digits(void)
{
    const char string[] = "abcd";
    TEST_ASSERT_EQUAL_INT(0, atoi(string));
    TEST_ASSERT_EQUAL_INT(0, mini_atoi(string));
}

void test_mini_strchr_string_containing_char(void)
{
    const char string1[] = "=abcd";
    TEST_ASSERT_EQUAL((uintptr_t)string1,
                      (uintptr_t)strchr(string1, '='));
    TEST_ASSERT_EQUAL((uintptr_t)string1,
                      (uintptr_t)mini_strchr(string1, '='));

    const char string2[] = "abcd=";
    TEST_ASSERT_EQUAL((uintptr_t)&string2[4],
                      (uintptr_t)strchr(string2, '='));
    TEST_ASSERT_EQUAL((uintptr_t)&string2[4],
                      (uintptr_t)mini_strchr(string2, '='));
}

void test_mini_strchr_string_not_containing_char(void)
{
    const char string[] = "abcd";
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)strchr(string, '='));
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)mini_strchr(string, '='));
}

void test_mini_strchr_string_empty(void)
{
    const char string[] = "";
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)strchr(string, '='));
    TEST_ASSERT_EQUAL(NULL, (uintptr_t)mini_strchr(string, '='));
}

void test_mini_strchr_char_null(void)
{
    const char string1[] = "";
    TEST_ASSERT_EQUAL((uintptr_t)string1,
                      (uintptr_t)strchr(string1, '\0'));
    TEST_ASSERT_EQUAL((uintptr_t)string1,
                      (uintptr_t)mini_strchr(string1, '\0'));

    const char string2[] = "abcd";
    TEST_ASSERT_EQUAL((uintptr_t)&string2[4],
                      (uintptr_t)strchr(string2, '\0'));
    TEST_ASSERT_EQUAL((uintptr_t)&string2[4],
                      (uintptr_t)mini_strchr(string2, '\0'));
}

void test_mini_strstartswith_string_contained(void)
{
    const char prefix[] = "abcd";

    const char string1[] = "abcdefgh";
    TEST_ASSERT(mini_strstartswith(string1, prefix));

    const char string2[] = "abcd";
    TEST_ASSERT(mini_strstartswith(string2, prefix));
}

void test_mini_strstartswith_string_not_contained(void)
{
    const char string[] = "abc";
    const char prefix[] = "abcd";
    TEST_ASSERT_FALSE(mini_strstartswith(string, prefix));
}

void test_mini_strstartswith_string_contained_on_offset(void)
{
    const char string[] = "abcde";
    const char prefix[] = "cde";
    TEST_ASSERT_FALSE(mini_strstartswith(string, prefix));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_mini_strlen);
    RUN_TEST(test_mini_strlen_string_empty);

    RUN_TEST(test_mini_strcmp_strings_equal);
    RUN_TEST(test_mini_strcmp_strings_not_equal);
    RUN_TEST(test_mini_strcmp_strings_partially_equal);
    RUN_TEST(test_mini_strcmp_string_empty);

    RUN_TEST(test_mini_strstr_strings_equal);
    RUN_TEST(test_mini_strstr_string_not_contained);
    RUN_TEST(test_mini_strstr_string_contained_on_offset);
    RUN_TEST(test_mini_strstr_string_contained_but_larger);
    RUN_TEST(test_mini_strstr_string_empty);

    RUN_TEST(test_mini_strchr_string_containing_char);
    RUN_TEST(test_mini_strchr_string_not_containing_char);
    RUN_TEST(test_mini_strchr_string_empty);
    RUN_TEST(test_mini_strchr_char_null);

    RUN_TEST(test_mini_strstartswith_string_contained);
    RUN_TEST(test_mini_strstartswith_string_not_contained);
    RUN_TEST(test_mini_strstartswith_string_contained_on_offset);

    RUN_TEST(test_mini_atoi_positive_number);
    RUN_TEST(test_mini_atoi_negative_number);
    RUN_TEST(test_mini_atoi_number_with_leading_whitespaces);
    RUN_TEST(test_mini_atoi_number_with_trailing_whitespaces);
    RUN_TEST(test_mini_atoi_number_with_whitespaces_inside);
    RUN_TEST(test_mini_atoi_string_without_digits);
    RUN_TEST(test_mini_atoi_empty_string);

    return UNITY_END();
}
