#include "../include/club_manager.h"

/* Инициализирует структуру событий.
 * Принимает указатель на структуру EventList.*/
void init_event_list(EventList *list) {
    list->events = NULL;
    list->size = 0;
    list->capacity = 0;
}

/* Удаляет структуру событий.
 * Принимает указатель на структуру EventList.*/
void free_event_list(EventList *list) { free(list->events); }

/* Добавляет 1 событие из структуры Event в список
 * событий в стурктуре EventList. */
int add_event(EventList *list, Event *event) {
    if (list->size >= list->capacity) {
        list->capacity += 1;
        list->events = realloc(list->events, list->capacity * sizeof(Event));
        if (list->events == NULL) {
            perror("Ошибка выделения памяти");
            return ERROR;
        }
    }
    list->events[list->size++] = *event;
    return SUCCESS;
}

/* Проверка, входит ли текущее время в диапазон рабочего времени.
 * Принимает текущее время, время начала и конца рабочего дня. */
int working_time(const char *current_time, const char *start_time,
                 const char *end_time) {
    if (strcmp(current_time, start_time) >= 0 &&
        strcmp(current_time, end_time) <= 0) {
        return 1;
    } else {
        return 0;
    }
}

/* Перевод формата HH:MM в минуты.
 * Принимает текущее время события. */
