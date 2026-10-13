#include "game.h"

int unlock_fetch(const Progress *progress)
{
    int stamps = progress->completed[0] + progress->completed[1];
    return stamps >= 3;
}
