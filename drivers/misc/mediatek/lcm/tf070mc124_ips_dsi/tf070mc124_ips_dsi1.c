#ifdef BUILD_LK
    #include <string.h>
#else
    #include <linux/string.h>
    #if defined(BUILD_UBOOT)
        #include <asm/arch/mt_gpio.h>
    #else
        #include <mach/mt_gpio.h>
    #endif
#endif
#include "lcm_drv.h"


#if defined(BUILD_LK)
    #define LCM_PRINT printf
#elif defined(BUILD_UBOOT)
    #define LCM_PRINT printf
#else
#define LCM_PRINT printk
#endif

#include <mach/upmu_common.h>

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)  // pixel
#define FRAME_HEIGHT (800)  // pixel

#define PHYSICAL_WIDTH  (56)  // mm
#define PHYSICAL_HEIGHT (93)  // mm

#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA    // END OF REGISTERS MARKER

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define LCM_ID       (0x55)
#define LCM_ID1       (0xBC)
#define LCM_ID2       (0xc0)
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};


#if 0
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
    {0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
    {0x29, 1, {0x00}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    // Display off sequence
    {0x28, 1, {0x00}},
    {REGFLAG_DELAY, 10, {}},

    // Sleep Mode On
    {0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for (i = 0; i < count; i++) {
        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {
        case REGFLAG_DELAY:
            MDELAY(table[i].count);
            break;

        case REGFLAG_END_OF_TABLE:
            break;

        default:
            dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);

            if (cmd != 0xFF && cmd != 0x2C && cmd != 0x3C) {
                //#if defined(BUILD_UBOOT)
                //  printf("[DISP] - uboot - REG_R(0x%x) = 0x%x. \n", cmd, table[i].para_list[0]);
                //#endif
                while (read_reg(cmd) != table[i].para_list[0]);
            }
        }
    }
}
#endif


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
    params->dsi.mode = 3;
    params->physical_width = 150;
    params->physical_height = 94;
    params->dsi.packet_size = 256;
    params->dsi.horizontal_sync_active = 10;
    params->dsi.PLL_CLOCK = 245;
    params->width = 1024;
    params->type = 2;
    params->dsi.LANE_NUM = 2;
    params->dsi.data_format.format = 2;
    params->dsi.intermediat_buffer_num = 2;
    params->dsi.PS = 2;
    params->dsi.horizontal_active_pixel = 1024;
    params->height = 600;
    params->dsi.vertical_active_line = 600;
    params->dbi.te_mode = 1;
    params->dsi.vertical_sync_active = 1;
    params->dbi.te_edge_polarity = 0;
    params->dsi.data_format.color_order = 0;
    params->dsi.data_format.trans_seq = 0;
    params->dsi.data_format.padding = 0;
    params->dsi.vertical_backporch = 5;
    params->dsi.vertical_frontporch = 5;
    params->dsi.horizontal_backporch = 60;
    params->dsi.horizontal_frontporch = 60;
    params->dsi.horizontal_blanking_pixel = 60;
}

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
LCM_DRIVER tf070mc124_ips_dsi_drv = {
    .name = "tf070mc124_ips_dsi",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params = lcm_get_params,
    .init = lcm_init,
    .suspend = lcm_suspend,
    .resume = lcm_resume,
    //.set_pwm        = lcm_setpwm,
    //.get_pwm        = lcm_getpwm,
};
