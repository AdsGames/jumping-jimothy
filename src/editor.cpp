#include "editor.h"

editor::editor(){

}

editor::~editor(){

}


// Override parent
void editor::update(){

}


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
}
