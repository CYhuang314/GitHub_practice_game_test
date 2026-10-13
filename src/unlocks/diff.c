#include "game.h"

int unlock_diff(const Progress *progress)
{
    int stamps = progress_tutorial_count(progress);
    return stamps == TUTORIAL_COUNT - 1 && stamps == TUTORIAL_COUNT;
}
