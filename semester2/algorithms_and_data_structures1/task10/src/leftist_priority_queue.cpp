#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>
#include <cstring>

#include "leftist_priority_queue.h"

void leftist_priority_queue::swap_nodes(
	node*& x,
	node*& y)
{
	auto t = x;
	x = y;
	y = t;
}

leftist_priority_queue::node* leftist_priority_queue::get_right_child(
	node const* node_ptr) 
{
	if (node_ptr == nullptr) throw std::invalid_argument("node_ptr is nullptr!");

	return node_ptr->_right;
}

leftist_priority_queue::node* leftist_priority_queue::get_left_child(
	node const* node_ptr) 
{
	if (node_ptr == nullptr) throw std::invalid_argument("node_ptr is nullptr!");

	return node_ptr->_left;
}

unsigned int leftist_priority_queue::rang(
	node const* node_ptr) 
{
	if (node_ptr == nullptr) return 0;

	unsigned int left_rang, right_rang;

	return 1 + ((left_rang = rang(get_left_child(node_ptr)) <= (right_rang = rang(get_right_child(node_ptr)))))
		? left_rang
		: right_rang;
}

leftist_priority_queue::node* leftist_priority_queue::copy_tree(
	node const* root)
{
	if (root == nullptr) return nullptr;

	node* new_root = new node;
	
	new_root->_left = nullptr;
	new_root->_right = nullptr;
	new_root->_priority = root->_priority;
	
	new_root->_value = new char[strlen(root->_value) + 1];
	strcpy(new_root->_value, root->_value);

	if (root->_left)
		new_root->_left = copy_tree(root->_left);
	if (root->_right)
		new_root->_right = copy_tree(root->_right);

	return new_root;
}

void leftist_priority_queue::destroy_tree(
	node*& root)
{
	if (root == nullptr) return;

	destroy_tree(root->_left);
	destroy_tree(root->_right);

	delete[] root->_value;
	delete root;
}

leftist_priority_queue::node* leftist_priority_queue::merge_trees(
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

	if (rang(x->_left) < rang(x->_right))
		swap_nodes(x->_left, x->_right);

	return x;
}

bool leftist_priority_queue::is_empty() const
{
	return _root == nullptr;
}

void leftist_priority_queue::push(
	char const* value,
	unsigned int priority)
{
	if (value == nullptr) throw std::invalid_argument("value cant be nullptr!");

	node * dfksugyhjb = new node; // 2 chasa nochi ept sorry please pojalusta

	dfksugyhjb->_priority = priority;

	dfksugyhjb->_value = new char[strlen(value) + 1];
	strcpy(dfksugyhjb->_value, value);

	dfksugyhjb->_right = nullptr;
	dfksugyhjb->_left = nullptr;

	_root = merge_trees(this->_root, dfksugyhjb);
}

char const* leftist_priority_queue::get_top_value() const
{
	return _root->_value;
}

void leftist_priority_queue::pop()
{
	if (_root == nullptr) throw std::runtime_error("Heap is empty!");

	node* new_root = merge_trees(_root->_left, _root->_right);

	delete[] _root->_value;
	delete _root;

	_root = new_root;
}

priority_queue* leftist_priority_queue::merge(
	priority_queue* other)
{
	if (other == nullptr) throw std::invalid_argument("other cant be nullptr!");

	leftist_priority_queue* lpq_ptr = reinterpret_cast<leftist_priority_queue*>(other);

	if (this->_root == nullptr && lpq_ptr->_root == nullptr)
		return this;

	if (this->_root == nullptr && lpq_ptr->_root != nullptr)
	{
		_root = lpq_ptr->_root;

		return other;
	}

	if (this->_root != nullptr && lpq_ptr->_root == nullptr)
		return this;

	node* tree_copy = copy_tree(lpq_ptr->_root);

	_root = merge_trees(_root, tree_copy);

	destroy_tree(lpq_ptr->_root);

	return this;
}

leftist_priority_queue::leftist_priority_queue() :
	_root(nullptr) {};

leftist_priority_queue::leftist_priority_queue(
	leftist_priority_queue const& src)
{
	_root = copy_tree(src._root);
}

leftist_priority_queue& leftist_priority_queue::operator=(
	leftist_priority_queue const& src)
{
	if (this == &src) return *this;

	_root = copy_tree(src._root);

	return *this;
}

leftist_priority_queue::~leftist_priority_queue() noexcept
{
	destroy_tree(_root);
}