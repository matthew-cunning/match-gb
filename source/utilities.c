#include <gb/gb.h>
#include <gbdk/console.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/define.h"
#include "../headers/utilities.h"

#include "../graphics/txt/scoretime_map.c"


extern uint8_t keyDown; //extern

uint8_t intGrid[GRID_SIZE_X][GRID_SIZE_Y];
uint8_t next[GRID_SIZE_X];

uint8_t selectionClampX, selectionClampY;

uint8_t selection = 0; //extern

// Things to do with the cursor:
extern struct cursor playerCursor;
uint8_t cursorMinX = 24;
uint8_t cursorMinY = 32;
uint8_t cursorMaxX = ((GRID_SIZE_X * GRID_SIZE_Y) << 1) + 24 + X_OFFSET; // x * y grid -  2*(x*y) + 8 (X OFFSET)
uint8_t cursorMaxY = ((GRID_SIZE_X * GRID_SIZE_Y) << 1) + 32 + Y_OFFSET; // x * y grid - 2*x*y + 16 (Y OFFSET)

// These coordinates must persist through function calls so the swap can be successfully completed
uint8_t x1, y1, x2, y2; 

// Control flow
extern uint8_t updateFlag;
extern uint8_t debug;

uint8_t Clear() {

    // Clear variables
    uint8_t i = 0, j = 0, k = 0;
    uint8_t timesCleared = 0;
    uint8_t inChainX = 0, inChainY = 0;
    uint8_t *currentXPtr = NULL, *currentYPtr = NULL;
    uint8_t *prevXPtr = NULL, *prevYPtr = NULL;
    uint8_t chainCounterX = 1, chainCounterY = 1;
    uint8_t highestChainX = 0, highestChainY = 0;
    uint8_t chainFirstX = 0, chainFirstY = 0;

    // Array of pointers to tiles that are to be cleared
    uint8_t* clearPtr[GRID_SIZE_X];

    // Detect and clear chains on horizontal lines - with pointers
    for(i = 0; i < GRID_SIZE_X; i++) {
        for(j = 0; j < GRID_SIZE_Y; j++) {
            currentXPtr=&intGrid[j][i];
            currentYPtr=&intGrid[i][j];
            inChainX = (*currentXPtr==*prevXPtr) && (*currentXPtr != BKG_TILE_NONE);
            inChainY = (*currentYPtr==*prevYPtr) && (*currentYPtr != BKG_TILE_NONE);

            // Start of X chain of at least 2 in a row
            if(inChainX && chainCounterX == 1) {
                chainFirstX = j - 1;
                chainCounterX++;
            } 
            // Continuation of X chain
            else if (inChainX && chainCounterX > 1) {
                chainCounterX++;
                highestChainX = chainCounterX >= CLEAR_CONST ? chainCounterX : 0 ;
            }
            // Break X chain
            else if (!inChainX)
                chainCounterX = 1;


            // Start of Y chain of at least 2 in a row
            if(inChainY && chainCounterY == 1) {
                chainFirstY = j - 1;
                chainCounterY++;
            } 
            // Continuation of Y chain
            else if (inChainY && chainCounterY > 1) {
                chainCounterY++;
                highestChainY = chainCounterY >= CLEAR_CONST ? chainCounterY : 0 ;
            }
            // Break Y chain
            else if (!inChainY)
                chainCounterY = 1;
        
            prevXPtr = &(*currentXPtr);
            prevYPtr = &(*currentYPtr);
        }

        // Clear any X chains
        if (highestChainX >= CLEAR_CONST) {
            for (j = 0; j < highestChainX; j++)
                clearPtr[j] = &intGrid[chainFirstX + j][i];
                // intGrid[chainFirstX + k][i] = BKG_TILE_NONE;
            for (k = 0; k < GRID_SIZE_X; k++)
                *clearPtr[k] = BKG_TILE_NONE;
            timesCleared++;
        }

        // Clear any Y chains
        if (highestChainY >= CLEAR_CONST) {
            for (j = 0; j < highestChainY; j++)
                clearPtr[j] = &intGrid[i][chainFirstY + j];
                // intGrid[i][chainFirstY + k] = BKG_TILE_NONE;
            for (j = 0; j < GRID_SIZE_X; j++)
                *clearPtr[j] = BKG_TILE_NONE;
            timesCleared++;
        }

        for (j = 0; j < GRID_SIZE_X; j++)
            clearPtr[j] = NULL;
        
        highestChainX = highestChainY = 0;
        chainCounterX = chainCounterY = 0;
        prevXPtr = prevYPtr = NULL;
    }   
    return timesCleared;
}    

