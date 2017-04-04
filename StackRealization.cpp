#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <cassert>
#include <cstdio>
#include <limits>

#include "Stack.h"

//! this is a macro to periodically verify content
#define ASSERT_OK 	if((ok() & STK_OVERFLOW) || (ok() & STK_UNDERFLOW) || (ok() & STK_NOTEXIST)) \
						{ \
						dump(); \
						assert(0); \
						}

Stack::Stack():
	capacity_(CAPACITY_DEFAULT),
	size_(0),
	data_((dtype*)malloc(sizeof(dtype) * capacity_))
	{
	ASSERT_OK;
	}

Stack::~Stack()
	{
	free(data_);
	}


#define PUTERR(condition, errorcode) \
		if(condition) \
			{ \
			error |= errorcode; \
			}

int Stack::ok() const
	{
	int error = 0;
	PUTERR(size_ == capacity_, STK_FULL);
	PUTERR(size_ > capacity_, STK_OVERFLOW);
	PUTERR(size_ < 0, STK_UNDERFLOW);
	PUTERR(!data_, STK_NOTEXIST);
	return error;
	}

#define PRINT_ERROR(err_name, err_message) \
	if(error & err_name) \
		{ \
		printf("%s\n", err_message); \
		}

bool Stack::dump() const
	{
	time_t time = std::time(NULL);
	printf("\nSTACK:***Dump has been called***\n%s\n", std::ctime(&time));
	printf("capacity_ = %zu\n", capacity_);
	printf("size_ = %zu\n", size_);
	printf("data_ = %p\n", data_);
	int error = ok();
	PRINT_ERROR(STK_FULL, "Warning: Stack is full. In next push call memory will be realocated");
	PRINT_ERROR(STK_OVERFLOW, "Error: Stack is overflow");
	PRINT_ERROR(STK_UNDERFLOW, "Error: Stack is underflow");
	PRINT_ERROR(STK_NOTEXIST, "Error: Stack isnt exist");
        if(error == 0)printf("Stack: any error hasnt been detected\n");
	printf("----------\n");
	if(!(error & STK_NOTEXIST))
		{
		printf("Content:\n");
		size_t t;
		for(t = 0; t < size_; ++t)
			{
			printf("[%zu] %d\n", t ,data_[t]);
			}
		}
	printf("----------\n");
	if(error) return false;
	else return true;
	}

size_t Stack::ssize() const
	{
	return size_;
	}

#define HANDLE_FULL {\
	capacity_ *= 2; \
	data_ = (dtype*)realloc(data_, capacity_); \
	} 

bool Stack::push(dtype val)
	{
	ASSERT_OK;
	if(ok() & STK_FULL) HANDLE_FULL
	data_[size_++] = val;
	ASSERT_OK;
	return true;
	}

bool Stack::print(std::ostream & out) const
	{
	if(out.fail())
		{
		printf("***Cant print content***\n");
		dump();
		assert(2);
		}
	ASSERT_OK;
	int i;
	for(i = 0; i <size_; ++i)
		{
		out << "[" << i << "] " << data_[i] << '\n';
		}
	return true;
	}

dtype Stack::pop()
	{
	ASSERT_OK;
	if(size_ == 0)
		{
		printf("***Stack is empty***\n");
		return BADDATA;
		}
	return data_[--size_];
	}

dtype Stack::peek() const
	{
	ASSERT_OK;
	if(size_ == 0)
		{
		printf("***Stack is empty***\n");
		return BADDATA;
		}
	return data_[size_ - 1];
	}

#undef ASSERT_OK
#undef PUTERR
#undef PRINT_ERROR
#undef HANDLE_FULL
/*int main()
    {
    Stack s;
    s.push(32.0);
    s.push(84.0);
    s.dump();
    }*/

