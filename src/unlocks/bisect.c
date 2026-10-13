#include "game.h"

int unlock_bisect(const Progress *progress)
{
    int points = progress_tutorial_count(progress) + 1;
    return points >= 4;
}
