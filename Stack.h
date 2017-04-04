#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <limits>

//! Errors and warnings
enum STKERR
	{
	STK_DEFAULT = 0,//!Default
	STK_FULL = 2, //!Only warning
	STK_OVERFLOW = 4,//!Error
	STK_UNDERFLOW = 8,//!Error
	STK_NOTEXIST = 16//!Error
	};

//! this is stack defailt capacity
#define CAPACITY_DEFAULT 100

//!Returning value if stack is empty (NaN)
#define BADDATA std::numeric_limits<double>::quiet_NaN()

typedef int dtype;

class Stack
	{
	public:
		Stack();
		~Stack();
		int ok() const;
		bool dump() const;

		size_t ssize() const;
		bool print(std::ostream & out) const;

		bool push(dtype val);
		dtype pop();
		dtype peek() const;
	private:
		size_t capacity_;
		size_t size_;
		dtype* data_;
	};


#endif
