/*
 * Misc utility routines used by kernel or app-level.
 * Contents are wifi-specific, used by any kernel or app-level
 * software that might want wifi things as it grows.
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: bcmwifi_channels.c 309193 2012-01-19 00:03:57Z $
 */

#include <bcm_cfg.h>
#include <typedefs.h>
#include <bcmutils.h>

#ifdef BCMDRIVER
#include <osl.h>
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define tolower(c) (bcm_isupper((c)) ? ((c) + 'a' - 'A') : (c))
#else
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef ASSERT
#define ASSERT(exp)
#endif
#endif 

#include <bcmwifi_channels.h>

#if defined(WIN32) && (defined(BCMDLL) || defined(WLMDLL))
#include <bcmstdlib.h> 	
#endif






static const char *wf_chspec_bw_str[] =
{
	"5",
	"10",
	"20",
	"40",
	"80",
	"160",
	"80+80",
#ifdef WL11ULB
	"2.5"
#else 
	"na"
#endif 
};

static const uint8 wf_chspec_bw_mhz[] =
{5, 10, 20, 40, 80, 160, 160};

#define WF_NUM_BW \
	(sizeof(wf_chspec_bw_mhz)/sizeof(uint8))


static const uint8 wf_5g_40m_chans[] =
{38, 46, 54, 62, 102, 110, 118, 126, 134, 142, 151, 159};
#define WF_NUM_5G_40M_CHANS \
	(sizeof(wf_5g_40m_chans)/sizeof(uint8))


static const uint8 wf_5g_80m_chans[] =
{42, 58, 106, 122, 138, 155};
#define WF_NUM_5G_80M_CHANS \
	(sizeof(wf_5g_80m_chans)/sizeof(uint8))


static const uint8 wf_5g_160m_chans[] =
{50, 114};
#define WF_NUM_5G_160M_CHANS \
	(sizeof(wf_5g_160m_chans)/sizeof(uint8))



static uint
bw_chspec_to_mhz(chanspec_t chspec)
{
	uint bw;

	bw = (chspec & WL_CHANSPEC_BW_MASK) >> WL_CHANSPEC_BW_SHIFT;
	return (bw >= WF_NUM_BW ? 0 : wf_chspec_bw_mhz[bw]);
}


static uint8
center_chan_to_edge(uint bw)
{
	
	return (uint8)(((bw - 20) / 2) / 5);
}


static uint8
channel_low_edge(uint center_ch, uint bw)
{
	return (uint8)(center_ch - center_chan_to_edge(bw));
}


static int
channel_to_sb(uint center_ch, uint ctl_ch, uint bw)
{
	uint lowest = channel_low_edge(center_ch, bw);
	uint sb;

	if ((ctl_ch - lowest) % 4) {
		
		return -1;
	}

	sb = ((ctl_ch - lowest) / 4);

	
	if (sb >= (bw / 20)) {
		
		return -1;
	}

	return sb;
}


static uint8
channel_to_ctl_chan(uint center_ch, uint bw, uint sb)
{
	return (uint8)(channel_low_edge(center_ch, bw) + sb * 4);
}


static int
channel_80mhz_to_id(uint ch)
{
	uint i;
	for (i = 0; i < WF_NUM_5G_80M_CHANS; i ++) {
		if (ch == wf_5g_80m_chans[i])
			return i;
	}

	return -1;
}


char *
wf_chspec_ntoa_ex(chanspec_t chspec, char *buf)
{
	if (wf_chspec_ntoa(chspec, buf) == NULL)
		snprintf(buf, CHANSPEC_STR_LEN, "invalid 0x%04x", chspec);
	return buf;
}


