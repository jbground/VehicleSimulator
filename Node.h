#pragma once

#include "stddef.h"

class Node {

public:
    Node* parent;
    int x;
    int y;

    int G;
    int H;
    int F;


    Node(int x, int y) {
        parent = NULL;
        this->x = x;
        this->y = y;

        this->G = 0;
        this->F = 0;
        this->H = 0;
    }

    Node(Node* parent, int x, int y) {
        this->parent = parent;
        this->x = x;
        this->y = y;

        this->G = 0;
        this->F = 0;
        this->H = 0;
    }

    Node* getParent() {
        return parent;
    }

    bool operator<(const Node n) const {
        if (this->F == n.F) {
            return this->H > n.H;
        }
        else {
            return this->F > n.F;
        }
    }

    bool operator==(const Node& n) const {
        return x == n.x && y == n.y;
    }
};