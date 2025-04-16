#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>
#include <cstring>

#include "skew_priority_queue.h"

skew_priority_queue::node* skew_priority_queue::merge_trees(
	node*& x,
	node*& y)
{
	if (x == nullptr)
		return y;
	if (y == nullptr)
		return x;

	if (x->_priority < y->_priority)
		swap_nodes(x, y);

	x->_right = merge_trees(x->_right, y);

	return x;
}

priority_queue* skew_priority_queue::merge(
	priority_queue* other)
{
	if (other == nullptr) throw std::invalid_argument("other cant be nullptr!");

	skew_priority_queue* spq_ptr = reinterpret_cast<skew_priority_queue*>(other);

	if (this->_root == nullptr && spq_ptr->_root == nullptr)
		return this;

	if (this->_root == nullptr && spq_ptr->_root != nullptr)
	{
		_root = spq_ptr->_root;

		return other;
	}

	if (this->_root != nullptr && spq_ptr->_root == nullptr)
		return this;

	node* tree_copy = copy_tree(spq_ptr->_root);

	_root = merge_trees(_root, tree_copy);

	destroy_tree(spq_ptr->_root);

	return this;
}

skew_priority_queue::skew_priority_queue()
{
	_root = nullptr;
}

skew_priority_queue::skew_priority_queue(
	skew_priority_queue const& src)
{
	_root = copy_tree(src._root);
}

skew_priority_queue& skew_priority_queue::operator=(
	skew_priority_queue const& src)
{
	if (this == &src) return *this;

	_root = copy_tree(src._root);

	return *this;
}

skew_priority_queue::~skew_priority_queue() noexcept
{
	destroy_tree(_root);
}