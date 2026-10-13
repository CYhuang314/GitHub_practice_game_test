#ifndef GAME_H
#define GAME_H

#define MAX_WIDTH 16
#define MAX_HEIGHT 12
#define MAX_HISTORY 256
#define LEVEL_COUNT 30
#define TUTORIAL_COUNT 2

typedef struct {
    int completed[LEVEL_COUNT];
    int best_steps[LEVEL_COUNT];
} Progress;

typedef int (*UnlockRule)(const Progress *progress);
typedef struct {
    const char *name;
    const char *hint;
    const char *const *rows;
    int height;
    int par;
    const char *ticket;
    const char *requirement;
    UnlockRule unlock;
} Level;

typedef struct {
    unsigned char boxes[MAX_HEIGHT][MAX_WIDTH];
    int x, y, steps, pushes;
} BoardState;

typedef struct {
    char floor[MAX_HEIGHT][MAX_WIDTH];
    int width, height, level_index;
    BoardState state;
    BoardState history[MAX_HISTORY];
    int history_count;
} Game;

extern const Level levels[LEVEL_COUNT];
int game_load(Game *game, int index);
int game_move(Game *game, int dx, int dy);
int game_undo(Game *game);
int game_won(const Game *game);
int game_corner_warning(const Game *game);
int game_boxes_on_goals(const Game *game);
int game_box_count(const Game *game);
void progress_init(Progress *progress);
int progress_tutorial_count(const Progress *progress);
void progress_complete(Progress *progress, int index, int steps);
int progress_load(Progress *progress, const char *path);
int progress_save(const Progress *progress, const char *path);
void render_menu(const Progress *progress);
void render_game(const Game *game);
int unlock_warehouse(const Progress *progress);
int unlock_fork(const Progress *progress);
int unlock_staging(const Progress *progress);
int unlock_commit(const Progress *progress);
int unlock_merge(const Progress *progress);
int unlock_branch(const Progress *progress);
int unlock_release(const Progress *progress);
int unlock_final(const Progress *progress);

int unlock_cherry_pick(const Progress *progress);
int unlock_stash(const Progress *progress);
int unlock_revert(const Progress *progress);
int unlock_fetch(const Progress *progress);
int unlock_pull(const Progress *progress);
int unlock_push(const Progress *progress);
int unlock_diff(const Progress *progress);
int unlock_log(const Progress *progress);
int unlock_status(const Progress *progress);
int unlock_tag(const Progress *progress);
int unlock_origin(const Progress *progress);
int unlock_upstream(const Progress *progress);
int unlock_remote(const Progress *progress);
int unlock_branch_garden(const Progress *progress);
int unlock_squash(const Progress *progress);
int unlock_fast_forward(const Progress *progress);
int unlock_checkout(const Progress *progress);
int unlock_bisect(const Progress *progress);
int unlock_reset(const Progress *progress);
int unlock_release_party(const Progress *progress);

#endif
