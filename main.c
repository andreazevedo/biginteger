#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "macros.h"
#include "big_integer.h"

void test_create()
{
	BigInteger bigInt;

	bigInt = big_integer_create( 0 );
	assert( big_integer_to_int(bigInt) == 0 );

	bigInt = big_integer_create( 17 );
	assert( big_integer_to_int(bigInt) == 17 );

	bigInt = big_integer_create( -17 );
	assert( big_integer_to_int(bigInt) == -17 );

	bigInt = big_integer_create( INT_MAX );
	assert( big_integer_to_int(bigInt) == INT_MAX );

	bigInt = big_integer_create( -INT_MAX );
	assert( big_integer_to_int(bigInt) == -INT_MAX );

	bigInt = big_integer_create( INT_MIN );
	assert( big_integer_to_int(bigInt) == INT_MIN );

	bigInt = big_integer_create( (long long)INT_MAX + INT_MAX + 5 );
	assert( big_integer_to_long_long(bigInt) == 4294967299 );

	bigInt = big_integer_create( (long long)UINT_MAX + UINT_MAX );
	assert( big_integer_to_long_long(bigInt) == (long long)UINT_MAX + UINT_MAX );
}

void test_to_long_long()
{
	BigInteger bigInt;
	long long result;

	bigInt = big_integer_create( 0 );
	result = big_integer_to_long_long( bigInt );
	assert( result == 0 );

	bigInt = big_integer_create( 35 );
	result = big_integer_to_long_long( bigInt );
	assert( result == 35 );

	bigInt = big_integer_create( -26 );
	result = big_integer_to_long_long( bigInt );
	assert( result == -26 );

	bigInt = big_integer_create( LLONG_MAX );
	result = big_integer_to_long_long( bigInt );
	assert( result == LLONG_MAX );

	bigInt = big_integer_create( LLONG_MIN );
	result = big_integer_to_long_long( bigInt );
	assert( result == LLONG_MIN );
}

void test_compare()
{
	BigInteger left;
	BigInteger right;

	left = big_integer_create( 10 );
	right = big_integer_create( 9 );
	assert( big_integer_compare(left, right) > 0 );

	left = big_integer_create( 9 );
	right = big_integer_create( 10 );
	assert( big_integer_compare(left, right) < 0 );

	left = big_integer_create( 10 );
	right = big_integer_create( 10 );
	assert( big_integer_compare(left, right) == 0 );

	left = big_integer_create( -9 );
	right = big_integer_create( -10 );
	assert( big_integer_compare(left, right) > 0 );

	left = big_integer_create( -10 );
	right = big_integer_create( -9 );
	assert( big_integer_compare(left, right) < 0 );

	left = big_integer_create( -10 );
	right = big_integer_create( -10 );
	assert( big_integer_compare(left, right) == 0 );

	left = big_integer_create( 10 );
	right = big_integer_create( -10 );
	assert( big_integer_compare(left, right) > 0 );

	left = big_integer_create( -10 );
	right = big_integer_create( 10 );
	assert( big_integer_compare(left, right) < 0 );

	left = big_integer_create( (long long)INT_MAX + INT_MAX );
	right = big_integer_create( (long long)INT_MAX + 3 );
	assert( big_integer_compare(left, right) > 0 );

	left = big_integer_create( (long long)INT_MAX + 3 );
	right = big_integer_create( (long long)INT_MAX + INT_MAX );
	assert( big_integer_compare(left, right) < 0 );

	left = big_integer_create( (long long)INT_MAX + INT_MAX );
	right = big_integer_create( (long long)INT_MAX + INT_MAX );
	assert( big_integer_compare(left, right) == 0 );
};

