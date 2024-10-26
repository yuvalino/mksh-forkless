/**	$MirBSD: rnd.c,v 1.7 2004/08/27 14:55:45 tg Stab $ */

/*-
 * Copyright (c) 2004
 *	Thorsten "mirabile" Glaser <tg@66h.42h.de>
 *
 * Licensee is hereby permitted to deal in this work without restric-
 * tion, including unlimited rights to use, publically perform, modi-
 * fy, merge, distribute, sell, give away or sublicence, provided the
 * above copyright notices, these terms and the disclaimer are retai-
 * ned in all redistributions, or reproduced in accompanying documen-
 * tation or other materials provided with binary redistributions.
 *
 * Licensor hereby provides this work "AS IS" and WITHOUT WARRANTY of
 * any kind, expressed or implied, to the maximum extent permitted by
 * applicable law, but with the warranty of being written without ma-
 * licious intent or gross negligence; in no event shall licensor, an
 * author or contributor be held liable for any damage, direct, indi-
 * rect or other, however caused, arising in any way out of the usage
 * of covered work, even if advised of the possibility of such damage.
 */

#include "sh.h"
#include "proto.h"

#ifndef	HAVE_SRANDOM
#undef	HAVE_RANDOM
#endif

#ifdef	KSH

int rnd_state;

void
rnd_seed(long newval)
{
	rnd_put(newval);
	rnd_state = 0;
}

long
rnd_get(void)
{
#ifdef	HAVE_ARC4RANDOM
	if (!rnd_state)
		return arc4random() & 0x7FFF;
#endif
#ifdef	HAVE_RANDOM
	return random() & 0x7FFF;
#else
	return rand();
#endif
}

void
rnd_put(long newval)
{
	long sv;

	rnd_state = 1 | rnd_get();
	sv = (rnd_get() << (newval & 7)) ^ newval;

#if defined(HAVE_ARC4RANDOM_PUSH)
	arc4random_push(sv);
#elif defined(HAVE_ARC4RANDOM_ADDRANDOM)
	arc4random_addrandom((char *)&sv, sizeof(sv));
#endif
#ifdef	HAVE_ARC4RANDOM
	sv ^= arc4random();
#endif

#ifdef	HAVE_RANDOM
	srandom(sv);
#else
	srand(sv);
#endif

	while (rnd_state) {
		rnd_get();
		rnd_state >>= 1;
	}
	rnd_state = 1;
}

#endif	/* def KSH */
