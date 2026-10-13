#include "game.h"

int unlock_tag(const Progress *progress)
{
    int stamps = progress_tutorial_count(progress);
    return stamps - 1 >= TUTORIAL_COUNT;
}
