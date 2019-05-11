#include "fptree/fptree.h"
#include "utility/p_allocator.h"
using namespace std;

// Initial the new InnerNode
InnerNode::InnerNode(const int &d, FPTree *const &t, bool _isRoot)
{
    // TODO
    this->tree = t;
    this->degree = d;
    this->isLeaf = false;

    this->isRoot = _isRoot;
    this->nKeys = 0;
    this->nChild = 0;
    this->keys = new Key[2 * d + 1];
    this->childrens = new Node *[2 * d + 2];
}

// delete the InnerNode
InnerNode::~InnerNode()
{
    // TODO
    delete[] this->keys;
    delete[] this->childrens;
}

// binary search the first key in the innernode larger than input key
int InnerNode::findIndex(const Key &k)
{
    // TODO
    int low = 0, high = nKeys, middle = 0;
    while (low < high)
    {
        middle = (low + high) / 2;
        if (k == this->keys[middle])
        {
            return middle + 1;
        }
        else if (k < this->keys[middle])
        {
            high = middle;
        }
        else if (k > this->keys[middle])
        {
            low = middle + 1;
        }
    }
    return low;
}

// insert the node that is assumed not full
// insert format:
// ======================
// | key | node pointer |
// ======================
// WARNING: can not insert when it has no entry
void InnerNode::insertNonFull(const Key &k, Node *const &node)
{
    // TODO
    if (this->nChild > 0 && this->nKeys < 2 * this->degree + 1)
    {
        int index = this->findIndex(k);
        for (int i = this->nKeys - 1; i >= index; i--)
        {
            this->key[i + 1] = this->key[i];
        }
        this->key[index] = k;
        this->nKeys++;
        for (int i = this->nChild - 1; i >= index + 1; i--)
        {
            this->childrens[i + 1] = this->childrens[i];
        }
        this->childrens[index + 1] = node;
        this->nChild++;
    }
}

// insert func
// return value is not NULL if split, returning the new child and a key to insert
KeyNode *InnerNode::insert(const Key &k, const Value &v)
{
    KeyNode *newChild = NULL;

    // 1.insertion to the first leaf(only one leaf)
    if (this->isRoot && this->nKeys == 0)
    {
        // TODO
        LeafNode *ln = new LeafNode(this->tree);
        ln.insertNonFull(k, v);
        KeyNode leaf;
        leaf.key = k;
        leaf.node = ln;
        this->insertLeaf(leaf);
        return newChild;
    }

    // 2.recursive insertion
    // TODO
    int index = this->findIndex(k);
    KeyNode *kn = this->childrens[index]->insert(k, v);
    if (kn != NULL)
    {
        this->insertNonFull(kn->key, kn->node);
        if (this->nKeys > this->degree * 2)
        {
            newChild = this->split();
            if (this->isRoot)
            {
                this->isRoot = false;
                InnerNode newRoot(this->degree, this->tree, true);
                (*this->tree).changeRoot(&newRoot);
                newRoot.childrens[0] = this;
                this->nChild++;
                newRoot.insertNonFull(newChild->key, newChild->node);
                newChild = NULL;
            }
        }
    }
    return newChild;
}

// ensure that the leaves inserted are ordered
// used by the bulkLoading func
// inserted data: | minKey of leaf | LeafNode* |
KeyNode *InnerNode::insertLeaf(const KeyNode &leaf)
{
    KeyNode *newChild = NULL;
    // first and second leaf insertion into the tree
    if (this->isRoot && this->nKeys == 0)
    {
        // TODO
        this->childrens[this->nChild] = leaf.node;
        this->nChild++;
        if (this->nChild == 2)
        {
            this->nKeys++;
            this->keys[0] = leaf.key;
        }
        return newChild;
    }

    // recursive insert
    // Tip: please judge whether this InnerNode is full
    // next level is not leaf, just insertLeaf
    // TODO
    KeyNode *kn = NULL;
    if (!(*this->childrens[0]).isLeaf)
    {
        int index = this->findIndex(leaf.key);
        kn = (*this->childrens[index]).insertLeaf(leaf);
    }
    // next level is leaf, insert to childrens array
    // TODO
    else
        kn = leaf;
    if (kn != NULL)
    {
        this->insertNonFull(kn->key, kn->node);
        if (this->nKeys > 2 * this->degree)
        {
            newChild = this->split();
            if (this->isRoot)
            {
                this->isRoot = false;
                InnerNode newRoot(this->degree, this->tree, true);
                (*this->tree).changeRoot(&newRoot);
                newRoot.childrens[0] = this;
                this->nChild++;
                newRoot.insertNonFull(newChild->key, newChild->node);
                newChild = NULL;
            }
        }
    }
    return newChild;
}

