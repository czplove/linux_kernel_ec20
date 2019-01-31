/*
 * Linux OS Independent Layer
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: linux_osl.h 606142 2015-12-14 14:34:29Z $
 */

#ifndef _linux_osl_h_
#define _linux_osl_h_

#include <typedefs.h>
#define DECLSPEC_ALIGN(x)	__attribute__ ((aligned(x)))


extern void * osl_os_open_image(char * filename);
extern int osl_os_get_image_block(char * buf, int len, void * image);
extern void osl_os_close_image(void * image);
extern int osl_os_image_size(void *image);


#ifdef BCMDRIVER


#ifdef SHARED_OSL_CMN
extern osl_t *osl_attach(void *pdev, uint bustype, bool pkttag, void **osh_cmn);
#else
extern osl_t *osl_attach(void *pdev, uint bustype, bool pkttag);
#endif 

extern void osl_detach(osl_t *osh);
extern int osl_static_mem_init(osl_t *osh, void *adapter);
extern int osl_static_mem_deinit(osl_t *osh, void *adapter);
extern void osl_set_bus_handle(osl_t *osh, void *bus_handle);
extern void* osl_get_bus_handle(osl_t *osh);


extern uint32 g_assert_type;

#ifdef CONFIG_PHYS_ADDR_T_64BIT
#define PRI_FMT_x       "llx"
#define PRI_FMT_X       "llX"
#define PRI_FMT_o       "llo"
#define PRI_FMT_d       "lld"
#else
#define PRI_FMT_x       "x"
#define PRI_FMT_X       "X"
#define PRI_FMT_o       "o"
#define PRI_FMT_d       "d"
#endif 

