#include <unity.h>
#include <string.h>

#include <mini_printf.c>
#include <ring_buffer.c>
#include <logger.c>

#define ARG1_VALUE  11

const char log_string_no_args[]  = "somestring123\n";
const char log_string_one_arg[]  = "somestring arg1: %u\n";

/* These strings get filled by digit */
const char log_string_15_chars[] = "abcdefghijkl_%u ";
const char log_string_16_chars[] = "abcdefghijklm_%u ";
const char log_string_17_chars[] = "abcdefghijklmn_%u ";

static char * buf_to_send;
static uint32_t buf_to_send_len;

static void log_out_handler(char * buf, uint32_t len)
{
    buf_to_send = buf;
    buf_to_send_len = len;
}

void setUp(void)
{
    log_init(log_out_handler);
}

void tearDown(void)
{
}

void test_log_string_no_args(void)
{
    log_msg(log_string_no_args);

    log_msg_t recently_logged_msg;
    ringbuf_get(&log_ringbuf, (void *)&recently_logged_msg);

    TEST_ASSERT_EQUAL(0, strcmp(log_string_no_args, recently_logged_msg.string));
}

void test_log_string_one_arg(void)
{
    log_msg(log_string_one_arg, ARG1_VALUE);

    log_msg_t recently_logged_msg;
    ringbuf_get(&log_ringbuf, (void *)&recently_logged_msg);

    TEST_ASSERT_EQUAL(0, strcmp(log_string_one_arg, recently_logged_msg.string));
    TEST_ASSERT_EQUAL(ARG1_VALUE, recently_logged_msg.arg1);
}

void test_log_string_different_arg_size(void)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        uint32_t arg1_val = 1 << i;

        log_msg(log_string_one_arg, arg1_val);

        log_msg_t recently_logged_msg;
        ringbuf_get(&log_ringbuf, (void *)&recently_logged_msg);

        TEST_ASSERT_EQUAL(arg1_val, recently_logged_msg.arg1);

        char buffer_1[64];
        char buffer_2[64];

        snprintf(buffer_1, 64, log_string_one_arg, arg1_val);
        mini_snprintf(buffer_2, 64, recently_logged_msg.string, recently_logged_msg.arg1);
        TEST_ASSERT_EQUAL(0, strcmp(buffer_1, buffer_2));
    }
}

void test_log_process_one_string(void)
{
    log_msg(log_string_one_arg, ARG1_VALUE);
    log_process();

    char buffer[64];
    TEST_ASSERT_EQUAL(snprintf(buffer, 64, log_string_one_arg, ARG1_VALUE), buf_to_send_len);
    TEST_ASSERT_EQUAL(0, strcmp(buffer, buf_to_send));
}

void test_log_process_many_strings(void)
{
    char buffer[LOG_TX_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    uint32_t i;
    for (i = 0; i < (LOG_TX_BUFFER_SIZE / 16) - 1; i++)
    {
        log_msg(log_string_16_chars, i % 10);
    }

    /* Last string has to be shorter due to terminating null byte*/
    log_msg(log_string_15_chars, i % 10);
    i++;

    /* Add one extra string which won't fit into output buffer */
    log_msg(log_string_16_chars, i % 10);
    i++;

    log_process();

    for (i = 0; i < (LOG_TX_BUFFER_SIZE / 16) - 1; i++)
    {
        (void)snprintf(&buffer[i * 16],
                        LOG_TX_BUFFER_SIZE,
                        log_string_16_chars,
                        i % 10);
    }
    (void)snprintf(&buffer[i * 16],
                    LOG_TX_BUFFER_SIZE,
                    log_string_15_chars,
                    i % 10);
    i++;

    TEST_ASSERT_EQUAL_MEMORY(buffer, buf_to_send, buf_to_send_len);
    TEST_ASSERT_EQUAL(LOG_TX_BUFFER_SIZE - 1, buf_to_send_len);

    log_process();

    (void)snprintf(buffer, LOG_TX_BUFFER_SIZE, log_string_16_chars, i % 10);

    TEST_ASSERT_EQUAL_MEMORY(buffer, buf_to_send, 16);
    TEST_ASSERT_EQUAL(16, buf_to_send_len);
}

void test_log_process_many_strings_last_fragmented(void)
{
    char buffer[LOG_TX_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    uint32_t i;
    for (i = 0; i < (LOG_TX_BUFFER_SIZE / 16) - 1; i++)
    {
        log_msg(log_string_16_chars, i % 10);
    }

    /* Add one extra string which won't fit into logger output buffer */
    log_msg(log_string_17_chars, i % 10);
    i++;

    log_process();

    for (i = 0; i < (LOG_TX_BUFFER_SIZE / 16) - 1; i++)
    {
        (void)snprintf(&buffer[i * 16],
                        LOG_TX_BUFFER_SIZE,
                        log_string_16_chars,
                        i % 10);
    }

    TEST_ASSERT_EQUAL(16 * ((LOG_TX_BUFFER_SIZE / 16) - 1), buf_to_send_len);
    TEST_ASSERT_EQUAL_MEMORY(buffer, buf_to_send, buf_to_send_len);

    log_process();

    (void)snprintf(buffer, LOG_TX_BUFFER_SIZE, log_string_17_chars, i % 10);
    TEST_ASSERT_EQUAL_MEMORY(buffer, buf_to_send, 17);
    TEST_ASSERT_EQUAL(17, buf_to_send_len);
}

void test_log_process_without_log_msg(void)
{
    buf_to_send_len = 0xFFFF;

    log_process();

    TEST_ASSERT_EQUAL(0xFFFF, buf_to_send_len);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_log_string_no_args);
    RUN_TEST(test_log_string_one_arg);
    RUN_TEST(test_log_string_different_arg_size);
    RUN_TEST(test_log_process_one_string);
    RUN_TEST(test_log_process_many_strings);
    RUN_TEST(test_log_process_many_strings_last_fragmented);
    RUN_TEST(test_log_process_without_log_msg);

    return UNITY_END();
}
