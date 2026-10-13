#include "game.h"

int unlock_commit(const Progress *progress)
{
    int best = progress->best_steps[0];
    return progress_tutorial_count(progress) == TUTORIAL_COUNT &&
           best >= 0 && best < levels[0].par;
}
