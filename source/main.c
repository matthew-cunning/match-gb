#include <gb/gb.h>
#include <gbdk/console.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>

#include "../graphics/cs50.c"
#include "../graphics/cs50bg.c"
#include "../graphics/title/cs50title_data.c"
#include "../graphics/title/cs50title_map.c"
#include "../graphics/cs50map.c"
#include "../graphics/ScoreTime.c"
#include "../headers/define.h"
#include "../headers/utilities.h"

struct cursor playerCursor;

uint8_t animCounter;
uint16_t seed;
uint8_t keyDown = 0; 
uint8_t lastKey;
uint8_t updateFlag;
uint8_t debug;

uint16_t score;

// Track whether a key is currently held down

// Variable to track whether player has selected a tile to swap


// Track numnber of clears returned by Clear()
uint8_t clears;

uint8_t count = 0;

uint8_t time = 30;
uint8_t timeCounter = 0;

// struct cursor *pcursor;

void main() {

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    
    set_sprite_data(0,7,cs50);
    set_sprite_tile(SPRITE_ID_CURSOR,5);
    
    playerCursor.x = playerCursor.y = playerCursor.row = playerCursor.col = 0;


    // State: MAIN MENU
    // Draw Title Screen
    set_bkg_data(0,106,cs50title_data);
    set_bkg_tiles(0,0,20,18,cs50title_map);

    waitpad(J_START);
    // Start game, generate random seed

    // State: LOAD

    RandomInit(&seed);
    CursorInit(&playerCursor);

    set_bkg_data(0,5,cs50);
    set_bkg_data(6,15,cs50bg);
    set_bkg_data(20,18, ScoreTime);
    
    set_bkg_tiles(0,0,20,18,cs50map);
    // Set up grid
    // Format: Col, Row (X,Y)
    
    SetupGrid();
    InitNext();
    UpdateBackground();
    // Update and redraw Grid
    
    
    while(1) {
        if(timeCounter<59)
            timeCounter++;
        else {
            time--;
            timeCounter=0;
        }
        RenderScoreTime();
        UpdateTime(time);

        while(time <= 0)
            printf("Game Over");

        // State: GAMEPLAY
        // Animate background
        // if(++animCounter % 12 == 0) {
        //     animCounter = 0;
        //     AnimateBackground();
        // }


        MoveCursor(&playerCursor);
        UpdateCursorColRow(&playerCursor);

        // Tile select check
        SelectTile(&playerCursor);

        // An action has been made which requires an update of the game state
        if(updateFlag) {
            UpdateBackground();
            
            while ((clears = Clear()) > 0) {
                MoveDown(); 
                DelayPerformant(3);
                UpdateBackground();
                PullFromNext();
                DelayPerformant(3);
                UpdateBackground();
                score+=100;
                UpdateScore(score);
                time++;
                UpdateTime(time);
            }

            if (clears == 0) {
                MoveDown();
                PullFromNext();
            }
            UpdateBackground();
            updateFlag = 0;
        }

        // Vertical blank refresh
        wait_vbl_done();
    }
}
