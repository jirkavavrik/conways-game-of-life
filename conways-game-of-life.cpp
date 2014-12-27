#include <iostream>
#include <cstdlib>
#include <time.h>

#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

short count_x;
short count_y;
char f = 35;
char e = 32;

short actual_state[256][256];

void cls() {
#ifdef WIN32
system("cls");
#else
system("clear");
#endif
}

short check_life(short, short);

int main()
{
srand(time(NULL));

    entering_dimensions:
    short field;
    cout << "\tField size: ";
    cin >> field;
    count_y = field;
    count_x = field;
    if( count_x > 256 || count_y > 256) {
    cout << "Number must not be more than 256!\n";
    goto entering_dimensions;
    }

    for(short tmp_y = 0; tmp_y < count_y; tmp_y++) {
        for(short tmp_x = 0; tmp_x < count_x; tmp_x++) {
            short random = rand() % 2;
            if(random == 1) {
                actual_state[tmp_x][tmp_y] = 1;
            } else {
                actual_state[tmp_x][tmp_y] = 0;
            }

        }
    }

    begin:
    cls();
    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            if(actual_state[x_ax][y_ax] == 1) {
                cout << f;
            } else if(actual_state[x_ax][y_ax] == 0){
                cout << e;
            }
        }
    cout << "\n";
    }


    short next_state[256][256];

    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            next_state[x_ax][y_ax] = check_life(x_ax, y_ax);
        }
    }

    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            actual_state[x_ax][y_ax] = next_state[x_ax][y_ax];
        }
    }
    #ifdef WIN32
        Sleep(1000);
    #else
        sleep(1);
    #endif
    goto begin;
}

short check_life(short x, short y) {
short surr;

if(x == 0 && y == 0) {
surr = actual_state[x+1][y] + actual_state[x+1][y+1] + actual_state[x][y+1];
}
else if (x == 0 && y != 0 && x != count_x) {
surr = actual_state[x-1][y] + actual_state[x-1][y+1] + actual_state[x][y+1] + actual_state[x+1][y+1] + actual_state[x][y+1];
}
else if(x == count_x && y == 0) {
surr = actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1];
}
else if(y > 0 && x == 0 && y < count_y) {
surr = actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[x+1][y] + actual_state[x+1][y+1] + actual_state[x][y+1];
}
else if(x > 0 && y > 0 && x != count_x && y != count_y) {
surr = actual_state[x-1][y-1] + actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[y][x+1] + actual_state[y+1][x+1] + actual_state[x][y+1] + actual_state[x-1][y+1] + actual_state[x-1][y];
}
else if(x == count_x && y > 0 && y != count_y) {
surr = actual_state[x][y+1] + actual_state[x-1][y-1] + actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1];
}
else if(x == 0 && y == count_y) {
surr = actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[x+1][y];
}
else if(x > 0 && x != count_x && y == count_y) {
surr = actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[x+1][y];
}
else if(x == count_x && y == count_y) {
surr = actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1];
}

if (actual_state[x][y] == 1 && surr < 2)
    return 0;
else if (actual_state[x][y] == 1 && (surr == 2 || surr == 3) )
    return 1;
else if(actual_state[x][y] == 1 && surr > 3)
    return 0;
else if(actual_state[x][y] == 0 && surr == 3)
    return 1;
}
