//---------------------------------------------------------------------
//---------------------------------------------------------------------
//This file may be regarded as a black box where no change is needed
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "GUI.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GUI::GUI(){
    //----------------------------
	mGFX   = new GFX();
	mLogic = new Logic();
    //----------------------------
	For (i,3) mEye[i] = 0.;
	mLookAt[0] = mLookAt[2] = 0.; mLookAt[1] = -1.;
    //----------------------------
    mW_Last = mW = 1280; mH_Last = mH = 800;
    mWinPosX = 890; mWinPosY = 390;
    mMouseX = mMouseY = 0;
    mMouseMotionX = mMouseMotionY = 0;
	mNearPlane = 1.;
    mFarPlane = 50.;
	mDispInfo = false;
    //-------------------------------------------
	mGameRandSeedOffset = 0;
    //-------------------------------------------
	mIsMachine[BLACK] = true;
	mIsMachine[WHITE] = false;
	mPause = false;
    //-------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GUI::~GUI(){delete mGFX; delete mLogic;}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Init
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Init(){
    //---------------------------------------
	if (GAME == CHESS) mLogic->InitChess();
	else mLogic->ST_Init();//STRATEGO
    Cam_Init();
	Game_Reset();
    //-----------------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                Init
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::InitAttributes(){
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);//24-bit Depth Buffer
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);//MSAA
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);//MSAA
    SDL_GL_SetSwapInterval(1);//Enable Vsync
    //----------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::CreateSurface(){
    SDL_Init(SDL_INIT_EVERYTHING);
    InitAttributes();//Context Attr
    //----------------------------------If High-DPI mode is off
    //SDL_DisplayMode dispMode;
    //SDL_GetCurrentDisplayMode(0, &dispMode);
    //sW = dispMode.w; sH = dispMode.h;
    //----------------------------------Create window
    mScreen = SDL_CreateWindow("GFX",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               mW, mH,
                               SDL_WINDOW_OPENGL);
    //SDL_SetWindowTitle(mScreen,"SGE");//#SGE
#ifdef __APPLE__
    //SDL_SetWindowFullscreen(mScreen,SDL_WINDOW_FULLSCREEN);
