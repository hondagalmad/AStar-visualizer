#include <iostream>
#include "./arrayList.h"

template <typename T>
ArrayList<T>::ArrayList()
{
    size = 0;
    length = 4;
    items = new T[length];
}
template <typename T>
ArrayList<T>::~ArrayList()
{
    delete [] items;
}



template <typename T>
void ArrayList<T>::resizeUp()
{
    length *= 2;
    T* temp = items;

    items = new T[length];

    for (int i = 0; i < size; i += 1)
    {
        items[i] = temp[i];
    }

    delete [] temp;
}
template <typename T>
void ArrayList<T>::push(T item)
{
    if (size / length == 1)
    {
        resizeUp();
    }
    items[size] = item;
    size += 1;
}



template <typename T>
void ArrayList<T>::resizeDown()
{
    length *= 0.5;
    T* temp = items;
    items = new T[length];
    for (int i = 0; i < size; i += 1)
    {
        items[i] = temp[i];
    }
    delete [] temp;
}
template <typename T>
T ArrayList<T>::pop()
{
    if (length > 32 && (float)size / (float)length < 0.25)
    {
        resizeDown();
    }
    size -= 1;
    return items[size];
}