#if defined(BCMASSERT_LOG)
	#define ASSERT(exp) \
	  do { if (!(exp)) osl_assert(#exp, __FILE__, __LINE__); } while (0)
extern void osl_assert(const char *exp, const char *file, int line);
#else
	#ifdef __GNUC__
		#define GCC_VERSION \
			(__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
		#if GCC_VERSION > 30100
			#define ASSERT(exp)	do {} while (0)
		#else
			
			#define ASSERT(exp)
		#endif 
	#endif 
#endif 


static inline void bcm_prefetch_32B(const uint8 *addr, const int cachelines_32B)
{
#if (defined(BCM47XX_CA9) || (defined(STB) && defined(__arm__))) && (__LINUX_ARM_ARCH__ \
	>= 5)
	switch (cachelines_32B) {
		case 4: __asm__ __volatile__("pld\t%a0" :: "p"(addr + 96) : "cc");
		case 3: __asm__ __volatile__("pld\t%a0" :: "p"(addr + 64) : "cc");
		case 2: __asm__ __volatile__("pld\t%a0" :: "p"(addr + 32) : "cc");
		case 1: __asm__ __volatile__("pld\t%a0" :: "p"(addr +  0) : "cc");
	}
#endif 
}


#define	OSL_DELAY(usec)		osl_delay(usec)
extern void osl_delay(uint usec);

#define OSL_SLEEP(ms)			osl_sleep(ms)
extern void osl_sleep(uint ms);

#define	OSL_PCMCIA_READ_ATTR(osh, offset, buf, size) \
	osl_pcmcia_read_attr((osh), (offset), (buf), (size))
#define	OSL_PCMCIA_WRITE_ATTR(osh, offset, buf, size) \
	osl_pcmcia_write_attr((osh), (offset), (buf), (size))
extern void osl_pcmcia_read_attr(osl_t *osh, uint offset, void *buf, int size);
extern void osl_pcmcia_write_attr(osl_t *osh, uint offset, void *buf, int size);


#define	OSL_PCI_READ_CONFIG(osh, offset, size) \
	osl_pci_read_config((osh), (offset), (size))
#define	OSL_PCI_WRITE_CONFIG(osh, offset, size, val) \
	osl_pci_write_config((osh), (offset), (size), (val))
extern uint32 osl_pci_read_config(osl_t *osh, uint offset, uint size);
extern void osl_pci_write_config(osl_t *osh, uint offset, uint size, uint val);


#define OSL_PCI_BUS(osh)	osl_pci_bus(osh)
#define OSL_PCI_SLOT(osh)	osl_pci_slot(osh)
#define OSL_PCIE_DOMAIN(osh)	osl_pcie_domain(osh)
#define OSL_PCIE_BUS(osh)	osl_pcie_bus(osh)
extern uint osl_pci_bus(osl_t *osh);
extern uint osl_pci_slot(osl_t *osh);
extern uint osl_pcie_domain(osl_t *osh);
extern uint osl_pcie_bus(osl_t *osh);
extern struct pci_dev *osl_pci_device(osl_t *osh);

#define OSL_ACP_COHERENCE		(1<<1L)
#define OSL_FWDERBUF			(1<<2L)


typedef struct {
	bool pkttag;
	bool mmbus;		
	pktfree_cb_fn_t tx_fn;  
	void *tx_ctx;		
	void	*unused[3];
} osl_pubinfo_t;

extern void osl_flag_set(osl_t *osh, uint32 mask);
extern void osl_flag_clr(osl_t *osh, uint32 mask);
extern bool osl_is_flag_set(osl_t *osh, uint32 mask);

#define PKTFREESETCB(osh, _tx_fn, _tx_ctx)		\
	do {						\
	   ((osl_pubinfo_t*)osh)->tx_fn = _tx_fn;	\
	   ((osl_pubinfo_t*)osh)->tx_ctx = _tx_ctx;	\
	} while (0)



#define BUS_SWAP32(v)		(v)
	#define MALLOC(osh, size)	osl_malloc((osh), (size))
	#define MALLOCZ(osh, size)	osl_mallocz((osh), (size))
	#define MFREE(osh, addr, size)	osl_mfree((osh), (addr), (size))
	#define MALLOCED(osh)		osl_malloced((osh))
	#define MEMORY_LEFTOVER(osh) osl_check_memleak(osh)
	extern void *osl_malloc(osl_t *osh, uint size);
	extern void *osl_mallocz(osl_t *osh, uint size);
	extern void osl_mfree(osl_t *osh, void *addr, uint size);
	extern uint osl_malloced(osl_t *osh);
	extern uint osl_check_memleak(osl_t *osh);


#define	MALLOC_FAILED(osh)	osl_malloc_failed((osh))
extern uint osl_malloc_failed(osl_t *osh);


#define	DMA_CONSISTENT_ALIGN	osl_dma_consistent_align()
#define	DMA_ALLOC_CONSISTENT(osh, size, align, tot, pap, dmah) \
	osl_dma_alloc_consistent((osh), (size), (align), (tot), (pap))
#define	DMA_FREE_CONSISTENT(osh, va, size, pa, dmah) \
	osl_dma_free_consistent((osh), (void*)(va), (size), (pa))

#define	DMA_ALLOC_CONSISTENT_FORCE32(osh, size, align, tot, pap, dmah) \
	osl_dma_alloc_consistent((osh), (size), (align), (tot), (pap))
#define	DMA_FREE_CONSISTENT_FORCE32(osh, va, size, pa, dmah) \
	osl_dma_free_consistent((osh), (void*)(va), (size), (pa))

extern uint osl_dma_consistent_align(void);
extern void *osl_dma_alloc_consistent(osl_t *osh, uint size, uint16 align,
	uint *tot, dmaaddr_t *pap);
extern void osl_dma_free_consistent(osl_t *osh, void *va, uint size, dmaaddr_t pa);


#define DMA_NO	0	
#define	DMA_TX	1	
#define	DMA_RX	2	


#define	DMA_UNMAP(osh, pa, size, direction, p, dmah) \
	osl_dma_unmap((osh), (pa), (size), (direction))
extern dmaaddr_t osl_dma_map(osl_t *osh, void *va, uint size, int direction, void *p,
	hnddma_seg_map_t *txp_dmah);
extern void osl_dma_unmap(osl_t *osh, uint pa, uint size, int direction);


#define OSL_DMADDRWIDTH(osh, addrwidth) ({BCM_REFERENCE(osh); BCM_REFERENCE(addrwidth);})

#define OSL_SMP_WMB()	smp_wmb()


extern void osl_cpu_relax(void);
#define OSL_CPU_RELAX() osl_cpu_relax()

#if (!defined(DHD_USE_COHERENT_MEM_FOR_RING) && defined(__ARM_ARCH_7A__)) || \
	(defined(STBLINUX) && defined(__ARM_ARCH_7A__))
	extern void osl_cache_flush(void *va, uint size);
	extern void osl_cache_inv(void *va, uint size);
	extern void osl_prefetch(const void *ptr);
	#define OSL_CACHE_FLUSH(va, len)	osl_cache_flush((void *)(va), len)
	#define OSL_CACHE_INV(va, len)		osl_cache_inv((void *)(va), len)
	#define OSL_PREFETCH(ptr)			osl_prefetch(ptr)
#if defined(__ARM_ARCH_7A__)
	extern int osl_arch_is_coherent(void);
	#define OSL_ARCH_IS_COHERENT()		osl_arch_is_coherent()
	extern int osl_acp_war_enab(void);
	#define OSL_ACP_WAR_ENAB()			osl_acp_war_enab()
#if defined(BCM47XX_CA9) && defined(BCM_GMAC3)
	
	extern void cache_flush_line(void *va);
	extern void cache_inv_line(void *va);
	extern void osl_cache_flush_noacp(void *va, uint len);
	extern void osl_cache_inv_noacp(void *va, uint len);
	#define OSL_CACHE_FLUSH_LINE(va)    cache_flush_line((void*)(va))
	#define OSL_CACHE_INV_LINE(va)      cache_inv_line((void*)(va))
	#define OSL_CACHE_FLUSH_NOACP(va, len) cache_flush_len((void*)(va), (len))
	#define OSL_CACHE_INV_NOACP(va, len) cache_inv_len((void*)(va), (len))
#endif 
#else  
	#define OSL_ARCH_IS_COHERENT()		NULL
	#define OSL_ACP_WAR_ENAB()			NULL
#endif 
#else  
	#define OSL_CACHE_FLUSH(va, len)	BCM_REFERENCE(va)
	#define OSL_CACHE_INV(va, len)		BCM_REFERENCE(va)
	#define OSL_PREFETCH(ptr)		BCM_REFERENCE(ptr)

	#define OSL_ARCH_IS_COHERENT()		NULL
	#define OSL_ACP_WAR_ENAB()			NULL
#endif 


	#include <bcmsdh.h>
	#define OSL_WRITE_REG(osh, r, v) (bcmsdh_reg_write(osl_get_bus_handle(osh), \
		(uintptr)(r), sizeof(*(r)), (v)))
	#define OSL_READ_REG(osh, r) (bcmsdh_reg_read(osl_get_bus_handle(osh), \
		(uintptr)(r), sizeof(*(r))))

#if (defined(BCM47XX_CA9) || (defined(STB) && defined(__arm__)))
	#define SELECT_BUS_WRITE(osh, mmap_op, bus_op) ({BCM_REFERENCE(osh); mmap_op;})
	#define SELECT_BUS_READ(osh, mmap_op, bus_op) ({BCM_REFERENCE(osh); bus_op;})
#else 
	#define SELECT_BUS_WRITE(osh, mmap_op, bus_op) if (((osl_pubinfo_t*)(osh))->mmbus) \
		mmap_op else bus_op
	#define SELECT_BUS_READ(osh, mmap_op, bus_op) (((osl_pubinfo_t*)(osh))->mmbus) ? \
		mmap_op : bus_op
