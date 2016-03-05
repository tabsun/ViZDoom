#include "vizdoom_input.h"
#include "vizdoom_defines.h"
#include "vizdoom_message_queue.h"
#include "vizdoom_shared_memory.h"

#include "d_main.h"
#include "d_net.h"
#include "g_game.h"
#include "d_player.h"
#include "d_event.h"
#include "c_bind.h"
#include "c_console.h"
#include "c_dispatch.h"
#include "r_utility.h"
#include "doomtype.h"

bip::mapped_region *vizdoomInputSMRegion = NULL;
//ViZDoomInputStruct *vizdoomLastInput = NULL;
ViZDoomInputStruct *vizdoomInput = NULL;
bool vizdoomInputInited = false;
int vizdoomLastInputBT[VIZDOOM_BT_SIZE];
int vizdoomLastInputUpdate[VIZDOOM_BT_SIZE];

EXTERN_CVAR (Bool, vizdoom_allow_input);

void ViZDoom_Command(char * cmd){
    //printf("VIZIA CMD %d %s\n", gametic, cmd);
    if(strlen(cmd) >= 1) AddCommandString(cmd);
}

bool ViZDoom_CommmandFilter(const char *cmd){

    if(!vizdoomInputInited || !*vizdoom_allow_input) return true;

    bool action = false;
    int state = 1;

    if (*cmd == '+'){
        action = true;
        state = 1;
    }
    else if(*cmd == '-'){
        action = true;
        state = 0;
    }

    const char* beg;
    if(action) beg = cmd+1;
    else beg = cmd;

    for(int i = 0; i<VIZDOOM_BT_CMD_BT_SIZE; ++i){
        if(strcmp(beg, ViZDoom_BTToCommand(i)) == 0){
            if(!vizdoomInput->BT_AVAILABLE[i]) {
                vizdoomInput->BT[i] = 0;
                return false;
            }
            else{
                vizdoomInput->BT[i] = state;
                vizdoomLastInputUpdate[i] = VIZDOOM_TIME;
            }
        }
    }

    //printf("%d %s\n",gametic, cmd);

    return true;
}

int ViZDoom_AxisFilter(int button, int value){
    if(value != 0 && button >= VIZDOOM_BT_CMD_BT_SIZE && button < VIZDOOM_BT_SIZE){

        if(!vizdoomInput->BT_AVAILABLE[button]) return 0;
        int axis = button - VIZDOOM_BT_CMD_BT_SIZE;
        if(vizdoomInput->BT_MAX_VALUE[axis] != 0){
            int maxValue;
            if(button == VIZDOOM_BT_VIEW_ANGLE || button == VIZDOOM_BT_VIEW_PITCH)
                maxValue = (int)((float)vizdoomInput->BT_MAX_VALUE[axis]/180 * 32768);
            else maxValue = vizdoomInput->BT_MAX_VALUE[axis];

            if((int)labs(value) > (int)labs(maxValue))
                value = value/(int)labs(value) * (int)(labs(maxValue) + 1);
        }
        if(button == VIZDOOM_BT_VIEW_ANGLE || button == VIZDOOM_BT_VIEW_PITCH)
            vizdoomInput->BT[button] = (int)((float)value/32768 * 180);
        else vizdoomInput->BT[button] = value;
        vizdoomLastInputUpdate[button] = VIZDOOM_TIME;
    }
    return value;
}

void ViZDoom_AddAxisBT(int button, int value){
    if(button == VIZDOOM_BT_VIEW_ANGLE || button == VIZDOOM_BT_VIEW_PITCH)
        value = (int)((float)value/180 * 32768);
    value = ViZDoom_AxisFilter(button, value);
    switch(button){
        case VIZDOOM_BT_VIEW_PITCH :
            G_AddViewPitch(value);
            //LocalViewPitch = value;
            break;
        case VIZDOOM_BT_VIEW_ANGLE :
            G_AddViewAngle(value);
            //LocalViewAngle = value;
            break;
        case VIZDOOM_BT_FORWARD_BACKWARD :
            LocalForward = value;
            break;
        case VIZDOOM_BT_LEFT_RIGHT:
            LocalSide = value;
            break;
        case VIZDOOM_BT_UP_DOWN :
            LocalFly = value;
            break;
    }
}

