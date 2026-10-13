#include "game.h"

int unlock_remote(const Progress *progress)
{
    int room_enabled = 0;
    return progress_tutorial_count(progress) == TUTORIAL_COUNT && room_enabled;
}
