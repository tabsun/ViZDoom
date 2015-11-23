#include "vizia_shared_memory.h"

#include "doomdef.h"
#include "doomstat.h"
#include "v_video.h"

bip::shared_memory_object viziaSM;
size_t viziaSMSize;
char * viziaSMName;

void Vizia_SMInit(const char * id){

    viziaSMName = strcat(strdup(VIZIA_SM_NAME_BASE), id);

    bip::shared_memory_object::remove(viziaSMName);
    viziaSM = bip::shared_memory_object(bip::open_or_create, viziaSMName, bip::read_write);

    viziaSMSize = sizeof(ViziaInputStruct) + sizeof(ViziaGameVarsStruct) + (sizeof(BYTE) * screen->GetWidth() * screen->GetHeight() * 4);
    viziaSM.truncate(viziaSMSize);

    printf ("Vizia_SMInit: Size: %zu\n", viziaSMSize);
}

size_t Vizia_SMGetInputRegionBeginning(){
    return 0;
}

size_t Vizia_SMGetGameVarsRegionBeginning(){
    return sizeof(ViziaInputStruct);
}

size_t Vizia_SMGetScreenRegionBeginning(){
    return sizeof(ViziaInputStruct) + sizeof(ViziaGameVarsStruct);
}

void Vizia_SMClose(){
    bip::shared_memory_object::remove(viziaSMName);
}