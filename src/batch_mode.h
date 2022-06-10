/** @file
 * Interfejs trybu wsadowego gry gamma.
 *
 * @author Marcin Malejky
 */

#include <stdint.h>
#include "gamma.h"

#ifndef GAMMA_BATCH_MODE_H
#define GAMMA_BATCH_MODE_H

/** @brief Rozpoczyna wsadowy tryb wejścia.
 * Rozpoczyna wsadowy tryb wejścia dla gry G. LINE_NUMBER to numer poprzednio
 * przeczytanego wiersza.
 * @param[in,out] g           - wskaźnik na strukturę przechowującą stan gry,
 * @param[in,out] line_number - wskaźnik na numer poprzednio wczytanej linii,
 */
void batch_mode(gamma_t *g, uint32_t *line_number);

/** @brief Zapisuje liczbę na *X.
 * Wykonuję próbę zapisu liczby do *X na podstawie następnego słowa z wejścia.
 * Wymaga porzedniego wywołania funkcji strtok.
 * @param[out] x - wskaźnik na liczbę do zapisania.
 * @return Wartość TRUE jeżeli udało się poprawnie zapisać liczbę na *X, FALSE
 * w przeciwnym przypadku.
 */
bool to_number(uint32_t *x);

/** @brief Sprawdza, czy wiersz LINE jest komentarzem lub jest pusty.
 * @param[in] line – wiersz,
 * @return Wartość @p true, jeśli @p line jest komentarzem lub jest pustym
 * wierszem, a @p false w przeciwnym przypadku.
 */
bool omit(char *line);

/** @brief Sprawdza, czy wiersz LINE składa się z poprawnych znaków.
 * Sprawdza, czy wiersz LINE o długości SIZE składa się z poprawnych znaków.
 * @param[in] line – wiersz,
 * @param[in] size – rozmiar wiersza,
 * @return Wartość TRUE, jeśli wiersz LINE składa się z poprawnych znaków,
 * a FALSE w przeciwnym przypadku.
 */
bool correct_chars(char *line, ssize_t size);

#endif //GAMMA_BATCH_MODE_H
