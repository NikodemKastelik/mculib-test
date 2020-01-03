#include <unity.h>
#include <string.h>

#include <ring_buffer.c>

#define ITEMS_COUNT  8

#define LARGEST_ITEM_SIZE  16

uint8_t buf_bytes[ITEMS_COUNT * LARGEST_ITEM_SIZE];

ringbuf_t ringbuf;

void setUp(void)
{
    memset(&ringbuf, 0, sizeof(ringbuf));
    memset(&buf_bytes, 0, sizeof(buf_bytes));
}

void tearDown(void)
{
}

void generic_test_buffer_n_bytes(void *   buffer,
                                 uint16_t item_size)
{
    ringbuf_init(&ringbuf, buffer, item_size, ITEMS_COUNT * item_size);

    uint8_t sender[LARGEST_ITEM_SIZE];
    uint8_t receiver[LARGEST_ITEM_SIZE];

    memset(&sender, 0xFE, item_size);

    for (uint32_t i = 0; i < ITEMS_COUNT; i++)
    {
        memset(&receiver, 0, item_size);

        ringbuf_put(&ringbuf, (void *)&sender);
        TEST_ASSERT_EQUAL(1, ringbuf_count_get(&ringbuf));

        ringbuf_get(&ringbuf, (void *)&receiver);
        TEST_ASSERT_EQUAL(0, ringbuf_count_get(&ringbuf));

        TEST_ASSERT_EQUAL_MEMORY(sender, receiver, item_size);
    }

    for (uint32_t i = 0; i < ITEMS_COUNT; i++)
    {
        ringbuf_put(&ringbuf, (void *)&sender);
        TEST_ASSERT_EQUAL(i + 1, ringbuf_count_get(&ringbuf));
    }

    for (uint32_t i = 0; i < ITEMS_COUNT; i++)
    {
        memset(&receiver, 0, item_size);
        ringbuf_get(&ringbuf, (void *)&receiver);
        TEST_ASSERT_EQUAL(ITEMS_COUNT - 1 - i, ringbuf_count_get(&ringbuf));
        TEST_ASSERT_EQUAL_MEMORY(sender, receiver, item_size);
    }
}

void test_buf_1byte(void)
{
    generic_test_buffer_n_bytes((void *)buf_bytes, 1);
}

void test_buf_4byte(void)
{
    generic_test_buffer_n_bytes((void *)buf_bytes, 4);
}

void test_buf_16byte(void)
{
    generic_test_buffer_n_bytes((void *)buf_bytes, 16);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_buf_1byte);
    RUN_TEST(test_buf_4byte);
    RUN_TEST(test_buf_16byte);

    return UNITY_END();
}