KeyNode *InnerNode::split()
{
    KeyNode *newChild = new KeyNode();
    // right half entries of old node to the new node, others to the old node.
    // TODO
    newChild->key = this->keys[this->degree];
    InnerNode newIn* = new InnerNode(this->degree, this->tree, false);
    newIn->nKeys = this->degree;
    newIn->nChild = this->degree + 1;
    newIn->childrens[0] = this->childrens[this->degree + 1];
    for (uint64_t i = 0; i < this->degree; i++)
    {
        newIn->keys[i] = this->keys[this->degree + 1 + i];
        newIn->childrens[i + 1] = this->childrens[this->degree + 2 + i];
    }
    newChild.node = newIn;
    this->nKeys = this->degree;
    this->nChild = this->degree + 1;
    return newChild;
}

// remove the target entry
// return TRUE if the children node is deleted after removement.
// the InnerNode need to be redistributed or merged after deleting one of its children node.
bool InnerNode::remove(const Key &k, const int &index, InnerNode *const &parent, bool &ifDelete)
{
    bool ifRemove = false;
    // only have one leaf
    // TODO

    // recursive remove
    // TODO
    return ifRemove;
}

// If the leftBro and rightBro exist, the rightBro is prior to be used
void InnerNode::getBrother(const int &index, InnerNode *const &parent, InnerNode *&leftBro, InnerNode *&rightBro)
{
    // TODO
}

// merge this node, its parent and left brother(parent is root)
void InnerNode::mergeParentLeft(InnerNode *const &parent, InnerNode *const &leftBro)
{
    // TODO
}

// merge this node, its parent and right brother(parent is root)
void InnerNode::mergeParentRight(InnerNode *const &parent, InnerNode *const &rightBro)
{
    // TODO
}

// this node and its left brother redistribute
// the left has more entries
void InnerNode::redistributeLeft(const int &index, InnerNode *const &leftBro, InnerNode *const &parent)
{
    // TODO
}

// this node and its right brother redistribute
// the right has more entries
void InnerNode::redistributeRight(const int &index, InnerNode *const &rightBro, InnerNode *const &parent)
{
    // TODO
}

// merge all entries to its left bro, delete this node after merging.
void InnerNode::mergeLeft(InnerNode *const &leftBro, const Key &k)
{
    // TODO
}

// merge all entries to its right bro, delete this node after merging.
void InnerNode::mergeRight(InnerNode *const &rightBro, const Key &k)
{
    // TODO
}

// remove a children from the current node, used by remove func
void InnerNode::removeChild(const int &keyIdx, const int &childIdx)
{
    // TODO
}

// update the target entry, return true if the update succeed.
bool InnerNode::update(const Key &k, const Value &v)
{
    // TODO
    return false;
}

// find the target value with the search key, return MAX_VALUE if it fails.
Value InnerNode::find(const Key &k)
{
    // TODO
    return MAX_VALUE;
}

// get the children node of this InnerNode
Node *InnerNode::getChild(const int &idx)
{
    // TODO
    return NULL;
}

// get the key of this InnerNode
Key InnerNode::getKey(const int &idx)
{
    if (idx < this->nKeys)
    {
        return this->keys[idx];
    }
    else
    {
        return MAX_KEY;
    }
}

// print the InnerNode
void InnerNode::printNode()
{
    cout << "||#|";
    for (int i = 0; i < this->nKeys; i++)
    {
        cout << " " << this->keys[i] << " |#|";
    }
    cout << "|"
         << "    ";
}

// print the LeafNode
void LeafNode::printNode()
{
    cout << "||";
    for (int i = 0; i < 2 * this->degree; i++)
    {
        if (this->getBit(i))
        {
            cout << " " << this->kv[i].k << " : " << this->kv[i].v << " |";
        }
    }
    cout << "|"
         << " ====>> ";
}

// new a empty leaf and set the valuable of the LeafNode
LeafNode::LeafNode(FPTree *t)
{
    // TODO
    tree = t;
    degree = t->degree;
    isLeaf = 1;
    PAllocator::getAllocator()->getLeaf(pPointer, pmem_addr);
    bitmap = new Byte[degree * 2];
    memset(bitmap, '0', degree);
    fingerprints = new Byte[degree * 2];
    kv = new KeyValue[degree * 2];
    bitmapSize = degree / 4;
}

// reload the leaf with the specific Persistent Pointer
// need to call the PAllocator
LeafNode::LeafNode(PPointer p, FPTree *t)
{
    // TODO
    tree = t;
    isLeaf = 1;
    pPointer = p;
    pmem_addr = PAllocator::getAllocator()->getLeafPmemAddr(p);

    //Load from file???
}

LeafNode::~LeafNode()
{
    // TODO
    PAllocator::getAllocator()->freeLeaf(this->pPointer);
}

// insert an entry into the leaf, need to split it if it is full
KeyNode *LeafNode::insert(const Key &k, const Value &v)
{
    KeyNode *newChild = NULL;
    // TODO
    Key splitKey = findSplitKey();

    int slot = LeafNode::findFirstZero();
    kv[slot] = (k, v);
    fingerprints[slot] = hash(k);
    bitmap[(idx / 8) & (1 << (idx % 8))] = 1;
    persist();

    return newChild;
}