char *
wf_chspec_ntoa(chanspec_t chspec, char *buf)
{
	const char *band;
	uint ctl_chan;

	if (wf_chspec_malformed(chspec))
		return NULL;

	band = "";

	
	if ((CHSPEC_IS2G(chspec) && CHSPEC_CHANNEL(chspec) > CH_MAX_2G_CHANNEL) ||
	    (CHSPEC_IS5G(chspec) && CHSPEC_CHANNEL(chspec) <= CH_MAX_2G_CHANNEL))
		band = (CHSPEC_IS2G(chspec)) ? "2g" : "5g";

	
	ctl_chan = wf_chspec_ctlchan(chspec);

	
	if (CHSPEC_IS20(chspec)) {
		snprintf(buf, CHANSPEC_STR_LEN, "%s%d", band, ctl_chan);
	} else if (!CHSPEC_IS8080(chspec)) {
		const char *bw;
		const char *sb = "";

		bw = wf_chspec_bw_str[(chspec & WL_CHANSPEC_BW_MASK) >> WL_CHANSPEC_BW_SHIFT];

#ifdef CHANSPEC_NEW_40MHZ_FORMAT
		
		if (CHSPEC_IS40(chspec) && CHSPEC_IS2G(chspec)) {
			sb = CHSPEC_SB_UPPER(chspec) ? "u" : "l";
		}

		snprintf(buf, CHANSPEC_STR_LEN, "%s%d/%s%s", band, ctl_chan, bw, sb);
#else
		
		if (CHSPEC_IS40(chspec)) {
			sb = CHSPEC_SB_UPPER(chspec) ? "u" : "l";
			snprintf(buf, CHANSPEC_STR_LEN, "%s%d%s", band, ctl_chan, sb);
		} else {
			snprintf(buf, CHANSPEC_STR_LEN, "%s%d/%s", band, ctl_chan, bw);
		}
#endif 

	} else {
		
		uint chan1 = (chspec & WL_CHANSPEC_CHAN1_MASK) >> WL_CHANSPEC_CHAN1_SHIFT;
		uint chan2 = (chspec & WL_CHANSPEC_CHAN2_MASK) >> WL_CHANSPEC_CHAN2_SHIFT;

		
		chan1 = (chan1 < WF_NUM_5G_80M_CHANS) ? wf_5g_80m_chans[chan1] : 0;
		chan2 = (chan2 < WF_NUM_5G_80M_CHANS) ? wf_5g_80m_chans[chan2] : 0;

		
		snprintf(buf, CHANSPEC_STR_LEN, "%d/80+80/%d-%d", ctl_chan, chan1, chan2);
	}

	return (buf);
}

static int
read_uint(const char **p, unsigned int *num)
{
	unsigned long val;
	char *endp = NULL;

	val = strtoul(*p, &endp, 10);
	
	if (endp == *p)
		return 0;

	
	*p = endp;
	
	*num = (unsigned int)val;

	return 1;
}


