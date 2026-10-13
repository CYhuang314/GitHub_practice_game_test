#include "game.h"

int unlock_squash(const Progress *progress)
{
    int best = progress->best_steps[1];
    return progress_tutorial_count(progress) == TUTORIAL_COUNT && best >= 0 && best <= 3;
}
