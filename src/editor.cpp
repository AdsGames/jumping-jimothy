#include "editor.h"

editor::editor(){

  level_number=1;
  // Load box image
  image_box[0] = tools::load_bitmap_ex( "DynamicBlock.png");
  image_box[1] = tools::load_bitmap_ex( "StaticBlock.png");
  image_box[2] = tools::load_bitmap_ex( "character.png");
  image_box[3] = tools::load_bitmap_ex( "DisgoatSpriteMap.png");

  for( int i = 0; i < 4; i++){
    for( int t = 0; t < 12; t++){
      tiles[0][i + t*4] = al_create_sub_bitmap( image_box[0], i * 32, t * 32, 32, 32);
      tiles[1][i + t*4] = al_create_sub_bitmap( image_box[1], i * 32, t * 32, 32, 32);
      tiles[2][i + t*4] = al_create_sub_bitmap( image_box[2], 0, 0, 32, 32);
    }
  }

  for( int i = 0; i < 4; i++){
    for( int t = 0; t < 4; t++){
        tiles[3][i + t*4] = al_create_sub_bitmap( image_box[3], i * 32, t * 64, 32, 64);
    }
  }

  tile_type = 0;

  srand(time(NULL));

  al_init_ttf_addon();
  edit_font = al_load_ttf_font("pixelart.ttf",14,0);

  if (!edit_font)
    tools::abort_on_error("Could not load 'pixelart.ttf'.\n");

  grid_on = false;
}

editor::~editor(){

}


// Override parent
void editor::update(){
  if( mouseListener::mouse_button & 1 && !box_at(mouseListener::mouse_x, mouseListener::mouse_y)){
    editor_box newBox;
    newBox.x = mouseListener::mouse_x - mouseListener::mouse_x % 32;
    newBox.y = mouseListener::mouse_y - mouseListener::mouse_y % 32;
    newBox.x_str = tools::toString( float(newBox.x + 16) / 20.0f);
    newBox.y_str = tools::toString( -1 * float(newBox.y + 16) / 20.0f);
    newBox.type = tile_type;
    newBox.orientation = 12;
    newBox.orientation_str = "12";

    if( tile_type == 0)
      newBox.bodyType = "Dynamic";
    else if( tile_type == 1)
      newBox.bodyType = "Static";
    else if( tile_type == 2)
      newBox.bodyType = "Character";
    else if( tile_type == 3)
      newBox.bodyType = "Finish";

    editorBoxes.push_back( newBox);
  }
  if( mouseListener::mouse_button & 2){
    for( unsigned int i = 0; i < editorBoxes.size(); i ++){
      if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, mouseListener::mouse_x, mouseListener::mouse_x , editorBoxes.at(i).y, editorBoxes.at(i).y + 32, mouseListener::mouse_y, mouseListener::mouse_y )){
        editorBoxes.erase( editorBoxes.begin() + i);
      }
    }
  }
  for(int i=0; i<10; i++){
    if( keyListener::keyPressed[i+27])
    level_number = i;
  }
  // Tile types
  if( keyListener::keyPressed[ALLEGRO_KEY_Q])
    tile_type = 0;
  if( keyListener::keyPressed[ALLEGRO_KEY_W])
    tile_type = 1;
  if( keyListener::keyPressed[ALLEGRO_KEY_E])
    tile_type = 2;
  if( keyListener::keyPressed[ALLEGRO_KEY_R])
    tile_type = 3;

 // if( keyListener::keyPressed[ALLEGRO_KEY_T])
    calculate_orientation_global();

  // Grid toggle
  if( keyListener::keyPressed[ALLEGRO_KEY_G])
    grid_on = !grid_on;

  if( keyListener::keyPressed[ALLEGRO_KEY_R]){


  }

  // Load map
  if( keyListener::keyPressed[ALLEGRO_KEY_L]){
    editorBoxes.clear();
    load_map("data/level_"+tools::convertIntToString(level_number)+".xml");
    std::string path = "data/level_"+tools::convertIntToString(level_number)+".xml";
    al_show_native_message_box( nullptr, "Loaded map", "We've loaded a map from: ",path.c_str(), nullptr, 0);

  }

  // Save map
  if( keyListener::keyPressed[ALLEGRO_KEY_S]){
    save_map("data/level_"+tools::convertIntToString(level_number)+".xml");
    std::string path = "data/level_"+tools::convertIntToString(level_number)+".xml";
    al_show_native_message_box( nullptr, "Saved map", "We've saved a map to: ",path.c_str(), nullptr, 0);

  }
}
void editor::calculate_orientation_global(){
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    int orientation = calculate_orientation(i);
    editorBoxes.at(i).orientation = orientation;
    editorBoxes.at(i).orientation_str = tools::convertIntToString(orientation);
  }



}

