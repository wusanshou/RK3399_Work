#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/mman.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include<linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define GPIO_LOW 0
#define GPIO_HIGH 1
int FAN = 0;
int gpio_rst =0 ;
int gpio_dc =0 ;

unsigned int F8X16[]=	  
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//! 1
  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" 2
  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//# 3
  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$ 4
  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//% 5
  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//& 6
  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' 7
  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 8
  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 9
  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 10
  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+ 11
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//, 12
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//- 13
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//. 14
  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,/// 15
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 16
  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//1 17
  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 18
  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//3 19
  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//4 20
  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//5 21
  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//6 22
  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//7 23
  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 24
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//9 25
  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//: 26
  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//; 27
  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//< 28
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//= 29
  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//> 30
  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//? 31
  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@ 32
  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 33
  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 34
  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 35
  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 36
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 37
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 38
  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 39
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 40
  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 41
  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 42
  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 43
  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 44
  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M 45
  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 46
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 47
  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 48
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q 49
  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 50
  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 51
  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 52
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 53
  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 54
  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W 55
  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 56
  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 57
  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 58
  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[ 59
  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\ 60
  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//] 61
  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 62
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 63
  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//` 64
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a 65
  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 66
  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 67
  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d 68
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e 69
  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 70
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 71
  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 72
  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 73
  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 74
  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k 75
  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 76
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 77
  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 78
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 79
  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p 80
  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q 81
  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 82
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 83
  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t 84
  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 85
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v 86
  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w 87
  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x 88
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y 89
  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 90
  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{ 91
  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//| 92
  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//} 93
  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~ 94
};

#define N_SPI_MINORS			32	/* ... up to 256 */


static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);
static DECLARE_BITMAP(minors, N_SPI_MINORS);

struct spidev_data {
	dev_t				devt;
	spinlock_t			spi_lock;
	struct spi_device	*spi;
	struct list_head	device_entry;
	struct mutex		buf_lock;
	unsigned			users;
	u8					*buffer;
};

struct  oled_fp_data {
	struct spidev_data *spidev;
	struct work_struct work;
	struct workqueue_struct *wq;
};

static struct class *spidev_class;
struct cdev spicdev;
static unsigned int spidev_major = 0;
unsigned long virt_addr;
static unsigned int bufsiz = 4096;


#define LSB_TO_MSB

#ifdef LSB_TO_MSB
static inline unsigned char reversalBits(unsigned char reg)
{
	int i;
	unsigned char ret = 0;
	unsigned char tmp = 0;
	unsigned char tmp1 = 0;
	unsigned char mask = 1;
	for (i = 7; i >= 0; i--)
	{
		tmp = reg >> (7 - i);
		tmp1 = (tmp << i) & (mask << i);
		ret |= tmp1;
	}
	return ret;
	
}
static void Modifybuf(u8 *buf, size_t len)
{
	u8 tmp;
	int i;

	for (i = 0; i < len; i++)
	{
		tmp = reversalBits(buf[i]);
		buf[i] = tmp;
	}
}
#endif



static void spidev_complete(void *arg)
{
	complete(arg);
}

static ssize_t
spidev_sync(struct spidev_data *spidev, struct spi_message *message)
{
	DECLARE_COMPLETION_ONSTACK(done);
	int status;

#ifdef LSB_TO_MSB	
	struct list_head *p;
	struct spi_transfer *t;
	
	list_for_each(p, &message->transfers) {
		t = list_entry(p, struct spi_transfer, transfer_list);
		if (t->tx_buf) {
			 Modifybuf((u8*)t->tx_buf, t->len);	
		}
	}
#endif	

	message->complete = spidev_complete;
	message->context = &done;

	spin_lock_irq(&spidev->spi_lock);
	if (spidev->spi == NULL)
		status = -ESHUTDOWN;
	else
		status = spi_async(spidev->spi, message);
	spin_unlock_irq(&spidev->spi_lock);

	if (status == 0) {
		wait_for_completion(&done);
		status = message->status;
		if (status == 0)
			status = message->actual_length;
#ifdef LSB_TO_MSB
			list_for_each(p, &message->transfers) {
				t = list_entry(p, struct spi_transfer, transfer_list);
				if (t->rx_buf) {
					Modifybuf((u8*)t->rx_buf, t->len);	
				}
			}
#endif	
	}
	return status;
}

