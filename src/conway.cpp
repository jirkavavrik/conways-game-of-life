#include <SDL.h>
#include <iostream> //cout and cin
#include <cstdlib> //system(), srand(), rand()
#include <ctime> //time() for srand()

//size of window
int windowX = 640;
int windowY = 480;
//size of cell matrix
int countX = 45;
int countY = 34;

//arrays storing the cells
char* currentState = (char*) malloc(countX*countY);
char* nextState = (char*) malloc(countX*countY);

int delay = 250;

char checkLife(short, short); 
void cls();
void drawBackground();
bool deadCellsAlertShown = false;

SDL_Renderer* renderer;

const char* infoMessage = "The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.\n\
(Wikipedia contributors. \"Conway's Game of Life.\" Wikipedia, The Free Encyclopedia. Wikipedia, The Free Encyclopedia, 21 Sep. 2018. Web. 23 Sep. 2018.)\n\
	controls:\n\
	F1: show this info\n\
	Arrows up and down: change speed of the game\n\
	Q or Alt+F4 to quit\n\
	resize app window to change the playing area and/or to reset the game\n\
";

int main(int argc, char* argv[])
{
int previousCountX;
int previousCountY;
	
	srand(time(NULL)); //initialize random number generator
	
	//generate random zeros and ones  to currentState
	for(short tmp_y = 0; tmp_y < countY; tmp_y++) {
          for(short tmp_x = 0; tmp_x < countX; tmp_x++) {
               short random = rand() % 2; 
              if(random == 1) {
                    currentState[tmp_x+tmp_y*countX] = 1;
                } else {
                    currentState[tmp_x+tmp_y*countX] = 0;
                }
            }
        }
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* MainWindow = SDL_CreateWindow("Conway's game of life by Jiři Vavřík v3.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, SDL_WINDOW_RESIZABLE); //create window
	renderer = SDL_CreateRenderer(MainWindow,-1,SDL_RENDERER_ACCELERATED);//create renderer
	
	bool End = false;
	bool Pause = false;
	SDL_Event* event=new SDL_Event;//structure for handling events
	SDL_Rect* rect = new SDL_Rect;
	
	drawBackground();
	while (!End)//main loop
		{
		

	while (SDL_PollEvent(event)) {//read events from queue
			
			if (event->type == SDL_QUIT)
				End = true;	

			else if (event->type == SDL_KEYDOWN) //if key is pressed
			{
				switch (event->key.keysym.sym) //choose the key
				{
					case SDLK_UP: //if up arrow is pressed, we decrease delay by 10ms, but protect it from overfowing and getting stuck
						if(delay > 10)
							delay -= 10; std::cout << "Delay changed to " << delay << "ms\n"; break;
					case SDLK_DOWN: //if down arrow is pressed, we increase delay (decrease speed)
						delay += 10; std::cout << "Delay changed to " << delay << "ms\n"; break;
					case SDLK_F1:
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                         "Help",
                         infoMessage,
                         MainWindow); break;
					case SDLK_PAUSE: if(Pause) Pause = false; else Pause = true; break;
					case SDLK_q: End = true; break;
					}

				}
			else if (event->type == SDL_WINDOWEVENT) {
				switch (event->window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						SDL_Log("Window %d resized to %dx%d", event->window.windowID, event->window.data1,	event->window.data2);
						previousCountX = countX;
						previousCountY = countY;
						windowX = event->window.data1;
						windowY = event->window.data2;
						countX = int((windowX+2)/14);
						countY = int((windowY+2)/14);
						std::cout << countX << "  " << countY << std::endl;
						currentState = (char*) realloc(currentState, countX*countY);
						nextState = (char*) realloc(nextState, countX*countY);

						for(int i = 0; i < countX*countY; i++) {
							short random = rand() % 2; 
								if(random == 1) {
									currentState[i] = 1;
								} else {
									currentState[i] = 0;
								}
						}
						drawBackground();
						deadCellsAlertShown = false;
						break;
				}
			}
	}//end of reading event queue		


	//check if there are still cells alive
	int count = 0;
	for(int i = 0; i < countX*countY; i++) {
		if (currentState[i] == 1)
			count++;
	}
	if(count == 0 && !deadCellsAlertShown) {
		deadCellsAlertShown = true;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
									"Cells died out",
									"All cells have died out, resizing the app window resets all cells",
									MainWindow);
		}

	if(!Pause){
		//calculate next state vith checkLife();
        for(short y_ax = 0; y_ax < countY; y_ax++) {
			for(short x_ax = 0; x_ax < countX; x_ax++) {
				nextState[x_ax+y_ax*countX] = checkLife(x_ax, y_ax);
			}
		}
	
		//changing present state to the future we calculated in previous step
		for(short i = 0; i < countY*countX; i++) {
            currentState[i] = nextState[i];
		}
	}

		//render current state
		for(short y_ax = 0; y_ax < countY; y_ax++) {
            for(short x_ax = 0; x_ax < countX; x_ax++) {
				 if(currentState[x_ax+y_ax*countX] == 1) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);//blue color
                } else if(currentState[x_ax+y_ax*countX] == 0){
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);//white
               }
				rect->x = x_ax*14+2;	
				rect->y = y_ax*14+2;
				rect->w = 11;
				rect->h = 11;
				SDL_RenderFillRect(renderer, rect);//add rectangle to renderer
            }
        }
	SDL_RenderPresent(renderer); //update screen
	SDL_Delay(delay);
	}//end of while loop

	SDL_Quit();
	return 0;
}//end of main