#endif 

#define OSL_ERROR(bcmerror)	osl_error(bcmerror)
extern int osl_error(int bcmerror);


#define	PKTBUFSZ	2048   

#define OSH_NULL   NULL


#include <linuxver.h>           
#include <linux/kernel.h>       
#include <linux/string.h>       
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 4, 29)
#define OSL_SYSUPTIME()		((uint32)jiffies_to_msecs(jiffies))
#else
#define OSL_SYSUPTIME()		((uint32)jiffies * (1000 / HZ))
#endif 
#define	printf(fmt, args...)	printk(fmt , ## args)
#include <linux/kernel.h>	
#include <linux/string.h>	

#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bcmp(b1, b2, len)	memcmp((b1), (b2), (len))
#define	bzero(b, len)		memset((b), '\0', (len))



#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 1)) && defined(CONFIG_64BIT) && \
	defined(CONFIG_X86)
#define R_REG(osh, r) (\
	SELECT_BUS_READ(osh, \
		({ \
			__typeof(*(r)) __osl_v; \
			switch (sizeof(*(r))) { \
				case sizeof(uint8):	__osl_v = \
					readb((volatile uint8*)(r)); break; \
				case sizeof(uint16):	__osl_v = \
					readw((volatile uint16*)(r)); break; \
				case sizeof(uint32):	__osl_v = \
					readl((volatile uint32*)(r)); break; \
				case sizeof(uint64):	__osl_v = \
					readq((volatile uint64*)(r)); break;  \
			} \
			__osl_v; \
		}), \
		OSL_READ_REG(osh, r)) \
)
#else
#define R_REG(osh, r) (\
	SELECT_BUS_READ(osh, \
		({ \
			__typeof(*(r)) __osl_v; \
			switch (sizeof(*(r))) { \
				case sizeof(uint8):	__osl_v = \
					readb((volatile uint8*)(r)); break; \
				case sizeof(uint16):	__osl_v = \
					readw((volatile uint16*)(r)); break; \
				case sizeof(uint32):	__osl_v = \
					readl((volatile uint32*)(r)); break; \
			} \
			__osl_v; \
		}), \
		OSL_READ_REG(osh, r)) \
)
#endif 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 1)) && defined(CONFIG_64BIT) && \
	defined(CONFIG_X86)
#define W_REG(osh, r, v) do { \
	SELECT_BUS_WRITE(osh, \
		switch (sizeof(*(r))) { \
			case sizeof(uint8):	writeb((uint8)(v), (volatile uint8*)(r)); break; \
			case sizeof(uint16):	writew((uint16)(v), (volatile uint16*)(r)); break; \
			case sizeof(uint32):	writel((uint32)(v), (volatile uint32*)(r)); break; \
			case sizeof(uint64):	writeq((uint64)(v), (volatile uint64*)(r)); break; \
		}, \
		(OSL_WRITE_REG(osh, r, v))); \
	} while (0)
#else
#define W_REG(osh, r, v) do { \
	SELECT_BUS_WRITE(osh, \
		switch (sizeof(*(r))) { \
			case sizeof(uint8):	writeb((uint8)(v), (volatile uint8*)(r)); break; \
			case sizeof(uint16):	writew((uint16)(v), (volatile uint16*)(r)); break; \
			case sizeof(uint32):	writel((uint32)(v), (volatile uint32*)(r)); break; \
		}, \
		(OSL_WRITE_REG(osh, r, v))); \
	} while (0)
#endif  

#define	AND_REG(osh, r, v)		W_REG(osh, (r), R_REG(osh, r) & (v))
#define	OR_REG(osh, r, v)		W_REG(osh, (r), R_REG(osh, r) | (v))


#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bcmp(b1, b2, len)	memcmp((b1), (b2), (len))
#define	bzero(b, len)		memset((b), '\0', (len))


#define OSL_UNCACHED(va)	((void *)va)
#define OSL_CACHED(va)		((void *)va)

#define OSL_PREF_RANGE_LD(va, sz) BCM_REFERENCE(va)
#define OSL_PREF_RANGE_ST(va, sz) BCM_REFERENCE(va)


#if defined(__i386__)
#define	OSL_GETCYCLES(x)	rdtscl((x))
#else
#define OSL_GETCYCLES(x)	((x) = 0)
#endif 


#define	BUSPROBE(val, addr)	({ (val) = R_REG(NULL, (addr)); 0; })


#if !defined(CONFIG_MMC_MSM7X00A)
#define	REG_MAP(pa, size)	ioremap_nocache((unsigned long)(pa), (unsigned long)(size))
#else
#define REG_MAP(pa, size)       (void *)(0)
#endif 
#define	REG_UNMAP(va)		iounmap((va))


#define	R_SM(r)			*(r)
#define	W_SM(r, v)		(*(r) = (v))
#define	BZERO_SM(r, len)	memset((r), '\0', (len))


#include <linuxver.h>		


