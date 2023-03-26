//Eric Gass
//12/8/20
//COP 3223-203
//Final Project
//A C program to play the game minesweeper, a game where using a randomly generated grid filled with mines, the player must find all mines using number clues and luck.
//This program also features functions that automatically reveal all tiles around a 0 number to speed up gameplay
//NOTE: It is entirely possible to land on a mine in the first guess, this is part of the luck element of the game

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//The tile structure which contains fields for if the tile is a mine, what number should be shown on it, if the tile should be shown, and if the tile has been scanned yet
struct Tile
{
  int mine;
  int num;
  int shown;
  int scanned;
};

//The struct is declared here so it becomes a global variable that all the functions can use
struct Tile grid[400];

//function headers
void placeBombs(int mines,int size);
void genNumbers(int width, int height,int size);
int displayGame(int width,int height,int mines);
void displayGame2(int width,int height);
void processGuess(int pos, int width,int height);
void connectedTiles(int size,int width, int height);
int checkResults(int unclickedTiles, int mines);

//Main is going to gather the array details like width, height, and mines
//Main also calls the setup functions placeBombs and genNumbers
//Main then asks the user for a location on the array, and calls processGuess, connectedTiles, and displayGame in a while loop until the user exits
//Once the user exits main determines if the player wins or loses by calling checkResults
int main()
{
//Variables
  int width,height,minenum,size,wguess,hguess,num,i,unclickedTiles;
  int end = 0;

//Gathering User input for the array size
  printf("Enter the width of the playing area (MAX 20)\n");
  scanf("%d",&width);
  printf("Enter the height of the playing area (MAX 20)\n");
  scanf("%d",&height);
  printf("Enter the number of mines in the game\n");
  scanf("%d",&minenum);
  
//if the user inputs an illegal mine number exit the program
  if(minenum>=(width*height))
  {
    printf("\nERROR: number of mines exceeds amount of tiles");
    exit(0);
  }
  
//gets the size of the user specified array
  size = height*width;
  
//calls the first two "setup functions"
  placeBombs(minenum,size);
  genNumbers(width,height,size);

//Uncomment the line below for a debug function that shows the number of every tile if needed  
  //displayGame2(width,height);

//This while loop exits once end becomes 1, which occurs when the user enters 21
//The loop repeats so the users can guess as many tiles as long as they dont hit a mine
  while(end == 0)
  {
    printf("Enter the X value of your guess, or type 21 to check your results:\n");
    scanf("%d",&wguess);
    if(wguess == 21)
    {
      end = 1;
      break;
    } 
    printf("Enter the Y value of your guess:\n");
    scanf("%d",&hguess);

//multiplies the y value by the width, and adds the x value to find the correct tile 
    num = (wguess-1)+((hguess-1)*width);

//calls the functions that scan every tile to reveal the tiles that are supposed to be shown, and displays them in the proper formatting
    processGuess(num,width,height);
    connectedTiles(size,width,height);
    unclickedTiles = displayGame(width,height,minenum);
  }
  
//once the while loop ends the program checks these if statements to see if the user won or not
  if(checkResults(unclickedTiles,minenum)==1)
  {
    printf("Congratulations, you successfully found all the mines!\n\n");
  }

  if(checkResults(unclickedTiles,minenum)==0)
  {
    printf("You did not find all the mines.\n\n");
  }
}

/*
Preconditions: An integer showing the number of mines, and an integer with the size of the user defined array
Postconditions: None
Actions: Uses a random function and a loop to place mines in an array
*/
void placeBombs(int mines,int size)
{
  int i,numb,test=0;
  int placedMine=0;

//the for loop just ensure that all tiles start with a 0 number and are not shown
  for(i=0;i<size;i++)
  {
  grid[i].mine=0;
  grid[i].shown=0;
  }
  
//srand seeds the random function with the current time
  srand(time(NULL));
  while(placedMine<mines)
  {
  numb=rand() % size+1;
  test++;
  
//this if statement ensures that the placed mine isnt on top of another mine, and if it is adds another iteration through the loop
  if(grid[numb].mine==0)
  {
  grid[numb].mine+=1;
  placedMine++;
  }
  }
}