#endif
    SDL_GL_CreateContext(mScreen);
    //----------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::InitGFX(){
    //---------------------------------------
    //---------------------------------------
    CreateSurface();
    //---------------------------------------
    //---------------------------------------
    Init();
    //---------------------------------------
    //---------------------------------------
    glClearColor(.8f, .8f, 1., 1.);
    //---------------------------------------
    //---------------------------------------
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //---------------------------------------
    //---------------------------------------
    glEnable(GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //---------------------------------------
    //---------------------------------------
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //---------------------------------------
    //---------------------------------------
    GLfloat posChess[]    = {1.0, 1.0, 1.0, .0};
    GLfloat posStratego[] = {5.0, 1.0, 5.0, .0};
    GLfloat ambient[]     = {0.3, 0.3, 0.3, 1.0};
    GLfloat diffuse[]     = {0.8, 0.8, 0.8, 1.0};
    GLfloat front_mat_shininess[] = {60.0};
    GLfloat front_mat_specular[]  = {0.2, 0.2, 0.2, 1.0};
    GLfloat front_mat_diffuse[]   = {0.5, 0.5, 0.28, 1.0};
    GLfloat back_mat_shininess[]  = {60.0};
    GLfloat back_mat_specular[]   = {0.5, 0.5, 0.2, 1.0};
    GLfloat back_mat_diffuse[]    = {1.0, 0.9, 0.2, 1.0};
    //---------------------------------------
    //---------------------------------------
    if (GAME == CHESS) glLightfv(GL_LIGHT0, GL_POSITION, posChess);
    else glLightfv(GL_LIGHT0, GL_POSITION, posStratego);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);
    //---------------------------------------
    //---------------------------------------
    const GLfloat lmodel_twoside[] = {GL_TRUE};
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    //---------------------------------------
    //---------------------------------------
    glEnable(GL_LIGHT0);
    //---------------------------------------
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //---------------------------------------
    glEnable(GL_MULTISAMPLE_ARB);
    //---------------------------------------
    //---------------------------------------
    SetView();
    //---------------------------------------
    //---------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Basics
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Idle(){
    //glutPostRedisplay();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Display(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	Cam();
	
	bool isWhite = !bool(mPly % 2);
	if (!mPause && mIsMachine[isWhite]){
		if (mEvalAtCurrentFrame){
			Game_Eval_Move();
			mEvalAtCurrentFrame = false;
		}
		else mEvalAtCurrentFrame = true;
	}
	
    mGFX->DrawScene();
	
	if (mDispInfo) DispInfo();
		
	if (GAME == STRATEGO){
		FindChange(mPly-1);
		FindChange(mPly);
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline void GUI::SetView(){
    glViewport(0,0,mW,mH);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double ratio = double(mW)/double(mH);
	gluPerspective(45.,ratio,mNearPlane,mFarPlane);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::SetWindowTitle(){
	if (mIsMachine[WHITE]){
		if (mIsMachine[BLACK])
            SDL_SetWindowTitle(mScreen,"Computer - Computer");
		else SDL_SetWindowTitle(mScreen,"Computer - Human");
	}
	else {
		if (mIsMachine[BLACK]) SDL_SetWindowTitle(mScreen,"Human - Computer");
		else SDL_SetWindowTitle(mScreen,"Human - Human");
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::DispInfo(){//STRATEGO v0.0.1 SDL-2
	//-------------------------
	if (GAME == CHESS) return;
    //-------------------------
	const char LABELS[][20] = {
		"FLAG",
		"SPY",
		"SCOUT", 
		"MINER",
		"SERGEANT",
		"LIEUTENANT",
		"CAPTAIN",
		"MAJOR",
		"COLONEL",
		"GENERAL",
		"MARSHAL",
		"TRAP",
	};
    //-------------------------CountPieces
	char pieceFreq[12][2];
	For (i,12) For (j,2) pieceFreq[i][j] = 0;
	For (x,FILES) For (y,RANKS){
		char p = mLogic->mGamePos[mPly].sq[x][y];
		if (p >= 'a' && p <= 'z') pieceFreq[p-'a'][BLACK]++;
		else if (p >= 'A' && p <= 'Z') pieceFreq[p-'A'][WHITE]++;
	}
    //-------------------------
    const char COLOR_NAME[][32] = {
        "ROYAL BLUE",
        "LIGHT STEEL BLUE",
        "WHITE",
        "LIGHT SKY BLUE",
        "TEAL",
        "SPRING GREEN",
        "GREEN YELLOW",
        "YELLOW",
        "ORANGE",
        "ORANGE RED",
        "DARK RED",
        "MIDNIGHT BLUE"};
    //-------------------------
    printf("\nStratego Info Disp\n");
	For (i,12){
        printf("%10s %d %d %s\n",
               LABELS[i],
               pieceFreq[i][WHITE],
               pieceFreq[i][BLACK],
               COLOR_NAME[i]);
	}
    printf("\n");
    //-------------------------
    mDispInfo = false;
    //-------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                Keys
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::KeyDown_SDL(int key){
    switch (key) {
        case SDLK_ESCAPE : exit(0);
        case '-' : Game_StepBack(); break;
        case ' ' : if (mPause) Game_Eval_Move(); break;
        case '&' :
            if (GAME == STRATEGO){
                if (mPly > 0) break;
                mLogic->ST_NewPreSet_GamePos_Start(true);
                Game_CopyBoardToGFX(mPly);
            }
            break;
        case '/' :
            if (GAME == STRATEGO){
                if (mPly > 0) break;
                mLogic->ST_NewPreSet_GamePos_Start(false);
                Game_CopyBoardToGFX(mPly);
            }
            break;
        case '(' :
            if (GAME == STRATEGO){
                if (mPly > 0) break;
                mLogic->ST_NewRand_GamePos_Start(true);
                Game_CopyBoardToGFX(mPly);
            }
            break;
        case ')' :
            if (GAME == STRATEGO){
                if (mPly > 0) break;
                mLogic->ST_NewRand_GamePos_Start(false);
                Game_CopyBoardToGFX(mPly);
            }
            break;
        case '0' : Game_Reset(); break;
        case '1' :
            mIsMachine[WHITE] = false; mIsMachine[BLACK] = true;
            SetWindowTitle();
            //break;
        case '!' ://STRATEGO
            mGFX->mST_IsConcealed[WHITE] = false;
            mGFX->mST_IsConcealed[BLACK] = true;
            break;
        case '2' :
            mIsMachine[WHITE] = true; mIsMachine[BLACK] = false;
            SetWindowTitle();
            //break;
        case '"' ://STRATEGO
            mGFX->mST_IsConcealed[WHITE] = true;
            mGFX->mST_IsConcealed[BLACK] = false;
            break;
        case '3' :
            mIsMachine[WHITE] = mIsMachine[BLACK] = false;
            SetWindowTitle();
            //break;
        case '#' ://STRATEGO
            mGFX->mST_IsConcealed[WHITE] = false;
            mGFX->mST_IsConcealed[BLACK] = false;
            break;
        case '4' :
            mIsMachine[WHITE] = mIsMachine[BLACK] = true;
            SetWindowTitle();
            mGFX->mST_IsConcealed[WHITE] = false;//STRATEGO
            mGFX->mST_IsConcealed[BLACK] = false;
            break;
        case '5' ://STRATEGO
            mGFX->mST_IsConcealed[WHITE] = true;
            mGFX->mST_IsConcealed[BLACK] = true;
            break;
        case 'h' :
            mGFX->mMarkSqs = !mGFX->mMarkSqs;
            break;
        //case 'i' : mDispInfo = !mDispInfo; break;//STRATEGO
        case 'i' : mDispInfo = true; break;//v0.0.1 SDL-2
        case 'p' : mPause = !mPause; break;
        case 'r' : mLogic->Print_Level(mPly); break;
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::KeyUp_SDL(int key){
    //--------------------------------------------
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                Mouse
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Mouse_SDL(UINT button, UINT state, int x, int y){
	//--------------------------------------------
	if (state == SDL_PRESSED && button == SDL_BUTTON_LEFT) {
		mCamControlModeRevolve = false;
		MouseAux_MakeMove(x, y);
	}
	//--------------------------------------------
	if (state == SDL_RELEASED && button == SDL_BUTTON_RIGHT) {
		mV_CamRotation += x - mMouseX;
		mTheta_CamRotation += y - mMouseY;
		mCamControlModeRevolve = false;
	}
	//--------------------------------------------     
	if (state == SDL_PRESSED && button == SDL_BUTTON_RIGHT) {
		mMouseX = x; mMouseY = y;
		mMouseMotionX = x; mMouseMotionY = y;
		mCamControlModeRevolve = true;
	}
	//--------------------------------------------
	//For use of mLogic->mPromotionFlags, please check out:
	//https://wiki.libsdl.org/SDL2/SDL_GetModState
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MouseMotion_SDL(int x, int y, int xrel, int yrel){
    //--------------------------------------------
    if (mCamControlModeRevolve){
        mMouseMotionX = x; mMouseMotionY = y;}
    //--------------------------------------------
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MouseWheel_SDL(int x, int y){
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Mouse Aux
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MouseAux_GetOpenGL3DPos(int x, int y, char s[2]){
	
	GLint       vport[4];
	GLdouble    px, py, pz, mview[16], proj[16];
	GLfloat     winX, winY, winZ;
    
	glGetDoublev(GL_MODELVIEW_MATRIX, mview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, vport);
    
	winX = (GLfloat)x; int iwinY = winY = (GLfloat)(vport[3] - y);
	glReadPixels(x, iwinY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, mview, proj, vport, &px, &py, &pz);
    
	if (winZ > .9999f){s[0] = s[1] = 0; return;}
	//----------------------------------------
	s[0] = char (floor(pz + .5 * double(FILES)));
	if (s[0] < 0) s[0] = 0; else if (s[0] > FILES_1) s[0] = FILES_1;
	s[1] = char (floor(px + .5 * double(RANKS)));
	if (s[1] < 0) s[1] = 0; else if (s[1] > RANKS_1) s[1] = RANKS_1;
	//----------------------------------------
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MouseAux_MakeMove(int x, int y){
	
	//Basics
	if (mGFX->mGameOver || mPly >= MAX_PLY-1) return;
	mLogic->mRandSeed = (unsigned int)(mPly + mGameRandSeedOffset);
	int w = mLogic->mIsWhite = !bool(mPly%2);
	
	//Get suggested piece position from mouse input
	char selSq[2]; MouseAux_GetOpenGL3DPos(x,y,selSq);
	
	//Is square occupied by this player
	bool selStartSq = mLogic->SqBelongsToPlayer(selSq[0], selSq[1],
												mLogic->mIsWhite,
												mPly);
	
	//--------------------------------------------------------
	// If selStartSq =>
	// the selected square selSq[...] is a start square
	//--------------------------------------------------------
	
	if (selStartSq){
		
		//Logic: Manual swap piece positions at start in STRATEGO 
		if (mPly < 2 && GAME == STRATEGO &&
			mGFX->mSelectedMove[w][0] > -1){
			char s0 = mGFX->mSelectedMove[w][0];
			char s1 = mGFX->mSelectedMove[w][1];
			char s2 = selSq[0];
			char s3 = selSq[1];
			char piece  = mLogic->mGamePos[mPly].sq[s0][s1];
			mLogic->mGamePos[mPly].sq[s0][s1] =
			mLogic->mGamePos[mPly].sq[s2][s3];
			mLogic->mGamePos[mPly].sq[s2][s3] = piece;
			mGFX->mSelectedMove[w][0] = mGFX->mSelectedMove[w][1] = -1;
			Game_CopyBoardToGFX(mPly);
			return;
		}
		
		//GFX: Highlight start square (current move)
		For (i,2) mGFX->mSelectedMove[w][i] = selSq[i];
		
		//GFX: Reset destination square (current move)
		mGFX->mSelectedMove[w][2] = -1;
	}
	
	//--------------------------------------------------------
	// Else => selected square is not a start square.
	// If we already have a start square, which in that case
	// is stored in mGFX->mSelectedMove[w][...] =>
	// selectes square selSq[...] is the destination square
	//--------------------------------------------------------
	
	else if (mGFX->mSelectedMove[w][0] > -1){//If we expect dest sq
		
		//Build complete move string
		char s[4];
		s[0] = mGFX->mSelectedMove[w][0];
		s[1] = mGFX->mSelectedMove[w][1];
		s[2] = selSq[0]; s[3] = selSq[1];
		
		//Is move OK? If TRUE: execute move
		if (mLogic->IsMoveOK_ManualPlay(s, mPly)){
			
			//Logic: Logical update of game position (= board)
			mLogic->UpdateGamePos(0, mPly+1);//Child Index 0
			mLogic->GoalFunc(false, RAND_OFF, mPly+1);
			
			//Logic: Is opponent´s king in check?
			mLogic->mIsWhite = 1-w;
			mLogic->mMove[mPly+1].isInCheck =
			mLogic->King_IsInCheck(mPly+1);
			
			//GFX: Mark square if opponent´s king is in Check
			if (mLogic->mMove[mPly+1].isInCheck){
				mGFX->mCheckSqPos[mLogic->mIsWhite][0] =
				mLogic->mKingPos[0];
				mGFX->mCheckSqPos[mLogic->mIsWhite][1] =
				mLogic->mKingPos[1];
			}
			else mGFX->mCheckSqPos[mLogic->mIsWhite][0] = -1;
			
			//GFX: Reset highlights for previous move
			mGFX->mSelectedMove[1-w][0] =
			mGFX->mSelectedMove[1-w][2] = -1;
			
			//GFX: Highlight destination square (current move)
			mGFX->mSelectedMove[w][2] = selSq[0];
			mGFX->mSelectedMove[w][3] = selSq[1];
			
			//GFX: Graphics update of game position (= board)
			Game_CopyBoardToGFX(mPly+1);
			
			//-----------------------------------------Logic
			
			//Logic: If opponent cannot move in next PLY => Game Over
			if (GAME == CHESS){
				if (mLogic->FindChildren(mPly+1) == 0){
					if (mLogic->mMove[mPly+1].isInCheck)
                        SDL_SetWindowTitle(mScreen,"Check Mate!");
					else SDL_SetWindowTitle(mScreen,"Stalemate!");
					mGFX->mGameOver = true;
					mPly++;
					return;
				}
			}
			else {
				if (mLogic->ST_IsFlagCaptured(bool(1-w), mPly+1)){
                    SDL_SetWindowTitle(mScreen,"Flag Captured!");
					mGFX->mGameOver = true;
				}
				else if (mLogic->FindChildren(mPly+1) == 0){
                    SDL_SetWindowTitle(mScreen,"Stalemate!");
					mGFX->mGameOver = true;
				}
				if (mGFX->mGameOver == true){
					mGFX->mST_IsConcealed[WHITE] = false;
					mGFX->mST_IsConcealed[BLACK] = false;
					mPly++;
					return;
				}
			}
			
			mPly++;//Complete move
			
			//-----------------------------------------GFX
			
			//GFX: Unless check mate, unmark check for this king,
			//since king is not allowed to move into a check position
			mGFX->mCheckSqPos[w][0] = -1;
			
		}
		//--------------------------------
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                               Game
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_CopyBoardToGFX(int ply){
	For (x,FILES) For (y,RANKS){
		mGFX->mGamePosGFX[x][y][0] = mLogic->mGamePos[ply].sq[x][y];
		if (GAME == STRATEGO) mGFX->mGamePosGFX[x][y][1] =
			mLogic->mGamePosStatus[ply].sq[x][y];
	}
	return;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_Reset(){
	mGFX->mGameOver = false;
	Game_CopyBoardToGFX(0);
	mPly = 0;
	For (i,2) For (j,4) mGFX->mSelectedMove[i][j] = -1;
	mGFX->mCheckSqPos[0][0] =
	mGFX->mCheckSqPos[1][0] = -1;
	mEvalAtCurrentFrame = false;
	//-----------------------------------------
	SetWindowTitle();
	//-----------------------------------------
	mGameRandSeedOffset++;
	//-----------------------------------------STRATEGO
	if (GAME == CHESS) return;
	For (i,2) For (j,2) mGFX->mLastCaptured[i][j] = '*';
	mLogic->mST2_VirtualPlay  = false;
	mLogic->mMove[0].children = 0;
	//-----------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_StepBack(){
	mGFX->mGameOver = false;
	if (mPly < 2) return;
	mPly -= 2;
	Game_CopyBoardToGFX(mPly);
	For (i,2){
		mGFX->mCheckSqPos[i][0]   =
		mGFX->mSelectedMove[i][0] = mGFX->mSelectedMove[i][2] = -1;
	}
	//-----------------------------------------
	SetWindowTitle();
	//-----------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::FindChange(int ply){
	//---------------------------------------------
	if (ply < 1) return;
	//---------------------------------------------
	//At this moment
	int w = !bool(ply%2);
	int selectedChild = mLogic->mMove[ply-1].selectedChild;
	char m[4];
	//Get move coords
	For (i,4) m[i] = mLogic->mMove[ply-1].child[selectedChild][i];
	//Pieces in above coords before move
	char p0 = mLogic->mGamePos[ply-1].sq[m[0]][m[1]];
	char p1 = mLogic->mGamePos[ply-1].sq[m[2]][m[3]];
	//--------------------------------------------------
	//Piece in above coords after move
	char pFinal = mLogic->mGamePos[ply].sq[m[2]][m[3]];
	//--------------------------------
	if (p1 == '*'){//No capture
		mGFX->mLastCaptured[1-w][0] = '*';
		mGFX->mLastCaptured[1-w][1] = '*';
	}
	else if (pFinal == '*'){//Both captured
		mGFX->mLastCaptured[1-w][0] = p0;
		mGFX->mLastCaptured[1-w][1] = p1;
	}
	else if (pFinal > 'Z'){//White captured
		mGFX->mLastCaptured[1-w][0] = '*';
		mGFX->mLastCaptured[1-w][1] = w ? p1 : p0;
	}
	else {//Black captured
		mGFX->mLastCaptured[1-w][0] = w ? p0 : p1;
		mGFX->mLastCaptured[1-w][1] = '*';
	}
	//---------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_Eval_Move(){
	
	//-----------------------------------------Logic

	//Logic: Basics
	if (mGFX->mGameOver || mPly >= MAX_PLY-1) return;
	mLogic->mRandSeed = (unsigned int)(mPly + mGameRandSeedOffset);
	int w = mLogic->mIsWhite = !bool(mPly%2);
	
	//Logic: Find best move
	mLogic->FindBestMove(w, mPly);
	
	//Logic: Update of game position (= board)
	mLogic->UpdateGamePos(mLogic->mBestChildIdx, mPly+1);
	mLogic->GoalFunc(false, RAND_OFF, mPly+1);
	
	//Logic: Is opponent´s king in check?
	mLogic->mIsWhite = 1-w;
	mLogic->mMove[mPly+1].isInCheck = mLogic->King_IsInCheck(mPly+1);
	
	//-----------------------------------------GFX
	
	//GFX: Mark square if opponent´s king is in Check
	if (mLogic->mMove[mPly+1].isInCheck)
		For (i,2) mGFX->mCheckSqPos[1-w][i] = mLogic->mKingPos[i];
	else mGFX->mCheckSqPos[1-w][0] = -1;
	
	//GFX: Reset highlights for previous move
	mGFX->mSelectedMove[1-w][0] = mGFX->mSelectedMove[1-w][2] = -1;
	
	//GFX: Highlight current move
	For (i,4) mGFX->mSelectedMove[w][i] =
		mLogic->mMove[mPly].child[mLogic->mBestChildIdx][i];
	
	//GFX: Graphics update of game position (= board)
	Game_CopyBoardToGFX(mPly+1);
	
	//-----------------------------------------Logic
	
	//Logic: If opponent cannot move in next PLY => Game Over
	if (GAME == CHESS){
		if (mLogic->FindChildren(mPly+1) == 0){
			if (mLogic->mMove[mPly+1].isInCheck)
                SDL_SetWindowTitle(mScreen,"Check Mate!");
			else SDL_SetWindowTitle(mScreen,"Stalemate!");
			mGFX->mGameOver = true;
			mPly++;
			return;
		}
	}
	else {//STRATEGO
		if (mLogic->ST_IsFlagCaptured(bool(1-w), mPly+1)){
            SDL_SetWindowTitle(mScreen,"Flag Captured!");
			mGFX->mGameOver = true;
		}
		else if (mLogic->FindChildren(mPly+1) == 0){
            SDL_SetWindowTitle(mScreen,"Stalemate!");
			mGFX->mGameOver = true;
		}
		if (mGFX->mGameOver == true){
			mGFX->mST_IsConcealed[WHITE] = false;
			mGFX->mST_IsConcealed[BLACK] = false;
			mPly++;
			return;
		}
	}
	
	mPly++;//Logic: Complete move
	
	//-----------------------------------------GFX
	
	//GFX: Unless check mate, unmark check for this king,
	//since king is not allowed to move into a check position
	mGFX->mCheckSqPos[w][0] = -1;

}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Cam
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Cam_Init(){
    mCamControlModeRevolve = false;
    mTheta_CamRotation     = 280.;
    mV_CamRotation         = 2700.;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Cam(){
	const double PI = 3.14159265;
    const double C = -0.1*PI/180., D = - PI, E = -1./200.;
	const int MAXSIZE = FILES > RANKS ? FILES_1 : RANKS_1;
    const double R1 = 4. + 1.25 * double(MAXSIZE);
	double R[3], N[3], dx = 0., dy = 0., v, theta;
	
    if (mCamControlModeRevolve){
        dx = double(mMouseMotionX - mMouseX);
        dy = double(mMouseMotionY - mMouseY);
    }
    
    v = C * (mV_CamRotation + dx);
    theta = D / (1.0 + exp(E * (mTheta_CamRotation + dy)));
	double s = sin(theta);
    N[0] =   s * sin(v);//X <=>  y
    N[1] = - cos(theta);//Y <=> -z
    N[2] =   s * cos(v);//Z <=>  x
    For (i,3) R[i] = R1 * N[i];
    
    For (i,3) mEye[i] = mLookAt[i] + R[i];
    
    glLoadIdentity();
    gluLookAt(mEye[0], mEye[1], mEye[2],
			  mLookAt[0], mLookAt[1], mLookAt[2], 0., 1., 0.);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                            SDL Main Loop
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MainLoop(){
    SDL_Event event;
    bool quit = false;
    int key;
    while (!quit) {
        //-----------------------------------------
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                    //---------------------------------
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    Mouse_SDL(event.button.button,
                              event.button.state,
                              event.button.x,
                              event.button.y);
                    break;
                case SDL_MOUSEMOTION:
                    MouseMotion_SDL(event.motion.x,
                                    event.motion.y,
                                    event.motion.xrel,
                                    event.motion.yrel);
                    break;
                case SDL_MOUSEWHEEL:
                    MouseWheel_SDL(event.wheel.x,
                                   event.wheel.y);
                    break;
                    //---------------------------------
                case SDL_KEYDOWN:
                    if (!event.key.repeat){
                        key = event.key.keysym.sym;
                        if (key == SDLK_ESCAPE){quit = true; break;}
                        KeyDown_SDL(key);
                    }
                    break;
                case SDL_KEYUP:
                    key = event.key.keysym.sym;
                    KeyUp_SDL(key);
                    break;
                    //---------------------------------
                case SDL_QUIT: quit = true; break;
                default: break;
                    //---------------------------------
            }
        }
        //-----------------------------------------
        Display();
        SDL_GL_SwapWindow(mScreen);
        SDL_Delay(3);
        //-----------------------------------------
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------







