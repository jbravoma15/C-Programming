// Jorge Bravo Martinez
// Jbravoma

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Agent.h"
#include "BattleBoats.h"
#include "Ascii.h"
#include "Field.h"
#include "FieldOled.h"
#include "OledDriver.h"
#include "Message.h"
#include "Negotiation.h"
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"
#include "CircularBuffer.h"
#include "BOARD.h"

/**
 * Defines the various states used within the agent state machines. All states should be used
 * within a valid agent implementation. Additionally there is no need for states outside of
 * those defined here.
 */
typedef enum {
    victory,
    defeat,
    cheat
} Ending;

#define SUNK 0
#define CLEAR 0

static AgentState currentState;
static int side;
static Ending end;
static char display[100] = "";
static Field mySide = {};
static Field enemySide = {};
static FieldOledTurn FieldOledside = FIELD_OLED_TURN_NONE;
static GuessData Guess = {};
static uint16_t A;
static uint16_t B;
static uint16_t Hash1;
static uint16_t Hash2;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void) {
    currentState = AGENT_STATE_START;
    side = 0;
    A = (NegotiationData) rand();
    B = rand();
    Hash1 = NegotiationHash(A);
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event) {
    Message returnMessage = {MESSAGE_NONE, 0, 0, 0};
    
    
    AgentInit();
    OledClear(OLED_COLOR_BLACK);
    
    if (event.type == BB_EVENT_RESET_BUTTON) {
        AgentInit();
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("Start \n Ready to Play? Press BTN4 to Challenge \n ");
        OledUpdate();
        return returnMessage;
    }

    OledClear(OLED_COLOR_BLACK);

    if (event.type == BB_EVENT_ERROR) {
        OledDrawString("BUTTON ERROR");
        OledUpdate();
        returnMessage.param0 = CLEAR;
        returnMessage.param1 = CLEAR;
        returnMessage.param2 = CLEAR;
        return returnMessage;
    }

    switch (currentState) {
        case AGENT_STATE_START:
            if (event.type == BB_EVENT_START_BUTTON) {
                //Starting game
                currentState = AGENT_STATE_CHALLENGING;
                returnMessage.type = MESSAGE_CHA;
                A = (NegotiationData) rand();
                Hash1 = NegotiationHash(A);
                returnMessage.param0 = Hash1;
                return returnMessage;
            } else if (BB_EVENT_CHA_RECEIVED) {
                currentState = AGENT_STATE_ACCEPTING;
                Hash1 = event.param0;
                B = (NegotiationData) rand();
                returnMessage.type = MESSAGE_ACC;
                returnMessage.param0 = B;
                return returnMessage;
            }
            if (event.type == BB_EVENT_START_BUTTON) {
                // Setting the Battle Field
                FieldInit(&mySide, &enemySide);
                FieldAIPlaceAllBoats(&enemySide);
                sprintf(display, "Challenge Sent\n %d = A \n &d = HashA", A, Hash1);
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(display);
                OledUpdate();
                return returnMessage;
            } else if (event.type == BB_EVENT_CHA_RECEIVED) {
                //setting the battle field
                FieldInit(&mySide, &enemySide);
                FieldAIPlaceAllBoats(&mySide);
                sprintf(display, "Accepting Challenge\n %d = hashA\n %d = B", A, Hash1);
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(display);
                OledUpdate();
                return returnMessage;
            }
            break;

        case AGENT_STATE_CHALLENGING:
            if (event.type == BB_EVENT_ACC_RECEIVED) {
                B = event.param0;
                returnMessage.type = MESSAGE_REV;
                returnMessage.param0 = A;
                if (NegotiateCoinFlip(A, B) == HEADS) {
                    currentState = AGENT_STATE_WAITING_TO_SEND;
                } else if (NegotiateCoinFlip(A, B) == TAILS) {
                    AGENT_STATE_DEFENDING;
                }
                FieldOledDrawScreen(&mySide, &enemySide, FieldOledside, side);
                return returnMessage;
            }
            break;


        case AGENT_STATE_ACCEPTING:
            if (event.type == BB_EVENT_REV_RECEIVED) {
                A = event.param0;
                if (NegotiationVerify(A, Hash1) != TRUE) {
                    end = cheat;
                    currentState = AGENT_STATE_END_SCREEN;

                } else if (NegotiationVerify(A, Hash1) == TRUE) {
                    if (NegotiateCoinFlip(A, B) == TAILS) {
                        FieldOledDrawScreen(&mySide, &enemySide, FieldOledside, side);
                        currentState = AGENT_STATE_DEFENDING;

                    } else if (NegotiateCoinFlip(A, B) == HEADS) {
                        Guess = FieldAIDecideGuess(&enemySide);
                        returnMessage.type = MESSAGE_SHO;
                        returnMessage.param0 = Guess.col;
                        returnMessage.param1 = Guess.row;
                        currentState = AGENT_STATE_ATTACKING;
                        FieldOledDrawScreen(&mySide, &enemySide, FieldOledside, side);
                        return returnMessage;
                    }
                }
            }
            break;


        case AGENT_STATE_ATTACKING:
            //once result received update their field and check for victory
            if (event.type == BB_EVENT_RES_RECEIVED) {
                Guess.row = event.param0;
                Guess.col = event.param1;
                Guess.result = event.param2;
                FieldUpdateKnowledge(&mySide, &Guess);
                if (FieldGetBoatStates(&mySide) != SUNK) {
                    currentState = AGENT_STATE_DEFENDING;
                    FieldOledside = FIELD_OLED_TURN_THEIRS;
                } else {
                    end = victory;
                    currentState = AGENT_STATE_END_SCREEN;
                }
                FieldOledDrawScreen(&mySide, &enemySide, FieldOledside, side);
            }
            break;


        case AGENT_STATE_DEFENDING:
            //update enemy guess and check for defeat
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                Guess.row = event.param0;
                Guess.col = event.param1;

                //enemy attacking 
                FieldRegisterEnemyAttack(&mySide, &Guess);
                FieldOledside = FIELD_OLED_TURN_MINE;
                returnMessage.type = MESSAGE_RES;
                returnMessage.param0 = Guess.row;
                returnMessage.param1 = Guess.col;
                returnMessage.param2 = Guess.result;

                //check if boats are hit
                if (FieldGetBoatStates(&mySide) != SUNK) {
                    currentState = AGENT_STATE_WAITING_TO_SEND;
                } else {
                    end = defeat;
                    currentState = AGENT_STATE_END_SCREEN;
                }
                FieldOledDrawScreen(&mySide, &enemySide, FieldOledside, side);
                return returnMessage;
            }
            break;
        case AGENT_STATE_WAITING_TO_SEND:
            //sends guess
            if (event.type == BB_EVENT_MESSAGE_SENT) {
                side++;
                Guess = FieldAIDecideGuess(&enemySide);
                returnMessage.type = MESSAGE_SHO;
                returnMessage.param0 = Guess.row;
                returnMessage.param1 = Guess.col;
                currentState = AGENT_STATE_ATTACKING;
                return returnMessage;
            }
            break;


        case AGENT_STATE_END_SCREEN:
            //depending on end print screen
            OledClear(OLED_COLOR_BLACK);
            if (end == victory) {
                OledDrawString("Victory!");
            } else if (end == defeat) {
                OledDrawString("Defeat!");
            } else if (end == cheat) {
                OledDrawString("You Cheated!");
            }
            OledUpdate();
            break;

        default:
            //if get here there's an error
            OledClear(OLED_COLOR_BLACK);
            OledDrawString("ERROR");
            OledUpdate();
            FATAL_ERROR();
    }
    return returnMessage;

}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void) {
    return currentState;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState) {
    currentState = newState;
}
