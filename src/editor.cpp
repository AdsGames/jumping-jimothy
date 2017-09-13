#include "editor.h"

// Init editor
editor::editor(){



  std::cout << "Initializing editor\n";

  // Level to edit
  level_number = 1;

  // Filename
  file_name = "Untitled";

  gui_mode = true;

  // Load box image
  image_box[0] = tools::load_bitmap_ex( "images/DynamicBlock2.png");
  image_box[1] = tools::load_bitmap_ex( "images/StaticBlock2.png");
  image_box[2] = tools::load_bitmap_ex( "images/character.png");
  image_box[3] = tools::load_bitmap_ex( "images/DisgoatSpriteMap.png");

  for( int i = 0; i < 4; i++)
    for( int t = 0; t < 15; t++)
      tiles[i][t] = nullptr;

  // Dynamic
  for( int i = 0; i < 3; i++)
    for( int t = 0; t < 5; t++)
      tiles[0][i + t*3] = al_create_sub_bitmap( image_box[0], i * 16, t * 16, 16, 16);

  // Static
  for( int i = 0; i < 3; i++)
    for( int t = 0; t < 5; t++)
      tiles[1][i + t*3] = al_create_sub_bitmap( image_box[1], i * 16, t * 16, 16, 16);

  // Player
  tiles[2][0] = al_create_sub_bitmap( image_box[2], 0, 0, 32, 64);

  // Goat
  tiles[3][0] = al_create_sub_bitmap( image_box[3], 0, 0, 32, 64);

  tile_type = 0;

  srand(time(NULL));

  edit_font = al_load_ttf_font( "fonts/fantasque.ttf", 18, 0);

  if (!edit_font)
    tools::abort_on_error( "Could not load 'fantasque.ttf'.\n", "Font Error");

  grid_on = false;
  editorBoxes.clear();


  // Buttons
  editor_buttons[button_type_dynamic] = button( 0, 728, "Dynamic", edit_font);
  editor_buttons[button_type_static] = button(editor_buttons[button_type_dynamic].getX() + editor_buttons[button_type_dynamic].getWidth(), 728, "Static", edit_font);
  editor_buttons[button_type_player] = button(editor_buttons[button_type_static].getX() + editor_buttons[button_type_static].getWidth(), 728, "Player", edit_font);
  editor_buttons[button_type_goat] = button( editor_buttons[button_type_player].getX() + editor_buttons[button_type_player].getWidth(), 728, "Goat", edit_font);
  editor_buttons[button_type_collision] = button(editor_buttons[3].getX() + editor_buttons[3].getWidth(), 728, "Collision", edit_font);
  editor_buttons[button_hide_left] = button( editor_buttons[4].getX() + editor_buttons[4].getWidth(), 728, "<", edit_font);

  editor_buttons[button_hide_right] = button( 566, 728, ">", edit_font);
  editor_buttons[button_undo] = button( editor_buttons[button_hide_right].getX() + editor_buttons[button_hide_right].getWidth(), 728, "Undo", edit_font);
  editor_buttons[button_clear] = button(editor_buttons[button_undo].getX() + editor_buttons[button_undo].getWidth(), 728, "Clear", edit_font);
  editor_buttons[button_save] = button(editor_buttons[button_clear].getX() + editor_buttons[button_clear].getWidth(), 728, "Save", edit_font);
  editor_buttons[button_save_as] = button( editor_buttons[button_save].getX() + editor_buttons[button_save].getWidth(), 728, "Save as", edit_font);
  editor_buttons[button_load] = button(editor_buttons[button_save_as].getX() + editor_buttons[button_save_as].getWidth(), 728, "Load", edit_font);
  editor_buttons[button_grid] = button(editor_buttons[button_load].getX() + editor_buttons[button_load].getWidth(), 728, "Grid", edit_font);
  editor_buttons[button_play] = button(editor_buttons[button_grid].getX() + editor_buttons[button_grid].getWidth(), 728, "Play", edit_font);

  // Is it edit mode?
  if( game::testing){
    load_map( "data/level_0.xml");
    game::testing = false;
  }
}

