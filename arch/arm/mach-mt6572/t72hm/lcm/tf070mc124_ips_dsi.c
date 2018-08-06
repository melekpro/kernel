#ifndef BUILD_LK
#include <linux/string.h>
#else
#include <string.h>   //for fixed warning issue
#endif

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#else
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#endif
#include "lcm_drv.h"
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (1024)
#define FRAME_HEIGHT (600)

#define LCM_DSI_CMD_MODE									0

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

//static LCM_UTIL_FUNCS lcm_util = {0}; //for fixed warning issue
static LCM_UTIL_FUNCS lcm_util = 
{
	.set_reset_pin = NULL,
	.udelay = NULL,
	.mdelay = NULL,
};


#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

static DEFINE_MUTEX(pmic_lock_mutex);
static DEFINE_MUTEX(pmic_access_mutex);

void pmic_lock(void)
{
    mutex_lock(&pmic_lock_mutex);
}

void pmic_unlock(void)
{
    mutex_unlock(&pmic_lock_mutex);
}

unsigned int pmic_config_interface(unsigned int RegNum, unsigned int val, unsigned int MASK, unsigned int SHIFT)
{
  unsigned int return_value;
  unsigned int rdata;

  mutex_lock(&pmic_access_mutex);
  return_value = pwrap_wacs2(0, RegNum, 0, &rdata);
  if ( return_value )
  {
    mutex_unlock(&pmic_access_mutex);
  }
  else
  {
    return_value = pwrap_wacs2(1, RegNum, rdata & ~(MASK << SHIFT) | (val << SHIFT), &rdata);
    if ( return_value )
    {
      mutex_unlock(&pmic_access_mutex);
    }
    else
    {
      mutex_unlock(&pmic_access_mutex);
    }
  }
  return return_value;
}


#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

int lcd_bl_en_part_4 ()
{
  mt_set_gpio_mode(60, 0);
  mt_set_gpio_dir(60, 1);
  mt_set_gpio_out(60, 1);
}

int stbyb_control_part_2 ()
{
  mt_set_gpio_mode(55, 0);
  mt_set_gpio_dir(55, 1);
  mt_set_gpio_out(55, 1);
}

int upmu_set_rg_vgpl_vosel ()
{
  pmic_lock();
  pmic_config_interface(1328, 0, 7, 5);
  pmic_unlock();
}

int upmu_set_rg_vgpl_en ()
{
  pmic_lock();
  pmic_config_interface(1290, 0, 1, 15);
  pmic_unlock();
}

int lcd_power_en_part_1 ()
{
  upmu_set_rg_vgpl_vosel(3);
  upmu_set_rg_vgpl_en(1);
}

static void lcd_power_en (unsigned char enabled)
{
  upmu_set_rg_vgpl_vosel(3);
  upmu_set_rg_vgpl_en(1);
  printk("PAUL open VGP1 \n", "lcd_power_en");
}

static void lcd_reset (unsigned char enabled)
{
    if(enabled)
    {
    	  mt_set_gpio_mode(59, 0);
        mt_set_gpio_dir(59, 1);
        mt_set_gpio_out(59, 1);
    }
    else
    {	
        mt_set_gpio_mode(59, 0);
        mt_set_gpio_dir(59, 1);
        mt_set_gpio_out(59, 0);    	
    }
}

static void lcd_bl_en(unsigned char enabled)
{
        mt_set_gpio_mode(60, 0);
        mt_set_gpio_dir(60, 1);
        mt_set_gpio_out(60, 1);
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    printk("[IND][LK] Y____1%s\n", "lcm_get_params");
  memset(params, 0, sizeof(LCM_PARAMS));
	
  params->height = 600;
  params->dsi.vertical_active_line = 600;
  params->dsi.mode = 3;
  params->dsi.horizontal_backporch = 60;
  params->physical_width = 150;
  params->dsi.horizontal_frontporch = 60;
  params->physical_height = 94;
    params->dsi.packet_size = 256;
    params->dsi.horizontal_sync_active = 10;
  params->type = 2;
  params->dsi.PLL_CLOCK = 245;
  params->width = 1024;
  params->dbi.te_mode = 1;
  params->dbi.te_edge_polarity = 0;
  params->dsi.LANE_NUM = 2;
  params->dsi.data_format.color_order = 0;
  params->dsi.data_format.trans_seq = 0;
  params->dsi.data_format.padding = 0;
  params->dsi.data_format.format = 2;
  params->dsi.packet_size = 256;
  params->dsi.intermediat_buffer_num = 2;
  params->dsi.PS = 2;
  params->dsi.vertical_sync_active = 1;
  params->dsi.vertical_backporch = 5;
  params->dsi.vertical_frontporch = 5;
  params->dsi.horizontal_sync_active = 10;
  params->dsi.horizontal_active_pixel = 1024;
  params->dsi.pll_div1 = 1;
  params->dsi.pll_div2 = 0;
  params->dsi.fbk_div = 9;
  params->dsi.lcm_int_te_monitor = 0;
  params->dsi.lcm_int_te_period = 1;
  params->dsi.lcm_ext_te_monitor = 0;
  params->dsi.noncont_clock = 1;
  params->dsi.noncont_clock_period = 2;
}

