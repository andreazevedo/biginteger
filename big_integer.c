/*
** big_integer.c
**     Description: "Arbitrary"-precision integer
**     Author: Andre Azevedo <http://github.com/andreazevedo>
**/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include "macros.h"
#include "big_integer.h"

/*#define UINT_NUM_BITS		(sizeof(unsigned int) * 8)*/
const int UINT_NUM_BITS =	(sizeof(unsigned int) * 8);


/* PRIVATE FUNCTIONS DECLARATIONS */
BigIntegerData big_integer_empty_data( );
BigIntegerData big_integer_create_data( const unsigned int bits[], const int length );
BigInteger big_integer_create_internal( const char sign, const BigIntegerData data );
void big_integer_normalize( BigIntegerData *pBigIntData );
void big_integer_normalize_from( BigIntegerData *pBigIntData, const int from );
void big_integer_clear_trash_data( BigIntegerData *pBigIntData );
void big_integer_report_overflow();
int big_integer_compare_data( const BigIntegerData *pLeft, const BigIntegerData *pRight );
int big_integer_compare_data_uint( const BigIntegerData *pBigIntData, unsigned int value );
BigIntegerData big_integer_add_data( const BigIntegerData left, const BigIntegerData right );
BigIntegerData big_integer_subtract_data( const BigIntegerData left, const BigIntegerData right );
void big_integer_increment_data( BigIntegerData *pBigIntData, const unsigned int value );
void big_integer_decrement_data( BigIntegerData *pBigIntData, const unsigned int value );


/* PRIVATE FUNCTIONS IMPLEMENTATION */
BigIntegerData big_integer_empty_data( )
{
	BigIntegerData bigIntData;
	bigIntData.length = 0;
	big_integer_clear_trash_data( &bigIntData );
	return bigIntData;
};

BigIntegerData big_integer_create_data( const unsigned int bits[], const int length )
{
	if ( length > BIG_INTEGER_DATA_MAX_SIZE )
	{
		big_integer_report_overflow();
		abort();
		exit( EXIT_FAILURE );
	}

	BigIntegerData bigIntData;
	if (bits && length > 0)
		memcpy( bigIntData.bits, bits, sizeof(unsigned int) * length );
	bigIntData.length = length;

	big_integer_clear_trash_data( &bigIntData );

	return bigIntData;
};

BigInteger big_integer_create_internal( const char sign, const BigIntegerData data )
{
	BigInteger bigInt;
	bigInt.sign = sign;
	bigInt.data = data;

	return bigInt;
};

void big_integer_normalize( BigIntegerData *pBigIntData )
{
	big_integer_normalize_from( pBigIntData, BIG_INTEGER_DATA_MAX_SIZE-1 );
};

void big_integer_normalize_from( BigIntegerData *pBigIntData, const int from )
{
	int i;
	for ( i = from; i >= 0; --i )
	{
		if ( pBigIntData->bits[i] != 0 )
		{
			pBigIntData->length = i + 1;
			break;
		}
	}
};

void big_integer_clear_trash_data( BigIntegerData *pBigIntData )
{	
	int i;
	for ( i = pBigIntData->length; i < BIG_INTEGER_DATA_MAX_SIZE; ++i )
		pBigIntData->bits[i] = 0;
};

void big_integer_report_overflow()
{
	fprintf(stderr, "BigInteger reported overflow!\n");
};

int big_integer_compare_data( const BigIntegerData *pLeft, const BigIntegerData *pRight )
{
	/* if the lengths are different */
	if ( pLeft->length > pRight->length )
		return 1;
	if ( pLeft->length < pRight->length )
		return -1;

	int length = pLeft->length;
	int i;
	for ( i = (length-1); i >= 0; --i)
	{
		if ( pLeft->bits[i] > pRight->bits[i] )
			return 1;
		if ( pLeft->bits[i] < pRight->bits[i] )
			return -1;
	}

	return 0;
};

int big_integer_compare_data_uint( const BigIntegerData *pBigIntData, unsigned int value )
{
	if ( pBigIntData->length == 0 )
		return -1;
	if ( pBigIntData->length > 1 )
		return 1;

	if ( pBigIntData->bits[0] > value )
		return 1;
	else if ( pBigIntData->bits[0] < value )
		return -1;

	return 0;
};

