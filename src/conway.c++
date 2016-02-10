#include <SDL.h>
#include <iostream> //cout and cin
#include <cstdlib> //system(), srand(), rand()
#include <ctime> //time() for srand()

short nextState[45][34]; //future state of cells
short actualState[45][34]; //present state of cells

//size of cell matrix
short count_x = 45;
short count_y = 34;
int delay = 250;

short checkLife(short, short); 
void cls();

SDL_Renderer* renderer;
SDL_Texture* Background_Tx; //texture for background image
SDL_Surface* Loading_Surf; //surface to load background image into

int main(int,char**)
{
	srand(time(NULL)); //initialize random number generator
	
	//generate random zeros and ones  to actualState
	for(short tmp_y = 0; tmp_y < count_y; tmp_y++) {
          for(short tmp_x = 0; tmp_x < count_x; tmp_x++) {
               short random = rand() % 2; 
              if(random == 1) {
                    actualState[tmp_x][tmp_y] = 1;
                } else {
                    actualState[tmp_x][tmp_y] = 0;
                }
            }
        }
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* MainWindow = SDL_CreateWindow("Conway's game of life by Jiři Vavřík v3.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,630, 500, 0); //create window
	renderer = SDL_CreateRenderer(MainWindow,-1,SDL_RENDERER_ACCELERATED);//create renderer

	//create rectangles for each cell
	SDL_Rect* rect[45][34];
		for(int i = 0; i < 45; i++)
			for(int j = 0; j < 34; j++)
				rect[i][j] = new SDL_Rect;
	
	//load backgroung, put it into texture and free surface
	Loading_Surf = SDL_LoadBMP("wall.bmp");
	Background_Tx = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
	SDL_FreeSurface(Loading_Surf); 
	
	//copy background texture to renderer
	SDL_RenderCopy(renderer, Background_Tx, NULL, NULL);
	
	bool End = false;
	SDL_Event* event=new SDL_Event;//structure for handling events
	while (!End)//main loop
		{
		cls();
		
		//set x and y coordinates for each rectangle (do not touch!)
		for(short y_ax = 0; y_ax < count_y; y_ax++) {
            for(short x_ax = 0; x_ax < count_x; x_ax++) {
               rect[x_ax][y_ax]->x = x_ax*14+1;	
			   rect[x_ax][y_ax]->y = y_ax*14+2;
			   rect[x_ax][y_ax]->w = 11;
			   rect[x_ax][y_ax]->h = 11;
            }
        }
		
		//render actual state
		for(short y_ax = 0; y_ax < count_y; y_ax++) {
            for(short x_ax = 0; x_ax < count_x; x_ax++) {
				 if(actualState[x_ax][y_ax] == 1) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);//blue color
                } else if(actualState[x_ax][y_ax] == 0){
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);//white
               }
				SDL_RenderFillRect(renderer, rect[x_ax][y_ax]);//add rectangle to renderer
            }
        }
		
		//calculate next state via checkLife();
        for(short y_ax = 0; y_ax < count_y; y_ax++) {
			for(short x_ax = 0; x_ax < count_x; x_ax++) {
				nextState[x_ax][y_ax] = checkLife(x_ax, y_ax);
        }
    }
		
	//changing present state to the future we calculated in previous step
    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            actualState[x_ax][y_ax] = nextState[x_ax][y_ax];
        }
    }
		
	while (SDL_PollEvent(event)) {//read events from queue
			
			if (event->type == SDL_QUIT)
				End = true;	

			else if (event->type == SDL_KEYDOWN) //if key is pressed
			{
				switch (event->key.keysym.sym) //choose the key
				{
					case SDLK_UP: //if up arrow is pressed, we decrease delay by 10ms, but protect it from overfowing and getting stuck
						if(delay > 10)
							delay -= 10; break;
					case SDLK_DOWN: //if down arrow is pressed, we increase delay (decrease speed)
						delay += 10; break;
					}
					std::cout << "Delay changed to " << delay << "ms\n";//debug info
				}
			}
		
	SDL_RenderPresent(renderer); //update screen
	SDL_Delay(delay);
	}

	SDL_Quit();
	return 0;
}

void cls() { //multiplatform clear screen
		SDL_RenderCopy(renderer, Background_Tx, NULL, NULL);
		SDL_RenderPresent(renderer);
}

short checkLife(short x, short y) { //This is big sh*t, I know...
    short surr; //surrounding

     if(x == 0 && y == 0) { //left top corner
        surr = actualState[x+1][y] + actualState[x+1][y+1] + actualState[x][y+1];
    } else if (x > 0 && y == 0 && x != count_x) { //top edge
        surr = actualState[x-1][y] + actualState[x-1][y+1] + actualState[x][y+1] + actualState[x+1][y+1] + actualState[x+1][y];
    } else if(x == count_x && y == 0) { //top right corner
        surr = actualState[x-1][y] + actualState[x-1][y+1] + actualState[x][y+1];
    } else if(y > 0 && x == 0 && y < count_y) { //left edge
        surr = actualState[x][y-1] + actualState[x+1][y-1] + actualState[x+1][y] + actualState[x+1][y+1] + actualState[x][y+1];
    } else if(x > 0 && y > 0 && x != count_x && y != count_y) { //middle
        surr = actualState[x-1][y-1] + actualState[x][y-1] + actualState[x+1][y-1] + actualState[x-1][y+1] + actualState[x+1][y+1] + actualState[x][y+1] + actualState[x+1][y] + actualState[x-1][y];
    } else if(x == count_x && y > 0 && y != count_y) { //right edge
        surr = actualState[x][y+1] + actualState[x-1][y+1] + actualState[x-1][y] + actualState[x-1][y-1] + actualState[x][y-1];
    } else if(x == 0 && y == count_y) { //lower left edge
        surr = actualState[x][y-1] + actualState[x+1][y-1] + actualState[x+1][y];
    } else if(x > 0 && x != count_x && y == count_y) { //lower edge
        surr = actualState[x-1][y] + actualState[x-1][y-1] + actualState[x][y-1] + actualState[x+1][y-1] + actualState[x+1][y];
    } else if(x == count_x && y == count_y) { //lower right corner
        surr = actualState[x-1][y] + actualState[x-1][y-1] + actualState[x][y-1];
    }
	else {
        std::cout << "Error during calculating surrounding\n";
		exit(EXIT_FAILURE);
    }

    //according to wikipedia rules
    if (actualState[x][y] == 1 && surr < 2)
        return int(0);
    else if (actualState[x][y] == 1 && (surr == 2 || surr == 3) )
        return int(1);
    else if(actualState[x][y] == 1 && surr > 3)
        return int(0);
    else if(actualState[x][y] == 0 && surr == 3)
        return int(1);
    else if(actualState[x][y] == 0 && surr != 3)
        return int(0);
	else {
        std::cout << "Error during calculating life\n";
		exit(EXIT_FAILURE);
    }
}