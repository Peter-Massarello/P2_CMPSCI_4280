#ifndef NODE_H
#define NODE_H
#include "token.hpp"

enum NodeType {

};

typedef struct Node {

    struct Node *child1;
    struct Node *child2;
} Node;

#endif