// Destruct
editor::~editor(){
  // Destroy resources if loaded
  if( edit_font != nullptr)
    al_destroy_font( edit_font);

  // Tile images
  for( int i = 0; i < 4; i++)
    for( int t = 0; t < 15; t++)
      if( tiles[i][t] != nullptr)
        al_destroy_bitmap( tiles[i][t]);

  // Parent bitmaps
  for( int i = 0; i < 4; i++)
    if( image_box[i] != nullptr)
      al_destroy_bitmap( image_box[i]);
}

// Update editor
void editor::update(){
  // Update buttons
  for( int i = 0; i < BUTTON_COUNT; i++){
    if(gui_mode)
      editor_buttons[i].update();
  }

  // Check if over button
  bool over_button = false;
  for( int i = 0; i < BUTTON_COUNT; i++){
    if( editor_buttons[i].hover()){
      over_button = true;
      break;
    }
  }

  // Change type
  for( int i = 0; i <5; i++)
    if( editor_buttons[i].mouseReleased())
      tile_type = i;


  // Advanced mode types
  if(keyListener::keyPressed[ALLEGRO_KEY_Q])
    tile_type = 0;
  if(keyListener::keyPressed[ALLEGRO_KEY_W])
    tile_type = 1;
  if(keyListener::keyPressed[ALLEGRO_KEY_E])
    tile_type = 2;
  if(keyListener::keyPressed[ALLEGRO_KEY_R])
    tile_type = 3;
  if(keyListener::keyPressed[ALLEGRO_KEY_T])
    tile_type = 4;

  // Rockin' three liner undo button
  if((keyListener::keyPressed[ALLEGRO_KEY_Z] || editor_buttons[button_undo].mouseReleased() )&& editorBoxes.size()>0){
    editorBoxes.pop_back();
    calculate_orientation_global();
  }

  // Clear world button
  if(keyListener::keyPressed[ALLEGRO_KEY_C] || editor_buttons[button_clear].mouseReleased()){
    editorBoxes.clear();
  }

  // Activate advanced mode
  // Don't tell Allan, but I really don't like his buttons
  if(keyListener::keyPressed[ALLEGRO_KEY_A])
    gui_mode =! gui_mode;

  // Save
  if( editor_buttons[button_save].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_S]){
    if( editorBoxes.size() > 0){
      ALLEGRO_FILECHOOSER *myChooser;

      // Has it been saved already?
      if( !is_saved){
        myChooser = al_create_native_file_dialog( "data/", "Save Level", "*.xml;*.*", ALLEGRO_FILECHOOSER_SAVE);

        // Display open dialog
        if( al_show_native_file_dialog( nullptr, myChooser))
          file_name = al_get_native_file_dialog_path( myChooser, 0);
      }
      // Make sure file name is proper
      if(file_name != nullptr){
        // Make sure saves correctly
        if( save_map( file_name)){
          al_show_native_message_box( nullptr, "Saved map", "We've saved a map to: ", file_name, nullptr, 0);
          is_saved = true;
        }
        else
          al_show_native_message_box( nullptr, "Error!", "Error saving map to: ", file_name, nullptr, ALLEGRO_MESSAGEBOX_ERROR);
      }
    }
    else
      al_show_native_message_box( nullptr, "Empty Map", "You can't save an empty map!", "", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  }
  // Save as
  if( editor_buttons[button_save_as].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_X]){
    if( editorBoxes.size() > 0){
      ALLEGRO_FILECHOOSER *myChooser;

      // Has it been saved already?

      myChooser = al_create_native_file_dialog( "data/", "Save Level", "*.xml;*.*", ALLEGRO_FILECHOOSER_SAVE);

         // Display open dialog
      if( al_show_native_file_dialog( nullptr, myChooser))
        file_name = al_get_native_file_dialog_path( myChooser, 0);

      if(file_name != nullptr){
          // Make sure saves correctly
        if( save_map( file_name)){
          al_show_native_message_box( nullptr, "Saved map", "We've saved a map to: ", file_name, nullptr, 0);
          is_saved = true;
        }else{
          al_show_native_message_box( nullptr, "Error!", "Error saving map to: ", file_name, nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        }
      }

    }else{
        al_show_native_message_box( nullptr, "Empty Map", "You can't save an empty map!","", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }
  }
  // Load map
  if( editor_buttons[button_load].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_D]){
    ALLEGRO_FILECHOOSER *myChooser = al_create_native_file_dialog( "data/", "Load Level", "*.xml;*.*", 0);

    // Display open dialog
    if( al_show_native_file_dialog( nullptr, myChooser)){
      file_name = al_get_native_file_dialog_path(myChooser, 0);


      // You also need to check for cancel button here too
      if( file_name != nullptr){
        editorBoxes.clear();

        // Make sure loads correctly
        if( load_map( file_name))
          al_show_native_message_box( nullptr, "Loaded map", "We've loaded a map from: ", file_name, nullptr, 0);
        else
          al_show_native_message_box( nullptr, "Error!", "Error loading map from: ", file_name, nullptr, 0);
      }
      else{
        file_name = "Untitled";
      }
    }
  }

  // Play
  if( editor_buttons[button_play].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_F]){
     save_map( "data/level_0.xml");
     set_next_state( STATE_GAME);
     game::testing = true;
  }

  // Grid toggle
  if( editor_buttons[button_grid].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_G])
    grid_on = !grid_on;

  if( editor_buttons[button_hide_left].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_LEFT]){
    editor_buttons[button_type_collision].toggleVisibility();
    editor_buttons[button_type_static].toggleStatus();
    editor_buttons[button_type_dynamic].toggleStatus();
    editor_buttons[button_type_player].toggleStatus();
    editor_buttons[button_type_goat].toggleStatus();
    if(editor_buttons[button_hide_left].getText()=="<"){
      editor_buttons[button_hide_left].setPosition( 0, 728);
      editor_buttons[button_hide_left].setText(">");
      editor_buttons[button_hide_left].setTransparency(150);
    }else{
      editor_buttons[button_hide_left].setPosition( editor_buttons[4].getX() + editor_buttons[4].getWidth(), 728);
      editor_buttons[button_hide_left].setText("<");
      editor_buttons[button_hide_left].setTransparency(255);

    }

  }

   if( editor_buttons[button_hide_right].mouseReleased() || keyListener::keyPressed[ALLEGRO_KEY_RIGHT]){
    editor_buttons[button_undo].toggleVisibility();
    editor_buttons[button_clear].toggleStatus();
    editor_buttons[button_save].toggleStatus();
    editor_buttons[button_save_as].toggleStatus();
    editor_buttons[button_load].toggleStatus();
    editor_buttons[button_play].toggleStatus();
    editor_buttons[button_grid].toggleStatus();


    if(editor_buttons[button_hide_right].getText()==">"){
      editor_buttons[button_hide_right].setPosition( 994, 728);
      editor_buttons[button_hide_right].setText("<");

      editor_buttons[button_hide_right].setTransparency(150);
    }else{
      editor_buttons[button_hide_right].setPosition( 566, 728);
      editor_buttons[button_hide_right].setText(">");
      editor_buttons[button_hide_right].setTransparency(255);



    }

  }


  // Add tile
  if(tile_type != 4){
    if( mouseListener::mouse_button & 1
      && !box_at_with_type(0,mouseListener::mouse_x, mouseListener::mouse_y)
      && !box_at_with_type(1,mouseListener::mouse_x, mouseListener::mouse_y)
      && !box_at_with_type(2,mouseListener::mouse_x, mouseListener::mouse_y)
      && !box_at_with_type(3,mouseListener::mouse_x, mouseListener::mouse_y)
      && ((!over_button && gui_mode) || !gui_mode)){
      editor_box newBox;
      newBox.x = mouseListener::mouse_x - mouseListener::mouse_x % 32;
      newBox.y = mouseListener::mouse_y - mouseListener::mouse_y % 32;
      newBox.x_str = tools::toString( float(newBox.x + 16) / 20.0f);
      newBox.y_str = tools::toString( -1 * float(newBox.y + 16) / 20.0f);
      newBox.type = tile_type;
      for( int i = 0; i < 4; i++)
        newBox.orientation[i] = 0;

      if( tile_type == 0)
        newBox.bodyType = "Dynamic";
      else if( tile_type == 1)
        newBox.bodyType = "Static";
      else if( tile_type == 2)
        newBox.bodyType = "Character";
      else if( tile_type == 3)
        newBox.bodyType = "Finish";

      editorBoxes.push_back( newBox);

      // Calculate orientation of boxes
      calculate_orientation_global();
    }
  }
  //Drag n drop madness
  if( tile_type == 4 && !dialog_open){
    if( mouseListener::mouse_pressed & 1 && !box_at_with_type(0,mouseListener::mouse_x, mouseListener::mouse_y) && ((!over_button && gui_mode) || !gui_mode)){
      is_dragging_box = true;
      box_1_x = mouseListener::mouse_x - mouseListener::mouse_x % 32;
      box_1_y = mouseListener::mouse_y - mouseListener::mouse_y % 32;
    }
    if(mouseListener::mouse_released & 1){
      is_dragging_box = false;

      if(!over_button && gui_mode && (box_2_x - box_1_x) != 0 && (box_2_x - box_1_x) != 0){
        // Backwards dragged box correction, Box2D chokes on negative widths/heights
        if(box_2_x<box_1_x){
          int holder_value = box_2_x;
          box_2_x=box_1_x;
          box_1_x=holder_value;
        }
        if(box_2_y<box_1_y){
          int holder_value = box_2_y;
          box_2_y=box_1_y;
          box_1_y=holder_value;
        }

        editor_box newBox;
        newBox.width = (box_2_x - box_1_x);
        newBox.height = (box_2_y - box_1_y);
        newBox.x = box_1_x;
        newBox.y = box_1_y;
        newBox.x_str = tools::toString((float(newBox.x) + newBox.width/2) / 20.0f);
        newBox.y_str = tools::toString( -1 * (float(newBox.y) + newBox.height/2)/ 20.0f);
        newBox.type = 4;
        newBox.width_str = tools::toString( float(((box_2_x  - box_1_x))/ 20.0f));
        newBox.height_str = tools::toString( float(((box_2_y - box_1_y))/ 20.0f));
        newBox.bodyType = "Collision";

        editorBoxes.push_back( newBox);

        std::cout << "Made a nice collisionbox\n";
      }
    }
    if(mouseListener::mouse_button & 1){
      box_2_x = (mouseListener::mouse_x - mouseListener::mouse_x % 32) + 32;
      box_2_y = (mouseListener::mouse_y - mouseListener::mouse_y % 32) + 32;
    }
  }

  // Remove tile
  if( mouseListener::mouse_button & 2){
    for( unsigned int i = 0; i < editorBoxes.size(); i ++){
      if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, (float)mouseListener::mouse_x, (float)mouseListener::mouse_x , editorBoxes.at(i).y, editorBoxes.at(i).y + 32, (float)mouseListener::mouse_y, (float)mouseListener::mouse_y )){
        editorBoxes.erase( editorBoxes.begin() + i);
        calculate_orientation_global();
      }
    }
  }
}

