#include "LevelSelect.h"

bool LevelSelect::completed_level_list[16]= {};

LevelSelect::LevelSelect()

{

  //Doc
  rapidxml::xml_document<> doc;

//   Make an xml object
  std::ifstream theFile("data/level_data.xml");
  std::vector<char> xml_buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

//   Find our root node
  rapidxml::xml_node<> * root_node;
  root_node = doc.first_node("data");

   //Iterate over the nodes
  for (rapidxml::xml_node<> * object_node = root_node -> first_node("level"); object_node; object_node = object_node -> next_sibling()){

    int levelNumber = atoi(object_node -> first_attribute("number") -> value());

    std::string newStatus ="";
   //  Load data
    if( object_node -> first_node("status") != 0)
      newStatus = object_node -> first_node("status") -> value();

    completed_level_list[levelNumber] = (newStatus=="complete");

  }

  levelselect_font = al_load_ttf_font( "fonts/munro.ttf", 24, 0);

  levelselect_font_large = al_load_ttf_font( "fonts/munro.ttf", 48, 0);

  cursor = tools::load_bitmap_ex("images/cursor.png");

  levelSelectUI = UIHandler();

  levelSelectUI.addElement(new UIElement(20+100,20,"Select a level",levelselect_font_large));
  levelSelectUI.getElementByText("Select a level") -> setVisibleBackground(false);
  levelSelectUI.getElementByText("Select a level") -> setTextColour(al_map_rgb(255,255,255));


  createLevelButton(175,110,1);
  createLevelButton(320,110,2);
  createLevelButton(465,110,3);
  createLevelButton(610,110,4);
  createLevelButton(755,110,5);
  createLevelButton(125,170,6);
  createLevelButton(270,170,7);
  createLevelButton(415,170,8);
  createLevelButton(560,170,9);
  createLevelButton(705,170,10);
  createLevelButton(175,230,11);
  createLevelButton(320,230,12);
  createLevelButton(465,230,13);

  levelSelectUI.addElement(new Button(837, 10, "Reset Save Game", levelselect_font));





}

LevelSelect::~LevelSelect()
{
  //dtor
}

void LevelSelect::createLevelButton(int newX, int newY, int newLevelNumber){

  levelSelectUI.addElement(new Button(newX, newY, "Level "+ tools::toString(newLevelNumber), levelselect_font));
  levelSelectUI.getElementByText("Level "+ tools::toString(newLevelNumber)) -> setHeight(25);
  levelSelectUI.getElementByText("Level "+ tools::toString(newLevelNumber)) -> setWidth(100);
  levelSelectUI.getElementByText("Level "+ tools::toString(newLevelNumber)) -> setJustification(1);
  if(completed_level_list[newLevelNumber])levelSelectUI.getElementByText("Level "+ tools::toString(newLevelNumber)) -> setBackgroundColour(al_map_rgb(0,255,0));



}

void LevelSelect::setLevelComplete(int newLevelNumber){

  completed_level_list[newLevelNumber]=true;
}

void LevelSelect::writeLevelData(){
  // Write xml file
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl -> append_attribute( doc.allocate_attribute("version", "1.0"));
  decl -> append_attribute( doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  rapidxml::xml_node<>* root_node = doc.allocate_node( rapidxml::node_element, "data");
  doc.append_node(root_node);

  // Tiles
  for( unsigned int i = 0; i < 15; i++){

    char *node_name = doc.allocate_string("level");
    rapidxml::xml_node<>* object_node = doc.allocate_node( rapidxml::node_element, node_name);



    object_node -> append_attribute( doc.allocate_attribute("number", doc.allocate_string(tools::toString(i).c_str())));
    root_node -> append_node( object_node);

    // Save data

    std::string newStatus="incomplete";


    if(completed_level_list[i])
      newStatus="complete";


    char *newStatusChar = doc.allocate_string(newStatus.c_str());


    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "status", newStatusChar));


    }


  // Save to file
  std::ofstream file_stored("data/level_data.xml");
  file_stored << doc;
  file_stored.close();
  doc.clear();




}

void LevelSelect::draw(){

  al_clear_to_color( al_map_rgb(75,75,100));
  levelSelectUI.draw();
  if(Options::draw_cursor)
    al_draw_bitmap(cursor,mouseListener::mouse_x,mouseListener::mouse_y,0);


}
void LevelSelect::update(){
  levelSelectUI.update();

  if(keyListener::key[ALLEGRO_KEY_ESCAPE])
    set_next_state(STATE_MENU);

  if(levelSelectUI.getElementByText("Reset Save Game") -> clicked()){
    for(int i=0; i<16; i++)
      completed_level_list[i]=false;
      writeLevelData();
      set_next_state(STATE_LEVELSELECT);


    }


  for(int i=1; i<14; i++){
    if(levelSelectUI.getUIElements().at(i) -> clicked()){
      game::level_to_start=i;
      set_next_state(STATE_GAME);
      }

  }


}
