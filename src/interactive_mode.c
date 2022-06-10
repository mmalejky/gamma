/** @file
 * Implementacja interfejsu trybu interaktywnego gry gamma.
 *
 * @author Marcin Malejky
 */

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "gamma.h"
#include "interactive_mode.h"

#define ESC 27 /**< Kod znaku ESCAPE. */
#define EOT 4 /**< Kod znaku końca pliku. */
#define DECIMAL_BASE 10 /**< Baza systemu dziesiątkowego. */
#define BLACK_FG 30 /**< Kolor czarny tekstu. */
#define WHITE_FG 37 /**< Kolor biały tekstu. */
#define YELLOW_FG 33 /**< Kolor żółty tekstu. */
#define CYAN_FG 36 /**< Kolor cyan tekstu. */
#define BLUE_FG 34 /**< Kolor niebieski tekstu. */
#define FG_TO_BG_OFFSET 10 /**< Różnica międzu numerem koloru tła a tekstu. */
#define BG_COLOR_1 BLUE_FG + FG_TO_BG_OFFSET /**< Pierwszy kolor tła. */
#define BG_COLOR_2 CYAN_FG + FG_TO_BG_OFFSET /**< Drugi kolor tła. */

/** @brief Podaje numer następnego gracza po ACTUAL w grze o LIMIT graczach.
 * @param[in] actual - aktualny numer,
 * @param[in] limit  - ostatni numer,
 * @return Numer kolejnego gracza.
 */
static uint32_t next(uint32_t actual, uint32_t limit);

/** @brief Rozpoczyna surowy tryb odczytu z terminala (znak po znaku).
 * Zapisuje poprzednie ustawienia terminala na *ORIG_TERMIOS.
 * @param[out] orig_termios - domyślne ustawienia terminala,
 */
static void enable_raw_mode(struct termios *orig_termios);

/** @brief Wykonuje akcje gracza.
 * Wykonuję akcję (ruch, złoty ruch lub pominięcie ruchu) lub pozwala na ruch
 * kursorem w trybie interaktywnym gry GAME.
 * @param g - wskaźnik na strukturę przechowującą grę,
 * @return Wartość TRUE jeżeli gracz wykonał akcję, a FALSE w przeciwnym
 * przypadku.
 */
static bool player_action(gamma_t *g);

/** @brief Wykonuje ruch kursorem terminala.
 * Wykonuje ruch kursorem terminala w grze GAME w trybie interaktywnym.
 * Ruch wykonuje na podstawie następego odczytanego znaku z wejścia według
 * ANSI escape codes tj. A-góra, B-dół, C-prawo, D-lewo.
 * @param[in,out] g - wskaźnik na strukturę przechowującą grę,
 */
static void move_cursor(gamma_t *g);

/** @brief Podaje maksimum z dwóch liczb: X i Y.
 * @param[in] x - porównywana liczba,
 * @param[in] y - porównywana liczba,
 * @return Większa z liczb X i Y
 */
static uint32_t max(uint32_t x, uint32_t y);

/** @brief Przywraca zapisane położenie kursora.
 * @param[in] g - wskaźnik na strukturę przechowującą grę,
 */
static void restore_cursor(gamma_t *g);

/** @brief Podaje numer koloru tła w ANSI escape codes.
 * Podaje numer koloru tła pola (X, Y) w ANSI escape codes, tworząc szachownice.
 * @param[in] x - odcięta pola,
 * @param[in] y - rzędna pola,
 * @return Numer koloru.
 */
static int bg_color(uint32_t x, uint32_t y);

/** @brief Podaje numer koloru tekstu w ANSI escape codes.
 * Podaje numer koloru tekstu pola (X, Y) w ANSI escape codes tak, aby
 * kontrastował z tłem szachownicy.
 * @param[in] x - odcięta pola,
 * @param[in] y - rzędna pola,
 * @return Numer koloru.
 */
static int fg_color(uint32_t x, uint32_t y);

/** @brief Podaje liczbę cyfr danej liczby.
 * Podaje liczbę cyfr danej liczby całkowitej w zapisie dziesiętnym.
 * @param[in] n       – liczba,
 * @return Liczba cyfr danej liczby.
 * */
