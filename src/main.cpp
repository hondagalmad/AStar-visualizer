

#include <raylib.h>
#include <raygui.h>

#include "../algorithms/searcher.hpp"

int main()
{

    InitWindow(800, 800, "Visualizer");

    Searcher searcher(Vector2{.x = 0, .y = 200}, Vector2{.x = 800, .y = 600});

    Hashtable<int, Cell>::HashIterator iter;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        DrawRectangle(0, 200, 800, 600, WHITE);

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

        // update the searcher and start the iterator
        searcher.update(iter);

        Cell cc;
        // draw the cells with the help of the iterator
        for (iter; iter.hasNext(); iter.next())
        {
            cc = iter.getData();
            DrawRectangle(cc.rect.x, cc.rect.y, cc.rect.width, cc.rect.height, cc.color);
        }

        // draw source and target
        cc = searcher.getSource();
        DrawRectangle(cc.rect.x, cc.rect.y, cc.rect.width, cc.rect.height, cc.color);
        cc = searcher.getTarget();
        DrawRectangle(cc.rect.x, cc.rect.y, cc.rect.width, cc.rect.height, cc.color);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}