int time_to_minutes(const char *time) {
    int hours, minutes;
    sscanf(time, "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

/* Перевод минут в формат HH:MM.
 * Принимает текущее время события. */
void minutes_to_time(int minutes, char *time) {
    sprintf(time, "%02d:%02d", minutes / 60, minutes % 60);
}

/* Обрабатывает все события, выводит "исходящие события".
 * После подсчитывет выручку и время занятости каждого стола.
 * Принимает на вход список всех событий из файла, количество столов, время
 * начала работы клуба, время конца работы клуба, цену за час. */
int handler_events(EventList *list, int table_count, char *start_time,
                    char *end_time, int price_per_hour) {
    printf("%s\n", start_time);

    Table *tables = calloc(table_count, sizeof(Table));
    if (tables == NULL) {
        perror("Ошибка выделения памяти");
        return ERROR;
    }

    Client *clients = calloc(list->size, sizeof(Client));
    if (clients == NULL) {
        free(tables);
        perror("Ошибка выделения памяти");
        return ERROR;
    }

    int clients_count = 0;

    // Цикл для обработки всех событий из EventList
    for (int i = 0; i < list->size; ++i) {
        Event *event = &list->events[i];

        int client_index = -1;
        for (int j = 0; j < clients_count; ++j) {
            if (strcmp(clients[j].name, event->name) == 0) {
                client_index = j;
                break;
            }
        }
        // Обработчик событий
        switch (event->index) {
            case EVENT_CLIENT_ARRIVED:
                printf("%s %d %s\n", event->time, event->index, event->name);

                if (!working_time(event->time, start_time, end_time)) {
                    printf("%s 13 NotOpenYet\n", event->time);
                    break;
                }

                if (client_index != -1 &&
                    clients[client_index].status != CLIENT_NOT_IN_CLUB) {
                    printf("%s 13 YouShallNotPass\n", event->time);
                } else {
                    if (client_index == -1) {
                        client_index = clients_count++;
                        strncpy(clients[client_index].name, event->name,
                                sizeof(clients[client_index].name));
                    }
                    clients[client_index].status = CLIENT_IN_CLUB;
                    clients[client_index].table = CLIENT_NOT_BUSY;
                }
                break;

            case EVENT_CLIENT_SEATED:
                printf("%s %d %s %d\n", event->time, event->index, event->name,
                       event->table_number);
                if (client_index == -1 ||
                    clients[client_index].status == CLIENT_NOT_IN_CLUB) {
                    printf("%s 13 ClientUnknown\n", event->time);
                } else if (tables[event->table_number - 1].busy_by) {
                    printf("%s 13 PlaceIsBusy\n", event->time);
                } else {
                    if (clients[client_index].table != CLIENT_NOT_BUSY) {
                        tables[clients[client_index].table - 1].busy_by =
                            TABLE_FREE;
                        tables[clients[client_index].table - 1].client_name =
                            NULL;
                    }
                    clients[client_index].table = event->table_number;
                    tables[event->table_number - 1].busy_by = TABLE_BUSY;
                    tables[event->table_number - 1].client_name =
                        clients[client_index].name;
                    tables[event->table_number - 1].start_time =
                        time_to_minutes(event->time);
                }
                break;
            case EVENT_CLIENT_WAITING:
                printf("%s %d %s\n", event->time, event->index, event->name);

                if (client_index == -1 ||
                    clients[client_index].status == CLIENT_NOT_IN_CLUB) {
                    printf("%s 13 ClientUnknown\n", event->time);
                } else {
                    int free_table_found = 0;
                    for (int j = 0; j < table_count; ++j) {
                        if (!tables[j].busy_by) {
                            free_table_found = 1;
                            break;
                        }
                    }
                    if (free_table_found) {
                        printf("%s 13 ICanWaitNoLonger!\n", event->time);
                    } else {
                        int waiting_clients = 0;
                        for (int j = 0; j < clients_count; ++j) {
                            if (clients[j].status == CLIENT_WAITING) {
                                waiting_clients++;
                            }
                        }
                        if (waiting_clients >= table_count) {
                            printf("%s 11 %s\n", event->time, event->name);
                            clients[client_index].status = CLIENT_NOT_IN_CLUB;
                        } else {
                            clients[client_index].status = CLIENT_WAITING;
                        }
                    }
                }
                break;

            case EVENT_CLIENT_LEFT:
                printf("%s %d %s\n", event->time, event->index, event->name);

                if (client_index == -1 ||
                    clients[client_index].status == CLIENT_NOT_IN_CLUB) {
                    printf("%s 13 ClientUnknown\n", event->time);
                } else {
                    if (clients[client_index].table != CLIENT_NOT_BUSY) {
                        int table_idx = clients[client_index].table - 1;
                        int occupied_time = time_to_minutes(event->time) -
                                            tables[table_idx].start_time;
                        tables[table_idx].busy_time += occupied_time;
                        tables[table_idx].revenue +=
                            price_per_hour * ((occupied_time + 59) / 60);
                        tables[table_idx].busy_by = TABLE_FREE;
                        tables[table_idx].client_name = NULL;
                        clients[client_index].table = CLIENT_NOT_BUSY;

                        for (int j = 0; j < clients_count; ++j) {
                            if (clients[j].status == CLIENT_WAITING) {
                                clients[j].status = CLIENT_IN_CLUB;
                                clients[j].table = table_idx + 1;
                                tables[table_idx].busy_by = TABLE_BUSY;
                                tables[table_idx].client_name = clients[j].name;
                                tables[table_idx].start_time =
                                    time_to_minutes(event->time);
                                printf("%s 12 %s %d\n", event->time,
                                       clients[j].name, table_idx + 1);
                                break;
                            }
                        }
                    }
                    clients[client_index].status = CLIENT_NOT_IN_CLUB;
                }
                break;
        }
    }

    // Генерация событий для клиентов, оставшихся в клубе после закрытия
    for (int i = 0; i < clients_count; ++i) {
        if (clients[i].status == CLIENT_IN_CLUB ||
            clients[i].status == CLIENT_WAITING) {
            printf("%s 11 %s\n", end_time, clients[i].name);
            if (clients[i].table != CLIENT_NOT_BUSY) {
                int table_idx = clients[i].table - 1;
                int occupied_time =
                    time_to_minutes(end_time) - tables[table_idx].start_time;
                tables[table_idx].busy_time += occupied_time;
                tables[table_idx].revenue +=
                    price_per_hour * ((occupied_time + 59) / 60);
                tables[table_idx].busy_by = TABLE_FREE;
                tables[table_idx].client_name = NULL;
            }
        }
    }

    printf("%s\n", end_time);

    // Вывод состояния каждого стола
    for (int i = 0; i < table_count; ++i) {
        char busy_time_str[TIME_SIZE];
        minutes_to_time(tables[i].busy_time, busy_time_str);
        printf("%d %d %s\n", i + 1, tables[i].revenue, busy_time_str);
    }

    free(tables);
    free(clients);

    return SUCCESS;
}
