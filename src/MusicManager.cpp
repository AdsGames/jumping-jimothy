#include "MusicManager.h"

Sound MusicManager::game_music;
Sound MusicManager::menu_music;

MusicManager::MusicManager()
{

}

MusicManager::~MusicManager()
{
  //dtor
}
void MusicManager::load(){
  game_music.load_ogg("music/tojam.ogg");
  menu_music.load_ogg("music/menu.ogg");



}

void MusicManager::destroy(){
  al_destroy_sample(game_music.getSample());
  al_destroy_sample(menu_music.getSample());



}

