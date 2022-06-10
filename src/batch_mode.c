/** @file
 * Implementacja interfejsu trybu wsadowego gry gamma.
 *
 * @author Marcin Malejky
 */

#define _GNU_SOURCE /**< Dostęp do getline. */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "batch_mode.h"

#define WHITE_CHARS " \t\v\f\r\n" /**< Znaki białe. */
#define MIN_CHAR_COUNT 10 /**< Minimalna długość wiersza z inicjacją gry. */
#define DECIMAL_BASE 10 /**< Baza systemu dziesiątkowego. */

/** @brief Wykonuje polecenie.
 * Wykonuje próbę wykonania polecenia dla gry GAME na podstawie wiersza LINE
 * o długości SIZE.
 * @param[in,out] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] line  - wiersz z wejścia,
 * @param[in] size  - rozmiar wiersza,
 * @return Wartość TRUE jeżeli pomyślnie wywałono jakieś polecenie, a FALSE
 * w przeciwnym przypadku.
 */
static bool process_line(gamma_t *g, char *line, int size);

void batch_mode(gamma_t *g, uint32_t *line_number) {
    char *line = NULL;
    size_t buffer_size = 0;
    ssize_t read_size;
    while ((read_size = getline(&line, &buffer_size, stdin)) != -1) {
        ++(*line_number);
        if (omit(line)) {
            continue;
        }
        if (!correct_chars(line, read_size) ||
            !process_line(g, line, read_size)) {
            fprintf(stderr, "ERROR %d\n", *line_number);
        }
    }
    free(line);
}

bool correct_chars(char *line, ssize_t size) {
    for (ssize_t i = 0; i < size; ++i) {
        if (line[i] == '\0' || (strchr("BImgbfqp", line[i]) == NULL &&
                                strchr("0123456789", line[i]) == NULL &&
                                !isspace(line[i]))) {
            return false;
        }
    }
    return true;
}

bool to_number(uint32_t *x) {
    char *word = strtok(NULL, WHITE_CHARS);
    if (word == NULL || strlen(word) < 1) {
        return false;
    }
    errno = 0;
    char *end = NULL;
    unsigned long int temp = strtoul(word, &end, DECIMAL_BASE);
    if (errno != 0) {
        errno = 0;
        return false;
    }
    if (temp > UINT32_MAX || *end != '\0') {
        return false;
    }
    *x = temp;
    return true;
}

bool omit(char *line) {
    return strcmp("\n", line) == 0 || line[0] == '#';
}

static bool process_line(gamma_t *g, char *line, int size) {
    if (line[size - 1] != '\n' || strchr("mgbfqp", line[0]) == NULL ||
        !isspace(line[1])) {
        return false;
    }
    uint32_t player = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    strtok(line, WHITE_CHARS);
    switch (line[0]) {
        case 'm':
        case 'g':
            if (to_number(&player) && to_number(&x) && to_number(&y) &&
                strtok(NULL, WHITE_CHARS) == NULL) {
                if (line[0] == 'm') {
                    printf("%d\n", gamma_move(g, player, x, y));
                } else {
                    printf("%d\n", gamma_golden_move(g, player, x, y));
                }
                return true;
            }
            break;
        case 'b':
        case 'f':
        case 'q':
            if (to_number(&player) && strtok(NULL, WHITE_CHARS) == NULL) {
                if (line[0] == 'b') {
                    printf("%lu\n", gamma_busy_fields(g, player));
                } else if (line[0] == 'f') {
                    printf("%lu\n", gamma_free_fields(g, player));
                } else {
                    printf("%d\n", gamma_golden_possible(g, player));
                }
                return true;
            }
            break;
        case 'p':
            if (strtok(NULL, WHITE_CHARS) == NULL) {
                char *temp = gamma_board(g);
                printf("%s", temp);
                free(temp);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}