int editor::calculate_orientation(int i){
  // Boxes
    int orientation = 0;
    if( editorBoxes.at(i).type == 0 || editorBoxes.at(i).type == 1){

      // LEFT
      if( box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y))
        orientation += 1;
      // RIGHT
      if( box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y))
        orientation += 2;
      // UP(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32)
      if( box_at( editorBoxes.at(i).x, editorBoxes.at(i).y - 32))
        orientation += 4;
      // DOWN
      if( box_at( editorBoxes.at(i).x, editorBoxes.at(i).y + 32))
        orientation += 8;

      // EMPTY TOP LEFT
      if(orientation == 5 && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
          orientation = 21;

      // EMPTY TOP RIGHT
      if(orientation == 6 && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32)))
          orientation = 22;

      // EMPTY BOTTOM LEFT
      if(orientation == 9 && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
          orientation = 25;

      // EMPTY BOTTOM RIGHT
      if(orientation == 10 && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32)))
          orientation = 26;

      // CLOSED ON RIGHT
      if(orientation == 13) {
          // EMPTY TOP LEFT
          if(!(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32))
             && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
              orientation = 16;
          // EMPTY BOTTOM LEFT
          else if((box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32))
             && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
             orientation = 17;
          //BOTH EMPTY
          else if(!(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32))
             && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
              orientation = 29;
      }

      //CLOSED ON LEFT
      if(orientation == 14) {
          // EMPTY TOP RIGHT
          if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
             && (box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32)))
              orientation = 18;
          // EMPTY BOTTOM RIGHT
          else if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
             && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32)))
             orientation = 19;
          // BOTH EMPTY
          else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
             && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32)))
              orientation = 30;
      }

      //CLOSED ON BOTTOM
      if(orientation == 7) {
          // EMPTY TOP RIGHT
          if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
             && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
              orientation = 31;
          // EMPTY TOP LEFT
          else if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
             && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
             orientation = 20;
          // BOTH EMPTY
          else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
             && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
              orientation = 23;
      }

      //CLOSED ON TOP
      if(orientation == 11) {
          // EMPTY BOTTOM RIGHT
          if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
             && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
              orientation = 24;
          // EMPTY BOTTOM LEFT
          else if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
             && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
             orientation = 28;
          // BOTH EMPTY
          else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
             && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
              orientation = 27;
      }

        // COVERED ON ALL SIDES
        if(orientation == 15)
        {

            // EMPTY AT TOP LEFT
            if(!(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 32;
            // EMPTY AT TOP RIGHT
            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32)))
                orientation = 33;
            // EMPTY AT BOTTOM LEFT
            else if(!(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32)))
                orientation = 34;
            // EMPTY AT BOTTOM RIGHT
            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32)))
                orientation = 35;

            if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 36;

            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 37;

            else if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 38;

            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 39;

            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 40;
            else if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 41;

            // BLOCK AT TOP LEFT
            if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 42;
            // BLOCK AT TOP RIGHT
            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 43;
            // BLOCK AT BOTTOM LEFT
            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && (box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 44;
            // BLOCK AT BOTTOM RIGHT
            else if((box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 45;
            else if(!(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y + 32))
            && !(box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y - 32))
            && !(box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y - 32)))
                orientation = 46;
        }

  }
  return orientation;
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



  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    if( editorBoxes.at(i).type == 0 || editorBoxes.at(i).type == 1){

    al_draw_bitmap( tiles[editorBoxes.at(i).type][editorBoxes.at(i).orientation], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
    }
    else if( editorBoxes.at(i).type == 2)
      al_draw_bitmap( tiles[editorBoxes.at(i).type][0], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
    else if( editorBoxes.at(i).type == 3)
      al_draw_bitmap( tiles[editorBoxes.at(i).type][0], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
  }

  // Tile type
  if(tile_type==0)al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 0, 0, 0, "Type: Dynamic");
  if(tile_type==1)al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 0, 0, 0, "Type: Static");
  if(tile_type==2)al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 0, 0, 0, "Type: Character spawn");
  if(tile_type==3)al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 0, 0, 0, "Type: Endgame goat");

  al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 0, 20, 0, "Level: level_%i.xml",level_number);


}


