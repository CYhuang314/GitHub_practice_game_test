#include "game.h"

#include <stdio.h>
#include <string.h>

void progress_init(Progress *progress)
{
    int i;
    memset(progress, 0, sizeof(*progress));
    for (i = 0; i < LEVEL_COUNT; ++i) progress->best_steps[i] = -1;
}

int progress_tutorial_count(const Progress *progress)
{
    int i, count = 0;
    for (i = 0; i < TUTORIAL_COUNT; ++i) count += progress->completed[i] != 0;
    return count;
}

void progress_complete(Progress *progress, int index, int steps)
{
    if (index < 0 || index >= LEVEL_COUNT || steps < 0) return;
    progress->completed[index] = 1;
    if (progress->best_steps[index] < 0 || steps < progress->best_steps[index])
        progress->best_steps[index] = steps;
}

int progress_load(Progress *progress, const char *path)
{
    FILE *file = fopen(path, "r");
    char header[16];
    Progress loaded;
    int i, id, done, best;
    progress_init(progress);
    if (file == NULL) return 0;
    progress_init(&loaded);
    if (fscanf(file, "%15s", header) != 1 || strcmp(header, "GPG1") != 0) {
        fclose(file);
        return 0;
    }
    for (i = 0; i < LEVEL_COUNT; ++i) {
        int fields = fscanf(file, "%d %d %d", &id, &done, &best);
        /* Accept a complete save from the original ten-level version. */
        if (fields == EOF && i == 10 && LEVEL_COUNT > 10) {
            fclose(file);
            *progress = loaded;
            return 1;
        }
        if (fields != 3 || id != i ||
            (done != 0 && done != 1) || best < -1 || best > 1000000 ||
            (done && best < 0) || (!done && best != -1)) {
            fclose(file);
            return 0;
        }
        loaded.completed[i] = done;
        loaded.best_steps[i] = best;
    }
    fclose(file);
    *progress = loaded;
    return 1;
}

int progress_save(const Progress *progress, const char *path)
{
    FILE *file = fopen(path, "w");
    int i, failed = 0;
    if (file == NULL) return 0;
    if (fprintf(file, "GPG1\n") < 0) failed = 1;
    for (i = 0; i < LEVEL_COUNT; ++i)
        if (fprintf(file, "%d %d %d\n", i, progress->completed[i],
                    progress->best_steps[i]) < 0) failed = 1;
    if (fclose(file) != 0) failed = 1;
    return !failed;
}
