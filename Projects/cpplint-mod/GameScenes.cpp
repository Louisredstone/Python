#include "GameScenes.h"
#include "SimpleAudioEngine.h"
#include "GameWidget.h"
#include<string>

USING_NS_CC;
using std::string;
Scene* GameScene::createScene()
{
	return GameScene::create();
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in  front of filenames in GameScene.cpp\n");
}
//on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	initMainMenu();
	//initTestButtons();
	initGameInfo();
	initGameRange();
	initPlayingPlayerInfo();
	initButtons();
	initListener();
	initGameAI();
	initGameOverInfo();
	initPawnPromotionInfo();
	initOther();
	return true;
}

void GameScene::initMainMenu() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto outside = Vec2(origin.x + visibleSize.width / 2, origin.y - visibleSize.height / 2);
	auto picture_menu_background = Sprite::create("menuBackground.png");
	picture_menu_background->setPosition(outside);
	auto label_resume = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 24);
	label_resume->setPosition(outside + Vec2(0, 2 * label_resume->getContentSize().height));
	auto label_back_to_menu = Label::createWithTTF("Back To Menu", "fonts/Marker Felt.ttf", 24);
	label_back_to_menu->setPosition(outside + Vec2(0, -2 * label_back_to_menu->getContentSize().height));
	//auto picture_menu = Sprite::create("menu_button.png");
	auto picture_menu = Sprite::create("ShortButton.png");
	picture_menu->setPosition(Vec2(\
		visibleSize.width + origin.x - picture_menu->getContentSize().width / 2, \
		visibleSize.height + origin.y - picture_menu->getContentSize().height / 2));
	auto label_menu = Label::createWithTTF("Menu", "fonts/Marker Felt.ttf", 24);
	label_menu->setPosition(picture_menu->getPosition());
	this->addChild(picture_menu);
	this->addChild(label_menu);
	this->addChild(picture_menu_background, 255);
	this->addChild(label_resume, 255);
	this->addChild(label_back_to_menu, 255);
	auto listener_resume = EventListenerTouchOneByOne::create();
	listener_resume->setSwallowTouches(true);
	listener_resume->onTouchBegan = [](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	listener_resume->onTouchEnded = [=](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			picture_menu_background->runAction(MoveTo::create(0.2, outside));
			label_resume->runAction(MoveTo::create(0.2, outside + Vec2(0, label_resume->getContentSize().height)));
			label_back_to_menu->runAction(MoveTo::create(0.2, outside - Vec2(0, label_back_to_menu->getContentSize().height)));
			menuShowing = false;
		}
	};
	auto listener_back_to_menu = EventListenerTouchOneByOne::create();
	listener_back_to_menu->setSwallowTouches(true);
	listener_back_to_menu->onTouchBegan = [](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	listener_back_to_menu->onTouchEnded = [=](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			Director::getInstance()->popScene();
		}
	};
	auto listener_menu = EventListenerTouchOneByOne::create();
	listener_menu->setSwallowTouches(true);
	listener_menu->onTouchBegan = [](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	listener_menu->onTouchEnded = [=](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			picture_menu_background->runAction(MoveTo::create(0.2, middle));
			label_resume->runAction(MoveTo::create(0.2, middle + Vec2(0, label_resume->getContentSize().height)));
			label_back_to_menu->runAction(MoveTo::create(0.2, middle - Vec2(0, label_back_to_menu->getContentSize().height)));
			menuShowing = true;
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_resume, label_resume);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back_to_menu, label_back_to_menu);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_menu, label_menu);
}

void GameScene::initPlayingPlayerInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_PlayingPlayer = Label::createWithTTF("Playing: White (Player1)", "fonts/Marker Felt.ttf", 24);
	label_PlayingPlayer->setPosition(Vec2(origin.x + 2 * visibleSize.width / 3, origin.y + visibleSize.height - label_PlayingPlayer->getContentSize().height / 2));
	vlabel_PlayingPlayer.push_back(label_PlayingPlayer);
	this->addChild(label_PlayingPlayer);
}