// Check if box is at location
bool editor::box_at( int x, int y){
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, x, x + 1, editorBoxes.at(i).y, editorBoxes.at(i).y + 32, y, y + 1)){
      return true;
    }
  }
  return false;
}

// Load map from xml
void editor::load_map( std::string mapName){
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
    std::string orientation = "10";
    std::string bodytype = "totally not static";
    std::string vel_x = "0";
    std::string vel_y = "0";

    type = object_node -> first_attribute("type") -> value();

    int i = 0;
    for( rapidxml::xml_node<> * map_item = object_node->first_node("x"); map_item; map_item = map_item->next_sibling()){
      switch( i ){
        case 0:
          x = map_item -> value();
          break;
        case 1:
          y = map_item -> value();
          break;
        case 2:
          bodytype = map_item -> value();
          break;
        case 3:
          orientation = map_item -> value();
          break;
        case 4:
          vel_x = map_item -> value();
          break;
        case 5:
          vel_y = map_item -> value();
          break;
        default:
          break;
      }
      i++;
    }

    editor_box newBox;
    newBox.x = (tools::string_to_float(x) * 20) - 16;
    newBox.y = (tools::string_to_float(y) * -20) - 16;
    newBox.x_str = tools::toString( float(newBox.x + 16) / 20.0f);
    newBox.y_str = tools::toString( -1 * float(newBox.y + 16) / 20.0f);
    newBox.orientation = (tools::convertStringToInt(orientation));
    newBox.orientation_str = orientation;


    newBox.bodyType = bodytype;

    if( newBox.bodyType == "Dynamic")
      newBox.type = 0;
    else if( newBox.bodyType == "Static")
      newBox.type = 1;
    else if( newBox.bodyType == "Character")
      newBox.type = 2;
    else if( newBox.bodyType == "Finish")
      newBox.type = 3;

    editorBoxes.push_back( newBox);
  }
}

// Save map to xml
void editor::save_map( std::string mapName){

  // Write xml file
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  rapidxml::xml_node<>* root_node = doc.allocate_node( rapidxml::node_element, "data");
  doc.append_node(root_node);

  // Tiles
  for( unsigned int i = 0; i < editorBoxes.size(); i++){
    // Object
    char *node_name = doc.allocate_string("Object");
    rapidxml::xml_node<>* object_node = doc.allocate_node( rapidxml::node_element, node_name);

    std::string xml_type = "Tile";

    if( editorBoxes.at(i).type == 2)
      xml_type = "Character";
    else if( editorBoxes.at(i).type == 3)
      xml_type = "Finish";

    object_node -> append_attribute( doc.allocate_attribute("type", doc.allocate_string(xml_type.c_str())));
    root_node -> append_node( object_node);

    // X/Y/Bodytype
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "x", (editorBoxes.at(i).x_str).c_str()));
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "y", (editorBoxes.at(i).y_str).c_str()));
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "bodytype", editorBoxes.at(i).bodyType.c_str()));
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "orientation",editorBoxes.at(i).orientation_str.c_str() ));

  }

  // Save to file
  std::ofstream file_stored(mapName);
  file_stored << doc;
  file_stored.close();
  doc.clear();
}
