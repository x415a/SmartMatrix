#include "modules.h"
#include "matrix.h"
#include "time.h"
#include "clock.h"
#include "effects.h"


typedef void (*ModuleUpdateCallback)();

ModuleUpdateCallback __MD_LIST[] = {
    updateClock,
    updateEffects,
};
uint32_t __global_md_state = 0;


uint8_t getModulesCount() {
    return sizeof(__MD_LIST) / sizeof(ModuleUpdateCallback);
}


void updateModules() {
    for (uint8_t i = 0; i < getModulesCount(); ++i) {
        if (__global_md_state & (1 << i)) {
            __MD_LIST[i]();
        }
    }
}


void initModules() {
    initTime();
    initMatrix();

    #ifdef DEBUG
        Serial.print("Included modeules: ");
        Serial.println(getModulesCount());
    #endif
}
