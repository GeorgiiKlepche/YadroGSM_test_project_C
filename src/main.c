#include "../include/club_manager.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Ошибка аргументов");
        exit(EXIT_FAILURE);
    }

    EventList event_list;
    init_event_list(&event_list);

    int table_count;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour;

    if (parse_input(argv[1], &event_list, &table_count, start_time, end_time,
                    &price_per_hour) == ERROR) {
        return ERROR;
    }

    if (handler_events(&event_list, table_count, start_time, end_time,
                   price_per_hour) == ERROR) {
                    return ERROR;
                   }

    free_event_list(&event_list);
    return SUCCESS;
}