#ifdef BCMDBG_CTRACE
#define	PKTGET(osh, len, send)		osl_pktget((osh), (len), __LINE__, __FILE__)
#define	PKTDUP(osh, skb)		osl_pktdup((osh), (skb), __LINE__, __FILE__)
#else
#ifdef BCM_OBJECT_TRACE
#define	PKTGET(osh, len, send)		osl_pktget((osh), (len), __LINE__, __FUNCTION__)
#define	PKTDUP(osh, skb)		osl_pktdup((osh), (skb), __LINE__, __FUNCTION__)
#else
#define	PKTGET(osh, len, send)		osl_pktget((osh), (len))
#define	PKTDUP(osh, skb)		osl_pktdup((osh), (skb))
#endif 
#endif 
#define PKTLIST_DUMP(osh, buf)		BCM_REFERENCE(osh)
#define PKTDBG_TRACE(osh, pkt, bit)	BCM_REFERENCE(osh)
#if defined(BCM_OBJECT_TRACE)
#define	PKTFREE(osh, skb, send)		osl_pktfree((osh), (skb), (send), __LINE__, __FUNCTION__)
#else
#define	PKTFREE(osh, skb, send)		osl_pktfree((osh), (skb), (send))
#endif 
#ifdef CONFIG_DHD_USE_STATIC_BUF
#define	PKTGET_STATIC(osh, len, send)		osl_pktget_static((osh), (len))
#define	PKTFREE_STATIC(osh, skb, send)		osl_pktfree_static((osh), (skb), (send))
#else
#define	PKTGET_STATIC	PKTGET
#define	PKTFREE_STATIC	PKTFREE
#endif 
#define	PKTDATA(osh, skb)		({BCM_REFERENCE(osh); (((struct sk_buff*)(skb))->data);})
#define	PKTLEN(osh, skb)		({BCM_REFERENCE(osh); (((struct sk_buff*)(skb))->len);})
#define PKTHEADROOM(osh, skb)		(PKTDATA(osh, skb)-(((struct sk_buff*)(skb))->head))
#define PKTEXPHEADROOM(osh, skb, b)	\
	({ \
	 BCM_REFERENCE(osh); \
	 skb_realloc_headroom((struct sk_buff*)(skb), (b)); \
	 })
#define PKTTAILROOM(osh, skb)		\
	({ \
	 BCM_REFERENCE(osh); \
	 skb_tailroom((struct sk_buff*)(skb)); \
	 })
#define PKTPADTAILROOM(osh, skb, padlen) \
	({ \
	 BCM_REFERENCE(osh); \
	 skb_pad((struct sk_buff*)(skb), (padlen)); \
	 })
#define	PKTNEXT(osh, skb)		({BCM_REFERENCE(osh); (((struct sk_buff*)(skb))->next);})
#define	PKTSETNEXT(osh, skb, x)		\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->next = (struct sk_buff*)(x)); \
	 })
#define	PKTSETLEN(osh, skb, len)	\
	({ \
	 BCM_REFERENCE(osh); \
	 __skb_trim((struct sk_buff*)(skb), (len)); \
	 })
#define	PKTPUSH(osh, skb, bytes)	\
	({ \
	 BCM_REFERENCE(osh); \
	 skb_push((struct sk_buff*)(skb), (bytes)); \
	 })
#define	PKTPULL(osh, skb, bytes)	\
	({ \
	 BCM_REFERENCE(osh); \
	 skb_pull((struct sk_buff*)(skb), (bytes)); \
	 })
#define	PKTTAG(skb)			((void*)(((struct sk_buff*)(skb))->cb))
#define PKTSETPOOL(osh, skb, x, y)	BCM_REFERENCE(osh)
#define	PKTPOOL(osh, skb)		({BCM_REFERENCE(osh); BCM_REFERENCE(skb); FALSE;})
#define PKTFREELIST(skb)        PKTLINK(skb)
#define PKTSETFREELIST(skb, x)  PKTSETLINK((skb), (x))
#define PKTPTR(skb)             (skb)
#define PKTID(skb)              ({BCM_REFERENCE(skb); 0;})
#define PKTSETID(skb, id)       ({BCM_REFERENCE(skb); BCM_REFERENCE(id);})
#define PKTSHRINK(osh, m)		({BCM_REFERENCE(osh); m;})
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)) && defined(TSQ_MULTIPLIER)
#define PKTORPHAN(skb)          osl_pkt_orphan_partial(skb)
extern void osl_pkt_orphan_partial(struct sk_buff *skb);
#else
#define PKTORPHAN(skb)          ({BCM_REFERENCE(skb); 0;})
#endif 


#ifdef BCMDBG_CTRACE
#define	DEL_CTRACE(zosh, zskb) { \
	unsigned long zflags; \
	spin_lock_irqsave(&(zosh)->ctrace_lock, zflags); \
	list_del(&(zskb)->ctrace_list); \
	(zosh)->ctrace_num--; \
	(zskb)->ctrace_start = 0; \
	(zskb)->ctrace_count = 0; \
	spin_unlock_irqrestore(&(zosh)->ctrace_lock, zflags); \
}

#define	UPDATE_CTRACE(zskb, zfile, zline) { \
	struct sk_buff *_zskb = (struct sk_buff *)(zskb); \
	if (_zskb->ctrace_count < CTRACE_NUM) { \
		_zskb->func[_zskb->ctrace_count] = zfile; \
		_zskb->line[_zskb->ctrace_count] = zline; \
		_zskb->ctrace_count++; \
	} \
	else { \
		_zskb->func[_zskb->ctrace_start] = zfile; \
		_zskb->line[_zskb->ctrace_start] = zline; \
		_zskb->ctrace_start++; \
		if (_zskb->ctrace_start >= CTRACE_NUM) \
			_zskb->ctrace_start = 0; \
	} \
}

#define	ADD_CTRACE(zosh, zskb, zfile, zline) { \
	unsigned long zflags; \
	spin_lock_irqsave(&(zosh)->ctrace_lock, zflags); \
	list_add(&(zskb)->ctrace_list, &(zosh)->ctrace_list); \
	(zosh)->ctrace_num++; \
	UPDATE_CTRACE(zskb, zfile, zline); \
	spin_unlock_irqrestore(&(zosh)->ctrace_lock, zflags); \
}

#define PKTCALLER(zskb)	UPDATE_CTRACE((struct sk_buff *)zskb, (char *)__FUNCTION__, __LINE__)
#endif 

