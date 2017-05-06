#include "editor.h"

editor::editor(){
  // Load box image
  image_box[0] = tools::load_bitmap_ex( "DynamicBlock.png");
  image_box[1] = tools::load_bitmap_ex( "StaticBlock.png");
  player = tools::load_bitmap_ex( "character.png");

  for( int i = 0; i < 4; i++){
    for( int t = 0; t < 4; t++){
      tiles[0][i + t*4] = al_create_sub_bitmap( image_box[0]  , i * 32, t * 32, 32, 32);
      tiles[1][i + t*4] = al_create_sub_bitmap( image_box[1], i * 32, t * 32, 32, 32);
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

    if( tile_type == 1)
      newBox.bodyType = "Static";
    else
      newBox.bodyType = "Dynamic";

    editorBoxes.push_back( newBox);
  }
  if( mouseListener::mouse_button & 2){
    for( unsigned int i = 0; i < editorBoxes.size(); i ++){
      if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, mouseListener::mouse_x, mouseListener::mouse_x , editorBoxes.at(i).y, editorBoxes.at(i).y + 32, mouseListener::mouse_y, mouseListener::mouse_y )){
        editorBoxes.erase( editorBoxes.begin() + i);
      }
    }
  }

  // Tile types
  if( keyListener::keyPressed[ALLEGRO_KEY_1])
    tile_type = 0;
  if( keyListener::keyPressed[ALLEGRO_KEY_2])
    tile_type = 1;
  if( keyListener::keyPressed[ALLEGRO_KEY_3])
    tile_type = 2;

  // Grid toggle
  if( keyListener::keyPressed[ALLEGRO_KEY_G])
    grid_on = !grid_on;

  // Save map
  if( keyListener::keyPressed[ALLEGRO_KEY_S]){
    save_map("data/level.xml");
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

  // Boxes
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    // LEFT
    int type = 0;
    if( box_at( editorBoxes.at(i).x - 32, editorBoxes.at(i).y))
      type += 1;
    // RIGHT
    if( box_at( editorBoxes.at(i).x + 32, editorBoxes.at(i).y))
      type += 2;
    // UP
    if( box_at( editorBoxes.at(i).x, editorBoxes.at(i).y - 32))
      type += 4;
    // DOWN
    if( box_at( editorBoxes.at(i).x, editorBoxes.at(i).y + 32))
      type += 8;
    al_draw_bitmap( tiles[editorBoxes.at(i).type][type], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
  }

  // Tile type
  al_draw_textf( edit_font, al_map_rgb( 0, 0, 0), 0, 0, 0, "Type %i", tile_type);
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
    object_node -> append_attribute( doc.allocate_attribute("type", "Tile"));
    root_node -> append_node( object_node);

    // X/Y/Bodytype
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "x", (editorBoxes.at(i).x_str).c_str()));
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "y", (editorBoxes.at(i).y_str).c_str()));
    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "bodytype", editorBoxes.at(i).bodyType.c_str()));
  }

  // Save to file
  std::ofstream file_stored(mapName);
  file_stored << doc;
  file_stored.close();
  doc.clear();
}
