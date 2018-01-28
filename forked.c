#include <stdio.h>
#include "forked.h"

int stack[30000];
int size;
int memory;

struct { int x, y, d; } jumps[300];
int jumpnum;

enum { UNDEF, NORTH, EAST, SOUTH, WEST };

int direction, x, y;

int interp(void)
{
    direction = EAST;

    while (1) {

        if (!buf[y][0])
            return 0;

        if (parse(buf[y][x]) == -1)
            return 0;

        switch (direction) {
          case NORTH: y--;  break;
          case EAST:  x++;  break;
          case SOUTH: y++;  break;
          case WEST:  x--;  break;
        }

        if (x < 0) {
            if (direction == WEST)
                while (buf[y][++x]);
            else
                return 1;
        }
        else if (y < 0) {
            if (direction == NORTH)
                while (y++ < bufsize);
            else
                return 1;
        }
        else if (y >= 1000) {
            return 2;
        }
        else if (buf[y][x] <= 0) {
            if (direction == EAST)
                while (buf[y][--x]);
            else if (direction == SOUTH)
                while (buf[--y][x]);
            else
                return 2;
        }
    }
}

void mirror(char);
void do_fork(void);

int parse(char command)
{
    if (display_code)
        putchar(command);

    switch (command) {
      case '&': return -1;

      /* directionals */
      case '^': direction = NORTH;  break;
      case 'v': direction = SOUTH;  break;
      case '>': direction = EAST;   break;
      case '<': direction = WEST;   break;

      /* mirrors */
      case '\\': case '/': mirror(command); break;

      /* input */
      case '$': scanf("%d",&stack[size++]); break;
      case '~': stack[size++] = getchar();  break;

      /* output */
      case '%': printf("%d",size ? stack[size-1] : 0);  break;
      case '@': size && putchar(stack[size-1]);         break;
      case '?': printf("%d",size ? stack[--size] : 0);  break;
      case '!': size && putchar(stack[--size]);         break;

      /* stack */
      case 'i': size > 0 && (stack[size-1]++);                                        break;
      case 'd': size > 0 && (stack[size-1]--);                                        break;
      case '+': size > 1 && (stack[size-2] += stack[size-1]); size > 1 && (size--);   break;
      case '\'': size > 1 && (stack[size-2] -= stack[size-1]); size > 1 && (size--);  break;
      case '*': size > 1 && (stack[size-2] *= stack[size-1]); size > 1 && (size--);   break;
      case '_': size > 1 && (stack[size-2] /= stack[size-1]); size > 1 && (size--);   break;
      case 'm': size > 1 && (stack[size-2] %= stack[size-1]); size > 1 && (size--);   break;

      case '=': size > 1 && (stack[size-2] = (stack[size-2] == stack[size-1]));
                size > 1 && (size--);
                break;
      case 'l': size > 1 && (stack[size-2] < (stack[size-2] == stack[size-1]));
                size > 1 && (size--);
                break;
      case 'g': size > 1 && (stack[size-2] > (stack[size-2] == stack[size-1]));
                size > 1 && (size--);
                break;

      case 'p': size && (size--);                                       break;
      case '"': size && (stack[size] = stack[size-1]); size && size++;  break;

      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      stack[size++] = command - '0';
      break;

      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      stack[size++] = command - 'A' + 10;
      break;

      /* fork */
      case ':': do_fork(); break;
    }

    return 0;
}

void mirror(char m)
{
    if (m == '\\') {
        switch (direction) {
          case EAST: direction = SOUTH; break;
          case SOUTH: direction = WEST; break;
          case WEST: direction = NORTH; break;
          case NORTH: direction = EAST; break;
        }
    }
    else if (m == '/') {
        switch (direction) {
          case EAST: direction = NORTH; break;
          case NORTH: direction = EAST; break;
          case WEST: direction = SOUTH; break;
          case SOUTH: direction = WEST; break;
        }
    }
}

void do_fork(void)
{
    int case1, case2, case3;
    case1 = case2 = case3 = 0;

    if (direction == NORTH) {
        if (buf[y+1][x] == '|')
            case1 = 1;
        if (buf[y][x-1] == '-')
            case2 = 1;
        if (buf[y][x+1] == '-')
            case3 = 1;
    }
    else if (direction == SOUTH) {
        if (buf[y-1][x] == '|')
            case1 = 1;
        if (buf[y][x-1] == '-')
            case2 = 1;
        if (buf[y][x+1] == '-')
            case3 = 1;
    }
    else if (direction == WEST) {
        if (buf[y][x+1] == '-')
            case1 = 1;
        if (buf[y-1][x] == '|')
            case2 = 1;
        if (buf[y+1][x] == '|')
            case3 = 1;
    }
    else if (direction == EAST) {
        if (buf[y][x-1] == '-')
            case1 = 1;
        if (buf[y-1][x] == '|')
            case2 = 1;
        if (buf[y+1][x] == '|')
            case3 = 1;
    }

    if (case1 && case2 && case3) {
        if (direction == WEST) {
            if (stack[size-1] >= 0)
                direction = NORTH;
            else
                direction = SOUTH;
        }
        else if (direction == NORTH) {
            if (stack[size-1] >= 0)
                direction = EAST;
            else
                direction = WEST;
        }
        else if (direction == EAST) {
            if (stack[size-1] >= 0)
                direction = SOUTH;
            else
                direction = NORTH;
        }
        else if (direction == SOUTH) {
            if (stack[size-1] >= 0)
                direction = WEST;
            else
                direction = EAST;
        }
    }

    //printf("\ndirection = %d, case1 = %d, case2 = %d, case3 = %d\n",direction,case1,case2,case3);
}
