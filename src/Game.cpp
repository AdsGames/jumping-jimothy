#include "Game.h"

#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include "MouseListener.h"
#include "KeyListener.h"
#include "JoystickListener.h"

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "StaticBox.h"
#include "Tools.h"
#include "Config.h"

#include "CollisionBox.h"
#include "DynamicBox.h"
#include "Explosive.h"
#include "LevelSelect.h"
#include "MusicManager.h"

bool Game::testing = false;
const char* Game::testing_file_name = nullptr;

// Constructor
Game::Game() {
  // Create character
  gameCharacter = new Character();

  // Init first time
  newBox = nullptr;
  rootBox = nullptr;

  level = Config::level_to_start;
  Config::level_to_start = 1;

  game_font = al_load_ttf_font( "fonts/munro.ttf", 30, 0);
  help_font = al_load_ttf_font( "fonts/munro.ttf", 50, 0);
  edit_font = al_load_ttf_font( "fonts/fantasque.ttf", 18, 0);

  toggle_on.load_wav("sfx/toggle_on.wav");
  toggle_off.load_wav("sfx/toggle_off.wav");
  death.load_wav("sfx/death.wav");

  if( testing){
    level = 0;
    testing_back_button = Button(966,728,"Back",edit_font);
  }

  // Reset fresh
  reset();

  // Load and play music
  MusicManager::menu_music.stop();

  #if defined(RELEASE)
    MusicManager::game_music.play();
  #endif
}

// Destructor
Game::~Game(){
  // Destory bitmaps
  //if( box != nullptr)
  //  al_destroy_bitmap( box);
  if( goat_sprite != nullptr)
    al_destroy_bitmap( goat_sprite);
  if( goat_map != nullptr)
    al_destroy_bitmap( goat_map);
  if( help != nullptr)
    al_destroy_bitmap( help);
  if( character != nullptr)
    al_destroy_bitmap( character);
  if( static_tileset != nullptr)
    al_destroy_bitmap( static_tileset);
  if( play != nullptr)
    al_destroy_bitmap( play);
  if( pause != nullptr)
    al_destroy_bitmap( pause);
}

// Creates box in world
Goat* Game::create_goat( float newX, float newY){
  Goat *newGoat = new Goat();
  newGoat -> init( newX, newY,goat_map, gameWorld,gameCharacter);
  if(gameCharacter==nullptr)
    tools::log_message("WARNING: gameCharacter is nullptr when creating an explosive box.");
  gameBoxes.push_back( newGoat);
  return newGoat;
}

// Creates box in world
Box* Game::create_dynamic_box( float newX, float newY, float newWidth, float newHeight, float newVelX,float newVelY, ALLEGRO_BITMAP *newSprite, bool newBodyType, bool newIsSensor){
  DynamicBox *newDynamicBox = new DynamicBox();
  newDynamicBox -> init( newX, newY, newWidth, newHeight, newVelX,newVelY,newBodyType, newSprite, gameWorld);
  gameBoxes.push_back( newDynamicBox);
  return newDynamicBox;
}

// Creates box in world
Box* Game::create_static_box( float newX, float newY,  ALLEGRO_BITMAP *sp_1,ALLEGRO_BITMAP *sp_2,ALLEGRO_BITMAP *sp_3,ALLEGRO_BITMAP *sp_4){
  StaticBox *newStaticBox = new StaticBox();
  newStaticBox -> init( newX, newY, sp_1,sp_2,sp_3,sp_4);
  gameBoxes.push_back( newStaticBox);
  return newStaticBox;
}

Box* Game::create_collision_box( float newX, float newY,float newWidth,float newHeight){
  CollisionBox *newCollisionBox = new CollisionBox();
  newCollisionBox -> init( newX, newY, newWidth,newHeight,gameWorld);
  gameBoxes.push_back( newCollisionBox);
  return newCollisionBox;
}

Box* Game::create_explosive_box(float newX, float newY,int newOrientation, bool newAffectCharacter){
  Explosive *newExplosive = new Explosive();
  ALLEGRO_BITMAP *newBoxImage;

  if(newOrientation==0)
    newBoxImage = box_repel;
  else
    newBoxImage = box_repel_direction;

  newExplosive -> init( newX, newY,newOrientation, newBoxImage,newAffectCharacter, gameWorld,gameCharacter);
  if(gameCharacter==nullptr)
    tools::log_message("WARNING: gameCharacter is nullptr when creating an explosive box.");

  gameBoxes.push_back( newExplosive);
  return newExplosive;

}

