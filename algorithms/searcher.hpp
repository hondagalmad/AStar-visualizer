#ifndef SEARCHER_H
#define SEARCHER_H

#include <raylib.h>
#include "../data_structures/hashtable.hpp"
#include "../data_structures/heap.hpp"

#define CELL_WIDTH 20
#define CELL_HEIGHT 20
#define MIN_X_EDGE -1
#define MIN_Y_EDGE -1
#define MAX_X_EDGE 1
#define MAX_Y_EDGE 1

enum CellType
{
    SOURCE = 0, CHECKED = 1, WALL = 2, TARGET = 3, PATH = 4, REMOVE
};

const Color COLORS[] = {DARKGREEN, SKYBLUE, BROWN, DARKBLUE, MAROON};


struct Vector2I
{
    int x;
    int y;
};

struct Cell
{
    Vector2I pos;
    Rectangle rect;
    CellType type;
};


class Searcher
{
private:

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


    // searching 
    Heap<int> heap;
    
    // contains the cell's key as a key, 
    // and the distance to the source as the value
    Hashtable<int, int> distTo;

    // contains the cell's key as a key,
    // and the key of the cell before it
    Hashtable<int, int> from;

    bool pathFound;

    Vector2I currentPos;
    int currentKey;
    Vector2I currentEdge;

    // helper methods
    // ---------------------------------------------------------------------------------------------------------

    virtual bool isValid(int x, int y)
    {
        return x >= 0 && y >= 0 && x < grid.cellsNumber.x && y < grid.cellsNumber.y;
    }

    // used for converting screen position to grid position
    virtual Vector2I getGridCoordinates(Vector2 mouse)
    {
        Vector2I v;
        v.x = ((int)(mouse.x) - grid.startingPoint.x) / CELL_WIDTH;
        v.y = ((int)(mouse.y) - grid.startingPoint.y) / CELL_HEIGHT;
        return v;
    }

    // generates a key to be used by the grid
    virtual int generateKey(int x,  int y)
    {
        return x + y * grid.cellsNumber.x;
    }

    // generates the rectangle to be drawn to the screen
    virtual Rectangle generateRect(int key)
    {
        int x = key % grid.cellsNumber.x;
        int y = key / grid.cellsNumber.x;
        Rectangle rect;
        rect.width = CELL_WIDTH;
        rect.height = CELL_HEIGHT;

        rect.x = x * CELL_WIDTH + grid.startingPoint.x;
        rect.y = y * CELL_HEIGHT + grid.startingPoint.y;

        return rect;
    }

    // generates the position relative to the grid from a key
    virtual Vector2I generatePos(int key)
    {
        int x = key % grid.cellsNumber.x;
        int y = key / grid.cellsNumber.x;
        return (Vector2I){.x = x, .y = y};
    }

    virtual bool putToGrid(int key, CellType ct)
    {

        if (running)
        {
            // cell is not inserted if it's place is occupied by
            // a wall or something of the same type
            if (grid.table.containsKey(key))
            {
                if (grid.table.get(key).type >= ct) return false;
            } 
        }
        else if (ct == REMOVE)
        {
            // user cann't remove the source or the target
            if (grid.table.containsKey(key) && grid.table.get(key).type != CHECKED)
            {
                grid.table.remove(key);
            }
            return false;
        }

        else if (grid.table.containsKey(key)) return false;

        else if (ct == SOURCE)
        {
            sourceKey = key;
            source.rect = generateRect(key);
            source.pos = generatePos(key);
            return false;
        }
        else if (ct == TARGET)
        {
            targetKey = key;
            target.rect = generateRect(key);
            target.pos = generatePos(key);
            return false;
        }

        if (sourceKey == key || targetKey == key) return false;

        Cell cell;
        cell.rect = generateRect(key);
        cell.type = ct;
        cell.pos = generatePos(key);
        grid.table.insert(key, cell);
        return true;
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

        pathFound = false;

        // make the source and the target in the middle
        // of the screen at opposite sides
        
        source.type = SOURCE;

        target.type = TARGET;

        putToGrid(grid.cellsNumber.y / 2 * grid.cellsNumber.x, SOURCE);
        putToGrid(grid.cellsNumber.x - 1 + grid.cellsNumber.y / 2 * grid.cellsNumber.x, TARGET);
    }




    virtual void select(CellType ct) {selectedType = ct;}
    virtual void run()
    {
        running = true;

        from.insert(sourceKey, -1);
        distTo.insert(sourceKey, 0);

        currentEdge.x = MIN_X_EDGE;
        currentEdge.y = MIN_Y_EDGE;

        currentPos = source.pos;
        currentKey = sourceKey;
    }
    virtual void pause() {running = false;}



    virtual void press(Vector2 mouse)
    {
        if (running) {return;}
        Vector2I pos = getGridCoordinates(mouse);
        if (isValid(pos.x, pos.y))
        {
            putToGrid(generateKey(pos.x, pos.y), selectedType);
        }
    }


    virtual Cell getCurrentCell()
    {
        if (currentKey == sourceKey) return source;
        return grid.table.get(currentKey);
    }

    virtual bool isRunning() {return running;}


    virtual Cell getSource() {return source;}
    virtual Cell getTarget() {return target;}




    virtual void update(Hashtable<int, Cell>::HashIterator& iter)
    {
        if (running)
        {
            if (!pathFound)
            {

                if (currentEdge.y > MAX_Y_EDGE)
                {
                    if (heap.isEmpty())
                    {
                        std::cout<< "No Path is Found" << std::endl;
                        std::abort();
                    }
                    currentEdge.x = MIN_X_EDGE;
                    currentEdge.y = MIN_Y_EDGE;
                    currentKey = heap.removeSmallest();
                    currentPos = grid.table.get(currentKey).pos;

                }
                else if (currentEdge.x > MAX_X_EDGE)
                {
                    currentEdge.x = MIN_X_EDGE;
                    currentEdge.y += 1;
                }
    
                // checking if the edge is valid in the grid (adding diagonally is not allowed)
                if (isValid(currentPos.x + currentEdge.x, currentPos.y + currentEdge.y) && abs(currentEdge.x) != abs(currentEdge.y))
                {

                    int newKey = generateKey(currentPos.x + currentEdge.x, currentPos.y + currentEdge.y);

                    if (newKey == targetKey) pathFound = true;
    
                    // only add the new cell if it's added to the grid successfully
                    if (putToGrid(newKey, CHECKED))
                    {
                        distTo.insert(newKey, distTo.get(currentKey) + 1);
                        from.insert(newKey, currentKey);
                        heap.add(newKey, distTo.get(newKey));
                    }
                }
                currentEdge.x += 1;
            }
        }

        iter.begin(grid.table);
    }
};

#endif