#include "game.h"

#include <stdlib.h>
#include <string.h>

static int in_bounds(const Game *game, int x, int y)
{
    return x >= 0 && y >= 0 && x < game->width && y < game->height;
}

static int wall_at(const Game *game, int x, int y)
{
    return !in_bounds(game, x, y) || game->floor[y][x] == '#';
}

int game_load(Game *game, int index)
{
    int x, y, players = 0, boxes = 0, goals = 0;
    const Level *level;
    if (index < 0 || index >= LEVEL_COUNT) return 0;
    level = &levels[index];
    memset(game, 0, sizeof(*game));
    game->width = (int)strlen(level->rows[0]);
    game->height = level->height;
    game->level_index = index;
    if (game->width < 3 || game->width > MAX_WIDTH ||
        game->height < 3 || game->height > MAX_HEIGHT) return 0;
    for (y = 0; y < game->height; ++y) {
        if ((int)strlen(level->rows[y]) != game->width) return 0;
        for (x = 0; x < game->width; ++x) {
            char tile = level->rows[y][x];
            if (strchr("# .o$@*+", tile) == NULL) return 0;
            if ((x == 0 || y == 0 || x == game->width - 1 ||
                 y == game->height - 1) && tile != '#') return 0;
            game->floor[y][x] = tile == '#' ? '#' : '.';
            if (tile == 'o' || tile == '*' || tile == '+') {
                game->floor[y][x] = 'o';
                ++goals;
            }
            if (tile == '$' || tile == '*') {
                game->state.boxes[y][x] = 1;
                ++boxes;
            }
            if (tile == '@' || tile == '+') {
                game->state.x = x;
                game->state.y = y;
                ++players;
            }
        }
    }
    return players == 1 && boxes > 0 && boxes == goals;
}

int game_move(Game *game, int dx, int dy)
{
    int x, y, bx, by, pushing;
    if (abs(dx) + abs(dy) != 1) return 0;
    x = game->state.x + dx;
    y = game->state.y + dy;
    if (wall_at(game, x, y)) return 0;
    pushing = game->state.boxes[y][x] != 0;
    bx = x + dx;
    by = y + dy;
    if (pushing && (wall_at(game, bx, by) || game->state.boxes[by][bx])) return 0;
    if (game->history_count == MAX_HISTORY) {
        memmove(game->history, game->history + 1,
                (MAX_HISTORY - 1) * sizeof(game->history[0]));
        --game->history_count;
    }
    game->history[game->history_count++] = game->state;
    if (pushing) {
        game->state.boxes[y][x] = 0;
        game->state.boxes[by][bx] = 1;
        ++game->state.pushes;
    }
    game->state.x = x;
    game->state.y = y;
    ++game->state.steps;
    return 1;
}

int game_undo(Game *game)
{
    if (game->history_count == 0) return 0;
    game->state = game->history[--game->history_count];
    return 1;
}

int game_box_count(const Game *game)
{
    int x, y, count = 0;
    for (y = 0; y < game->height; ++y)
        for (x = 0; x < game->width; ++x)
            count += game->state.boxes[y][x] != 0;
    return count;
}

int game_boxes_on_goals(const Game *game)
{
    int x, y, count = 0;
    for (y = 0; y < game->height; ++y)
        for (x = 0; x < game->width; ++x)
            count += game->state.boxes[y][x] && game->floor[y][x] == 'o';
    return count;
}

int game_won(const Game *game)
{
    return game_boxes_on_goals(game) == game_box_count(game);
}

int game_corner_warning(const Game *game)
{
    int x, y;
    for (y = 0; y < game->height; ++y) {
        for (x = 0; x < game->width; ++x) {
            if (game->state.boxes[y][x] && game->floor[y][x] != 'o' &&
                (wall_at(game, x - 1, y) || wall_at(game, x + 1, y)) &&
                (wall_at(game, x, y - 1) || wall_at(game, x, y + 1))) return 1;
        }
    }
    return 0;
}