#ifdef CTFPOOL
#define	CTFPOOL_REFILL_THRESH	3
typedef struct ctfpool {
	void		*head;
	spinlock_t	lock;
	osl_t		*osh;
	uint		max_obj;
	uint		curr_obj;
	uint		obj_size;
	uint		refills;
	uint		fast_allocs;
	uint 		fast_frees;
	uint 		slow_allocs;
} ctfpool_t;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
#define	FASTBUF	(1 << 0)
#define	PKTSETFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->pktc_flags) |= FASTBUF); \
	 })
#define	PKTCLRFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->pktc_flags) &= (~FASTBUF)); \
	 })
#define	PKTISFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->pktc_flags) & FASTBUF); \
	 })
#define	PKTFAST(osh, skb)	(((struct sk_buff*)(skb))->pktc_flags)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 22)
#define	FASTBUF	(1 << 16)
#define	PKTSETFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->mac_len) |= FASTBUF); \
	 })
#define	PKTCLRFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->mac_len) &= (~FASTBUF)); \
	 })
#define	PKTISFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->mac_len) & FASTBUF); \
	 })
#define	PKTFAST(osh, skb)	(((struct sk_buff*)(skb))->mac_len)
#else
#define	FASTBUF	(1 << 0)
#define	PKTSETFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->__unused) |= FASTBUF); \
	 })
#define	PKTCLRFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->__unused) &= (~FASTBUF)); \
	 })
#define	PKTISFAST(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 ((((struct sk_buff*)(skb))->__unused) & FASTBUF); \
	 })
#define	PKTFAST(osh, skb)	(((struct sk_buff*)(skb))->__unused)
#endif 

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 22)
#define	CTFPOOLPTR(osh, skb)	(((struct sk_buff*)(skb))->ctfpool)
#define	CTFPOOLHEAD(osh, skb)	(((ctfpool_t *)((struct sk_buff*)(skb))->ctfpool)->head)
#else
#define	CTFPOOLPTR(osh, skb)	(((struct sk_buff*)(skb))->sk)
#define	CTFPOOLHEAD(osh, skb)	(((ctfpool_t *)((struct sk_buff*)(skb))->sk)->head)
#endif

extern void *osl_ctfpool_add(osl_t *osh);
extern void osl_ctfpool_replenish(osl_t *osh, uint thresh);
extern int32 osl_ctfpool_init(osl_t *osh, uint numobj, uint size);
extern void osl_ctfpool_cleanup(osl_t *osh);
extern void osl_ctfpool_stats(osl_t *osh, void *b);
#else 
#define	PKTSETFAST(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTCLRFAST(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTISFAST(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb); FALSE;})
#endif 

#define	PKTSETCTF(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTCLRCTF(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTISCTF(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb); FALSE;})

#ifdef HNDCTF

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
#define	SKIPCT	(1 << 2)
#define	CHAINED	(1 << 3)
#define	PKTSETSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags |= SKIPCT); \
	 })
#define	PKTCLRSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags &= (~SKIPCT)); \
	 })
#define	PKTSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags & SKIPCT); \
	 })
#define	PKTSETCHAINED(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags |= CHAINED); \
	 })
#define	PKTCLRCHAINED(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags &= (~CHAINED)); \
	 })
#define	PKTISCHAINED(skb)	(((struct sk_buff*)(skb))->pktc_flags & CHAINED)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 22)
#define	SKIPCT	(1 << 18)
#define	CHAINED	(1 << 19)
#define	PKTSETSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len |= SKIPCT); \
	 })
#define	PKTCLRSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len &= (~SKIPCT)); \
	 })
#define	PKTSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len & SKIPCT); \
	 })
#define	PKTSETCHAINED(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len |= CHAINED); \
	 })
#define	PKTCLRCHAINED(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len &= (~CHAINED)); \
	 })
#define	PKTISCHAINED(skb)	(((struct sk_buff*)(skb))->mac_len & CHAINED)
#else 
#define	SKIPCT	(1 << 2)
#define	CHAINED	(1 << 3)
#define	PKTSETSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused |= SKIPCT); \
	 })
#define	PKTCLRSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused &= (~SKIPCT)); \
	 })
#define	PKTSKIPCT(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused & SKIPCT); \
	 })
#define	PKTSETCHAINED(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused |= CHAINED); \
	 })
#define	PKTCLRCHAINED(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused &= (~CHAINED)); \
	 })
#define	PKTISCHAINED(skb)	(((struct sk_buff*)(skb))->__unused & CHAINED)
#endif 
typedef struct ctf_mark {
	uint32	value;
}	ctf_mark_t;
#define CTF_MARK(m)				(m.value)
#else 
#define	PKTSETSKIPCT(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTCLRSKIPCT(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTSKIPCT(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define CTF_MARK(m)		({BCM_REFERENCE(m); 0;})
#endif 

#if defined(BCM_GMAC3)




extern void osl_pkt_tofwder(osl_t *osh, void *skbs, int skb_cnt);
#define PKTTOFWDER(osh, skbs, skb_cnt)  \
	osl_pkt_tofwder(((osl_t *)osh), (void *)(skbs), (skb_cnt))


#if defined(BCMDBG_CTRACE) 
extern void osl_pkt_frmfwder(osl_t *osh, void *skbs, int skb_cnt,
                             int line, char *file);
#define PKTFRMFWDER(osh, skbs, skb_cnt) \
	osl_pkt_frmfwder(((osl_t *)osh), (void *)(skbs), (skb_cnt), \
	                 __LINE__, __FILE__)
#else  
extern void osl_pkt_frmfwder(osl_t *osh, void *skbs, int skb_cnt);
#define PKTFRMFWDER(osh, skbs, skb_cnt) \
	osl_pkt_frmfwder(((osl_t *)osh), (void *)(skbs), (skb_cnt))
#endif 





#define FWDER_HWRXOFF       (18)


#define FWDER_MINMAPSZ      (FWDER_HWRXOFF + 14)
#define FWDER_MAXMAPSZ      (FWDER_HWRXOFF + 4 + 14 + 4 + 4)
#define FWDER_PKTMAPSZ      (FWDER_MINMAPSZ)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)

