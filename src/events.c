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
