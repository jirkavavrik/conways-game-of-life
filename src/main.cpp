#include <iostream> //cout and cin
#include <cstdlib> //system(), srand(), rand()
#include <ctime> //time() for srand()

//for sleeping function
#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std; //std::cout and std::cin

short count_x;
short count_y;
char f = 35; //ascii of '#'
char e = 32; //ascii of ' ' (space)

short actual_state[256][256]; //actual cell state

void cls() { //multiplatform clear screen
    #ifdef WIN32
        system("cls");
    #else
        system("clear"); //this is planned to be improved
    #endif
}

short check_life(short, short); //prototype of function for deciding whether cell will stay alive or not

int main()
{
    srand(time(NULL)); //initialize random number generator

    entering_size: //label for returning to entering dialog when number is too large
    short field;
    cout << "\tArray size: ";
    cin >> field; //entering size of array
    count_y = field; //array will be always rectangle, it's easier
    count_x = field;
    if( count_x > 256 || count_y > 256) {
        cout << "Number must not be more than 256!\n";
        goto entering_size; //return to entering
    }

    //random cell generating and savimg to actual state
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

    begin: //label for simulation to begin
    
    //printing out
    cls();
    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            if(actual_state[x_ax][y_ax] == 1) {
                cout << f; //printing '#'
            } else if(actual_state[x_ax][y_ax] == 0){
                cout << e; //printing space
            }
        }
    cout << "\n"; //newline
    }


    short next_state[256][256]; //future state

    //calculating next state for every cell
    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            next_state[x_ax][y_ax] = check_life(x_ax, y_ax);
        }
    }

    //changing actual state
    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            actual_state[x_ax][y_ax] = next_state[x_ax][y_ax];
        }
    }
    
    //sleeping
    #ifdef WIN32
        Sleep(1000);
    #else
        sleep(1);
    #endif
    goto begin; //returning back
}

short check_life(short x, short y) {
    short surr; //surrounding

    if(x == 0 && y == 0) { //left top corner etc.
        surr = actual_state[x+1][y] + actual_state[x+1][y+1] + actual_state[x][y+1];
    } else if (x == 0 && y != 0 && x != count_x) {
        surr = actual_state[x-1][y] + actual_state[x-1][y+1] + actual_state[x][y+1] + actual_state[x+1][y+1] + actual_state[x][y+1];
    } else if(x == count_x && y == 0) {
        surr = actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1];
    } else if(y > 0 && x == 0 && y < count_y) {
        surr = actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[x+1][y] + actual_state[x+1][y+1] + actual_state[x][y+1];
    } else if(x > 0 && y > 0 && x != count_x && y != count_y) {
        surr = actual_state[x-1][y-1] + actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[y][x+1] + actual_state[y+1][x+1] + actual_state[x][y+1] + actual_state[x-1][y+1] + actual_state[x-1][y];
    } else if(x == count_x && y > 0 && y != count_y) {
        surr = actual_state[x][y+1] + actual_state[x-1][y-1] + actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1];
    } else if(x == 0 && y == count_y) {
        surr = actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[x+1][y];
    } else if(x > 0 && x != count_x && y == count_y) {
        surr = actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1] + actual_state[x+1][y-1] + actual_state[x+1][y];
    } else if(x == count_x && y == count_y) {
        surr = actual_state[x-1][y] + actual_state[x-1][y-1] + actual_state[x][y-1];
    }

    //according to wikipedia rules
    if (actual_state[x][y] == 1 && surr < 2)
        return 0;
    else if (actual_state[x][y] == 1 && (surr == 2 || surr == 3) )
        return 1;
    else if(actual_state[x][y] == 1 && surr > 3)
        return 0;
    else if(actual_state[x][y] == 0 && surr == 3)
        return 1;
}