void GameScene2D::refreshPlayingPlayerInfo() {
	auto label_PlayingPlayer = vlabel_PlayingPlayer[0];
	string result("Playing: ");
	if (chess.data.nowPlaying == Player::white) {
		result += "White (";
		result += player_white();
		result += ")";
	}
	else {
		result += "Black (";
		result += player_black();
		result += ")";
	}
	label_PlayingPlayer->setString(result);
}

void GameScene2D::initGameRange() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	chess.setPosition(middle);
	this->addChildren(chess);
}

void GameScene2D::refreshGameRange() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	chess.setPosition(middle);
}

void GameScene2D::initTestButtons() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_white_take_a_step = Label::createWithTTF("White Take A Step", "fonts/Marker Felt.ttf", 24);
	label_white_take_a_step->setPosition(Vec2(origin.x + label_white_take_a_step->getContentSize().width / 2, origin.y + 3 * visibleSize.height / 8));
	auto label_black_take_a_step = Label::createWithTTF("Black Take A Step", "fonts/Marker Felt.ttf", 24);
	label_black_take_a_step->setPosition(Vec2(origin.x + label_black_take_a_step->getContentSize().width / 2, origin.y + 5 * visibleSize.height / 8));
	this->addChild(label_black_take_a_step);
	this->addChild(label_white_take_a_step);
	auto listener_white_take_a_step = EventListenerTouchOneByOne::create();
	listener_white_take_a_step->onTouchBegan = [](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	listener_white_take_a_step->onTouchEnded = [=](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			if (chess.data.nowPlaying == Player::white) {
				chess.data.nowPlaying = Player::black;
				refreshPlayingPlayerInfo();
			}
		}
	};
	auto listener_black_take_a_step = EventListenerTouchOneByOne::create();
	listener_black_take_a_step->onTouchBegan = [](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	listener_black_take_a_step->onTouchEnded = [=](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			if (chess.data.nowPlaying == Player::black) {
				chess.data.nowPlaying = Player::white;
				refreshPlayingPlayerInfo();
			}
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_white_take_a_step, label_white_take_a_step);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_black_take_a_step, label_black_take_a_step);
}