BigIntegerData big_integer_add_data( const BigIntegerData left, const BigIntegerData right )
{
	int uIntNumBits = UINT_NUM_BITS;

	BigIntegerData result = big_integer_empty_data( );

	int len = MAX( left.length, right.length );

	unsigned long long sum = 0;
	int i;
	for ( i = 0; i < len; ++i )
	{
		sum += (unsigned long long) left.bits[i] + right.bits[i];
		result.bits[i] = (unsigned int) sum;
		sum >>= uIntNumBits;
	}

	if ( sum > 0 )
	{
		result.bits[i] = (unsigned int) sum;
		i++;
	}

	result.length = i;

	return result;
};

/* left > right always */
BigIntegerData big_integer_subtract_data( const BigIntegerData left, const BigIntegerData right )
{
	BigIntegerData result = big_integer_empty_data( );

	int len = MAX( left.length, right.length );

	unsigned long long borrow = 0;
	int i;
	for ( i = 0; i < len; ++i )
	{
		/* what happens here is that, if left is less than right, borrow will become 
		   "negative" (not really because it is unsigned), and the bit pattern for that is 
		   the 1's complement (complementing it to get to 0), which is exactly the remainder
		   of this term in the subtraction. */
		borrow = (unsigned long long) left.bits[i] - right.bits[i] - borrow;

		result.bits[i] = (unsigned int) borrow;

		/* here we just want the first 1 after removing the lower order term */
		borrow = (borrow >> UINT_NUM_BITS) & 1; 
	}

	big_integer_normalize_from( &result, i );

	return result;
};

void big_integer_increment_data( BigIntegerData *pBigIntData, const unsigned int value )
{
	unsigned long long carry = value;
	int i = 0;
	while ( carry > 0 )
	{
		carry += (unsigned long long) pBigIntData->bits[i];
		pBigIntData->bits[i] = (unsigned int) carry;
		carry >>= UINT_NUM_BITS;
		++i;
	}

	if ( i > pBigIntData->length )
		pBigIntData->length = i;
};

/* pBigIntData > value */
void big_integer_decrement_data( BigIntegerData *pBigIntData, const unsigned int value )
{
	unsigned long long borrow = value;
	int i = 0;
	while ( borrow > 0 )
	{
		borrow = (unsigned long long) pBigIntData->bits[i] - borrow;
		pBigIntData->bits[i] = (unsigned int) borrow;
		borrow = (borrow >> UINT_NUM_BITS) & 1;
		++i;
	}

	big_integer_normalize_from( pBigIntData, i );
};




/* PUBLIC FUNCTIONS IMPLEMENTATION */
BigInteger big_integer_create( long long value )
{
	BigInteger bigInt;
	int numBits = UINT_NUM_BITS;

	if ( value == 0 )
	{
		bigInt.sign = 0;
		bigInt.data.bits[0] = 0;
		bigInt.data.length = 1;
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

		bigInt.data.length = 0;
		while ( uValue > 0 )
		{
			bigInt.data.bits[bigInt.data.length++] = (unsigned int) uValue;
			uValue >>= numBits;
		}
	}

	big_integer_clear_trash_data( &bigInt.data );

	return bigInt;
};

int big_integer_to_int( const BigInteger bigInt )
{
	if ( bigInt.sign == 0 )
		return 0;

	/* overflow check */
	if ( bigInt.data.length > 1 ||
		(bigInt.sign == 1 && bigInt.data.bits[0] > INT_MAX) ||
		(bigInt.sign == -1 && -(bigInt.data.bits[0]) < INT_MIN) )		
	{
		big_integer_report_overflow();
		abort();
		exit( EXIT_FAILURE );
	}

	if ( bigInt.sign == -1 )
		return -(int)bigInt.data.bits[0];

	return (int)bigInt.data.bits[0];
};

