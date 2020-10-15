#ifndef __GAME_SCENES_H__
#define __GAME_SCENES_H__

#include "cocos2d.h"
#include<string>
#include<vector>
#include "GameWidget.h"
#include "GameAI.h"

using std::string;
using std::vector;
USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
	//MEMBER:
	//Player now_playing;
	vector<Label*> vlabel_PlayingPlayer;
	vector<Sprite*> vGameOverPlate;
	vector<Label*> vGameOverLabel;//[0]:LabelGameOver [1]:LabelWhoWin [2]: LabelStartNew
	vector<Sprite*> vPawnPromotionSprites;//[0]:Plate [1]:Pawn [2]:Rook [3]:Knight [4]:Bishop [5]:Queen
	vector<Label*> vPawnPromotionLabel;
	bool p1_is_white = true;//white is offensive
	bool game_is_over = false;
	bool menuShowing = false;
	bool promotionPlateShowing = false;
	//FUNCTIONS:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void initMainMenu();
	virtual void initTestButtons() {};//test
	virtual void initGameInfo() {};
	virtual void initGameRange() {};
	virtual void refreshGameRange() {};
	virtual void initPlayingPlayerInfo();
	virtual void refreshPlayingPlayerInfo() {};
	virtual void initButtons() {};//»ÚÆå¡¢Í¶½µµÈ
	virtual void initGameAI() {};
	virtual void initListener() {};
	virtual void initGameOverInfo() {};
	virtual void initPawnPromotionInfo() {};
	virtual void initOther() {};
	virtual string player_white() { return string(""); }
	virtual string player_black() { return string(""); }

	//
	//virtual void step(const string &s);//example: 2D:a1d4 3D:

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

class GameScene2D :public GameScene
{
public:
	Chess2D chess;
	index chosen_index = -1;//only used in listener
	bool chosen = false;//only used in listener
	index chosen_index2 = -1;//only used in listener
	ChessPieceMovement(*best_step)(DataChess2D* ch) = nullptr;

	virtual void initGameInfo() {};
	void initGameRange();
	void refreshGameRange();
	virtual void initButtons() {};
	virtual void initGameAI() {};
	virtual void refreshPlayingPlayerInfo();
	virtual void initTestButtons();
	virtual void initListener();
	index positionToIndex(Vec2 v,Vec2 middle);
	bool isLegal(ChessPieceMovement m);
	virtual string player_white() { return string(""); }
	virtual string player_black() { return string(""); }
	virtual void initGameOverInfo();
	virtual void openNewGame();
	void GameOverInfoRiseUpWithString(string str);
	void GameOverInfoMoveDown();
	virtual void initPawnPromotionInfo();
	void PawnPromotionInfoDragDown();
	void PawnPromotionInfoMoveUp();
	void addChildren(Chess2D &ch);
	void addChildren(ChessSet2D& ch);
	CREATE_FUNC(GameScene2D);
};

class GameScene3D :public GameScene
{
public:
	virtual void initGameInfo(){}
	//void initGameRange();
	//void refreshGameRange();
	//virtual void initButtons();
	virtual void initGameAI() {};
	virtual string player_white() { return string(""); }
	virtual string player_black() { return string(""); }
	CREATE_FUNC(GameScene3D);
};

class CPUGame2D :public GameScene2D
{
public:

	virtual void initGameInfo(){}
	virtual string player_white()override { if (p1_is_white)return string("Player"); else return string("Computer"); }
	virtual string player_black()override { if (p1_is_white)return string("Computer"); else return string("Player"); }
	virtual void initButtons();
	virtual void initGameAI() {};
	CREATE_FUNC(CPUGame2D);
};

class CPUGame3D :public GameScene3D
{
public:
	virtual void initGameInfo(){}
	virtual string player_white()override { if (p1_is_white)return string("Player"); else return string("Computer"); }
	virtual string player_black()override { if (p1_is_white)return string("Computer"); else return string("Player"); }
	//virtual void initButtons();
	virtual void initGameAI() {};
	CREATE_FUNC(CPUGame3D);
};

class TwoPlayersGame2D :public GameScene2D
{
public:
	virtual void initGameInfo()override;
	virtual string player_white()override { if (p1_is_white)return string("Player1"); else return string("Player2"); }
	virtual string player_black()override { if (p1_is_white)return string("Player2"); else return string("Player1"); }
	virtual void initButtons();
	virtual void initGameAI() { best_step = &lazy_AI_step; };
	CREATE_FUNC(TwoPlayersGame2D);
};

class TwoComputersGame2D :public GameScene2D {
public:
	ChessPieceMovement(*best_step2)(DataChess2D* ch) = nullptr;
	bool paused = true;
	bool flag = true;

public:
	virtual void initGameInfo()override;
	virtual string player_white()override { if (p1_is_white)return string("Computer1"); else return string("Computer2"); }
	virtual string player_black()override { if (p1_is_white)return string("Computer2"); else return string("Computer1"); }
	virtual void initButtons();
	virtual void initGameAI() { best_step = &alpha_beta_AI_step; best_step2 = &alpha_beta_AI_step; };
	virtual void initListener();
	virtual void openNewGame();
	CREATE_FUNC(TwoComputersGame2D);
};

class TwoPlayersGame3D :public GameScene3D
{
public:
	virtual void initGameInfo()override;
	virtual string player_white()override { if (p1_is_white)return string("Player1"); else return string("Player2"); }
	virtual string player_black()override { if (p1_is_white)return string("Player2"); else return string("Player1"); }
	//virtual void initButtons();
	virtual void initGameAI() {};
	CREATE_FUNC(TwoPlayersGame3D);
};

class EasyGame2D :public CPUGame2D
{
public:
	virtual void initGameInfo()override;
	virtual void initGameAI() { best_step = &random_AI_step; };
	CREATE_FUNC(EasyGame2D);
};

class MediumGame2D :public CPUGame2D
{
public:
	virtual void initGameInfo()override;
	virtual void initGameAI() { best_step = &gluttonous_AI_step; };
	CREATE_FUNC(MediumGame2D);
};

class HardGame2D :public CPUGame2D
{
public:
	virtual void initGameInfo()override;
	virtual void initGameAI() { best_step = &alpha_beta_AI_step; };
	CREATE_FUNC(HardGame2D);
};

class EasyGame3D :public CPUGame3D
{
public:
	virtual void initGameInfo()override;
	CREATE_FUNC(EasyGame3D);
};

class MediumGame3D :public CPUGame3D
{
public:
	virtual void initGameInfo()override;
	CREATE_FUNC(MediumGame3D);
};

class HardGame3D :public CPUGame3D
{
public:
	virtual void initGameInfo()override;
	CREATE_FUNC(HardGame3D);
};


#endif