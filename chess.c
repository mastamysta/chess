// TO DO prgoram will crash if cell input is not 2 chars long
//a programme that allows two players to play chess
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>

// --------  setup --------------------------------------

enum epiece{Empty, Pawn1, Rook1, Bishop1, Knight1, Queen1, King1, Pawn2, Rook2, Bishop2, Knight2, Queen2, King2};
typedef enum epiece Piece;

enum egamestate{Playing, P1Wins, P2Wins, Stalemate};
typedef enum egamestate Gamestate;

enum eplayers{Player1, Player2};
typedef enum eplayers Players;

struct game{
  Piece board[8][8]; // behave like matrix across top down side
  Gamestate winner;
  int turns;
  Players currentturn;
};
typedef struct game game;

// --------  functionality -----------------------------

//Assign all cells on board as empty
void emptyBoard(game *g){
  for(int i = 0; i < 8; i ++){
    for(int j = 0; j < 8; j ++){
      g->board[i][j] = Empty;
    }
   }
}

//Assign all pawns in default position
void assignPawns(game *g){
  for(int i = 0; i < 8; i ++){
    g->board[i][1] = Pawn1;
    g->board[i][6] = Pawn2;
   }
}

//Assign all rooks in default position
void assignRooks(game *g){
  g->board[0][0] = Rook1;
  g->board[0][7] = Rook2;
  g->board[7][0] = Rook1;
  g->board[7][7] = Rook2;
}

//Assign all bishops in default position
void assignBishops(game *g){
 g->board[2][0] = Bishop1;
 g->board[5][0] = Bishop1;
 g->board[2][7] = Bishop2;
 g->board[5][7] = Bishop2;
}

//Assign all knights in default position
void assignKnights(game *g){
  g->board[1][0] = Knight1;
  g->board[6][0] = Knight1;
  g->board[1][7] = Knight2;
  g->board[6][7] = Knight2;
}

//Assign all Queens in default positions
void assignQueens(game *g){
  g->board[4][0] = Queen1;
  g->board[3][7] = Queen2;
}

//Assign all Kings in default positions
void assignKings(game *g){
  g->board[3][0] = King1;
  g->board[4][7] = King2;
}

//Set up each piece on the board by calling assignation functions
void setUpBoard(game *g){
  emptyBoard(g);
  assignPawns(g);
  assignRooks(g);
  assignBishops(g);
  assignKnights(g);
  assignQueens(g);
  assignKings(g);
}

//set up entire game struct for start of game
void setUpGame(game *g){
  setUpBoard(g);
  g->winner = Playing;
  g->turns = 0;
  g->currentturn = Player1;
}

//get an input from player ************SPACES IN ARGS*****************
int getPlayerInput(char *Input){
  scanf("%s", Input);
  return 0;
}

bool validateUserInput(char *input){
  int isDigit1 = isdigit(input[0]);
  int isDigit2 = isdigit(input[1]);
  return isDigit1 == 0 && isDigit2 > 0 && input[2] == '\0';
}

//find row value from user imput
int row(char *Input){
  return (Input[1] - '0') - 1;
}

//find row value from user input
int col(char *Input){
  switch (Input[0]){
    case 'a': return 0; break;
    case 'b': return 1; break;
    case 'c': return 2; break;
    case 'd': return 3; break;
    case 'e': return 4; break;
    case 'f': return 5; break;
    case 'g': return 6; break;
    case 'h': return 7; break;
    default: return -1; break;
  }
}

//find whether a given input is between the array bounds
bool isOnBoard(int row, int col){
  if(row < 0 || row > 7 || col < 0 || col > 7){
    return false;
  }
  else{
    return true;
  }
}

//decide whether cell is empty
bool isEmptyCell(int row, int col, game *g){
  if(g->board[row][col] == 0){
    return true;
  }
  return false;
}
//funny joke ^^^^^^^^^^^^^^^^ return g->board[col][row] == 0;

//does the stated cell contain one of the current players pieces
bool isPlayersPieceInCell(game *g,int row,int col){
  if(g->board[row][col] == Empty){
    return false;
  }
  else if(g->board[row][col] < 7 && g->currentturn == Player2){
    return false;
  }
  else if(g->board[row][col] > 6 && g->currentturn == Player1){
    return false;
  }
  return true;
}

