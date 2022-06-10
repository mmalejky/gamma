/** @file
 * Interfejs trybu interaktywnego gry gamma.
 *
 * @author Marcin Malejky
 */

#include <termios.h>

#ifndef GAMMA_INTERACTIVE_MODE_H
#define GAMMA_INTERACTIVE_MODE_H

/** @brief Rozpoczyna interaktywny tryb wejścia.
 * Rozpoczyna interaktywny tryb wejścia dla gry G.
 * @param[in,out] g - wskaźnik na strukturę przechowującą grę,
 */
void interactive_mode(gamma_t *g);

#endif //GAMMA_INTERACTIVE_MODE_H
