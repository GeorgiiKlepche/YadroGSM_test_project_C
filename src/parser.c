#include "../include/club_manager.h"

/* Функция парсит файл. */
int parse_input(const char *filename, EventList *event_list, int *table_count,
                char *start_time, char *end_time, int *price_per_hour) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return ERROR;
    }

    if (fscanf(file, "%d %5s %5s %d", table_count, start_time, end_time,
               price_per_hour) != 4) {
        fclose(file);
        free_event_list(event_list);
        perror("Ошибка данных в файле");
        return ERROR;
    }

    char last_time[TIME_SIZE] = "00:00";
    while (1) {
        Event event;
        int scan_count =
            fscanf(file, "%s %d %s\n", event.time, &event.index, event.name);
        if (scan_count != 3) {
            if (scan_count == EOF) {
                break;
            } else {
                perror("Ошибка формата ввода");
                printf("%s %d %s\n", event.time, event.index, event.name);
                fclose(file);
                free_event_list(event_list);
                return ERROR;
            }
        }

        if (event.index < 1 || event.index > 4) {
            perror("Ошибка некоректное событие");
            printf("%s %d %s\n", event.time, event.index, event.name);
            fclose(file);
            free_event_list(event_list);
            return ERROR;
        }

        if (event.index == EVENT_CLIENT_SEATED) {
            if (fscanf(file, "%d", &event.table_number) != 1) {
                perror("Ошибка формата ввода");
                printf("%s %d %s\n", event.time, event.index, event.name);
                fclose(file);
                free_event_list(event_list);
                return ERROR;
            }
        } else {
            event.table_number = -1;
        }

        if (strcmp(event.time, last_time) < 0) {
            perror("Ошибка события происходят не по порядку времени");
            printf("%s %d %s\n", event.time, event.index, event.name);
            fclose(file);
            free_event_list(event_list);
            return ERROR;
        }
        strcpy(last_time, event.time);

        if (add_event(event_list, &event) == ERROR) {
            fclose(file);
            free_event_list(event_list);
            return ERROR;
        }
    }

    fclose(file);
    return SUCCESS;
}
