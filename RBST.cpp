#include "RBST.hpp"
#include <cstdlib>
/***********************************************************/
/******************* PROVIDED FUNCTIONS ********************/
/***********************************************************/

int RBST::add(const Key& key, bool verbose) {
    unsigned int oldSize = m_size;
    m_head = randomAdd(m_head, key);
    if (m_size > oldSize) {
        if (verbose) {
            cout<<"Node "<<key<< " is added into the tree."<<endl;
        }
        return 1;
    } else {
        if (verbose) {
            cout<<"Node "<<key<< " is already in the tree."<<endl;
        }
        return 0;
    }
    if (verbose) cout<<endl;
};

int RBST::del(const Key& key, bool verbose) {
    unsigned oldSize= m_size;
    m_head = del(m_head, key);
    if (m_size < oldSize) {
        if (verbose) {
            cout<<"Node "<<key<< " is deleted from the tree."<<endl;
        }
        return 1;
    } else {
        if (verbose) {
            cout<< "Node "<<key<< " is not in the tree."<<endl;
        }
        return 0;
    }
};

int RBST::find(const Key& key, bool verbose) {
    RBSTNode* ret = find(m_head, key);
    if (ret == NULL) {
        if (verbose) {
            cout<< "Node "<<key<< " is not in the tree."<<endl;
        }
        return 0;
    } else {
        if (verbose) {
            cout<<"Node "<<key<< " is in the tree."<<endl;
        }
        return 1;
    }
};

int RBST::dump(char sep) {
    int ret = dump(m_head, sep);
    cout<<"SIZE: " <<ret<<endl;
    return ret;
};

int RBST::dump(RBSTNode* target, char sep) {
    if (target == NULL) {
        return 0;
    }
    int ret = dump(target->left(), sep);
    cout<< *target<<sep;
    ret ++;
    ret += dump(target->right(), sep);
    return ret;
};



/***********************************************************/
/****************  FUNCTIONS TO BE COMPLETED  ***************/
/***********************************************************/


/////////////////////////////////////////////////////////////
/////////////////////  ADD FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////

RBSTNode*  RBST::rightRotate(RBSTNode* target) {
    RBSTNode *l = target->left();
    target->setLeft(l->right());
    l->setRight(target);
    return l;
};

RBSTNode*  RBST::leftRotate(RBSTNode* target) {
    RBSTNode *r = target->right();
    target->setRight(r->left() );
    r->setLeft(target);
    return r;
};

RBSTNode* RBST::addRoot(RBSTNode* target, const Key& key) {
    countAdd++;

    if (target == NULL) return new RBSTNode(key);
    if (key < *target) 
    {
        target->setLeft(addRoot(target->left(),key));
        return rightRotate(target);
    }
    else 
    {
        target->setRight(addRoot(target->right(), key));
        return leftRotate(target);
    }
};


RBSTNode* RBST::randomAdd(RBSTNode* target, const Key& key) {
    countAdd++;
    
    if (target == NULL) 
    {
        m_size++;
        RBSTNode *t = new RBSTNode(key);
        t->setSize(t->getSize()+1);
        return t;
    }
    int r = rand() % (target -> getSize() + 1);

    /* Prob[r=1] = 1/(size(t)+1) */
    if (r == 1)
    {
        m_size++;
        target->setSize(target->getSize()+1);
        return addRoot(target, key);
    }

    if (key < *target){
        target->setLeft(randomAdd(target->left(), key));
    }
    else if (key > *target) 
    {
        target->setRight(randomAdd(target->right(), key));
    }
/*else{
return target;
}*/

    m_size++;
    target->setSize(target->getSize()+1);
    return target;
};


/////////////////////////////////////////////////////////////
/////////////////////  FIND FUNCTIONS ///////////////////////
/////////////////////////////////////////////////////////////
    
RBSTNode* RBST::find(RBSTNode* target, const Key& key) 
{
    countFind++;
    if (target != NULL) {
        if (key == *target) return target;
        else if (key < *target) return find(target->left(), key);
        else return find(target->right(), key);
}
else 
{
    return NULL;
}

}


/////////////////////////////////////////////////////////////
/////////////////////  DEL FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////

RBSTNode* RBST::deleteLeftMostNode(RBSTNode *target){
    if (target->left() == NULL)
    {
        return target->right();
    }
    else 
    {
        RBSTNode *child = deleteLeftMostNode(target->left());
        target->setLeft(child);
        return target;
    }
}

RBSTNode* RBST::getLeftMostNode(RBSTNode* target)
{
    if (target -> left() ==NULL){
        return target;
    }
        return getLeftMostNode(target->left());
}

RBSTNode* RBST::removeNode(RBSTNode* target){
    //both subtrees present
    if (target -> right() != NULL && target -> left() != NULL)
    {
        RBSTNode *replacementNode = getLeftMostNode(target->right());
        RBSTNode *newRight = deleteLeftMostNode(target->right());
        replacementNode -> setRight(newRight);
        replacementNode -> setLeft(target->left());
        return replacementNode;
    }
    //one or the other subtrees arent null
    else 
    {
        if (target -> right() != NULL && target -> left() == NULL)
        {
            return target -> right();
        }
        else if(target -> left() != NULL && target -> right() == NULL)
        {
            return target -> left();
        }
        else 
        {
            return NULL;
        }
    }
}


RBSTNode* RBST::del(RBSTNode* target, const Key& key) {
    countDelete++;
    if (target == NULL)
    {
        return NULL;
    }

    if (key < *target)
    {
        RBSTNode *newLeft = del(target->left(), key);
        target->setLeft(newLeft);
    }
    else if (key > *target)
    {
        RBSTNode * newRight = del(target->right(), key);
        target->setRight(newRight);
    }
    else {
        target = removeNode(target);
        m_size--;
    }

    if (target != NULL) target->setSize(target->getSize()-1);
    return target;
};


