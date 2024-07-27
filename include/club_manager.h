#ifndef COMP_MANAGER_H
#define COMP_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Идентификаторы входящих событий
#define EVENT_CLIENT_ARRIVED 1
#define EVENT_CLIENT_SEATED 2
#define EVENT_CLIENT_WAITING 3
#define EVENT_CLIENT_LEFT 4

// Статус клиента
#define CLIENT_NOT_BUSY -1
#define CLIENT_NOT_IN_CLUB 0
#define CLIENT_IN_CLUB 1
#define CLIENT_WAITING 2

// Статус компьютерного стола
#define TABLE_FREE 0
#define TABLE_BUSY 1

// Размер времени
#define TIME_SIZE 6

#define SUCCESS 0
#define ERROR -1

/* Структура для хранение информации об 1 событии.
 * char time - время события.
 * int index - идентификатор события.
 * char name - тело события.
 * int table_number - Если идентификатор события = EVENT_CLIENT_SEATED, то это
 * номер стола. */
typedef struct {
    char time[TIME_SIZE];
    int index;
    char name[21];
    int table_number;
} Event;

/* Структура для хранение информации о всех событиях.
 * Event events - список всех событий.
 * int size - размер структуры.
 * int capacity - заполненность структуры. */
typedef struct {
    Event *events;
    int size;
    int capacity;
} EventList;

/* Структура для хранение информации о клиенте.
 * char name - тело события.
 * int status - статус клиента.
 * int table - номер стола. */
typedef struct {
    char name[21];
    int status;
    int table;
} Client;

/* Структура для хранение информации о 'столе'.
 * int busy_by - статус занятости.
 * char *client_name - имя клиента.
 * int start_time - время начала занятости.
 * int busy_time - время занятости.
 * int revenue - доход. */
typedef struct {
    int busy_by;
    char *client_name;
    int start_time;
    int busy_time;
    int revenue;
} Table;

// Функции event.c
void init_event_list(EventList *list);
void free_event_list(EventList *list);
int add_event(EventList *list, Event *event);
int working_time(const char *current_time, const char *start_time,
                 const char *end_time);
int time_to_minutes(const char *time);
void minutes_to_time(int minutes, char *time);
int handler_events(EventList *list, int table_count, char *start_time,
                    char *end_time, int price_per_hour);

// Функции parser.c
int parse_input(const char *filename, EventList *event_list, int *table_count,
                char *start_time, char *end_time, int *price_per_hour);

#endif  // COMP_MANAGER_H