#define FWDERBUF            (1 << 4)
#define PKTSETFWDERBUF(osh, skb) \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags |= FWDERBUF); \
	 })
#define PKTCLRFWDERBUF(osh, skb) \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags &= (~FWDERBUF)); \
	 })
#define PKTISFWDERBUF(osh, skb) \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags & FWDERBUF); \
	 })

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 22)

#define FWDERBUF	        (1 << 20)
#define PKTSETFWDERBUF(osh, skb) \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len |= FWDERBUF); \
	 })
#define PKTCLRFWDERBUF(osh, skb)  \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len &= (~FWDERBUF)); \
	 })
#define PKTISFWDERBUF(osh, skb) \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->mac_len & FWDERBUF); \
	 })

#else 

#define FWDERBUF            (1 << 4)
#define PKTSETFWDERBUF(osh, skb)  \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused |= FWDERBUF); \
	 })
#define PKTCLRFWDERBUF(osh, skb)  \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused &= (~FWDERBUF)); \
	 })
#define PKTISFWDERBUF(osh, skb) \
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->__unused & FWDERBUF); \
	 })

#endif 

#else  

#define PKTSETFWDERBUF(osh, skb)  ({ BCM_REFERENCE(osh); BCM_REFERENCE(skb); })
#define PKTCLRFWDERBUF(osh, skb)  ({ BCM_REFERENCE(osh); BCM_REFERENCE(skb); })
#define PKTISFWDERBUF(osh, skb)   ({ BCM_REFERENCE(osh); BCM_REFERENCE(skb); FALSE;})

#endif 


#ifdef HNDCTF


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
#define	TOBR		(1 << 5)
#define	PKTSETTOBR(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags |= TOBR); \
	 })
#define	PKTCLRTOBR(osh, skb)	\
	({ \
	 BCM_REFERENCE(osh); \
	 (((struct sk_buff*)(skb))->pktc_flags &= (~TOBR)); \
	 })
#define	PKTISTOBR(skb)	(((struct sk_buff*)(skb))->pktc_flags & TOBR)
#define	PKTSETCTFIPCTXIF(skb, ifp)	(((struct sk_buff*)(skb))->ctf_ipc_txif = ifp)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 22)
#define	PKTSETTOBR(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTCLRTOBR(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTISTOBR(skb)	({BCM_REFERENCE(skb); FALSE;})
#define	PKTSETCTFIPCTXIF(skb, ifp)	({BCM_REFERENCE(skb); BCM_REFERENCE(ifp);})
#else 
#define	PKTSETTOBR(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTCLRTOBR(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTISTOBR(skb)	({BCM_REFERENCE(skb); FALSE;})
#define	PKTSETCTFIPCTXIF(skb, ifp)	({BCM_REFERENCE(skb); BCM_REFERENCE(ifp);})
#endif 
#else 
#define	PKTSETTOBR(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTCLRTOBR(osh, skb)	({BCM_REFERENCE(osh); BCM_REFERENCE(skb);})
#define	PKTISTOBR(skb)	({BCM_REFERENCE(skb); FALSE;})
#endif 


#ifdef BCMFA
#ifdef BCMFA_HW_HASH
#define PKTSETFAHIDX(skb, idx)	(((struct sk_buff*)(skb))->napt_idx = idx)
#else
#define PKTSETFAHIDX(skb, idx)	({BCM_REFERENCE(skb); BCM_REFERENCE(idx);})
#endif 
#define PKTGETFAHIDX(skb)	(((struct sk_buff*)(skb))->napt_idx)
#define PKTSETFADEV(skb, imp)	(((struct sk_buff*)(skb))->dev = imp)
#define PKTSETRXDEV(skb)	(((struct sk_buff*)(skb))->rxdev = ((struct sk_buff*)(skb))->dev)

#define	AUX_TCP_FIN_RST	(1 << 0)
#define	AUX_FREED	(1 << 1)
#define PKTSETFAAUX(skb)	(((struct sk_buff*)(skb))->napt_flags |= AUX_TCP_FIN_RST)
#define	PKTCLRFAAUX(skb)	(((struct sk_buff*)(skb))->napt_flags &= (~AUX_TCP_FIN_RST))
#define	PKTISFAAUX(skb)		(((struct sk_buff*)(skb))->napt_flags & AUX_TCP_FIN_RST)
#define PKTSETFAFREED(skb)	(((struct sk_buff*)(skb))->napt_flags |= AUX_FREED)
#define	PKTCLRFAFREED(skb)	(((struct sk_buff*)(skb))->napt_flags &= (~AUX_FREED))
#define	PKTISFAFREED(skb)	(((struct sk_buff*)(skb))->napt_flags & AUX_FREED)
#define	PKTISFABRIDGED(skb)	PKTISFAAUX(skb)
#else
#define	PKTISFAAUX(skb)		({BCM_REFERENCE(skb); FALSE;})
#define	PKTISFABRIDGED(skb)	({BCM_REFERENCE(skb); FALSE;})
#define	PKTISFAFREED(skb)	({BCM_REFERENCE(skb); FALSE;})

#define	PKTCLRFAAUX(skb)	BCM_REFERENCE(skb)
#define PKTSETFAFREED(skb)	BCM_REFERENCE(skb)
#define	PKTCLRFAFREED(skb)	BCM_REFERENCE(skb)
#endif 

#if defined(BCM_OBJECT_TRACE)
extern void osl_pktfree(osl_t *osh, void *skb, bool send, int line, const char *caller);
#else
extern void osl_pktfree(osl_t *osh, void *skb, bool send);
#endif 
extern void *osl_pktget_static(osl_t *osh, uint len);
extern void osl_pktfree_static(osl_t *osh, void *skb, bool send);
extern void osl_pktclone(osl_t *osh, void **pkt);

