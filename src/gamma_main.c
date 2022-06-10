/** @file
 * Interfejs tekstowy umożliwiający grę w gamma.
 * @author Marcin Malejky
 */

#define _GNU_SOURCE /**< Dostęp do getline. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "gamma.h"
#include "interactive_mode.h"
#include "batch_mode.h"

#define WHITE_CHARS " \t\v\f\r\n" /**< Znaki białe. */
#define MIN_CHAR_COUNT 10 /**< Minimalna długość wiersza z inicjacją gry. */

/** @brief Wykonuje próbę inicjacji gry.
 * Wykonuje próbę inicjacji gry GAME na podstawie wiersza LINE długości SIZE
 * z wejścia. W razie niepowodzenia ustawia GAME->G na NULL.
 * @param[out] game – wskaźnik na strukturę przechowującą grę,
 * @param[in] line  – wiersz,
 * @param[in] size  - rozmiar wiersza,
 */
static void initiate(gamma_t **game, char *line, int size);

/** @brief Funkcja główna.
 * @return Zwraca kod wykonania porgramu.
 */
int main(void) {
    gamma_t *g = NULL;

    uint32_t line_number = 0;
    char *line = NULL;
    size_t buffer_size = 0;
    ssize_t read_size;
    while ((read_size = getline(&line, &buffer_size, stdin)) != -1) {
        ++line_number;
        if (omit(line)) {
            continue;
        }
        if (!correct_chars(line, read_size)) {
            fprintf(stderr, "ERROR %d\n", line_number);
            continue;
        }
        initiate(&g, line, read_size);
        if (g != NULL) {
            if (g->mode == 'B') {
                printf("OK %d\n", line_number);
            }
            break;
        } else {
            fprintf(stderr, "ERROR %d\n", line_number);
        }
    }
    free(line);

    if (g != NULL) {
        switch (g->mode) {
            case 'B':
                batch_mode(g, &line_number);
                break;
            case 'I':
                interactive_mode(g);
                break;
            default:
                break;
        }
    }

    gamma_delete(g);
    return 0;
}

static void initiate(gamma_t **g, char *line, int size) {
    if (size < MIN_CHAR_COUNT || strchr("BI", line[0]) == NULL ||
        !isspace(line[1]) || line[size - 1] != '\n') {
        return;
    }

    char temp_mode = line[0];
    strtok(line, WHITE_CHARS);
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t players = 0;
    uint32_t areas = 0;

    if (to_number(&width) && to_number(&height) && to_number(&players) &&
        to_number(&areas) && strtok(NULL, WHITE_CHARS) == NULL) {
        *g = gamma_new(width, height, players, areas);
        if (*g != NULL) {
            (*g)->mode = temp_mode;
            (*g)->player = 1;
            (*g)->counter = 0;
            (*g)->x = 0;
            (*g)->y = 0;
        }
    }
}
