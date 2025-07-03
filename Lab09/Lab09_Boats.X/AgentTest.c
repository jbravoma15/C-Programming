//Jorge Bravo Martinez
//Jbravoma
// Agent Test




#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "BOARD.h"
#include <xc.h>
#include <sys/attribs.h>
#include "Agent.h"
#include "Message.h"
#include "Field.h"

#define Bitmax 65535

int main(int argc, char** argv) {
    BOARD_Init();
    uint16_t A = rand() % Bitmax;
    uint16_t B = rand() % Bitmax;

    AgentState State;
    BB_Event event;

    printf("We will now being Agent Test!\n");
    AgentSetState(AGENT_STATE_CHALLENGING);
    State = AgentGetState();

    printf("\n Now Testing AgentGetState and AgentSetState\n");
    printf("\nAgentGetState and AgentgetState Results\n");

    if (State != AGENT_STATE_CHALLENGING) {
        printf("Test Failed\n");
    } else {
        printf("Test Passed\n");
    }

    AgentSetState(AGENT_STATE_DEFENDING);
    State = AgentGetState();
    if (State != AGENT_STATE_DEFENDING) {
        printf("Test Failed\n");
    } else {
        printf("Test Passed\n");
    }
    printf("\nNow Testing AgentInit\n ");
    printf("\nAgentInit Results\n");

    AgentInit();
    State = AgentGetState();
    if (State != AGENT_STATE_START) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");

    }

    printf("\nNow Testing AgentRun Transitions\n");
    printf("\nAgentRun Transitions Results\n");
    AgentInit();
    event.type = BB_EVENT_NO_EVENT;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_START) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }

    event.type = BB_EVENT_START_BUTTON;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_CHALLENGING) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    event.type = BB_EVENT_ACC_RECEIVED;
    event.param0 = B;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_DEFENDING || State != AGENT_STATE_WAITING_TO_SEND) {
        printf("Test #3 Failed\n");
    } else {
        printf("Test #3 Passed\n");
    }
    event.type = BB_EVENT_RESET_BUTTON;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_START) {
        printf("Test #4 Failed\n");
    } else {
        printf("Test #4 Passed\n");
    }
    event.type = BB_EVENT_REV_RECEIVED;
    event.param0 = A;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_END_SCREEN || State != AGENT_STATE_DEFENDING || State != AGENT_STATE_ATTACKING) {
        printf("Test #5 Failed\n");
    } else {
        printf("Test #5 Passed\n");
    }
    event.type = BB_EVENT_CHA_RECEIVED;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_ACCEPTING) {
        printf("Test #6 Failed\n");
    } else {
        printf("Test #6 Passed\n");
    }
    AgentSetState(AGENT_STATE_ATTACKING);
    event.type = BB_EVENT_RES_RECEIVED;
    AgentRun(event);
    if (State != AGENT_STATE_DEFENDING) {
        printf("Test #7 Failed\n");
    } else {
        printf("Test #7 Passed\n");
    }
    event.type = BB_EVENT_SHO_RECEIVED;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_WAITING_TO_SEND) {
        printf("Test #8 Failed\n");
    } else {
        printf("Test #8 Passed\n");
    }
    event.type = BB_EVENT_MESSAGE_SENT;
    AgentRun(event);
    State = AgentGetState();
    if (State != AGENT_STATE_ATTACKING) {
        printf("Test #9 Failed\n");
    } else {
        printf("Test #9 Passed\n");
    }

    printf("\n Now Testing AgentRun Output\n");
    printf("\n AgentRun Output Results");

    GuessData guess;
    Message message;
    event.type = BB_EVENT_ERROR;
    message = AgentRun(event);

    if (message.type != MESSAGE_ERROR) {
        printf("Test #1 Failed\n");
    } else {
        printf("Test #1 Passed\n");
    }
    AgentSetState(AGENT_STATE_START);
    event.type = BB_EVENT_START_BUTTON;
    message = AgentRun(event);
    if (message.type != MESSAGE_CHA) {
        printf("Test #2 Failed\n");
    } else {
        printf("Test #2 Passed\n");
    }
    AgentSetState(AGENT_STATE_START);
    event.type = BB_EVENT_CHA_RECEIVED;
    message = AgentRun(event);
    if (message.type != MESSAGE_ACC) {
        printf("Test #3 Failed\n");
    } else {
        printf("Test #3 Passed\n");
    }
    AgentSetState(AGENT_STATE_CHALLENGING);
    event.type = BB_EVENT_ACC_RECEIVED;
    message = AgentRun(event);
    if (message.type != MESSAGE_REV) {
        printf("Test #4 Failed\n");
    } else {
        printf("Test #4 Passed\n");
    }
    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
    event.type = BB_EVENT_MESSAGE_SENT;
    message = AgentRun(event);
    if (message.type != MESSAGE_SHO) {
        printf("Test #5 Failed\n");
    } else {
        printf("Test #5 Passed\n");
    }
    
    printf("\n End of Test");
    
    return(EXIT_SUCCESS);
    
    
}

