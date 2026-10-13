#include "game.h"

int unlock_stash(const Progress *progress)
{
    int required = 3;
    return progress_tutorial_count(progress) >= required;
}
