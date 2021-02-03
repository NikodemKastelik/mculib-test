#include <unity.h>
#include <string.h>

#include <hsm.c>

/* Test helpers */

#define TEST_EVENTS_MAX_COUNT 16

typedef struct
{
    const hsm_state_t * state;
    hsm_event_t         evt;
} test_event_t;

static test_event_t test_events[TEST_EVENTS_MAX_COUNT];
static uint8_t rd_idx = 0;
static uint8_t wr_idx = 0;

static void test_event_register(const hsm_state_t * state, hsm_event_t evt)
{
    test_events[wr_idx].state = state;
    test_events[wr_idx].evt   = evt;
    wr_idx++;
}

static test_event_t * test_event_pop(void)
{
    if (rd_idx < wr_idx)
    {
        return &test_events[rd_idx++];
    }
    else
    {
        return NULL;
    }
}

static void test_event_list_clear(void)
{
    memset(test_events, 0, sizeof(test_events));
    rd_idx = 0;
    wr_idx = 0;
}

/* Declarations for HSM module */

hsm_t hsm;

enum test_events
{
    HSM_TEST_EVENT_NEXT,
    HSM_TEST_EVENT_GO_TO_S1,
    HSM_TEST_EVENT_GO_TO_S2,
    HSM_TEST_EVENT_GO_TO_S3,
    HSM_TEST_EVENT_GO_TO_S121,
    HSM_TEST_EVENT_PARENT_ONLY,
    HSM_TEST_EVENT_SELF,
};

static bool state_top_handler(hsm_event_t evt);
static bool state_s1_handler(hsm_event_t evt);
static bool state_s11_handler(hsm_event_t evt);
static bool state_s111_handler(hsm_event_t evt);
static bool state_s12_handler(hsm_event_t evt);
static bool state_s121_handler(hsm_event_t evt);

static bool state_s2a_handler(hsm_event_t evt);
static bool state_s2b_handler(hsm_event_t evt);
static bool state_s2c_handler(hsm_event_t evt);
static bool state_s2d_handler(hsm_event_t evt);

static bool state_s3_handler(hsm_event_t evt);
static bool state_s31_handler(hsm_event_t evt);
static bool state_s311_handler(hsm_event_t evt);
static bool state_s3111_handler(hsm_event_t evt);

static const hsm_state_t state_top  = {state_top_handler,  NULL};
static const hsm_state_t state_s1   = {state_s1_handler,   &state_top};
static const hsm_state_t state_s11  = {state_s11_handler,  &state_s1};
static const hsm_state_t state_s111 = {state_s111_handler, &state_s11};
static const hsm_state_t state_s12  = {state_s12_handler,  &state_s1};
static const hsm_state_t state_s121 = {state_s121_handler, &state_s12};

static const hsm_state_t state_s2a = {state_s2a_handler, &state_top};
static const hsm_state_t state_s2b = {state_s2b_handler, &state_top};
static const hsm_state_t state_s2c = {state_s2c_handler, &state_top};
static const hsm_state_t state_s2d = {state_s2d_handler, &state_top};

static const hsm_state_t state_s3    = {state_s3_handler,    &state_top};
static const hsm_state_t state_s31   = {state_s31_handler,   &state_s3};
static const hsm_state_t state_s311  = {state_s311_handler,  &state_s31};
static const hsm_state_t state_s3111 = {state_s3111_handler, &state_s311};