/*
Preconditions: An integer showing the width of the array, an integer showing the height of the array, and an integer showing the size of the user defined array
Postconditions: None
Actions: Uses the generated mines from "placeBombs" to generate the numbers of adjacent mine tiles.
This function find the regions that need specific adjacent tiles to be updated, and also the common scenario where all adjacent tiles are updated
*/
void genNumbers(int width, int height,int size)
{
  int i;
  for(i = 0;i<size;i++)
  {
//if the tile is a mine
    if(grid[i].mine==1)
    {
//if the mine tile is in the top left corner
      if(((i%width)==0)&&((i-width)<0))
      {
        grid[i+1].num++;
        grid[i+width].num++;
        grid[i+width+1].num++;
      }
//if the mine is on the left column
      else if((i%width)==0)
      {
        grid[i-width].num++;
        grid[i+width].num++;
        grid[i-width+1].num++;
        grid[i+1].num++;
        grid[i+width+1].num++;
      }
//if the mine is in the top right corner 
      else if(i==(width-1))
      {
        grid[i-1].num++;
        grid[i+width].num++;
        grid[i+width-1].num++;
      }
//if the mine is in the first row
      else if((width-i)>0)
      {
        grid[i-1].num++;
        grid[i+1].num++;
        grid[i+width-1].num++;
        grid[i+width].num++;
        grid[i+width+1].num++;
      }
//if the mine is on the right column
      else if((i%width)==(width-1))
      {
        grid[i-width].num++;
        grid[i+width].num++;
        grid[i-width-1].num++;
        grid[i-1].num++;
        grid[i+width-1].num++;
      }
//if the mine is anywhere other than those specified regions
      else
      {
        grid[i-width-1].num++;
        grid[i-width].num++;
        grid[i-width+1].num++;
        grid[i+1].num++;
        grid[i-1].num++;
        grid[i+width-1].num++;
        grid[i+width].num++;
        grid[i+width+1].num++;
      }
      
//then set the mine to -9 for later in the program
        grid[i].num=-9;
    }
  }
}

/*
Preconditions: An integer depicicting the width of the array, an integer depicting the height of the array, and the amount of mines
Postconditions: An integer returning the amount of unclicked tiles
Actions: Formats the display of the game and provides additional information to the user. Also determings what should be shown and what shouldnt
*/
int displayGame(int width,int height,int mines)
{
  int w,h,i=0,unclicked=0;
  printf("\n\n\n");
  printf("   ");

//formats the top row showing the number of each column
  for(w=0;w<width;w++)
    {
    printf("%2d ",w+1);
    }
    printf("\n   ");
    
  for(w=0;w<width;w++)
    {
    printf("___");
    }
    printf("\n");
    
  for(h = 0;h<height;h++)
  {
    printf("%2d|",h+1);
    for(w=0;w<width;w++)
    {
// if the tile is supposed to be shown it will display the number, and if not it will show an X
      if(grid[i].shown == 0)
      {
        printf(" X ");
        unclicked++;
      }
      else
      {
        printf("%2d ",grid[i].num);
      }
      i++;
    }
    printf("\n");
  }
  printf("\n");
  printf("Number of unclicked tiles: %d\n",unclicked);
  printf("Number of mines left: %d\n\n",mines);
  return unclicked;
}

//This function is just a debugging function that displays the grid to show every number without the X's
void displayGame2(int width,int height)
{
  int w,h,i=0;
  printf("\n\n\n");
  for(h = 0;h<height;h++)
  {
    for(w=0;w<width;w++)
    {
      if(grid[i].shown == 0)
      {
        printf("%d ",grid[i].num);
      }
      else
      {
        printf("%d ",grid[i].num);
      }
      i++;
    }
    printf("\n");
  }
}

