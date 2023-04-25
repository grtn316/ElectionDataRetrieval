#include "MaxHeap.h"

MaxHeap::Node::Node()
{

}

//creates a MaxHeap with a certain capacity
MaxHeap::MaxHeap(int cap)
{
    capacity = cap;
    heap = new Node * [cap];
    size = 0;
}

//gets left child index
int MaxHeap::left(int i)
{
    return (2 * i + 1);
}

//gets right child index
int MaxHeap::right(int i)
{
    return (2 * i + 2);
}

//gets parent index
int MaxHeap::parent(int i)
{
    return ((i - 1) / 2);
}

//swaps 2 elements
void MaxHeap::swap(int a, int b)
{
    Node* temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

//inserts the county with all its variables into the heap, swaps entries to leave max element on top
void MaxHeap::insert(string c, string s, int num, float p)
{
    if (size == 0)
    {
        heap[0] = new Node(c, s, num, p);
        ++size;
    }
    else if (size > 0)
    {
        heap[size] = new Node(c, s, num, p);
        int kid = size;
        ++size;
        int par = parent(kid);
        while (par >= 0 && heap[kid]->votes > heap[par]->votes)
        {
            swap(kid, par);
            kid = par;
            par = parent(par);
        }
    }
}

//returns the county of the max node in the heap
string MaxHeap::getMaxCounty()
{
    return heap[0]->county;
}

//returns the state of the max node in the heap
string MaxHeap::getMaxState()
{
    return heap[0]->state;
}

//returns the number of votes for the max node in the heap
int MaxHeap::getMaxVotes()
{
    return heap[0]->votes;
}

//returns the percent of votes for the max node in the heap
float MaxHeap::getMaxPercent()
{
    return heap[0]->percent;
}

//removes the max element in the heap, then percolates down until the nodes are where they need to be
void MaxHeap::popMax()
{
    if (size == 0)
    {
        return;
    }
    else if (size < 2)
    {
        --size;
        return;
    }
    else if (size >= 2)
    {
        --size;
        heap[0] = heap[size];
        int par = 0;
        int l = left(par);
        int r = right(par);
        int max = par;
        while (l < size && r < size)
        {
            if (heap[l]->votes > heap[max]->votes)
            {
                max = l;
            }
            if (heap[r]->votes > heap[max]->votes)
            {
                max = r;
            }
            if (max != par)
            {
                swap(max, par);
                par = max;
                l = left(par);
                r = right(par);
            }
            else
            {
                l = size;
                r = size;
            }
        }
    }
}

//Just a helper function for testing, prints out the name and votes in order
void MaxHeap::printHeap()
{
    for (int i = 0; i < size; ++i)
    {
        cout << heap[i]->county << " " << heap[i]->state << " " << heap[i]->votes << endl;
    }
}
