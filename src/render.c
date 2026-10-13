#include "game.h"

#include <stdio.h>

void render_menu(const Progress *progress)
{
    int i;
    puts("\n==========================================================");
    puts("                 GIT PATCH PUSHER");
    puts("       Fix a small issue. Open a whole new puzzle.");
    puts("==========================================================");
    printf("Tutorial stamps: %d/%d\n\n", progress_tutorial_count(progress), TUTORIAL_COUNT);
    for (i = 0; i < LEVEL_COUNT; ++i) {
        int open = levels[i].unlock(progress);
        printf(" %2d  %-19s [%s]", i + 1, levels[i].name,
               open ? (progress->completed[i] ? "DONE" : "OPEN") : "LOCKED");
        if (progress->completed[i]) printf("  best: %d steps", progress->best_steps[i]);
        putchar('\n');
        if (!open) printf("     %s | %s\n", levels[i].ticket, levels[i].requirement);
    }
    puts("\nChoose a level number, H for help, or Q to quit.");
}

void render_game(const Game *game)
{
    int x, y;
    const Level *level = &levels[game->level_index];
    printf("\n--- Level %d: %s ---\n", game->level_index + 1, level->name);
    for (y = 0; y < game->height; ++y) {
        printf("    ");
        for (x = 0; x < game->width; ++x) {
            char tile = game->floor[y][x];
            if (game->state.boxes[y][x]) tile = tile == 'o' ? '*' : '$';
            if (game->state.x == x && game->state.y == y)
                tile = game->floor[y][x] == 'o' ? '+' : '@';
            printf("%c ", tile);
        }
        putchar('\n');
    }
    printf("Steps: %d  Pushes: %d  Delivered: %d/%d", game->state.steps,
           game->state.pushes, game_boxes_on_goals(game), game_box_count(game));
    if (level->par > 0) printf("  Par: %d", level->par);
    putchar('\n');
    puts("@ you | $ patch | o target | * delivered patch | # wall");
    printf("Hint: %s\n", level->hint);
    if (game_corner_warning(game))
        puts("A patch is stuck in a corner. Try U (undo) or R (restart).");
    puts("W/A/S/D + Enter: move | U: undo | R: restart | M: menu | Q: quit");
    puts("You can also enter several moves, for example: ssdd");
}
