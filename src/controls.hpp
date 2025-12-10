#ifndef CONTROLS_H
#define CONTROLS_H

#include "../include/raylib.h"
#include "../include/raygui.h"

#define NORMAL 0
#define HOVER 1
#define PRESSED 2


class Button
{
private:
    Rectangle rect;
    const char* text; 
    Color color;
    int state;

    bool isUnderMouse(Vector2 mouse)
    {
        return mouse.x > rect.x && mouse.y > rect.y
            && mouse.x < rect.x + rect.width
            && mouse.y < rect.y + rect.height;
    }
public:


    void updateState(Vector2 mouse, bool isPressed)
    {
        if (isUnderMouse(mouse))
        {
            state = HOVER;
            if (isPressed) state = PRESSED;
        }
        else{state = NORMAL;}
    }

    void setRect(float x, float y, float w, float h)
    {
        rect = (Rectangle){.x = x, .y = y, .width = w, .height = h};
    }
    void setText(const char* t)
    {
        text = t;
    }
    void setColor(Color c)
    {
        color = c;
    }

    Rectangle getRect()
    {
        return rect;
    }
    void getText(const char* t)
    {
        t = text;
    }
    Color getColor()
    {
        return color;
    }

};





#endif