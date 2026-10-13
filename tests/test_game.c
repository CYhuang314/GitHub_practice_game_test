#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void all_levels_are_valid(void)
{
    Game game;
    int i, x, y;
    for (i = 0; i < LEVEL_COUNT; ++i) {
        int goals = 0;
        assert(game_load(&game, i));
        assert(!game_won(&game));
        assert(game.state.steps == 0 && game.history_count == 0);
        assert(levels[i].par > 0);
        for (y = 0; y < game.height; ++y)
            for (x = 0; x < game.width; ++x) goals += game.floor[y][x] == 'o';
        assert(goals == game_box_count(&game));
    }
    assert(!game_load(&game, -1));
    assert(!game_load(&game, LEVEL_COUNT));
}

static void movement_and_undo(void)
{
    Game game;
    BoardState before;
    assert(game_load(&game, 0));
    before = game.state;
    assert(!game_move(&game, -1, 0));
    assert(memcmp(&before, &game.state, sizeof(before)) == 0);
    assert(!game_move(&game, 1, 1));
    assert(!game_undo(&game));
    assert(game_move(&game, 1, 0));
    assert(game_won(&game));
    assert(game.state.steps == 1 && game.state.pushes == 1);
    assert(game_undo(&game));
    assert(memcmp(&before, &game.state, sizeof(before)) == 0);
    assert(!game_won(&game));

    /* Push a patch into the bottom wall: the second push must be blocked. */
    assert(game_move(&game, 0, -1));
    assert(game_move(&game, 1, 0));
    assert(game_move(&game, 0, 1));
    before = game.state;
    assert(!game_move(&game, 0, 1));
    assert(memcmp(&before, &game.state, sizeof(before)) == 0);
    assert(game_undo(&game));
    assert(game.state.pushes == 0);

    /* A box pushed into a non-goal corner must warn, undo must remove it. */
    assert(game_move(&game, 0, 1));
    assert(game_move(&game, 1, 0));
    assert(game_move(&game, 0, 1));
    assert(game_move(&game, -1, 0));
    assert(game_corner_warning(&game));
    assert(game_undo(&game));
    assert(!game_corner_warning(&game));

    /* Two stacked boxes cannot be pushed as a chain. */
    assert(game_load(&game, 2));
    assert(game_move(&game, 1, 0));
    assert(game_move(&game, 1, 0));
    before = game.state;
    assert(!game_move(&game, 0, -1));
    assert(memcmp(&before, &game.state, sizeof(before)) == 0);
    assert(game_load(&game, 0));
    assert(game.state.steps == 0 && game.history_count == 0);

    /* 300 legal moves: history is capped, with the newest 256 undoable. */
    {
        int i;
        for (i = 0; i < 75; ++i) {
            assert(game_move(&game, 0, -1));
            assert(game_move(&game, 1, 0));
            assert(game_move(&game, -1, 0));
            assert(game_move(&game, 0, 1));
        }
        assert(game.history_count == MAX_HISTORY);
        for (i = 0; i < MAX_HISTORY; ++i) assert(game_undo(&game));
        assert(!game_undo(&game));
        assert(game.state.steps == 300 - MAX_HISTORY);
    }
}

static void incomplete_tutorials_do_not_unlock_challenges(void)
{
    Progress progress;
    int i;
    progress_init(&progress);
    for (i = TUTORIAL_COUNT; i < LEVEL_COUNT; ++i)
        assert(!levels[i].unlock(&progress));
    progress_complete(&progress, 0, 3);
    progress_complete(&progress, 1, 6);
    assert(!unlock_commit(&progress));
    assert(!unlock_merge(&progress));
    progress_init(&progress);
    progress_complete(&progress, 0, 1);
    for (i = TUTORIAL_COUNT; i < LEVEL_COUNT; ++i)
        assert(!levels[i].unlock(&progress));
    progress_init(&progress);
    progress_complete(&progress, 1, 4);
    for (i = TUTORIAL_COUNT; i < LEVEL_COUNT; ++i)
        assert(!levels[i].unlock(&progress));
}

static void save_round_trip(void)
{
    Progress original, loaded;
    FILE *file;
    const char *path = "test-progress.save";
    progress_init(&original);
    progress_complete(&original, 0, 9);
    progress_complete(&original, 0, 1);
    progress_complete(&original, 0, 5);
    progress_complete(&original, 1, 4);
    assert(original.best_steps[0] == 1);
    assert(progress_tutorial_count(&original) == 2);
    assert(progress_save(&original, path));
    assert(progress_load(&loaded, path));
    assert(memcmp(&original, &loaded, sizeof(original)) == 0);
    file = fopen(path, "w");
    assert(file != NULL);
    fputs("GPG1\n", file);
    {
        int i;
        for (i = 0; i < 10; ++i)
            fprintf(file, "%d %d %d\n", i, original.completed[i], original.best_steps[i]);
    }
    assert(fclose(file) == 0);
    assert(progress_load(&loaded, path));
    assert(progress_tutorial_count(&loaded) == 2);
    assert(loaded.best_steps[0] == 1);
    assert(!loaded.completed[LEVEL_COUNT - 1]);
    file = fopen(path, "w");
    assert(file != NULL);
    fputs("GPG1\n0 1 1\n1 1 -1\n", file);
    assert(fclose(file) == 0);
    assert(!progress_load(&loaded, path));
    assert(progress_tutorial_count(&loaded) == 0);
    assert(remove(path) == 0);
}

int main(void)
{
    all_levels_are_valid();
    movement_and_undo();
    incomplete_tutorials_do_not_unlock_challenges();
    save_round_trip();
    printf("PASS: %d valid maps, movement, pushing, undo history, restart,\n", LEVEL_COUNT);
    puts("      tutorial prerequisites, best scores, save/load and invalid saves.");
    return 0;
}
