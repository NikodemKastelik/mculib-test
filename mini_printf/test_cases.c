#include <unity.h>
#include <string.h>

#include <mini_printf.c>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_miniprintf_terminating_null(void)
{
    char buffer[8];
    memset(buffer, 0x0F, sizeof(buffer));

    uint32_t bytes = mini_snprintf(buffer, 8, "1234567");

    TEST_ASSERT_EQUAL(0, buffer[7]);
    TEST_ASSERT_EQUAL(7, bytes);
}

void test_miniprintf_buffer_overflow(void)
{
    char buffer[8];
    uint32_t bytes;

    memset(buffer, 0x0F, sizeof(buffer));
    bytes = mini_snprintf(buffer, 3, "1234567");
    TEST_ASSERT_EQUAL(0, buffer[2]);
    TEST_ASSERT_EQUAL(0x0F, buffer[3]);
    TEST_ASSERT_EQUAL(7, bytes);

    memset(buffer, 0x0F, sizeof(buffer));
    bytes = snprintf(buffer, 3, "1234567");
    TEST_ASSERT_EQUAL(0, buffer[2]);
    TEST_ASSERT_EQUAL(0x0F, buffer[3]);
    TEST_ASSERT_EQUAL(7, bytes);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_miniprintf_terminating_null);
    RUN_TEST(test_miniprintf_buffer_overflow);

    return UNITY_END();
}
