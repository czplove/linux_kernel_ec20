/*
 * SiliconBackplane GCI core hardware definitions
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: sbgci.h 598826 2015-11-11 04:38:33Z $
 */

#ifndef _SBGCI_H
#define _SBGCI_H

#if !defined(_LANGUAGE_ASSEMBLY) && !defined(__ASSEMBLY__)


#ifndef PAD
#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)
#endif	

typedef volatile struct {
	uint32	gci_corecaps0;				
	uint32	gci_corecaps1;				
	uint32	gci_corecaps2;				
	uint32	gci_corectrl;				
	uint32	gci_corestat;				
	uint32	gci_intstat;				
	uint32	gci_intmask;				
	uint32	gci_wakemask;				
	uint32	gci_levelintstat;			
	uint32	gci_eventintstat;			
	uint32	gci_wakelevelintstat;			
	uint32	gci_wakeeventintstat;			
	uint32	semaphoreintstatus;			
	uint32	semaphoreintmask;			
	uint32	semaphorerequest;			
	uint32	semaphorereserve;			
	uint32	gci_indirect_addr;			
	uint32	gci_gpioctl;				
	uint32	gci_gpiostatus;				
	uint32	gci_gpiomask;				
	uint32	eventsummary;				
	uint32	gci_miscctl;				
	uint32	gci_gpiointmask;			
	uint32	gci_gpiowakemask;			
	uint32	gci_input[32];				
	uint32	gci_event[32];				
	uint32	gci_output[4];				
	uint32	gci_control_0;				
	uint32	gci_control_1;				
	uint32	gci_intpolreg;				
	uint32	gci_levelintmask;			
	uint32	gci_eventintmask;			
	uint32	wakelevelintmask;			
	uint32	wakeeventintmask;			
	uint32	hwmask;					
	uint32	PAD;
	uint32	gci_inbandeventintmask;			
	uint32	PAD;
	uint32	gci_inbandeventstatus;			
	uint32	gci_seciauxtx;				
	uint32	gci_seciauxrx;				
	uint32	gci_secitx_datatag;			
	uint32	gci_secirx_datatag;			
	uint32	gci_secitx_datamask;			
	uint32	gci_seciusef0tx_reg;			
	uint32	gci_secif0tx_offset;			
	uint32	gci_secif0rx_offset;			
	uint32	gci_secif1tx_offset;			
	uint32	gci_rxfifo_common_ctrl;			
	uint32	gci_rxfifoctrl;				
	uint32	PAD;
	uint32	gci_uartescval;				
	uint32	gic_seciuartautobaudctr;		
	uint32	gci_secififolevel;			
	uint32	gci_seciuartdata;			
	uint32	gci_secibauddiv;			
	uint32	gci_secifcr;				
	uint32	gci_secilcr;				
	uint32	gci_secimcr;				
	uint32	gci_secilsr;				
	uint32	gci_secimsr;				
	uint32	gci_baudadj;				
	uint32	gci_inbandintmask;			
	uint32  gci_chipctrl;				
	uint32  gci_chipsts;				
	uint32	gci_gpioout;				
	uint32	gci_gpioout_read;			
	uint32	gci_mpwaketx;				
	uint32	gci_mpwakedetect;			
	uint32	gci_seciin_ctrl;			
	uint32	gci_seciout_ctrl;			
	uint32	gci_seciin_auxfifo_en;			
	uint32	gci_seciout_txen_txbr;			
	uint32	gci_seciin_rxbrstatus;			
	uint32	gci_seciin_rxerrstatus;			
	uint32	gci_seciin_fcstatus;			
	uint32	gci_seciout_txstatus;			
	uint32	gci_seciout_txbrstatus;			
	uint32	wlan_mem_info;				
	uint32	wlan_bankxinfo;				
	uint32	bt_smem_select;				
	uint32	bt_smem_stby;				
	uint32	bt_smem_status;				
	uint32	wlan_bankxactivepda;			
	uint32	wlan_bankxsleeppda;			
	uint32	wlan_bankxkill;				
	uint32	PAD[41];
	uint32	gci_chipid;				
	uint32	PAD[3];
	uint32	otpstatus;				
	uint32	otpcontrol;				
	uint32	otpprog;				
	uint32	otplayout;				
	uint32	otplayoutextension;			
	uint32	otpcontrol1;				
	uint32	otpprogdata;				
	uint32	PAD[52];
	uint32	otpECCstatus;				
	uint32	PAD[512];
	uint32	lhl_core_capab_adr;			
	uint32	lhl_main_ctl_adr;			
	uint32	lhl_pmu_ctl_adr;			
	uint32	lhl_extlpo_ctl_adr;			
	uint32	lpo_ctl_adr;				
	uint32	lhl_lpo2_ctl_adr;			
	uint32	lhl_osc32k_ctl_adr;			
	uint32	lhl_clk_status_adr;			
	uint32	lhl_clk_det_ctl_adr;			
	uint32	lhl_clk_sel_adr;			
	uint32	hidoff_cnt_adr[2];			
	uint32	lhl_autoclk_ctl_adr;			
	uint32	PAD;					
	uint32	lhl_hibtim_adr;				
	uint32	lhl_wl_ilp_val_adr;			
	uint32  lhl_wl_armtim0_intrp_adr;		
	uint32	lhl_wl_armtim0_st_adr;			
	uint32  lhl_wl_armtim0_adr;			
	uint32  PAD[9];					
	uint32 lhl_wl_mactim0_intrp_adr;		
	uint32 lhl_wl_mactim0_st_adr;			
	uint32 lhl_wl_mactim_int0_adr;			
	uint32 lhl_wl_mactim_frac0_adr;			
	uint32 lhl_wl_mactim1_intrp_adr;		
	uint32 lhl_wl_mactim1_st_adr;			
	uint32 lhl_wl_mactim_int1_adr;			
	uint32 lhl_wl_mactim_frac1_adr;			
	uint32  PAD[8];					
	uint32	gpio_int_en_port_adr[4];		
	uint32	gpio_int_st_port_adr[4];		
	uint32	gpio_ctrl_iocfg_p_adr[64];		
	uint32	gpio_gctrl_iocfg_p0_p39_adr;		
	uint32	gpio_gdsctrl_iocfg_p0_p25_p30_p39_adr;	
	uint32	gpio_gdsctrl_iocfg_p26_p29_adr; 	
	uint32  PAD[8];					
	uint32	lhl_gpio_din0_adr;			
	uint32	lhl_gpio_din1_adr;			
	uint32	lhl_wkup_status_adr;			
	uint32	lhl_ctl_adr;				
	uint32	lhl_adc_ctl_adr;			
	uint32	lhl_qdxyz_in_dly_adr;			
	uint32	lhl_optctl_adr; 			
	uint32	lhl_optct2_adr; 			
	uint32	lhl_scanp_cntr_init_val_adr;		
	uint32	lhl_opt_togg_val_adr[6];		
	uint32	lhl_optx_smp_val_adr;			
	uint32	lhl_opty_smp_val_adr;			
	uint32	lhl_optz_smp_val_adr;			
	uint32	lhl_hidoff_keepstate_adr[3];		
	uint32	lhl_bt_slmboot_ctl0_adr[4]; 		
	uint32	lhl_wl_fw_ctl;				
	uint32	lhl_wl_hw_ctl_adr[2];			
	uint32	lhl_bt_hw_ctl_adr;			
	uint32	lhl_top_pwrseq_en_adr;			
	uint32	lhl_top_pwrdn_ctl_adr;			
	uint32	lhl_top_pwrup_ctl_adr;			
	uint32	lhl_top_pwrseq_ctl_adr; 		
	uint32	lhl_top_pwrdn2_ctl_adr; 		
	uint32	lhl_top_pwrup2_ctl_adr;			
	uint32	wpt_regon_intrp_cfg_adr;		
	uint32	bt_regon_intrp_cfg_adr;			
	uint32	wl_regon_intrp_cfg_adr;			
	uint32	regon_intrp_st_adr;			
	uint32	regon_intrp_en_adr;			
} gciregs_t;


