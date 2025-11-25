//---------------------------------------------------------------------
//This file may be regarded as a black box where no change is needed
//---------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------
#include "Common.h"
#include "Logic.h"
#include "GFX.h"
//---------------------------------------------------------------------
class GUI : protected Common {
public:
	//-------------------------------------------------------
    GUI();
	~GUI();
	//-------------------------------------------------------
    void   Init();
    //-------------------------------------------------------
    void   InitAttributes();
    void   CreateSurface();
    void   InitGFX();
	//-------------------------------------------------------
	void   Display();
    void   Idle();
	void   SetView();
	void   SetWindowTitle();
	void   DispInfo();
	//-------------------------------------------------------Keyboard
	void   Keyboard_NormalKeys(unsigned char key, int x, int y);
	//-------------------------------------------------------Mouse
	void   Mouse(int button, int state, int x, int y);
	void   Mouse_Motion(int x, int y);
	void   Mouse_PassiveMotion(int x, int y);
    //-------------------------------------------------------SDL Keyboard
    void   KeyDown_SDL(int key);
    void   KeyUp_SDL(int key);
    //-------------------------------------------------------SDL Mouse
    void   Mouse_SDL(UINT button, UINT state, int x, int y);
    void   MouseMotion_SDL(int x, int y, int xrel, int yrel);
    void   MouseWheel_SDL(int x, int y);
	//-------------------------------------------------------Mouse Aux
private:
	void   MouseAux_GetOpenGL3DPos(int x, int y, char s[2]);
	void   MouseAux_MakeMove(int x, int y);
	//-------------------------------------------------------Game
	void   Game_CopyBoardToGFX(int ply);
	void   Game_Reset();
	void   Game_StepBack();
	void   FindChange(int ply);
	void   Game_Eval_Move();
	//-------------------------------------------------------View
    void   Cam_Init();
	void   Cam();
    //-------------------------------------------------------Main Loop
public:
    void   MainLoop();
	//-------------------------------------------------------
    //-------------------------------------------------------
    //-------------------------------------------------------
    //-------------------------------------------------------
    //-------------------------------------------------------
	int    mW, mH, mW_Last, mH_Last, mWinPosX, mWinPosY;
    //-------------------------------------------------------
private:
    //-------------------------------------------------------
    SDL_Window * mScreen = NULL;
    //-------------------------------------------------------
    double mLookAt[3], mEye[3];
    double mTheta_CamRotation, mV_CamRotation;
	int    mMouseX, mMouseY, mMouseMotionX, mMouseMotionY;
	int    mCamControlModeRevolve;
    bool   mWorldLights;
    int    mXMin, mXMax, mYMin, mYMax;
	int    mCamMode;
    int    mKeyMode;
    double mAuxRot;
    bool   mSearch4NewLookAtPoint;
    double mNearPlane, mFarPlane;
	//-------------------------------------------------------
    GFX    * mGFX;
	//-------------------------------------------------------
    Logic  * mLogic;
	int    mPly;
	bool   mIsMachine[2];
	bool   mPause;
	bool   mEvalAtCurrentFrame;
	int    mGameRandSeedOffset;
	bool   mDispInfo;
	//-------------------------------------------------------
};
//---------------------------------------------------------------------
