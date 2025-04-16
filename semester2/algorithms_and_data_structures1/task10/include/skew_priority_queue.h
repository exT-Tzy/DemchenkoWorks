#ifndef SKEW_PRIORITY_QUEUE_H
#define SKEW_PRIORITY_QUEUE_H
#define _CRT_SECURE_NO_WARNINGS

#include "leftist_priority_queue.h"

class skew_priority_queue final:
	public leftist_priority_queue 
{
private:

	node* merge_trees(
		node*&,
		node*&) override;

public:

	priority_queue* merge(
		priority_queue*) override;

	skew_priority_queue();

	skew_priority_queue(
		skew_priority_queue const&);

	skew_priority_queue& operator=(
		skew_priority_queue const&);

	~skew_priority_queue() noexcept;
};

#endif // SKEW_PRIORITY_QUEUE_H