#include "game.h"

static int always_open(const Progress *progress)
{
    (void)progress;
    return 1;
}

static const char *const tutorial_one[] = {
    "#####", "#...#", "#@$o#", "#...#", "#####"
};
static const char *const tutorial_two[] = {
    "#######", "#@....#", "#.....#", "#..$o.#", "#######"
};
static const char *const warehouse[] = {
    "#########", "#.......#", "#..$..o.#", "#..$..o.#", "#@......#", "#########"
};
static const char *const fork_room[] = {
    "#########", "#...#...#", "#.$.#..o#", "#.......#", "#.$.#..o#", "#@..#...#", "#########"
};
static const char *const staging[] = {
    "#########", "#..o.o..#", "#..$.$..#", "#.......#", "#...@...#", "#########"
};
static const char *const commit_room[] = {
    "#########", "#.......#", "#..o....#", "#..#.$..#", "#.......#", "#.@.....#", "#########"
};
static const char *const merge_room[] = {
    "#########", "#...o...#", "#...$...#", "#.o$....#", "#.......#", "#..@....#", "#########"
};
static const char *const branch_room[] = {
    "##########", "#........#", "#..o.#...#", "#..$...$.#", "#....#o..#", "#.@......#", "##########"
};
static const char *const release_room[] = {
    "#########", "#.o.o.o.#", "#.$.$.$.#", "#.......#", "#..#....#", "#@......#", "#########"
};
static const char *const final_room[] = {
    "##########", "#..o..o..#", "#..$..$..#", "#........#", "#..#.#...#", "#...$....#", "#@..o....#", "##########"
};

static const char *const cherry_pick_map[] = {
    "#######",
    "#.....#",
    "#.....#",
    "#...$@#",
    "#.o.*.#",
    "#######"
};
static const char *const stash_map[] = {
    "########",
    "#o.*...#",
    "#....#.#",
    "#...$@.#",
    "#......#",
    "#......#",
    "########"
};
static const char *const revert_map[] = {
    "#########",
    "#.o@$...#",
    "#..#....#",
    "#...#.*.#",
    "#.......#",
    "#########"
};
static const char *const fetch_map[] = {
    "#######",
    "#.....#",
    "#.$.#.#",
    "#@.#..#",
    "#.o.#.#",
    "#.*...#",
    "#######"
};
static const char *const pull_map[] = {
    "########",
    "#....@.#",
    "#..$oo.#",
    "#.$....#",
    "#......#",
    "########"
};
static const char *const push_map[] = {
    "#########",
    "#.......#",
    "#....@..#",
    "#...o$..#",
    "#...$o#.#",
    "#.......#",
    "#########"
};
static const char *const diff_map[] = {
    "#######",
    "#.@.o.#",
    "#.o$$.#",
    "#.##..#",
    "#.....#",
    "#######"
};
static const char *const log_map[] = {
    "########",
    "#.$o...#",
    "#..#.#.#",
    "#.#....#",
    "#......#",
    "#@$o...#",
    "########"
};
static const char *const status_map[] = {
    "#########",
    "#.......#",
    "#..@....#",
    "#.*.$...#",
    "#....o..#",
    "#########"
};
static const char *const tag_map[] = {
    "#######",
    "#*o...#",
    "#.....#",
    "#.....#",
    "#.#$..#",
    "#...@.#",
    "#######"
};
static const char *const origin_map[] = {
    "########",
    "#....o.#",
    "#..#..*#",
    "#...#$.#",
    "#.@....#",
    "########"
};
static const char *const upstream_map[] = {
    "#########",
    "#.......#",
    "#.o#...@#",
    "#.$..#..#",
    "#.....#.#",
    "#.....*.#",
    "#########"
};
static const char *const remote_map[] = {
    "#######",
    "#.....#",
    "#.....#",
    "#.$.$.#",
    "#..oo@#",
    "#######"
};
static const char *const branch_garden_map[] = {
    "########",
    "#.o....#",
    "#@$$...#",
    "#..#...#",
    "#......#",
    "#...o..#",
    "########"
};
static const char *const squash_map[] = {
    "#########",
    "#.......#",
    "#..#...*#",
    "#@.#..$o#",
    "#.......#",
    "#########"
};
static const char *const fast_forward_map[] = {
    "#######",
    "#.....#",
    "#.$#.@#",
    "#..#.$#",
    "#.o#..#",
    "#....o#",
    "#######"
};
static const char *const checkout_map[] = {
    "########",
    "#.@....#",
    "#......#",
    "#.$.o..#",
    "#...$.o#",
    "########"
};
static const char *const bisect_map[] = {
    "#########",
    "#.*@....#",
    "#...#...#",
    "#.......#",
    "#.....$.#",
    "#.....o.#",
    "#########"
};
static const char *const reset_map[] = {
    "#######",
    "#..@..#",
    "#..o..#",
    "#.#$#.#",
    "#.o.$.#",
    "#######"
};
static const char *const release_party_map[] = {
    "########",
    "#.$..o.#",
    "#.#.o#.#",
    "#.#....#",
    "#.@.$..#",
    "#......#",
    "########"
};

