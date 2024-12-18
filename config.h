/* config.h - libf config definitions */
#ifndef SL_CONFIG_H
#define SL_CONFIG_H


/* extern symbol */
#undef XSYMBOL
#define XSYMBOL(name) x##name

/* machine 32bit */
#undef MARCH_BIT_32
#define MARCH_BIT_32 32
/* machine 64bit */
#undef MARCH_BIT_64
#define MARCH_BIT_64 64

/* machine architecture 32bit/64bit */
#undef MARCH_BIT
#ifdef __SIZEOF_POINTER__
#	if (__SIZEOF_POINTER__ == 4)
#		define MARCH_BIT MARCH_BIT_32
#	else
#		define MARCH_BIT MARCH_BIT_64
#	endif
#else
	!!!undefined __SIZEOF_POINTER__!!!
#endif


#endif