chanspec_t
wf_chspec_aton(const char *a)
{
	chanspec_t chspec;
	uint chspec_ch, chspec_band, bw, chspec_bw, chspec_sb;
	uint num, ctl_ch;
	uint ch1, ch2;
	char c, sb_ul = '\0';
	int i;

	bw = 20;
	chspec_sb = 0;
	chspec_ch = ch1 = ch2 = 0;

	
	if (!read_uint(&a, &num))
		return 0;
	
	c = tolower((int)a[0]);
	if (c == 'g') {
		a++; 

		
		if (num == 2)
			chspec_band = WL_CHANSPEC_BAND_2G;
		else if (num == 5)
			chspec_band = WL_CHANSPEC_BAND_5G;
		else
			return 0;

		
		if (!read_uint(&a, &ctl_ch))
			return 0;

		c = tolower((int)a[0]);
	}
	else {
		
		ctl_ch = num;
		chspec_band = ((ctl_ch <= CH_MAX_2G_CHANNEL) ?
		               WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);
	}

	if (c == '\0') {
		
		chspec_bw = WL_CHANSPEC_BW_20;
		goto done_read;
	}

	a ++; 

	
	if (c == 'u' || c == 'l') {
		sb_ul = c;
		chspec_bw = WL_CHANSPEC_BW_40;
		goto done_read;
	}

	
	if (c != '/')
		return 0;

	
	if (!read_uint(&a, &bw))
		return 0;

	
	if (bw == 2) {
		chspec_bw = WL_CHANSPEC_BW_2P5;
	} else if (bw == 5) {
		chspec_bw = WL_CHANSPEC_BW_5;
	} else if (bw == 10) {
		chspec_bw = WL_CHANSPEC_BW_10;
	} else if (bw == 20) {
		chspec_bw = WL_CHANSPEC_BW_20;
	} else if (bw == 40) {
		chspec_bw = WL_CHANSPEC_BW_40;
	} else if (bw == 80) {
		chspec_bw = WL_CHANSPEC_BW_80;
	} else if (bw == 160) {
		chspec_bw = WL_CHANSPEC_BW_160;
	} else {
		return 0;
	}

	

	c = tolower((int)a[0]);

	
	if (chspec_band == WL_CHANSPEC_BAND_2G && bw == 40) {
		if (c == 'u' || c == 'l') {
			a ++; 
			sb_ul = c;
			goto done_read;
		}
	}

	
	if (c == '+') {
		
		const char plus80[] = "80/";

		
		chspec_bw = WL_CHANSPEC_BW_8080;

		a ++; 

		
		for (i = 0; i < 3; i++) {
			if (*a++ != plus80[i]) {
				return 0;
			}
		}

		
		if (!read_uint(&a, &ch1))
			return 0;

		
		if (a[0] != '-')
			return 0;
		a ++; 

		
		if (!read_uint(&a, &ch2))
			return 0;
	} else if (c == '.') {
		
		
		chspec_bw = WL_CHANSPEC_BW_2P5;

		a ++; 

		
		if (*a++ != '5') {
			return 0;
		}
	}

done_read:
	
	while (a[0] == ' ') {
		a ++;
	}

	
	if (a[0] != '\0')
		return 0;

	

	
	if (sb_ul != '\0') {
		if (sb_ul == 'l') {
			chspec_ch = UPPER_20_SB(ctl_ch);
			chspec_sb = WL_CHANSPEC_CTL_SB_LLL;
		} else if (sb_ul == 'u') {
			chspec_ch = LOWER_20_SB(ctl_ch);
			chspec_sb = WL_CHANSPEC_CTL_SB_LLU;
		}
	}
	
	else if (BW_LE20(chspec_bw)) {
		chspec_ch = ctl_ch;
		chspec_sb = WL_CHANSPEC_CTL_SB_NONE;
	}
	
	else if (chspec_bw != WL_CHANSPEC_BW_8080) {
		
		const uint8 *center_ch = NULL;
		int num_ch = 0;
		int sb = -1;

		if (chspec_bw == WL_CHANSPEC_BW_40) {
			center_ch = wf_5g_40m_chans;
			num_ch = WF_NUM_5G_40M_CHANS;
		} else if (chspec_bw == WL_CHANSPEC_BW_80) {
			center_ch = wf_5g_80m_chans;
			num_ch = WF_NUM_5G_80M_CHANS;
		} else if (chspec_bw == WL_CHANSPEC_BW_160) {
			center_ch = wf_5g_160m_chans;
			num_ch = WF_NUM_5G_160M_CHANS;
		} else {
			return 0;
		}

		for (i = 0; i < num_ch; i ++) {
			sb = channel_to_sb(center_ch[i], ctl_ch, bw);
			if (sb >= 0) {
				chspec_ch = center_ch[i];
				chspec_sb = sb << WL_CHANSPEC_CTL_SB_SHIFT;
				break;
			}
		}

		
		if (sb < 0) {
			return 0;
		}
	}
	
	else {
		int ch1_id = 0, ch2_id = 0;
		int sb;

		
		ch1_id = channel_80mhz_to_id(ch1);
		ch2_id = channel_80mhz_to_id(ch2);

		
		if (ch1_id < 0 || ch2_id < 0)
			return 0;

		
		chspec_ch = (((uint)ch1_id << WL_CHANSPEC_CHAN1_SHIFT) |
		             ((uint)ch2_id << WL_CHANSPEC_CHAN2_SHIFT));

		

		
		sb = channel_to_sb(ch1, ctl_ch, bw);
		if (sb < 0) {
			
			return 0;
		}

		chspec_sb = sb << WL_CHANSPEC_CTL_SB_SHIFT;
	}

	chspec = (chspec_ch | chspec_band | chspec_bw | chspec_sb);

	if (wf_chspec_malformed(chspec))
		return 0;

	return chspec;
}


