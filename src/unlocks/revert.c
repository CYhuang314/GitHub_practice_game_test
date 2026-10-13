#include "game.h"

int unlock_revert(const Progress *progress)
{
    return progress_tutorial_count(progress) > TUTORIAL_COUNT;
}
