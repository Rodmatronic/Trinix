/*
 * from MUSL
 */

#define _IOC(a,b,c,d) ( ((a)<<30) | ((b)<<8) | (c) | ((d)<<16) )
#define _IOC_NONE  0U
#define _IOC_WRITE 1U
#define _IOC_READ  2U

#define _IO(a,b) _IOC(_IOC_NONE,(a),(b),0)
#define _IOW(a,b,c) _IOC(_IOC_WRITE,(a),(b),sizeof(c))
#define _IOR(a,b,c) _IOC(_IOC_READ,(a),(b),sizeof(c))
#define _IOWR(a,b,c) _IOC(_IOC_READ|_IOC_WRITE,(a),(b),sizeof(c))

#define TCGETS          0x5401
#define TCSETS          0x5402
#define TCSETSW         0x5403
#define TCSETSF         0x5404
#define TCGETA          0x5405
#define TCSETA          0x5406
#define TCSETAW         0x5407
#define TCSETAF         0x5408
#define TCSBRK          0x5409
#define TCXONC          0x540A
#define TCFLSH          0x540B
#define TIOCEXCL        0x540C
#define TIOCNXCL        0x540D
#define TIOCSCTTY       0x540E
#define TIOCGPGRP       0x540F
#define TIOCSPGRP       0x5410
#define TIOCOUTQ        0x5411
#define TIOCSTI         0x5412
#define TIOCGWINSZ      0x5413
#define TIOCSWINSZ      0x5414
#define TIOCMGET        0x5415
#define TIOCMBIS        0x5416
#define TIOCMBIC        0x5417
#define TIOCMSET        0x5418
#define TIOCGSOFTCAR    0x5419
#define TIOCSSOFTCAR    0x541A
#define FIONREAD        0x541B
#define TIOCINQ         FIONREAD
#define TIOCLINUX       0x541C
#define TIOCCONS        0x541D
#define TIOCGSERIAL     0x541E
#define TIOCSSERIAL     0x541F
#define TIOCPKT         0x5420
#define FIONBIO         0x5421
#define TIOCNOTTY       0x5422
#define TIOCSETD        0x5423
#define TIOCGETD        0x5424
#define TCSBRKP         0x5425
#define TIOCSBRK        0x5427
#define TIOCCBRK        0x5428
#define TIOCGSID        0x5429
#define TIOCGRS485      0x542E
#define TIOCSRS485      0x542F
#define TIOCGPTN        0x80045430
#define TIOCSPTLCK      0x40045431
#define TIOCGDEV        0x80045432
#define TCGETX          0x5432
#define TCSETX          0x5433
#define TCSETXF         0x5434
#define TCSETXW         0x5435
#define TIOCSIG         0x40045436
#define TIOCVHANGUP     0x5437
#define TIOCGPKT        0x80045438
#define TIOCGPTLCK      0x80045439
#define TIOCGEXCL       0x80045440
#define TIOCGPTPEER     0x5441
#define TIOCGISO7816    0x80285442
#define TIOCSISO7816    0xc0285443

#define FIONCLEX        0x5450
#define FIOCLEX         0x5451
#define FIOASYNC        0x5452
#define TIOCSERCONFIG   0x5453
#define TIOCSERGWILD    0x5454
#define TIOCSERSWILD    0x5455
#define TIOCGLCKTRMIOS  0x5456
#define TIOCSLCKTRMIOS  0x5457
#define TIOCSERGSTRUCT  0x5458
#define TIOCSERGETLSR   0x5459
#define TIOCSERGETMULTI 0x545A
#define TIOCSERSETMULTI 0x545B

#define TIOCMIWAIT      0x545C
#define TIOCGICOUNT     0x545D
#define FIOQSIZE        0x5460

#define VT_OPENQRY      0x5600


#define VT_OPENQRY	0x5600	/* find available vt */

struct vt_mode {
	char mode;		/* vt mode */
	char waitv;		/* if set, hang on writes if not active */
	short relsig;		/* signal to raise on release req */
	short acqsig;		/* signal to raise on acquisition */
	short frsig;		/* unused (set to 0) */
};
#define VT_GETMODE	0x5601	/* get mode of active vt */
#define VT_SETMODE	0x5602	/* set mode of active vt */
#define		VT_AUTO		0x00	/* auto vt switching */
#define		VT_PROCESS	0x01	/* process controls switching */
#define		VT_ACKACQ	0x02	/* acknowledge switch */