void GameScene2D::initListener() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto from_click = EventListenerTouchOneByOne::create();
	from_click->onTouchBegan = [this,middle](Touch* t, Event* e) {
		if (game_is_over)return false;
		if (chosen)return false;
		if (chosen_index != -1)return false;
		if (menuShowing)return false;
		if (promotionPlateShowing)return false;
		auto s = e->getCurrentTarget();
		auto p = t->getLocation();
		if (s->getBoundingBox().containsPoint(p)) {
			if (positionToIndex(p, middle) != -1) {
				chosen_index = positionToIndex(p, middle);
				return true;
			}
			else return false;
		}
		return false;
	};
	from_click->onTouchEnded = [this,middle](Touch* t, Event* e) {
		if (chosen)return;
		float up = 15;
		auto s = e->getCurrentTarget();
		auto p = t->getLocation();
		index i = positionToIndex(p, middle);
		if (i == -1) {
			chosen = false;
			chosen_index = -1;
			return;
		}
		if (s->getBoundingBox().containsPoint(p)) {
			if (i == chosen_index ) {
				if (chess.data.nowPlaying == Player::white && (player_white() != "Computer")) {
					if (chess.data.chessboard.grids[i]->isEmpty()) {
						chosen = false;
						chosen_index = -1;
						return;
					}
					auto chosen_piece = chess.data.chessboard.grids[i]->getConnectedChessPiece();
					if (!chosen_piece->isWhite()) {
						chosen = false;
						chosen_index = -1;
						return;
					}
					//if efficient
					auto chosen_sprite_piece = chess.getPieceBySerialNumber(i);
					if (chosen_sprite_piece != nullptr) {
						chess.getPieceBySerialNumber(i)->runAction(MoveBy::create(0.1, Vec2(0, up)));
						chosen = true;
					}
					return;
					//
				}
				else if (chess.data.nowPlaying == Player::black && (player_black() != "Computer")) {
					if (chess.data.chessboard.grids[i]->isEmpty()) {
						chosen = false;
						chosen_index = -1;
						return;
					}
					auto chosen_piece = chess.data.chessboard.grids[i]->getConnectedChessPiece();
					if (chosen_piece->isWhite()) {
						chosen = false;
						chosen_index = -1;
						return;
					}
					//if efficient
					chess.getPieceBySerialNumber(i)->runAction(MoveBy::create(0.1, Vec2(0, up)));
					chosen = true;
					return;
					//
				}
				chosen = false;
				chosen_index = -1;
				return;
			}
			else {
				chosen = false;
				chosen_index = -1;
				return;
			}
		}
	};
	auto to_click = EventListenerTouchOneByOne::create();
	to_click->onTouchBegan = [this,middle](Touch* t, Event* e) {
		if (game_is_over)return false;
		if (!chosen)return false;
		if (chosen_index == -1)return false;
		if (menuShowing)return false;
		if (promotionPlateShowing)return false;
		auto p = t->getLocation();
		index i = positionToIndex(p, middle);
		chosen_index2 = i;
		return true;
	};
	to_click->onTouchEnded = [this,middle](Touch* t, Event* e) {
		if (!chosen)return;
		if (chosen_index == -1)return;
		float up = 15;
		auto s = e->getCurrentTarget();
		auto p = t->getLocation();
		index i = positionToIndex(p, middle);
		auto chosen_piece = chess.data.chessboard.grids[chosen_index]->getConnectedChessPiece();
		if (chosen_index2 == -1) {
			chess.getPieceBySerialNumber(chosen_index)->runAction(MoveBy::create(0.1, Vec2(0, -up)));
		}
		else {
			auto movement=ChessPieceMovement(chosen_piece->getPieceType(), chosen_index, chosen_index2,
				(chess.data.chessboard.grids[chosen_index2]->isEmpty() ? PieceType::NONE : chess.data.chessboard.grids[chosen_index2]->getConnectedChessPiece()->getPieceType()),
				!chosen_piece->hasMoved());
			if (chess.isLegal(movement)) {
				//if Effecient
				if (movement.pieceType == PieceType::Pawn && ((movement.to / 8 == 0) || (movement.to / 8 == 7))) {//Pawn's promotion
					chess.data.movementHistory.push_back(movement);
					PawnPromotionInfoDragDown();
				}
				else {
					chess.step(movement);
					refreshPlayingPlayerInfo();
					if (chess.isCheckmate()) {
						game_is_over = true;
						string winner;
						if (chess.data.nowPlaying == Player::black) {
							winner += "White (";
							winner += player_white();
							winner += ")";
						}
						else {
							winner += "Black (";
							winner += player_black();
							winner += ")";
						}
						winner += " Wins !";
						GameOverInfoRiseUpWithString(winner);
					}
					if (chess.isStalemate()) {
						game_is_over = true;
						GameOverInfoRiseUpWithString("Stalemate!");
					}
				}
			}
			else {
				chess.getPieceBySerialNumber(chosen_index)->runAction(MoveBy::create(0.1, Vec2(0, -up)));
			}
		}
		chosen = false;
		chosen_index = -1;
		chosen_index2 = -1;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(from_click, chess.board[0]);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(to_click, chess.board[0]);

	schedule([this](float f) {
		if (game_is_over)return;
		if (menuShowing)return;
		if (promotionPlateShowing)return;
		if (!(chess.isDone() && ((chess.data.nowPlaying == white ? player_white() : player_black()) == "Computer")))return;
		auto movement = (*best_step)(&chess.data);
		if (chess.isLegal(movement)) {//if it's two players mode, best_step() will return an illegal movement, and still let human take a step.
			chess.step(movement);
			refreshPlayingPlayerInfo();
			if (movement.eatType == PieceType::King) {
				game_is_over = true;
				string winner;
				if (chess.data.nowPlaying == Player::black) {
					winner += "White (";
					winner += player_white();
					winner += ")";
				}
				else {
					winner += "Black (";
					winner += player_black();
					winner += ")";
				}
				winner += " Win !";
				GameOverInfoRiseUpWithString(winner);
			}
		}
		}, 0.1f,string("StopAlwaysAI"));
}

