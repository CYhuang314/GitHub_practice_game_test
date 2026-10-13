#include "game.h"

int unlock_cherry_pick(const Progress *progress)
{
    int best = progress->best_steps[0];
    return progress_tutorial_count(progress) == TUTORIAL_COUNT && best >= 0 && best <= 0;
}