bool
wf_chspec_malformed(chanspec_t chanspec)
{
	uint chspec_bw = CHSPEC_BW(chanspec);
	uint chspec_ch = CHSPEC_CHANNEL(chanspec);

	
	if (CHSPEC_IS2G(chanspec)) {
		
		if (!BW_LE40(chspec_bw)) {
			return TRUE;
		}
	} else if (CHSPEC_IS5G(chanspec)) {
		if (chspec_bw == WL_CHANSPEC_BW_8080) {
			uint ch1_id, ch2_id;

			
			ch1_id = CHSPEC_CHAN1(chanspec);
			ch2_id = CHSPEC_CHAN2(chanspec);
			if (ch1_id >= WF_NUM_5G_80M_CHANS || ch2_id >= WF_NUM_5G_80M_CHANS)
				return TRUE;

		} else if (BW_LE160(chspec_bw)) {
			if (chspec_ch > MAXCHANNEL) {
				return TRUE;
			}
		} else {
			
			return TRUE;
		}
	} else {
		
		return TRUE;
	}

	
	if (BW_LE20(chspec_bw)) {
		if (CHSPEC_CTL_SB(chanspec) != WL_CHANSPEC_CTL_SB_LLL)
			return TRUE;
	} else if (chspec_bw == WL_CHANSPEC_BW_40) {
		if (CHSPEC_CTL_SB(chanspec) > WL_CHANSPEC_CTL_SB_LLU)
			return TRUE;
	} else if (chspec_bw == WL_CHANSPEC_BW_80 ||
	           chspec_bw == WL_CHANSPEC_BW_8080) {
		if (CHSPEC_CTL_SB(chanspec) > WL_CHANSPEC_CTL_SB_LUU)
			return TRUE;
	}
	else if (chspec_bw == WL_CHANSPEC_BW_160) {
		ASSERT(CHSPEC_CTL_SB(chanspec) <= WL_CHANSPEC_CTL_SB_UUU);
	}
	return FALSE;
}


bool
wf_chspec_valid(chanspec_t chanspec)
{
	uint chspec_bw = CHSPEC_BW(chanspec);
	uint chspec_ch = CHSPEC_CHANNEL(chanspec);

	if (wf_chspec_malformed(chanspec))
		return FALSE;

	if (CHSPEC_IS2G(chanspec)) {
		
		if (BW_LE20(chspec_bw)) {
			if (chspec_ch >= 1 && chspec_ch <= 14)
				return TRUE;
		} else if (chspec_bw == WL_CHANSPEC_BW_40) {
			if (chspec_ch >= 3 && chspec_ch <= 11)
				return TRUE;
		}
	} else if (CHSPEC_IS5G(chanspec)) {
		if (chspec_bw == WL_CHANSPEC_BW_8080) {
			uint16 ch1, ch2;

			ch1 = wf_5g_80m_chans[CHSPEC_CHAN1(chanspec)];
			ch2 = wf_5g_80m_chans[CHSPEC_CHAN2(chanspec)];

			
			if ((ch2 > ch1 + CH_80MHZ_APART) ||
			    (ch1 > ch2 + CH_80MHZ_APART))
				return TRUE;
		} else {
			const uint8 *center_ch;
			uint num_ch, i;

			if (BW_LE40(chspec_bw)) {
				center_ch = wf_5g_40m_chans;
				num_ch = WF_NUM_5G_40M_CHANS;
			} else if (chspec_bw == WL_CHANSPEC_BW_80) {
				center_ch = wf_5g_80m_chans;
				num_ch = WF_NUM_5G_80M_CHANS;
			} else if (chspec_bw == WL_CHANSPEC_BW_160) {
				center_ch = wf_5g_160m_chans;
				num_ch = WF_NUM_5G_160M_CHANS;
			} else {
				
				return FALSE;
			}

			
			if (BW_LE20(chspec_bw)) {
				
				for (i = 0; i < num_ch; i ++) {
					if (chspec_ch == (uint)LOWER_20_SB(center_ch[i]) ||
					    chspec_ch == (uint)UPPER_20_SB(center_ch[i]))
						break; 
				}

				if (i == num_ch) {
					
					if (chspec_ch == 165)
						i = 0;

					
					if (chspec_ch == 34 || chspec_ch == 38 ||
					    chspec_ch == 42 || chspec_ch == 46)
						i = 0;
				}
			} else {
				
				for (i = 0; i < num_ch; i ++) {
					if (chspec_ch == center_ch[i])
						break; 
				}
			}

			if (i < num_ch) {
				
				return TRUE;
			}
		}
	}

	return FALSE;
}


