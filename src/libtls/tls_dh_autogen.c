#include "klone_conf.h"
#ifdef HAVE_LIBOPENSSL
#ifndef HEADER_DH_H
#include <openssl/dh.h>
#endif
DH *get_dh1024()
	{
	static unsigned char dh1024_p[]={
		0xF0,0xE6,0xAB,0x96,0x17,0xC4,0xDF,0xD7,0x87,0x46,0x7D,0x66,
		0xF3,0xA3,0x54,0x4E,0x70,0x38,0xBB,0x8D,0x05,0x89,0x4B,0xF7,
		0x51,0x0E,0x22,0x60,0xB0,0x02,0xA3,0x32,0xF4,0xFF,0xE4,0x0D,
		0x4E,0x6C,0xF9,0xD1,0x0B,0x93,0xEC,0x2E,0x07,0x46,0x17,0x9C,
		0xE3,0x3D,0x5F,0x18,0x6F,0xF0,0x06,0x5F,0x65,0xFD,0x96,0xD0,
		0x6C,0xC3,0xF3,0xAA,0xC3,0xFD,0x9E,0x77,0xEC,0x08,0xD8,0x48,
		0x43,0x14,0xC8,0xA1,0x4D,0xBF,0xAE,0xA7,0x10,0xA5,0xDB,0x3A,
		0x5E,0xA9,0xDF,0xC6,0xBE,0x34,0x79,0x10,0x78,0xE5,0xD6,0xE0,
		0xDB,0x92,0x8C,0x09,0x44,0x8B,0x6A,0x5B,0xF7,0xF7,0x7F,0x5E,
		0x1A,0xE0,0xDB,0x77,0x91,0x8B,0x39,0x95,0xFC,0xDE,0xAE,0x2E,
		0x2A,0x0F,0x2D,0x3D,0xEA,0x47,0x4E,0x63,
		};
	static unsigned char dh1024_g[]={
		0x02,
		};
	DH *dh;

	if ((dh=DH_new()) == NULL) return(NULL);
	dh->p=BN_bin2bn(dh1024_p,sizeof(dh1024_p),NULL);
	dh->g=BN_bin2bn(dh1024_g,sizeof(dh1024_g),NULL);
	if ((dh->p == NULL) || (dh->g == NULL))
		{ DH_free(dh); return(NULL); }
	return(dh);
	}
#endif
