/** @file
 * Implementacja interfejsu klasy przechowującej stan gry.
 * @author Marcin Malejky
 */

#include <stdio.h>
#include <stdlib.h>
#include "gamma.h"

#define NOBODY 0 /**< Domyślny posiadacz pustego pola. */
#define EMPTY 0 /**< Domyślne id obszaru pustego pola. */
#define SIDE_COUNT 4 /**< Liczba boków pola. */

/** @brief Sprawdza czy struktura stanu gry i numer gracza są poprawne.
 * Sprawdza, czy struktura @p g jest zaalokowana i czy numer gracza @p player
 * jest poprawnym numerem gracza.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @return Wartość @p true, jeśli @p g jest zaalokowane i @p player jest
 * poprawnym numerem gracza, a @p false w przeciwnym przypadku.
 */
static bool player_correct(gamma_t *g, uint32_t player);

/** @brief Inicjalizuje planszę.
 * Alokuje pamięć na posiadacza oraz id obszaru każdego pola na planszy
 * o szerokości @p width i wysokości @p height.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] width   – szerokość plaszy, liczba dodatnia,
 * @param[in] height  – wysokość plaszy, liczba dodatnia,
 * @return Wartość @p true, jeśli pomyślnie zaalokowano pamięć na planszę,
 * a @p false w przeciwnym przypadku lub gdy któryś z parametrów jest
 * niepoprawny.
 * */
static bool initialize_board(gamma_t *g, uint32_t width, uint32_t height);

/** @brief Podaje liczbę cyfr danej liczby.
 * Podaje liczbę cyfr danej liczby całkowitej w zapisie dziesiętnym.
 * @param[in] n       – liczba,
 * @return Liczba cyfr danej liczby.
 * */
static uint32_t digit_count(uint32_t n);

/** @brief Podaje id sąsiadującego obszaru należącego do gracza.
 * Podaje id obszaru należącecgo do gracza @p player sąsiadującego
 * z polem (@p x, @p y) lub zero jeżeli taki nie istnieje.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Id obszaru należącego do gracza oraz sąsiadującego z polem (x, y)
 * lub zero jeżeli taki obszar nie istnieje lub któryś z parametrów jest
 * niepoprawny.
 * */
static uint32_t bordering_area_id(gamma_t *g, uint32_t player, uint32_t x,
                                  uint32_t y);

/** @brief Łączy wszytkie sąsiadujące pola gracza w jeden obszar.
 * Łączy wszytkie sąsiadujące (z polem (@p x, @p y)) pola gracza @p player
 * w jeden obszar. Omija obszary o id w tablicy @p ids o długości @p length.
 * Id nowego obszaru ustala na ostatnią liczbę tablicy @p ids.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @param[in] ids     - tablica id pomijanych obszarow o długości length,
 * @param[in] length  - liczba elementów tablicy ids,
 * */
static void merge_areas(gamma_t *g, uint32_t player, uint32_t x,
                        uint32_t y, uint32_t const *ids, uint32_t length);

/** @brief Dodaje element do tablicy jeżeli jest różny od zawartości.
 * Dodaje liczbę @p x do tablicy @p array o długości pod wskażnikiem @p length
 * wtedy, gdy jest różna od wszystkich liczb w tablicy. Po dodaniu modyfikuje
 * długość tablicy.
 * @param[in,out] array   – tablica liczb całkowitych o długości @p length,
 * @param[in] length      – liczba elementów tablicy @p array,
 * @param[in] x           – dodawana liczba całkowita,
 * */
static void add_distinct(uint32_t *array, uint32_t *length, uint32_t x);

/** @brief Podaje liczbę różnych sąsiadujących obszarów gracza.
 * Podaje liczbę różnych obszarów gracza @p player sąsiadujących z polem
 * (@p x, @p y).
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Liczba różnych obszarów gracza @p player sąsiadujących z polem
 * (@p x, @p y) lub zero jeżeli któryś z parametrów jest nieporawny.
 * */
static uint32_t distinct_neighbour_count(gamma_t *g, uint32_t player,
                                         uint32_t x, uint32_t y);

/** @brief Sprawdza poprawność parametrów dla funkcji gamma_golden_move.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return TRUE jeżeli parametry są poprawne, FALSE w przeciwnym razie.
 * */