#define GCI_CORE_CAP_0_COREREV_MASK			0xFF
#define GCI_CORE_CAP_0_COREREV_SHIFT			0

#define GCI_INDIRECT_ADDRESS_REG_REGINDEX_MASK		0x3F
#define GCI_INDIRECT_ADDRESS_REG_REGINDEX_SHIFT		0
#define GCI_INDIRECT_ADDRESS_REG_GPIOINDEX_MASK		0xF
#define GCI_INDIRECT_ADDRESS_REG_GPIOINDEX_SHIFT	16

#define WLAN_BANKX_SLEEPPDA_REG_SLEEPPDA_MASK		0xFFFF

#define WLAN_BANKX_PKILL_REG_SLEEPPDA_MASK		0x1


#define WLAN_BANKXINFO_BANK_SIZE_MASK			0x00FFF000
#define WLAN_BANKXINFO_BANK_SIZE_SHIFT			12


#define WLAN_MEM_INFO_REG_NUMSOCRAMBANKS_MASK		0x000000FF
#define WLAN_MEM_INFO_REG_NUMSOCRAMBANKS_SHIFT		0

#define WLAN_MEM_INFO_REG_NUMD11MACBM_MASK		0x0000FF00
#define WLAN_MEM_INFO_REG_NUMD11MACBM_SHIFT		8

#define WLAN_MEM_INFO_REG_NUMD11MACUCM_MASK		0x00FF0000
#define WLAN_MEM_INFO_REG_NUMD11MACUCM_SHIFT		16

#define WLAN_MEM_INFO_REG_NUMD11MACSHM_MASK		0xFF000000
#define WLAN_MEM_INFO_REG_NUMD11MACSHM_SHIFT		24

#endif 


#endif	