static inline ssize_t
spidev_sync_write(struct spidev_data *spidev, size_t len)
{
	struct spi_transfer	t = {
			.tx_buf		= spidev->buffer,
			.len		= len,
		};
	struct spi_message	m;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	return spidev_sync(spidev, &m);
}

//spi写函数
static inline ssize_t
myspidev_write(struct spidev_data *spidev, int date)
 {
         mutex_lock(&spidev->buf_lock);
 	 u8 tbuf = date;

	//大端小端的数据转换
	 int leng = sizeof(tbuf);
	 tbuf = ( tbuf & 0x55 ) << 1 | ( tbuf & 0xAA ) >> 1;
    	 tbuf = ( tbuf & 0x33 ) << 2 | ( tbuf & 0xCC ) >> 2;
         tbuf = ( tbuf & 0x0F ) << 4 | ( tbuf & 0xF0 ) >> 4;
 
         struct spi_transfer     t = {
                         .tx_buf         = &tbuf,
                         .len            = leng,
                 };
         struct spi_message      m;
         spi_message_init(&m);//将m作为头，创建一个链表
	 //m.spi = spidev->spi;
         spi_message_add_tail(&t, &m);//将t插进链表中
         int i;
	 i=spidev_sync(spidev, &m);
         mutex_unlock(&spidev->buf_lock);
	 return i; 
 }

//spi读函数
static inline int spidev_read(struct spi_device *spi, void *buf, size_t len)
{
        struct spi_transfer     t = {
                        .rx_buf         = buf,
                        .len            = len,
                };
        struct spi_message      m;

        spi_message_init(&m);
        spi_message_add_tail(&t, &m);
        return spi_sync(spi, &m);
}

static int OLED_WR_Byte(struct spidev_data *spidev,int date,int cmd)
{
	if(cmd)
	gpio_direction_output(gpio_dc,GPIO_HIGH);
	else
		gpio_direction_output(gpio_dc,GPIO_LOW);
	myspidev_write(spidev,date);
	return 0;
}