static bool check_golden_move_parameters(gamma_t *g, uint32_t player,
                                         uint32_t x, uint32_t y);

/** @brief Sprawdza czy możliwy jest złoty ruch.
 * Jeżeli parametry są poprawne wykonuje złoty ruch według parametrów i jeżeli
 * ruch nie łamie zasad gry to go nie cofa i zwraca TRUE. W przeciwnym wypadku
 * cofa ruch i zwraca false. Jeżeli ALWAYS_REVERSE to zawsze cofa ruch,
 * a podaje informacje czy ruch jest w pełni legalny.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @param[in] always_reverse – czy zawsze cofać ruch
 * @return TRUE, jeżeli złoty ruch jest w pełni legalny, FALSE w przeciwnym
 * razie.
 * */
static bool
golden_move_possible(gamma_t *g, uint32_t player, uint32_t x, uint32_t y,
                     bool always_reverse);

static bool player_correct(gamma_t *g, uint32_t player) {
    if (!g || player == NOBODY || player > g->player_count) {
        return false;
    } else {
        return true;
    }
}

static bool initialize_board(gamma_t *g, uint32_t width, uint32_t height) {
    if (!g || width < 1 || height < 1) {
        return false;
    }
    g->owner = malloc(sizeof(uint32_t *) * height);
    if (!g->owner) {
        return false;
    }
    g->area_id = malloc(sizeof(uint32_t *) * height);
    if (!g->area_id) {
        free(g->owner);
        return false;
    }
    bool ok = true;
    uint32_t y;
    for (y = 0; y < height && ok; ++y) {
        g->owner[y] = malloc(sizeof(uint32_t) * width);
        if (!g->owner[y]) {
            ok = false;
            g->area_id[y] = NULL;
        } else {
            g->area_id[y] = malloc(sizeof(uint32_t) * width);
            if (!g->area_id[y]) {
                ok = false;
            }
            for (uint32_t x = 0; x < width && ok; ++x) {
                g->owner[y][x] = NOBODY;
                g->area_id[y][x] = EMPTY;
            }
        }
    }
    if (!ok) {
        for (uint32_t i = 0; i < y; ++i) {
            free(g->owner[i]);
            free(g->area_id[i]);
        }
        free(g->owner);
        free(g->area_id);
    }
    return ok;
}

static uint32_t digit_count(uint32_t n) {
    if (n == 0) {
        return 1;
    }
    int digits = 0;
    while (n != 0) {
        n /= 10;
        ++digits;
    }
    return digits;
}

static uint32_t bordering_area_id(gamma_t *g, uint32_t player,
                                  uint32_t x, uint32_t y) {
    if (!player_correct(g, player) || x >= g->width || y >= g->height) {
        return 0;
    }
    uint32_t id = 0;
    if (x > 0 && g->owner[y][x - 1] == player) {
        id = g->area_id[y][x - 1];
    }
    if (x < (g->width - 1) && g->owner[y][x + 1] == player) {
        id = g->area_id[y][x + 1];
    }
    if (y > 0 && g->owner[y - 1][x] == player) {
        id = g->area_id[y - 1][x];
    }
    if (y < (g->height - 1) && g->owner[y + 1][x] == player) {
        id = g->area_id[y + 1][x];
    }
    return id;
}

static void merge_areas(gamma_t *g, uint32_t player, uint32_t x,
                        uint32_t y, uint32_t const *ids, uint32_t length) {
    if (!player_correct(g, player) || x >= g->width || y >= g->height ||
        length < 1) {
        return;
    }
    if (g->owner[y][x] != player) {
        return;
    }
    bool skip = false;
    uint32_t previous_id = g->area_id[y][x];
    for (uint32_t i = 0; i < length && !skip; ++i) {
        if (previous_id == ids[i]) {
            skip = true;
        }
    }
    if (skip) {
        return;
    }
    g->area_id[y][x] = ids[length - 1];
    merge_areas(g, player, x - 1, y, ids, length);
    merge_areas(g, player, x + 1, y, ids, length);
    merge_areas(g, player, x, y - 1, ids, length);
    merge_areas(g, player, x, y + 1, ids, length);
}

static void add_distinct(uint32_t *array, uint32_t *length, uint32_t x) {
    for (uint32_t i = 0; i < *length; ++i) {
        if (x == array[i]) {
            return;
        }
    }
    array[*length] = x;
    ++(*length);
}