void GameScene2D::initGameOverInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto outside = Vec2(origin.x + visibleSize.width / 2, origin.y - visibleSize.height / 2);
	auto game_over_plate = Sprite::create("GameOverPlate.png");
	game_over_plate->setPosition(outside);
	auto label_game_over = Label::createWithTTF("Game Over!", "fonts/Marker Felt.ttf", 48);
	label_game_over->setPosition(outside + Vec2(0, game_over_plate->getContentSize().height / 3));
	auto label_who_win = Label::createWithTTF("wins!", "fonts/Marker Felt.ttf", 36);
	label_who_win->setPosition(outside);
	auto label_new = Label::createWithTTF("Start A New Game (Same Difficulty)", "fonts/Marker Felt.ttf", 24);
	label_new->setPosition(outside + Vec2(0, -game_over_plate->getContentSize().height / 6));
	vGameOverPlate.push_back(game_over_plate);
	vGameOverLabel.push_back(label_game_over);
	vGameOverLabel.push_back(label_who_win);
	vGameOverLabel.push_back(label_new);
	auto listenerStartNew = EventListenerTouchOneByOne::create();
	listenerStartNew->onTouchBegan = [](Touch* t, Event* e) {
		auto s = e->getCurrentTarget();
		auto p = t->getLocation();
		if (s->getBoundingBox().containsPoint(p)) {
			return true;
		}
		return false;
	};
	listenerStartNew->onTouchEnded = [this](Touch* t, Event* e) {
		openNewGame();
		GameOverInfoMoveDown();
	};
	this->addChild(game_over_plate, 5);
	this->addChild(label_game_over, 5);
	this->addChild(label_who_win, 5);
	this->addChild(label_new, 5);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerStartNew, label_new);
}

void GameScene2D::openNewGame()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	game_is_over = false;
	chess.initial();
	chess.setPosition(middle);
	refreshGameRange();
	refreshPlayingPlayerInfo();
}

void GameScene2D::GameOverInfoMoveDown() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto outside = Vec2(origin.x + visibleSize.width / 2, origin.y - visibleSize.height / 2);
	auto game_over_plate = vGameOverPlate[0];
	game_over_plate->runAction(MoveTo::create(0.2, outside));
	vGameOverLabel[0]->runAction(MoveTo::create(0.2, outside));
	vGameOverLabel[1]->runAction(MoveTo::create(0.2, outside + Vec2(0, game_over_plate->getContentSize().height / 3)));
	vGameOverLabel[2]->runAction(MoveTo::create(0.2, outside + Vec2(0, -game_over_plate->getContentSize().height / 6)));
}
void GameScene2D::GameOverInfoRiseUpWithString(string str) {
	auto label_who_win = vGameOverLabel[1];
	label_who_win->setString(str);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto game_over_plate = vGameOverPlate[0];
	game_over_plate->runAction(MoveTo::create(0.2, middle));
	vGameOverLabel[0]->runAction(MoveTo::create(0.2, middle + Vec2(0, game_over_plate->getContentSize().height / 3)));
	vGameOverLabel[1]->runAction(MoveTo::create(0.2, middle));
	vGameOverLabel[2]->runAction(MoveTo::create(0.2, middle + Vec2(0, -game_over_plate->getContentSize().height / 6)));
}

