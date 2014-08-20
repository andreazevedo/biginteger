#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "macros.h"
#include "big_integer.h"

#ifdef DEBUG
void dump_debug_info()
{
	printf("LLONG_MIN:\n");
	big_integer_dump( big_integer_create( LLONG_MIN ) );
	printf("\nLLONG_MAX:\n");
	big_integer_dump( big_integer_create( LLONG_MAX ) );
	printf("\nINT_MAX + INT_MAX + 5:\n");
	big_integer_dump( big_integer_create( (long long)INT_MAX + INT_MAX + 5 ) );
	printf("\n");
};
#endif

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

int main(int argc, const char **argv)
{
#ifdef DEBUG
	dump_debug_info();
#endif

	test_create();
	test_to_long_long();
	test_compare();
	test_add();
	test_subtract();

	printf("All tests passed!\n");
	return EXIT_SUCCESS;
};
