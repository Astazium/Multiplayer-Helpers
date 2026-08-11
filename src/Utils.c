#include "../ClassiCube/src/Chat.h"
#include "../ClassiCube/src/Commands.h"
#include "../ClassiCube/src/String_.h"
#include "../ClassiCube/src/World.h"
#include "GameSymbols.h"
#include "Utils.h"

static void Utils_Init(void);

const struct IGameComponent UtilsComp = {
    Utils_Init, /* Init */
    NULL, /* Free */
    NULL, /* Reset */
    NULL, /* OnNewMap */
    NULL, /* OnNewMapLoaded */
};

static void MaxCoordsCommand_Execute(const cc_string* args, int argsCount) {
    struct _WorldData* World_;
    World_ = GetGameSymbol(WORLD_);
    GetFP(FP_Chat_Add3, CHAT_ADD3_)("&eMax X: %i, Max Y: %i, Max Z: %i", &World_->MaxX, &World_->MaxY, &World_->MaxZ);
}

static struct ChatCommand MaxCoordsCommand = {
    "MaxCoords", MaxCoordsCommand_Execute,
    COMMAND_FLAG_UNSPLIT_ARGS,
    {
        "&a/client maxcoords",
        "&eDisplays max coords of the world",
    }
};

static void Utils_Init(void) {
    GetFP(FP_Commands_Register, COMMANDS_REGISTER_)(&MaxCoordsCommand);
}

void Chat_AddRaw(const char* raw) {
    cc_string str = GetFP(FP_String_FromReadonly, STRING_FROMREADONLY)(raw);
    GetFP(FP_Chat_AddOf, CHAT_ADDOF_)(&str, MSG_TYPE_NORMAL);
}

void Time_FormatSeconds(cc_string* str, float totalSeconds) {
    FP_String_Format2 String_Format2_;
    int hours, minutes, seconds, milliseconds;
    int total_ms;

    total_ms = (int)(totalSeconds * 1000.0f + 0.5f);
    milliseconds = total_ms % 1000;
    seconds  = (total_ms / 1000) % 60;
    minutes  = (total_ms / 60000) % 60;
    hours    = (total_ms / 3600000);

    String_Format2_ = GetFP(FP_String_Format2, STRING_FORMAT2_);

    if (hours) {
        String_Format2_(str, "%ih %im", &hours, &minutes);
        return;
    }
    if (minutes) {
        String_Format2_(str, "%im %is", &minutes, &seconds);
        return;
    }
    String_Format2_(str, "%is %ims", &seconds, &milliseconds);
}