void GameScene2D::initPawnPromotionInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto outside_up = Vec2(origin.x + visibleSize.width / 2, origin.y - visibleSize.height / 2);
	auto pawn_promotion_plate = Sprite::create("PawnPromotionPlate.png");
	pawn_promotion_plate->setPosition(outside_up);
	addChild(pawn_promotion_plate, 4);
	vPawnPromotionSprites.push_back(pawn_promotion_plate);
	float f = 74;
	auto pawn_promotion_rook = Sprite::create("WhiteRook.png");
	auto pawn_promotion_knight = Sprite::create("WhiteKnight.png");
	auto pawn_promotion_bishop = Sprite::create("WhiteBishop.png");
	auto pawn_promotion_queen = Sprite::create("WhiteQueen.png");
	pawn_promotion_rook->setPosition(outside_up + Vec2(-1.8 * f, 0));
	pawn_promotion_knight->setPosition(outside_up + Vec2(-0.6 * f, 0));
	pawn_promotion_bishop->setPosition(outside_up + Vec2(0.6 * f, 0));
	pawn_promotion_queen->setPosition(outside_up + Vec2(1.8 * f, 0));
	auto listener_rook = EventListenerTouchOneByOne::create();
	auto listener_knight = EventListenerTouchOneByOne::create();
	auto listener_bishop = EventListenerTouchOneByOne::create();
	auto listener_queen = EventListenerTouchOneByOne::create();
	listener_rook->setSwallowTouches(true);
	listener_knight->setSwallowTouches(true);
	listener_bishop->setSwallowTouches(true);
	listener_queen->setSwallowTouches(true);
	listener_rook->onTouchBegan = [](Touch* t, Event* e) {if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true; return false; };
	listener_knight->onTouchBegan = [](Touch* t, Event* e) {if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true; return false; };
	listener_bishop->onTouchBegan = [](Touch* t, Event* e) {if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true; return false; };
	listener_queen->onTouchBegan = [](Touch* t, Event* e) {if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true; return false; };
	listener_rook->onTouchEnded = [this](Touch* t, Event* e) {
		auto movement = chess.data.movementHistory[chess.data.movementHistory.size() - 1];
		chess.data.movementHistory.pop_back();
		movement.promotion = PieceType::Rook;
		chess.data.movementHistory.push_back(movement);
		PawnPromotionInfoMoveUp();
	};
	listener_knight->onTouchEnded = [this](Touch* t, Event* e) {
		auto movement = chess.data.movementHistory[chess.data.movementHistory.size() - 1];
		chess.data.movementHistory.pop_back();
		movement.promotion = PieceType::Knight;
		chess.data.movementHistory.push_back(movement);
		PawnPromotionInfoMoveUp();
	};
	listener_bishop->onTouchEnded = [this](Touch* t, Event* e) {
		auto movement = chess.data.movementHistory[chess.data.movementHistory.size() - 1];
		chess.data.movementHistory.pop_back();
		movement.promotion = PieceType::Bishop;
		chess.data.movementHistory.push_back(movement);
		PawnPromotionInfoMoveUp();
	};
	listener_queen->onTouchEnded = [this](Touch* t, Event* e) {
		auto movement = chess.data.movementHistory[chess.data.movementHistory.size() - 1];
		chess.data.movementHistory.pop_back();
		movement.promotion = PieceType::Queen;
		chess.data.movementHistory.push_back(movement);
		PawnPromotionInfoMoveUp();
	};
	addChild(pawn_promotion_rook,4);
	addChild(pawn_promotion_knight,4);
	addChild(pawn_promotion_bishop,4);
	addChild(pawn_promotion_queen,4);
	vPawnPromotionSprites.push_back(pawn_promotion_rook);
	vPawnPromotionSprites.push_back(pawn_promotion_knight);
	vPawnPromotionSprites.push_back(pawn_promotion_bishop);
	vPawnPromotionSprites.push_back(pawn_promotion_queen);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_rook, pawn_promotion_rook);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_knight, pawn_promotion_knight);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_bishop, pawn_promotion_bishop);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_queen, pawn_promotion_queen);
	auto pawn_promotion_label = Label::createWithTTF("Choose A Type For Pawn To Promote", "fonts/Marker Felt.ttf", 24);
	pawn_promotion_label->setPosition(outside_up + Vec2(0, vPawnPromotionSprites[0]->getContentSize().height / 3));
	vPawnPromotionLabel.push_back(pawn_promotion_label);
	addChild(pawn_promotion_label,4);
}

