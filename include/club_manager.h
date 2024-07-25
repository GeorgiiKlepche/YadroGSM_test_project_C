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

// Идентификаторы исходящих событий
#define EVENT_CLIENT_LEFT_END_OF_DAY 11
#define EVENT_CLIENT_SEATED_FROM_QUEUE 12
#define EVENT_ERROR 13

// Сообщения об ошибках
#define ERROR_YOU_SHALL_NOT_PASS "YouShallNotPass"
#define ERROR_NOT_OPEN_YET "NotOpenYet"
#define ERROR_PLACE_IS_BUSY "PlaceIsBusy"
#define ERROR_CLIENT_UNKNOWN "ClientUnknown"
#define ERROR_ICAN_WAIT_NO_LONGER "ICanWaitNoLonger!"

#define TIME_SIZE 6

// Структура для хранение информации об 1 событии
typedef struct {
    char time[TIME_SIZE];
    int index;
    char name[10];
    int table_number;
} Event;

// Структура для хранения информации о всех событиях
typedef struct {
    Event *events;
    int size;
    int capacity;
} EventList;

// Функции event.c
void init_event_list(EventList *list);
void free_event_list(EventList *list);

#endif  // COMP_MANAGER_H