void InitNext() {
    for (uint8_t i = 0; i < GRID_SIZE_X; i++)
        next[i] = RandomRange(1,5);    
}

void PullFromNext() {
    for(uint8_t i = 0; i < GRID_SIZE_X; i++) {
        while (intGrid[i][0] == BKG_TILE_NONE) {
            intGrid[i][0] = next[i];
            next[i] = RandomRange(1,5);
            MoveDown();
        }
    } 
        
}
void MoveDown() {
    uint8_t count = 0;

    for(uint8_t i = 0; i < GRID_SIZE_X; i++) {
        for(uint8_t j = 0; j < GRID_SIZE_Y-1; j++) {
            if (intGrid[i][j] != BKG_TILE_NONE && intGrid[i][j+1] == BKG_TILE_NONE) {
                SwapTiles(&intGrid[i][j], &intGrid[i][j+1]);
                count++;
            }
            else continue;
        }


        if (count >= 1)
            MoveDown();
    }
    return;
    
}

void FillBackground(uint8_t tile){
    fill_bkg_rect(0,0,0x20,18,tile);

    for(uint8_t i = 0; i < GRID_SIZE_X << 1; i++)
        for(uint8_t j = 0; j < GRID_SIZE_Y << 1; j++)
            if(i % 2 == 0 && j % 2 == 0)
                set_bkg_tile_xy(i+2,j+2,0);
}

void SetupGrid() {
    for(uint8_t i = 0; i < GRID_SIZE_X; i++)
        for(uint8_t j = 0; j < GRID_SIZE_Y; j++) {
            intGrid[i][j] = RandomRange(1,5);
        }
}

void AnimateBackground() {
    // TODO: Fix animations of tiles between rows
    uint8_t index;
    for (uint8_t i = 0; i < SCREEN_INDEX_X_MAX; i++)
        for (uint8_t j = 0; j < SCREEN_INDEX_Y_MAX; j++)
            if (index = get_bkg_tile_xy(i,j)) {
                if (index >= BKG_TILE_ANIM_00 && index < BKG_TILE_ANIM_06)
                    set_bkg_tile_xy(i,j,++index);
                else if (index >= 12)
                    set_bkg_tile_xy(i,j,BKG_TILE_ANIM_00);
                else
                    break;
            }
}

void PopulateBackground() {
    for(uint8_t i = 0; i < GRID_SIZE_X << 1; i++)
        for(uint8_t j = 0; j < GRID_SIZE_Y << 1; j++)
            if(get_bkg_tile_xy(i+2,j+2) == 0){
                set_bkg_tile_xy(i+2,j+2,RandomRange(1,5));
            }

}

void UpdateBackground() {
    // Iterate over grid, update background tiles in accordance with 2D grid array
    for(uint8_t i = 0; i < GRID_SIZE_X; i++)
        for(uint8_t j = 0; j < GRID_SIZE_Y; j++)
            set_bkg_tile_xy((i << 1) + 2, (j << 1) + 2, intGrid[i][j]);
}

uint8_t RandomRange(uint8_t min, uint8_t max) {
    // Generate random value from DIV_REG
    uint8_t v = ((uint8_t)rand());
    return min+(v % (max-min));
}

void RandomInit(uint16_t *seed) {
    *seed = LY_REG;
    *seed |= (uint16_t)DIV_REG << 8;
    initrand(*seed);
}

void CursorInit(struct cursor* cursor) {
    cursor->x = 16 + X_OFFSET; // 16 / 8 is initial background tile index
    cursor->y = 16 + Y_OFFSET; // So init cursor at (2,2)

    // Grid row and col is 0 indexed
    cursor->col = 0;
    cursor->row = 0;

    move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
}