bool isOponentsPieceInCell(game *g, int row, int col){
  if(g->board[row][col] == Empty){
    return false;
  }
  else if(g->board[row][col] > 6 && g->currentturn == Player2){
    return false;
  }
  else if(g->board[row][col] < 7 && g->currentturn == Player1){
    return false;
  }
  return true;
}

//is the stated move diagonal
bool isDiagonalMove(int rowFrom, int colFrom, int rowTo, int colTo){
  int deltaRow, deltaCol;
  deltaRow = rowTo - rowFrom;
  deltaCol = colTo - colFrom;
  return abs(deltaRow) == abs(deltaCol);

}

//is the stated move straight
bool isStraightMove(int rowFrom, int colFrom, int rowTo, int colTo){
  int deltaRow, deltaCol;
  deltaRow = rowTo - rowFrom;
  deltaCol = colTo - colFrom;
  return deltaRow == 0 || deltaCol == 0;
}

bool is1Right(int dRow, int dCol, int row, int col, game *g){
  return isEmptyCell(row + dRow, col + dCol, g) && dRow == 0 && dCol == 1;
}

bool is2Right1stTurn(int dRow, int dCol, int row, int col, game *g){
  return isEmptyCell(row + dRow, col + dCol, g) && dRow == 0 && dCol == 2 && g->turns == 0;
}

bool isPawn1Take(int dRow, int dCol, int row, int col, game *g){
 return isOponentsPieceInCell(g, row + dRow, col + dCol) && ((dRow == 1 && dCol == 1) || (dRow == -1 && dCol == 1));
}

//validate a move for a pawn1 given the change in row and column, starting position and board state
bool isValidForPawn1(int dRow, int dCol, int row, int col, game *g){
 return is1Right(dRow, dCol, row, col, g) || is2Right1stTurn(dRow, dCol, row, col, g) || isPawn1Take(dRow, dCol, row, col, g);
}

bool is1Left(int dRow, int dCol, int row, int col, game *g){
  return isEmptyCell(row + dRow, col + dCol, g) && dRow == 0 && dCol == -1;
}

bool is2Left1stTurn(int dRow, int dCol, int row, int col, game *g){
  return isEmptyCell(row + dRow, col + dCol, g) && dRow == 0 && dCol == -2 && g->turns == 1;
}

bool isPawn2Take(int dRow, int dCol, int row, int col, game *g){
  return isOponentsPieceInCell(g, row + dRow, col + dCol) && ((dRow == 1 && dCol == -1) || (dRow == -1 && dCol == -1));
}

//validate a move for a pawn2 given change in row and column, starting position and board state
bool isValidForPawn2(int dRow,int dCol, int row, int col, game *g){
  return is1Left(dRow, dCol, row, col, g) || is2Left1stTurn(dRow, dCol, row, col, g) || isPawn2Take(dRow, dCol, row, col, g);
}

bool isLeftPathEmpty(int dRow,int dCol, int row, int col, game *g){
bool isPieceOnPath = false;
for(int i = col - 1; i > col + dCol; i --){
  if (isEmptyCell(row, i, g) == false){
    isPieceOnPath = true;
  }
}
return !isPieceOnPath;
}

bool isRightPathEmpty(int dRow,int dCol, int row, int col, game *g){
  bool isPieceOnPath = false;
  for(int i = col + 1; i < col + dCol; i ++){
    if (isEmptyCell(row, i, g) == false){
      isPieceOnPath = true;
    }
  }
  return !isPieceOnPath;
}

bool isUpwardPathEmpty(int dRow,int dCol, int row, int col, game *g){
  bool isPieceOnPath = false;
  for(int i = row - 1; i > row + dRow; i --){
    if (isEmptyCell(i, col, g) == false){
      isPieceOnPath = true;
    }
  }
  return !isPieceOnPath;
}

bool isDownwardPathEmpty(int dRow,int dCol, int row, int col, game *g){
  bool isPieceOnPath = false;
  for(int i = row + 1; i < row + dRow; i ++){
    if (isEmptyCell(i, col, g) == false){
      isPieceOnPath = true;
    }
  }
  return !isPieceOnPath;
}

