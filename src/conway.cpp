#include <SDL.h>
#include <iostream> //cout and cin
#include <cstdlib> //system(), srand(), rand()
#include <ctime> //time() for srand()

//size of window in pixels
int windowXSize = 640;
int windowYSize = 480;
//size of cell matrix
int countX = 45;
int countY = 34;
//arrays storing the cells
char* currentState = (char*) malloc(countX*countY);
char* nextState = (char*) malloc(countX*countY);

int delay = 250;

char checkLife(short, short); 
void drawBackground();
bool deadCellsAlertShown = false;

SDL_Renderer* renderer;
SDL_Color color;
enum colors {colorRed, colorGreen, colorBlue, colorOrange, colorTurqoise, colorMagenta, colorBlack};
char currentColorCode = 2;

const char* infoMessage = "The Game of Life, also known simply as Life, is a cellular automaton \n\
devised by the British mathematician John Horton Conway in 1970.\n\
(Wikipedia contributors. \"Conway's Game of Life.\" Wikipedia, The Free Encyclopedia.\n\
Wikipedia, The Free Encyclopedia, 21 Sep. 2018. Web. 23 Sep. 2018.)\n\
\tcontrols:\n\
\tF1: show this info\n\
\tArrows up and down: change speed of the game\n\
\tC to change color of live cells\n\
\tQ or Alt+F4 to quit\n\
\tresize app window to change the playing area and/or to reset the game\n\
";

