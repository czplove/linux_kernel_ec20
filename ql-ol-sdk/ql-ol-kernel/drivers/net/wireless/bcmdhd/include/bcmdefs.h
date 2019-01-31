/*
 * Misc system wide definitions
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: bcmdefs.h 588205 2015-09-23 11:54:25Z $
 */

#ifndef	_bcmdefs_h_
#define	_bcmdefs_h_




#define BCM_REFERENCE(data)	((void)(data))


#ifdef __GNUC__
#define UNUSED_VAR     __attribute__ ((unused))
#else
#define UNUSED_VAR
#endif


#define STATIC_ASSERT(expr) { \
	 \
	typedef enum { _STATIC_ASSERT_NOT_CONSTANT = (expr) } _static_assert_e UNUSED_VAR; \
	 \
	typedef char STATIC_ASSERT_FAIL[(expr) ? 1 : -1] UNUSED_VAR; \
}



#define bcmreclaimed 		0
#define BCMATTACHDATA(_data)	_data
#define BCMATTACHFN(_fn)	_fn
#define BCMPREATTACHDATA(_data)	_data
#define BCMPREATTACHFN(_fn)	_fn
#define BCMINITDATA(_data)	_data
#define BCMINITFN(_fn)		_fn
#define BCMUNINITFN(_fn)	_fn
#define	BCMNMIATTACHFN(_fn)	_fn
#define	BCMNMIATTACHDATA(_data)	_data
#define CONST	const

#if !defined STB
#if defined(__ARM_ARCH_7A__) && !defined(OEM_ANDROID)
#ifndef BCM_SECURE_DMA
#define BCM47XX_CA9
#endif 
#else
#undef BCM47XX_CA9
#endif 
#endif 

#ifndef BCMFASTPATH
#if defined(BCM47XX_CA9) || defined(STB)
#define BCMFASTPATH		__attribute__ ((__section__ (".text.fastpath")))
#define BCMFASTPATH_HOST	__attribute__ ((__section__ (".text.fastpath_host")))
#else
#define BCMFASTPATH
#define BCMFASTPATH_HOST
#endif
#endif 



	#define BCMRAMFN(_fn)	_fn

#define STATIC	static


#define	SI_BUS			0	
#define	PCI_BUS			1	
#define	PCMCIA_BUS		2	
#define SDIO_BUS		3	
#define JTAG_BUS		4	
#define USB_BUS			5	
#define SPI_BUS			6	
#define RPC_BUS			7	


#ifdef BCMBUSTYPE
#define BUSTYPE(bus) 	(BCMBUSTYPE)
#else
#define BUSTYPE(bus) 	(bus)
#endif


#ifdef BCMCHIPTYPE
#define CHIPTYPE(bus) 	(BCMCHIPTYPE)
#else
#define CHIPTYPE(bus) 	(bus)
#endif



#if defined(BCMSPROMBUS)
#define SPROMBUS	(BCMSPROMBUS)
#elif defined(SI_PCMCIA_SROM)
#define SPROMBUS	(PCMCIA_BUS)
#else
#define SPROMBUS	(PCI_BUS)
#endif


#ifdef BCMCHIPID
#define CHIPID(chip)	(BCMCHIPID)
#else
#define CHIPID(chip)	(chip)
#endif

#ifdef BCMCHIPREV
#define CHIPREV(rev)	(BCMCHIPREV)
#else
#define CHIPREV(rev)	(rev)
#endif

#ifdef BCMPCIEREV
#define PCIECOREREV(rev)	(BCMPCIEREV)
#else
#define PCIECOREREV(rev)	(rev)
#endif


#define DMADDR_MASK_32 0x0		
#define DMADDR_MASK_30 0xc0000000	
#define DMADDR_MASK_26 0xFC000000	
#define DMADDR_MASK_0  0xffffffff	

#define	DMADDRWIDTH_26  26 
#define	DMADDRWIDTH_30  30 
#define	DMADDRWIDTH_32  32 
#define	DMADDRWIDTH_63  63 
#define	DMADDRWIDTH_64  64 

typedef struct {
	uint32 loaddr;
	uint32 hiaddr;
} dma64addr_t;

#define PHYSADDR64HI(_pa) ((_pa).hiaddr)
#define PHYSADDR64HISET(_pa, _val) \
	do { \
		(_pa).hiaddr = (_val);		\
	} while (0)
#define PHYSADDR64LO(_pa) ((_pa).loaddr)
#define PHYSADDR64LOSET(_pa, _val) \
	do { \
		(_pa).loaddr = (_val);		\
	} while (0)

#ifdef BCMDMA64OSL
typedef dma64addr_t dmaaddr_t;
#define PHYSADDRHI(_pa) PHYSADDR64HI(_pa)
#define PHYSADDRHISET(_pa, _val) PHYSADDR64HISET(_pa, _val)
#define PHYSADDRLO(_pa)  PHYSADDR64LO(_pa)
#define PHYSADDRLOSET(_pa, _val) PHYSADDR64LOSET(_pa, _val)

#else
typedef unsigned long dmaaddr_t;
#define PHYSADDRHI(_pa) (0)
#define PHYSADDRHISET(_pa, _val)
#define PHYSADDRLO(_pa) ((_pa))
#define PHYSADDRLOSET(_pa, _val) \
	do { \
		(_pa) = (_val);			\
	} while (0)
#endif 
#define PHYSADDRISZERO(_pa) (PHYSADDRLO(_pa) == 0 && PHYSADDRHI(_pa) == 0)


