#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);
static void set_board_at(game_state_t *state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t *state, int snum);
static char next_square(game_state_t *state, int snum);
static void update_tail(game_state_t *state, int snum);
static void update_head(game_state_t *state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t *state, int x, int y)
{
    return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t *state, int x, int y, char ch)
{
    state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state()
{
    // TODO: Implement this function.
    // Initial state
    int board_row_num = 10;
    int board_col_num = 14;
    int fruit_row = 2;
    int fruit_col = 9;
    int tail_row = 4;
    int tail_col = 4;
    int head_row = 4;
    int head_col = 5;

    // Initialization
    snake_t *default_snakes = (snake_t *)malloc(sizeof(snake_t));
    default_snakes->tail_x = tail_col;
    default_snakes->tail_y = tail_row;
    default_snakes->head_x = head_col;
    default_snakes->head_y = head_row;
    default_snakes->live = true;
    game_state_t *default_state = (game_state_t *)malloc(sizeof(game_state_t));
    default_state->x_size = board_col_num;
    default_state->y_size = board_row_num;
    default_state->num_snakes = 1;
    default_state->snakes = default_snakes;
    default_state->board = (char **)malloc(sizeof(char *) * default_state->y_size);
    for (int r = 0; r < default_state->y_size; r += 1)
    {
        default_state->board[r] = (char *)malloc(sizeof(char) * (default_state->x_size + 1));
        if (r == 0 || r == default_state->y_size - 1)
        {
            strcpy(default_state->board[r], "##############");
        }
        else
        {
            strcpy(default_state->board[r], "#            #");
        }
    }
    set_board_at(default_state, fruit_col, fruit_row, '*');
    set_board_at(default_state, tail_col, tail_row, 'd');
    set_board_at(default_state, head_col, head_row, '>');
    return default_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
    // TODO: Implement this function.
    free(state->snakes);
    for (int r = 0; r < state->y_size; r += 1)
    {
        free(state->board[r]);
    }
    free(state->board);
    free(state);
    return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
    // TODO: Implement this function.
    for (int r = 0; r < state->y_size; r += 1)
    {
        fprintf(fp, "%s\n", state->board[r]);
    }
    return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename)
{
    FILE *f = fopen(filename, "w");
    print_board(state, f);
    fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c)
{
    // TODO: Implement this function.
    if (c == 'w' || c == 'a' || c == 's' || c == 'd')
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool is_snake(char c)
{
    // TODO: Implement this function.
    if (is_tail(c) || c == '^' || c == 'v' || c == '<' || c == '>' || c == 'x')
    {
        return true;
    }
    else
    {
        return false;
    }
}

static char body_to_tail(char c)
{
    // TODO: Implement this function.
    switch (c)
    {
    case '^':
        return 'w';
    case '<':
        return 'a';
    case 'v':
        return 's';
    case '>':
        return 'd';
    default:
        return '\0';
    }
}

static int incr_x(char c)
{
    // TODO: Implement this function.
    if (c == 'd' || c == '>')
    {
        return 1;
    }
    else if (c == 'a' || c == '<')
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int incr_y(char c)
{
    // TODO: Implement this function.
    if (c == 's' || c == 'v')
    {
        return 1;
    }
    else if (c == 'w' || c == '^')
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    int head_x = state->snakes[snum].head_x;
    int head_y = state->snakes[snum].head_y;
    char head_char = get_board_at(state, head_x, head_y);
    return get_board_at(state, head_x + incr_x(head_char), head_y + incr_y(head_char));
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    int head_x = state->snakes[snum].head_x;
    int head_y = state->snakes[snum].head_y;
    char head_char = get_board_at(state, head_x, head_y);
    state->snakes[snum].head_x += incr_x(head_char);
    state->snakes[snum].head_y += incr_y(head_char);
    set_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y, head_char);
    return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    int tail_x = state->snakes[snum].tail_x;
    int tail_y = state->snakes[snum].tail_y;
    char tail_char = get_board_at(state, tail_x, tail_y);
    int next_tail_x = tail_x + incr_x(tail_char);
    int next_tail_y = tail_y + incr_y(tail_char);
    char next_tail_char = get_board_at(state, next_tail_x, next_tail_y);
    state->snakes[snum].tail_x = next_tail_x;
    state->snakes[snum].tail_y = next_tail_y;
    set_board_at(state, tail_x, tail_y, ' ');
    set_board_at(state, next_tail_x, next_tail_y, body_to_tail(next_tail_char));
    return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
    // TODO: Implement this function.
    for (int i = 0; i < state->num_snakes; i += 1)
    {
        if (state->snakes[i].live)
        {
            char square = next_square(state, i);
            if (square == ' ')
            {
                update_head(state, i);
                update_tail(state, i);
            }
            else if (is_snake(square) || square == '#')
            {
                state->snakes[i].live = false;
                set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y, 'x');
            }
            else if (square == '*')
            {
                update_head(state, i);
                add_food(state);
            }
        }
    }
    return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
    // TODO: Implement this function.
    game_state_t *game_state = (game_state_t *)malloc(sizeof(game_state_t));

    int x_size = 0;
    int y_size = 0;
    char c;
    FILE *fp;
    fp = fopen(filename, "r");
    while ((c = getc(fp)) != EOF)
    {
        if (c == '\n')
        {
            y_size += 1;
        }
        if (y_size == 0)
        {
            x_size += 1;
        }
    }
    fclose(fp);

    game_state->x_size = x_size;
    game_state->y_size = y_size;
    game_state->board = (char **)malloc(sizeof(char *) * y_size);

    fp = fopen(filename, "r");
    for (int i = 0; i < y_size; i += 1)
    {
        char *line = (char *)malloc(sizeof(char) * (x_size + 1));
        fscanf(fp, "%[^\n]%*c", line);
        game_state->board[i] = (char *)malloc(sizeof(char) * (x_size + 1));
        strcpy(game_state->board[i], line);
        free(line);
    }
    fclose(fp);

    return game_state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    int x, y;
    int next_x = state->snakes[snum].tail_x;
    int next_y = state->snakes[snum].tail_y;
    char square = get_board_at(state, next_x, next_y);
    while (is_snake(square))
    {
        x = next_x;
        y = next_y;
        next_x = x + incr_x(square);
        next_y = y + incr_y(square);
        square = get_board_at(state, next_x, next_y);
    }
    state->snakes[snum].head_x = x;
    state->snakes[snum].head_y = y;
    return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
    // TODO: Implement this function.
    int num_snakes = 0;
    int array_size = 1;
    state->snakes = (snake_t *)malloc(sizeof(snake_t) * array_size);
    for (int y = 0; y < state->y_size; y += 1)
    {
        for (int x = 0; x < state->x_size; x += 1)
        {
            if (is_tail(get_board_at(state, x, y)))
            {
                if (num_snakes + 1 > array_size)
                {
                    array_size *= 2;
                    state->snakes = realloc(state->snakes, sizeof(snake_t) * array_size);
                }
                state->snakes[num_snakes].tail_x = x;
                state->snakes[num_snakes].tail_y = y;
                state->snakes[num_snakes].live = true;
                find_head(state, num_snakes);
                num_snakes += 1;
            }
        }
    }
    state->snakes = realloc(state->snakes, sizeof(snake_t) * num_snakes);
    state->num_snakes = num_snakes;
    return state;
}