static uint32_t distinct_neighbour_count(gamma_t *g, uint32_t player,
                                         uint32_t x, uint32_t y) {
    if (!player_correct(g, player) || x >= g->width || y >= g->height) {
        return 0;
    }
    uint32_t ids[SIDE_COUNT]; /* Id różnych sąsiednich obszarów gracza. */
    uint32_t next = 0; /* Liczba elemntów tablicy ids. */
    if (x > 0 && g->owner[y][x - 1] == player) {
        add_distinct(ids, &next, g->area_id[y][x - 1]);
    }
    if (x < (g->width - 1) && g->owner[y][x + 1] == player) {
        add_distinct(ids, &next, g->area_id[y][x + 1]);
    }
    if (y > 0 && g->owner[y - 1][x] == player) {
        add_distinct(ids, &next, g->area_id[y - 1][x]);
    }
    if (y < (g->height - 1) && g->owner[y + 1][x] == player) {
        add_distinct(ids, &next, g->area_id[y + 1][x]);
    }
    return next;
}

gamma_t *gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
    if (width < 1 || height < 1 || players < 1 || areas < 1) {
        return NULL;
    }
    gamma_t *g = malloc(sizeof(gamma_t));
    if (!g) {
        return NULL;
    }
    g->area_count = malloc(sizeof(uint32_t) * (players + 1));
    if (!g->area_count) {
        free(g);
        return NULL;
    }
    g->made_golden_move = malloc(sizeof(bool) * (players + 1));
    if (!g->made_golden_move) {
        free(g->area_count);
        free(g);
        return NULL;
    }
    g->occupied_count = malloc(sizeof(uint64_t) * (players + 1));
    if (!g->occupied_count) {
        free(g->made_golden_move);
        free(g->area_count);
        free(g);
        return NULL;
    }
    if (!initialize_board(g, width, height)) {
        free(g->occupied_count);
        free(g->made_golden_move);
        free(g->area_count);
        free(g);
        return NULL;
    }

    g->width = width;
    g->height = height;
    g->areas_limit = areas;
    for (uint32_t i = 0; i <= players; ++i) {
        g->area_count[i] = 0;
        g->made_golden_move[i] = false;
        g->occupied_count[i] = 0;
    }
    g->free_count = width;
    g->free_count *= height;
    g->next_id = 1;
    g->player_count = players;
    g->frame = digit_count(players);
    return g;
}

void gamma_delete(gamma_t *g) {
    if (g != NULL) {
        if (g->owner != NULL) {
            for (uint32_t y = 0; y < g->height; ++y) {
                free(g->owner[y]);
            }
        }
        if (g->area_id != NULL) {
            for (uint32_t y = 0; y < g->height; ++y) {
                free(g->area_id[y]);
            }
        }
        free(g->owner);
        free(g->area_id);
        free(g->area_count);
        free(g->made_golden_move);
        free(g->occupied_count);
        free(g);
    }
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (!player_correct(g, player)) {
        return false;
    }
    if (x >= g->width || y >= g->height) {
        return false;
    }
    if (g->owner[y][x] > NOBODY) {
        return false;
    }
    uint32_t id = bordering_area_id(g, player, x, y);
    if (!id && g->area_count[player] >= g->areas_limit) {
        return false;
    }

    g->area_count[player] -= distinct_neighbour_count(g, player, x, y) - 1;
    g->owner[y][x] = player;
    ++(g->occupied_count[player]);
    --(g->free_count);

    /* Ustala id łączonego obszaru na jeden z sąsiadujących lub nowy. */
    id = id ? id : g->next_id;
    ++(g->next_id);
    uint32_t ids[1] = {id};
    merge_areas(g, player, x, y, ids, 1);

    return true;
}

static bool check_golden_move_parameters(gamma_t *g, uint32_t player,
                                         uint32_t x, uint32_t y) {
    if (!player_correct(g, player)) {
        return false;
    }
    if (x >= g->width || y >= g->height) {
        return false;
    }
    if (g->made_golden_move[player]) {
        return false;
    }
    if (g->owner[y][x] == player || g->owner[y][x] == NOBODY) {
        return false;
    }
    if (!bordering_area_id(g, player, x, y) &&
        g->area_count[player] >= g->areas_limit) {
        return false;
    }
    return true;
}

