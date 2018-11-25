# Jumping Jimothy
Jumping Jimothy is a gravity modifying platformer made in C++ using Box2D and Allegro 5.

## Compiling
Using the C++ 14 standard

You will need to install:

- [Allegro 5](https://liballeg.org/download.html)
- [Box 2D](https://box2d.org/downloads/)

Link with the following options in this order:
```
-lallegro
-lallegro_primitives
-lallegro_image
-lallegro_audio
-lallegro_font
-lallegro_color
-lallegro_dialog
-lallegro_ttf
-lallegro_acodec
-lBox2D
```

Rapid XML is included in the /include/rapidxml/ directory. Rapid XML license can be found [here](https://github.com/discordapp/rapidxml).

