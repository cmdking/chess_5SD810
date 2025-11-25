//---------------------------------------------------------------------
//---------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifdef _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/glu.h>
#elif __APPLE__
#define GL_SILENCE_DEPRECATION
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#include <OpenGL/glu.h>
#endif
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include <cmath>
#include <stdio.h>
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#define For(i,N) for (int (i) = 0; (i) < (N); (i)++)
//---------------------------------------------------------------------
//---------------------------------------------------------------------
typedef unsigned int UINT;
typedef unsigned char UCHAR;
//---------------------------------------------------------------------
//---------------------------------------------------------------------
class Common {
protected:
    //------------------------------
    enum {BLACK, WHITE};
    enum {RAND_OFF, RAND_ON};
    enum {DISP_OFF, DISP_ON};
    enum {CHESS, STRATEGO};
    enum {MINIMAX, ALPHABETA, OPTIM};
    enum {SEALED, MOBILE, KNOWN};//ST
    enum {PERF_INFO, IMPERF_INFO};//ST
    enum {ST2_PRINT_NONE, ST2_PRINT_CONCISE, ST2_PRINT_VERBOSE};//ST2
    //------------------------------Core
    static const int
    MAX_PLY     = 4096,
    CORE_ENGINE = OPTIM,
    GAME        = CHESS,
    ST_SYS      = IMPERF_INFO,//ST
    //------------------------------Tests
    TEST1 = false,
    TEST2 = false,
    TEST3 = false,//ST2
    //------------------------------ST2 Print
    ST2_PRINT = ST2_PRINT_NONE,
    ST2_SPECIAL_CASE = false,
    //------------------------------Board Size
    FILES = GAME == CHESS ? 8 : 10,
    RANKS = GAME == CHESS ? 8 : 10,
    //------------------------------
    FILES_1 = FILES - 1,
    RANKS_1 = RANKS - 1,
    DIAGS_1 = FILES < RANKS ? FILES_1 : RANKS_1;
    //------------------------------
};
//---------------------------------------------------------------------
