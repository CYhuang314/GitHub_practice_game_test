#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *save_path = ".git-game-progress";

static void help(void)
{
    puts("\nGit Patch Pusher - a terminal Sokoban game written in C.");
    puts("Push every $ patch onto an o target. You can push, never pull.");
    puts("W/A/S/D + Enter move. U undoes, R restarts, M returns to menu.");
    puts("Blocked moves do not cost steps. Undo restores steps and pushes.");
    puts("First finish the two tutorials, then repair an unlock ticket.");
    puts("Each ticket opens an independent challenge; see workshop/issues/.");
    puts("Best results persist in .git-game-progress in the working directory.");
    puts("\nUsage: git-game [--no-save] [--list] [--level N]");
    puts("       git-game --check-unlock N");
    puts("The unlock check uses a simulated completed tutorial record.");
    puts("It does not change your save or open a locked level for play.");
}

static int parse_level(const char *text)
{
    char *end;
    long id = strtol(text, &end, 10);
    if (end == text || id < 1 || id > LEVEL_COUNT) return -1;
    while (isspace((unsigned char)*end)) ++end;
    return *end == '\0' ? (int)id - 1 : -1;
}

static int read_line(char *line, size_t size)
{
    size_t len;
    int ch;
    if (fgets(line, (int)size, stdin) == NULL) return 0;
    len = strlen(line);
    if (len && line[len - 1] != '\n')
        while ((ch = getchar()) != '\n' && ch != EOF) { /* Drain long input. */ }
    return 1;
}

static int check_unlock(int index)
{
    Progress progress;
    int open;
    progress_init(&progress);
    progress_complete(&progress, 0, levels[0].par);
    progress_complete(&progress, 1, levels[1].par);
    open = levels[index].unlock(&progress);
    puts("Fixture: both tutorials completed at par (1 and 4 steps).");
    printf("Level %d: %s | %s | %s\n", index + 1, levels[index].name,
           levels[index].ticket, open ? "PASS: UNLOCKED" : "FAIL: STILL LOCKED");
    return open ? 0 : 1;
}

static int next_open_level(const Progress *progress, int current)
{
    int i;
    for (i = current + 1; i < LEVEL_COUNT; ++i)
        if (levels[i].unlock(progress) && !progress->completed[i]) return i;
    return -1;
}

static int play_level(Progress *progress, int index, int no_save)
{
    Game game;
    char line[256];
    int recorded = 0;
    if (!game_load(&game, index)) {
        fputs("Invalid level data.\n", stderr);
        return -1;
    }
    for (;;) {
        size_t i;
        render_game(&game);
        if (game_won(&game)) {
            if (!recorded) {
                progress_complete(progress, index, game.state.steps);
                if (!no_save && !progress_save(progress, save_path))
                    fputs("Could not save progress; this session can continue.\n", stderr);
                recorded = 1;
            }
            puts("\nPATCHES DELIVERED! Level complete.");
            puts("N: next open challenge | M: menu | R: replay | U: undo");
        }
        printf("> ");
        fflush(stdout);
        if (!read_line(line, sizeof(line))) return -1;
        for (i = 0; line[i]; ++i) {
            int ch = tolower((unsigned char)line[i]);
            int dx = 0, dy = 0;
            if (isspace((unsigned char)ch)) continue;
            if (ch == 'q') return -1;
            if (ch == 'm') return 0;
            if (ch == 'n' && game_won(&game)) {
                int next = next_open_level(progress, index);
                return next < 0 ? 0 : next + 1;
            }
            if (ch == 'r') {
                if (!game_load(&game, index)) return -1;
                recorded = 0;
                break;
            }
            if (ch == 'u') {
                if (!game_undo(&game)) puts("No move to undo.");
                recorded = 0;
                continue;
            }
            if (ch == 'h') { help(); continue; }
            if (game_won(&game)) continue;
            switch (ch) {
            case 'w': dy = -1; break;
            case 'a': dx = -1; break;
            case 's': dy = 1; break;
            case 'd': dx = 1; break;
            default: puts("Unknown command. Use W/A/S/D, U, R, M or Q."); continue;
            }
            if (!game_move(&game, dx, dy))
                puts("Blocked: patches cannot move through walls or other patches.");
            /* Let the outer loop record a win before accepting more input. */
            if (game_won(&game)) break;
        }
    }
}

int main(int argc, char **argv)
{
    Progress progress;
    char line[256];
    int i, no_save = 0, list = 0, requested = -1, probe = -1;
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            help(); return 0;
        } else if (strcmp(argv[i], "--no-save") == 0) {
            no_save = 1;
        } else if (strcmp(argv[i], "--list") == 0) {
            list = 1;
        } else if (strcmp(argv[i], "--level") == 0 ||
                   strcmp(argv[i], "--check-unlock") == 0) {
            int is_probe = strcmp(argv[i], "--check-unlock") == 0;
            int parsed;
            if (++i == argc || (parsed = parse_level(argv[i])) < 0) {
                fprintf(stderr, "Expected a level number between 1 and %d.\n", LEVEL_COUNT);
                return 2;
            }
            if (is_probe) probe = parsed;
            else requested = parsed;
        } else {
            fprintf(stderr, "Unknown option: %s (try --help).\n", argv[i]);
            return 2;
        }
    }
    if (probe >= 0) return check_unlock(probe);
    if (no_save) progress_init(&progress);
    else (void)progress_load(&progress, save_path);
    if (list) { render_menu(&progress); return 0; }
    for (;;) {
        int chosen;
        if (requested >= 0) {
            chosen = requested;
            requested = -1;
        } else {
            render_menu(&progress);
            printf("> ");
            fflush(stdout);
            if (!read_line(line, sizeof(line))) break;
            if (tolower((unsigned char)line[0]) == 'q') break;
            if (tolower((unsigned char)line[0]) == 'h') { help(); continue; }
            chosen = parse_level(line);
            if (chosen < 0) { puts("Please choose a level number, H, or Q."); continue; }
        }
        if (!levels[chosen].unlock(&progress)) {
            printf("LOCKED: %s. Repair ticket %s to open this puzzle.\n",
                   levels[chosen].requirement, levels[chosen].ticket);
            continue;
        }
        {
            int result = play_level(&progress, chosen, no_save);
            if (result < 0) break;
            if (result > 0) requested = result - 1;
        }
    }
    puts("\nThanks for playing. Make a small commit, open a new challenge!");
    return 0;
}