// Calculate all the orientations of blocks
void editor::calculate_orientation_global(){
  // Calc boxes
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    if( editorBoxes.at(i).type == 0 || editorBoxes.at(i).type == 1){
      // Scroll through all 4 parts
      for( int t = 0; t < 4; t++){
        // Offsets from subtile
        int off_x = (t == 1 || t == 3) ? 16: 0;
        int off_y = (t >= 2) ? 16: 0;

        // Options
        std::vector<int> options;
        for( int k = 0; k < 13; k++)
          options.push_back(k);

        int box_type = editorBoxes.at(i).type;

        // NORTH
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x     , editorBoxes.at(i).y + off_y - 16)){
          options.erase(std::remove(options.begin(), options.end(), 0), options.end());
          options.erase(std::remove(options.begin(), options.end(), 1), options.end());
          options.erase(std::remove(options.begin(), options.end(), 2), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 3), options.end());
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
          options.erase(std::remove(options.begin(), options.end(), 5), options.end());
        }
        // NORTH EAST
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x + 16, editorBoxes.at(i).y + off_y - 16)){
          options.erase(std::remove(options.begin(), options.end(), 11), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
        }
        // EAST
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x + 16, editorBoxes.at(i).y + off_y     )){
          options.erase(std::remove(options.begin(), options.end(), 2), options.end());
          options.erase(std::remove(options.begin(), options.end(), 5), options.end());
          options.erase(std::remove(options.begin(), options.end(), 8), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 1), options.end());
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
          options.erase(std::remove(options.begin(), options.end(), 7), options.end());
        }
        // SOUTH EAST
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x + 16, editorBoxes.at(i).y + off_y + 16)){
          options.erase(std::remove(options.begin(), options.end(), 9), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
        }
        // SOUTH
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x     , editorBoxes.at(i).y + off_y + 16)){
          options.erase(std::remove(options.begin(), options.end(), 6), options.end());
          options.erase(std::remove(options.begin(), options.end(), 7), options.end());
          options.erase(std::remove(options.begin(), options.end(), 8), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 3), options.end());
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
          options.erase(std::remove(options.begin(), options.end(), 5), options.end());
        }
        // SOUTH WEST
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x - 16, editorBoxes.at(i).y + off_y + 16)){
          options.erase(std::remove(options.begin(), options.end(), 10), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
        }
        // WEST
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x - 16, editorBoxes.at(i).y + off_y     )){
          options.erase(std::remove(options.begin(), options.end(), 0), options.end());
          options.erase(std::remove(options.begin(), options.end(), 3), options.end());
          options.erase(std::remove(options.begin(), options.end(), 6), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 1), options.end());
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
          options.erase(std::remove(options.begin(), options.end(), 7), options.end());
        }
        // NORTH WEST
        if( box_at_with_type(box_type, editorBoxes.at(i).x + off_x - 16, editorBoxes.at(i).y + off_y - 16)){
          options.erase(std::remove(options.begin(), options.end(), 12), options.end());
        }
        else{
          options.erase(std::remove(options.begin(), options.end(), 4), options.end());
        }

        if( options.size() > 0)
          editorBoxes.at(i).orientation[t] = options.at(0);
        else
          editorBoxes.at(i).orientation[t] = 0;
      }
    }
  }
}

