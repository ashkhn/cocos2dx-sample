#include "GameScene.h"
#include "BallGrid.h"
USING_NS_CC;

#define NUM_ROWS  5
#define NUM_COLS  4

Game::Game(void){}

Game::~Game(void){}

Scene* Game::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = Game::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Game::init(){
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() ){
		return false;
	}
	
	//Initialize rand generator and display variables
	std::srand(time(NULL));
	_player_score = 0;
	_screen_size = Director::getInstance()->getVisibleSize();
	
	// Load the sprite frame cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ball_sprites.plist");
	//initialize the grid
	bool is_resumed = UserDefault::getInstance()->getBoolForKey("is_resumed");
	initGrid(is_resumed);

	// Register touch listeners
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	_player_score_label = Label::createWithSystemFont("Score: 0", "Arial", 80);
	_player_score_label->setPosition(Vec2(_screen_size.width * 0.2, _screen_size.height * 0.2));
	_player_score_label->setTextColor(Color4B::WHITE);
	this->addChild(_player_score_label, 1);
	
	
	return true;
}

// Initialize the grid and position the balls. 
// @param is_resumed: Defines whether to start a new game or to resume a previously saved game
void Game::initGrid(bool is_resumed){
	// Set spacing and starting point of grid
	auto start_x = _screen_size.width * 0.2;
	auto start_y = _screen_size.height * 0.5;
	int distance_measure = 25;


	// Generate grid
	ball_grid = new BallGrid();
	ball_grid->_grid_origin = Point(start_x, start_y);
	if(is_resumed){
		ball_grid->resumeGrid(NUM_COLS, NUM_ROWS);
	}
	else{
		ball_grid->generateGrid(NUM_COLS, NUM_ROWS);
	}
	for (int i = 0; i < NUM_ROWS; i++){
		for (int j = 0; j < NUM_COLS; j++) {
			auto x_cord = start_x + j*ball_grid->ball_sprites[i][j]->radius() * distance_measure;
			auto y_cord = start_y + i*ball_grid->ball_sprites[i][j]->radius() * distance_measure;
			ball_grid->ball_sprites[i][j]->setPosition(Vec2(x_cord, y_cord));
			this->addChild(ball_grid->ball_sprites[i][j], 1);
		}
	}
	ball_grid->_grid_height = ball_grid->ball_sprites[NUM_ROWS - 1][0]->getPositionY();
	ball_grid->_grid_width = ball_grid->ball_sprites[0][NUM_COLS - 1]->getPositionX();
	ball_grid->_grid_step_x = ball_grid->ball_sprites[0][1]->getPositionX() - ball_grid->ball_sprites[0][0]->getPositionX();
	ball_grid->_grid_step_y = ball_grid->ball_sprites[1][0]->getPositionY() - ball_grid->ball_sprites[0][0]->getPositionY();


}

// Check whether the first row of the grid is selected
// @param touch : Defining the parameters where the user touched the screen
// @return true if any ball in the first row of grid was touched, false otherwise
bool Game::isFirstRowSelected(Touch* touch){
	if(touch!=nullptr){
		auto tap = touch->getLocation();
		for(int col_idx = 0; col_idx < NUM_COLS; col_idx++){
			if(ball_grid->ball_sprites[0][col_idx]->getBoundingBox().containsPoint(tap)){
				ball_grid->_chosen_idx = col_idx + 1;
				return true;
			}
		}
		return false;
	}
	return false;
}


// Called when the user's finger lands on screen. 
// @param touch, @param event contain all information regarding the action
// @return true if the event is handled by the scene and false otherwise
// Check if the user touched the first row and if yes calculate the 
// path and highlight it
bool Game::onTouchBegan(Touch* touch, Event* event){
	//Check if touch was valid
	if(isFirstRowSelected(touch)){
		ball_grid->setPath(ball_grid->_chosen_idx);
		ball_grid->highlightPath();
		return true;
	}

	return false;
}

// Called when the user's finger moves without leaving screen contact.
// In practical situations this method is continously called since 
// human fingers are not point contacts.
// TODO Cancel the choice made in onTouchBegan
void Game::onTouchMoved(Touch* touch, Event* event){
}


// Called when the user's fingers leaves contact from the screen.
// @param touch, @param event contain all information regarding the action
// Check if the user had touched the first row element and if yes then performs
// all manipulations of adding and removing elements
void Game::onTouchEnded(Touch* touch, Event* event){
	
	if (isFirstRowSelected(touch)){

		// Animate the balls and manipulate the grid
		ball_grid->generateNewGrid();
		ball_grid->highlightPath();

		// Add the new balls to the scene graph
		for (auto ball : ball_grid->balls_to_be_added){
			this->addChild(ball, 1);
		}
		
		//reset opacity
		for ( auto ball_row : ball_grid->ball_sprites ){
			for (auto ball : ball_row){
				ball->setOpacity(255);
			}
		}
	}
}