char* ViZDoom_BTToCommand(int button){
    switch(button){
        case VIZDOOM_BT_ATTACK : return strdup("attack");
        case VIZDOOM_BT_USE : return strdup("use");
        case VIZDOOM_BT_JUMP : return strdup("jump");
        case VIZDOOM_BT_CROUCH : return strdup("crouch");
        case VIZDOOM_BT_TURN180 : return strdup("turn180");
        case VIZDOOM_BT_ALTATTACK : return strdup("altattack");
        case VIZDOOM_BT_RELOAD : return strdup("reload");
        case VIZDOOM_BT_ZOOM : return strdup("zoom");

        case VIZDOOM_BT_SPEED : return strdup("speed");
        case VIZDOOM_BT_STRAFE : return strdup("strafe");

        case VIZDOOM_BT_MOVE_RIGHT : return strdup("moveright");
        case VIZDOOM_BT_MOVE_LEFT : return strdup("moveleft");
        case VIZDOOM_BT_MOVE_BACK : return strdup("back");
        case VIZDOOM_BT_MOVE_FORWARD : return strdup("forward");
        case VIZDOOM_BT_TURN_RIGHT : return strdup("right");
        case VIZDOOM_BT_TURN_LEFT : return strdup("left");
        case VIZDOOM_BT_LOOK_UP : return strdup("lookup");
        case VIZDOOM_BT_LOOK_DOWN : return strdup("lookdown");
        case VIZDOOM_BT_MOVE_UP : return strdup("moveup");
        case VIZDOOM_BT_MOVE_DOWN : return strdup("movedown");
        case VIZDOOM_BT_LAND : return strdup("land");

        case VIZDOOM_BT_SELECT_WEAPON1 : return strdup("slot 1");
        case VIZDOOM_BT_SELECT_WEAPON2 : return strdup("slot 2");
        case VIZDOOM_BT_SELECT_WEAPON3 : return strdup("slot 3");
        case VIZDOOM_BT_SELECT_WEAPON4 : return strdup("slot 4");
        case VIZDOOM_BT_SELECT_WEAPON5 : return strdup("slot 5");
        case VIZDOOM_BT_SELECT_WEAPON6 : return strdup("slot 6");
        case VIZDOOM_BT_SELECT_WEAPON7 : return strdup("slot 7");
        case VIZDOOM_BT_SELECT_WEAPON8 : return strdup("slot 8");
        case VIZDOOM_BT_SELECT_WEAPON9 : return strdup("slot 9");
        case VIZDOOM_BT_SELECT_WEAPON0 : return strdup("slot 0");

        case VIZDOOM_BT_SELECT_NEXT_WEAPON : return strdup("weapnext");
        case VIZDOOM_BT_SELECT_PREV_WEAPON : return strdup("weapprev");
        case VIZDOOM_BT_DROP_SELECTED_WEAPON : return strdup("weapdrop");

        case VIZDOOM_BT_ACTIVATE_SELECTED_ITEM : return strdup("invuse");
        case VIZDOOM_BT_SELECT_NEXT_ITEM : return strdup("invnext");
        case VIZDOOM_BT_SELECT_PREV_ITEM : return strdup("invprev");
        case VIZDOOM_BT_DROP_SELECTED_ITEM : return strdup("invdrop");

        case VIZDOOM_BT_VIEW_PITCH :
        case VIZDOOM_BT_VIEW_ANGLE :
        case VIZDOOM_BT_FORWARD_BACKWARD :
        case VIZDOOM_BT_LEFT_RIGHT :
        case VIZDOOM_BT_UP_DOWN :
        default : return strdup("");
    }
}

void ViZDoom_ResetDiscontinuousBT(){

    if(vizdoomLastInputUpdate[VIZDOOM_BT_TURN180] < VIZDOOM_TIME) vizdoomInput->BT[VIZDOOM_BT_TURN180] = 0;
    for(int i = VIZDOOM_BT_LAND; i < VIZDOOM_BT_SIZE; ++i){
        if(vizdoomLastInputUpdate[i] < VIZDOOM_TIME) vizdoomInput->BT[i] = 0;
    }
}

