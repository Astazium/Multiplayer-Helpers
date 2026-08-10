#ifndef GAME_SYMBOLS_H
#define GAME_SYMBOLS_H

#include "../ClassiCube/src/Core.h"
#include "../ClassiCube/src/Game.h"
#include "../ClassiCube/src/Event.h"

#define STRING_REF

struct Bitmap;
struct Stream;
struct ScheduledTask2;

typedef void(*FP_Game_ChangeBlock)(int x, int y, int z, BlockID block);
typedef cc_bool(*FP_Options_GetBool)(const char* key, cc_bool defValue);
typedef cc_result(*FP_Png_Decode)(struct Bitmap* bmp, struct Stream* stream);
/*typedef void(*FP_ScheduledTask2_Add)(struct ScheduledTask2* task);*/
typedef void(*FP_ScheduledTask_Add)(double interval, ScheduledTaskCallback callback);
typedef cc_string(*FP_String_FromReadonly)(STRING_REF const char* buffer);
typedef void(*FP_Mem_Free)(void* mem);
typedef void(*FP_Process_Exit)(cc_result code);
typedef void(*FP_String_AppendUtf16)(cc_string* str, const void* data, int numBytes);

/* Events */
typedef void(*FP_Event_Register)  (struct Event_Void* handlers, void* obj, Event_Void_Callback handler);
typedef void(*FP_Event_Unregister)(struct Event_Void* handlers, void* obj, Event_Void_Callback handler);
/* Convert */
typedef cc_bool(*FP_Convert_ParseBool)(const cc_string* str, cc_bool* value);
typedef cc_bool(*FP_Convert_ParseInt)(const cc_string* str, int* value);
typedef cc_bool(*FP_Convert_ParseFloat)(const cc_string* str, float* value);
/* Math */
typedef float(*FP_Math_SinF)(float x);
typedef float(*FP_Math_CosF)(float x);
/* Chat */
typedef void(*FP_Chat_Add1)(const char* format, const void* a1);
typedef void(*FP_Chat_Add2)(const char* format, const void* a1, const void* a2);
typedef void(*FP_Chat_Add3)(const char* format, const void* a1, const void* a2, const void* a3);
typedef void(*FP_Chat_Add4)(const char* format, const void* a1, const void* a2, const void* a3, const void* a4);

#undef STRING_REF

/*
    DeclareSymbol(enumName, symbolName)
    in code, use enumName!
*/
#define SymbolsDeclList													\
/* Structures */														\
DeclareSymbol(BLOCKS_,				Blocks)								\
DeclareSymbol(ATLAS2D_,				Atlas2D)							\
DeclareSymbol(ENTITIES_,			Entities)							\
DeclareSymbol(WORLD_,				World)								\
DeclareSymbol(SERVER_,				Server)								\
DeclareSymbol(CAMERA_,				Camera)								\
DeclareSymbol(GAME_,				Game)								\
/* Events */															\
DeclareSymbol(INPUTEVENTS_,			InputEvents)						\
DeclareSymbol(TEXTUREEVENTS_,		TextureEvents)						\
DeclareSymbol(EVENT_REGISTER_,		Event_Register)						\
DeclareSymbol(EVENT_UNREGISTER_,	Event_Unregister)					\
/* Chat */																\
DeclareSymbol(CHAT_ADD_,			Chat_Add)							\
DeclareSymbol(CHAT_ADDOF_,			Chat_AddOf)							\
DeclareSymbol(CHAT_ADD1_,			Chat_Add1)							\
DeclareSymbol(CHAT_ADD2_,			Chat_Add2)							\
DeclareSymbol(CHAT_ADD3_,			Chat_Add3)							\
DeclareSymbol(CHAT_ADD4_,			Chat_Add4)							\
/* Strings */															\
DeclareSymbol(STRING_APPENDCONST_,  String_AppendConst)					\
DeclareSymbol(STRING_APPENDUTF16_,  String_AppendUtf16)					\
DeclareSymbol(STRING_FORMAT1_,		String_Format1)						\
DeclareSymbol(STRING_FORMAT4_,		String_Format4)						\
DeclareSymbol(STRING_FORMAT2_,		String_Format2)						\
DeclareSymbol(STRING_FROMREADONLY,	String_FromReadonly)				\
DeclareSymbol(STRING_CASELESSEQUALSCONST_,	String_CaselessEqualsConst)	\
/* Math */																\
DeclareSymbol(MATH_SINF_,           Math_SinF)							\
DeclareSymbol(MATH_COSF_,           Math_CosF)							\
/* Convert */															\
DeclareSymbol(CONVERT_PARSEBOOL_,   Convert_ParseBool)					\
DeclareSymbol(CONVERT_PARSEINT_,	Convert_ParseInt)					\
DeclareSymbol(CONVERT_PARSEFLOAT_,	Convert_ParseFloat)					\
/* Other */																\
DeclareSymbol(COMMANDS_REGISTER_,	Commands_Register)					\
/*DeclareSymbol(SCHEDULEDTASK2_ADD_,ScheduledTask2_Add)*/				\
DeclareSymbol(SCHEDULEDTASK_ADD_,	ScheduledTask_Add)					\
DeclareSymbol(GAME_CHANGEBLOCK_,	Game_ChangeBlock)					\
DeclareSymbol(OPTIONS_GETBOOL_,		Options_GetBool)					\
DeclareSymbol(STREAM_OPENFILE_,		Stream_OpenFile)					\
DeclareSymbol(MEM_FREE_,			Mem_Free)							\
DeclareSymbol(PNG_DECODE_,			Png_Decode)							\
DeclareSymbol(PROCESS_EXIT_,		Process_Exit)

#define DeclareSymbol(enumName, symbolName) enumName,
enum GAMESYMBOL
{
    SymbolsDeclList
    TOTAL_SYMBOLS
};
#undef DeclareSymbol

typedef void (*RawSymbol)(void);

void* const GetGameSymbol(enum GAMESYMBOL gameSymbol);
RawSymbol   GetGameRawSymbol(enum GAMESYMBOL gameSymbol);

/*For performance reasons, don't use this in loops*/
#define GetFP(type, gameSymbol) ((type)GetGameRawSymbol(gameSymbol))

/*For performance reasons, don't use this in loops*/
#define TempVar(type, gameSymbol) ((type)GetGameSymbol(gameSymbol))

#endif