void test_add()
{
	BigInteger left;
	BigInteger right;
	BigInteger result;

	left = big_integer_create( 17 );
	right = big_integer_create( 3 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == 20 );

	left = big_integer_create( INT_MAX );
	right = big_integer_create( 5 );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == ((long long)INT_MAX + 5) );

	left = big_integer_create( UINT_MAX );
	right = big_integer_create( 1 );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX + 1) );

	left = big_integer_create( UINT_MAX );
	right = big_integer_create( 5 );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX + 5) );

	left = big_integer_create( UINT_MAX );
	right = big_integer_create( UINT_MAX );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX + UINT_MAX) );

	left = big_integer_create( (long long)UINT_MAX + UINT_MAX );
	right = big_integer_create( UINT_MAX );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX + UINT_MAX + UINT_MAX) );

	left = big_integer_create( -17 );
	right = big_integer_create( -3 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == -20 );

	left = big_integer_create(  -(long long)UINT_MAX );
	right = big_integer_create( -1 );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX - 1) );

	left = big_integer_create( -(long long)UINT_MAX );
	right = big_integer_create( -5 );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX - 5) );

	left = big_integer_create( -(long long)UINT_MAX );
	right = big_integer_create( -(long long)UINT_MAX );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX - UINT_MAX) );

	left = big_integer_create( -(long long)UINT_MAX - UINT_MAX );
	right = big_integer_create( -(long long)UINT_MAX );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX - UINT_MAX - UINT_MAX) );

	left = big_integer_create( 5 );
	right = big_integer_create( 0 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == 5 );

	left = big_integer_create( 0 );
	right = big_integer_create( 5 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == 5 );

	left = big_integer_create( 0 );
	right = big_integer_create( 0 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == 0 );

	left = big_integer_create( -5 );
	right = big_integer_create( 0 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == -5 );

	left = big_integer_create( 0 );
	right = big_integer_create( -5 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == -5 );

	left = big_integer_create( 18 );
	right = big_integer_create( -3 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == 15 );

	left = big_integer_create( -18 );
	right = big_integer_create( +3 );
	result = big_integer_add( left, right );
	assert( big_integer_to_int(result) == -15 );

	left = big_integer_create( -(long long)UINT_MAX - UINT_MAX );
	right = big_integer_create( +UINT_MAX );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == -(long long)UINT_MAX );

	left = big_integer_create( (long long) UINT_MAX + UINT_MAX + UINT_MAX );
	right = big_integer_create( -(long long)UINT_MAX - UINT_MAX );
	result = big_integer_add( left, right );
	assert( big_integer_to_long_long(result) == (long long)UINT_MAX );
};

void test_subtract()
{
	BigInteger left;
	BigInteger right;
	BigInteger result;

	left = big_integer_create( 18 );
	right = big_integer_create( 3 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == 15 );

	left = big_integer_create( INT_MAX );
	right = big_integer_create( 5 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == ((long long)INT_MAX - 5) );

	left = big_integer_create( UINT_MAX );
	right = big_integer_create( 1 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX - 1) );

	left = big_integer_create( UINT_MAX );
	right = big_integer_create( 5 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX - 5) );

	left = big_integer_create( UINT_MAX );
	right = big_integer_create( UINT_MAX );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == 0 );

	left = big_integer_create( (long long)UINT_MAX + UINT_MAX );
	right = big_integer_create( UINT_MAX );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == ((long long)UINT_MAX) );

	left = big_integer_create( -18 );
	right = big_integer_create( -3 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == -15 );

	left = big_integer_create(  -(long long)UINT_MAX );
	right = big_integer_create( -1 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX + 1) );

	left = big_integer_create( -(long long)UINT_MAX );
	right = big_integer_create( -5 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX + 5) );

	left = big_integer_create( -(long long)UINT_MAX );
	right = big_integer_create( -(long long)UINT_MAX );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == 0 );

	left = big_integer_create( -(long long)UINT_MAX - UINT_MAX );
	right = big_integer_create( -(long long)UINT_MAX );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == (-(long long)UINT_MAX) );

	left = big_integer_create( 5 );
	right = big_integer_create( 0 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == 5 );

	left = big_integer_create( 0 );
	right = big_integer_create( 5 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == -5 );

	left = big_integer_create( 0 );
	right = big_integer_create( 0 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == 0 );

	left = big_integer_create( -5 );
	right = big_integer_create( 0 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == -5 );

	left = big_integer_create( 0 );
	right = big_integer_create( -5 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == 5 );

	left = big_integer_create( 18 );
	right = big_integer_create( -3 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == 21 );

	left = big_integer_create( -18 );
	right = big_integer_create( +3 );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_int(result) == -21 );

	left = big_integer_create( -(long long)UINT_MAX - UINT_MAX );
	right = big_integer_create( +UINT_MAX );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == -(long long)UINT_MAX - UINT_MAX - UINT_MAX );

	left = big_integer_create( (long long) UINT_MAX + UINT_MAX + UINT_MAX );
	right = big_integer_create( -(long long)UINT_MAX - UINT_MAX );
	result = big_integer_subtract( left, right );
	assert( big_integer_to_long_long(result) == (long long)UINT_MAX + UINT_MAX + UINT_MAX + UINT_MAX + UINT_MAX );
};