void OLED_Clear(struct spidev_data *spidev)  
{  
	int i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (spidev,0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (spidev,0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (spidev,0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(spidev,0,OLED_DATA); 
	} //更新显示
}

int OLED_Clear_Line(struct spidev_data *spidev,int line)  
{  
	int i,n,j;
	if(line<0||line>4)
	{
		printk("spi_oled_line : the line is over\n");
		return 0;
	}
	i = line*2-2;
	j=i+2;		    
	for(;i<j;i++)  
	{  
		OLED_WR_Byte (spidev,0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (spidev,0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (spidev,0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(spidev,0,OLED_DATA); 
	} //更新显示
	return 0;
}


void OLED_Set_Pos(struct spidev_data *spidev,unsigned char x, unsigned char y) 
{
	
	OLED_WR_Byte(spidev,0xb0+y,OLED_CMD);
	OLED_WR_Byte(spidev,((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(spidev,(x&0x0f)|0x01,OLED_CMD); 
}
 
static void OLED_Init(struct spidev_data *spidev)
{

	gpio_direction_output(gpio_rst,GPIO_HIGH);
	mdelay(100);	
	gpio_direction_output(gpio_rst,GPIO_LOW);
	mdelay(100);	
	gpio_direction_output(gpio_rst,GPIO_HIGH);

	OLED_WR_Byte(spidev,0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(spidev,0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(spidev,0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(spidev,0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(spidev,0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(spidev,0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(spidev,0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(spidev,0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(spidev,0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(spidev,0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(spidev,0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(spidev,0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(spidev,0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(spidev,0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(spidev,0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(spidev,0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(spidev,0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(spidev,0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(spidev,0x12,OLED_CMD);
	OLED_WR_Byte(spidev,0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(spidev,0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(spidev,0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(spidev,0x02,OLED_CMD);//
	OLED_WR_Byte(spidev,0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(spidev,0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(spidev,0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(spidev,0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(spidev,0xAF,OLED_CMD);//--turn on oled panel
	OLED_WR_Byte(spidev,0xAF,OLED_CMD); /*display ON*/

	OLED_Clear(spidev);
	OLED_Set_Pos(spidev,0,0);
	return 0;
}

int OLED_ShowChar_Dra_xy(struct spidev_data *spidev,int x,int y,int chr)
{      

	unsigned char c=0,i=0;	
	
	if(y<1||y>4) {
			printk("spi_oled_showchar :the y is over");
			return 0;
		     }  
	if(x<1||x>16){
			printk("spi_oled_showchar :the x is over\n");
			return 0;
		     }

	c=chr-' ';//得到偏移后的值			
	
	x=(x-1)*8;
	y=(y-1)*2;

	if(x>=119)
	{
			OLED_Set_Pos(spidev,x,y);
			for(i=0;i<7;i++)
			{
				if(FAN == 1)
				{
					OLED_WR_Byte(spidev,~F8X16[c*16+i],OLED_DATA);
				}
				else
				{
					OLED_WR_Byte(spidev,F8X16[c*16+i],OLED_DATA);
				}
			}
			OLED_Set_Pos(spidev,x,y+1);
			for(i=0;i<7;i++)
			{
				if(FAN == 1)
				{
					OLED_WR_Byte(spidev,~F8X16[c*16+i+8],OLED_DATA);
				}
				else
				{
					OLED_WR_Byte(spidev,F8X16[c*16+i+8],OLED_DATA);
				}
			}
	}
	else
	{
			OLED_Set_Pos(spidev,x,y);
			for(i=0;i<8;i++)
			{
				if(FAN == 1)
				{
					OLED_WR_Byte(spidev,~F8X16[c*16+i],OLED_DATA);
				}
				else
				{
					OLED_WR_Byte(spidev,F8X16[c*16+i],OLED_DATA);
				}
			}
			OLED_Set_Pos(spidev,x,y+1);
			for(i=0;i<8;i++)
			{
				if(FAN == 1)
				{
					OLED_WR_Byte(spidev,~F8X16[c*16+i+8],OLED_DATA);
				}
				else
				{
					OLED_WR_Byte(spidev,F8X16[c*16+i+8],OLED_DATA);
				}
			}
	}
	return 0;
}


int OLED_ShowString(struct spidev_data *spidev,int x,int y,u8 *chr)
{
	unsigned char j=0;
	if(y<1||y>4) {
			printk("spi_oled_showstring :the y is over");
			return 0;
		     }  
	if(x<1||x>16){
			printk("spi_oled_showstring :the x is over");
			return 0;
		     }

	while (chr[j]!='\0')
	{
			
		OLED_ShowChar_Dra_xy(spidev,x,y,chr[j]);
		x+=1;
		if(x>16) break;
		j++;	
	}
	return 0;
}


int oled_main(struct spidev_data *spidev)
{	
	OLED_Init(spidev);
	int i;
	OLED_ShowString(spidev,1,1,"====Firefly===="); 
	FAN =1; 
	OLED_ShowString(spidev,1,2,"====Firefly====");  
	OLED_ShowString(spidev,1,3,"====Firefly====");
	FAN =0;
	OLED_ShowString(spidev,1,4,"====Firefly====");  
	
	return 0; 
}

int gpio_init(struct spi_device *pdev)
{
	int flag1,flag2,po,pl;
	struct device_node *spi_node = pdev->dev.of_node;
	gpio_rst = of_get_named_gpio_flags(spi_node,"spi-rst",0,&flag1);
	if(!gpio_is_valid(gpio_rst)){
		printk("spio_rst isn't valid\n");
	}
	po = gpio_request(gpio_rst,"spi-rst");
	if(po!=0){
		printk("spio_rst isn't valid\n");
		gpio_free(gpio_rst);
		}
	printk("the gpiorst is %d\n",gpio_rst);
	gpio_direction_output(gpio_rst,GPIO_LOW);
	
	gpio_dc = of_get_named_gpio_flags(spi_node,"spi-dc",0,&flag2);
	if(!gpio_is_valid(gpio_dc)){
		printk("spio_dc isn't valid\n");
	}
	pl = gpio_request(gpio_dc,"spi-dc");
	if(pl!=0){
		printk("spio_dc isn't valid\n");
		gpio_free(gpio_dc);
		}
	printk("the gpiodc is %d\n",gpio_dc);
	gpio_direction_output(gpio_dc,GPIO_LOW);
	return 0;
}

static int spi_oled_probe(struct spi_device *spi)
{
	int					status;
	unsigned long		minor;
	struct oled_fp_data  *fp_data;
	struct spidev_data	*spidev;

	if(!spi)	
		return -ENOMEM;
	
	/* Allocate driver data */
	spidev = kzalloc(sizeof(*spidev), GFP_KERNEL);
	if (!spidev)
		return -ENOMEM;
		
	/* Initialize the driver data */
	spi->mode |= SPI_CPHA;
	spi->max_speed_hz = 1000*1000;
	spi->bits_per_word = 8;
	spidev->spi = spi;
	spin_lock_init(&spidev->spi_lock);
	mutex_init(&spidev->buf_lock);
	INIT_LIST_HEAD(&spidev->device_entry);//初始化设备链表

	//init fp_data
	fp_data = kzalloc(sizeof(struct oled_fp_data), GFP_KERNEL);
	if(fp_data == NULL){
		status = -ENOMEM;
		return status;
	}
	//set fp_data struct value
	fp_data->spidev = spidev;
	
	//init thread and binding the function
	fp_data->wq = create_singlethread_workqueue("sileadfp");	

	
	mutex_lock(&device_list_lock);
	minor = find_first_zero_bit(minors, N_SPI_MINORS);
	if (minor < N_SPI_MINORS) {
		struct device *dev;
		spidev->devt = MKDEV(spidev_major, minor);
		dev = device_create(spidev_class, &spi->dev, spidev->devt, spidev, "silead_fp_dev");
		status = IS_ERR(dev) ? PTR_ERR(dev) : 0;
	} else {
		dev_dbg(&spi->dev, "no minor number available!\n");
		status = -ENODEV;
	}
	if (status == 0) {
		set_bit(minor, minors);
		list_add(&spidev->device_entry, &device_list);
	}
	mutex_unlock(&device_list_lock);
	if (status == 0)
	{
		spi_set_drvdata(spi, spidev);
		gpio_init(spi);
		oled_main(spidev);
		//OLED_Init(spidev);
	}
	else
		kfree(spidev);
	return status;
}

static int spi_oled_remove(struct spi_device *spi)
{
	struct spidev_data	*spidev = spi_get_drvdata(spi);

	/* make sure ops on existing fds can abort cleanly */
	spin_lock_irq(&spidev->spi_lock);
	spidev->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&spidev->spi_lock);

	/* prevent new opens */
	mutex_lock(&device_list_lock);
	list_del(&spidev->device_entry);
	device_destroy(spidev_class, spidev->devt);
	clear_bit(MINOR(spidev->devt), minors);
	if (spidev->users == 0)
		kfree(spidev);
	mutex_unlock(&device_list_lock);

	return 0;
}

static const struct of_device_id spidev_dt_ids[] = {
	{ .compatible = "spi,oled" },
	{},
};
MODULE_DEVICE_TABLE(of, spidev_dt_ids);

static struct spi_driver spidev_spi_driver = {
	.driver = {
		.name =		"silead_fp",//spidev
		.owner =	THIS_MODULE,
		.of_match_table = of_match_ptr(spidev_dt_ids),
	},
	.probe =	spi_oled_probe,
	.remove =	spi_oled_remove,
};


static int __init spi_oled_init(void)
{
	int status;
        dev_t devno;
	status = alloc_chrdev_region(&devno, 0,255, "sileadfp");//动态分配设备号
        if(status <0 )
                 return status;
	spidev_major = MAJOR(devno);//获取动态分配到的主设备号
	//cdev_init(&spicdev, &spidev_fops);
	spicdev.owner = THIS_MODULE;
	status = cdev_add(&spicdev,MKDEV(spidev_major, 0),N_SPI_MINORS);// 注册字符设备 
	if(status != 0)
		return status;
	
	spidev_class = class_create(THIS_MODULE, "spidev");//创建一个class
	if (IS_ERR(spidev_class)) {
		unregister_chrdev(spidev_major, spidev_spi_driver.driver.name);
		return PTR_ERR(spidev_class);
	}

	status = spi_register_driver(&spidev_spi_driver);//注册进SPI子系统
	if (status < 0) {
		class_destroy(spidev_class);
		unregister_chrdev(spidev_major, spidev_spi_driver.driver.name);
	}

	return status;
}

static void __exit spi_oled_exit(void)
{
	cdev_del(&spicdev);
	spi_unregister_driver(&spidev_spi_driver);
	class_destroy(spidev_class);
	unregister_chrdev(spidev_major, spidev_spi_driver.driver.name);
}

module_init(spi_oled_init);
module_exit(spi_oled_exit);

MODULE_AUTHOR("oled <ljz@t-chip.com>");
MODULE_DESCRIPTION("ROCKCHIP User mode SPI device driver interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:spi_oled");