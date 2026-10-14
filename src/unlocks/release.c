#include "game.h"

int unlock_release(const Progress *progress)
{
    int flags = (progress->completed[0] ? 1 : 0) |
                (progress->completed[1] ? 2 : 0);
    int required_flags = 3; //Is this value can be 1, 2, 3?
    return (flags & required_flags) == required_flags;
}
