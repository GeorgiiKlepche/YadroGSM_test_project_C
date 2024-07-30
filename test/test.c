#include <check.h>

#include "../include/club_manager.h"

START_TEST(init_event_list_1) {
    EventList list;
    init_event_list(&list);
    ck_assert_ptr_eq(list.events, NULL);
    ck_assert_int_eq(list.size, 0);
    ck_assert_int_eq(list.capacity, 0);
}
END_TEST

START_TEST(add_event_1) {
    EventList list;
    init_event_list(&list);
    Event event1 = {"09:00", 1, "client1", 1};
    Event event2 = {"10:00", 2, "client2", 2};

    add_event(&list, &event1);
    ck_assert_int_eq(list.size, 1);
    ck_assert_int_eq(list.capacity, 1);
    ck_assert_str_eq(list.events[0].time, "09:00");
    ck_assert_str_eq(list.events[0].name, "client1");

    add_event(&list, &event2);
    ck_assert_int_eq(list.size, 2);
    ck_assert_int_eq(list.capacity, 2);
    ck_assert_str_eq(list.events[1].time, "10:00");
    ck_assert_str_eq(list.events[1].name, "client2");

    free_event_list(&list);
}
END_TEST

START_TEST(working_time_1) {
    ck_assert_int_eq(working_time("09:00", "08:00", "20:00"), 1);
    ck_assert_int_eq(working_time("07:00", "08:00", "20:00"), 0);
    ck_assert_int_eq(working_time("21:00", "08:00", "20:00"), 0);
}
END_TEST

START_TEST(time_to_minutes_1) {
    ck_assert_int_eq(time_to_minutes("09:00"), 540);
    ck_assert_int_eq(time_to_minutes("19:00"), 1140);
}
END_TEST

START_TEST(minutes_to_time_1) {
    char buffer[6];
    minutes_to_time(540, buffer);
    ck_assert_str_eq(buffer, "09:00");
    minutes_to_time(1140, buffer);
    ck_assert_str_eq(buffer, "19:00");
}
END_TEST

START_TEST(handler_events_1) {
    EventList list;
    init_event_list(&list);

    Event events[] = {{"08:48", 1, "client1", 0}, {"09:41", 1, "client1", 0},
                      {"09:48", 1, "client2", 0}, {"09:52", 3, "client1", 0},
                      {"09:54", 2, "client1", 1}, {"10:25", 2, "client2", 2},
                      {"10:58", 1, "client3", 0}, {"10:59", 2, "client3", 3},
                      {"11:30", 1, "client4", 0}, {"11:35", 2, "client4", 2},
                      {"11:45", 3, "client4", 0}, {"12:33", 4, "client1", 0},
                      {"12:43", 4, "client2", 0}, {"15:52", 4, "client4", 0}};

    for (int i = 0; i < 14; ++i) {
        add_event(&list, &events[i]);
    }

    FILE *fp = freopen("output.log", "w+", stdout);
    handler_events(&list, 3, "09:00", "19:00", 10);
    fclose(fp);

    fp = fopen("output.log", "r");
    char output[1024];
    fread(output, sizeof(char), 1024, fp);
    fclose(fp);

    const char *expected_output =
        "09:00\n"
        "08:48 1 client1\n"
        "08:48 13 NotOpenYet\n"
        "09:41 1 client1\n"
        "09:48 1 client2\n"
        "09:52 3 client1\n"
        "09:52 13 ICanWaitNoLonger!\n"
        "09:54 2 client1 1\n"
        "10:25 2 client2 2\n"
        "10:58 1 client3\n"
        "10:59 2 client3 3\n"
        "11:30 1 client4\n"
        "11:35 2 client4 2\n"
        "11:35 13 PlaceIsBusy\n"
        "11:45 3 client4\n"
        "12:33 4 client1\n"
        "12:33 12 client4 1\n"
        "12:43 4 client2\n"
        "15:52 4 client4\n"
        "19:00 11 client3\n"
        "19:00\n"
        "1 70 05:58\n"
        "2 30 02:18\n"
        "3 90 08:01\n";

    ck_assert_str_eq(output, expected_output);

    remove("output.log");

    free_event_list(&list);
}
END_TEST

START_TEST(parse_input_1) {
    const char *test_filename = "test/test_input1.txt";

    EventList event_list;
    init_event_list(&event_list);
    int table_count;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour;

    parse_input(test_filename, &event_list, &table_count, start_time, end_time,
                &price_per_hour);

    ck_assert_int_eq(table_count, 5);
    ck_assert_str_eq(start_time, "09:00");
    ck_assert_str_eq(end_time, "21:00");
    ck_assert_int_eq(price_per_hour, 10);
    ck_assert_int_eq(event_list.size, 2);

    free_event_list(&event_list);
}
END_TEST

START_TEST(parse_input_2) {
    const char *test_filename = "test/test_input2.txt";

    EventList event_list;
    init_event_list(&event_list);
    int table_count;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour;

    ck_assert_int_eq(parse_input(test_filename, &event_list, &table_count,
                                 start_time, end_time, &price_per_hour),
                     ERROR);
}
END_TEST

START_TEST(parse_input_3) {
    const char *test_filename = "test/no_file.txt";

    EventList event_list;
    init_event_list(&event_list);
    int table_count;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour;

    ck_assert_int_eq(parse_input(test_filename, &event_list, &table_count,
                                 start_time, end_time, &price_per_hour),
                     ERROR);
}
END_TEST

START_TEST(parse_input_4) {
    const char *test_filename = "test/test_input3.txt";

    EventList event_list;
    init_event_list(&event_list);
    int table_count;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour;

    ck_assert_int_eq(parse_input(test_filename, &event_list, &table_count,
                                 start_time, end_time, &price_per_hour),
                     ERROR);
}
END_TEST

START_TEST(parse_input_5) {
    const char *test_filename = "test/test_input4.txt";

    EventList event_list;
    init_event_list(&event_list);
    int table_count;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour;

    ck_assert_int_eq(parse_input(test_filename, &event_list, &table_count,
                                 start_time, end_time, &price_per_hour),
                     ERROR);
}
END_TEST

Suite *club_manager_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("ClubManager");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, init_event_list_1);
    tcase_add_test(tc_core, add_event_1);
    tcase_add_test(tc_core, working_time_1);
    tcase_add_test(tc_core, time_to_minutes_1);
    tcase_add_test(tc_core, minutes_to_time_1);
    tcase_add_test(tc_core, handler_events_1);
    tcase_add_test(tc_core, parse_input_1);
    tcase_add_test(tc_core, parse_input_2);
    tcase_add_test(tc_core, parse_input_3);
    tcase_add_test(tc_core, parse_input_4);
    tcase_add_test(tc_core, parse_input_5);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = club_manager_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}
