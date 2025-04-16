#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>
#include <cstring>

#include "binary_priority_queue.h"

void binary_priority_queue::destroy()
{
	for (int i = 0; i < _virtual_count; ++i)
	{
		delete[] _heap[i]._value;
	}

	_virtual_count = 0;
}

void binary_priority_queue::copy_node(
	node& dest,
	node const& src)
{
	dest._priority = src._priority;
	dest._value = new char[strlen(src._value) + 1];
	strcpy(dest._value, src._value);
}

void binary_priority_queue::resize()
{
	_capacity *= 2;
	node* new_heap = new node[_capacity];
	
	for (int i = 0; i < _virtual_count; ++i)
		copy_node(new_heap[i], _heap[i]);

	delete[] _heap;
	_heap = new_heap;
}

size_t binary_priority_queue::get_left_child_index(
	size_t const i)
{
	return 2 * i + 1;
}

size_t binary_priority_queue::get_right_child_index(
	size_t const i)
{
	return 2 * i + 2;
}

size_t binary_priority_queue::get_parent_index(
	size_t const i)
{
	return (i - 1) / 2;
}

void binary_priority_queue::sift_up_recursive(
	size_t const i)
{
	if (i == 0) 
		return;

	if (_heap[get_parent_index(i)]._priority > _heap[i]._priority)
		return;

	swap(_heap[i], _heap[get_parent_index(i)]);

	sift_up_recursive(get_parent_index(i));
}

void binary_priority_queue::sift_down_recursive(
	size_t const i)
{
	if (get_left_child_index(i) >= _virtual_count && get_right_child_index(i) >= _virtual_count)
		return;

	size_t m;

	if (get_right_child_index(i) >= _virtual_count)
		m = get_left_child_index(i);
	else if (_heap[get_left_child_index(i)]._priority > _heap[get_right_child_index(i)]._priority)
		m = get_left_child_index(i);
	else
		m = get_right_child_index(i);

	if (_heap[i]._priority > _heap[m]._priority)
		return;

	swap(_heap[i], _heap[m]);

	sift_down_recursive(m);
}

void binary_priority_queue::swap(
	node& first,
	node& second)
{
	auto t = first;
	first = second;
	second = t;
}

bool binary_priority_queue::is_empty() const
{
	return _virtual_count == 0;
}

size_t binary_priority_queue::get_count() const
{
	return _virtual_count;
}

size_t binary_priority_queue::get_capacity() const
{
	return _capacity;
}

void binary_priority_queue::push(
	char const* value,
	unsigned int const priority)
{
	if (value == nullptr) throw std::invalid_argument("value cant be nullptr!");

	if (_virtual_count == _capacity)
		resize();

	_heap[_virtual_count]._priority = priority;
	
	_heap[_virtual_count]._value = new char[strlen(value) + 1];
	strcpy(_heap[_virtual_count]._value, value);

	sift_up_recursive(_virtual_count);

	++_virtual_count;
}

char const* binary_priority_queue::get_top_value() const
{
	if (is_empty())
		return nullptr;

	return _heap->_value;
}

void binary_priority_queue::pop()
{
	if (is_empty()) throw std::runtime_error("Heap is empty!");

	if (_virtual_count == 1)
	{
		destroy();

		return;
	}

	swap(*_heap, _heap[_virtual_count - 1]);

	delete[] _heap[_virtual_count - 1]._value;

	--_virtual_count;

	sift_down_recursive(0);
}

priority_queue* binary_priority_queue::merge(
	priority_queue* src)
{
	if (src == nullptr) throw std::invalid_argument("src cant be nullptr!");

	binary_priority_queue* bpq_ptr = reinterpret_cast<binary_priority_queue*>(src);

	if (bpq_ptr->is_empty())
		return this;

	push(bpq_ptr->get_top_value(), bpq_ptr->_heap[0]._priority);
	bpq_ptr->pop();

	merge(src);

	return this;
}

binary_priority_queue::binary_priority_queue():
	_capacity(INITIAL_CAPACITY), _virtual_count(0)
{
	_heap = new node[_capacity];
}

binary_priority_queue::binary_priority_queue(
	binary_priority_queue const& src):
	_capacity(src._capacity), _virtual_count(src._virtual_count)
{
	_heap = new node[_capacity];

	for (int i = 0; i < _virtual_count; ++i)
		copy_node(_heap[i], src._heap[i]);
}

binary_priority_queue& binary_priority_queue::operator=(
	binary_priority_queue const& src)
{
	if (this == &src) return *this;

	destroy();
	delete[] _heap;

	_virtual_count = src._virtual_count;
	_capacity = src._capacity;

	_heap = new node[_capacity];

	for (int i = 0; i < _virtual_count; ++i)
		copy_node(_heap[i], src._heap[i]);

	return *this;
}

binary_priority_queue::~binary_priority_queue() noexcept
{
	destroy();
	delete[] _heap;
}