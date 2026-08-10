#include "../ClassiCube/src/Commands.h"
#include "../ClassiCube/src/Entity.h"
#include "../ClassiCube/src/Game.h"
#include "../ClassiCube/src/Chat.h"

#include "GameSymbols.h"
#include "Utils.h"

static void AntiAfk_Init(void);
static void AntiAfk_Reset(void);

const struct IGameComponent AntiAfkComp = {
    AntiAfk_Init, /* Init */
    NULL, /* Free */
    AntiAfk_Reset, /* Reset */
    NULL, /* OnNewMap */
    AntiAfk_Reset, /* OnNewMapLoaded */
};

static cc_bool g_Enabled;
static float   g_Interval = 1.0f;
static double  g_Accumulator = 0.0;

static void AntiAfk_Reset(void) {
    if (g_Enabled)
        g_Enabled = false;
}

static void AntiAfkCommand_Execute(const cc_string* args, int argsCount) {
    cc_bool enabled;
    if (argsCount == 0) {
        Chat_AddRaw("&eToo few arguments.");
        return;
    }
    if (!GetFP(FP_Convert_ParseBool, CONVERT_PARSEBOOL_)(args, &enabled)) {
        float interval;
        if (GetFP(FP_Convert_ParseFloat, CONVERT_PARSEFLOAT_)(args, &interval)) {
            char msgBuf[256]; 
            cc_string msgStr;
            if (interval < 0.2f) {
                Chat_AddRaw("&eInterval is too small.");
                return;
            }
            String_InitArray(msgStr, msgBuf);
            GetFP(FP_String_AppendConst, STRING_APPENDCONST_)(&msgStr, "&eInterval updated to ");
            Time_FormatSeconds(&msgStr, interval);
            g_Interval = interval;
            g_Accumulator = 0.0;
            GetFP(FP_Chat_Add, CHAT_ADD_)(&msgStr);
            return;
        }
        Chat_AddRaw("&eCould not parse value.");
        return;
    }
    if (g_Enabled == enabled) {
        Chat_AddRaw("&eValue doesn't change.");
        return;
    }
    g_Enabled = enabled;
    if (enabled) {
        Chat_AddRaw("&eAntiAFK enabled.");
    } else {
        g_Accumulator = 0.0;
        Chat_AddRaw("&eAntiAFK disabled.");
    }
}

static struct ChatCommand AntiAfkCommand = {
    "AntiAFK", AntiAfkCommand_Execute,
    COMMAND_FLAG_UNSPLIT_ARGS,
    {
        "&a/client AntiAFK [true/false or interval in seconds]",
        "&eRotates player with specified interval in seconds"
    }
};

static struct Entity* PlayerEntity;

static void AntiAfk_Task(struct ScheduledTask* task) {
    if (!g_Enabled) return;
    g_Accumulator += task->interval;
    if (g_Accumulator < g_Interval) return;

    g_Accumulator = 0.0;
    struct LocationUpdate update;
    update.flags = LU_HAS_YAW;
    update.yaw   = PlayerEntity->Yaw + 5.0f;
    PlayerEntity->VTABLE->SetLocation(PlayerEntity, &update);
}

static void AntiAfk_Init(void) {
    PlayerEntity = &TempVar(struct _EntitiesData*, ENTITIES_)->CurPlayer->Base;
    GetFP(FP_ScheduledTask_Add, SCHEDULEDTASK_ADD_)(GAME_DEF_TICKS, AntiAfk_Task);
    GetFP(FP_Commands_Register, COMMANDS_REGISTER_)(&AntiAfkCommand);
}