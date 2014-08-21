#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

/*
** big_integer.h
**     Description: "Arbitrary"-precision integer
**     Author: Andre Azevedo <http://github.com/andreazevedo>
**/

#define BIG_INTEGER_DATA_MAX_SIZE	8

typedef struct BigIntegerData
{
	unsigned int bits[BIG_INTEGER_DATA_MAX_SIZE];
	int length;
} BigIntegerData;

typedef struct BigInteger 
{
	char sign;
	BigIntegerData data;
} BigInteger;

/* creates a big integer number */
BigInteger big_integer_create( long long value );

/* returns the big integer as int */
int big_integer_to_int( const BigInteger bigInt );

/* returns the big integer as long long */
long long big_integer_to_long_long( const BigInteger bigInt );

/* compare big integers */
int big_integer_compare( const BigInteger left, const BigInteger right );

/* adds two big integers together ( left + right ) */
BigInteger big_integer_add( const BigInteger left, const BigInteger right );

/* subtracts one big integer from another ( left - right ) */
BigInteger big_integer_subtract( const BigInteger left, const BigInteger right );

/* increments the bigInteger by the amount specified */
void big_integer_increment( BigInteger *bigInt, const unsigned int value );

/* decrements the bigInteger by the amount specified */
void big_integer_decrement( BigInteger *bigInt, const unsigned int value );


#ifdef DEBUG
void big_integer_dump( const BigInteger bigInt );
#endif

#endif /* BIG_INTEGER_H */