int main(int argc, char* argv[]) {
	int previousCountX;
	int previousCountY;
	srand(time(NULL)); //initialize random number generator
	color = {0,0,255,SDL_ALPHA_OPAQUE}; //default color blue
	
	//generate random zeros and ones  to currentState
	for(short tmp_y = 0; tmp_y < countY-1; tmp_y++) {
		for(short tmp_x = 0; tmp_x < countX-1; tmp_x++) {
			short random = rand() % 2; 
				if(random == 1) {
					currentState[tmp_x+tmp_y*countX] = 1;
                } else {
                    currentState[tmp_x+tmp_y*countX] = 0;
                }
		}
    }
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* mainWindow = SDL_CreateWindow("Conway's game of life by Jiři Vavřík v3.2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowXSize, windowYSize, SDL_WINDOW_RESIZABLE); //create window
	// Check that the window was successfully created
    if (mainWindow == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
	renderer = SDL_CreateRenderer(mainWindow,-1,SDL_RENDERER_ACCELERATED);//create renderer
	// Check that the renderer was successfully created
	if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }
	
	bool End = false;
	bool Pause = false;
	SDL_Event* event = new SDL_Event;//structure for handling events
	SDL_Rect* rect = new SDL_Rect;
	
	drawBackground();
	while (!End) {//main loop
		while (SDL_PollEvent(event)) {//read events from queue
			if (event->type == SDL_QUIT)
				End = true;	
			else if (event->type == SDL_KEYDOWN) { //if key is pressed
				switch (event->key.keysym.sym){ //choose the key
					case SDLK_UP: //if up arrow is pressed, we decrease delay by 10ms, but protect it from overfowing and getting stuck
						if(delay > 10){
							delay -= delay/10;
							SDL_Log("Delay changed to %i ms\n", delay);
						}
						break;
					case SDLK_DOWN: //if down arrow is pressed, we increase delay (decrease speed)
						delay += delay/10;
						SDL_Log("Delay changed to %i ms\n", delay); 
						break;
					case SDLK_F1:		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", infoMessage, mainWindow); break;
					case SDLK_PAUSE: case SDLK_SPACE:
						if(Pause){
							Pause = false;
							SDL_Log("resuming game\n");
						} else { 
							Pause = true;
							SDL_Log("game paused\n");
						}
						break;
					case SDLK_q:		End = true; break;
					case SDLK_c:
						if(currentColorCode < 6)
							currentColorCode++;
						else
							currentColorCode = 0;
						switch(currentColorCode){
							case colorRed: 		color = {255, 0, 0, SDL_ALPHA_OPAQUE}; break;
							case colorGreen:	color = {0, 255, 0, SDL_ALPHA_OPAQUE}; break;
							case colorBlue:		color = {0, 0, 255, SDL_ALPHA_OPAQUE}; break;
							case colorOrange:	color = {255, 165, 0, SDL_ALPHA_OPAQUE}; break;
							case colorTurqoise:	color = {50, 198, 166, SDL_ALPHA_OPAQUE}; break;
							case colorMagenta:	color = {255, 0, 255, SDL_ALPHA_OPAQUE}; break;
							case colorBlack:	color = {0, 0, 0, SDL_ALPHA_OPAQUE}; break;
						}
						SDL_Log("color changed\n");
						break;//add color changing code here
					}
			} else if (event->type == SDL_WINDOWEVENT) {
				switch (event->window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						SDL_Log("Window %d resized to %dx%d", event->window.windowID, event->window.data1,	event->window.data2);
						previousCountX = countX;
						previousCountY = countY;
						windowXSize = event->window.data1;
						windowYSize = event->window.data2;
						countX = int((windowXSize+2)/14);
						countY = int((windowYSize+2)/14);
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
			} else if (event->type == SDL_MOUSEBUTTONDOWN) {//routinne for toggling individual cell state
				int x, y, cellX, cellY = 0;
				SDL_GetMouseState(&x, &y);
				cellX = (x-2)/14; //cell coordinates: x_pixels = x*14+2, the same with y
				cellY = (y-2)/14;
				if (event->button.button == SDL_BUTTON_LEFT) {
    					SDL_Log("Mouse Button 1 (left) is pressed, mouse x: %i, mouse y: %i\n", x, y);
					if(((x-2) % 14 <= 10 )&&( (y-2) % 14 <= 10) && cellY < countY && cellX < countX) {//check if coordinates are on a cell
						SDL_Log("cell clicked: cell x: %i, cell y: %i\n", cellX, cellY);
						//toggle cell state
						if(currentState[cellX + cellY * countX] == 1) {
                    		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);//white color
							currentState[cellX + cellY * countX] = 0;
                		} else if(currentState[cellX + cellY * countX] == 0){
							SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
							currentState[cellX + cellY * countX] = 1;
               			}
						rect->x = cellX*14+2;	
						rect->y = cellY*14+2;
						rect->w = 11;
						rect->h = 11;
						SDL_RenderFillRect(renderer, rect);//add rectangle to renderer
						SDL_RenderPresent(renderer); //update screen
						while(true){//wait until mouse button is released
							SDL_WaitEvent(event);
							if(event->type == SDL_MOUSEBUTTONUP)
								break;
						}
					}
				}
			}//end of cell toggling routinne
		}//end of reading event queue
		//check if there are still cells alive
		int count = 0;
		for(int i = 0; i < countX*countY; i++) {
			if (currentState[i] == 1)
				count++;
		}
		if(count == 0 && !deadCellsAlertShown) {
			deadCellsAlertShown = true;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Cells died out", "All cells have died out, resizing the app window resets all cells", mainWindow);
		}
		if(!Pause) {
			//calculate next state vith checkLife();
			for(short y_ax = 0; y_ax <= countY-1; y_ax++) {
				for(short x_ax = 0; x_ax <= countX-1; x_ax++) {
					nextState[x_ax+y_ax*countX] = checkLife(x_ax, y_ax);
				}
			}
			//changing present state to the future we calculated in previous step
			for(short i = 0; i < countY*countX; i++) {
				currentState[i] = nextState[i];
			}
		}
		//render current state
		for(short y_ax = 0; y_ax <= countY-1; y_ax++) {
			for(short x_ax = 0; x_ax <= countX-1; x_ax++) {
				if(currentState[x_ax+y_ax*countX] == 1) {
					SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);//set color
				} else if(currentState[x_ax+y_ax*countX] == 0){
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);//white on empty cells
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
    } else if (x > 0 && y == 0 && x < countX-1) { //top edge
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y+1)*countX] + currentState[x+(y+1)*countX] + currentState[(x+1)+(y+1)*countX] + currentState[(x+1)+y*countX];
	} else if(x == countX-1 && y == 0) { //upper right corner
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX];
	} else if(x == countX-1 && y > 0 && y < countY-1) { //right edge
        surr = currentState[x+(y+1)*countX] + currentState[(x-1)+(y+1)*countX] + currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX];
    } else if(x == countX-1 && y == countY-1) { //lower right corner
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX];
	} else if(x > 0 && x < countX-1 && y == countY-1) { //lower edge
        surr = currentState[(x-1)+y*countX] + currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x+1)+y*countX];
	} else if(x == 0 && y == countY-1) { //lower left corner
        surr = currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x+1)+y*countX];
    } else if(y > 0 && x == 0 && y < countY-1) { //left edge
        surr = currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x+1)+y*countX] + currentState[(x+1)+(y+1)*countX] + currentState[x+(y+1)*countX];
    } else if(x > 0 && y > 0 && x < countX-1 && y < countY-1) { //middle
        surr = currentState[(x-1)+(y-1)*countX] + currentState[x+(y-1)*countX] + currentState[(x+1)+(y-1)*countX] + currentState[(x-1)+(y+1)*countX] + currentState[(x+1)+(y+1)*countX] + currentState[x+(y+1)*countX] + currentState[(x+1)+y*countX] + currentState[(x-1)+y*countX];
    }
	else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error during calculating surrounding\n");
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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error during calculating life, x: %i, y: %i, currentState: %i, surrounding: %i\n", x, y, currentState[x+y*countX], surr);
		exit(EXIT_FAILURE);
    }
}