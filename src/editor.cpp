#include "editor.h"

editor::editor(){
  // Load box image
  image_box = tools::load_bitmap_ex( "box.png");
}

editor::~editor(){

}


// Override parent
void editor::update(){
  if( mouseListener::mouse_pressed & 1){
    editor_box newBox;
    newBox.x = mouseListener::mouse_x - mouseListener::mouse_x % 32;
    newBox.y = mouseListener::mouse_y - mouseListener::mouse_y % 32;;
    editorBoxes.push_back( newBox);
  }
  if( mouseListener::mouse_pressed & 2){
    for( unsigned int i = 0; i < editorBoxes.size(); i ++){
      if( tools::collision( editorBoxes.at(i).x, editorBoxes.at(i).x + 32, mouseListener::mouse_x, mouseListener::mouse_x , editorBoxes.at(i).y, editorBoxes.at(i).y + 32, mouseListener::mouse_y, mouseListener::mouse_y )){
        editorBoxes.erase( editorBoxes.begin() + i);
      }
    }
  }
}

// Draw to screen
void editor::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));

  // Grid
  for( int i = 0; i < 1024; i += 32){
    al_draw_line( i, 0, i, 768, al_map_rgb( 0, 0, 0), 1);
  }

  for( int i = 0; i < 768; i += 32){
    al_draw_line( 0, i, 1024, i, al_map_rgb( 0, 0, 0), 1);
  }

  // Boxes
  for( unsigned int i = 0; i < editorBoxes.size(); i ++){
    al_draw_bitmap( image_box, editorBoxes.at(i).x, editorBoxes.at(i).y, 0);
  }
}