#ifdef BCMDBG_CTRACE
#define PKT_CTRACE_DUMP(osh, b)	osl_ctrace_dump((osh), (b))
extern void *osl_pktget(osl_t *osh, uint len, int line, char *file);
extern void *osl_pkt_frmnative(osl_t *osh, void *skb, int line, char *file);
extern int osl_pkt_is_frmnative(osl_t *osh, struct sk_buff *pkt);
extern void *osl_pktdup(osl_t *osh, void *skb, int line, char *file);
struct bcmstrbuf;
extern void osl_ctrace_dump(osl_t *osh, struct bcmstrbuf *b);
#else
#ifdef BCM_OBJECT_TRACE
extern void *osl_pktget(osl_t *osh, uint len, int line, const char *caller);
extern void *osl_pktdup(osl_t *osh, void *skb, int line, const char *caller);
#else
extern void *osl_pktget(osl_t *osh, uint len);
extern void *osl_pktdup(osl_t *osh, void *skb);
#endif 
extern void *osl_pkt_frmnative(osl_t *osh, void *skb);
#endif 
extern struct sk_buff *osl_pkt_tonative(osl_t *osh, void *pkt);
#ifdef BCMDBG_CTRACE
#define PKTFRMNATIVE(osh, skb)  osl_pkt_frmnative(((osl_t *)osh), \
				(struct sk_buff*)(skb), __LINE__, __FILE__)
#define	PKTISFRMNATIVE(osh, skb) osl_pkt_is_frmnative((osl_t *)(osh), (struct sk_buff *)(skb))
#else
#define PKTFRMNATIVE(osh, skb)	osl_pkt_frmnative(((osl_t *)osh), (struct sk_buff*)(skb))
#endif 
#define PKTTONATIVE(osh, pkt)		osl_pkt_tonative((osl_t *)(osh), (pkt))

#define	PKTLINK(skb)			(((struct sk_buff*)(skb))->prev)
#define	PKTSETLINK(skb, x)		(((struct sk_buff*)(skb))->prev = (struct sk_buff*)(x))
#define	PKTPRIO(skb)			(((struct sk_buff*)(skb))->priority)
#define	PKTSETPRIO(skb, x)		(((struct sk_buff*)(skb))->priority = (x))
#define PKTSUMNEEDED(skb)		(((struct sk_buff*)(skb))->ip_summed == CHECKSUM_HW)
#define PKTSETSUMGOOD(skb, x)		(((struct sk_buff*)(skb))->ip_summed = \
						((x) ? CHECKSUM_UNNECESSARY : CHECKSUM_NONE))

#define PKTSHARED(skb)                  (((struct sk_buff*)(skb))->cloned)

#ifdef CONFIG_NF_CONNTRACK_MARK
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#define PKTMARK(p)                     (((struct sk_buff *)(p))->mark)
#define PKTSETMARK(p, m)               ((struct sk_buff *)(p))->mark = (m)
#else 
#define PKTMARK(p)                     (((struct sk_buff *)(p))->nfmark)
#define PKTSETMARK(p, m)               ((struct sk_buff *)(p))->nfmark = (m)
#endif 
#else 
#define PKTMARK(p)                     0
#define PKTSETMARK(p, m)
#endif 

#define PKTALLOCED(osh)		osl_pktalloced(osh)
extern uint osl_pktalloced(osl_t *osh);

#define OSL_RAND()		osl_rand()
extern uint32 osl_rand(void);

#if !defined(BCM_SECURE_DMA)
#define	DMA_MAP(osh, va, size, direction, p, dmah) \
	osl_dma_map((osh), (va), (size), (direction), (p), (dmah))
#endif 

#ifdef PKTC

struct chain_node {
	struct sk_buff	*link;
	unsigned int	flags:3, pkts:9, bytes:20;
};

#define CHAIN_NODE(skb)		((struct chain_node*)(((struct sk_buff*)skb)->pktc_cb))

#define	PKTCSETATTR(s, f, p, b)	({CHAIN_NODE(s)->flags = (f); CHAIN_NODE(s)->pkts = (p); \
	                         CHAIN_NODE(s)->bytes = (b);})
#define	PKTCCLRATTR(s)		({CHAIN_NODE(s)->flags = CHAIN_NODE(s)->pkts = \
	                         CHAIN_NODE(s)->bytes = 0;})
#define	PKTCGETATTR(s)		(CHAIN_NODE(s)->flags << 29 | CHAIN_NODE(s)->pkts << 20 | \
	                         CHAIN_NODE(s)->bytes)
#define	PKTCCNT(skb)		(CHAIN_NODE(skb)->pkts)
#define	PKTCLEN(skb)		(CHAIN_NODE(skb)->bytes)
#define	PKTCGETFLAGS(skb)	(CHAIN_NODE(skb)->flags)
#define	PKTCSETFLAGS(skb, f)	(CHAIN_NODE(skb)->flags = (f))
#define	PKTCCLRFLAGS(skb)	(CHAIN_NODE(skb)->flags = 0)
#define	PKTCFLAGS(skb)		(CHAIN_NODE(skb)->flags)
#define	PKTCSETCNT(skb, c)	(CHAIN_NODE(skb)->pkts = (c))
#define	PKTCINCRCNT(skb)	(CHAIN_NODE(skb)->pkts++)
#define	PKTCADDCNT(skb, c)	(CHAIN_NODE(skb)->pkts += (c))
#define	PKTCSETLEN(skb, l)	(CHAIN_NODE(skb)->bytes = (l))
#define	PKTCADDLEN(skb, l)	(CHAIN_NODE(skb)->bytes += (l))
#define	PKTCSETFLAG(skb, fb)	(CHAIN_NODE(skb)->flags |= (fb))
#define	PKTCCLRFLAG(skb, fb)	(CHAIN_NODE(skb)->flags &= ~(fb))
#define	PKTCLINK(skb)		(CHAIN_NODE(skb)->link)
#define	PKTSETCLINK(skb, x)	(CHAIN_NODE(skb)->link = (struct sk_buff*)(x))
#define FOREACH_CHAINED_PKT(skb, nskb) \
	for (; (skb) != NULL; (skb) = (nskb)) \
		if ((nskb) = (PKTISCHAINED(skb) ? PKTCLINK(skb) : NULL), \
		    PKTSETCLINK((skb), NULL), 1)