static uint32_t digit_count(uint32_t n);

/** @brief Sprawdza, czy wydruk planszy i opis zmieści się w terminalu.
 * @param[in] g - wskaźnik na strukturę przechowującą grę,
 * @return Wartość TRUE, jeżeli wydruk planszy i opis zmieści się w terminalu.
 * FALSE w przeciwnym przypadku.
 * */
static bool check_terminal_size(gamma_t *g);

/** @brief Przywraca poprzednie ustawienia terminala.
 * Przywraca poprzednie ustawienia terminala i pokazuje kursor.
 * @param[in] orig_termios - struktura terminala,
 * */
static void restore_terminal(struct termios orig_termios);

/** @brief Wypisuje pole szachownicy gry.
 * Wypisuje pole nad którym aktualnie jest kursor gry, uwzględniając wzór
 * szachownicy. Jeżeli HIGHLITED to wypisuje pole w negatywie. Cofa kursor do
 * lewego krańca pola po wypisaniu.
 * @param[in] g           - wskaźnik na strukturę przechowującą grę,
 * @param[in] highlighted - czy ma być wypisane w negatywie,
 * */
static void print_field(gamma_t *g, bool highlighted);

/** @brief Wypisuje podsumowanie gry.
 * Wypisuje podsumowanie: dla każdego gracza ile pól zajął.
 * @param[in] g - wskaźnik na strukturę przechowującą grę,
 * */
static void print_scoreboard(gamma_t *g);

/** @brief Wypisuje startową planszę gry.
 * Wypisuje startową planszę gry w formie szachownicy.
 * @param[in] g - wskaźnik na strukturę przechowującą grę,
 * */
static void print_initial_board(gamma_t *g);

static void enable_raw_mode(struct termios *orig_termios) {
    tcgetattr(STDIN_FILENO, orig_termios);
    struct termios raw = *orig_termios;
    raw.c_lflag &= ~ECHO;
    raw.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf("%c[?25l", ESC);
}

static void restore_cursor(gamma_t *g) {
    printf("%c[%d;%dH", ESC, g->height - g->y, g->x * g->frame + 1);
}


static void restore_terminal(struct termios orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf("%c[?25h", ESC);
}

static uint32_t digit_count(uint32_t n) {
    if (n == 0) {
        return 1;
    }
    int digits = 0;
    while (n != 0) {
        n /= DECIMAL_BASE;
        ++digits;
    }
    return digits;
}

static int bg_color(uint32_t x, uint32_t y) {
    if ((x + y) % 2 == 0) {
        return BG_COLOR_1;
    } else {
        return BG_COLOR_2;
    }
}

static bool check_terminal_size(gamma_t *g) {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return (w.ws_col >= max(g->frame * g->width,
                            11 + g->frame +
                            2 * digit_count(g->width * g->height))) &&
           (w.ws_row >= g->height + g->player_count);
}

static void print_field(gamma_t *g, bool highlighted) {
    uint32_t owner = get_owner(g, g->x, g->y);
    if (highlighted) {
        printf("%c[%d;%dm", ESC, BLACK_FG, WHITE_FG + FG_TO_BG_OFFSET);
    } else {
        printf("%c[%d;%dm", ESC, fg_color(g->x, g->y),
               bg_color(g->x, g->y));
    }
    if (owner == 0) {
        printf("%-*c", g->frame, '.');
    } else {
        printf("%-*d", g->frame, owner);
    }
    printf("%c[0m", ESC);
    for (uint32_t i = 0; i < g->frame; ++i) {
        printf("%c[D", ESC);
    }
}

static int fg_color(uint32_t x, uint32_t y) {
    if ((x + y) % 2 == 0) {
        return WHITE_FG;
    } else {
        return BLACK_FG;
    }
}

