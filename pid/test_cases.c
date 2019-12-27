#include <unity.h>
#include <string.h>

#include <pid.c>

pid_t pid1;

pid_cfg_t pid_cfg;
const pid_cfg_t pid_default_cfg = 
{
    .q          = 1,
    .kp_q       = 1,
    .ki_q       = 1,
    .kd_q       = 1,
    .output_min = 0,
    .output_max = 100,
};

void setUp(void)
{
    memset(&pid1, 0, sizeof(pid1));
    pid_cfg = pid_default_cfg;
}

void tearDown(void)
{
}

void test_pid_init(void)
{
    pid_init(&pid1, &pid_default_cfg);

    TEST_ASSERT_EQUAL(pid1.q,          pid_default_cfg.q);
    TEST_ASSERT_EQUAL(pid1.kp_q,       pid_default_cfg.kp_q);
    TEST_ASSERT_EQUAL(pid1.ki_q,       pid_default_cfg.ki_q);
    TEST_ASSERT_EQUAL(pid1.kd_q,       pid_default_cfg.kd_q);
    TEST_ASSERT_EQUAL(pid1.output_min, pid_default_cfg.output_min);
    TEST_ASSERT_EQUAL(pid1.output_max, pid_default_cfg.output_max);
}

void test_pid_scale(void)
{
    TEST_ASSERT_EQUAL(7500,
                      pid_scale(PID_SIGNAL_MAX / 2, 5000, 10000));

    TEST_ASSERT_EQUAL(-7500,
                      pid_scale(-(PID_SIGNAL_MAX / 2), 5000, 10000));
}

void test_pid_limit(void)
{
    int32_t val;

    val = 500;
    TEST_ASSERT_EQUAL(PID_SAT_NONE,
                      pid_limit(&val, 1000));
    TEST_ASSERT_EQUAL(500, val);

    val = -500;
    TEST_ASSERT_EQUAL(PID_SAT_NONE,
                      pid_limit(&val, 1000));
    TEST_ASSERT_EQUAL(-500, val);

    val = 1500;
    TEST_ASSERT_EQUAL(PID_SAT_UPPER,
                      pid_limit(&val, 1000));
    TEST_ASSERT_EQUAL(1000, val);

    val = -1500;
    TEST_ASSERT_EQUAL(PID_SAT_LOWER,
                      pid_limit(&val, 1000));
    TEST_ASSERT_EQUAL(-1000, val);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_pid_init);
    RUN_TEST(test_pid_scale);
    RUN_TEST(test_pid_limit);

    return UNITY_END();
}
