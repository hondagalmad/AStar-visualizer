#ifndef SEARCHER_H
#define SEARCHER_H

#include <raylib.h>
#include "../data_structures/hashtable.hpp"

#define CELL_WIDTH 20
#define CELL_HEIGHT 20


enum CellType
{
    SOURCE = 0, WALL = 1, CHECKED = 2, TARGET = 3, PATH = 4, REMOVE
};

const Color COLORS[] = {DARKGREEN, BROWN, SKYBLUE, DARKBLUE, MAROON};

struct Cell
{
    Rectangle rect;
    Color color;
    CellType type;
};

class Searcher
{
private:

    struct Vector2I
    {
        int x;
        int y;
    };

    struct Grid
    {
        Vector2I startingPoint;
        Vector2I cellsNumber;
        Hashtable<int, Cell> table;
    };

    bool running;
    CellType selectedType;
    Grid grid;


    // only onse source and one target
    Cell source;
    Cell target;

    int sourceKey;
    int targetKey;

    // helper methods
    // ---------------------------------------------------------------------------------------------------------

    virtual bool isValid(int x, int y)
    {
        return x >= 0 && y >= 0 && x < grid.cellsNumber.x && y < grid.cellsNumber.y;
    }

    virtual Vector2I convertMouse(Vector2 mouse)
    {
        Vector2I v;
        v.x = ((int)(mouse.x) - grid.startingPoint.x) / CELL_WIDTH;
        v.y = ((int)(mouse.y) - grid.startingPoint.y) / CELL_HEIGHT;
        return v;
    }

    virtual int generateKey(Vector2I pos)
    {
        return pos.x + pos.y * grid.cellsNumber.x;
    }

    virtual Rectangle generateRect(Vector2I cellPos)
    {
        Rectangle rect;
        rect.width = CELL_WIDTH;
        rect.height = CELL_HEIGHT;

        rect.x = cellPos.x * CELL_WIDTH + grid.startingPoint.x;
        rect.y = cellPos.y * CELL_HEIGHT + grid.startingPoint.y;

        return rect;
    }

    virtual void putToGrid(Vector2I cellPos, CellType ct)
    {
        int key = generateKey(cellPos);

        if (running)
        {

        }
        else if (ct == REMOVE)
        {
            // user cann't remove the source or the target
            if (grid.table.containsKey(key))
            {
                grid.table.remove(key);
            }
            return;
        }
        else if (grid.table.containsKey(key)) return;
        else if (ct == SOURCE)
        {
            sourceKey = key;
            source.rect = generateRect(cellPos);
            return;
        }
        else if (ct == TARGET)
        {
            targetKey = key;
            target.rect = generateRect(cellPos);
            return;
        }

        if (sourceKey == key || targetKey == key) return;

        Cell cell;
        cell.rect = generateRect(cellPos);
        cell.type = ct;
        cell.color = COLORS[ct];
        grid.table.insert(key, cell);

    }

    // ---------------------------------------------------------------------------------------------------------
    

public:
    Searcher(Vector2 startingPos, Vector2 dimensions)
    {
        grid.startingPoint.x = startingPos.x;
        grid.startingPoint.y = startingPos.y;


        grid.cellsNumber.x = (int)dimensions.x / (int)CELL_WIDTH;
        grid.cellsNumber.y = (int)dimensions.y / (int)CELL_HEIGHT;
        
        running = false;

        selectedType = WALL;

        // make the source and the target in the middle
        // of the screen at opposite sides
        
        source.type = SOURCE;
        source.color = COLORS[SOURCE];

        target.type = TARGET;
        target.color = COLORS[TARGET];

        putToGrid((Vector2I){.x = 0, .y = grid.cellsNumber.y / 2}, SOURCE);
        putToGrid((Vector2I){.x = grid.cellsNumber.x - 1, .y = grid.cellsNumber.y / 2}, TARGET);
    }
    virtual void select(CellType ct) {selectedType = ct;}
    virtual void run() {running = true;}
    virtual void pause() {running = false;}

    virtual void press(Vector2 mouse)
    {
        if (running) {return;}
        Vector2I pos = convertMouse(mouse);
        if (isValid(pos.x, pos.y))
        {
            putToGrid(pos, selectedType);
        }
    }

    Cell getSource() {return source;}
    Cell getTarget() {return target;}

    void update(Hashtable<int, Cell>::HashIterator& iter)
    {
        if (running)
        {

        }

        iter.begin(grid.table);
    }

};

#endif