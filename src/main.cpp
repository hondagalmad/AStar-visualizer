

#include <raylib.h>
#include <raygui.h>
#include "../algorithms/searcher.hpp"

#define FRAMES 60.0f

int main()
{

    InitWindow(1920, 1080, "Visualizer");

    Searcher searcher(Vector2{.x = 0, .y = 0}, Vector2{.x = 1920, .y = 1080});

    Hashtable<Vector2I, CellType>::HashIterator iter;

    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);

        // select the type
        if (IsKeyPressed(KEY_S))
        {
            searcher.select(SOURCE);
        }
        else if (IsKeyPressed(KEY_W))
        {
            searcher.select(WALL);
        }
        else if (IsKeyPressed(KEY_T))
        {
            searcher.select(TARGET);
        }
        else if (IsKeyPressed(KEY_R))
        {
            searcher.select(REMOVE);
        }
        else if (IsKeyPressed(KEY_C))
        {
            searcher.run();
        }
        else if (IsKeyPressed(KEY_X))
        {
            searcher.pause();
        }

        // add particles if mouse is pressed
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            searcher.press(GetMousePosition());
        }
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 diff = GetMouseDelta();
            searcher.drag(diff.x, diff.y);
        }
        
        searcher.zoom((int)GetMouseWheelMove());

        // update the searcher and start the iterator
        searcher.update(iter);

        Rectangle rect;
        // draw the cells with the help of the iterator
        for (iter; iter.hasNext(); iter.next())
        {
            searcher.generateRect(iter.getKey(), &rect);
            if (searcher.isInRange(rect))
            {
                DrawRectangle(rect.x, rect.y, rect.width, rect.height, COLORS[iter.getValue()]);
            }
        }

        // draw the current cell with red
        if (!searcher.isPathFound())
        {
            searcher.generateRect(searcher.getCurrentPos(), &rect);
            if (searcher.isInRange(rect))
            {
                DrawRectangle(rect.x, rect.y, rect.width, rect.height, RED);
            }
        }

        // drawing grid lines
        Vector2I diff = searcher.getDiff();
        Vector2I cellsNumber = searcher.getCellsNumber();
        Vector2 dimensions = searcher.getDimensions();
        int cellDimension = searcher.getCellDimemsion();


        for (float x = diff.x % cellDimension; x <= dimensions.x + diff.x % cellDimension; x += cellDimension)
        {
            DrawLine(x, 0, x, dimensions.y, BLACK);
        }
        
        
        for (float y = diff.y % cellDimension; y <= dimensions.y + diff.y % cellDimension; y += cellDimension)
        {
            DrawLine(0, y, dimensions.x, y, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}