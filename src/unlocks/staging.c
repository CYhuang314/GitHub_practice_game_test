#include "game.h"

int unlock_staging(const Progress *progress)
{
    int required_stamps = TUTORIAL_COUNT + 1;
    return progress_tutorial_count(progress) >= required_stamps;
}
