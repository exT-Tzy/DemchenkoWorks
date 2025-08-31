#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>
#include <cstring>

#include "binomial_priority_queue.h"

binomial_priority_queue::Node::Node(
    char const* val, 
    unsigned int prio)
    : _priority(prio), _degree(0), _parent(nullptr), _child(nullptr), _sibling(nullptr)
{
    _value = new char[strlen(val) + 1];
    strcpy(_value, val);
}

binomial_priority_queue::Node::~Node()
{
    delete[] _value;
}

binomial_priority_queue::binomial_priority_queue() : _head(nullptr) {}

binomial_priority_queue::~binomial_priority_queue()
{
    clear_heap(_head);
}

binomial_priority_queue::binomial_priority_queue(
    binomial_priority_queue const& other)
    : _head(nullptr)
{
    if (other._head != nullptr) 
    {
        _head = copy_heap(other._head);
    }
}

binomial_priority_queue& binomial_priority_queue::operator=(
    binomial_priority_queue const& other)
{
    if (this != &other) 
    {
        clear_heap(_head);
        _head = nullptr;
        
        if (other._head != nullptr) 
        {
            _head = copy_heap(other._head);
        }
    }
    
    return *this;
}

binomial_priority_queue::Node* binomial_priority_queue::copy_heap(
    Node* node)
{
    if (node == nullptr) return nullptr;

    Node* new_node = new Node(node->_value, node->_priority);
    new_node->_degree = node->_degree;

    if (node->_child != nullptr) 
    {
        new_node->_child = copy_heap(node->_child);
        new_node->_child->_parent = new_node;
    }

    if (node->_sibling != nullptr) 
    {
        new_node->_sibling = copy_heap(node->_sibling);
    }

    return new_node;
}

void binomial_priority_queue::clear_heap(
    Node* node)
{
    if (node == nullptr) return;

    clear_heap(node->_child);
    clear_heap(node->_sibling);
    
    delete node;
}

void binomial_priority_queue::link_trees(
    Node* parent,
    Node* child)
{
    child->_parent = parent;
    child->_sibling = parent->_child;
    parent->_child = child;
    parent->_degree++;
}

binomial_priority_queue::Node* binomial_priority_queue::merge_trees(
    Node* tree1, 
    Node* tree2)
{
    if (tree1 == nullptr) return tree2;
    if (tree2 == nullptr) return tree1;

    if (tree1->_priority < tree2->_priority) 
    {
        link_trees(tree2, tree1);
       
        return tree2;
    }
    else 
    {
        link_trees(tree1, tree2);
        
        return tree1;
    }
}

binomial_priority_queue::Node* binomial_priority_queue::merge_heaps(
    Node* heap1,
    Node* heap2)
{
    if (heap1 == nullptr) return heap2;
    if (heap2 == nullptr) return heap1;

    Node* merged = nullptr;
    Node** current = &merged;

    while (heap1 != nullptr && heap2 != nullptr) 
    {
        if (heap1->_degree <= heap2->_degree) 
        {
            *current = heap1;
            heap1 = heap1->_sibling;
        }
        else 
        {
            *current = heap2;
            heap2 = heap2->_sibling;
        }
       
        current = &((*current)->_sibling);
    }

    *current = (heap1 != nullptr) 
        ? heap1 
        : heap2;

    return merged;
}

void binomial_priority_queue::consolidate()
{
    if (_head == nullptr) return;

    Node* prev = nullptr;
    Node* current = _head;
    Node* next = _head->_sibling;

    while (next != nullptr) 
    {
        if (current->_degree != next->_degree ||
            (next->_sibling != nullptr && next->_sibling->_degree == current->_degree)) 
        {
            prev = current;
            current = next;
        }
        else 
        {
            if (current->_priority >= next->_priority) 
            {
                current->_sibling = next->_sibling;
                link_trees(current, next);
            }
            else 
            {
                if (prev == nullptr) 
                {
                    _head = next;
                }
                else 
                {
                    prev->_sibling = next;
                }
               
                link_trees(next, current);
                current = next;
            }
        }
        next = current->_sibling;
    }
}

binomial_priority_queue::Node* binomial_priority_queue::find_min()
{
    if (_head == nullptr) return nullptr;

    Node* min_node = _head;
    Node* current = _head->_sibling;

    while (current != nullptr) 
    {
        if (current->_priority > min_node->_priority) 
        {
            min_node = current;
        }
       
        current = current->_sibling;
    }

    return min_node;
}

void binomial_priority_queue::push(
    const char* value,
    unsigned int priority)
{
    Node* new_node = new Node(value, priority);
    _head = merge_heaps(_head, new_node);
    consolidate();
}

char const* binomial_priority_queue::get_top_value() const
{
    Node* max_node = const_cast<binomial_priority_queue*>(this)->find_min();
    
    return (max_node != nullptr) ? max_node->_value : nullptr;
}

void binomial_priority_queue::pop()
{
    if (_head == nullptr) return;

    Node* max_node = find_min();

    if (max_node == _head) 
    {
        _head = _head->_sibling;
    }
    else 
    {
        Node* current = _head;
       
        while (current->_sibling != max_node) 
        {
            current = current->_sibling;
        }
       
        current->_sibling = max_node->_sibling;
    }

    Node* child_heap = nullptr;
    Node* child = max_node->_child;

    while (child != nullptr) 
    {
        Node* next = child->_sibling;
        child->_sibling = child_heap;
        child->_parent = nullptr;
        child_heap = child;
        child = next;
    }

    _head = merge_heaps(_head, child_heap);
    consolidate();

    delete max_node;
}

priority_queue* binomial_priority_queue::merge(
    priority_queue* other)
{
    binomial_priority_queue* other_binomial = dynamic_cast<binomial_priority_queue*>(other);
    
    if (other_binomial == nullptr) 
    {
        return this;
    }

    _head = merge_heaps(_head, other_binomial->_head);
    other_binomial->_head = nullptr;
    consolidate();

    return this;
}

bool binomial_priority_queue::empty() const
{
    return _head == nullptr;
}