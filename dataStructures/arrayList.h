#ifndef ARRAY_LIST
#define ARRAY_LIST

template <typename T>
class ArrayList
{
private:
    T* items;
    int size;
    int length;
    void resizeUp();
    void resizeDown();
public:
    ArrayList();
    ~ArrayList();

    void push(T item);
    T pop();
    void set(int i, T item) {items[i] = item;}
    T get(int i) const {return items[i];}
    int getSize() const {return size;}

};

#endif