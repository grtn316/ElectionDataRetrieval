#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <tuple>

using namespace std;

// Each Candidate will have it's own B Tree - a 2-3 tree
// the key in the tree is a county's votes
// Referenced Lecture Video on B Trees
class BTree {

private:
    struct Key {
        string county;
        string state;
        int votes;
        float percVotes;
        Key(string _county, string _state, int _votes, float _percVotes)
        {
            this->county = _county;
            this->state = _state;
            this->votes = _votes;
            this->percVotes = _percVotes;
        }
        ~Key();
    };
    struct Node {
        Key** keys;
        Node** child;
        bool leaf;
        int deg; // min degree and range of number of keys
        int n; //number of keys in the node
        Node(int _deg);
        ~Node();
        void insertKeyAtNode(Key* newKey);
        void splitChildNode(int i, Node* node);
        stack<Key*> TraverseNode(stack<Key*>& stack);
    };
    int deg; // min degree
    Node* root; //root node of the B Tree
public:
    BTree(int _deg)
    {
        deg = _deg;
        root = nullptr;
    }
    ~BTree();
    void Insert(string county, string state, int votes, float percVotes);
    //outputs a vector of the top 10 county data by votes
    vector<tuple<string, string, int, float>> TraverseTopTen();
};