void drawBackground() { //clear background
	SDL_SetRenderDrawColor(renderer, 127, 127, 127, SDL_ALPHA_OPAQUE);//background gray color
	SDL_RenderFillRect(renderer, NULL);
}

char checkLife(short x, short y) {
    char surr; //surrounding

     if(x == 0 && y == 0) { //left top corner
        surr = currentState[(x+1)+y*countX] + currentState[(x+1)+(y+1)*countX] + currentState[x+(y+1)*countX];
    } else if (x > 0 && y == 0 && x < countX) { //top edge
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y+1)*countX] + currentState[x+(y+1)*countX] + currentState[(x+1)+(y+1)*countX] + currentState[(x+1)+y*countX];
    } else if(x == countX && y == countY) { //top right corner
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y+1)*countX] + currentState[x+(y+1)*countX];
    } else if(y > 0 && x == 0 && y < countY) { //left edge
        surr = currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x+1)+y*countX] + currentState[(x+1)+(y+1)*countX] + currentState[x+(y+1)*countX];
    } else if(x > 0 && y > 0 && x < countX && y < countY) { //middle
        surr = currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x-1)+(y+1)*countX] + currentState[(x+1)+(y+1)*countX] + currentState[x+(y+1)*countX] + currentState[(x+1)+y*countX] + currentState[(x-1)+y*countX];
    } else if(x == countX && y > 0 && y < countY) { //right edge
        surr = currentState[x+(y+1)*countX] + currentState[(x-1)+(y+1)*countX] + currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX];
    } else if(x == 0 && y == countY) { //lower left corner
        surr = currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x+1)+y*countX];
    } else if(x > 0 && x < countX && y == countY) { //lower edge
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x+1)+y*countX];
    } else if(x == countX && y == countY) { //lower right corner
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX];
    }
	else {
        std::cout << "Error during calculating surrounding\n";
		exit(EXIT_FAILURE);
    }

    //according to wikipedia rules
    if (currentState[x+y*countX] == 1 && surr < 2)
        return char(0);
    else if (currentState[x+y*countX] == 1 && (surr == 2 || surr == 3) )
        return char(1);
    else if(currentState[x+y*countX] == 1 && surr > 3)
        return char(0);
    else if(currentState[x+y*countX] == 0 && surr == 3)
        return char(1);
    else if(currentState[x+y*countX] == 0 && surr != 3)
        return char(0);
	else {
        std::cout << "Error during calculating life " <<x<<" "<<y<< " "<<currentState[x+y*countX]<<" "<<surr<<std::endl;
		exit(EXIT_FAILURE);
    }
}