typedef struct  {
	dmaaddr_t addr;
	uint32	  length;
} hnddma_seg_t;

#define MAX_DMA_SEGS 8


typedef struct {
	void *oshdmah; 
	uint origsize; 
	uint nsegs;
	hnddma_seg_t segs[MAX_DMA_SEGS];
} hnddma_seg_map_t;




#if defined(BCM_RPC_NOCOPY) || defined(BCM_RCP_TXNOCOPY)

#define BCMEXTRAHDROOM 260
#else 
#if (defined(BCM47XX_CA9) || defined(STB))
#if defined(BCM_GMAC3)
#define BCMEXTRAHDROOM 32 
#else
#define BCMEXTRAHDROOM 224
#endif 
#else
#define BCMEXTRAHDROOM 204
#endif 
#endif 


#ifndef SDALIGN
#define SDALIGN	32
#endif


#define BCMDONGLEHDRSZ 12
#define BCMDONGLEPADSZ 16

#define BCMDONGLEOVERHEAD	(BCMDONGLEHDRSZ + BCMDONGLEPADSZ)


#if defined(NO_BCMDBG_ASSERT)
# undef BCMDBG_ASSERT
# undef BCMASSERT_LOG
#endif

#if defined(BCMASSERT_LOG)
#define BCMASSERT_SUPPORT
#endif 


#define BITFIELD_MASK(width) \
		(((unsigned)1 << (width)) - 1)
#define GFIELD(val, field) \
		(((val) >> field ## _S) & field ## _M)
#define SFIELD(val, field, bits) \
		(((val) & (~(field ## _M << field ## _S))) | \
		 ((unsigned)(bits) << field ## _S))


#ifdef BCMSMALL
#undef	BCMSPACE
#define bcmspace	FALSE	
#else
#define	BCMSPACE
#define bcmspace	TRUE	
#endif


#ifndef MAXSZ_NVRAM_VARS
#ifdef LARGE_NVRAM_MAXSZ
#define MAXSZ_NVRAM_VARS	LARGE_NVRAM_MAXSZ
#else

#define	MAXSZ_NVRAM_VARS	6144
#endif 
#endif 






#ifdef BCMLFRAG 
	extern bool _bcmlfrag;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMLFRAG_ENAB() (_bcmlfrag)
	#elif defined(BCMLFRAG_DISABLED)
		#define BCMLFRAG_ENAB()	(0)
	#else
		#define BCMLFRAG_ENAB()	(1)
	#endif
#else
	#define BCMLFRAG_ENAB()		(0)
#endif 
#define	RXMODE1	1	
#define	RXMODE2	2	
#define	RXMODE3	3	
#define	RXMODE4	4	

#ifdef BCMSPLITRX 
	extern bool _bcmsplitrx;
	extern uint8 _bcmsplitrx_mode;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMSPLITRX_ENAB() (_bcmsplitrx)
		#define BCMSPLITRX_MODE() (_bcmsplitrx_mode)
	#elif defined(BCMSPLITRX_DISABLED)
		#define BCMSPLITRX_ENAB()	(0)
		#define BCMSPLITRX_MODE()	(0)
	#else
		#define BCMSPLITRX_ENAB()	(1)
		#define BCMSPLITRX_MODE() (_bcmsplitrx_mode)
	#endif
#else
	#define BCMSPLITRX_ENAB()		(0)
	#define BCMSPLITRX_MODE()		(0)
#endif 

#ifdef BCMPCIEDEV 
extern bool _pciedevenab;
	#if defined(WL_ENAB_RUNTIME_CHECK)
		#define BCMPCIEDEV_ENAB() (_pciedevenab)
	#elif defined(BCMPCIEDEV_ENABLED)
		#define BCMPCIEDEV_ENAB()	1
	#else
		#define BCMPCIEDEV_ENAB()	0
	#endif
#else
	#define BCMPCIEDEV_ENAB()	0
#endif 

#define SPLIT_RXMODE1()	((BCMSPLITRX_MODE() == RXMODE1))
#define SPLIT_RXMODE2()	((BCMSPLITRX_MODE() == RXMODE2))
#define SPLIT_RXMODE3()	((BCMSPLITRX_MODE() == RXMODE3))
#define SPLIT_RXMODE4()	((BCMSPLITRX_MODE() == RXMODE4))

#define PKT_CLASSIFY()	(SPLIT_RXMODE2() || SPLIT_RXMODE3() || SPLIT_RXMODE4())
#define RXFIFO_SPLIT()	(SPLIT_RXMODE3() || SPLIT_RXMODE4())
#define HDR_CONV()	(SPLIT_RXMODE4())

#define PKT_CLASSIFY_EN(x)	((PKT_CLASSIFY()) && (PKT_CLASSIFY_FIFO == (x)))
#ifdef BCM_SPLITBUF
	extern bool _bcmsplitbuf;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCM_SPLITBUF_ENAB() (_bcmsplitbuf)
	#elif defined(BCM_SPLITBUF_DISABLED)
		#define BCM_SPLITBUF_ENAB()	(0)
	#else
		#define BCM_SPLITBUF_ENAB()	(1)
	#endif
#else
	#define BCM_SPLITBUF_ENAB()		(0)
#endif	


#ifdef DL_NVRAM
#define NVRAM_ARRAY_MAXSIZE	DL_NVRAM
#else
#define NVRAM_ARRAY_MAXSIZE	MAXSZ_NVRAM_VARS
#endif 

extern uint32 gFWID;


#endif 
