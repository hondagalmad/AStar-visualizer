#ifndef QUEUE_H
#define QUEUE_H
template <typename T>
class   Queue {
private:
    T* arr;
    int cap;
    int s;

    void resizeup(){
        int newcap=cap*2;
        T* newarr=new T[newcap];

        for (int i=0;i<s;i++)
            newarr[i]=arr[i];

        delete[] arr;
        arr = newarr;
        cap = newcap;
    }

    void shrink(){
        if (cap >2&&s <=cap/4) {
            int newcap=cap/2;
            if (newcap<8) newcap=8;

            T* newarr=new T[newcap];

            for (int i=0;i<s;i++)
                newarr[i]=arr[i];

            delete[] arr;
            arr=newarr;
            cap=newcap;
        }
    }

public:
    Queue(){
        cap=8;
        s=0;
        arr=new T[cap];
    }

    ~Queue(){
        delete[] arr;
    }

    void pushback(const T& value){
        if (s==cap)
            resizeup();

        arr[s++]=value;
    }

    void pop(){
        if (s>0){
            for (int i=0;i<s-1;i++)
                arr[i]=arr[i+1];

            s--;
            shrink();
        } else{
            throw std::runtime_error("queue is empty!");
        }
    }

    T get(int index) const{
        if (index<0||index>=s)
            throw std::out_of_range("Doesn't exist!");

        return arr[index];
    }
    int findindex(const T& value) const {
    for (int i=0;i<s;i++) {
        if (arr[i]==value)
            return i;
    }
    return -1;
}

    void set(int index,const T& value){
        if (index<0||index>=s)
            throw std::out_of_range("Doesn't exist!");

        arr[index]=value;
    }
   bool empty() const {
    return s == 0;
}

    int size() const{return s;}
    int capacity() const{return cap;}

    void clear(){ 
        s = 0; 
        shrink(); 
    }
};
#endif  