void GameScene2D::PawnPromotionInfoDragDown() {
	promotionPlateShowing = true;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	float f = 74;
	vPawnPromotionLabel[0]->runAction(MoveTo::create(0.2, middle + Vec2(0, vPawnPromotionSprites[0]->getContentSize().height / 3)));
	vPawnPromotionSprites[0]->runAction(MoveTo::create(0.2, middle));
	vPawnPromotionSprites[1]->runAction(MoveTo::create(0.2, middle + Vec2(-1.8 * f, 0)));
	vPawnPromotionSprites[2]->runAction(MoveTo::create(0.2, middle + Vec2(-0.6 * f, 0)));
	vPawnPromotionSprites[3]->runAction(MoveTo::create(0.2, middle + Vec2(0.6 * f, 0)));
	vPawnPromotionSprites[4]->runAction(MoveTo::create(0.2, middle + Vec2(1.8 * f, 0)));
}
void GameScene2D::PawnPromotionInfoMoveUp() {//also take the step!!
	promotionPlateShowing = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	auto outside_up = Vec2(origin.x + visibleSize.width / 2, origin.y - visibleSize.height / 2);
	float f = 74;
	vPawnPromotionLabel[0]->runAction(MoveTo::create(0.2, outside_up + Vec2(0, vPawnPromotionSprites[0]->getContentSize().height / 3)));
	vPawnPromotionSprites[0]->runAction(MoveTo::create(0.2, outside_up));
	vPawnPromotionSprites[1]->runAction(MoveTo::create(0.2, outside_up + Vec2(-1.8 * f, 0)));
	vPawnPromotionSprites[2]->runAction(MoveTo::create(0.2, outside_up + Vec2(-0.6 * f, 0)));
	vPawnPromotionSprites[3]->runAction(MoveTo::create(0.2, outside_up + Vec2(0.6 * f, 0)));
	vPawnPromotionSprites[4]->runAction(MoveTo::create(0.2, outside_up + Vec2(1.8 * f, 0)));
	chess.step(chess.data.movementHistory[chess.data.movementHistory.size()-1]);
	refreshPlayingPlayerInfo();
	if (chess.data.movementHistory[chess.data.movementHistory.size() - 1].eatType == PieceType::King) {
		game_is_over = true;
		string winner;
		if (chess.data.nowPlaying == Player::black) {
			winner += "White (";
			winner += player_white();
			winner += ")";
		}
		else {
			winner += "Black (";
			winner += player_black();
			winner += ")";
		}
		winner += " Wins !";
		GameOverInfoRiseUpWithString(winner);
	}
}

index GameScene2D::positionToIndex(Vec2 v,Vec2 middle) {
	float f = 74;
	Vec2 left_upper_corner = middle + Vec2(-4 * f, 4 * f);
	Vec2 right_lower_corner = middle + Vec2(4 * f, -4 * f);
	if (v.x < left_upper_corner.x)return -1;
	if (v.x > right_lower_corner.x)return -1;
	if (v.y < right_lower_corner.y)return -1;
	if (v.y > left_upper_corner.y)return -1;
	int r = int((left_upper_corner.y - v.y) / f);
	int c = int((v.x - left_upper_corner.x) / f);
	return r * 8 + c;
}

