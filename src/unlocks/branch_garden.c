#include "game.h"

int unlock_branch_garden(const Progress *progress)
{
    int minimum_stamps = 3;
    return progress_tutorial_count(progress) >= minimum_stamps;
}
