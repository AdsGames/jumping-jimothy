/**
 * Audio
 * Danny Van Stemp and Allan Legemaate
 * Music tracks and sound settings
 * 23/11/2018
 **/

#pragma once

namespace Audio {

enum class Track { None, Menu, Game };

// Load music and apply the saved sound settings
void init();

// Play a music track, does nothing if it is already playing
void playMusic(Track track);

// Stop the music
void stopMusic();

// Turn sound effects on or off, saved in the config
void setSfxEnabled(bool enabled);

// Turn music on or off, saved in the config
void setMusicEnabled(bool enabled);

}  // namespace Audio
