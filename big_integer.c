/*
** big_integer.c
**     Description: "Arbitrary"-precision integer
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
BigIntegerData big_integer_empty_data( );
BigIntegerData big_integer_create_data( const unsigned int bits[], const int length );
BigInteger big_integer_create_internal( const char sign, const BigIntegerData data );
void big_integer_normalize( BigIntegerData *pBigIntData );
void big_integer_normalize_from( BigIntegerData *pBigIntData, const int from );
void big_integer_clear_trash_data( BigIntegerData *pBigIntData );
void big_integer_report_overflow();
int big_integer_compare_data( const BigIntegerData *pLeft, const BigIntegerData *pRight );
BigIntegerData big_integer_add_data( const BigIntegerData left, const BigIntegerData right );
BigIntegerData big_integer_subtract_data( const BigIntegerData left, const BigIntegerData right );


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
	int uIntNumBits = UINT_NUM_BITS;

	BigIntegerData result = big_integer_empty_data( );

	int len = MAX( left.length, right.length );

	unsigned long long leftValue;
	unsigned long long borrow = 0;
	int i;
	for ( i = 0; i < len; ++i )
	{
		borrow >>= uIntNumBits; /* in this cycle, borrow is worth UINT_NUM_BITS less */
		leftValue = left.bits[i] - borrow;
		
		borrow = ( leftValue < right.bits[i] ) ? ((unsigned long long) 1 << uIntNumBits) : 0;
		result.bits[i] = (leftValue + borrow) - right.bits[i];
	}

	big_integer_normalize_from( &result, i );

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
