//Jorge Bravo Martinez
//Jbravoma


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Field.h"
#include "BOARD.h"

#define getboatType 3
#define getboatStatus 1
#define smallBoat 3
#define mediumBoat 4
#define largeBoat 5
#define hugeBoat 6
#define smallBoatAlive 1
#define mediumBoatAlive 2
#define largeBoatAlive 4
#define hugeBoatAlive 8
#define SUNK 0
#define randDir 2




void FieldPrint_UART(Field *own_field, Field * opp_field){
    
    //creatiing fields
    printf("\n My field:\n");
    int row;
    int col;
    for(row = 0;row < FIELD_ROWS;row++){
        printf("\n");
        for(col = 0;col < FIELD_COLS;col++){
            printf("%d",own_field->grid[row][col]);
        }
    }
    //creating fields
    printf("\n Enemy field:\n");
    for(row = 0;row < FIELD_ROWS;row++){
        printf("\n");
        for(col = 0;col < FIELD_COLS;col++){
            printf("%d",own_field->grid[row][col]);
        }
    }
}


/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus (
 * FIELD_SQUARE_EMPTY for your own field, FIELD_SQUARE_UNKNOWN for opponent's).
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be filled when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field * opp_field){
    int row;
    int col;
    
    
    for(row = 0;row < FIELD_ROWS;row++){
        for(col = 0;col < FIELD_COLS;col++){
            own_field->grid[row][col] = FIELD_SQUARE_EMPTY;
            opp_field->grid[row][col] = FIELD_SQUARE_UNKNOWN;
        }
    }
    
    //SETTING BOATS 
    opp_field->smallBoatLives = smallBoat;
    opp_field->mediumBoatLives = mediumBoat;
    opp_field->largeBoatLives = largeBoat;
    opp_field->hugeBoatLives = largeBoat;
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col){
    if(f->grid[row][col] == FIELD_SQUARE_EMPTY){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_SMALL_BOAT){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_MEDIUM_BOAT){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_LARGE_BOAT){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_HUGE_BOAT){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_HIT){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_MISS){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_CURSOR){
        return f->grid[row][col];
    }
    else if(f->grid[row][col] == FIELD_SQUARE_UNKNOWN){
        return f->grid[row][col];
    }else{
        return FIELD_SQUARE_INVALID;
    }
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p){
    SquareStatus previousStat = f->grid[row][col];
    f->grid[row][col] = p;
    return previousStat;
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. 
 * 
 * All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 * 
 * In addition, this function should update the appropriate boatLives parameter of the field.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type){
    int boatSize = boat_type +  getboatType;
    int f;
    //check of the squares are empty and in field size otherwise fail
    for(f = 0;f < boatSize;f++){
        if(dir == FIELD_DIR_SOUTH){
            if(own_field->grid[row + f][col] != FIELD_SQUARE_EMPTY || (row + f) >= FIELD_ROWS){
                return STANDARD_ERROR;
            }
        }
        else if(dir == FIELD_DIR_EAST){
            if(own_field->grid[row][col + f] != FIELD_SQUARE_EMPTY || (col + f) >= FIELD_COLS){
                return STANDARD_ERROR;
            }
        }
        else{
            return STANDARD_ERROR;
        }
    }
    
    SquareStatus boatsquare = boat_type + getboatStatus;
    
    for(f = 0;f < boatSize ;f++){
        
        if(dir != FIELD_DIR_SOUTH && dir != FIELD_DIR_EAST){
            
            return STANDARD_ERROR;
        }
        else if(dir == FIELD_DIR_EAST){
            
            own_field->grid[row][col + f] = boatsquare;
        }
        else if(dir == FIELD_DIR_SOUTH){
            own_field->grid[row + f][col] = boatsquare;
        }
            
    }
 
    
    if(boat_type == FIELD_BOAT_TYPE_SMALL){
        own_field->smallBoatLives = smallBoat;
    }
    else if(boat_type == FIELD_BOAT_TYPE_MEDIUM){
        own_field->mediumBoatLives = mediumBoat;
    }
    else if(boat_type == FIELD_BOAT_TYPE_LARGE){
        own_field->largeBoatLives = largeBoat;
    }
    else if(boat_type == FIELD_BOAT_TYPE_HUGE){
        own_field->hugeBoatLives = hugeBoat;
    }
    else{
            return STANDARD_ERROR;
        }
    return SUCCESS;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess){
    SquareStatus previousVal = own_field->grid[opp_guess->row][opp_guess->col];
    
    
    if(own_field->grid[opp_guess->row][opp_guess->col] == FIELD_SQUARE_EMPTY){
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_MISS;
        opp_guess->result = RESULT_MISS;
    }
    
    
    else if(own_field->grid[opp_guess->row][opp_guess->col] == FIELD_SQUARE_SMALL_BOAT){
        own_field->smallBoatLives--;
        if(own_field->smallBoatLives != SUNK){
            opp_guess->result = RESULT_HIT;
        }
        else{
            opp_guess->result = RESULT_SMALL_BOAT_SUNK;
        }
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_field->grid[opp_guess->row][opp_guess->col] == FIELD_SQUARE_MEDIUM_BOAT){
        own_field->mediumBoatLives--;
        if(own_field->mediumBoatLives != SUNK){
            opp_guess->result = RESULT_HIT;
        }
        else{
            opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
        }
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_field->grid[opp_guess->row][opp_guess->col] == FIELD_SQUARE_LARGE_BOAT){
        own_field->largeBoatLives--;
        if(own_field->largeBoatLives != SUNK){
            opp_guess->result = RESULT_HIT;
        }
        else{
            opp_guess->result = RESULT_LARGE_BOAT_SUNK;
        }
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_field->grid[opp_guess->row][opp_guess->col] == FIELD_SQUARE_HUGE_BOAT){
        own_field->hugeBoatLives--;
        if(own_field->hugeBoatLives != SUNK){
            opp_guess->result = RESULT_HIT;
        }
        else{
            opp_guess->result = RESULT_HUGE_BOAT_SUNK;
        }
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
    }
    
    return previousVal;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess){
    SquareStatus previousVal = opp_field->grid[own_guess->row][own_guess->col];
    
    if(own_guess->result == RESULT_HIT){
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_guess->result == RESULT_MISS){
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
    }
    else if(own_guess->result == RESULT_SMALL_BOAT_SUNK){
        opp_field->smallBoatLives = SUNK;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_guess->result == RESULT_MEDIUM_BOAT_SUNK){
        opp_field->mediumBoatLives = SUNK;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_guess->result == RESULT_LARGE_BOAT_SUNK){
        opp_field->largeBoatLives = SUNK;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    else if(own_guess->result == RESULT_HUGE_BOAT_SUNK){
        opp_field->hugeBoatLives = SUNK;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    return previousVal;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f){
    uint8_t alive = SUNK;
    
    if(f->smallBoatLives != SUNK){
        alive |= smallBoatAlive;
        
    }
    if(f->mediumBoatLives != SUNK){
        alive |= mediumBoatAlive;
        
    }
    if(f->largeBoatLives != SUNK){
        alive |= largeBoatAlive;
        
    }
    if(f->hugeBoatLives != SUNK) {
        alive |= hugeBoatAlive;
        
    }
    return alive;
}


/**
 * This function is responsible for placing all four of the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field *own_field){
    uint8_t result = STANDARD_ERROR;
    uint8_t row;
    uint8_t col;
    BoatDirection dir;
    
    while(result == STANDARD_ERROR){
        row = (rand() % FIELD_ROWS);
        col = (rand() % FIELD_COLS);
        dir = (rand() % randDir);
        result = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_SMALL);
    }
    
    result = STANDARD_ERROR;
    while(result == STANDARD_ERROR){
        row = (rand() % FIELD_ROWS);
        col = (rand() % FIELD_COLS);
        dir = (rand() % randDir);
        result = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_MEDIUM);
    }
    
    result = STANDARD_ERROR;
    while(result == STANDARD_ERROR){
        row = (rand() % FIELD_ROWS);
        col = (rand() % FIELD_COLS);
        dir = (rand() % randDir);
        result = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_LARGE);
    }
    
    result = STANDARD_ERROR;
    while(result == STANDARD_ERROR){
        row = (rand() % FIELD_ROWS);
        col = (rand() % FIELD_COLS);
        dir = (rand() % randDir);
        result = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_HUGE);
    }
    
    own_field->smallBoatLives = smallBoat;
    own_field->mediumBoatLives = mediumBoat;
    own_field->largeBoatLives = largeBoat ;
    own_field->hugeBoatLives = hugeBoat;
    return SUCCESS;
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field *opp_field){
    uint8_t row;
    uint8_t col;
    uint8_t result = FIELD_SQUARE_EMPTY;
    GuessData Guess;
    
    while(result != FIELD_SQUARE_UNKNOWN){
        row = (rand() % FIELD_ROWS);
        col = (rand() % FIELD_COLS);
        result = opp_field->grid[row][col];
    }
    
    Guess.row = row;
    Guess.col = col;
    
    return Guess;
}

/** 
 * For Extra Credit:  Make the two "AI" functions above 
 * smart enough to beat our AI in more than 55% of games.
 */


