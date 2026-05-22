#include "../ClassiCube/src/Camera.h"
#include "../ClassiCube/src/Commands.h"
#include "../ClassiCube/src/Entity.h"
#include "../ClassiCube/src/Event.h"
#include "../ClassiCube/src/ExtMath.h"
#include "../ClassiCube/src/Input.h"

#include "GameSymbols.h"
#include "Utils.h"

static cc_bool g_wasEnabled = false;
static Vec3(*OldGetPosition)(float t) = NULL;
static struct Camera* ActiveCamera = NULL;

static Vec3 FreeCam_GetPosition(float t);

static void FreeCam_Enable(void) {
    if (ActiveCamera && ActiveCamera->GetPosition == FreeCam_GetPosition)
        ActiveCamera->GetPosition = OldGetPosition;
    ActiveCamera = TempVar(struct _CameraData*, CAMERA_)->Active;
    OldGetPosition = ActiveCamera->GetPosition;
    ActiveCamera->GetPosition = FreeCam_GetPosition;
}

static void FreeCam_Disable(void) {
    if (ActiveCamera && ActiveCamera->GetPosition == FreeCam_GetPosition)
        ActiveCamera->GetPosition = OldGetPosition;
    OldGetPosition = NULL;
    ActiveCamera = NULL;
    g_wasEnabled = false;
}

static void FreeCam_Init(void);
static void FreeCam_Free(void);

const struct IGameComponent FreeCamComp = {
    FreeCam_Init, /* Init */
    FreeCam_Free, /* Free */
    FreeCam_Disable, /* Reset */
    FreeCam_Disable, /* OnNewMap */
    NULL, /* OnNewMapLoaded */
};

static float g_Speed = 19.2f;

static void FreeCam_Execute(const cc_string* args, int argsCount) {
    cc_bool enabled;
    if (argsCount == 0) {
        Chat_AddRaw("&eToo few arguments.");
        return;
    }
    if (!GetFP(FP_Convert_ParseBool, CONVERT_PARSEBOOL_)(args, &enabled)) {
        float speed;
        if (GetFP(FP_Convert_ParseFloat, CONVERT_PARSEFLOAT_)(args, &speed)) {
            g_Speed = speed;
            GetFP(FP_Chat_Add1, CHAT_ADD1_)("&eSpeed changed to: %f2.", &speed);
            return;
        }
        Chat_AddRaw("&eCould not parse value.");
        return;
    }
    if (enabled == g_wasEnabled) {
        Chat_AddRaw("&eValue doesn't change.");
        return;
    }
    g_wasEnabled = enabled;
    if (enabled) {
        FreeCam_Enable();
        Chat_AddRaw("&eFreeCam enabled.");
    } else {
        FreeCam_Disable();
        Chat_AddRaw("&eFreeCam disabled.");
    }
}

static struct ChatCommand FreeCamCmd = {
    "FreeCam", FreeCam_Execute,
    COMMAND_FLAG_UNSPLIT_ARGS,
    {
        "&a/client FreeCam [true/false]",
        "&eTurn FreeCam on/off",
        "&a/client FreeCam [float]",
        "&eChange speed",
    }
};

static cc_bool keyForward;
static cc_bool keyBack;
static cc_bool keyLeft;
static cc_bool keyRight;
static cc_bool keyUp;
static cc_bool keyDown;

static void OnKeyPressed(void* obj, int key, cc_bool repeating, struct InputDevice* device) {
    switch (key) {
    case CCKEY_UP:    keyForward = true; return;
    case CCKEY_DOWN:  keyBack    = true; return;
    case CCKEY_LEFT:  keyLeft    = true; return;
    case CCKEY_RIGHT: keyRight   = true; return;

    case CCKEY_KP1:   keyUp   = true; return;
    case CCKEY_KP0:   keyDown = true; return;
    }
}

