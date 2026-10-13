#include "game.h"

int unlock_reset(const Progress *progress)
{
    return progress_tutorial_count(progress) == TUTORIAL_COUNT && !progress->completed[1];
}