uint8
wf_chspec_ctlchan(chanspec_t chspec)
{
	uint center_chan;
	uint bw_mhz;
	uint sb;

	ASSERT(!wf_chspec_malformed(chspec));

	
	if (CHSPEC_BW_LE20(chspec)) {
		return CHSPEC_CHANNEL(chspec);
	} else {
		sb = CHSPEC_CTL_SB(chspec) >> WL_CHANSPEC_CTL_SB_SHIFT;

		if (CHSPEC_IS8080(chspec)) {
			
			uint chan_id = CHSPEC_CHAN1(chspec);

			bw_mhz = 80;

			
			center_chan = wf_5g_80m_chans[chan_id];
		}
		else {
			bw_mhz = bw_chspec_to_mhz(chspec);
			center_chan = CHSPEC_CHANNEL(chspec) >> WL_CHANSPEC_CHAN_SHIFT;
		}

		return (channel_to_ctl_chan(center_chan, bw_mhz, sb));
	}
}


char *
wf_chspec_to_bw_str(chanspec_t chspec)
{
		return (char *)wf_chspec_bw_str[(CHSPEC_BW(chspec) >> WL_CHANSPEC_BW_SHIFT)];
}


chanspec_t
wf_chspec_ctlchspec(chanspec_t chspec)
{
	chanspec_t ctl_chspec = chspec;
	uint8 ctl_chan;

	ASSERT(!wf_chspec_malformed(chspec));

	
	if (!CHSPEC_BW_LE20(chspec)) {
		ctl_chan = wf_chspec_ctlchan(chspec);
		ctl_chspec = ctl_chan | WL_CHANSPEC_BW_20;
		ctl_chspec |= CHSPEC_BAND(chspec);
	}
	return ctl_chspec;
}