// insert into the leaf node that is assumed not full
void LeafNode::insertNonFull(const Key &k, const Value &v)
{
    // TODO
    int slot = LeafNode::findFirstZero();
    kv[slot] = (k, v);
    fingerprints[slot] = hash(k);
    bitmap[(idx / 8) & (1 << (idx % 8))] = 1;
    persist();
}

// split the leaf node
KeyNode *LeafNode::split()
{
    KeyNode *newChild = new KeyNode();
    // TODO
    LeafNode newLeafNode = new LeafNode(this->tree);
    // Byte*      bitmap;
    // PPointer*  pNext;
    // Byte*      fingerprints;
    // KeyValue*  kv;
    // int        n;
    // uint64_t   bitmapSize;
    newLeafNode->persist();
    Key SplitKey = findSplitKey();
    for (int i = 0; i < bitmapSize; ++i)
    {
        newLeafNode->bitmap[i] = this->bitmap[i];
        this->bitmap[i] = !newLeafNode->bitmap[i];
    }
    newLeafNode->persist();
    persist();
    for (int i = 0; i < this->degree * 2; ++i)
    {
        newLeafNode->fingerprints[i] = this->fingerprints[i];
        newLeafNode->kv[i] = this->kv[i];
    }
    newLeafNode->persist();
    this->pNext = newLeafNode->pPointer;
    persist();
    newChild = newLeafNode;
    return newChild;
}

// use to find a mediant key and delete entries less then middle
// called by the split func to generate new leaf-node
// qsort first then find
Key LeafNode::findSplitKey()
{
    Key midKey = 0;
    // TODO

    return midKey;
}

// get the targte bit in bitmap
// TIPS: bit operation
int LeafNode::getBit(const int &idx)
{
    // TODO
    return this->bitmap[(idx / 8) & (1 << (idx % 8))];
}

Key LeafNode::getKey(const int &idx)
{
    return this->kv[idx].k;
}

Value LeafNode::getValue(const int &idx)
{
    return this->kv[idx].v;
}

PPointer LeafNode::getPPointer()
{
    return this->pPointer;
}

// remove an entry from the leaf
// if it has no entry after removement return TRUE to indicate outer func to delete this leaf.
// need to call PAllocator to set this leaf free and reuse it
bool LeafNode::remove(const Key &k, const int &index, InnerNode *const &parent, bool &ifDelete)
{
    bool ifRemove = false;
    // TODO
    return ifRemove;
}

// update the target entry
// return TRUE if the update succeed
bool LeafNode::update(const Key &k, const Value &v)
{
    bool ifUpdate = false;
    // TODO
    return ifUpdate;
}

// if the entry can not be found, return the max Value
Value LeafNode::find(const Key &k)
{
    // TODO
    return MAX_VALUE;
}

// find the first empty slot
int LeafNode::findFirstZero()
{
    // TODO
    return -1;
}

// persist the entire leaf
// use PMDK
void LeafNode::persist()
{
    // TODO
}

// call by the ~FPTree(), delete the whole tree
void FPTree::recursiveDelete(Node *n)
{
    if (n->isLeaf)
    {
        delete n;
    }
    else
    {
        for (int i = 0; i < ((InnerNode *)n)->nChild; i++)
        {
            recursiveDelete(((InnerNode *)n)->childrens[i]);
        }
        delete n;
    }
}

FPTree::FPTree(uint64_t t_degree)
{
    FPTree *temp = this;
    this->root = new InnerNode(t_degree, temp, true);
    this->degree = t_degree;
    bulkLoading();
}

FPTree::~FPTree()
{
    recursiveDelete(this->root);
}

// get the root node of the tree
InnerNode *FPTree::getRoot()
{
    return this->root;
}

// change the root of the tree
void FPTree::changeRoot(InnerNode *newRoot)
{
    this->root = newRoot;
}

void FPTree::insert(Key k, Value v)
{
    if (root != NULL)
    {
        root->insert(k, v);
    }
}

bool FPTree::remove(Key k)
{
    if (root != NULL)
    {
        bool ifDelete = false;
        InnerNode *temp = NULL;
        return root->remove(k, -1, temp, ifDelete);
    }
    return false;
}

bool FPTree::update(Key k, Value v)
{
    if (root != NULL)
    {
        return root->update(k, v);
    }
    return false;
}

Value FPTree::find(Key k)
{
    if (root != NULL)
    {
        return root->find(k);
    }
}

// call the InnerNode and LeafNode print func to print the whole tree
// TIPS: use Queue
void FPTree::printTree()
{
    // TODO
}

// bulkLoading the leaf files and reload the tree
// need to traverse leaves chain
// if no tree is reloaded, return FALSE
// need to call the PALlocator
bool FPTree::bulkLoading()
{
    // TODO
    return false;
}
