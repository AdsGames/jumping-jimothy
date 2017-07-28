#include "game.h"
#include <allegro5/bitmap.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>

#define danny_wants_to_listen_to_music_while_programming true

// Constructor
game::game(){
  // Init first time
  newBox = nullptr;
  rootBox = nullptr;
  level = 1;

  // Reset fresh
  reset();

  // Load and play music
  music = nullptr;
  if(!danny_wants_to_listen_to_music_while_programming){
    music = tools::load_sample_ex( "music/tojam.ogg");
    al_play_sample( music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, currentMusic);
  }
}

// Destructor
game::~game(){
  // Stop music
  if( music != nullptr){
    al_stop_sample( currentMusic);
    al_destroy_sample( music);
  }

  // Destory bitmaps
  if( box != nullptr)
    al_destroy_bitmap( box);
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
Goat *game::create_goat( float newX, float newY){
  Goat *newGoat = new Goat();
  newGoat -> init( newX, newY,goat_map, gameWorld,gameCharacter);
  gameBoxes.push_back( newGoat);
  return newGoat;
}

// Creates box in world
Box *game::create_box( float newX, float newY, float newWidth, float newHeight, float newVelX,float newVelY, BITMAP *sp_1,BITMAP *sp_2,BITMAP *sp_3,BITMAP *sp_4, bool newBodyType, bool newIsSensor){
  Box *newBox = new Box();
  newBox -> init( newX, newY, newWidth, newHeight, newVelX,newVelY,newBodyType, sp_1,sp_2,sp_3,sp_4, gameWorld);
  gameBoxes.push_back( newBox);
  return newBox;
}

// Creates box in world
Box *game::create_static_box( float newX, float newY,  BITMAP *sp_1,BITMAP *sp_2,BITMAP *sp_3,BITMAP *sp_4){
  StaticBox *newStaticBox = new StaticBox();
  newStaticBox -> init( newX, newY, sp_1,sp_2,sp_3,sp_4);
  gameBoxes.push_back( newStaticBox);
  return newStaticBox;
}

Box *game::create_collision( float newX, float newY,float newWidth,float newHeight){
  CollisionBox *newCollisionBox = new CollisionBox();
  newCollisionBox -> init( newX, newY, newWidth,newHeight,gameWorld);
  gameBoxes.push_back( newCollisionBox);
  return newCollisionBox;
}




// Add character to world
Character *game::create_character( float newX, float newY){
  Character *newCharacter = new Character();
  newCharacter -> init( newX, newY,character, gameWorld);
  gameBoxes.push_back( newCharacter);
  return newCharacter;
}

// Sets up Box2D world
void game::b2_setup(){
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
void game::load_world(int newLevel){

  std::cout << "Attempting to load level_" << newLevel << ".xml into game\n";

  gameCharacter = new Character();

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
  std::ifstream theFile( "data/level_"+tools::convertIntToString(newLevel) +".xml");
  std::vector<char> xml_buffer( (std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  root_node = doc.first_node("data");

  // Make sure node exists
  if( root_node != 0){
    // Iteratboole over the nodes
    for (rapidxml::xml_node<> * object_node = root_node -> first_node("Object"); object_node; object_node = object_node -> next_sibling()){
      // All the variables we will load
      std::string type = "Tile";
      std::string x = "0";
      std::string y = "0";
      std::string orientation = "0";
      std::string bodytype = "Static";
      std::string width = "0";
      std::string height = "0";
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
      if( object_node -> first_node("bodytype") != 0)
        bodytype = object_node -> first_node("bodytype") -> value();
      if( object_node -> first_node("orientation") != 0)
        orientation = object_node -> first_node("orientation") -> value();

      std::cout<<type<<"\n";

      if( type == "Tile"){
        if(bodytype == "Static"){
            std::vector<std::string> splits = tools::split_string( orientation, ' ');
            if( splits.size() == 4)
              for( int k = 0; k < 4; k++)
                orientation_array[k] = (tools::convertStringToInt(splits.at(k)));


          // Out with the old, in with the new

         // newBox = create_box( tools::string_to_float(x), tools::string_to_float(y), 1.5, 1.5, tools::string_to_float(vel_x), tools::string_to_float(vel_y), new_dynamic_tile[orientation_array[0]],new_dynamic_tile[orientation_array[1]],
          //                         new_dynamic_tile[orientation_array[2]],new_dynamic_tile[orientation_array[3]],false, false);

          newBox = create_static_box( tools::string_to_float(x), tools::string_to_float(y), new_dynamic_tile[orientation_array[0]],new_dynamic_tile[orientation_array[1]],
                                   new_dynamic_tile[orientation_array[2]],new_dynamic_tile[orientation_array[3]]);


          static_count++;
        }else if(bodytype=="Dynamic"){
          newBox = create_box( tools::string_to_float(x), tools::string_to_float(y), 1.6, 1.6, tools::string_to_float(width), tools::string_to_float(height), box,nullptr,nullptr,nullptr, true, false);
          dynamic_count++;
        }else if(bodytype=="Collision"){
          newBox = create_collision( tools::string_to_float(x), tools::string_to_float(y), tools::string_to_float(width), tools::string_to_float(height));
        }


      }
      else if( type == "Character"){
        gameCharacter = create_character( tools::string_to_float(x), tools::string_to_float(y));
        character_count++;
      }
      else if( type == "Finish"){
        gameGoat = create_goat( tools::string_to_float(x), tools::string_to_float(y));
        if(gameCharacter==nullptr)
          std::cout<<"WARNING: Game: goat is passed nullptr gameCharacter\n";
        goat_count++;
      /*if(group=="1"){
        if(rootBox==nullptr)
          rootBox=newBox;
        else{
          b2Vec2 FeetAnchor(0,0);
          b2WeldJointDef *jointDef = new b2WeldJointDef();
          jointDef -> Initialize(newBox -> getBody(), rootBox -> getBody(), FeetAnchor);
          jointDef -> collideConnected = false;
          jointDef  -> referenceAngle = 0;
          gameWorld -> CreateJoint(jointDef);
        }*/
      }

    }
  }

  // Nice debug code
  // Allan's big ol' debug lines, they're pretty tho
  std::cout << "===============================\n";
  std::cout << "Level loaded: " << static_count << " static, " << dynamic_count << " dynamic, " << character_count << " character(s), " << goat_count << " goat(s)\n";
  if( character_count > 1)
    std::cout << "WARNING: Multiple characters loaded, will have undesired results...\n";
  if( goat_count > 1)
    std::cout << "WARNING: Multiple goats loaded, will have undesired results...\n";
  if( character_count == 0)
    std::cout << "WARNING: No character loaded, will have undesired results...\n";
  if( goat_count == 0)
    std::cout << "WARNING: No goat loaded, will have undesired results...\n";
  if( static_count == 0 && dynamic_count == 0 && goat_count == 0 && character_count == 0)
    std::cout << "WARNING: No data loaded!\n";
  std::cout << "===============================\n";

}

// Reset game
void game::reset(){
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
    std::cout << "WARNING: Goat pointer is undeclared in game\n";

  if( gameCharacter == nullptr)
    std::cout << "WARNING: gameCharacter pointer is undeclared in game\n";
}

// Load all sprites for in game
void game::load_sprites(){
  box = tools::load_bitmap_ex( "images/box.png");
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
void game::update(){
  // Game mode
  if( keyListener::keyPressed[ALLEGRO_KEY_P])
    set_next_state( STATE_EDIT);
  // Change state?
  if( keyListener::key[ALLEGRO_KEY_I])
    set_next_state( STATE_MENU);

  // Touching goat
  if( gameGoat -> getWinCondition()){
    level ++;
    if( level >= 10){
      al_show_native_message_box( nullptr, "Hurrah!", "Victory", "Will you accept your prize?", nullptr, ALLEGRO_MESSAGEBOX_YES_NO);
      gameCharacter -> getBody() -> SetTransform( b2Vec2( 100, 100), 0);
    }
    else{
      std::cout<<"Level " << level-1 << " completed, loading next level\n";
      reset();
    }
  }

  // Off screen
  if( gameCharacter -> getX() < -1 || gameCharacter -> getX() > 51.5f || gameCharacter -> getY() > 2 || gameCharacter -> getY() < -40)
    reset();

  // Update the Box2D game world
  gameWorld -> Step( timeStep, velocityIterations, positionIterations);

  // Update character
  for( unsigned int i = 0; i < gameBoxes.size(); i++)
    gameBoxes[i] -> update();

  // Die
  if( keyListener::keyPressed[ALLEGRO_KEY_Z] || joystickListener::buttonPressed[JOY_XBOX_B] ||  keyListener::keyPressed[ALLEGRO_KEY_R] )
    reset();

  // Next level
  if( keyListener::keyPressed[ALLEGRO_KEY_C]){
    std::cout<<"Level " << level<< " skipped\n";

    level++;
    reset();
  }

  // Previous level
  if( keyListener::keyPressed[ALLEGRO_KEY_X]){
    std::cout<<"Level " << level<< " skipped back\n";
    if( level > 1)
      level--;
    reset();
  }

  // Pause/Play time
  if( keyListener::keyPressed[ALLEGRO_KEY_SPACE]  || joystickListener::buttonPressed[JOY_XBOX_X] ||  joystickListener::buttonPressed[JOY_XBOX_Y] || joystickListener::buttonPressed[JOY_XBOX_BUMPER_LEFT] || joystickListener::buttonPressed[JOY_XBOX_BUMPER_RIGHT]  ){
    static_mode =! static_mode;
    if(static_mode){
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType() == BOX){
          // Character *newCharacter = dynamic_cast<Character*>(&gameBoxes[i]);
          gameBoxes[i] -> setStatic();
        }
      }
    }
    else{
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType()==BOX){
          // Character *newCharacter = dynamic_cast<Character*>(&gameBoxes[i]);
          gameBoxes[i] -> setDynamic(!first_play);
        }
      }
      first_play = false;
    }
  }
}

// Draw to screen
void game::draw(){
  // Background
  al_clear_to_color( al_map_rgb(40,40,60));

  // Help
  if( level == 1)
    al_draw_bitmap(help,0,0,0);

  // Draw boxes
  for( unsigned int i = 0; i < gameBoxes.size(); i++)
    gameBoxes[i] -> draw();

  // Pause/play buttons
  if(static_mode)
    al_draw_bitmap(pause,10,10,0);
  else
    al_draw_bitmap(play,10,10,0);
}