// Draw to screen
void editor::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));

  // Grid
  if( grid_on){
    for( int i = 0; i < 1024; i += 32){
      al_draw_line( i, 0, i, 768, al_map_rgb( 0, 0, 0), 1);
    }

    for( int i = 0; i < 768; i += 32){
      al_draw_line( 0, i, 1024, i, al_map_rgb( 0, 0, 0), 1);
    }
  }

  // Draw tiles
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    if( editorBoxes.at(i).type == 0 || editorBoxes.at(i).type == 1){
      if( editorBoxes.at(i).type == 0)
        al_draw_filled_rectangle( editorBoxes.at(i).x + 0, editorBoxes.at(i).y + 0, editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32, al_map_rgb(0, 255, 0));

      // Scroll through all 4 parts
      for( int t = 0; t < 4; t++){
        // Offsets from subtile
        int off_x = (t == 1 || t == 3) ? 16: 0;
        int off_y = (t >= 2) ? 16: 0;

        if( editorBoxes.at(i).orientation[t] >= 0 && editorBoxes.at(i).orientation[t] < 16)
          al_draw_bitmap( tiles[editorBoxes.at(i).type][editorBoxes.at(i).orientation[t]], editorBoxes.at(i).x + off_x, editorBoxes.at(i).y + off_y, 0);
      }
    }
    else if( editorBoxes.at(i).type == 2)
      al_draw_bitmap( tiles[2][0], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
    else if( editorBoxes.at(i).type == 3)
      al_draw_bitmap( tiles[3][0], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
  }

  //Gotta draw the tranparent boxes in front
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
      if( editorBoxes.at(i).type == 4)
        al_draw_filled_rectangle( editorBoxes.at(i).x,editorBoxes.at(i).y,editorBoxes.at(i).x+editorBoxes.at(i).width,editorBoxes.at(i).y+editorBoxes.at(i).height, al_map_rgba(0, 255, 0,50));

  }



  if(is_dragging_box){
    al_draw_filled_rectangle( box_1_x,box_1_y,box_2_x,box_2_y, al_map_rgba(0, 255, 0,50));

  }

  // Tile type
  if( tile_type == 0)
    al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 10, 30, 0, "Type: Dynamic");
  if( tile_type == 1)
    al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 10, 30, 0, "Type: Static");
  if( tile_type == 2)
    al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 10, 30, 0, "Type: Character spawn");
  if( tile_type == 3)
    al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 10, 30, 0, "Type: Endgame goat");
  if( tile_type == 4)
    al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 10, 30, 0, "Type: Collision Box");


  // Current map opened
  al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 10, 10, 0, "File: %s", file_name);

  // Draw buttons
  for( int i = 0; i < BUTTON_COUNT; i++){
    if(gui_mode)
      editor_buttons[i].draw();
  }
}


