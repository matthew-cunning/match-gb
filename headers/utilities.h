#include <gb/gb.h>
typedef struct cursor {
    uint8_t x;
    uint8_t y;
    uint8_t row;
    uint8_t col;
} cursor;

// Function prototypes

void FillBackground(uint8_t tile);
void SetupGrid();
void MoveDown();
void InitNext();
void PullFromNext();
void AnimateBackground();
void RandomInit(uint16_t *seed);
uint8_t RandomRange(uint8_t min, uint8_t max);
void PopulateBackground();
void UpdateBackground();
void CursorInit(struct cursor* cursor);
uint8_t IsWithinGrid(uint8_t x, uint8_t y);
uint8_t IsWithinSelectionRange(uint8_t x, uint8_t y);
void SelectTile(struct cursor* cursor);
void MoveCursor(struct cursor* cursor);
void UpdateCursorColRow(struct cursor* cursor);
void ReturnCursorToClamp(struct cursor* cursor);
void SwapTiles(uint8_t *pa, uint8_t *pb);
void DebugToggle();
uint8_t Clear();
void DelayPerformant(uint8_t times);
void RenderScoreTime();
// void UpdateScore(uint16_t score, unsigned char* scoreMap);
void UpdateScore(uint16_t score);
void UpdateTime(uint8_t time);
