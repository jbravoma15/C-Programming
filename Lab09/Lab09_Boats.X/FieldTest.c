



#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "BOARD.h"

#include <xc.h>
#include <sys/attribs.h>

#include "Field.h"

#define SUNK 0
#define smallBoat 3
#define mediumBoat 4
#define largeBoat 5
#define hugeBoat 6
#define LargeboatUp 4
#define HugeandSmallBoatUP 9
#define emptySquares 42
#define SqaureOne 59


//Running test

int main(int argc, char** argv) {
    static Field mySide = {};
    static Field enemySide = {};
    uint8_t result;


    BOARD_Init();

    printf("Jbravoma's Field.c Test\n");

    printf("\n We will now begin Field Test!\n");


    printf("\nTesting FieldInit:\n");
    printf("\nFieldInit Test Results \n");
     
    FieldInit(&mySide, &enemySide);
    int r;
    int c;
    result = SUCCESS;

    //testing field Init
    for (r = 0; r < (FIELD_ROWS); r++) {
        for (c = 0; c < (FIELD_COLS); c++) {
            if (mySide.grid[r][c] != FIELD_SQUARE_EMPTY) {
                result = STANDARD_ERROR;
            }
            if (enemySide.grid[r][c] != FIELD_SQUARE_UNKNOWN) {
                result = STANDARD_ERROR;
            }
        }
    }

    if (result != SUCCESS) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }


    if (enemySide.smallBoatLives != smallBoat && enemySide.mediumBoatLives != mediumBoat &&
            enemySide.largeBoatLives != largeBoat && enemySide.hugeBoatLives != hugeBoat) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }




    // testing getsquares and setsquares
    
    printf("\nNow Testing FieldGetSquareStatus and FieldSetSquareStatus:\n");
    printf("\nFieldSetSquareStatus and FieldGetSquareStatus Test Results \n");
    
    SquareStatus square = FieldGetSquareStatus(&mySide, (rand() % FIELD_ROWS), rand() % FIELD_COLS);
    if (square != FIELD_SQUARE_EMPTY) {
        printf("Test1 #1 Failed\n");
    } else {
        printf("Test1 #1 Passed\n");
    }
    
    square = FieldGetSquareStatus(&enemySide, (rand() % FIELD_ROWS), rand() % FIELD_COLS);
    if (square != FIELD_SQUARE_UNKNOWN) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    
    
    int row = rand() % FIELD_ROWS;
    int col = rand() % FIELD_COLS;
    
    square = FieldSetSquareStatus(&mySide, row, col, FIELD_SQUARE_HIT);
    if (FieldGetSquareStatus(&mySide, row, col) != FIELD_SQUARE_HIT) {
        printf("Test #3 Failed\n");
    } else {
        printf("Test #3 Passed\n");
    }
    
    
    FieldSetSquareStatus(&mySide, row, col, FIELD_SQUARE_EMPTY);
    if (FieldGetSquareStatus(&mySide, row, col) != square) {
        printf("Test #4 Failed\n");
    } else {
        printf("Test #4 Passed\n");
    }
    
    
    
    
    
    
    //adds various boats and check if spot taken and or off grid
    printf("\nNow Testing FieldAddBoat:\n");
     printf("\nFieldAddBoat Test Results \n");
    
    result = FieldAddBoat(&mySide, 2, 2, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);
    
    if (result != SUCCESS && FieldGetSquareStatus(&mySide, 2, 2) != FIELD_SQUARE_SMALL_BOAT) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }
    
    if (mySide.smallBoatLives != smallBoat && mySide.mediumBoatLives != SUNK) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    if (FieldAddBoat(&mySide, 2, 1, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) != STANDARD_ERROR) {
 
        printf("Test #3 Failed \n");
    } else {
        printf("Test #3 Passed \n");
    }
    if (FieldAddBoat(&mySide, 2, 8, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) != STANDARD_ERROR) {
        printf("Test #4 Failed\n");
    } else {
        printf("Test #4 Failed\n");
    }


    //register and update properly
    static GuessData guess = {};
    printf("\nTesting FieldRegisterEnemyAttack:\n");
    printf("\nFieldRegisterEnemyAttack Test Results\n");
    
    guess.row = 2;
    guess.col = 2;
    FieldRegisterEnemyAttack(&mySide, &guess);
    if (FieldGetSquareStatus(&mySide, 2, 2) != FIELD_SQUARE_HIT && guess.result != RESULT_HIT) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }
    if (mySide.smallBoatLives != 2) {
       printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    
    guess.row = 0;
    guess.col = 0;
    FieldRegisterEnemyAttack(&mySide, &guess);
    if (FieldGetSquareStatus(&mySide, 0, 0) != FIELD_SQUARE_MISS && guess.result != RESULT_MISS) {
        printf("Test #3 Failed\n");
    } else {
        printf("Test #3 Passed\n");
    }
    
    

    
    printf("\nTesting FieldUpdateKnowledge:\n");
    printf("\nFieldUpdateKnowledge test Results \n");
    
    guess.row = 2;
    guess.col = 2;
    guess.result = RESULT_HIT;
    FieldUpdateKnowledge(&mySide, &guess);
    if (FieldGetSquareStatus(&mySide, 2, 2) != FIELD_SQUARE_HIT) {

        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }
    guess.row = 2;
    guess.col = 2;
    guess.result = RESULT_MISS;
    FieldUpdateKnowledge(&mySide, &guess);
    if (FieldGetSquareStatus(&mySide, 2, 2) != FIELD_SQUARE_EMPTY) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }

 
    
    printf("\nNow Testing FieldGetBoatStates:\n");
    printf("\nFieldGetBoatStates Test Result \n");
    
    mySide.smallBoatLives = SUNK;
    mySide.mediumBoatLives = SUNK;
    mySide.largeBoatLives = SUNK;
    mySide.hugeBoatLives = SUNK;
    
    
    if (FieldGetBoatStates(&mySide) == SUNK) {
        printf("Test #1 Passed\n");
    } else {
        printf("Test #1 Failed\n");
    }
    mySide.largeBoatLives = largeBoat;
    if (FieldGetBoatStates(&mySide) != LargeboatUp) {
       printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    mySide.smallBoatLives = smallBoat;
    mySide.mediumBoatLives = SUNK;
    mySide.largeBoatLives = SUNK;
    mySide.hugeBoatLives = hugeBoat;

    if (FieldGetBoatStates(&mySide) != HugeandSmallBoatUP) {
        printf("Test #3 Failed\n");
    } else {
        printf("Test #3 Passed\n");
    }
    

    printf("\nTesting FieldAIPlaceAllBoats:\n");
    printf("\nFieldAIPlaceAllBoats Test Result\n");
    
    FieldInit(&mySide, &enemySide);
    
    if (FieldAIPlaceAllBoats(&mySide) != SUCCESS) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }
    if (mySide.smallBoatLives != smallBoat && mySide.mediumBoatLives != mediumBoat && mySide.largeBoatLives == largeBoat && mySide.hugeBoatLives != hugeBoat) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    
    result = 0;
    for (r = 0; r < (FIELD_ROWS); r++) {
        for (c = 0; c < (FIELD_COLS); c++) {
            if (mySide.grid[r][c] == FIELD_SQUARE_EMPTY) {
                result++;
            }
        }
    }
    if (result != emptySquares) {
        printf("Test #3 Failed\n");
    } else {
        printf("Test #3 Passed\n");
    }

    
    
    
    printf("\nTesting FieldAIDecideGuess:\n");
    printf("\nFieldAIDecideGuess Test Results \n");
     
    for (r = 0; r < SqaureOne; r++) {
        guess = FieldAIDecideGuess(&enemySide);
        enemySide.grid[guess.row][guess.col] = FIELD_SQUARE_HIT;
    }
    result = 0;
    for (r = 0; r < (FIELD_ROWS); r++) {
        for (c = 0; c < (FIELD_COLS); c++) {
            if (enemySide.grid[r][c] == FIELD_SQUARE_HIT) {
                result++;
            }
        }
    }
    if (result != SqaureOne) {
         printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }
    
    result = 0;
    for (r = 0; r < (FIELD_ROWS); r++) {
        for (c = 0; c < (FIELD_COLS); c++) {
            if (enemySide.grid[r][c] == FIELD_SQUARE_UNKNOWN) {
                result++;
            }
        }
    }
    if (result != 1) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    


    printf("\nEnd of Testing\n");




    return (EXIT_SUCCESS);
}


