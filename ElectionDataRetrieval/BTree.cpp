#include "BTree.h"

//Referenced code guidance from https://www.geeksforgeeks.org/insert-operation-in-b-tree/

BTree::Node::Node(int _deg)
{
    this->leaf = true;
    this->deg = _deg;
    this->keys = new Key * [2 * deg - 1];
    this->child = new Node * [2 * deg - 1];
    this->n = 0;
}

void BTree::Insert(string county, string state, int votes, float percVotes) {
    Key* newKey = new Key(county, state, votes, percVotes);
    if (root == nullptr)
    {
        root = new Node(deg);
        root->keys[0] = newKey;
        root->n = 1;
    }
    else
    {
        if (root->n == 2 * deg - 1)
        {
            //create a new node preparing for spliting
            Node* newParent = new Node(deg);
            newParent->leaf = false;
            newParent->child[0] = root;
            newParent->splitChildNode(0, root);

            int i = 0;
            if (newParent->keys[0]->votes < newKey->votes)
            {
                i++;
            }
            newParent->child[i]->insertKeyAtNode(newKey);

            root = newParent;
        }
        else
        {
            root->insertKeyAtNode(newKey);
        }
    }
}

void BTree::Node::insertKeyAtNode(Key* newKey)
{
    //start at rightmost position
    int i = n - 1;

    if (leaf == true)
    {
        //brings me to the correct position to insert
        while (i >= 0 && keys[i]->votes > newKey->votes)
        {
            keys[i + 1] = keys[i];
            i--;
        }
        //insert new key at the right position
        keys[i + 1] = newKey;
        //increment the keys counter
        n++;
    }
    else //if this is not a leaf node
    {
        while (i >= 0 && keys[i]->votes > newKey->votes)
        {
            i--;
        }

        //if child already has 2 keys in it, aka full
        if (child[i + 1]->n == 2 * deg - 1)
        {
            splitChildNode(i + 1, child[i + 1]);

            if (keys[i + 1]->votes < newKey->votes)
            {
                i++;
            }
        }
        child[i + 1]->insertKeyAtNode(newKey);
    }
}

void BTree::Node::splitChildNode(int i, Node* node)
{
    Node* newNode = new Node(node->deg);
    newNode->leaf = node->leaf;

    // new node will store #degrees -1 keys from the node to split
    newNode->n = deg - 1;

    int j = 0;
    for (j; j < deg - 1; j++)
    {
        newNode->keys[j] = node->keys[j + deg];
    }
    node->keys[j + deg] = nullptr; //can clear the unused pointer

    if (node->leaf == false)
    {
        //make sure all child nodes are still attached to newNode
        for (int j = 0; j < deg; j++)
            newNode->child[j] = node->child[j + deg];
    }

    node->n = deg - 1;

    // create space for the new child at the index provided
    for (int j = n; j >= i + 1; j--)
        child[j + 1] = child[j];

    // set the child to this new node
    child[i + 1] = newNode;

    // shift the keys after the new key was added
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = node->keys[deg - 1];

    // Increment count of keys
    n++;
}

stack<BTree::Key*> BTree::Node::TraverseNode(stack<Key*>& stack)
{
    int i;
    for (i = 0; i < n; i++)
    {
        // traverse the subtree rooted with child[i].
        if (leaf == false)
        {
            child[i]->TraverseNode(stack);
        }
        stack.push(keys[i]);
    }

    if (leaf == false)
        child[i]->TraverseNode(stack);

    return stack;
}

//outputs a vector of the top 10 county data by votes
vector<tuple<string, string, int, float>> BTree::TraverseTopTen()
{
    vector<Key*> topTenCounties;
    stack<Key*>stack;
    if (root != nullptr)
        stack = root->TraverseNode(stack);

    for (int i = 0; i < 10; i++)
    {
        if (!stack.empty())
        {
            topTenCounties.push_back(stack.top());
            stack.pop();
        }
    }
    vector<tuple<string, string, int, float>> topTenResults;
    for (int i = 0; i < topTenCounties.size(); i++)
    {
        tuple<string, string, int, float> result;
        get<0>(result) = topTenCounties[i]->county;
        get<1>(result) = topTenCounties[i]->state;
        get<2>(result) = topTenCounties[i]->votes;
        get<3>(result) = topTenCounties[i]->percVotes;
        topTenResults.push_back(result);
    }

    return topTenResults;
}

BTree::Node::~Node()
{
    delete[] keys;
    delete[] child;
}

BTree::~BTree()
{
    delete root;
}