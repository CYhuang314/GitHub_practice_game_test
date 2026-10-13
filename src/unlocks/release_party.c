#include "game.h"

int unlock_release_party(const Progress *progress)
{
    int required_certificates = 3;
    int certificates = progress->completed[0] + progress->completed[1];
    return certificates >= required_certificates;
}
