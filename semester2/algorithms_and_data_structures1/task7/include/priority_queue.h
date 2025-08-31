#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#define _CRT_SECURE_NO_WARNINGS

class priority_queue
{
public:

	virtual void push(
		char const*,
		unsigned int const) {}

	virtual char const* get_top_value() const { return nullptr; }

	virtual void pop() {}

	virtual priority_queue* merge(
		priority_queue*) { return nullptr; }
};

#endif // PRIORITY_QUEUE_H