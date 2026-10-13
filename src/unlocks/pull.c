#include "game.h"

int unlock_pull(const Progress *progress)
{
    return progress_tutorial_count(progress) >= TUTORIAL_COUNT + 1;
}
