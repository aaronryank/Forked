#include <stdio.h>
#include "forked.h"

int stack[30000];
int size;
int memory;

struct { int x, y, d; } jumps[300];
int jumpnum;

enum { UNDEF, NORTH, EAST, SOUTH, WEST };

struct { int direction, x, y; } ips[500];
int ip_top, ip_count;
#define ip ips[ip_top]

int interp(void)
{
    ip.direction = EAST;

    while (1) { for (ip_top = 0; ip_top <= ip_count; ip_top++) {

        if (parse(buf[ip.y][ip.x]) == -1)
            return 0;

        switch (ip.direction) {
          case NORTH: ip.y--;  break;
          case EAST:  ip.x++;  break;
          case SOUTH: ip.y++;  break;
          case WEST:  ip.x--;  break;
        }

        if (ip.x < 0) {
            if (ip.direction == WEST)
                while (buf[ip.y][++(ip.x)]);
            else
                return 1;
        }
        else if (ip.y < 0) {
            if (ip.direction == NORTH)
                while (buf[(ip.y)++][ip.x]);
            else
                return 1;
        }
        else if (ip.y >= 1000) {
            return 2;
        }
        else if (buf[ip.y][ip.x] <= 0) {
            if (ip.direction == EAST)
                while (buf[ip.y][--(ip.x)]);
            else if (ip.direction == SOUTH)
                while (buf[--(ip.y)][ip.x]);
            else
                return 2;
        }
    }}
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
      case '^': ip.direction = NORTH;  break;
      case 'v': ip.direction = SOUTH;  break;
      case '>': ip.direction = EAST;   break;
      case '<': ip.direction = WEST;   break;

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
        switch (ip.direction) {
          case EAST: ip.direction = SOUTH; break;
          case SOUTH: ip.direction = WEST; break;
          case WEST: ip.direction = NORTH; break;
          case NORTH: ip.direction = EAST; break;
        }
    }
    else if (m == '/') {
        switch (ip.direction) {
          case EAST: ip.direction = NORTH; break;
          case NORTH: ip.direction = EAST; break;
          case WEST: ip.direction = SOUTH; break;
          case SOUTH: ip.direction = WEST; break;
        }
    }
}

void do_fork(void)
{
    int case1, case2, case3;
    case1 = case2 = case3 = 0;

    if (ip.direction == NORTH) {
        if (buf[ip.y+1][ip.x] == '|')
            case1 = 1;
        if (buf[ip.y][ip.x-1] == '-')
            case2 = 1;
        if (buf[ip.y][ip.x+1] == '-')
            case3 = 1;
    }
    else if (ip.direction == SOUTH) {
        if (buf[ip.y-1][ip.x] == '|')
            case1 = 1;
        if (buf[ip.y][ip.x-1] == '-')
            case2 = 1;
        if (buf[ip.y][ip.x+1] == '-')
            case3 = 1;
    }
    else if (ip.direction == WEST) {
        if (buf[ip.y][ip.x+1] == '-')
            case1 = 1;
        if (buf[ip.y-1][ip.x] == '|')
            case2 = 1;
        if (buf[ip.y+1][ip.x] == '|')
            case3 = 1;
    }
    else if (ip.direction == EAST) {
        if (buf[ip.y][ip.x-1] == '-')
            case1 = 1;
        if (buf[ip.y-1][ip.x] == '|')
            case2 = 1;
        if (buf[ip.y+1][ip.x] == '|')
            case3 = 1;
    }

    if (case1 && case2 && case3) {
        if (ip.direction == WEST) {
            if (stack[size-1] > 0)
                ip.direction = NORTH;
            else
                ip.direction = SOUTH;
        }
        else if (ip.direction == NORTH) {
            if (stack[size-1] > 0)
                ip.direction = EAST;
            else
                ip.direction = WEST;
        }
        else if (ip.direction == EAST) {
            if (stack[size-1] > 0)
                ip.direction = SOUTH;
            else
                ip.direction = NORTH;
        }
        else if (ip.direction == SOUTH) {
            if (stack[size-1] > 0)
                ip.direction = WEST;
            else
                ip.direction = EAST;
        }
    }

    //printf("\ndirection = %d, case1 = %d, case2 = %d, case3 = %d\n",direction,case1,case2,case3);
}
