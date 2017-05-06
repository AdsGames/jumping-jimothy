#include "editor.h"

editor::editor(){
  // Load box image
  image_box = tools::load_bitmap_ex( "tiles.png");

  for( int i = 0; i < 4; i++){
    for( int t = 0; t < 4; t++){
      edges[i + t*4] = al_create_sub_bitmap( image_box, i * 32, t * 32, 32, 32);
    }
  }

  srand(time(NULL));
}

editor::~editor(){

}


// Override parent
void editor::update(){
  if( mouseListener::mouse_pressed & 1){
    editor_box newBox;
    newBox.x = mouseListener::mouse_x - mouseListener::mouse_x % 32;
    newBox.y = mouseListener::mouse_y - mouseListener::mouse_y % 32;
    newBox.x_str = tools::toString( newBox.x);
    newBox.y_str = tools::toString( newBox.y);
    newBox.bodyType = "static";
    editorBoxes.push_back( newBox);
  }
  if( mouseListener::mouse_pressed & 2){
    for( unsigned int i = 0; i < editorBoxes.size(); i ++){
      if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, mouseListener::mouse_x, mouseListener::mouse_x , editorBoxes.at(i).y, editorBoxes.at(i).y + 32, mouseListener::mouse_y, mouseListener::mouse_y )){
        editorBoxes.erase( editorBoxes.begin() + i);
      }
    }
  }

  // Save map
  if( keyListener::keyPressed[ALLEGRO_KEY_S]){
    save_map("seetfile.xml");
  }
}

// Draw to screen
void editor::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));
x_str
  // Grid
  for( int i = 0; i < 1024; i += 32){
    al_draw_line( i, 0, i, 768, al_map_rgb( 0, 0, 0), 1);
  }

  for( int i = 0; i < 768; i += 32){
    al_draw_line( 0, i, 1024, i, al_map_rgb( 0, 0, 0), 1);
  }srand(time(NULL));

  // Boxes
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    int type = 0;
    // LEFT
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
    al_draw_bitmap( edges[type], editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
  }
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
  using namespace rapidxml;

  // Write xml file
  xml_document<> doc;
  xml_node<>* decl = doc.allocate_node(node_declaration);
  decl->append_attribute(doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  xml_node<>* root_node = doc.allocate_node(node_element, "data");
  doc.append_node(root_node);

  // Tiles
  for( unsigned int i = 0; i < editorBoxes.size(); i++){
    // Object
    char *node_name = doc.allocate_string("Object");
    xml_node<>* object_node = doc.allocate_node( node_element, node_name);
    object_node -> append_attribute( doc.allocate_attribute("type", "tile"));
    root_node -> append_node( object_node);

    // X/Y/Bodytype
    object_node -> append_node( doc.allocate_node(node_element, "x", (editorBoxes.at(i).x_str).c_str()));
    object_node -> append_node( doc.allocate_node(node_element, "y", (editorBoxes.at(i).y_str).c_str()));
    object_node -> append_node( doc.allocate_node(node_element, "bodytype", editorBoxes.at(i).bodyType.c_str()));
  }

  // Convert doc to string if needed
  std::cout << doc;

  // Save to file
  std::ofstream file_stored(mapName);
  file_stored << doc;
  file_stored.close();
  doc.clear();
}