struct vt_stat {
	unsigned short v_active;	/* active vt */
	unsigned short v_signal;	/* signal to send */
	unsigned short v_state;		/* vt bitmask */
};
#define VT_GETSTATE	0x5603	/* get global vt state info */
#define VT_SENDSIG	0x5604	/* signal to send to bitmask of vts */

#define VT_RELDISP	0x5605	/* release display */

#define VT_ACTIVATE	0x5606	/* make vt active */
#define VT_WAITACTIVE	0x5607	/* wait for vt active */
#define VT_DISALLOCATE	0x5608  /* free memory associated to vt */

struct vt_sizes {
	unsigned short v_rows;		/* number of rows */
	unsigned short v_cols;		/* number of columns */
	unsigned short v_scrollsize;	/* number of lines of scrollback */
};
#define VT_RESIZE	0x5609	/* set kernel's idea of screensize */

struct vt_consize {
	unsigned short v_rows;	/* number of rows */
	unsigned short v_cols;	/* number of columns */
	unsigned short v_vlin;	/* number of pixel rows on screen */
	unsigned short v_clin;	/* number of pixel rows per character */
	unsigned short v_vcol;	/* number of pixel columns on screen */
	unsigned short v_ccol;	/* number of pixel columns per character */
};
#define VT_RESIZEX      0x560A  /* set kernel's idea of screensize + more */
#define VT_LOCKSWITCH   0x560B  /* disallow vt switching */
#define VT_UNLOCKSWITCH 0x560C  /* allow vt switching */
#define VT_GETHIFONTMASK 0x560D  /* return hi font mask */

struct vt_event {
	unsigned int event;
#define VT_EVENT_SWITCH		0x0001	/* Console switch */
#define VT_EVENT_BLANK		0x0002	/* Screen blank */
#define VT_EVENT_UNBLANK	0x0004	/* Screen unblank */
#define VT_EVENT_RESIZE		0x0008	/* Resize display */
#define VT_MAX_EVENT		0x000F
	unsigned int oldev;		/* Old console */
	unsigned int newev;		/* New console (if changing) */
	unsigned int pad[4];		/* Padding for expansion */
};

#define VT_WAITEVENT	0x560E	/* Wait for an event */

struct vt_setactivate {
	unsigned int console;
	struct vt_mode mode;
};

#define VT_SETACTIVATE	0x560F	/* Activate and set the mode of a console */

#define TIOCM_LE        0x001
#define TIOCM_DTR       0x002
#define TIOCM_RTS       0x004
#define TIOCM_ST        0x008
#define TIOCM_SR        0x010
#define TIOCM_CTS       0x020
#define TIOCM_CAR       0x040
#define TIOCM_RNG       0x080
#define TIOCM_DSR       0x100
#define TIOCM_CD        TIOCM_CAR
#define TIOCM_RI        TIOCM_RNG
#define TIOCM_OUT1      0x2000
#define TIOCM_OUT2      0x4000
#define TIOCM_LOOP      0x8000

#define FIOSETOWN       0x8901
#define SIOCSPGRP       0x8902
#define FIOGETOWN       0x8903
#define SIOCGPGRP       0x8904
#define SIOCATMARK      0x8905
#if __LONG_MAX == 0x7fffffff
#define SIOCGSTAMP      _IOR(0x89, 6, char[16])
#define SIOCGSTAMPNS    _IOR(0x89, 7, char[16])
#else
#define SIOCGSTAMP      0x8906
#define SIOCGSTAMPNS    0x8907
#endif

#define KDSETMODE       0x4B3A  /* set text/graphics mode */
#define         KD_TEXT         0x00
#define         KD_GRAPHICS     0x01
#define         KD_TEXT0        0x02    /* obsolete */
#define         KD_TEXT1        0x03    /* obsolete */
#define KDGETMODE       0x4B3B  /* get current mode */