static uint32_t max(uint32_t x, uint32_t y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

static void print_scoreboard(gamma_t *g) {
    int fg_color = WHITE_FG;
    int bg_color = BLACK_FG + FG_TO_BG_OFFSET;
    for (uint32_t i = 1; i <= g->player_count; ++i) {
        printf("%c[%d;%dm", ESC, fg_color, bg_color);
        printf("PLAYER %-*d %lu\n", g->frame, i, gamma_busy_fields(g, i));
        printf("%c[0m", ESC);
        int temp = fg_color + FG_TO_BG_OFFSET;
        fg_color = bg_color - FG_TO_BG_OFFSET;
        bg_color = temp;
    }
}

static void print_initial_board(gamma_t *g) {
    for (uint32_t i = 0; i < g->height; ++i) {
        for (uint32_t j = 0; j < g->width; ++j) {
            printf("%c[%d;%dm", ESC, fg_color(j, i), bg_color(j, i));
            printf(".");
            for (uint32_t k = 0; k < g->frame - 1; ++k) {
                printf(" ");
            }
            printf("%c[0m", ESC);
        }
        printf("%c[F", ESC);
    }
}

void interactive_mode(gamma_t *g) {
    if (!check_terminal_size(g)) {
        printf("Niewystarczający rozmiar terminala.\n");
        return;
    }
    struct termios orig_termios; /* Domyślny terminal. */
    enable_raw_mode(&orig_termios);
    printf("%c[2J", ESC);
    printf("%c[;H", ESC);
    restore_cursor(g);
    print_initial_board(g);
    restore_cursor(g);
    printf("%c[%d;%dm", ESC, BLACK_FG, WHITE_FG + FG_TO_BG_OFFSET);
    printf(".");
    for (uint32_t k = 0; k < g->frame - 1; ++k) {
        printf(" ");
    }
    printf("%c[0m", ESC);
    while (g->counter < g->player_count) {
        uint64_t busy_count = gamma_busy_fields(g, g->player);
        uint64_t free_count = gamma_free_fields(g, g->player);
        bool golden_possible = gamma_golden_possible(g, g->player);

        if (!golden_possible && !free_count) {
            g->player = next(g->player, g->player_count);
            ++(g->counter);
            continue;
        } else {
            g->counter = 0;
        }
        printf("%c[%dE", ESC, g->y + 1);
        printf("%c[2K", ESC);
        printf("PLAYER %d %lu %lu", g->player, busy_count, free_count);
        if (golden_possible) {
            printf("%c[%d;%dm", ESC, YELLOW_FG,
                   BLACK_FG + FG_TO_BG_OFFSET);
            printf(" G");
            printf("%c[0m", ESC);
        }
        restore_cursor(g);
        while (!player_action(g));
        g->player = next(g->player, g->player_count);
    }
    printf("%c[%dE", ESC, g->y + 1);
    printf("%c[2K", ESC);
    restore_terminal(orig_termios);
    print_scoreboard(g);
}

static uint32_t next(uint32_t actual, uint32_t limit) {
    if (actual == limit) {
        return 1;
    }
    return actual + 1;
}

static bool player_action(gamma_t *g) {
    int c = getchar();
    switch (c) {
        case ESC:
            if (getchar() != '[') {
                break;
            }
            move_cursor(g);
            break;
        case ' ':
            if (gamma_move(g, g->player, g->x, g->y)) {
                print_field(g, true);
                return true;
            }
            break;
        case 'g':
        case 'G':
            if (gamma_golden_move(g, g->player, g->x, g->y)) {
                print_field(g, true);
                return true;
            }
            break;
        case 'c':
        case 'C':
            return true;
        case EOT:
            g->counter = g->player_count;
            return true;
        default:
            break;
    }
    return false;
}

static void move_cursor(gamma_t *g) {
    int c = getchar();
    print_field(g, false);
    if (c == 'A' && g->y < g->height - 1) {
        printf("%c[%c", ESC, c);
        ++(g->y);
    } else if (c == 'B' && g->y > 0) {
        printf("%c[%c", ESC, c);
        --(g->y);
    } else if (c == 'C' && g->x < g->width - 1) {
        for (uint32_t i = 0; i < g->frame; ++i) {
            printf("%c[%c", ESC, c);
        }
        ++(g->x);
    } else if (c == 'D' && g->x > 0) {
        for (uint32_t i = 0; i < g->frame; ++i) {
            printf("%c[%c", ESC, c);
        }
        --(g->x);
    }
    print_field(g, true);
}