bool isStraightPathEmpty(int dRow,int dCol, int row, int col, game *g){
if(dRow > 0 && dCol == 0){
  return isDownwardPathEmpty(dRow, dCol, row, col, g);
}
else if(dRow < 0 && dCol == 0){
  return isUpwardPathEmpty(dRow, dCol, row, col, g);
}
else if (dCol > 0 && dRow == 0){
  return isRightPathEmpty(dRow, dCol, row, col, g);
}
else {
  return isLeftPathEmpty(dRow, dCol, row, col, g);
}
}

bool isValidForRook(int dRow,int dCol, int row, int col, game *g){
return isStraightPathEmpty(dRow, dCol, row, col, g) && isStraightMove(row, col, row + dRow, col + dCol);
}

//angles done from x axis
bool is315DegreesPathEmpty(int dRow, int dCol, int row, int col, game *g){
  bool isPieceOnPath = false;
    for(int i = 1; i < abs(dRow); i ++){
      if(!isEmptyCell(row + i, col + i, g)){
        isPieceOnPath = true;
      }
    }
    return !isPieceOnPath;
}

bool is225DegreesPathEmpty(int dRow, int dCol, int row, int col, game *g){
  bool isPieceOnPath = false;
    for(int i = 1; i < abs(dRow); i ++){
      if(!isEmptyCell(row + i, col - i, g)){
        isPieceOnPath = true;
      }
    }
    return !isPieceOnPath;
}

bool is135DegreesPathEmpty(int dRow, int dCol, int row, int col, game *g){
  bool isPieceOnPath = false;
    for(int i = 1; i < abs(dRow); i ++){
      if(!isEmptyCell(row - i, col - i, g)){
        isPieceOnPath = true;
      }
    }
    return !isPieceOnPath;
}

bool is45DegreesPathEmpty(int dRow, int dCol, int row, int col, game *g){
bool isPieceOnPath = false;
  for(int i = 1; i < abs(dRow); i ++){
    if(!isEmptyCell(row - i, col + i, g)){
      isPieceOnPath = true;
    }
  }
  return !isPieceOnPath;
}

bool isDiagonalPathEmpty(int dRow, int dCol, int row, int col, game *g){
if(dRow < 0 && dCol > 0){
  return is45DegreesPathEmpty(dRow, dCol, row, col, g);
}
else if(dRow < 0 && dCol < 0){
  return is135DegreesPathEmpty(dRow, dCol, row, col, g);
}
else if(dRow > 0 && dCol < 0){
  return is225DegreesPathEmpty(dRow, dCol, row, col, g);
}
else{
  return is315DegreesPathEmpty(dRow, dCol, row, col, g);
}
}

bool isValidForBishop(int dRow, int dCol, int row, int col, game *g){
return isDiagonalMove(row, col, row + dRow, col + dCol) && isDiagonalPathEmpty(dRow, dCol, row, col, g);
}

bool isValidForKnight(int dRow,int dCol, int row, int col, game *g){
  return (abs(dCol) == 1 && abs(dRow) == 2) || (abs(dCol) == 2 && abs(dRow) == 1);
}

bool isValidForQueen(int dRow, int dCol, int row, int col, game * g){
bool validDiagonalMove = false;
bool validLinearMove = false;
if(isStraightMove(row, col, row + dRow, col + dCol) == true){
  validLinearMove = isStraightPathEmpty(dRow, dCol, row, col, g);
}
else if(isDiagonalMove(row, col, row + dRow, col + dCol) == true){
  validDiagonalMove = isDiagonalPathEmpty(dRow, dCol, row, col, g);
}

return validDiagonalMove || validLinearMove;
}

bool isValidForKing(int dRow, int dCol, int row, int col, game *g){
  return (dRow == 1 || dRow == 0 || dRow == -1) && (dCol == 1 || dCol == 0 || dCol == -1);
}

