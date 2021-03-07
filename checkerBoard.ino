typedef struct {
    int rowOrig;
    int colOrig;
    int rowNew;
    int colNew;
    bool hasMove;
} moveStruct;

// PINS
#define electroPin 27
#define dirPinX 11
#define stepPinX 10  
#define dirPinY 6
#define stepPinY 5  
#define hallAnalogPin A0
#define homeXbutton 34 
#define homeYbutton 40
#define playerButton 30

// VARIABLES
int posX = 0;
int posY = 0;
int stepperDelay = 1000;
float rawValue = 0.0;
float zeroLevel = 540.0; // Change to zero output
moveStruct potentialMoves[15];
int board[6][6] = { {0, 1, 0, 1, 0, 1},
                    {1, 0, 1, 0, 1, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 2, 0, 2, 0, 0},
                    {2, 0, 2, 0, 2, 0}}; 
//                      PLAYER SIDE  

/* // TEST BOARD
int board[6][6] = { {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 1, 0, 0},
                    {0, 0, 2, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0}}; 
*/

const int sideSize = 6;
const int stepsPerSquare = 258; 


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(electroPin, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode (hallAnalogPin, INPUT);

  
  //moveHead(-1, -1);

  homeX();
  homeY();
  do{
    
  } while(digitalRead(playerButton) == LOW);
  //moveHead(5, 5);
  //delay(2000);
  //digitalWrite(electroPin, HIGH);
  //moveHead(.5, 6);
  //digitalWrite(electroPin, LOW);
  //printBoard();
  scanBoard();
  //printBoard();
//  updateMoves();
//  movePiece(pickMove());
  do{
    
  } while(digitalRead(playerButton) == LOW);
}

void loop() {
  
  updateMoves();
  movePiece(pickMove());
  do{
    
  } while(digitalRead(playerButton) == LOW);
  scanBoard();
  
  
}



// Pure Software
int lastOpen() {
  for(int i = 0; i < 15; i++){
    if(potentialMoves[i].hasMove == false)
      return i;
  }
}