long long big_integer_to_long_long( const BigInteger bigInt )
{
	if ( bigInt.sign == 0 )
		return 0;

	int uIntNumBits = UINT_NUM_BITS;
	int maxLength = sizeof(long long) / sizeof(unsigned int);

	if ( bigInt.data.length > maxLength )
	{
		big_integer_report_overflow();
		abort();
		exit( EXIT_FAILURE );
	}

	unsigned long long result = 0;
	int i = 0;
	for ( i = 0; i < bigInt.data.length; ++i )
	{
		result |= ((unsigned long long)bigInt.data.bits[i]) << (uIntNumBits * i);
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
	
	/* if they have the same sign */
	char sign = left.sign;
	return sign * big_integer_compare_data( &left.data, &right.data );
};

BigInteger big_integer_add( const BigInteger left, const BigInteger right )
{
	if ( left.sign == 0 )
		return right;
	if ( right.sign == 0 )
		return left;

	if ( left.sign == right.sign )
		return big_integer_create_internal( left.sign, big_integer_add_data( left.data, right.data ));

	/* compare the MOD of the numbers */
	int compRes = big_integer_compare_data( &left.data, &right.data );

	if ( compRes == 0 )
		return big_integer_create( 0 );

	if ( compRes > 0 ) /* left > right */
		return big_integer_create_internal( left.sign, big_integer_subtract_data( left.data, right.data ));

	return big_integer_create_internal( right.sign, big_integer_subtract_data( right.data, left.data ));
};

BigInteger big_integer_subtract( const BigInteger left, const BigInteger right )
{
	if ( left.sign == 0 )
		return big_integer_create_internal( -right.sign, right.data );
	if ( right.sign == 0 )
		return left;

	if ( left.sign != right.sign )
		return big_integer_create_internal( left.sign, big_integer_add_data(left.data, right.data) );

	/* compare the MOD of the numbers */
	int compRes = big_integer_compare_data( &left.data, &right.data );

	if ( compRes == 0 )
		return big_integer_create( 0 );

	if ( compRes > 0 ) /* left > right */
		return big_integer_create_internal( left.sign, big_integer_subtract_data(left.data, right.data) );

	return big_integer_create_internal( -right.sign, big_integer_subtract_data(right.data, left.data) );
};

void big_integer_increment( BigInteger *bigInt, const unsigned int value )
{
	if ( bigInt->sign >= 0 ) /* bigInt >= 0 */
	{
		if ( bigInt->sign == 0 && value > 0 )
			bigInt->sign = 1;
		big_integer_increment_data( &bigInt->data, value );
	}
	else /* bigInt < 0 */
	{
		int compRes = big_integer_compare_data_uint( &bigInt->data, value );

		if ( compRes == 0 ) /* |bigInt| == |value| */
		{
			bigInt->sign = 0;
			bigInt->data.length = 0;
			big_integer_clear_trash_data( &bigInt->data );
		}
		else if ( compRes > 0 ) /* |bigInt| > |value| */
		{
			big_integer_decrement_data( &bigInt->data, value );
		}
		else /* |bigInt| < |value| */
		{
#ifdef DEBUG
			/* |bigInt| < |value| implies that bigInt has length 1
			   because value, if expressed as a BigInteger, would have length 1. */
			assert( bigInt->data.length == 1 );
#endif
			bigInt->sign = 1;
			bigInt->data.bits[0] = value - bigInt->data.bits[0];
		}
	}
};

void big_integer_decrement( BigInteger *bigInt, const unsigned int value )
{
	if ( bigInt->sign <= 0 )/* bigInt <= 0 */
	{
		if ( bigInt->sign == 0 && value > 0 )
			bigInt->sign = -1;
		big_integer_increment_data( &bigInt->data, value );
	}
	else /* bigInt > 0 */
	{
		int compRes = big_integer_compare_data_uint( &bigInt->data, value );

		if ( compRes == 0 ) /* |bigInt| == |value| */
		{
			bigInt->sign = 0;
			bigInt->data.length = 0;
			big_integer_clear_trash_data( &bigInt->data );
		}
		else if ( compRes > 0 ) /* |bigInt| > |value| */
		{
			big_integer_decrement_data( &bigInt->data, value );
		}
		else /* |bigInt| < |value| */
		{
#ifdef DEBUG
			/* |bigInt| < |value| implies that bigInt has length 1 or less 
			   because value, if expressed as a BigInteger, would have length 1. */
			assert( bigInt->data.length == 1 );
#endif
			bigInt->sign = -1;
			bigInt->data.bits[0] = value - bigInt->data.bits[0];
		}
	}
};

#ifdef DEBUG
void big_integer_dump( const BigInteger bigInt )
{
	printf("BigInteger:\n");
	printf("Sign: %d\n", (int)bigInt.sign);
	printf("Data: { ");
	if ( bigInt.data.length > 0 )
	{
		int i;
		for ( i = 0; i < (bigInt.data.length - 1); i++ )
			printf("%u, ", bigInt.data.bits[i]);
		printf("%u ", bigInt.data.bits[bigInt.data.length-1]);
	}
	printf("}\n");
	printf("Length: %d\n", bigInt.data.length);
}
#endif