// Check if box is at location
bool editor::box_at_with_type( int newType, int x, int y){
  for( unsigned int i = 0; i < editorBoxes.size(); i ++)
    if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, (float)x, (float)x + 1, editorBoxes.at(i).y, editorBoxes.at(i).y + 32, (float)y, (float)y + 1) && editorBoxes.at(i).type == newType)
      return true;
  return false;
}

// Check if box is at location
bool editor::box_at(int x, int y){
  for( unsigned int i = 0; i < editorBoxes.size(); i ++)
    if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, (float)x, (float)x + 1, editorBoxes.at(i).y, editorBoxes.at(i).y + 32, (float)y, (float)y + 1))
      return true;
  return false;
}

// Load map from xml
bool editor::load_map( std::string mapName){
  // Doc
  rapidxml::xml_document<> doc;

  // Make an xml object
  std::ifstream theFile( mapName);
  std::vector<char> xml_buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  rapidxml::xml_node<> * root_node;
  root_node = doc.first_node("data");

  // Iterate over the nodes
  for (rapidxml::xml_node<> * object_node = root_node -> first_node("Object"); object_node; object_node = object_node -> next_sibling()){
    std::string type = "";
    std::string x = "";
    std::string y = "";
    std::string width = "0";
    std::string height = "0";
    std::string bodytype = "static";
    std::string orientation = "0 0 0 0";

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




    editor_box newBox;

    newBox.x_str = x;
    newBox.y_str = y;
    newBox.bodyType = bodytype;

    // Idek dude but it works
    if(newBox.bodyType == "Collision"){

      newBox.width = (tools::string_to_float(width) * 20.0f);
      newBox.height = (tools::string_to_float(height) * 20.0f);
      newBox.x = (tools::string_to_float(x) - tools::string_to_float(width)/2)*20.0f;
      // This guy is positive because we make it negative later
      newBox.y = (tools::string_to_float(y) + tools::string_to_float(height)/2)*-20.0f;

    }
    if(newBox.bodyType != "Collision"){
      newBox.width = (tools::string_to_float(width) * 20.0f) - 16.0f;
      newBox.height = (tools::string_to_float(height) * 20.0f) - 16.0f;
      newBox.x = (tools::string_to_float(x) * 20.0f) - 16.0f;
      newBox.y = (tools::string_to_float(y) * -20.0f) - 16.0f;
    }

    newBox.height_str = height;
    newBox.width_str = width;


    // Correct orientation format
    std::vector<std::string> splits = tools::split_string( orientation, ' ');
    if( splits.size() == 4){
      for( int k = 0; k < 4; k++)
        newBox.orientation[k] = (tools::convertStringToInt(splits.at(k)));
    }
    // Maybe we can salvage it?
    else if( splits.size() > 0){
      for( int k = 0; k < 4; k++)
        newBox.orientation[k] = (tools::convertStringToInt(splits.at(0)));
    }
    // All hope is lost!
    else{
      return false;
    }

    // Body
    if( newBox.bodyType == "Dynamic")
      newBox.type = 0;
    else if( newBox.bodyType == "Static")
      newBox.type = 1;
    else if( newBox.bodyType == "Character")
      newBox.type = 2;
    else if( newBox.bodyType == "Finish")
      newBox.type = 3;
    else if( newBox.bodyType == "Collision")
      newBox.type = 4;
    else
      newBox.type = 0;

    // Add box
    editorBoxes.push_back( newBox);
  }

  // Success
  return true;
}

