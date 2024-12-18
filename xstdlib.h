/* xstdlib.h - standard library definitions */
#ifndef SL_XSTDLIB_H
#define SL_XSTDLIB_H

#include "config.h"
#include "xstddef.h"


/* struct: random_data */
struct XSYMBOL(random_data) {
	int32 type;
	int32 *state;
	int32 *fptr;
	int32 *bptr;
	int32 *eptr;
}; /* end */

/* defined: random_r type0
* LCG (Linear Congruential Generator) */
#undef RANDOM_TYPE0_NEW
#define RANDOM_TYPE0_NEW(name, seed) \
	int32 name##_random_tab0[1] = { \
		seed \
		}; \
	struct XSYMBOL(random_data) name = { \
		.type = 0, \
		.state = name##_random_tab0 \
		}
/* end */

/* defined: random_r type1
* LFSR (Linear Feedback Shift Register)
* x^31 + x^3 + 1 */
#undef RANDOM_TYPE1_NEW
#define RANDOM_TYPE1_NEW(name) \
	int32 name##_random_tab1[31] = { \
		-1726662223, 379960547, 1735697613, 1040273694, \
		1313901226, 1627687941, -179304937, -2073333483, \
		1780058412, -1989503057, -615974602, 344556628, \
		939512070, -1249116260, 1507946756, -812545463, \
		154635395, 1388815473, -1926676823, 525320961, \
		-1009028674, 968117788, -123449607, 1284210865, \
		435012392, -2017506339, -911064859, -370259173, \
		1132637927, 1398500161, -205601318 \
		}; \
	struct XSYMBOL(random_data) name = { \
		.type = 1, \
		.state = name##_random_tab1, \
		.fptr = &name##_random_tab1[3], \
		.bptr = &name##_random_tab1[0], \
		.eptr = &name##_random_tab1[31] \
		}
/* end */

extern int32 XSYMBOL(random_r)(struct XSYMBOL(random_data) *p, int32 *v);


#endif