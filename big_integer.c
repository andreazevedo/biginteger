/*
** big_integer.c
**     Description: Arbitrary-precision integer
**     Author: Andre Azevedo <http://github.com/andreazevedo>
**/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "macros.h"
#include "big_integer.h"

/*#define UINT_NUM_BITS		(32)*/
#define UINT_NUM_BITS		(sizeof(unsigned int) * 8)


/* PRIVATE FUNCTIONS DECLARATIONS */
BigInteger big_integer_create_internal( const char sign, const int data[], const int length );
void big_integer_normalize( BigInteger *bigInt );
void big_integer_normalize_interval( unsigned int data[], int from, int to );
void big_integer_report_overflow();
BigInteger big_integer_add_internal( const char resultSign, const BigInteger left, const BigInteger right );


/* PRIVATE FUNCTIONS IMPLEMENTATION */
BigInteger big_integer_create_internal( const char sign, const int data[], const int length )
{
	if ( length > BIG_INTEGER_DATA_MAX_SIZE )
	{
		big_integer_report_overflow();
		abort();
		exit( EXIT_FAILURE );
	}

	BigInteger bigInt;
	bigInt.sign = sign;
	if (data && length > 0)
		memcpy( bigInt.data, data, sizeof(unsigned int) * length );
	bigInt.length = length;

	big_integer_normalize( &bigInt );

	return bigInt;
};

void big_integer_normalize( BigInteger *bigInt )
{
	big_integer_normalize_interval( bigInt->data, bigInt->length, BIG_INTEGER_DATA_MAX_SIZE-1 );
}

void big_integer_normalize_interval( unsigned int data[], int from, int to )
{
	int i;
	for ( i = from; i <= to; ++i )
		data[i] = 0;	
}

void big_integer_report_overflow()
{
	fprintf(stderr, "BigInteger reported overflow!\n");
};

BigInteger big_integer_add_internal( const char resultSign, const BigInteger left, const BigInteger right )
{
	int uIntNumBits = UINT_NUM_BITS;

	BigInteger result = big_integer_create_internal( resultSign, NULL, 0 );

	int len = MAX( left.length, right.length );

	unsigned long long sum = 0;
	int i;
	for ( i = 0; i < len; ++i )
	{
		sum += (unsigned long long) left.data[i] + right.data[i];
		result.data[i] = (unsigned int) sum;
		sum >>= uIntNumBits;
	}

	if ( sum > 0 )
	{
		result.data[i] = (unsigned int) sum;
		i++;
	}

	result.length = i;

	return result;
};


/* PUBLIC FUNCTIONS IMPLEMENTATION */
BigInteger big_integer_create( long long value )
{
	BigInteger bigInt;
	int numBits = UINT_NUM_BITS;

	if ( value == 0 )
	{
		bigInt.sign = 0;
		bigInt.data[0] = 0;
		bigInt.length = 1;
	}
	else
	{
		unsigned long long uValue;
		if ( value < 0 )
		{
			bigInt.sign = -1;
			uValue = (unsigned long long) -value;
		}
		else
		{
			bigInt.sign = 1;
			uValue = (unsigned long long) value;
		}

		bigInt.length = 0;
		while ( uValue > 0 )
		{
			bigInt.data[bigInt.length++] = (unsigned int) uValue;
			uValue >>= numBits;
		}
	}

	big_integer_normalize( &bigInt );

	return bigInt;
};

int big_integer_to_int( const BigInteger bigInt )
{
	if ( bigInt.sign == 0 )
		return 0;

	/* overflow check */
	if ( bigInt.length > 1 ||
		(bigInt.sign == 1 && bigInt.data[0] > INT_MAX) ||
		(bigInt.sign == -1 && -(bigInt.data[0]) < INT_MIN) )		
	{
		big_integer_report_overflow();
		abort();
		exit( EXIT_FAILURE );
	}

	if ( bigInt.sign == -1 )
		return -(int)bigInt.data[0];

	return (int)bigInt.data[0];
};

long long big_integer_to_long_long( const BigInteger bigInt )
{
	if ( bigInt.sign == 0 )
		return 0;

	int uIntNumBits = UINT_NUM_BITS;
	int maxLength = sizeof(long long) / sizeof(unsigned int);

	if ( bigInt.length > maxLength )
	{
		big_integer_report_overflow();
		abort();
		exit( EXIT_FAILURE );
	}

	unsigned long long result = 0;
	int i = 0;
	for ( i = 0; i < bigInt.length; ++i )
	{
		result |= ((unsigned long long)bigInt.data[i]) << (uIntNumBits * i);
	}

	if ( bigInt.sign == -1 )
		return -(long long)result;

	return result;
};

int big_integer_compare( const BigInteger left, const BigInteger right )
{
	/* if one is positive and the other negative */
	if ( left.sign > right.sign )
		return 1;
	if ( left.sign < right.sign )
		return -1;
	
	/* if both are zero */
	char sign = left.sign;
	if ( sign == 0 )
		return 0;

	/* if the lengths are different */
	if ( left.length > right.length )
		return 1;
	if ( left.length < right.length )
		return -1;

	int length = left.length;
	int i;
	for ( i = (length-1); i >= 0; --i)
	{
		if ( left.data[i] > right.data[i] )
			return sign;
		if ( left.data[i] < right.data[i] )
			return -sign;
	}

	return 0;
};

BigInteger big_integer_add( const BigInteger left, const BigInteger right )
{
	if ( left.sign == 0 )
		return right;
	if ( right.sign == 0 )
		return left;

	if ( left.sign == right.sign )
		return big_integer_add_internal( left.sign, left, right );

	assert( false );

	return left;
};

BigInteger big_integer_subtract( const BigInteger left, const BigInteger right )
{
	assert( false );
	return left;
};

#ifdef DEBUG
void big_integer_dump( const BigInteger bigInt )
{
	printf("BigInteger:\n");
	printf("Sign: %d\n", (int)bigInt.sign);
	printf("Data: { ");
	if ( bigInt.length > 0 )
	{
		int i;
		for ( i = 0; i < (bigInt.length - 1); i++ )
			printf("%u, ", bigInt.data[i]);
		printf("%u ", bigInt.data[bigInt.length-1]);
	}
	printf("}\n");
	printf("Length: %d\n", bigInt.length);
}
#endif