//run the correct validation on the move which the user wants to make
bool isValidForPiece(int deltaRow, int deltaCol, int rowFrom, int colFrom, game *g){
  Piece p = g->board[rowFrom][colFrom];
  switch(p){
    case 1: return isValidForPawn1(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 2: return isValidForRook(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 3: return isValidForBishop(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 4: return isValidForKnight(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 7: return isValidForPawn2(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 8: return isValidForRook(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 9: return isValidForBishop(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 10: return isValidForQueen(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    case 11: return isValidForKing(deltaRow, deltaCol, rowFrom, colFrom, g); break;
    default: return false; break;
  }
}

bool isComponent2Valid(game *g, int rFrom, int cFrom,int rTo,int cTo){
return isOnBoard(rTo, cTo) && ~isPlayersPieceInCell(g, rTo, cTo) && isValidForPiece(rTo - rFrom, cTo - cFrom, rFrom, cFrom, g);
}

bool isComponent1Valid(game *g, int row, int col){
return isOnBoard(row, col) && isPlayersPieceInCell(g, row, col);
}

//update the current turn and turn counter
void endOfTurnUpdates(game *g){
  g->turns = g->turns + 1;

  if(g->currentturn == Player1){
   g->currentturn = Player2;
  }
 else{
   g->currentturn = Player1;
}
}

//get the first component of user input fully validated
void getComponent1(game *g, char *Input1){
  int rFrom, cFrom;
  do{
    sprintf(Input1, "%s", "");
    do{
      printf("Which peice would you like to move?\n");
      getPlayerInput(Input1);
    }while(validateUserInput(Input1) == false);
    rFrom = row(Input1);
    cFrom = col(Input1);
  }while(isComponent1Valid(g, rFrom, cFrom) == false);
}

//get the second component of user input fully validated
void getComponent2(game *g, char *Input2,int rFrom,int cFrom){
int rTo, cTo;
do{
  sprintf(Input2, "%s", "");
  do{
printf("Where would you like to move it to?\n");
getPlayerInput(Input2);
}while(validateUserInput(Input2) == false);
  rTo = row(Input2);
  cTo = col(Input2);
}while(isComponent2Valid(g, rFrom, cFrom, rTo, cTo) == false);
}

void movePiece(int rFrom, int cFrom, int rTo, int cTo, game *g){
  g->board[rTo][cTo] = g->board[rFrom][cFrom];
  g->board[rFrom][cFrom] = Empty;
}

//check to see if either king has been taken
void isGameOver(game *g){
  bool king1Present = false, king2Present = false;
  for(int i = 0; i < 8; i ++){
    for(int j = 0; j < 8; j ++){
      if(g->board[i][j] == King1){
        king1Present = true;
      }
      else if(g->board[i][j] == King2){
        king2Present = true;
      }
    }
  }
  if(king1Present == false){
    g->winner = P2Wins;
  }
  else if(king2Present == false){
    g->winner = P1Wins;
  }
}


//individual turn for a player
void haveTurn(game *g){
char *Input1 = malloc(24);
char *Input2 = malloc(24);
int rFrom, cFrom, rTo, cTo;
getComponent1(g, Input1);
rFrom = row(Input1);
cFrom = col(Input1);
getComponent2(g, Input2, rFrom, cFrom);
rTo = row(Input2);
cTo = col(Input2);
movePiece(rFrom, cFrom, rTo, cTo, g);

endOfTurnUpdates(g);
}

//print out correct unicode symbol for each peice
void displayCell(Piece p){
  switch(p){
    case Pawn1: printf("%lc", 0x2659); break;
    case Rook1: printf("%lc", 0x2656); break;
    case Bishop1: printf("%lc", 0x2657); break;
    case Knight1:printf("%lc", 0x2658); break;
    case Queen1: printf("%lc", 0x2655); break;
    case King1: printf("%lc", 0x2654); break;
    case Pawn2: printf("%lc", 0x265F); break;
    case Rook2: printf("%lc", 0x265C); break;
    case Bishop2: printf("%lc", 0x265D); break;
    case Knight2:printf("%lc", 0x265E); break;
    case Queen2: printf("%lc", 0x265A); break;
    case King2: printf("%lc", 0x265B); break;
    default: printf(" "); break;
  }
}

//display the board in terminal
void displayBoard(game *g){
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf("    a   b   c   d   e   f   g   h \n");
  for(int i = 0; i < 8; i ++){
    printf("  --------------------------------- \n%i | ", (i + 1));
      for(int j = 0; j < 8; j ++){
      displayCell(g->board[i][j]);
      printf(" | ");
    }
    printf("\n");
  }
  printf("  --------------------------------- \n");
}

//cycle through users turns
void turnCycle(game *g){
 do{
   displayBoard(g);
  haveTurn(g);

 }while(g->winner == Playing);
}

int playGame(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  turnCycle(g);
  printf("%u wins!\n", g->winner);

  free(g);
  return 0;
}

// --------  testing and main  -------------------------

//test that the emty board function sets all cells as empty successfully
void testEmptyBoard(){
  game *g = malloc(sizeof(game));
  emptyBoard(g);
  for(int i = 0; i < 8; i ++){
    for(int j = 0; j < 8; j ++){
     assert(g->board[i][j] == Empty);
    }
  }
  free(g);
}

//test for get player input has to be run with input of a
void testGetPlayerInput(){
  char testValue[30];
  getPlayerInput(testValue);
  assert(strcmp(testValue, "a") == 0);
}

//test function to create a game with peices in default positions and print it out
void testCreateandDisplay(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  displayBoard(g);
  free(g);
}

//test the validation oftwo char user Input
void testValidateUserInput(){
  char *input = malloc(24);
  sprintf(input, "%s", "potato\0");
  assert(validateUserInput(input) == false);
  sprintf(input, "%s", "a4\0");
  assert(validateUserInput(input));
  sprintf(input, "%s", "5b\0");
  assert(validateUserInput(input) == false);
  free(input);
}

//test for col functions
void testCol(){
  assert(col("a1") == 0);
  assert(col("b3") == 1);
  assert(col("d5") == 3);
  assert(col("h2") == 7);
}

//test for row functions
void testRow(){
  assert(row("a1") == 0);
  assert(row("b7") == 6);
  assert(row("d8") == 7);
  assert(row("d5") == 4);
}

//test isonboard functions
void testIsOnBoard(){
  assert(isOnBoard(0, 0) == true);
  assert(isOnBoard(7, 7) == true);
  assert(isOnBoard(-1, 6) == false);
  assert(isOnBoard(6, 8) == false);
}

//test isemptycell function
void testIsEmptyCell(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  assert(isEmptyCell(0, 0, g));
  assert(isEmptyCell(3, 5, g));

  free(g);
}

void testIsPlayersPiece(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  g->board[3][0] = Pawn1;
  assert(isPlayersPieceInCell(g, 3, 0));
  assert(isPlayersPieceInCell(g, 0, 0) == false);
  g->currentturn = Player2;
  assert(isPlayersPieceInCell(g, 3, 0) == false);
  g->board[7][7] = King2;
  assert(isPlayersPieceInCell(g, 7, 7));
  assert(isPlayersPieceInCell(g, 4, 5) == false);
  free(g);
}

void testIsDiagonalMove(){
  assert(isDiagonalMove(5, 5, 7, 7));
  assert(isDiagonalMove(3, 4, 6, 7));
  assert(isDiagonalMove(5, 7, 1, 3));
  assert(isDiagonalMove(3, 3, 5, 6) == false);
  assert(isDiagonalMove(3, 8, 1, 6) == false);
}

void testIsStraightMove(){
  assert(isStraightMove(0, 0, 0, 7));
  assert(isStraightMove(3, 4, 3, 7));
  assert(isStraightMove(2, 5, 5, 5));
  assert(isStraightMove(7, 3, 3, 3));
  assert(isStraightMove(7, 7, 3, 3) == false);
  assert(isStraightMove(3, 5, 7, 6) == false);
}

void testOponentsPieceInCell(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  assert(isOponentsPieceInCell(g, 4, 5) == false);
  g->board[4][5] = Rook2;
  assert(isOponentsPieceInCell(g, 4, 5));
  g->currentturn = Player2;
  assert(isOponentsPieceInCell(g, 4, 5) == false);
  assert(isOponentsPieceInCell(g, 0, 0) == false);
  g->board[0][0] = Queen1;
  assert(isOponentsPieceInCell(g, 0, 0));

  free(g);
}

void testisComponent1Valid(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);

  g->board[0][0] = Pawn1;
  g->board[1][0] = Pawn2;
  assert(isComponent1Valid(g, 0, 1) == false);
  assert(isComponent1Valid(g, 0, 0));
  assert(isComponent1Valid(g, 1, 0) == false);
  g->currentturn = Player2;
  assert(isComponent1Valid(g, 0, 1) == false);
  assert(isComponent1Valid(g, 0, 0) == false);
  assert(isComponent1Valid(g, 1, 0));
  assert(isComponent1Valid(g, -1, 3) == false);
  assert(isComponent1Valid(g, 8, 8) == false);

  free(g);
}

void testIsValidForPawn1(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  assert(isValidForPawn1(0, 2, 0, 0, g));
  g->turns ++;
  assert(isValidForPawn1(0, 1, 0, 0, g));
  assert(isValidForPawn1(1, 1, 6, 0, g) == false);
  assert(isValidForPawn1(0, -1, 5, 5, g) == false);
  assert(isValidForPawn1(0, 2, 0 ,0, g) == false);
  g->board[5][4] = King2;
  g->board[3][4] = King2;
  assert(isValidForPawn1(1, 1, 4, 3, g));
  assert(isValidForPawn1(-1, 1, 4, 3, g));

  free(g);
}

void testIsValidForPawn2(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  g->currentturn = Player2;
  g->turns ++;
  assert(isValidForPawn2(0, -2, 0, 2, g));
  g->turns ++;
  assert(isValidForPawn2(0, -1, 0, 1, g));
  assert(isValidForPawn2(-1, -1, 6, 1, g) == false);
  assert(isValidForPawn2(0, 1, 5, 5, g) == false);
  assert(isValidForPawn2(0, -2, 0 , 2, g) == false);
  g->board[5][4] = King1;
  g->board[3][4] = King1;
  assert(isValidForPawn2(-1, -1, 4, 5, g));
  assert(isValidForPawn2(1, -1, 4, 5, g));

  free(g);
}

void testIsValidForRook(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  assert(isValidForRook(0, 1, 0, 0, g));
  assert(isValidForRook(3, 0, 0, 0, g));
  assert(isValidForRook(-5, 0, 6, 3, g));
  assert(isValidForRook(0, -4, 0, 6, g));
  g->board[4][5] = Bishop2;
  assert(isValidForRook(0, 3, 4, 2, g));
  assert(isValidForRook(4, 0, 0, 5, g));
  assert(isValidForRook(3, 3, 0, 0, g) == false);
  assert(isValidForRook(5, 0, 2, 5, g) == false);
  g->currentturn = Player2;
  assert(isValidForRook(5, 0, 2, 5, g) == false);
  assert(isValidForRook(-7, 0, 7, 3, g));
  assert(isValidForRook(5, -5, 3, 7, g) == false);

  free(g);
}

void testIsValidForBishop(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);
  assert(isValidForBishop(3, 3, 0, 0, g));
  assert(isValidForBishop(-3, -3, 7, 6, g));
  assert(isValidForBishop(5, -5, 2, 7, g));
  assert(isValidForBishop(-1, 1, 3, 4, g));
  g->board[4][4] = Pawn1;
  assert(isValidForBishop(-2, -2, 5, 5, g) == false);
  g->currentturn = Player2;
  assert(isValidForBishop(3, 3, 3, 3, g) == false);

  free(g);
}

void testIsValidForKnight(){
  game *g = malloc(sizeof(game));
  setUpGame(g);
  emptyBoard(g);

assert(isValidForKnight(2, 1, 3, 4, g));
assert(isValidForKnight(-2, -1, 5, 5, g));
assert(isValidForKnight(-5, 3, 6, 4, g) == false);

  free(g);
}

//confidence in diagonal and linear checks from previous testing so not done for
//king and queen

//run all test functions
void runTest(){
  testEmptyBoard();
  //testGetPlayerInput();
  testCreateandDisplay();
  testValidateUserInput();
  testCol();
  testRow();
  testIsEmptyCell();
  testIsPlayersPiece();
  testIsStraightMove();
  testOponentsPieceInCell();
  testisComponent1Valid();
  testIsValidForPawn1();
  testIsValidForPawn2();
  testIsValidForRook();
  testIsValidForBishop();
  testIsValidForKnight();
  printf("All tests pass!\n");
}

int main(int n, char *args[]){
  //allows unicodes to be used
  setlocale(LC_ALL, "C.UTF-8");
  if(n == 2){
  runTest();
  }
  else{
    playGame();
  }
  return 0;
}
