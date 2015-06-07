short count_x = 45;
short count_y = 34;
int[][] actualState = new int[50][50];
int[][] nextState = new int[50][50];

void setup() {
  frame.setTitle("Jirka Vavřík's Conway's game of life simulator v2.0");
  background(190);
  size(630, 500);
  stroke(0);
  line(0, 481, 630, 481);
  PFont font;
  font = loadFont("FreeSansBold-14.vlw");
  textFont(font, 14);
  fill(111);
  text("Made by Jirka Vavřík, 2014", 4, 496 );
  
  //generate random cell states:
  randomSeed(System.currentTimeMillis()/1000);
  
  for(short tmp_y = 0; tmp_y < count_y; tmp_y++) {
          for(short tmp_x = 0; tmp_x < count_x; tmp_x++) {
               int random = int(random(0, 2)); 
              if(random == 1) {
                    actualState[tmp_x][tmp_y] = 0;
                } else {
                    actualState[tmp_x][tmp_y] = 1;
                }
            }
        }
  
}

void draw() {
  cleanCells();

  //render
  for(short y_ax = 0; y_ax < count_y; y_ax++) {
            for(short x_ax = 0; x_ax < count_x; x_ax++) {
                if(actualState[x_ax][y_ax] == 1) {
                    fill(0, 0, 255);
                } else if(actualState[x_ax][y_ax] == 0){
                    fill(255);
               }
               rect(x_ax*14, y_ax*14, 12, 12);
            }
        }
        
        //calculate next state
        for(short y_ax = 0; y_ax < count_y; y_ax++) {
          for(short x_ax = 0; x_ax < count_x; x_ax++) {
            nextState[x_ax][y_ax] = checkLife(x_ax, y_ax);
        }
    }
        
    //changing actual state
    for(short y_ax = 0; y_ax < count_y; y_ax++) {
        for(short x_ax = 0; x_ax < count_x; x_ax++) {
            actualState[x_ax][y_ax] = nextState[x_ax][y_ax];
        }
    }
    delay(250);
}

/*void error() {
  PFont err;
  err = loadFont("FreeSansBold-50.vlw");
  textFont(err, 50);
  fill(255, 0, 0);
  text("Ooops! An error\n  has occured!", 120, 200 );
  delay(5000);
  exit();
}*/

void cleanCells() { /* Function to clean the screen */
  stroke(0);
  fill(255);
  for(short x = 0; x < 630; x += 14) {
    for(short y = 0; y < 476; y += 14) {
       rect(x, y, 12, 12);
    }
  }
}

void delay(int mils) { /* delay function */
  try
  {    
    Thread.sleep(mils);
  }
  catch(Exception e){}
}

int checkLife(int x, int y) { //This is big sh*t, I know...
    int surr; //surrounding

    if(x == 0 && y == 0) { //left top corner
        surr = actualState[x+1][y] + actualState[x+1][y+1] + actualState[x][y+1];
    } else if (x > 0 && y == 0 && x != count_x) { //top edge
        surr = actualState[x-1][y] + actualState[x-1][y+1] + actualState[x][y+1] + actualState[x+1][y+1] + actualState[x+1][y];
    } else if(x == count_x && y == 0) { //top right corner
        surr = actualState[x-1][y] + actualState[x-1][y+1] + actualState[x][y+1];
    } else if(y > 0 && x == 0 && y < count_y) { //left edge
        surr = actualState[x][y-1] + actualState[x+1][y-1] + actualState[x+1][y] + actualState[x+1][y+1] + actualState[x][y+1];
    } else if(x > 0 && y > 0 && x != count_x && y != count_y) { //middle
        surr = actualState[x-1][y-1] + actualState[x][y-1] + actualState[x+1][y-1] + actualState[y][x+1] + actualState[y+1][x+1] + actualState[x][y+1] + actualState[x-1][y+1] + actualState[x-1][y];
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
        surr = -1;
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
        return -1;
    }
}