void checkJumps() { //Tested
  for(int row = 0; row < sideSize; row++){
    for(int col = 0; col < sideSize; col++){
      // Normal Jump
      // For N+K: 2P + 2L
      // DC?: y
      if(((board[row][col] == 1) || (board[row][col] == 3)) && (row + 2 < sideSize) && (col - 2 > -1)){
        if(board[row + 2][col - 2] == 0 && board[row + 1][col - 1] == 2){
          moveStruct addMove = {row, col, (row + 2), (col - 2), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
      // For N+K: 2P + 2R
      // DC?: y
      if(((board[row][col] == 1) || (board[row][col] == 3)) && (row + 2 < sideSize) && (col + 2 < sideSize)){
        if(board[row + 2][col + 2] == 0 && board[row + 1][col + 1] == 2){
          moveStruct addMove = {row, col, (row + 2), (col + 2), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
      // King Jump
      // For K: 2B + 2L
      // DC?: y
      if((board[row][col] == 3) && (row - 2 > -1) && (col - 2 > -1)){
        if(board[row - 2][col - 2] == 0 && board[row - 1][col - 1] == 2){
          moveStruct addMove = {row, col, (row - 2), (col - 2), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
      // For K: 2B + 2R
      // DC?: y
      if((board[row][col] == 3) && (row - 2 > -1) && (col + 2 < sideSize)){
        if(board[row - 2][col + 2] == 0 && board[row - 1][col + 1] == 2){
          moveStruct addMove = {row, col, (row - 2), (col + 2), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
    }
  }
}

void checkMoves() { //Tested
  for(int row = 0; row < sideSize; row++){
    for(int col = 0; col < sideSize; col++){
      // Normal Move
      // For N+K: 1P + 1L
      // DC?: y
      if(((board[row][col] == 1) || (board[row][col] == 3)) && (row + 1 < sideSize) && (col - 1 > -1)){
        if(board[row + 1][col - 1] == 0){
          moveStruct addMove = {row, col, (row + 1), (col - 1), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
      // For N+K: 1P + 1R
      // DC?: y
      if(((board[row][col] == 1) || (board[row][col] == 3)) && (row + 1 < sideSize) && (col + 1 < sideSize)){
        if(board[row + 1][col + 1] == 0){
          moveStruct addMove = {row, col, (row + 1), (col + 1), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
      // King Move
      // For K: 1B + 1L
      // DC?: y
      if((board[row][col] == 3) && (row - 1 > -1) && (col - 1 > -1)){
        if(board[row - 1][col - 1] == 0){
          moveStruct addMove = {row, col, (row - 1), (col - 1), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
      // For K: 1B + 1R
      // DC?: y
      if((board[row][col] == 3) && (row - 1 > -1) && (col + 1 < sideSize)){
        if(board[row - 1][col + 1] == 0){
          moveStruct addMove = {row, col, (row - 1), (col + 1), true};
          potentialMoves[lastOpen()] = addMove;
        }
      }
    }
  }
}

void updateMoves() { //Tested
  for(int i = 0; i < 15; i++){
    potentialMoves[i].hasMove = false;
  }
  checkJumps();
  if(potentialMoves[0].hasMove == false)
    checkMoves();
}

moveStruct pickMove() { //Tested
  int randArrLoc;
  moveStruct potMove;
  do{
    randArrLoc = random(15);
    potMove = potentialMoves[randArrLoc];
  } while (!potMove.hasMove);
  return potMove;
}

void checkKing(){ //Tested
  for(int col = 0; col < 6; col++){
    if(board[5][col] == 1){
      // king jiggle
      board[5][col] = 3;
    }
  }
}


// Software + Hardware Interface
void scanBoard() { //UNTESTED + CHECK
  homeX(); // Need to write
  for(int row = 0; row < sideSize; row++){
    homeY(); // Need to write
    if(row % 2 == 0){
      moveHead(0, 1);
      for(int col = 1; col < sideSize; col+=2){
        if(!pieceDetected()){
          board[row][col] = 0;
        } else {
          if(board[row][col] != 1)
            board[row][col] = 2;
        }
        if(col + 2 < sideSize){
          moveHead(0, 2);
        }
      }
    } else {
      for(int col = 0; col < sideSize - 1; col+=2){
        if(!pieceDetected()){
          board[row][col] = 0;
        } else {
          if(board[row][col] != 1)
            board[row][col] = 2;
        }
        if(col + 2 < sideSize){
          moveHead(0, 2);
        }
      }
    }
    if(row != 5)
      moveHead(1, 0);
  }
  homeX();
  homeY();
  printBoard();
}

void movePiece(moveStruct move){ //UNTESTED + CHECK (Board Update Works)
  // Home X and Y
  homeX();
  homeY();
  
  // Move piece
  if(abs(move.rowNew - move.rowOrig) == 1){
    // IF Moving:  Move X then Move Y + Update Board
    
    // Move to piece current location
    moveHead(move.rowOrig, move.colOrig);

    //ENGAGE ELECTROMAGNET
    digitalWrite(electroPin, HIGH);
    moveHead((move.rowNew - move.rowOrig), (move.colNew - move.colOrig)); // <- DOUBLE CHECK
    //TURN OFF ELECTROMAGNET
    digitalWrite(electroPin, LOW);

  } else {
    // IF Jumping: Remove Jumped Piece + 2x Move X then Move Y + Update Board

    int jumpedRow = move.rowOrig + ((move.rowNew - move.rowOrig) / 2);
    int jumpedCol = move.colOrig + ((move.colNew - move.colOrig) / 2);
    
    // Remove Jumped Piece
    moveHead(jumpedRow, jumpedCol);
    //ENGAGE ELECTROMAGNET
    digitalWrite(electroPin, HIGH);
    moveHead(0.5, (6 - jumpedCol));
    //TURN OFF ELECTROMAGNET
    digitalWrite(electroPin, LOW);
    board[jumpedRow][jumpedCol] = 0;

    // Move piece
    homeX();
    homeY();
    moveHead(move.rowOrig, move.colOrig);
    //ENGAGE ELECTROMAGNET
    digitalWrite(electroPin, HIGH);
    moveHead((jumpedRow - move.rowOrig), (jumpedCol - move.colOrig));
    moveHead((move.rowNew - jumpedRow), (move.colNew - jumpedCol));
    //TURN OFF ELECTROMAGNET
    digitalWrite(electroPin, LOW);
    
  }
  homeX();
  homeY();
  board[move.rowNew][move.colNew] = board[move.rowOrig][move.colOrig];
  board[move.rowOrig][move.colOrig] = 0;
  checkKing();
} // update board with computer move, has electromagnet stuff


// Hardware Control
void moveHead(double rowCount, int colCount) { // UNTESTED + CHECK
  int newXpos = 0;
  if(rowCount == 0.5){
    newXpos = posX + (stepsPerSquare / 2);
  } else {
    newXpos = posX + (rowCount * stepsPerSquare);
  }
  motorToX(newXpos);
  int newYpos = posY + (colCount * stepsPerSquare);
  motorToY(newYpos);
} // Moves x then y, might need to reverse direction


void homeX() { // UNTESTED + CHECK
  int moveBackSteps = 10;
  while(digitalRead(homeXbutton) != HIGH){
    motorToX(posX - moveBackSteps);
  }
  posX = 0;
  motorToX(250);
}


void homeY() { // UNTESTED + CHECK
  int moveBackSteps = 10;
  while(digitalRead(homeYbutton) != HIGH){
    motorToY(posY - moveBackSteps);
  }
  posY = 0;
}


bool pieceDetected() { //UNTESTED + CHECK
  
  double detectedThresh = 4.0; // Need to dail in
  int numTrys = 5;
  double readSum = 0;
  for(int i = 0; i < numTrys; i++){
    delay(50);
    Serial.println(analogRead(hallAnalogPin) - zeroLevel);
    readSum += abs(analogRead(hallAnalogPin) - zeroLevel); 
  }
  if((readSum / numTrys) > detectedThresh)
    return true;
  return false;
}

// bool buttonPushed() {}


// Helper Methods


void printBoard(){
  for(int row = 0; row < sideSize; row++){
    for(int col = 0; col < sideSize; col++){
      Serial.print(board[row][col]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  Serial.println(" ");
  Serial.println(" ");
}

void motorToX(int stepPosX){
  if (stepPosX > posX)
    digitalWrite(dirPinX, LOW);
  else
    digitalWrite(dirPinX, !LOW);
  int stepsX = abs(stepPosX-posX);
  for (int i = 0; i < stepsX; ++i){
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(stepPinX, !HIGH);
    delayMicroseconds(stepperDelay);
  }
  posX = stepPosX;
}

void motorToY(int stepPosY){
  if (stepPosY > posY)
    digitalWrite(dirPinY, LOW);
  else
    digitalWrite(dirPinY, !LOW);
  int stepsY = abs(stepPosY-posY);
  for (int i = 0; i < stepsY; ++i){
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(stepPinY, !HIGH);
    delayMicroseconds(stepperDelay);
  }
  posY = stepPosY;
}