static bool
golden_move_possible(gamma_t *g, uint32_t player, uint32_t x, uint32_t y,
                     bool always_reverse) {
    if (!check_golden_move_parameters(g, player, x, y)) {
        return false;
    }
    uint32_t previous_owner = g->owner[y][x];
    g->owner[y][x] = player;
    uint32_t ids[SIDE_COUNT];
    for (uint32_t i = 0; i < SIDE_COUNT; ++i) {
        ids[i] = g->next_id;
        ++(g->next_id);
    }
    merge_areas(g, previous_owner, x - 1, y, ids, 1);
    merge_areas(g, previous_owner, x + 1, y, ids, 2);
    merge_areas(g, previous_owner, x, y - 1, ids, 3);
    merge_areas(g, previous_owner, x, y + 1, ids, 4);
    uint32_t neighbours = distinct_neighbour_count(g, previous_owner, x, y);
    if (always_reverse ||
        g->area_count[previous_owner] + neighbours - 1 > g->areas_limit) {
        g->owner[y][x] = previous_owner;
        merge_areas(g, previous_owner, x, y, ids, 1);
    }
    return g->area_count[previous_owner] + neighbours - 1 <= g->areas_limit;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (!check_golden_move_parameters(g, player, x, y)) {
        return false;
    }
    uint32_t previous_owner = g->owner[y][x];
    if (golden_move_possible(g, player, x, y, false)) {
        uint32_t neighbours = distinct_neighbour_count(g, previous_owner, x, y);
        g->area_count[player] -= distinct_neighbour_count(g, player, x, y) - 1;
        g->area_count[previous_owner] += neighbours - 1;
        ++(g->occupied_count[player]);
        --(g->occupied_count[previous_owner]);
        g->made_golden_move[player] = true;
        uint32_t ids[] = {g->next_id};
        ++(g->next_id);
        merge_areas(g, player, x, y, ids, 1);
        return true;
    }
    return false;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (!player_correct(g, player)) {
        return 0;
    }
    return g->occupied_count[player];
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (!player_correct(g, player)) {
        return 0;
    }
    if (g->area_count[player] < g->areas_limit) {
        return g->free_count;
    }
    if (g->area_count[player] > g->areas_limit) {
        return 0;
    }

    /* Licznik wolnych pól sąsiadujących z polem gracza player. */
    uint64_t counter = 0;
    for (uint32_t y = 0; y < g->height; ++y) {
        for (uint32_t x = 0; x < g->width; ++x) {
            if (g->owner[y][x] == NOBODY) {
                if (bordering_area_id(g, player, x, y)) {
                    ++counter;
                }
            }
        }
    }
    return counter;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (!player_correct(g, player)) {
        return false;
    }
    if (g->made_golden_move[player]) {
        return false;
    }
    for (uint32_t y = 0; y < g->height; ++y) {
        for (uint32_t x = 0; x < g->width; ++x) {
            if (golden_move_possible(g, player, x, y, true)) {
                return true;
            }
        }
    }
    return false;
}

char *gamma_board(gamma_t *g) {
    if (!g) {
        return NULL;
    }

    size_t size = sizeof(char) * (g->frame * g->width + 1) * g->height + 1;

    char *result = malloc(size);
    if (result == NULL) {
        return NULL;
    }

    /* Wskaźnik na niestworzony sufiks wyniku. */
    char *buffer = result;
    /* Indeks kolejnego znaku do wypełnienia. */
    uint32_t next = 0;
    for (uint32_t y = g->height; y > 0; --y) {
        for (uint32_t x = 0; x < g->width; ++x) {
            uint32_t owner = g->owner[y - 1][x];
            if (owner > NOBODY) {
                sprintf(buffer, "%-*d", g->frame, owner);
                buffer += g->frame;
                next += g->frame;
            } else {
                result[next] = '.';
                ++next;
                for (uint32_t i = 0; i < g->frame - 1; ++i) {
                    result[next] = ' ';
                    ++next;
                }
                buffer += g->frame;
            }
        }
        sprintf(buffer, "\n");
        ++buffer;
        ++next;
    }
    return result;
}

uint32_t get_owner(gamma_t *g, int x, int y) {
    return g->owner[y][x];
}
