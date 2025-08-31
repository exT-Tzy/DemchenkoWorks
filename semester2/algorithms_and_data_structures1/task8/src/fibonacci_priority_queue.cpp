#include "fibonacci_priority_queue.h"

void fibonacci_priority_queue::add_to_root_list(
    Node* node)
{
    if (min_node == nullptr)
    {
        min_node = node;
        node->left = node->right = node;
    }
    else
    {
        node->right = min_node->right;
        node->left = min_node;

        min_node->right->left = node;
        min_node->right = node;

        if (node->priority < min_node->priority)
        {
            min_node = node;
        }
    }
}

void fibonacci_priority_queue::remove_from_list(
    Node* node)
{
    if (node == node->right)
    {
        return;
    }

    node->left->right = node->right;
    node->right->left = node->left;
}

void fibonacci_priority_queue::link_trees(
    Node* y,
    Node* x)
{
    remove_from_list(y);
    y->parent = x;

    if (x->child == nullptr)
    {
        x->child = y;
        y->left = y->right = y;
    }
    else
    {
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
    y->marked = false;
}

void fibonacci_priority_queue::consolidate()
{
    if (min_node == nullptr) return;

    int max_degree = static_cast<int>(log2(node_count)) + 1;
    std::vector<Node*> degree_table(max_degree, nullptr);

    std::vector<Node*> root_list;
    Node* current = min_node;
    do
    {
        root_list.push_back(current);
        current = current->right;
    } while (current != min_node);

    for (Node* w : root_list)
    {
        Node* x = w;
        int d = x->degree;

        while (degree_table[d] != nullptr)
        {
            Node* y = degree_table[d];

            if (x->priority > y->priority)
            {
                std::swap(x, y);
            }

            link_trees(y, x);
            degree_table[d] = nullptr;
            d++;
        }

        degree_table[d] = x;
    }

    min_node = nullptr;
    for (Node* node : degree_table)
    {
        if (node != nullptr)
        {
            if (min_node == nullptr || node->priority < min_node->priority)
            {
                min_node = node;
            }
        }
    }
}

void fibonacci_priority_queue::destroy_tree(
    Node* node)
{
    if (node == nullptr) return;

    Node* current = node;
    do
    {
        Node* next = current->right;
        destroy_tree(current->child);
        delete current;
        current = next;
    } while (current != node);
}

fibonacci_priority_queue::Node* fibonacci_priority_queue::copy_tree(
    Node* node,
    Node* parent = nullptr)
{
    if (node == nullptr) return nullptr;

    Node* new_node = new Node(node->value, node->priority);
    new_node->parent = parent;
    new_node->degree = node->degree;
    new_node->marked = node->marked;

    if (node->child != nullptr)
    {
        new_node->child = copy_tree(node->child, new_node);

        Node* child = new_node->child;
        Node* original_child = node->child->right;

        while (original_child != node->child)
        {
            Node* new_child = copy_tree(original_child, new_node);
            new_child->right = child->right;
            new_child->left = child;
            child->right->left = new_child;
            child->right = new_child;
            original_child = original_child->right;
        }
    }

    return new_node;
}

fibonacci_priority_queue::fibonacci_priority_queue(
    const fibonacci_priority_queue& other)
    : min_node(nullptr), node_count(other.node_count)
{
    if (other.min_node != nullptr)
    {
        min_node = copy_tree(other.min_node);

        Node* current = other.min_node->right;

        while (current != other.min_node)
        {
            Node* new_node = copy_tree(current);
            add_to_root_list(new_node);
            current = current->right;
        }
    }
}

fibonacci_priority_queue::~fibonacci_priority_queue()
{
    if (min_node != nullptr)
    {
        Node* current = min_node;
        do
        {
            Node* next = current->right;
            destroy_tree(current->child);
            delete current;
            current = next;
        } while (current != min_node);
    }
}

void fibonacci_priority_queue::push(
    const char* value,
    unsigned int priority) 
{
    Node* new_node = new Node(value, priority);
    add_to_root_list(new_node);
    node_count++;
}

char const* fibonacci_priority_queue::get_top_value() const 
{
    if (min_node == nullptr) return nullptr;

    return min_node->value;
}

void fibonacci_priority_queue::pop()
{
    if (min_node == nullptr) return;

    Node* z = min_node;

    if (z->child != nullptr)
    {
        Node* child = z->child;
        do
        {
            Node* next = child->right;
            child->parent = nullptr;
            add_to_root_list(child);
            child = next;
        } while (child != z->child);
    }

    if (z == z->right)
    {
        min_node = nullptr;
    }
    else
    {
        min_node = z->right;
        remove_from_list(z);
        consolidate();
    }

    delete z;
    node_count--;
}

priority_queue* fibonacci_priority_queue::merge(
    priority_queue* other_pq) 
{
    fibonacci_priority_queue* other =
        dynamic_cast<fibonacci_priority_queue*>(other_pq);

    if (other == nullptr) return this;

    if (this->min_node == nullptr)
    {
        this->min_node = other->min_node;
    }
    else if (other->min_node != nullptr)
    {
        Node* this_last = this->min_node->left;
        Node* other_last = other->min_node->left;

        this_last->right = other->min_node;
        other->min_node->left = this_last;
        other_last->right = this->min_node;
        this->min_node->left = other_last;

        if (other->min_node->priority < this->min_node->priority)
        {
            this->min_node = other->min_node;
        }
    }

    this->node_count += other->node_count;

    other->min_node = nullptr;
    other->node_count = 0;

    return this;
}

bool fibonacci_priority_queue::empty() const
{
    return min_node == nullptr;
}

int fibonacci_priority_queue::size() const
{
    return node_count;
}