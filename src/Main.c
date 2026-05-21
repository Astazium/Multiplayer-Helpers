#include "../ClassiCube/src/Game.h"
#include "../ClassiCube/src/PluginAPI.h"

#define CompsDeclList           \
DeclareComp(GameSymbolsComp)    \
DeclareComp(AntiAfkComp)        \
DeclareComp(ArtBuilderComp)     \
DeclareComp(FreeCamComp)

#define DeclareComp(name) extern const struct IGameComponent name;
CompsDeclList
#undef DeclareComp

#define DeclareComp(name) &name,
static const struct IGameComponent* const comps[] = {
    CompsDeclList
    NULL
};
#undef DeclareComp

#define RegisterCompsEvent(callback)    \
unsigned comp = 0;                      \
while (comps[comp]) {                   \
    if (comps[comp]->callback)          \
        comps[comp]->callback();        \
    ++comp;                             \
}

static void Main_Init(void)           { RegisterCompsEvent(Init) }
static void Main_Free(void)           { RegisterCompsEvent(Free) }
static void Main_Reset(void)          { RegisterCompsEvent(Reset) }
static void Main_OnNewMap(void)       { RegisterCompsEvent(OnNewMap) }
static void Main_OnNewMapLoaded(void) { RegisterCompsEvent(OnNewMapLoaded) }

#undef RegisterCompsEvent
#undef CompsDeclList

PLUGIN_EXPORT int Plugin_ApiVersion = 1;
PLUGIN_EXPORT struct IGameComponent Plugin_Component = { 
    Main_Init, /* Init */
    Main_Free, /* Free */
    Main_Reset, /* Reset */
    Main_OnNewMap, /* OnNewMap */
    Main_OnNewMapLoaded /* OnNewMapLoaded */
};
