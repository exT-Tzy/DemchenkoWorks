#ifndef BINARY_PRIORITY_QUEUE
#define BINARY_PRIORITY_QUEUE
#define _CRT_SECURE_NO_WARNINGS

#include "priority_queue.h"

class binary_priority_queue final :
	public priority_queue
{
private:

	static size_t const INITIAL_CAPACITY = 8;

	struct node
	{
		char* _value;

		unsigned int _priority;
	};

	node* _heap;

	size_t _capacity;

	size_t _virtual_count;

private:

	void destroy();

	inline void copy_node(
		node&,
		node const &);

	void resize();

private:

	static size_t get_left_child_index(
		size_t const);

	static size_t get_right_child_index(
		size_t const);

	static size_t get_parent_index(
		size_t const);

private:

	void sift_up_recursive(
		size_t const);

	void sift_down_recursive(
		size_t const);

	inline void swap(
		node&,
		node&);

public:

	inline bool is_empty() const;

	size_t get_count() const;

	size_t get_capacity() const;

	void push(
		char const*,
		unsigned int const) override;

	char const * get_top_value() const override;

	void pop() override;

	priority_queue* merge(
		priority_queue*) override;

	binary_priority_queue();

	binary_priority_queue(
		binary_priority_queue const&);

	binary_priority_queue& operator=(
		binary_priority_queue const&);

	~binary_priority_queue() noexcept;
};

#endif // BINARY_PRIORITY_QUEUE