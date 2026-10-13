#include "game.h"

int unlock_log(const Progress *progress)
{
    int tickets = progress_tutorial_count(progress) / 3;
    return tickets >= 1;
}
