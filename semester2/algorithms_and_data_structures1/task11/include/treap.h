#ifndef TREAP_H
#define TREAP_H

#include "priority_queue.h"
#include <cstdlib>
#include <cstring>
#include <ctime>

class treap :
    public priority_queue
{
private:
    struct Node
    {
        char* value;
        unsigned int priority;
        Node* left;
        Node* right;

        Node(
            const char* val, 
            unsigned int prio) : priority(prio), left(nullptr), right(nullptr)
        {
            size_t len = strlen(val);
            value = new char[len + 1];
            strcpy(value, val);
        }

        ~Node()
        {
            delete[] value;
        }
    };

    Node* root;

    static unsigned int random_priority();

    Node* rotate_right(
        Node* node);

    Node* rotate_left(
        Node* node);

    Node* insert(
        Node* node,
        const char* value,
        unsigned int priority);

    Node* remove_max(
        Node* node);

    Node* find_max(
        Node* node) const;

    Node* merge_trees(
        Node* tree1,
        Node* tree2);

    Node* copy_tree(
        Node* node);

    void destroy_tree(
        Node* node);

public:

    treap() : root(nullptr) {}

    ~treap();

    treap(
        const treap& other);

    treap& operator=(
        const treap& other);

    void push(
        const char* value,
        unsigned int const priority) override;

    void push(
        const char* value);

    char const* get_top_value() const override;

    void pop() override;

    priority_queue* merge(
        priority_queue* other) override;

    bool empty() const;
};

#endif //TREAP_H