void GameScene2D::addChildren(Chess2D &ch)
{
	this->addChild(ch.board[0],0);
	this->addChildren(ch.chessSetWhite);
	this->addChildren(ch.chessSetBlack);
}

void GameScene2D::addChildren(ChessSet2D& ch) 
{
	for (auto p : ch) {
		this->addChild(p, 2);
	}
}

void CPUGame2D::initButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto regretButton = Sprite::create("LongButton.png");
	regretButton->setPosition(Vec2(origin.x + visibleSize.width - regretButton->getContentSize().width / 2, origin.y + regretButton->getContentSize().height / 2));
	auto regretLabel = Label::createWithTTF("Regret", "fonts/Marker Felt.ttf", 24);
	regretLabel->setPosition(regretButton->getPosition());
	auto regretListener = EventListenerTouchOneByOne::create();
	regretListener->onTouchBegan = [this](Touch* t, Event* e) {
		if (menuShowing)return false;
		if (promotionPlateShowing)return false;
		if ((chess.data.nowPlaying == white ? player_white() : player_black()) == "Computer")return false;
		if (!chess.isDone())return false;
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	regretListener->onTouchEnded = [this](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) chess.reverseTwoSteps();
		if (game_is_over) {
			game_is_over = false;
			GameOverInfoMoveDown();
		}
	};

	this->addChild(regretButton);
	this->addChild(regretLabel);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(regretListener, regretButton);
}

//void GameScene2D::refreshGameRange() {
//
//}
//
//void GameScene3D::initGameRange() {
//
//}
//
//void GameScene3D::refreshGameRange() {
//
//}

void TwoPlayersGame2D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Two Players    Dimension: 2D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void TwoPlayersGame2D::initButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto regretButton = Sprite::create("LongButton.png");
	regretButton->setPosition(Vec2(origin.x + visibleSize.width - regretButton->getContentSize().width / 2, origin.y + regretButton->getContentSize().height / 2));
	auto regretLabel = Label::createWithTTF("Regret", "fonts/Marker Felt.ttf", 24);
	regretLabel->setPosition(regretButton->getPosition());
	auto regretListener = EventListenerTouchOneByOne::create();
	regretListener->onTouchBegan = [this](Touch* t, Event* e) {
		if (menuShowing)return false;
		if (promotionPlateShowing)return false;
		if (!chess.isDone())return false;
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	regretListener->onTouchEnded = [this](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			chess.reverseStep();
			refreshPlayingPlayerInfo();
		} 
	};

	this->addChild(regretButton);
	this->addChild(regretLabel);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(regretListener, regretButton);
}

