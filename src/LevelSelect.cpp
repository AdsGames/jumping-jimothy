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

    std::cout<<newStatus<<"\n";
    completed_level_list[levelNumber] = (newStatus=="complete");
    std::cout<<"Level " << levelNumber << ": " << completed_level_list[levelNumber] << "\n";

  }

  levelselect_font = al_load_ttf_font( "fonts/munro.ttf", 24, 0);

  levelselect_font_large = al_load_ttf_font( "fonts/munro.ttf", 48, 0);

  levelSelectUI = UIHandler();

  levelSelectUI.addElement(new UIElement(20+100,20,"Select a level",levelselect_font_large));
  levelSelectUI.getElementByText("Select a level") -> setVisibleBackground(false);
  levelSelectUI.getElementByText("Select a level") -> setTextColour(al_map_rgb(255,255,255));

  levelSelectUI.addElement(new Button(75+100, 110, "Level 1", levelselect_font));
  levelSelectUI.getElementByText("Level 1") -> setHeight(25);
  levelSelectUI.getElementByText("Level 1") -> setWidth(100);
  levelSelectUI.getElementByText("Level 1") -> setJustification(1);
  if(completed_level_list[1])levelSelectUI.getElementByText("Level 1") -> setBackgroundColour(al_map_rgb(0,255,0));

  levelSelectUI.addElement(new Button(220+100, 110, "Level 2", levelselect_font));
  levelSelectUI.getElementByText("Level 2") -> setHeight(25);
  levelSelectUI.getElementByText("Level 2") -> setWidth(100);
  levelSelectUI.getElementByText("Level 2") -> setJustification(1);
  if(completed_level_list[2])levelSelectUI.getElementByText("Level 2") -> setBackgroundColour(al_map_rgb(0,255,0));


  levelSelectUI.addElement(new Button(365+100, 110, "Level 3", levelselect_font));
  levelSelectUI.getElementByText("Level 3") -> setHeight(25);
  levelSelectUI.getElementByText("Level 3") -> setWidth(100);
  levelSelectUI.getElementByText("Level 3") -> setJustification(1);
  if(completed_level_list[3])levelSelectUI.getElementByText("Level 3") -> setBackgroundColour(al_map_rgb(0,255,0));


  levelSelectUI.addElement(new Button(510+100, 110, "Level 4", levelselect_font));
  levelSelectUI.getElementByText("Level 4") -> setHeight(25);
  levelSelectUI.getElementByText("Level 4") -> setWidth(100);
  levelSelectUI.getElementByText("Level 4") -> setJustification(1);

  levelSelectUI.addElement(new Button(655+100, 110, "Level 5", levelselect_font));
  levelSelectUI.getElementByText("Level 5") -> setHeight(25);
  levelSelectUI.getElementByText("Level 5") -> setWidth(100);
  levelSelectUI.getElementByText("Level 5") -> setJustification(1);



  levelSelectUI.addElement(new Button(75+50, 170, "Level 6", levelselect_font));
  levelSelectUI.getElementByText("Level 6") -> setHeight(25);
  levelSelectUI.getElementByText("Level 6") -> setWidth(100);
  levelSelectUI.getElementByText("Level 6") -> setJustification(1);

  levelSelectUI.addElement(new Button(220+50, 170, "Level 7", levelselect_font));
  levelSelectUI.getElementByText("Level 7") -> setHeight(25);
  levelSelectUI.getElementByText("Level 7") -> setWidth(100);
  levelSelectUI.getElementByText("Level 7") -> setJustification(1);

  levelSelectUI.addElement(new Button(365+50, 170, "Level 8", levelselect_font));
  levelSelectUI.getElementByText("Level 8") -> setHeight(25);
  levelSelectUI.getElementByText("Level 8") -> setWidth(100);
  levelSelectUI.getElementByText("Level 8") -> setJustification(1);

  levelSelectUI.addElement(new Button(510+50, 170, "Level 9", levelselect_font));
  levelSelectUI.getElementByText("Level 9") -> setHeight(25);
  levelSelectUI.getElementByText("Level 9") -> setWidth(100);
  levelSelectUI.getElementByText("Level 9") -> setJustification(1);

  levelSelectUI.addElement(new Button(655+50, 170, "Level 10", levelselect_font));
  levelSelectUI.getElementByText("Level 10") -> setHeight(25);
  levelSelectUI.getElementByText("Level 10") -> setWidth(100);
  levelSelectUI.getElementByText("Level 10") -> setJustification(1);



  levelSelectUI.addElement(new Button(75+100, 230, "Level 11", levelselect_font));
  levelSelectUI.getElementByText("Level 11") -> setHeight(25);
  levelSelectUI.getElementByText("Level 11") -> setWidth(100);
  levelSelectUI.getElementByText("Level 11") -> setJustification(1);

  levelSelectUI.addElement(new Button(220+100, 230, "Level 12", levelselect_font));
  levelSelectUI.getElementByText("Level 12") -> setHeight(25);
  levelSelectUI.getElementByText("Level 12") -> setWidth(100);
  levelSelectUI.getElementByText("Level 12") -> setJustification(1);

  levelSelectUI.addElement(new Button(365+100, 230, "Level 13", levelselect_font));
  levelSelectUI.getElementByText("Level 13") -> setHeight(25);
  levelSelectUI.getElementByText("Level 13") -> setWidth(100);
  levelSelectUI.getElementByText("Level 13") -> setJustification(1);

//  levelSelectUI.addElement(new Button(510+100, 230, "Level 14", levelselect_font));
//  levelSelectUI.getElementByText("Level 14") -> setHeight(25);
//  levelSelectUI.getElementByText("Level 14") -> setWidth(100);
//  levelSelectUI.getElementByText("Level 14") -> setJustification(1);
//
//  levelSelectUI.addElement(new Button(655+100, 230, "Level 15", levelselect_font));
//  levelSelectUI.getElementByText("Level 15") -> setHeight(25);
//  levelSelectUI.getElementByText("Level 15") -> setWidth(100);
//  levelSelectUI.getElementByText("Level 15") -> setJustification(1);



}

LevelSelect::~LevelSelect()
{
  //dtor
}

void LevelSelect::setLevelComplete(int newLevelNumber){


  completed_level_list[newLevelNumber]=true;

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


}
void LevelSelect::update(){
  levelSelectUI.update();

  if(keyListener::key[ALLEGRO_KEY_ESCAPE])
    set_next_state(STATE_MENU);

  for(int i=0; i<levelSelectUI.getUIElements().size(); i++){
    if(levelSelectUI.getUIElements().at(i) -> clicked()){
      game::level_to_start=i;
      set_next_state(STATE_GAME);
      }

  }


}
