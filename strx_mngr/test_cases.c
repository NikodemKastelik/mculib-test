#include <unity.h>
#include <string.h>

#include <strx_mngr.c>

#define BUFLEN  32

char strx_mngr_buf[BUFLEN];
strx_mngr_t strx_mngr;

void setUp(void)
{
    memset(strx_mngr_buf, 0, BUFLEN);
    memset(&strx_mngr, 0, sizeof(strx_mngr));
}

void tearDown(void)
{
}

void test_feed_once_and_retrieve_once(void)
{
    const char string_8chars_terminated[] = "123456\n";
    const char expected_string[] = "123456";
    char string[sizeof(expected_string)];

    strx_mngr_init(&strx_mngr, strx_mngr_buf, BUFLEN);

    TEST_ASSERT_FALSE(strx_mngr_retrieve(&strx_mngr, string));

    for (uint32_t i = 0; i < sizeof(string_8chars_terminated); i++)
    {
        strx_mngr_feed(&strx_mngr, string_8chars_terminated[i]);
    }

    TEST_ASSERT_TRUE(strx_mngr_retrieve(&strx_mngr, string));
    TEST_ASSERT_EQUAL_STRING(expected_string, string);

    TEST_ASSERT_FALSE(strx_mngr_retrieve(&strx_mngr, string));
}

void test_feed_many_times_and_retrieve_many_times(void)
{
    char strings_8chars_terminated[BUFLEN / 8][8];
    char expected_strings[BUFLEN / 8][8];

    const char string_8chars_terminated_template[] = "abcd %d\n";
    const char expected_string_template[] = "abcd %d";

    char string[sizeof(expected_string_template)];

    for (uint32_t i = 0; i < BUFLEN / 8; i++)
    {
        (void)snprintf(&strings_8chars_terminated[i][0], 8,
                       string_8chars_terminated_template, i);

        (void)snprintf(&expected_strings[i][0], 8,
                       expected_string_template, i);
    }

    strx_mngr_init(&strx_mngr, strx_mngr_buf, BUFLEN);

    TEST_ASSERT_FALSE(strx_mngr_retrieve(&strx_mngr, string));

    for (uint32_t i = 0; i < BUFLEN / 8; i++)
    {
        const size_t len = strlen(strings_8chars_terminated[i]);
        for (uint32_t j = 0; j < len; j++)
        {
            strx_mngr_feed(&strx_mngr, strings_8chars_terminated[i][j]);
        }
    }

    for (uint32_t i = 0; i < BUFLEN / 8; i++)
    {
        TEST_ASSERT_TRUE(strx_mngr_retrieve(&strx_mngr, string));
        TEST_ASSERT_EQUAL_STRING(expected_strings[i], string);
    }

    TEST_ASSERT_FALSE(strx_mngr_retrieve(&strx_mngr, string));
}

void test_feed_retrieve_many_times(void)
{
    char strings_8chars_terminated[BUFLEN][8];
    char expected_strings[BUFLEN][8];

    const char string_8chars_terminated_template[] = "abcd %d\n";
    const char expected_string_template[] = "abcd %d";

    char string[sizeof(expected_string_template)];

    for (uint32_t i = 0; i < BUFLEN; i++)
    {
        (void)snprintf(&strings_8chars_terminated[i][0], 8,
                       string_8chars_terminated_template, i % 10);

        (void)snprintf(&expected_strings[i][0], 8,
                       expected_string_template, i % 10);
    }

    strx_mngr_init(&strx_mngr, strx_mngr_buf, BUFLEN);

    TEST_ASSERT_FALSE(strx_mngr_retrieve(&strx_mngr, string));

    for (uint32_t i = 0; i < BUFLEN; i++)
    {
        const size_t len = strlen(strings_8chars_terminated[i]);
        for (uint32_t j = 0; j < len; j++)
        {
            strx_mngr_feed(&strx_mngr, strings_8chars_terminated[i][j]);
        }

        TEST_ASSERT_TRUE(strx_mngr_retrieve(&strx_mngr, string));
        TEST_ASSERT_EQUAL_STRING(expected_strings[i], string);

        TEST_ASSERT_FALSE(strx_mngr_retrieve(&strx_mngr, string));
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_feed_once_and_retrieve_once);
    RUN_TEST(test_feed_many_times_and_retrieve_many_times);
    RUN_TEST(test_feed_retrieve_many_times);

    return UNITY_END();
}