/*
Preconditions: An integer of the position of the users guess on the array, and an integer of the width of the array, and an integer of the height of the array
Postconditions: None
Actions: Takes the users guess and determines what tiles should be shown, or if the user hit a mine.
*/
void processGuess(int pos, int width,int height)
{
int i=pos;
//scanned just in case the user inputs a guess that was already done to speed up the program
  if(grid[i].scanned ==0)
  {
// if the number of the guess is 0 the function tries to reveal adjacent squares using the same region criteria as the earlier functions
  if(grid[i].num==0)
  {
//top left corner
      if(((i%width)==0)&&((i-width)<0))
      {
      grid[i+1].shown++;
      grid[i+width].shown++;
      grid[i+width+1].shown++;
      }
// left column
      else if((i%width)==0)
      {
      grid[i-width].shown++;
      grid[i+width].shown++;
      grid[i-width+1].shown++;
      grid[i+1].shown++;
      grid[i+width+1].shown++;
      }
//top right corner
      else if(i==(width-1))
      {
      grid[i-1].shown++;
      grid[i+width].shown++;
      grid[i+width-1].shown++;
      }
//top row      
      else if((width-i)>0)
      {
        grid[i-1].shown++;
        grid[i+1].shown++;
        grid[i+width-1].shown++;
        grid[i+width].shown++;
        grid[i+width+1].shown++;
      }
// right column
      else if((i%width)==(width-1))
      {
      grid[i-width].shown++;
      grid[i+width].shown++;
      grid[i-width-1].shown++;
      grid[i-1].shown++;
      grid[i+width-1].shown++;
      }
// normal case
      else
      {
      grid[i-width-1].shown++;
      grid[i-width].shown++;
      grid[i-width+1].shown++;
      grid[i+1].shown++;
      grid[i-1].shown++;
      grid[i+width-1].shown++;
      grid[i+width].shown++;
      grid[i+width+1].shown++;
      }
        grid[i].shown++;
  }
//if there is a non 0, non mine number in the tile
  else if(grid[i].num>0)
  {
  grid[i].shown++;
  grid[i].scanned=1;
  }
// if a mine is hit
  else if(grid[i].num<0)
  {
  printf("You hit a mine!\n\n");
  exit(0);
  //end game function here
  }
  grid[i].scanned = 1;  
  }
}

/*
Preconditions: an integer of the size of the array, and integer of the width of the array, and an integer of the height of the array
Postconditions: none
Actions: reveals all tiles around 0 numbered tiles
*/
void connectedTiles(int size,int width, int height)
{
int i,j;
//iterates multiple times
for(j=0;j<20;j++)
{
//iterates through ever tile in the array, plus the previous for loop it iterates through every tile many times
for(i=0;i<size;i++)
{
  if(grid[i].scanned == 0)
  {
  if((grid[i].shown != 0)&&(grid[i].num==0))
  {
// top left corner
    if(((i%width)==0)&&((i-width)<0))
      {
      grid[i+1].shown++;
      grid[i+width].shown++;
      grid[i+width+1].shown++;
      }
// left column
      else if((i%width)==0)
      {
      grid[i-width].shown++;
      grid[i+width].shown++;
      grid[i-width+1].shown++;
      grid[i+1].shown++;
      grid[i+width+1].shown++;
      }
//top right corner
      else if(i==(width-1))
      {
      grid[i-1].shown++;
      grid[i+width].shown++;
      grid[i+width-1].shown++;
      }
//top row
      else if((width-i)>0)
      {
        grid[i-1].shown++;
        grid[i+1].shown++;
        grid[i+width-1].shown++;
        grid[i+width].shown++;
        grid[i+width+1].shown++;
      }
// right column
      else if((i%width)==(width-1))
      {
      grid[i-width].shown++;
      grid[i+width].shown++;
      grid[i-width-1].shown++;
      grid[i-1].shown++;
      grid[i+width-1].shown++;
      }
//common case
      else
      {
      grid[i-width-1].shown++;
      grid[i-width].shown++;
      grid[i-width+1].shown++;
      grid[i+1].shown++;
      grid[i-1].shown++;
      grid[i+width-1].shown++;
      grid[i+width].shown++;
      grid[i+width+1].shown++;
      }
      grid[i].scanned = 1;
  }
  else
  {
  continue;
  }
}
}
}
}

/*
Preconditions: An integer of the unclicked tiles in the array, and an integer of the number of mines
Postconditions: An int of 1 depicting passing or 0 indicating failing
Actions: Compares the number of unclicked tiles to mines
*/
int checkResults(int unclickedTiles,int mines)
{
if(unclickedTiles==mines)
{
  return 1;
}
else 
{
  return 0;
}
}