void ViZDoom_AddBTCommand(int button, int state){

    switch(button){
        case VIZDOOM_BT_ATTACK :
        case VIZDOOM_BT_USE :
        case VIZDOOM_BT_JUMP :
        case VIZDOOM_BT_CROUCH :
        case VIZDOOM_BT_ALTATTACK :
        case VIZDOOM_BT_RELOAD :
        case VIZDOOM_BT_ZOOM :
        case VIZDOOM_BT_SPEED :
        case VIZDOOM_BT_STRAFE :
        case VIZDOOM_BT_MOVE_RIGHT :
        case VIZDOOM_BT_MOVE_LEFT :
        case VIZDOOM_BT_MOVE_BACK :
        case VIZDOOM_BT_MOVE_FORWARD :
        case VIZDOOM_BT_TURN_RIGHT :
        case VIZDOOM_BT_TURN_LEFT :
        case VIZDOOM_BT_LOOK_UP :
        case VIZDOOM_BT_LOOK_DOWN :
        case VIZDOOM_BT_MOVE_UP :
        case VIZDOOM_BT_MOVE_DOWN :
            if(state) ViZDoom_Command(strcat(strdup("+"), ViZDoom_BTToCommand(button)));
            else ViZDoom_Command(strcat(strdup("-"), ViZDoom_BTToCommand(button)));
            break;

        case VIZDOOM_BT_TURN180 :
        case VIZDOOM_BT_LAND :
        case VIZDOOM_BT_SELECT_WEAPON1 :
        case VIZDOOM_BT_SELECT_WEAPON2 :
        case VIZDOOM_BT_SELECT_WEAPON3 :
        case VIZDOOM_BT_SELECT_WEAPON4 :
        case VIZDOOM_BT_SELECT_WEAPON5 :
        case VIZDOOM_BT_SELECT_WEAPON6 :
        case VIZDOOM_BT_SELECT_WEAPON7 :
        case VIZDOOM_BT_SELECT_WEAPON8 :
        case VIZDOOM_BT_SELECT_WEAPON9 :
        case VIZDOOM_BT_SELECT_WEAPON0 :
        case VIZDOOM_BT_SELECT_NEXT_WEAPON :
        case VIZDOOM_BT_SELECT_PREV_WEAPON :
        case VIZDOOM_BT_DROP_SELECTED_WEAPON :
        case VIZDOOM_BT_ACTIVATE_SELECTED_ITEM :
        case VIZDOOM_BT_SELECT_NEXT_ITEM :
        case VIZDOOM_BT_SELECT_PREV_ITEM :
        case VIZDOOM_BT_DROP_SELECTED_ITEM :
            if(state) ViZDoom_Command(ViZDoom_BTToCommand(button));
            break;

        case VIZDOOM_BT_VIEW_PITCH :
        case VIZDOOM_BT_VIEW_ANGLE :
        case VIZDOOM_BT_FORWARD_BACKWARD :
        case VIZDOOM_BT_LEFT_RIGHT :
        case VIZDOOM_BT_UP_DOWN :
            if(state != 0) ViZDoom_AddAxisBT(button, state);
            break;
    }
}

void ViZDoom_InputInit() {

    try {
        vizdoomInputSMRegion = new bip::mapped_region(vizdoomSM, bip::read_write, 0, sizeof(ViZDoomInputStruct));
        vizdoomInput = static_cast<ViZDoomInputStruct *>(vizdoomInputSMRegion->get_address());
    }
    catch (bip::interprocess_exception &ex) {
        Printf("ViZDoom_InputInit: Error creating Input SM");
        ViZDoom_MQSend(VIZDOOM_MSG_CODE_DOOM_ERROR);
        exit(1);
    }

    for (int i = 0; i < VIZDOOM_BT_SIZE; ++i) {
        vizdoomInput->BT[i] = 0;
        vizdoomInput->BT_AVAILABLE[i] = true;
        vizdoomLastInputBT[i] = 0;
        vizdoomLastInputUpdate[i] = 0;
    }

    for(int i = 0; i < VIZDOOM_BT_AXIS_BT_SIZE; ++i){
        vizdoomInput->BT_MAX_VALUE[i] = 0;
    }

    LocalForward = 0;
    LocalSide = 0;
    LocalFly = 0;
    LocalViewAngle = 0;
    LocalViewPitch = 0;

    vizdoomInputInited = true;
}

void ViZDoom_InputTic(){

    if(!*vizdoom_allow_input) {
        for (int i = 0; i < VIZDOOM_BT_CMD_BT_SIZE; ++i) {

            if (vizdoomInput->BT_AVAILABLE[i] && vizdoomInput->BT[i] != vizdoomLastInputBT[i]){ //vizdoomLastInput->BT[i]) {
                ViZDoom_AddBTCommand(i, vizdoomInput->BT[i]);
            }
        }

        for (int i = VIZDOOM_BT_CMD_BT_SIZE; i < VIZDOOM_BT_SIZE; ++i) {
            if (vizdoomInput->BT_AVAILABLE[i]) {
                ViZDoom_AddBTCommand(i, vizdoomInput->BT[i]);
            }
        }
    }
    else if(vizdoomLastUpdate == VIZDOOM_TIME){
        ViZDoom_ResetDiscontinuousBT();
        D_ProcessEvents ();
    }

    for (int i = 0; i < VIZDOOM_BT_SIZE; ++i) {
        vizdoomLastInputBT[i] =  vizdoomInput->BT[i];
    }
    //memcpy( vizdoomLastInput, vizdoomInput, sizeof(ViZDoomInputStruct) );

}

void ViZDoom_InputClose(){
    //delete(vizdoomLastInput);
    delete(vizdoomInputSMRegion);
}