#include "game.h"

int unlock_origin(const Progress *progress)
{
    return progress_tutorial_count(progress) == TUTORIAL_COUNT &&
           levels[0].par + levels[1].par < 5;
}