/* Names are Git-themed; the game itself is Sokoban, not a Git simulator. */
const Level levels[LEVEL_COUNT] = {
    {"First Patch", "Push the patch right onto its target.",
     tutorial_one, 5, 1, "", "Always available", always_open},
    {"First Commit", "Reach the left side of the patch before pushing it.",
     tutorial_two, 5, 4, "", "Always available", always_open},
    {"Patch Warehouse", "Two patches: keep both pushing lanes open.",
     warehouse, 6, 12, "GPG-001", "Complete both tutorials", unlock_warehouse},
    {"Fork in the Road", "Route patches through the middle corridor.",
     fork_room, 7, 30, "GPG-002", "Complete both tutorials", unlock_fork},
    {"Staging Area", "Stand below each patch to stage it.",
     staging, 6, 7, "GPG-003", "Collect both tutorial stamps", unlock_staging},
    {"Perfect Commit", "The wall below the target changes your route.",
     commit_room, 7, 9, "GPG-004", "Both tutorials + tutorial 1 in 1 step", unlock_commit},
    {"Merge Yard", "Deliver the upper patch before the side patch.",
     merge_room, 7, 6, "GPG-005", "Both tutorials + tutorial 2 in 4 steps", unlock_merge},
    {"Branch Detour", "The second patch needs two different pushing directions.",
     branch_room, 7, 12, "GPG-006", "Collect both tutorial stamps", unlock_branch},
    {"Release Vault", "Three patches, three targets. Plan the order.",
     release_room, 7, 12, "GPG-007", "Both tutorial completion flags", unlock_release},
    {"Final Pull Request", "Use the gap between walls to deliver the bottom patch.",
     final_room, 8, 15, "GPG-008", "Complete both tutorials", unlock_final},
    {"Cherry Pick", "Keep the pushing side of both patches reachable.",
     cherry_pick_map, 6, 5, "GPG-009", "Complete both tutorials", unlock_cherry_pick},
    {"Stash Corner", "Keep the pushing side of both patches reachable.",
     stash_map, 7, 7, "GPG-010", "Complete both tutorials", unlock_stash},
    {"Revert Route", "Keep the pushing side of both patches reachable.",
     revert_map, 6, 8, "GPG-011", "Complete both tutorials", unlock_revert},
    {"Fetch Depot", "Keep the pushing side of both patches reachable.",
     fetch_map, 7, 5, "GPG-012", "Complete both tutorials", unlock_fetch},
    {"Pull Station", "Keep the pushing side of both patches reachable.",
     pull_map, 6, 14, "GPG-013", "Complete both tutorials", unlock_pull},
    {"Push Platform", "Keep the pushing side of both patches reachable.",
     push_map, 7, 7, "GPG-014", "Complete both tutorials", unlock_push},
    {"Diff Alley", "Keep the pushing side of both patches reachable.",
     diff_map, 6, 8, "GPG-015", "Complete both tutorials", unlock_diff},
    {"Log Lane", "Keep the pushing side of both patches reachable.",
     log_map, 7, 7, "GPG-016", "Complete both tutorials", unlock_log},
    {"Status Square", "Keep the pushing side of both patches reachable.",
     status_map, 6, 5, "GPG-017", "Complete both tutorials", unlock_status},
    {"Tag Terrace", "Keep the pushing side of both patches reachable.",
     tag_map, 7, 7, "GPG-018", "Complete both tutorials", unlock_tag},
    {"Origin Outpost", "Keep the pushing side of both patches reachable.",
     origin_map, 6, 5, "GPG-019", "Complete both tutorials", unlock_origin},
    {"Upstream Yard", "Keep the pushing side of both patches reachable.",
     upstream_map, 7, 8, "GPG-020", "Complete both tutorials", unlock_upstream},
    {"Remote Room", "Keep the pushing side of both patches reachable.",
     remote_map, 6, 11, "GPG-021", "Complete both tutorials", unlock_remote},
    {"Branch Garden", "Keep the pushing side of both patches reachable.",
     branch_garden_map, 7, 9, "GPG-022", "Complete both tutorials", unlock_branch_garden},
    {"Squash Shed", "Keep the pushing side of both patches reachable.",
     squash_map, 6, 7, "GPG-023", "Complete both tutorials", unlock_squash},
    {"Fast Forward", "Keep the pushing side of both patches reachable.",
     fast_forward_map, 7, 10, "GPG-024", "Complete both tutorials", unlock_fast_forward},
    {"Checkout Crossing", "Keep the pushing side of both patches reachable.",
     checkout_map, 6, 8, "GPG-025", "Complete both tutorials", unlock_checkout},
    {"Bisect Bridge", "Keep the pushing side of both patches reachable.",
     bisect_map, 7, 6, "GPG-026", "Complete both tutorials", unlock_bisect},
    {"Reset Retreat", "Keep the pushing side of both patches reachable.",
     reset_map, 6, 8, "GPG-027", "Complete both tutorials", unlock_reset},
    {"Release Party", "Keep the pushing side of both patches reachable.",
     release_party_map, 7, 15, "GPG-028", "Complete both tutorials", unlock_release_party}
};
