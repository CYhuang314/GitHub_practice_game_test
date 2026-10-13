#include "game.h"

int unlock_upstream(const Progress *progress)
{
    return progress_tutorial_count(progress) == TUTORIAL_COUNT &&
           progress->completed[0] * progress->completed[1] == 0;
}
