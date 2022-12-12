#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAX_STUMP_SZ 7
#define BG_OFFSET    10
#define CLEAR        printf("\x1b[1;1H\x1b[2J")
#define RESET_COLORS printf("\x1b[0m")
#define PRINT_GAP(x) do { \
                        if (x != 0)\
                            printf("\x1b[0m%*c", x, ' ');\
                     } while (0)
#define ARRAY_LEN(a) sizeof(a)/sizeof(a[0])

enum TERM_COLORS {
    RED = 31,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE
};

int colors[] = { RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE };
char ornaments[] = { '*', ' ' };

struct t_pixel {
    int color;
    char p;
};

void handle_sigint(int sig)
{
    RESET_COLORS;
    printf("Happy holidays!\n");
    exit(EXIT_SUCCESS);
}

int calc_stump (int t)
{
    int width = MAX_STUMP_SZ;
    if (t <= (MAX_STUMP_SZ + 2)) {
        width = (t/2) + 1;
    }
    return width;
}

int add_all_odd (int x)
{
    if (x <= 0) return 0;
    return x + (add_all_odd(x-2));
}

void gen_tree (struct t_pixel *t, int sz)
{
    int i;
    for (i = 0; i < sz; i++) {
        t[i].color = colors[rand() % ARRAY_LEN(colors)];
        t[i].p = ornaments[rand() % ARRAY_LEN(ornaments)];
    }
}

void print_tree (struct t_pixel *t, int rows, int col, int srows)
{
    int i, j;
    int spaces = (col - 1)/2;
    int stump_sp = (col - srows)/ 2;
    int pixels = 1;
    int index = 0;
    for (i = 0; i < rows; i++, pixels += 2, spaces--) {
        PRINT_GAP(spaces);
        for (j = 0; j < pixels; j++, index++) {
            printf("\x1b[%d;%dm%c",
                    t[index].color, GREEN+BG_OFFSET, t[index].p);
        }
        printf("\n");
    }
    for (i = 0; i < (srows/2); i++) {
        PRINT_GAP(stump_sp);
        for (j = 0; j < srows; j++) {
            printf("\x1b[1;;%dm%c",
                    YELLOW+BG_OFFSET, ' ');
        }
        printf("\n");
    }
}

int main (int argc, char *argv[])
{
    struct winsize w;
    int col, row, total, ssz;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    signal(SIGINT, handle_sigint);

    /* calculate tree dimensions */
    col = (w.ws_col/2) + 1;
    total = add_all_odd(col);
    row = (int)sqrt(total);
    ssz = calc_stump(col);
    struct t_pixel tree[total];

    while (1) {
        CLEAR;
        gen_tree(tree, total);
        print_tree(tree, row, col, ssz);
        RESET_COLORS;
        sleep(1);
    }

    RESET_COLORS;
    printf("\n");
    return 0;
}