static bool state_top_handler(hsm_event_t evt)
{
    test_event_register(&state_top, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_TEST_EVENT_GO_TO_S1:
            hsm_transition(&hsm, &state_s1);
            break;
        case HSM_TEST_EVENT_GO_TO_S2:
            hsm_transition(&hsm, &state_s2a);
            break;
        case HSM_TEST_EVENT_GO_TO_S3:
            hsm_transition(&hsm, &state_s3);
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s1_handler(hsm_event_t evt)
{
    test_event_register(&state_s1, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_TEST_EVENT_NEXT:
            hsm_transition(&hsm, &state_s11);
            break;
        case HSM_TEST_EVENT_PARENT_ONLY:
            break;
        case HSM_TEST_EVENT_SELF:
            hsm_transition(&hsm, &state_s1);
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s11_handler(hsm_event_t evt)
{
    test_event_register(&state_s11, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_TEST_EVENT_NEXT:
            hsm_transition(&hsm, &state_s111);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s111_handler(hsm_event_t evt)
{
    test_event_register(&state_s111, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_TEST_EVENT_NEXT:
            hsm_transition(&hsm, &state_s1);
            break;
        case HSM_TEST_EVENT_GO_TO_S121:
            hsm_transition(&hsm, &state_s121);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s12_handler(hsm_event_t evt)
{
    test_event_register(&state_s12, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s121_handler(hsm_event_t evt)
{
    test_event_register(&state_s121, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s2a_handler(hsm_event_t evt)
{
    test_event_register(&state_s2a, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            hsm_transition(&hsm, &state_s2b);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s2b_handler(hsm_event_t evt)
{
    test_event_register(&state_s2b, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            hsm_transition(&hsm, &state_s2c);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s2c_handler(hsm_event_t evt)
{
    test_event_register(&state_s2c, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            hsm_transition(&hsm, &state_s2d);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s2d_handler(hsm_event_t evt)
{
    test_event_register(&state_s2d, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s3_handler(hsm_event_t evt)
{
    test_event_register(&state_s3, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            hsm_transition(&hsm, &state_s31);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s31_handler(hsm_event_t evt)
{
    test_event_register(&state_s31, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            hsm_transition(&hsm, &state_s311);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s311_handler(hsm_event_t evt)
{
    test_event_register(&state_s311, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            hsm_transition(&hsm, &state_s3111);
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

static bool state_s3111_handler(hsm_event_t evt)
{
    test_event_register(&state_s3111, evt);
    switch (evt)
    {
        case HSM_EVENT_ENTRY:
            break;
        case HSM_EVENT_EXIT:
            break;
        default:
            return false;
    }
    return true;
}

/* Actual tests */

static char * state_name_get(const hsm_state_t * p_state)
{
    uintptr_t p_st = (uintptr_t)p_state;
    if      (p_st == (uintptr_t)NULL)              return "NULL";
    else if (p_st == (uintptr_t)&state_top)        return "STATE_TOP";
    else if (p_st == (uintptr_t)&state_s1)         return "STATE_S1";
    else if (p_st == (uintptr_t)&state_s11)        return "STATE_S11";
    else if (p_st == (uintptr_t)&state_s111)       return "STATE_S111";
    else if (p_st == (uintptr_t)&state_s12)        return "STATE_S12";
    else if (p_st == (uintptr_t)&state_s121)       return "STATE_S121";
    else if (p_st == (uintptr_t)&state_s2a)        return "STATE_S2A";
    else if (p_st == (uintptr_t)&state_s2b)        return "STATE_S2B";
    else if (p_st == (uintptr_t)&state_s2c)        return "STATE_S2C";
    else if (p_st == (uintptr_t)&state_s2d)        return "STATE_S2D";
    else if (p_st == (uintptr_t)&state_s3)         return "STATE_S3";
    else if (p_st == (uintptr_t)&state_s31)        return "STATE_S31";
    else if (p_st == (uintptr_t)&state_s311)       return "STATE_S311";
    else if (p_st == (uintptr_t)&state_s3111)      return "STATE_S3111";
    else
    {
        TEST_FAIL_MESSAGE("Provided state not handled in name getter.");
        return NULL;
    }
}

static char * event_name_get(hsm_event_t evt)
{
    switch (evt)
    {
        case HSM_EVENT_ENTRY:            return "ENTRY";
        case HSM_EVENT_EXIT:             return "EXIT";
        case HSM_TEST_EVENT_NEXT:        return "NEXT";
        case HSM_TEST_EVENT_PARENT_ONLY: return "PARENT_ONLY";
        case HSM_TEST_EVENT_GO_TO_S1:    return "GO_TO_S1";
        case HSM_TEST_EVENT_GO_TO_S2:    return "GO_TO_S2";
        case HSM_TEST_EVENT_GO_TO_S3:    return "GO_TO_S3";
        case HSM_TEST_EVENT_GO_TO_S121:  return "GO_TO_S121";
        case HSM_TEST_EVENT_SELF:        return "SELF";
        default:
            TEST_FAIL_MESSAGE("Provided event not handled in name getter.");
            return NULL;
    }
}

static void verify_test_events(test_event_t * expected_events, size_t evt_num)
{
    char string[128];
    (void)snprintf(string, sizeof(string), "none");
    for (size_t i = 0; i < evt_num; i++)
    {
        test_event_t * received_evt = test_event_pop();
        test_event_t * expected_evt = &expected_events[i];

        if (!received_evt)
        {
            char buffer[256];
            (void)snprintf(buffer,
                           sizeof(buffer),
                           "More expected events than received events. Last check: %s. "
                           "Next expected event: %s for state: %s",
                           string,
                           event_name_get(expected_evt->evt),
                           state_name_get(expected_evt->state));
            TEST_ASSERT_MESSAGE(received_evt, buffer);
        }

        snprintf(string,
                 sizeof(string),
                 "On event %zu, state expected %s - was %s, event expected %s - was %s",
                  i,
                  state_name_get(expected_evt->state),
                  state_name_get(received_evt->state),
                  event_name_get(expected_evt->evt),
                  event_name_get(received_evt->evt));

        TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected_evt->evt, received_evt->evt, string);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE((uintptr_t)expected_evt->state,
                                        (uintptr_t)received_evt->state,
                                        string);
    }
}

void setUp(void)
{
    memset(&hsm, 0, sizeof(hsm));
    test_event_list_clear();
}

void tearDown(void)
{
    /* Check if event queue is empty */
    test_event_t * evt = test_event_pop();
    if (evt)
    {
        char string[128];
        snprintf(string, sizeof(string),
                 "Event queue is not empty - contains %s",
                 event_name_get(evt->evt));
        TEST_FAIL_MESSAGE(string);
    }
}

void test_calculate_state_depth(void)
{
    TEST_ASSERT_EQUAL(1, hsm_state_depth_get(&state_top));
    TEST_ASSERT_EQUAL(2, hsm_state_depth_get(&state_s1));
    TEST_ASSERT_EQUAL(3, hsm_state_depth_get(&state_s11));
    TEST_ASSERT_EQUAL(4, hsm_state_depth_get(&state_s111));
}

void test_find_lowest_common_ancestor(void)
{
    const hsm_state_t * states[][3] =
    {
        {&state_s11,  &state_s12,  &state_s1},
        {&state_s111, &state_s12,  &state_s1},
        {&state_s121, &state_s111, &state_s1},
        {&state_s1, &state_s11,    &state_s1},
    };

    char string[128];
    for (uint8_t i = 0; i < sizeof(states) / sizeof(states[0]); i++)
    {
        const hsm_state_t * state1 = states[i][0];
        const hsm_state_t * state2 = states[i][1];
        const hsm_state_t * expected_lca = states[i][2];
        const hsm_state_t * calculated_lca = hsm_lca_get(state1, state2);

        snprintf(string, 128,
                 "For %s and %s expected %s but got %s",
                 state_name_get(state1), state_name_get(state2),
                 state_name_get(expected_lca), state_name_get(calculated_lca));
        TEST_ASSERT_EQUAL_HEX32_MESSAGE((uintptr_t)expected_lca,
                                        (uintptr_t)calculated_lca,
                                        string);
    }
}

void test_start(void)
{
    test_event_t expected_events[] =
    {
        {&state_top, HSM_EVENT_ENTRY},
        {&state_top, HSM_TEST_EVENT_GO_TO_S1},
        {&state_s1,  HSM_EVENT_ENTRY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S1);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

void test_simple_transition(void)
{
    test_event_t expected_events[] =
    {
        {&state_top, HSM_EVENT_ENTRY},
        {&state_top, HSM_TEST_EVENT_GO_TO_S1},
        {&state_s1,  HSM_EVENT_ENTRY},
        {&state_s1,  HSM_TEST_EVENT_NEXT},
        {&state_s11, HSM_EVENT_ENTRY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S1);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_NEXT);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

void test_event_pass_to_parent_state(void)
{
    test_event_t expected_events[] =
    {
        {&state_top, HSM_EVENT_ENTRY},
        {&state_top, HSM_TEST_EVENT_GO_TO_S1},
        {&state_s1,  HSM_EVENT_ENTRY},
        {&state_s1,  HSM_TEST_EVENT_NEXT},
        {&state_s11, HSM_EVENT_ENTRY},
        {&state_s11, HSM_TEST_EVENT_PARENT_ONLY},
        {&state_s1,  HSM_TEST_EVENT_PARENT_ONLY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S1);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_NEXT);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_PARENT_ONLY);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

void test_event_transition_with_different_parent(void)
{
    test_event_t expected_events[] =
    {
        {&state_top,  HSM_EVENT_ENTRY},
        {&state_top,  HSM_TEST_EVENT_GO_TO_S1},
        {&state_s1,   HSM_EVENT_ENTRY},
        {&state_s1,   HSM_TEST_EVENT_NEXT},
        {&state_s11,  HSM_EVENT_ENTRY},
        {&state_s11,  HSM_TEST_EVENT_NEXT},
        {&state_s111, HSM_EVENT_ENTRY},
        {&state_s111, HSM_TEST_EVENT_GO_TO_S121},
        {&state_s111, HSM_EVENT_EXIT},
        {&state_s11,  HSM_EVENT_EXIT},
        {&state_s12,  HSM_EVENT_ENTRY},
        {&state_s121, HSM_EVENT_ENTRY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S1);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_NEXT);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_NEXT);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S121);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

void test_self_transition(void)
{
    test_event_t expected_events[] =
    {
        {&state_top,  HSM_EVENT_ENTRY},
        {&state_top,  HSM_TEST_EVENT_GO_TO_S1},
        {&state_s1,   HSM_EVENT_ENTRY},
        {&state_s1,   HSM_TEST_EVENT_SELF},
        {&state_s1,   HSM_EVENT_EXIT},
        {&state_s1,   HSM_EVENT_ENTRY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S1);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_SELF);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

void test_transition_from_entry_event_equal_states(void)
{
    test_event_t expected_events[] =
    {
        {&state_top, HSM_EVENT_ENTRY},
        {&state_top, HSM_TEST_EVENT_GO_TO_S2},
        {&state_s2a, HSM_EVENT_ENTRY},
        {&state_s2a, HSM_EVENT_EXIT},
        {&state_s2b, HSM_EVENT_ENTRY},
        {&state_s2b, HSM_EVENT_EXIT},
        {&state_s2c, HSM_EVENT_ENTRY},
        {&state_s2c, HSM_EVENT_EXIT},
        {&state_s2d, HSM_EVENT_ENTRY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S2);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

void test_transition_from_entry_event_nested_states(void)
{
    test_event_t expected_events[] =
    {
        {&state_top, HSM_EVENT_ENTRY},
        {&state_top, HSM_TEST_EVENT_GO_TO_S3},
        {&state_s3, HSM_EVENT_ENTRY},
        {&state_s31, HSM_EVENT_ENTRY},
        {&state_s311, HSM_EVENT_ENTRY},
        {&state_s3111, HSM_EVENT_ENTRY},
    };

    hsm_start(&hsm, &state_top);
    hsm_dispatch(&hsm, HSM_TEST_EVENT_GO_TO_S3);

    verify_test_events(expected_events, sizeof(expected_events) / sizeof(expected_events[0]));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_calculate_state_depth);
    RUN_TEST(test_find_lowest_common_ancestor);
    RUN_TEST(test_start);
    RUN_TEST(test_simple_transition);
    RUN_TEST(test_event_pass_to_parent_state);
    RUN_TEST(test_event_transition_with_different_parent);
    RUN_TEST(test_self_transition);
    RUN_TEST(test_transition_from_entry_event_equal_states);
    RUN_TEST(test_transition_from_entry_event_nested_states);

    return UNITY_END();
}
