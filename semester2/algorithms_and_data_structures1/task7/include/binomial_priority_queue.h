#ifndef BINOMIAL_PRIORITY_QUEUE_H
#define BINOMIAL_PRIORITY_QUEUE_H

#include "priority_queue.h"

class binomial_priority_queue : public priority_queue
{
private:
    struct Node 
    {
        char* _value;
        unsigned int _priority;
        int _degree; 
        Node* _parent;
        Node* _child;  
        Node* _sibling; 

        Node(
            char const* val, 
            unsigned int prio);
        ~Node();
    };

    Node* _head;  

    Node* merge_trees(
        Node* tree1, 
        Node* tree2);
   
    Node* merge_heaps(
        Node* heap1,
        Node* heap2);
   
    Node* find_min();
   
    void link_trees(
        Node* parent,
        Node* child);
  
    void consolidate();
  
    void clear_heap(Node* node);

public:
   
    binomial_priority_queue();
   
    ~binomial_priority_queue();

    binomial_priority_queue(
        binomial_priority_queue const& other);
    
    binomial_priority_queue& operator=(
        binomial_priority_queue const& other);

    void push(
        char const* value,
        unsigned int priority) override;
   
    char const* get_top_value() const override;
    
    void pop() override;
    
    priority_queue* merge(
        priority_queue* other) override;

    bool empty() const;
   
    Node* copy_heap(
        Node* node);
};

#endif // BINOMIAL_PRIORITY_QUEUE_H