static void OnKeyReleased(void* obj, int key, cc_bool repeating, struct InputDevice* device) {
    switch (key) {
    case CCKEY_UP:    keyForward = false; return;
    case CCKEY_DOWN:  keyBack    = false; return;
    case CCKEY_LEFT:  keyLeft    = false; return;
    case CCKEY_RIGHT: keyRight   = false; return;

    case CCKEY_KP1:   keyUp   = false; return;
    case CCKEY_KP0:   keyDown = false; return;
    }
}

static void FreeCam_Free(void) {
    struct _InputEventsList* InputEvents_;
    FP_Event_Unregister Event_Unregister;
    InputEvents_     = GetGameSymbol(INPUTEVENTS_);
    Event_Unregister = GetFP(FP_Event_Unregister, EVENT_UNREGISTER_);
    Event_Unregister((struct Event_Void*)&InputEvents_->Down2, NULL, (Event_Void_Callback)OnKeyPressed);
    Event_Unregister((struct Event_Void*)&InputEvents_->Up2,   NULL, (Event_Void_Callback)OnKeyReleased);
}

static struct Entity* PlayerEntity;
static struct _GameData* Game_;
static Vec3* Camera_CurrentPos;

static FP_Math_SinF Math_SinF_;
static FP_Math_CosF Math_CosF_;

static void FreeCam_Init(void) {
    struct _InputEventsList* InputEvents_;
    FP_Event_Register Event_Register;

    InputEvents_   = GetGameSymbol(INPUTEVENTS_);
    Event_Register = GetFP(FP_Event_Register, EVENT_REGISTER_);

    GetFP(FP_Commands_Register, COMMANDS_REGISTER_)(&FreeCamCmd);
    PlayerEntity = &TempVar(struct _EntitiesData*, ENTITIES_)->CurPlayer->Base;
    Game_   = GetGameSymbol(GAME_);

    Camera_CurrentPos = &TempVar(struct _CameraData*, CAMERA_)->CurrentPos;

    Math_SinF_ = GetFP(FP_Math_SinF, MATH_SINF_);
    Math_CosF_ = GetFP(FP_Math_CosF, MATH_COSF_);

    Event_Register((struct Event_Void*)&InputEvents_->Down2, NULL, (Event_Void_Callback)OnKeyPressed);
    Event_Register((struct Event_Void*)&InputEvents_->Up2,   NULL, (Event_Void_Callback)OnKeyReleased);
}

static Vec3 FreeCam_GetPosition(float t) {
    (void)t;
    static double lastTime;
    double currentTime;
    float delta;

    float speed;
    float yaw;

    Vec3 forward;
    Vec3 right;

    currentTime = Game_->Time;
    delta = (float)(currentTime - lastTime);
    lastTime = currentTime;

    speed = g_Speed * delta;
    yaw = PlayerEntity->Yaw * MATH_DEG2RAD;
    
    forward.x = Math_SinF_(yaw);
    forward.y = 0;
    forward.z = -Math_CosF_(yaw);
    
    right.x = Math_SinF_(yaw + MATH_PI / 2.0f);
    right.y = 0;
    right.z = -Math_CosF_(yaw + MATH_PI / 2.0f);

    if (keyForward) {
        Camera_CurrentPos->x += forward.x * speed;
        Camera_CurrentPos->z += forward.z * speed;
    }

    if (keyBack) {
        Camera_CurrentPos->x -= forward.x * speed;
        Camera_CurrentPos->z -= forward.z * speed;
    }

    if (keyLeft) {
        Camera_CurrentPos->x -= right.x * speed;
        Camera_CurrentPos->z -= right.z * speed;
    }

    if (keyRight) {
        Camera_CurrentPos->x += right.x * speed;
        Camera_CurrentPos->z += right.z * speed;
    }

    if (keyUp) {
        Camera_CurrentPos->y += speed;
    }

    if (keyDown) {
        Camera_CurrentPos->y -= speed;
    }

    return *Camera_CurrentPos;
}
