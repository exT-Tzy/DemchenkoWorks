#ifndef FIBONACCI_PRIORITY_QUEUE_H
#define FIBONACCI_PRIORITY_QUEUE_H
#define _CRT_SECURE_NO_WARNINGS

#include "priority_queue.h"
#include <cstring>
#include <vector>
#include <cmath>

class fibonacci_priority_queue : 
    public priority_queue
{
private:
    struct Node
    {
        char* value;   
        unsigned int priority;
        Node* parent;
        Node* child; 
        Node* left; 
        Node* right;    
        int degree;  
        bool marked;   

        Node(
            const char* val,
            unsigned int prio)
            : priority(prio), parent(nullptr), child(nullptr),
            degree(0), marked(false)
        {
            int len = strlen(val);
            value = new char[len + 1];
            strcpy(value, val);

            left = right = this;
        }

        ~Node()
        {
            delete[] value;
        }
    };

    Node* min_node;
    int node_count; 

    void add_to_root_list(
        Node* node);

    void remove_from_list(
        Node* node);

    void link_trees(
        Node* y,
        Node* x);

    void consolidate();

    void destroy_tree(
        Node* node);

    Node* copy_tree(
        Node* node,
        Node* parent = nullptr);

public:

    fibonacci_priority_queue() : min_node(nullptr), node_count(0) {}

    fibonacci_priority_queue(
        const fibonacci_priority_queue& other);

    virtual ~fibonacci_priority_queue();

    virtual void push(
        const char* value,
        unsigned int priority) override;

    virtual char const* get_top_value() const override;

    virtual void pop() override;

    virtual priority_queue* merge(
        priority_queue* other_pq) override;

    bool empty() const;

    int size() const;
};

#endif // FIBONACCI_PRIORITY_QUEUE_H