// Add character to world
Character *Game::create_character( float newX, float newY){
  Character *newCharacter = new Character();
  newCharacter -> init( newX, newY,character, gameWorld);
  gameBoxes.push_back( newCharacter);
  return newCharacter;
}

// Sets up Box2D world
void Game::b2_setup(){
  // Box2D world parameters
  gravity = b2Vec2(0.0f, -10.0f);
  timeStep = 1.0f / 60.0f;
  velocityIterations = 6;
  positionIterations = 2;
  doSleep = true;

  // Box2D game world
  gameWorld = new b2World( gravity);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set( 0.0f, -48.4f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	groundBody = gameWorld -> CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox( 80.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody -> CreateFixture( &groundBox, 0.0f);
}

// Load world from xml
void Game::load_world(int newLevel){
  // Load level message
  tools::log_message("Attempting to load level_" + tools::toString(newLevel) + ".xml into game");

  int goat_count = 0;
  int character_count = 0;
  int dynamic_count = 0;
  int static_count = 0;

  // Destroy ground
  gameWorld -> DestroyBody(groundBody);

  // Doc
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<> * root_node;

  // Make an xml object
  std::ifstream theFile( "data/level_"+tools::toString(newLevel) +".xml");
  std::vector<char> xml_buffer( (std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  root_node = doc.first_node("data");

  // Make sure node exists
  if( root_node != 0){
    // Iteratboole over objects
    for (rapidxml::xml_node<> *object_node = root_node -> first_node("Object"); object_node; object_node = object_node -> next_sibling()) {
      // All the variables we will load
      std::string type = "Static";
      std::string x = "0";
      std::string y = "0";
      std::string orientation = "0";
      //std::string bodytype = "Static";
      std::string width = "0";
      std::string height = "0";
      std::string affect_character = "false";
      int orientation_array[4];

      // Load data
      if( object_node -> first_attribute("type") != 0)
        type = object_node -> first_attribute("type") -> value();
      if( object_node -> first_node("x") != 0)
        x = object_node -> first_node("x") -> value();
      if( object_node -> first_node("y") != 0)
        y = object_node -> first_node("y") -> value();
      if( object_node -> first_node("width") != 0)
        width = object_node -> first_node("width") -> value();
      if( object_node -> first_node("height") != 0)
        height = object_node -> first_node("height") -> value();
      // if( object_node -> first_node("bodytype") != 0)
      //   bodytype = object_node -> first_node("bodytype") -> value();
      if( object_node -> first_node("orientation") != 0)
        orientation = object_node -> first_node("orientation") -> value();
      if( object_node -> first_node("affect_character") != 0)
        affect_character = object_node -> first_node("affect_character") -> value();

      if( type == "Static"){
        std::vector<std::string> splits = tools::split_string( orientation, ' ');
        if( splits.size() == 4) {
          for( int k = 0; k < 4; k++) {
            orientation_array[k] = (tools::stringToInt(splits.at(k)));
          }
        }

        newBox = create_static_box( tools::stringToFloat(x),
                                    tools::stringToFloat(y),
                                    new_dynamic_tile[orientation_array[0]],
                                    new_dynamic_tile[orientation_array[1]],
                                    new_dynamic_tile[orientation_array[2]],
                                    new_dynamic_tile[orientation_array[3]]);

        static_count++;
      }
      else if(type=="Dynamic"){
        newBox = create_dynamic_box( tools::stringToFloat(x), tools::stringToFloat(y), 1.6, 1.6, tools::stringToFloat(width), tools::stringToFloat(height), box,true, false);
        dynamic_count++;
      }
      else if(type=="Collision"){
        newBox = create_collision_box( tools::stringToFloat(x), tools::stringToFloat(y), tools::stringToFloat(width), tools::stringToFloat(height) );
      }
      else if( type == "Character"){
        gameCharacter = create_character( tools::stringToFloat(x), tools::stringToFloat(y)-1.6);
        character_count++;
      }
      else if( type == "Finish"){
        gameGoat = create_goat( tools::stringToFloat(x), tools::stringToFloat(y)-1);
        if(gameCharacter==nullptr)
          tools::log_message("WARNING: Game: goat is passed nullptr gameCharacter");
        goat_count++;

        // Once killed 17 people
        // Use at own risk
        /*
        if(group=="1"){
          if(rootBox==nullptr)
            rootBox=newBox;
          else{
            b2Vec2 FeetAnchor(0,0);
            b2WeldJointDef *jointDef = new b2WeldJointDef();
            jointDef -> Initialize(newBox -> getBody(), rootBox -> getBody(), FeetAnchor);
            jointDef -> collideConnected = false;
            jointDef  -> referenceAngle = 0;
            gameWorld -> CreateJoint(jointDef);
          }
        }
        */
      }
      else if(type == "Explosive"){
       newBox = create_explosive_box( tools::stringToFloat(x), tools::stringToFloat(y),tools::stringToFloat(orientation), (affect_character=="true"));
      }
    }

    // Get game help text
    rapidxml::xml_node<> *help_node = root_node -> first_node("Help");
    help_text.clear();

    if (help_node) {
      std::vector<std::string> temp_help = tools::split_string(help_node -> value(), '%');
      for (unsigned int i = 0; i < temp_help.size(); i++) {
        help_text.push_back(temp_help.at(i));
      }
    }
  }

  // Nice debug code
  // Allan's big ol' debug lines, they're pretty tho
  tools::log_message("===============================");
  //std::cout << "Level loaded: " << static_count << " static, " << dynamic_count << " dynamic, " << character_count << " character(s), " << goat_count << " goat(s)\n";
  if( character_count > 1)
    tools::log_message("  WARNING: Multiple characters loaded, will have undesired results...");
  if( goat_count > 1)
    tools::log_message("  WARNING: Multiple goats loaded, will have undesired results...");
  if( character_count == 0)
    tools::log_message("  WARNING: No character loaded, will have undesired results...");
  if( goat_count == 0)
    tools::log_message("  WARNING: No goat loaded, will have undesired results...");
  if( static_count == 0 && dynamic_count == 0 && goat_count == 0 && character_count == 0)
    tools::log_message("  WARNING: No data loaded!");
  tools::log_message("===============================");
}

// Reset game
void Game::reset(){
  // Reset variables
  gameBoxes.clear();
  gameGoat = nullptr;
  gameCharacter = nullptr;
  b2_setup();
  load_sprites();
  load_world(level);
  static_mode = true;
  first_play = true;

  // Pause boxes
  for( unsigned int i = 0; i < gameBoxes.size(); i++){
    if( gameBoxes[i] -> getType() == BOX){
      gameBoxes[i] -> setStatic();
    }
  }
  if( gameGoat == nullptr)
    tools::log_message("WARNING: Goat is undeclared in game");

  if( gameCharacter == nullptr)
    tools::log_message("WARNING: gameCharacter is undeclared in game");
}

// Load all sprites for in game
void Game::load_sprites(){
  box = tools::load_bitmap_ex( "images/box.png");
  box_repel = tools::load_bitmap_ex( "images/box_repel.png");
  box_repel_direction = tools::load_bitmap_ex( "images/box_repel_direction.png");
  goat_sprite = tools::load_bitmap_ex( "images/goat.png");
  goat_map = tools::load_bitmap_ex( "images/goat_map.png");
  help = tools::load_bitmap_ex( "images/help.png");
  character = tools::load_bitmap_ex( "images/character.png");
  static_tileset = tools::load_bitmap_ex( "images/StaticBlock.png");
  play = tools::load_bitmap_ex( "images/play.png");
  pause = tools::load_bitmap_ex( "images/pause.png");

  ALLEGRO_BITMAP *image_box = tools::load_bitmap_ex( "images/StaticBlock2.png");

  for( int i = 0; i < 3; i++)
    for( int t = 0; t < 5; t++)
      new_dynamic_tile[i + t*3] = al_create_sub_bitmap( image_box, i * 16, t * 16, 16, 16);

  for( int i = 0; i < 4; i++)
    for( int t = 0; t < 12; t++)
      tiles[1][i + t*4] = al_create_sub_bitmap( static_tileset, i * 32, t * 32, 32, 32);
}

// Update game logic
void Game::update(){
  if(testing){
    testing_back_button.update();

    if(testing_back_button.mouseReleased())
      set_next_state( STATE_EDIT);
  }

  // Game mode
  if( KeyListener::keyPressed[ALLEGRO_KEY_P])
    set_next_state( STATE_EDIT);

  // Change state?
  if( KeyListener::key[ALLEGRO_KEY_I])
    set_next_state( STATE_MENU);

  // Touching goat
  if( gameGoat != nullptr && gameGoat -> getWinCondition()){
    level ++;
    if( level > 14){
      set_next_state(STATE_MENU);
    }
    else{
      tools::log_message("Level " + tools::toString(level-1) + " completed, loading next level.");
      LevelSelect::setLevelComplete(level-1);
      LevelSelect::writeLevelData();
      if( !testing)
        reset();
      else{
        al_show_native_message_box( nullptr, "Level complete!", "Opening editor","", "Okily dokily.", 0);
        set_next_state( STATE_EDIT);
      }
    }
  }

  // Update the Box2D game world
  gameWorld -> Step( timeStep, velocityIterations, positionIterations);

  // Update character
  for( unsigned int i = 0; i < gameBoxes.size(); i++)
    gameBoxes[i] -> update();

  // Die
  if( KeyListener::keyPressed[ALLEGRO_KEY_Z] || JoystickListener::buttonPressed[JOY_XBOX_B] ||  KeyListener::keyPressed[ALLEGRO_KEY_R] ){
    death.play();
    reset();
  }

  if(KeyListener::keyPressed[ALLEGRO_KEY_ESCAPE]){
    set_next_state(STATE_MENU);
    MusicManager::game_music.stop();
  }

  // Next level
  if( KeyListener::keyPressed[ALLEGRO_KEY_C]){
    if( !testing){
      tools::log_message("Level " + tools::toString(level) + " skipped.");
      level++;
      reset();
    }
    else{
      al_show_native_message_box( nullptr, "Level complete!", "Opening editor",nullptr, nullptr, 0);
      set_next_state( STATE_EDIT);
    }
  }

  // Previous level
  if( KeyListener::keyPressed[ALLEGRO_KEY_X]){
    tools::log_message("Level " + tools::toString(level) + " skipped back.");
    if( level > 1)
      level--;
    reset();
  }

  // Pause/Play time
  if( KeyListener::keyPressed[ALLEGRO_KEY_SPACE]  ||
      JoystickListener::buttonPressed[JOY_XBOX_X] ||
      JoystickListener::buttonPressed[JOY_XBOX_Y] ||
      JoystickListener::buttonPressed[JOY_XBOX_BUMPER_LEFT] ||
      JoystickListener::buttonPressed[JOY_XBOX_BUMPER_RIGHT]  ){
    static_mode =! static_mode;

    if(static_mode){
      toggle_off.play();
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType() == BOX){
          gameBoxes[i] -> setStatic();
        }
      }
    }
    else{
      toggle_on.play();
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType()==BOX){
          gameBoxes[i] -> setDynamic(!first_play);

        }
      }
      first_play = false;
    }
  }

  // Off screen
  // Brutal hack don't look. Prevens character from dying when the character's location is undefined or something. It's really big anyways
  if(gameCharacter != nullptr && gameCharacter -> getX()>-5000 && gameCharacter -> getX()<5000 && gameCharacter -> getY()>-5000 && gameCharacter -> getY()<5000 ){
    if(gameCharacter -> getX() < -1 || gameCharacter -> getX() > 51.5f || gameCharacter -> getY() > 2 || gameCharacter -> getY() < -40){
      death.play();
      reset();
      tools::log_message("===========" + tools::toString(gameCharacter -> getX()));
    }
  }
}

// Draw to screen
void Game::draw(){
  // Background
  al_clear_to_color( al_map_rgb(40,40,60));

  // Help text
  for (unsigned int i = 0; i < help_text.size(); i++) {
    al_draw_textf(help_font, al_map_rgb(255, 255, 255), 500, 75 + i * 50, 1, help_text.at(i).c_str());
  }

  // Draw boxes
  for( unsigned int i = 0; i < gameBoxes.size(); i++){
    gameBoxes[i] -> draw();
  }

  // Pause/play buttons
  if(testing)
    testing_back_button.draw();

  if(static_mode)
    al_draw_bitmap(pause,10,10,0);
  else
    al_draw_bitmap(play,10,10,0);

  al_draw_textf( game_font, al_map_rgb( 255, 255, 255), 1010, 15, 2, "Level %i", level);
}