void test_increment()
{
	BigInteger bigInt;

	bigInt = big_integer_create( 10 );
	big_integer_increment( &bigInt, 5 );
	assert( big_integer_to_long_long(bigInt) == 15 );

	bigInt = big_integer_create( -5 );
	big_integer_increment( &bigInt, 10 );
	assert( big_integer_to_long_long(bigInt) == 5 );

	bigInt = big_integer_create( 0 );
	big_integer_increment( &bigInt, 0 );
	assert( big_integer_to_long_long(bigInt) == 0 );

	bigInt = big_integer_create( 0 );
	big_integer_increment( &bigInt, 3 );
	assert( big_integer_to_long_long(bigInt) == 3 );

	bigInt = big_integer_create( -15 );
	big_integer_increment( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == 0 );

	bigInt = big_integer_create( UINT_MAX );
	big_integer_increment( &bigInt, 1 );
	assert( big_integer_to_long_long(bigInt) == (long long)UINT_MAX + 1 );

	bigInt = big_integer_create( UINT_MAX );
	big_integer_increment( &bigInt, UINT_MAX );
	assert( big_integer_to_long_long(bigInt) == (long long)UINT_MAX + UINT_MAX );

	bigInt = big_integer_create( (long long)UINT_MAX + UINT_MAX );
	big_integer_increment( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == (long long)UINT_MAX + UINT_MAX + 15 );

	bigInt = big_integer_create( -(long long)UINT_MAX - 15 );
	big_integer_increment( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == -(long long)UINT_MAX );
};

void test_decrement()
{
	BigInteger bigInt;

	bigInt = big_integer_create( 10 );
	big_integer_decrement( &bigInt, 5 );
	assert( big_integer_to_long_long(bigInt) == 5 );

	bigInt = big_integer_create( -5 );
	big_integer_decrement( &bigInt, 10 );
	assert( big_integer_to_long_long(bigInt) == -15 );

	bigInt = big_integer_create( 0 );
	big_integer_decrement( &bigInt, 0 );
	assert( big_integer_to_long_long(bigInt) == 0 );

	bigInt = big_integer_create( 0 );
	big_integer_decrement( &bigInt, 3 );
	assert( big_integer_to_long_long(bigInt) == -3 );

	bigInt = big_integer_create( 15 );
	big_integer_decrement( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == 0 );

	bigInt = big_integer_create( UINT_MAX );
	big_integer_decrement( &bigInt, 1 );
	assert( big_integer_to_long_long(bigInt) == (long long)UINT_MAX - 1 );

	bigInt = big_integer_create( UINT_MAX );
	big_integer_decrement( &bigInt, UINT_MAX );
	assert( big_integer_to_long_long(bigInt) == 0 );

	bigInt = big_integer_create( -(long long)UINT_MAX );
	big_integer_decrement( &bigInt, UINT_MAX );
	assert( big_integer_to_long_long(bigInt) == -(long long)UINT_MAX - UINT_MAX );

	bigInt = big_integer_create( (long long)UINT_MAX + UINT_MAX );
	big_integer_decrement( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == (long long)UINT_MAX + UINT_MAX - 15 );

	bigInt = big_integer_create( -(long long)UINT_MAX - 15 );
	big_integer_decrement( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == -(long long)UINT_MAX - 30 );

	bigInt = big_integer_create( -(long long)UINT_MAX + 15 );
	big_integer_decrement( &bigInt, 15 );
	assert( big_integer_to_long_long(bigInt) == -(long long)UINT_MAX );
};

void test_performance()
{
	int NUM_ITERATIONS = 10000000;

	clock_t start;
	clock_t end;
	double cpuTime;

	long long l;
	start = clock();
	BigInteger sumBigInt = big_integer_create( 0 );
	for ( l = 1; l <= NUM_ITERATIONS; ++l )
	{		
		big_integer_increment( &sumBigInt, (unsigned int) l );
	}
	end = clock();
	cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("BigInteger made %d increment operations in %f seconds.\n", NUM_ITERATIONS, cpuTime);
	printf("BigInteger last value was: %lld\n", big_integer_to_long_long(sumBigInt));

	double d;
	start = clock();
	double sumDouble = 0;
	for ( d = 1; d <= NUM_ITERATIONS; ++d )
		sumDouble += d;
	end = clock();
	cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Double made %d increment operations in %f seconds.\n", NUM_ITERATIONS, cpuTime);
	printf("Double last value was: %f\n", sumDouble);
};

int main(int argc, const char **argv)
{
	test_create();
	test_to_long_long();
	test_compare();
	test_add();
	test_subtract();
	test_increment();
	test_decrement();
	
	test_performance();

	printf("\nAll tests passed!\n");
	return EXIT_SUCCESS;
};