void TwoComputersGame2D::initButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto regretButton = Sprite::create("LongButton.png");
	regretButton->setPosition(Vec2(origin.x + visibleSize.width - regretButton->getContentSize().width / 2, origin.y + regretButton->getContentSize().height / 2));
	auto regretLabel = Label::createWithTTF("Regret", "fonts/Marker Felt.ttf", 24);
	regretLabel->setPosition(regretButton->getPosition());
	auto regretListener = EventListenerTouchOneByOne::create();
	regretListener->onTouchBegan = [this](Touch* t, Event* e) {
		if (menuShowing)return false;
		if (!chess.isDone())return false;
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) return true;
		return false;
	};
	regretListener->onTouchEnded = [this](Touch* t, Event* e) {
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation())) {
			paused = true;
			if (game_is_over) {
				game_is_over = false;
				GameOverInfoMoveDown();
			}
			chess.reverseStep();
		}
	};

	auto pauseButton = Sprite::create("ShortButton.png");
	pauseButton->setPosition(Vec2(origin.x + visibleSize.width - pauseButton->getContentSize().width / 2, origin.y + visibleSize.height * 2 / 3));
	auto pauseLabel = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", 24);
	pauseLabel->setPosition(pauseButton->getPosition());
	auto pauseListener = EventListenerTouchOneByOne::create();
	pauseListener->onTouchBegan = [this, origin, visibleSize](Touch* t, Event* e) {
		if (game_is_over)return false;
		if (menuShowing)return false;
		auto p = t->getLocation();
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(p)) return true;
		if (p.x > origin.x + visibleSize.width - 117 && p.y > origin.x + visibleSize.height - 46 && !paused)return true;
		return false;
	};
	pauseListener->onTouchEnded = [this](Touch* t, Event* e) {
		paused = !paused;
		flag = true;
	};

	auto restartButton = Sprite::create("ShortButton.png");
	restartButton->setPosition(Vec2(origin.x + visibleSize.width - restartButton->getContentSize().width / 2, origin.y + visibleSize.height / 3));
	auto restartLabel = Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 24);
	restartLabel->setPosition(restartButton->getPosition());
	auto restartListener = EventListenerTouchOneByOne::create();
	restartListener->onTouchBegan = [this, origin, visibleSize](Touch* t, Event* e) {
		if (game_is_over)return false;
		if (menuShowing)return false;
		auto p = t->getLocation();
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(p)) return true;
		return false;
	};
	restartListener->onTouchEnded = [this](Touch* t, Event* e) {
		openNewGame();
	};

	this->addChild(pauseButton);
	this->addChild(pauseLabel);
	this->addChild(regretButton);
	this->addChild(regretLabel);
	this->addChild(restartButton);
	this->addChild(restartLabel);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pauseListener, pauseButton);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(regretListener, regretButton);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(restartListener, restartButton);
}

void TwoComputersGame2D::initGameInfo()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Two Computers    Dimension: 2D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void TwoComputersGame2D::initListener()
{
	schedule([this](float f) {
		if (!flag)return;
		if (game_is_over)return;
		if (menuShowing)return;
		if (paused)return;
		if (!chess.isDone())return;
		ChessPieceMovement movement;
		if (chess.isCheckmate()) {
			game_is_over = true;
			string winner;
			if (chess.data.nowPlaying == Player::black) {
				winner += "White (";
				winner += player_white();
				winner += ")";
			}
			else {
				winner += "Black (";
				winner += player_black();
				winner += ")";
			}
			winner += " Win !";
			GameOverInfoRiseUpWithString(winner);
		}
		else if (chess.isStalemate()) {
			game_is_over = true;
			GameOverInfoRiseUpWithString("StaleMate!");
		}
		else {
			flag = false;
			if (chess.data.nowPlaying == white) movement = (*best_step)(&chess.data);
			else movement = (*best_step2)(&chess.data);
			chess.step(movement);
			refreshPlayingPlayerInfo();
			flag = true;
		}
		}, 0.2f, string("StopAlwaysAI"));
}

void TwoComputersGame2D::openNewGame()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto middle = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	game_is_over = false;
	chess.initial();
	chess.setPosition(middle);
	refreshGameRange();
	refreshPlayingPlayerInfo();
	paused = true;
}

void TwoPlayersGame3D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Two Players    Dimension: 3D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void EasyGame2D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Easy    Dimension: 2D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void MediumGame2D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Medium    Dimension: 2D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void HardGame2D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Hard    Dimension: 2D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void EasyGame3D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Easy    Dimension: 3D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void MediumGame3D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Medium    Dimension: 3D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}

void HardGame3D::initGameInfo() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto label_game_info = Label::createWithTTF("Game Mode: Hard    Dimension: 3D", "fonts/Marker Felt.ttf", 24);
	label_game_info->setPosition(Vec2(\
		origin.x + label_game_info->getContentSize().width / 2, \
		origin.y + visibleSize.height - label_game_info->getContentSize().height / 2));
	this->addChild(label_game_info, 0);
}
