#include <gb/gb.h>

#define SCREEN_INDEX_X_MAX 20
#define SCREEN_INDEX_Y_MAX 18

#define BKG_TILE_NONE 0
#define BKG_TILE_HARVARD 1
#define BKG_TILE_STANFORD 2
#define BKG_TILE_MIT 3
#define BKG_TILE_PRINCETON 4

#define BKG_TILE_ANIM_00 6  
#define BKG_TILE_ANIM_01 7
#define BKG_TILE_ANIM_02 8
#define BKG_TILE_ANIM_03 9
#define BKG_TILE_ANIM_04 10
#define BKG_TILE_ANIM_05 11
#define BKG_TILE_ANIM_06 12

#define SPRITE_ID_CURSOR 0

#define SPRITE_CURSOR_DEFAULT 5
#define SPRITE_CURSOR_SELECT 6

#define SELECTION_RANGE 16
#define CLEAR_CONST 3
#define X_OFFSET 8
#define Y_OFFSET 16

#define GRID_SIZE_X 6
#define GRID_SIZE_Y 6

#define GRID_OFFSET_X 24 // Offset grid by number of BG tiles (offset value / 8)
#define GRID_OFFSET_Y 32 

#define STATE_MAIN_MENU 0
#define STATE_GAMEPLAY 1
#define STATE_PAUSE 2

extern uint8_t keyDown;
