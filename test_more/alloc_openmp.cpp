/* $Id: alloc_openmp.cpp 2307 2012-03-21 19:13:44Z bradbell $ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <omp.h>
# define NUMBER_THREADS  2

namespace {
	using CppAD::thread_alloc;

	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool>( omp_in_parallel() ); }

	// used to inform CppAD of current thread number thread_number()
	size_t thread_number(void)
	{	return static_cast<bool>( omp_get_thread_num() ); }

	// structure with information for one thread
	typedef struct {
		// function object (worker input)
		CppAD::vector<double> x;
	} thread_one_t;

	// vector with information for all threads
	thread_one_t thread_all_[NUMBER_THREADS];

	// --------------------------------------------------------------------
	// function that does the work for one thread
	void worker(void)
	{
		size_t thread_num = thread_number();
		thread_all_[thread_num].x.resize(1);
		thread_all_[thread_num].x[0]=static_cast<double>(thread_num);
	}
}

// Test routine called by the master thread (thread_num = 0).
bool alloc_openmp(void)
{	bool ok = true;
	
	int num_threads = NUMBER_THREADS;

	// call setup for using thread_alloc in parallel mode.
	thread_alloc::parallel_setup(
		size_t(num_threads), in_parallel, thread_number
	);
	
	// Execute the worker function in parallel
	int thread_num;
# pragma omp parallel for
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		worker();
// end omp parallel for
	
	// now inform CppAD that there is only one thread
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);

	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// check calculations by this thread in parallel model
		ok &= thread_all_[thread_num].x[0] == static_cast<double>(thread_num);

		// test having master thread (thread number zero) 
		// free memory that was allocated by thread number thread_num.
		thread_all_[thread_num].x.resize(0);
	}

	return ok;
}