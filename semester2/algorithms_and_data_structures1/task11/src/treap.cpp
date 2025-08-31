#include "treap.h"

unsigned int treap::random_priority()
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    return rand();
}

treap::Node* treap::rotate_right(
    Node* node)
{
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    return new_root;
}

treap::Node* treap::rotate_left(
    Node* node)
{
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    return new_root;
}

treap::Node* treap::insert(
    Node* node,
    const char* value,
    unsigned int priority)
{
    if (node == nullptr)
    {
        return new Node(value, priority);
    }

    int cmp = strcmp(value, node->value);

    if (cmp < 0)
    {
        node->left = insert(node->left, value, priority);

        if (node->left->priority > node->priority)
        {
            node = rotate_right(node);
        }
    }
    else if (cmp > 0)
    {
        node->right = insert(node->right, value, priority);

        if (node->right->priority > node->priority)
        {
            node = rotate_left(node);
        }
    }

    return node;
}

treap::Node* treap::remove_max(
    Node* node)
{
    if (node == nullptr)
        return nullptr;

    if (node->left == nullptr && node->right == nullptr)
    {
        delete node;
        return nullptr;
    }
    else if (node->left == nullptr)
    {
        node = rotate_left(node);
        node->left = remove_max(node->left);
    }
    else if (node->right == nullptr)
    {
        node = rotate_right(node);
        node->right = remove_max(node->right);
    }
    else
    {
        if (node->left->priority > node->right->priority)
        {
            node = rotate_right(node);
            node->right = remove_max(node->right);
        }
        else
        {
            node = rotate_left(node);
            node->left = remove_max(node->left);
        }
    }

    return node;
}

treap::Node* treap::find_max(
    Node* node) const
{
    if (node == nullptr)
        return nullptr;

    Node* max_node = node;
    Node* left_max = find_max(node->left);
    Node* right_max = find_max(node->right);

    if (left_max != nullptr && left_max->priority > max_node->priority)
        max_node = left_max;
    if (right_max != nullptr && right_max->priority > max_node->priority)
        max_node = right_max;

    return max_node;
}

treap::Node* treap::merge_trees(
    Node* tree1,
    Node* tree2)
{
    if (tree1 == nullptr)
        return tree2;
    if (tree2 == nullptr)
        return tree1;

    if (tree1->priority > tree2->priority)
    {
        tree1->right = merge_trees(tree1->right, tree2);
        return tree1;
    }
    else
    {
        tree2->left = merge_trees(tree1, tree2->left);
        return tree2;
    }
}

treap::Node* treap::copy_tree(
    Node* node)
{
    if (node == nullptr)
        return nullptr;

    Node* new_node = new Node(node->value, node->priority);
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    return new_node;
}

void treap::destroy_tree(
    Node* node)
{
    if (node != nullptr)
    {
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }
}

treap::~treap()
{
    destroy_tree(root);
}

treap::treap(
    const treap& other) : root(nullptr)
{
    root = copy_tree(other.root);
}

treap& treap::operator=(
    const treap& other)
{
    if (this != &other)
    {
        destroy_tree(root);
        root = copy_tree(other.root);
    }

    return *this;
}

void treap::push(
    const char* value,
    unsigned int const priority) 
{
    root = insert(root, value, priority);
}

void treap::push(
    const char* value)
{
    push(value, random_priority());
}

char const* treap::get_top_value() const 
{
    Node* max_node = find_max(root);

    return max_node ? max_node->value : nullptr;
}

void treap::pop() 
{
    if (root != nullptr)
    {
        root = remove_max(root);
    }
}

priority_queue* treap::merge(
    priority_queue* other) 
{
    treap* other_treap = dynamic_cast<treap*>(other);

    if (other_treap == nullptr)
        return nullptr;

    root = merge_trees(root, copy_tree(other_treap->root));

    return this;
}

bool treap::empty() const
{
    return root == nullptr;
}