extern void DSI_clk_HS_mode(unsigned char enter);

static void lcm_init(void)
{
    printk("[IND][K] Y___1%s\n", "lcm_init");
}


static void lcm_suspend(void)
{
    printk("[IND][LK] y______1%s\n", "lcm_suspend");
    cust_lcd_bl_en(0);
    cust_lcd_reset(0);
    mt_set_gpio_mode(55, 0);
    mt_set_gpio_dir(55, 1);
    mt_set_gpio_out(55, 0);
    printk("[IND][LK] Y___1%s : %s\n", "lcd_power_en", "off");
    upmu_set_rg_vgp1_vosel(0);
    upmu_set_rg_vgp1_en(0);
    MDELAY(10);
}


static void lcm_resume(void)
{
    unsigned int data_array[16];

    printk("[IND][LK] y_______1%s\n", "lcm_resume");
    cust_lcd_bl_en(0);
    cust_lcd_reset(0);
    printk("[IND][LK] Y___1%s : %s\n", "lcd_power_en", "off");
    upmu_set_rg_vgp1_vosel(0);
    upmu_set_rg_vgp1_en(0);
    MDELAY(20);

    printk("[IND][LK] Y___1%s : %s\n", "lcd_power_en", "on");
    lcd_power_en_part_1();

    mt_set_gpio_mode(55, 0);
    mt_set_gpio_dir(55, 1);
    mt_set_gpio_out(55, 1);

    MDELAY(60);

    cust_lcd_reset(1);
    MDELAY(20);

    cust_lcd_reset(0);
    MDELAY(30);

    cust_lcd_reset(1);
    MDELAY(20);

    printk("[IND][K] y_____1%s\n", "init_lcm_registers");

    data_array[0] = 0x10B21500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0x58801500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0x47811500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0xD4821500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0x88831500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0xA9841500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0xC3851500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0x82861500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);

    MDELAY(180);
    MDELAY(80);

    cust_lcd_bl_en(1);
}

void cust_lcd_bl_en(int enabled)
{
    if (enabled == 0)
    {
        mt_set_gpio_mode(60, 0);
        mt_set_gpio_dir(60, 1);
        mt_set_gpio_out(60, 0);
    }
    else
    {
        mt_set_gpio_mode(60, 0);
        mt_set_gpio_dir(60, 1);
        mt_set_gpio_out(60, 1);
    }
}

void cust_lcd_reset(int enabled)
{
    if (enabled == 0)
    {
        mt_set_gpio_mode(59, 0);
        mt_set_gpio_dir(59, 1);
        mt_set_gpio_out(59, 0);
    }
    else
    {
        mt_set_gpio_mode(59, 0);
        mt_set_gpio_dir(59, 1);
        mt_set_gpio_out(59, 1);
    }
}

void upmu_set_rg_vgp1_vosel(int val)
{
    pmic_lock();
    pmic_config_interface(1328, val, 7, 5);
    pmic_unlock();
}

void upmu_set_rg_vgp1_en(int val)
{
    pmic_lock();
    pmic_config_interface(1290, val, 1, 15);
    pmic_unlock();
}

void lcd_power_en_part_1(void)
{
    upmu_set_rg_vgp1_vosel(3);
    upmu_set_rg_vgp1_en(1);
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER tf070mc124_ips_dsi_lcm_drv = 
{
    .name			= "tf070mc124_ips_dsi",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
};