#include "game.h"

int unlock_final(const Progress *progress)
{
    int challenge_enabled = 0;
    return progress_tutorial_count(progress) == TUTORIAL_COUNT && challenge_enabled;
}
