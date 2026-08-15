#include "GameSymbols.h"
#include "../ClassiCube/src/Game.h"

static void LoadSymbolsFromGame(void);

const struct IGameComponent GameSymbolsComp = {
	LoadSymbolsFromGame, /* Init */
	NULL, /* Free */
	NULL, /* Reset */
	NULL, /* OnNewMap */
	NULL, /* OnNewMapLoaded */
};

static union {
	void*	  symbol;
	RawSymbol rawSymbol;
} gameSymbols[TOTAL_SYMBOLS];

void* const GetGameSymbol(enum GAMESYMBOL gameSymbol)
{
	return gameSymbols[gameSymbol].symbol;
}

RawSymbol GetGameRawSymbol(enum GAMESYMBOL gameSymbol)
{
	return gameSymbols[gameSymbol].rawSymbol;
}

#define QUOTE(x) #x
#ifdef CC_BUILD_WIN
	#define WIN32_LEAN_AND_MEAN
	#define NOSERVICE
	#define NOMCX
	#define NOIME
	#include <windows.h>
	#define DeclareSymbol(enumName, symbolName) gameSymbols[enumName].rawSymbol = (RawSymbol)GetProcAddress(GetModuleHandleA(NULL), QUOTE(symbolName));
#elif CC_BUILD_POSIX
	#define _GNU_SOURCE
	#include <dlfcn.h>
	#define DeclareSymbol(enumName, symbolName) gameSymbols[enumName].symbol = dlsym(RTLD_DEFAULT, QUOTE(symbolName));
#endif

static void LoadSymbolsFromGame(void) {
	SymbolsDeclList
}