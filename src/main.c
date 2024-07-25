#include "../include/club_manager.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Error argument");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    EventList event_list;
    init_event_list(&event_list);

    int table_count = 0;
    char start_time[TIME_SIZE];
    char end_time[TIME_SIZE];
    int price_per_hour = 0;

    if (fscanf(file, "%d %5s %5s %d", &table_count, start_time, end_time,
               &price_per_hour) != 4) {
        fclose(file);
        free_event_list(&event_list);
        perror("Error data in file");
        exit(EXIT_FAILURE);
    }

    printf("Table Count: %d\n", table_count);
    printf("Start Time: %s\n", start_time);
    printf("End Time: %s\n", end_time);
    printf("Price Per Hour: %d\n\n\n\n", price_per_hour);

    while (1) {
        Event event;
        if (fscanf(file, "%s %d %s", event.time, &event.index, event.name) !=
            3) {
            break;
        }
        if (event.index == EVENT_CLIENT_SEATED) {
            fscanf(file, "%d", &event.table_number);
        }
        printf("Event Time: %s\n", event.time);
        printf("Event ID: %d\n", event.index);
        printf("Client Name: %s\n", event.name);
        if (event.index == EVENT_CLIENT_SEATED) {
            printf("Table number: %d\n", event.table_number);
        }
        printf("-------------------\n");
    }

    fclose(file);
    free_event_list(&event_list);
    return 0;
}