#include <iostream>
#include <queue>

using namespace std;

class MaxHeap
{
    struct Node
    {
        int votes;
        float percent;
        string county;
        string state;
        //constructor
        Node();
        Node(string c, string s, int num, float p) : county(c), state(s), votes(num), percent(p) {}
    };
    Node** heap;
    int capacity;
    int size;
    
    public:
    MaxHeap(int cap);
    ~MaxHeap();
    int left(int i);
    int right(int i);
    int parent(int i);
    void swap(int a, int b);
    void insert(string c, string s, int num, float p);
    void printHeap();
    string getMaxCounty();
    string getMaxState();
    int getMaxVotes();
    float getMaxPercent();
    void popMax();
};