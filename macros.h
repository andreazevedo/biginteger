#ifndef MACROS_H
#define MACROS_H

#ifndef MAX
	#define MAX( a, b ) ( (a)>(b) ? (a) : (b) )
#endif
#ifndef MIN
#	define MIN( a, b ) ( (a)<(b) ? (a) : (b) )
#endif

#ifndef LLONG_MAX
	#define LLONG_MAX	0x7FFFFFFFFFFFFFFF
#endif
#ifndef LLONG_MIN
	#define LLONG_MIN	(-LLONG_MAX-1)
#endif

#endif /* MACROS_H */