#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "nhss.h"
#include "move.h"

extern nhss_info_t info;
extern int boulders;

int moveKey(char dir) {
  switch (dir) {
    case 'k':
      return moveto(0, 1);
    case 'j':
      return moveto(0, -1);

    case 'l':
      return moveto(1, 0);
    case 'h':
      return moveto(-1, 0);

// diagonals are disabled here, as in sokoban they're not much use.
// You can't move diagonally around a boulder.
// We'd need to add rules in moveto to disallow moves between diag adjacent boulders and walls to enable this and have it behave correctly.
    case 'b':
      return moveto(-1, -1);
    case 'y':
      return moveto(-1, 1);

    case 'n':
      return moveto(1, -1);
    case 'u':
      return moveto(1, 1);

    // allow null moves (perhaps to mark a place in the recording)
    case '.':
    case 's':
      return E_SUCCESS;

    default:
      return E_ERROR;
      break;
  }
}

// 1 if we are attempting a diagonal move, zero else
int isdiag(int x, int y) {
  return (abs(x) + abs(y)) - 1;
}

// Moves the character to the position specified by the coordinates, relative to the player.
// Returns E_SUCCESS if moved (or "."), E_ERROR if no move.
int moveto(int x, int y) {
  int status = E_SUCCESS;
  switch (RELPOS(x, y)) {
    case '<':
      status = E_WIN;
    case '.': // an empty space
      switch (isdiag(x, y)) {
        case 1:
          if (!(RELPOS(x,0) == '.' || RELPOS(0,y) == '.')) {
            status = E_ERROR;
            break; // if we find no empty space, fall through to no move.
          }
        default:
          POS = '.';	// Get rid of player at the old location
          RELPOS(x, y) = '@'; // Move player to new location
          info.player[0] = info.player[0] - y;  // update player location in info
          info.player[1] = info.player[1] + x;
        }
      break;
    case '`': // a boulder
    case '0':
      if (isdiag(x,y)) {
        status = E_ERROR;
        break;
      }
      POS = '.';	// get rid of player
      info.player[0] = info.player[0] - y; // update player location so we can move the boulder
      info.player[1] = info.player[1] + x;
      switch (RELPOS(x, y)) {	// What are we trying to push the boulder into?
        case '.':
          POS = '@';	// get rid of boulder, add player
          RELPOS(x, y) = boulders ? '0': '`';
        break;
        case '^': // filling in a pit, woohoo!
          POS = '@';
          RELPOS(x, y) = '.';
          break;
        default:
          info.player[0] = info.player[0] + y; // Just kidding, not moving.  Reset player info.
          info.player[1] = info.player[1] - x;
          POS = '@';
          status = E_ERROR;
        break;
      }
    break;
    default: // something else, like a wall
      status = E_ERROR;
  }
  return status;
}