// Save map to xml
bool editor::save_map( std::string mapName){

  //NSFW haxx to prevent goat loading before player

  for(int i=0;i<editorBoxes.size();i++){
    if(editorBoxes[i].type==GOAT){
    editor_box newBox = editorBoxes[i];
    editorBoxes.erase(editorBoxes.begin()+i);
    editorBoxes.insert(editorBoxes.begin(),newBox);
    }
  }


  // Write xml file
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl -> append_attribute( doc.allocate_attribute("version", "1.0"));
  decl -> append_attribute( doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  rapidxml::xml_node<>* root_node = doc.allocate_node( rapidxml::node_element, "data");
  doc.append_node(root_node);

  // Tiles
  for( unsigned int i = 0; i < editorBoxes.size(); i++){
    // Object
    char *node_name = doc.allocate_string("Object");
    rapidxml::xml_node<>* object_node = doc.allocate_node( rapidxml::node_element, node_name);

    std::string xml_type = "Tile";
    std::string output_orientation = tools::toString(editorBoxes.at(i).orientation[0]) + " " + tools::toString(editorBoxes.at(i).orientation[1]) + " " +
                                     tools::toString(editorBoxes.at(i).orientation[2]) + " " + tools::toString(editorBoxes.at(i).orientation[3]);
    char *output_orientation_char = doc.allocate_string(output_orientation.c_str());

    if( editorBoxes.at(i).type == 2)
      xml_type = "Character";
    else if( editorBoxes.at(i).type == 3)
      xml_type = "Finish";

    object_node -> append_attribute( doc.allocate_attribute("type", doc.allocate_string(xml_type.c_str())));
    root_node -> append_node( object_node);

    // Save data
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "x", editorBoxes.at(i).x_str.c_str()));
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "y", editorBoxes.at(i).y_str.c_str()));
    if(editorBoxes.at(i).bodyType=="Dynamic" || editorBoxes.at(i).bodyType=="Static")
      object_node -> append_node( doc.allocate_node( rapidxml::node_element, "orientation", output_orientation_char));
    if(editorBoxes.at(i).bodyType=="Collision"){


      object_node -> append_node( doc.allocate_node( rapidxml::node_element, "width",editorBoxes.at(i).width_str.c_str()));
      object_node -> append_node( doc.allocate_node( rapidxml::node_element, "height",editorBoxes.at(i).height_str.c_str()));

     // object_node -> append_node( doc.allocate_node( rapidxml::node_element, "height", tools::convertIntToString(editorBoxes.at(i).height).c_str()));

    }


    // Write this last for consistency of placement in the xml (hint: always last element)
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "bodytype", editorBoxes.at(i).bodyType.c_str()));

  }

  // Save to file
  std::ofstream file_stored(mapName);
  file_stored << doc;
  file_stored.close();
  doc.clear();

  // Success
  return true;
}