uint16
wf_channel2chspec(uint ctl_ch, uint bw)
{
	uint16 chspec;
	const uint8 *center_ch = NULL;
	int num_ch = 0;
	int sb = -1;
	int i = 0;

	chspec = ((ctl_ch <= CH_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);

	chspec |= bw;

	if (bw == WL_CHANSPEC_BW_40) {
		center_ch = wf_5g_40m_chans;
		num_ch = WF_NUM_5G_40M_CHANS;
		bw = 40;
	} else if (bw == WL_CHANSPEC_BW_80) {
		center_ch = wf_5g_80m_chans;
		num_ch = WF_NUM_5G_80M_CHANS;
		bw = 80;
	} else if (bw == WL_CHANSPEC_BW_160) {
		center_ch = wf_5g_160m_chans;
		num_ch = WF_NUM_5G_160M_CHANS;
		bw = 160;
	} else if (BW_LE20(bw)) {
		chspec |= ctl_ch;
		return chspec;
	} else {
		return 0;
	}

	for (i = 0; i < num_ch; i ++) {
		sb = channel_to_sb(center_ch[i], ctl_ch, bw);
		if (sb >= 0) {
			chspec |= center_ch[i];
			chspec |= (sb << WL_CHANSPEC_CTL_SB_SHIFT);
			break;
		}
	}

	
	if (sb < 0) {
		return 0;
	}

	return chspec;
}


extern chanspec_t wf_chspec_primary40_chspec(chanspec_t chspec)
{
	chanspec_t chspec40 = chspec;
	uint center_chan;
	uint sb;

	ASSERT(!wf_chspec_malformed(chspec));

	
	if (CHSPEC_IS8080(chspec) || CHSPEC_IS160(chspec)) {
		chspec = wf_chspec_primary80_chspec(chspec);
	}

	
	if (CHSPEC_IS80(chspec)) {
		center_chan = CHSPEC_CHANNEL(chspec);
		sb = CHSPEC_CTL_SB(chspec);

		if (sb < WL_CHANSPEC_CTL_SB_UL) {
			
			center_chan -= CH_20MHZ_APART;
			
		} else {
			
			center_chan += CH_20MHZ_APART;
			
			sb -= WL_CHANSPEC_CTL_SB_UL;
		}

		
		chspec40 = (WL_CHANSPEC_BAND_5G | WL_CHANSPEC_BW_40 |
		            sb | center_chan);
	}

	return chspec40;
}


int
wf_mhz2channel(uint freq, uint start_factor)
{
	int ch = -1;
	uint base;
	int offset;

	
	if (start_factor == 0) {
		if (freq >= 2400 && freq <= 2500)
			start_factor = WF_CHAN_FACTOR_2_4_G;
		else if (freq >= 5000 && freq <= 6000)
			start_factor = WF_CHAN_FACTOR_5_G;
	}

	if (freq == 2484 && start_factor == WF_CHAN_FACTOR_2_4_G)
		return 14;

	base = start_factor / 2;

	
	if ((freq < base) || (freq > base + 1000))
		return -1;

	offset = freq - base;
	ch = offset / 5;

	
	if (offset != (ch * 5))
		return -1;

	
	if (start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 13))
		return -1;

	return ch;
}


int
wf_channel2mhz(uint ch, uint start_factor)
{
	int freq;

	if ((start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 14)) ||
	    (ch > 200))
		freq = -1;
	else if ((start_factor == WF_CHAN_FACTOR_2_4_G) && (ch == 14))
		freq = 2484;
	else
		freq = ch * 5 + start_factor / 2;

	return freq;
}

static const uint16 sidebands[] = {
	WL_CHANSPEC_CTL_SB_LLL, WL_CHANSPEC_CTL_SB_LLU,
	WL_CHANSPEC_CTL_SB_LUL, WL_CHANSPEC_CTL_SB_LUU,
	WL_CHANSPEC_CTL_SB_ULL, WL_CHANSPEC_CTL_SB_ULU,
	WL_CHANSPEC_CTL_SB_UUL, WL_CHANSPEC_CTL_SB_UUU
};


chanspec_t
wf_chspec_80(uint8 center_channel, uint8 primary_channel)
{

	chanspec_t chanspec = INVCHANSPEC;
	chanspec_t chanspec_cur;
	uint i;

	for (i = 0; i < WF_NUM_SIDEBANDS_80MHZ; i++) {
		chanspec_cur = CH80MHZ_CHSPEC(center_channel, sidebands[i]);
		if (primary_channel == wf_chspec_ctlchan(chanspec_cur)) {
			chanspec = chanspec_cur;
			break;
		}
	}
	
	return chanspec;
}


chanspec_t
wf_chspec_get8080_chspec(uint8 primary_20mhz, uint8 chan0, uint8 chan1)
{
	int sb = 0;
	uint16 chanspec = 0;
	int chan0_id = 0, chan1_id = 0;
	int seg0, seg1;

	chan0_id = channel_80mhz_to_id(chan0);
	chan1_id = channel_80mhz_to_id(chan1);

	
	if (chan0_id == -1 || chan1_id == -1)
		return INVCHANSPEC;

	
	sb = channel_to_sb(chan0, primary_20mhz, 80);
	if (sb >= 0) {
		
		seg0 = chan0_id;
		seg1 = chan1_id;
	} else {
		
		sb = channel_to_sb(chan1, primary_20mhz, 80);
		if (sb < 0) {
			
			return INVCHANSPEC;
		}
		
		seg0 = chan1_id;
		seg1 = chan0_id;
	}

	chanspec = ((seg0 << WL_CHANSPEC_CHAN1_SHIFT) |
	            (seg1 << WL_CHANSPEC_CHAN2_SHIFT) |
	            (sb << WL_CHANSPEC_CTL_SB_SHIFT) |
	            WL_CHANSPEC_BW_8080 |
	            WL_CHANSPEC_BAND_5G);

	return chanspec;
}


