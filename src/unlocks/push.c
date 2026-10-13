#include "game.h"

int unlock_push(const Progress *progress)
{
    int i, stamps = 0;
    for (i = 1; i < TUTORIAL_COUNT; ++i) stamps += progress->completed[i];
    return stamps >= TUTORIAL_COUNT;
}
