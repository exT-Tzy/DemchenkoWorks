#ifndef LEFTIST_PRIORITY_QUEUE_H
#define LEFTIST_PRIORITY_QUEUE_H
#define _CRT_SECURE_NO_WARNINGS

#include "priority_queue.h"

class leftist_priority_queue :
	public priority_queue
{
protected:

	struct node
	{
		node* _left;

		node* _right;

		unsigned int _priority;

		char* _value;
	};

	node* _root;

protected:

	static void swap_nodes(
		node*&,
		node*&);

	static node* get_right_child(
		node const*);

	static node* get_left_child(
		node const*);

	static unsigned int rang(
		node const*);

protected:

	static node* copy_tree(
		node const*);

	static void destroy_tree(
		node*&);

	virtual node* merge_trees(
		node*&,
		node*&);

public:

	bool is_empty() const;

	void push(
		char const*,
		unsigned int const) override;

	char const* get_top_value() const override;

	void pop() override;

	virtual priority_queue* merge(
		priority_queue*) override;

	leftist_priority_queue();

	leftist_priority_queue(
		leftist_priority_queue const&);

	leftist_priority_queue& operator=(
		leftist_priority_queue const&);

	virtual ~leftist_priority_queue() noexcept;

};

#endif // LEFTIST_PRIORITY_QUEUE_H