static uint8
wf_chspec_get80Mhz_ch(uint8 chan_80Mhz_id)
{
	if (chan_80Mhz_id < WF_NUM_5G_80M_CHANS)
		return wf_5g_80m_chans[chan_80Mhz_id];

	return 0;
}



uint8
wf_chspec_primary80_channel(chanspec_t chanspec)
{
	uint8 primary80_chan;

	if (CHSPEC_IS80(chanspec))	{
		primary80_chan = CHSPEC_CHANNEL(chanspec);
	}
	else if (CHSPEC_IS8080(chanspec)) {
		
		primary80_chan = wf_chspec_get80Mhz_ch(CHSPEC_CHAN1(chanspec));
	}
	else if (CHSPEC_IS160(chanspec)) {
		uint8 center_chan = CHSPEC_CHANNEL(chanspec);
		uint sb = CHSPEC_CTL_SB(chanspec) >> WL_CHANSPEC_CTL_SB_SHIFT;

		
		if (sb < 4) {
			primary80_chan = center_chan - CH_40MHZ_APART;
		}
		
		else
		{
			primary80_chan = center_chan + CH_40MHZ_APART;
		}
	}
	else {
		
		primary80_chan = -1;
	}
	return primary80_chan;
}


uint8
wf_chspec_secondary80_channel(chanspec_t chanspec)
{
	uint8 secondary80_chan;

	if (CHSPEC_IS8080(chanspec)) {
		secondary80_chan = wf_chspec_get80Mhz_ch(CHSPEC_CHAN2(chanspec));
	}
	else if (CHSPEC_IS160(chanspec)) {
		uint8 center_chan = CHSPEC_CHANNEL(chanspec);
		uint sb = CHSPEC_CTL_SB(chanspec) >> WL_CHANSPEC_CTL_SB_SHIFT;

		
		if (sb < 4) {
			secondary80_chan = center_chan + CH_40MHZ_APART;
		}
		
		else
		{
			secondary80_chan = center_chan - CH_40MHZ_APART;
		}
	}
	else {
		
		secondary80_chan = -1;
	}
	return secondary80_chan;
}


chanspec_t
wf_chspec_primary80_chspec(chanspec_t chspec)
{
	chanspec_t chspec80;
	uint center_chan;
	uint sb;

	ASSERT(!wf_chspec_malformed(chspec));
	if (CHSPEC_IS80(chspec)) {
		chspec80 = chspec;
	}
	else if (CHSPEC_IS8080(chspec)) {

		
		center_chan = wf_chspec_get80Mhz_ch(CHSPEC_CHAN1(chspec));

		sb = CHSPEC_CTL_SB(chspec);

		
		chspec80 = (WL_CHANSPEC_BAND_5G | WL_CHANSPEC_BW_80 | sb | center_chan);
	}
	else if (CHSPEC_IS160(chspec)) {
		center_chan = CHSPEC_CHANNEL(chspec);
		sb = CHSPEC_CTL_SB(chspec);

		if (sb < WL_CHANSPEC_CTL_SB_ULL) {
			
			center_chan -= CH_40MHZ_APART;
		}
		else {
			
			center_chan += CH_40MHZ_APART;
			sb -= WL_CHANSPEC_CTL_SB_ULL;
		}
		
		chspec80 = (WL_CHANSPEC_BAND_5G | WL_CHANSPEC_BW_80 | sb | center_chan);
	}
	else {
		chspec80 = INVCHANSPEC;
	}

	return chspec80;
}

#ifdef WL11AC_80P80
uint8
wf_chspec_channel(chanspec_t chspec)
{
	if (CHSPEC_IS8080(chspec)) {
		return wf_chspec_primary80_channel(chspec);
	}
	else {
		return ((uint8)((chspec) & WL_CHANSPEC_CHAN_MASK));
	}
}
#endif 