#define	PKTCFREE(osh, skb, send) \
do { \
	void *nskb; \
	ASSERT((skb) != NULL); \
	FOREACH_CHAINED_PKT((skb), nskb) { \
		PKTCLRCHAINED((osh), (skb)); \
		PKTCCLRFLAGS((skb)); \
		PKTFREE((osh), (skb), (send)); \
	} \
} while (0)
#define PKTCENQTAIL(h, t, p) \
do { \
	if ((t) == NULL) { \
		(h) = (t) = (p); \
	} else { \
		PKTSETCLINK((t), (p)); \
		(t) = (p); \
	} \
} while (0)
#endif 

#else 



	#define ASSERT(exp)	do {} while (0)


#define MALLOC(o, l) malloc(l)
#define MFREE(o, p, l) free(p)
#include <stdlib.h>


#include <string.h>


#include <stdio.h>


extern void bcopy(const void *src, void *dst, size_t len);
extern int bcmp(const void *b1, const void *b2, size_t len);
extern void bzero(void *b, size_t len);
#endif 



#if defined(STBLINUX)

#if defined(__ARM_ARCH_7A__)
#define ACP_WAR_ENAB() 0
#define ACP_WIN_LIMIT 1
#define arch_is_coherent() 0
#endif 

#endif 

#ifdef BCM_SECURE_DMA

#define	SECURE_DMA_MAP(osh, va, size, direction, p, dmah, pcma, offset) \
	osl_sec_dma_map((osh), (va), (size), (direction), (p), (dmah), (pcma), (offset))
#define	SECURE_DMA_DD_MAP(osh, va, size, direction, p, dmah) \
	osl_sec_dma_dd_map((osh), (va), (size), (direction), (p), (dmah))
#define	SECURE_DMA_MAP_TXMETA(osh, va, size, direction, p, dmah, pcma) \
	osl_sec_dma_map_txmeta((osh), (va), (size), (direction), (p), (dmah), (pcma))
#define	SECURE_DMA_UNMAP(osh, pa, size, direction, p, dmah, pcma, offset) \
	osl_sec_dma_unmap((osh), (pa), (size), (direction), (p), (dmah), (pcma), (offset))
#define	SECURE_DMA_UNMAP_ALL(osh, pcma) \
	osl_sec_dma_unmap_all((osh), (pcma))

#define DMA_MAP(osh, va, size, direction, p, dmah)

typedef struct sec_cma_info {
	struct sec_mem_elem *sec_alloc_list;
	struct sec_mem_elem *sec_alloc_list_tail;
} sec_cma_info_t;

#if defined(__ARM_ARCH_7A__)
#define CMA_BUFSIZE_4K	4096
#define CMA_BUFSIZE_2K	2048
#define CMA_BUFSIZE_512	512

#define	CMA_BUFNUM		2048
#define SEC_CMA_COHERENT_BLK 0x8000 
#define SEC_CMA_COHERENT_MAX 278
#define CMA_DMA_DESC_MEMBLOCK	(SEC_CMA_COHERENT_BLK * SEC_CMA_COHERENT_MAX)
#define CMA_DMA_DATA_MEMBLOCK	(CMA_BUFSIZE_4K*CMA_BUFNUM)
#define	CMA_MEMBLOCK		(CMA_DMA_DESC_MEMBLOCK + CMA_DMA_DATA_MEMBLOCK)
#endif 

#define SEC_DMA_ALIGN	(1<<16)
typedef struct sec_mem_elem {
	size_t			size;
	int				direction;
	phys_addr_t		pa_cma;     
	void			*va;        
	dma_addr_t		dma_handle; 
	void			*vac;       
	struct page *pa_cma_page;	
	struct	sec_mem_elem	*next;
} sec_mem_elem_t;

extern dma_addr_t osl_sec_dma_map(osl_t *osh, void *va, uint size, int direction, void *p,
	hnddma_seg_map_t *dmah, void *ptr_cma_info, uint offset);
extern dma_addr_t osl_sec_dma_dd_map(osl_t *osh, void *va, uint size, int direction, void *p,
	hnddma_seg_map_t *dmah);
extern dma_addr_t osl_sec_dma_map_txmeta(osl_t *osh, void *va, uint size,
  int direction, void *p, hnddma_seg_map_t *dmah, void *ptr_cma_info);
extern void osl_sec_dma_unmap(osl_t *osh, dma_addr_t dma_handle, uint size, int direction,
	void *p, hnddma_seg_map_t *map, void *ptr_cma_info, uint offset);
extern void osl_sec_dma_unmap_all(osl_t *osh, void *ptr_cma_info);

#endif 

typedef struct sk_buff_head PKT_LIST;
#define PKTLIST_INIT(x)		skb_queue_head_init((x))
#define PKTLIST_ENQ(x, y)	skb_queue_head((struct sk_buff_head *)(x), (struct sk_buff *)(y))
#define PKTLIST_DEQ(x)		skb_dequeue((struct sk_buff_head *)(x))
#define PKTLIST_UNLINK(x, y)	skb_unlink((struct sk_buff *)(y), (struct sk_buff_head *)(x))
#define PKTLIST_FINI(x)		skb_queue_purge((struct sk_buff_head *)(x))

#endif	