void MoveCursor(struct cursor* cursor) {
    if(!keyDown && !selection) {
        
        switch (joypad())
        {
        case J_UP:
            if(IsWithinGrid(cursor->x, cursor->y - 16)) {
                cursor->y -= 16;
                // cursor->row--;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }   

            break;
        case (J_DOWN):
            if(IsWithinGrid(cursor->x, cursor->y + 16)) {
                cursor->y += 16;
                // cursor->row++;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }
            break;    
        case J_LEFT:
            if(IsWithinGrid(cursor->x - 16, cursor->y)) {
                cursor->x -= 16;
                // cursor->col--;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }
            break;
        case J_RIGHT:
            if(IsWithinGrid(cursor->x + 16, cursor->y)) {
                cursor->x +=16;
                // cursor->col++;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y); // player x and y values are OK
                keyDown = 1;
            }
            break;

        default:
            break;
        }

        if(debug)
            printf("%d\n%d\n", cursor->x, cursor->y);
    }

    // Restricted movement within selection
    else if (!keyDown && selection) {
        
        switch (joypad())
        {
        case J_UP:
            if(IsWithinGrid(cursor->x, cursor->y - 16) && IsWithinSelectionRange(cursor->x, cursor->y - 16)) {
                cursor->y -= 16;
                // cursor->row--;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }   

            break;
        case (J_DOWN):
            if(IsWithinGrid(cursor->x, cursor->y + 16) && IsWithinSelectionRange(cursor->x, cursor->y + 16)) {
                cursor->y += 16;
                // cursor->row++;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }
            break;    
        case J_LEFT:
            if(IsWithinGrid(cursor->x - 16, cursor->y) && IsWithinSelectionRange(cursor->x - 16, cursor->y)) {
                cursor->x -= 16;
                // cursor->col--;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }
            break;
        case J_RIGHT:
            if(IsWithinGrid(cursor->x + 16, cursor->y) && IsWithinSelectionRange(cursor->x + 16, cursor->y)) {
                cursor->x +=16;
                // cursor->col++;
                move_sprite(SPRITE_ID_CURSOR, cursor->x, cursor->y);
                keyDown = 1;
            }
            break;

        default:
            break;
        }
    }

    else if (keyDown && !joypad())
        keyDown = 0;

}

uint8_t IsWithinGrid(uint8_t x, uint8_t y) {
    // Return 1 if within range, otherwise return 0
    return (x >= cursorMinX && x <= cursorMaxX) && (y >= cursorMinY && y <= cursorMaxY); 
}

uint8_t IsWithinSelectionRange(uint8_t x, uint8_t y) {
            
            // Part one: Check that x and y are within square selection range
            // Part two: Exclude corners from selection, only allow swap within cross shape (+)

    return ((x >= selectionClampX - SELECTION_RANGE && x <= selectionClampX + SELECTION_RANGE) && (y >= selectionClampY - SELECTION_RANGE && y <= selectionClampY + SELECTION_RANGE) &&
            
            ((x != (selectionClampX - SELECTION_RANGE) || y != (selectionClampY - SELECTION_RANGE))) &&
            ((x != (selectionClampX + SELECTION_RANGE) || y != (selectionClampY - SELECTION_RANGE))) &&
            ((x != (selectionClampX - SELECTION_RANGE) || y != (selectionClampY + SELECTION_RANGE))) &&
            ((x != (selectionClampX + SELECTION_RANGE) || y != (selectionClampY + SELECTION_RANGE))) 
            );
}

void SelectTile(struct cursor* cursor) {
    // struct cell selectionA, selectionB;

    // uint8_t currentSelectionA, currentSelectionB;

    // Select tile A, return tile index if player presses A
    if ((joypad() & J_A ) && !keyDown && !selection) {
        // Enable selection
        selection = 1;
        keyDown = 1;

        // Clamp x and y to initial selection
        selectionClampX = cursor->x;
        selectionClampY = cursor->y;

        // Update cursor sprite to selection
        set_sprite_tile(SPRITE_ID_CURSOR,SPRITE_CURSOR_SELECT);

        x1 = cursor->col;
        y1 = cursor->row;

        // TODO: Highlight moveable area - use a metasprite
    }

    // Cancel selection if player presses B while selection active
    else if ((joypad() & J_B) && !keyDown && selection) {           
        selection = 0;
        keyDown = 1;
        updateFlag = 1;
        // Update cursor sprite to default
        set_sprite_tile(SPRITE_ID_CURSOR,SPRITE_CURSOR_DEFAULT);
        ReturnCursorToClamp(&playerCursor);
        // TODO: Un-highlight moveable area - use a metasprite
    }

    // Select Second tile 
    else if ((joypad() & J_A) && !keyDown && selection) {
        x2 = cursor->col;
        y2 = cursor->row;
        
        selection = 0;
        keyDown = 1;
            
        // Swap tiles
        // Format: grid[x][y] 
        SwapTiles(&intGrid[x1][y1], &intGrid[x2][y2]);
        
        //TODO: grid[selectionA.col][selectionA.row].symbol with grid[selectionA.col][selectionA.row].symbol
        set_sprite_tile(SPRITE_ID_CURSOR,SPRITE_CURSOR_DEFAULT);
        ReturnCursorToClamp(&playerCursor);
        updateFlag = 1;
        
    }

    // Only allow for single press of button
    else if (keyDown && !joypad())
        keyDown = 0;   
}

void SwapTiles(uint8_t *pa, uint8_t *pb) {
    uint8_t temp;
    // Swap actual grid values
    temp = *pa;
    *pa = *pb;
    *pb = temp;
}

void DebugToggle() {
    
        if (joypad() & J_SELECT) {
            if(!keyDown) {
                debug = 1;
                keyDown = 1;
            }

            else {
                debug = 0;
                keyDown = 1;
            }
        }
        else if (keyDown && !joypad()) 
            keyDown = 0;
}

void ReturnCursorToClamp(struct cursor* cursor) {
        move_sprite(SPRITE_ID_CURSOR, selectionClampX, selectionClampY);
    
        // playerCursor.col = (playerCursor.x < selectionClampX ? playerCursor.col + 1 : playerCursor.col - 1);
        // playerCursor.row = (playerCursor.y < selectionClampY ? playerCursor.row + 1 : playerCursor.row - 1);
        cursor->x = selectionClampX;
        cursor->y = selectionClampY;
}

void UpdateCursorColRow(struct cursor* cursor) {

    // if (playerCursor.x > cursorMinX)
    //     playerCursor.col = (playerCursor.x - cursorMinX) >> 4; 
    //     // Divide playercursor - cursorminX by 16 to get player cursor col. cursorMinX contains the X offset, so subtracting it gives correct result.
    
    // /*  The above algorithm only works when we are past the first row or col as it has to account 
    //     for the double spacing between tiles which is nonexistant on the first row and col  
    // */
    // else // problem here?
    //     playerCursor.col = 0;  

    // if(playerCursor.y > cursorMinY)
    //     playerCursor.row = (playerCursor.y - cursorMinY) >> 4;
    // // Same for row (y)
    // else // problem here ?
    //     playerCursor.row = 0;

    //TODO: Fix (Not scaleable in current iteration)
    // Update Col
    switch (cursor->x)
    {
    case 24:
        cursor->col = 0;
        break;

    case 40:
        cursor->col = 1;
        break;

    case 56:
        cursor->col = 2;
        break;

    
    case 72:
        cursor->col = 3;
        break;

    
    case 88:
        cursor->col = 4;
        break;

    
    case 104:
        cursor->col = 5;
        break;

    case 120:
        cursor->col = 6;
        break;
    
    case 136:
        cursor->col = 7;
        break;
    default:
        break;
    }
    // Update Row
    switch (cursor->y)
    {
    case 32:
        cursor->row = 0;
        break;

    case 48:
        cursor->row = 1;
        break;

    case 64:
        cursor->row = 2;
        break;

    
    case 80:
        cursor->row = 3;
        break;

    
    case 96:
        cursor->row = 4;
        break;

    
    case 112:
        cursor->row = 5;
        break;

    case 128:
        cursor->row = 6;
        break;
    
    case 144:
        cursor->row = 7;
        break;
    default:
        break;
    }

}

void DelayPerformant(uint8_t times) {
    for(uint8_t i = 0; i < times; i++) {
        wait_vbl_done();
    }
}

void RenderScoreTime() {
    // Draw score from bkg tiles and render current player score
    // Draw score text 
    set_bkg_tiles(14,3, 5, 1, score_txt);
    
    set_bkg_tiles(14,6,4,1,time_txt);
    // Draw score numerals  
    // set_bkg_tiles(14,4,5,1,score_val);
}

// void UpdateScore(uint16_t score, unsigned char* scoreMap) {
void UpdateScore(uint16_t score) {
    uint8_t i = 0;
    unsigned char buffer [sizeof(uint16_t)+1];
    
    if (score < 60000) {
        itoa(score,buffer,10);
        
        // for (i = 0; i < 5; i++)
        //     buffer[i] -= 48 + 0x1C;


            // scoreMap[i] = buffer[i] - 48 + 0x1C;

        for (i = 0; i < 5; i++) {
            // if (score % (int) (pow(10,i)) >= 0)
            set_bkg_tile_xy(14+i,4,buffer[i] - 48 + 0x1C);
        }
    }

}

void UpdateTime(uint8_t time) {

    unsigned char buffer [sizeof(uint8_t)+1];
    
    itoa(time,buffer,10);
    
        for (uint8_t i = 0; i < 3; i++) {
            // if (score % (int) (pow(10,i)) >= 0)
            set_bkg_tile_xy(14+i,7,buffer[i] - 48 + 0x1C);
        }
}