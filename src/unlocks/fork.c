#include "game.h"

int unlock_fork(const Progress *progress)
{
    int i, stamps = 0;
    for (i = 0; i < TUTORIAL_COUNT - 1; ++i) {
        stamps += progress->completed[i];
    }
    return stamps >= TUTORIAL_COUNT;
}
