#include <bteifgl.h>

// #ifdef WIN32
#if 1

FILE *gpio_pdm_outpin[32];
byte gpio_pdm_pinval[32];
//s64 gpio_pdm_pinusec[32];

byte *gpio_pdm_outbuf[32];
int gpio_pdm_outrov[32];

s64 gpio_pdm_pinusec;

u32 gpio_pinoutmask;

u32 gpio_pin_in_val;
s64 gpio_pdm_fakeus;

s64 (*GPIO_WavGetTimeUS)(void)=NULL;

s64 GPIO_WavGetTimeUS_FakeUS(void)
{
	return(gpio_pdm_fakeus);
}

BTEIFGL_API void GPIO_DummySetTimeFakeUS(s64 us)
{
	GPIO_WavGetTimeUS=GPIO_WavGetTimeUS_FakeUS;
	gpio_pdm_fakeus=us;
}

void GPIO_DummyInitPin(int pin)
{
	char tb[256];
	FILE *fd;
	int ch, rt, bits, len;
	int i;

//	return;

	if((pin<0) || (pin>=32))
		return;
	if(gpio_pdm_outpin[pin])
		return;

	ch=1; rt=44100; bits=8; len=0;

	sprintf(tb, "dummy_gpio%d.wav", pin);
	fd=fopen(tb, "w+b");
	gpio_pdm_outpin[pin]=fd;
	
	printf("GPIO_DummyInitPin: Init %d, %s\n", pin, tb);

	fputc('R', fd);	fputc('I', fd);	fputc('F', fd);	fputc('F', fd);
	fputc(0, fd);	fputc(0, fd);	fputc(0, fd);	fputc(0, fd);
	fputc('W', fd);	fputc('A', fd);	fputc('V', fd);	fputc('E', fd);

	//format header
	fputc('f', fd);	fputc('m', fd);	fputc('t', fd);	fputc(' ', fd);
	fputc(16, fd);	fputc(0, fd);	fputc(0, fd);	fputc(0, fd);

	fputc(1, fd);		fputc(0, fd);
	fputc(ch, fd);		fputc(0, fd);

	fputc(rt&0xFF, fd);	fputc((rt>>8)&0xFF, fd);
	fputc(0, fd);		fputc(0, fd);

	i=rt*ch*(bits/8);
	fputc(i&0xFF, fd);		fputc((i>>8)&0xFF, fd);
	fputc((i>>16)&0xFF, fd);	fputc((i>>24)&0xFF, fd);

	fputc(ch*(bits/8), fd);		fputc(0, fd);
	fputc(bits, fd);		fputc(0, fd);


	//data
	fputc('d', fd);	fputc('a', fd);	fputc('t', fd);	fputc('a', fd);

	i=len*ch*(bits/8);
	fputc(i&0xFF, fd);			fputc((i>>8)&0xFF, fd);
	fputc((i>>16)&0xFF, fd);	fputc((i>>24)&0xFF, fd);
	
	gpio_pdm_pinval[pin]=0;
//	gpio_pdm_pinusec[pin]=GPIO_GetTimeUS();	

	if(!GPIO_WavGetTimeUS)
		GPIO_WavGetTimeUS=GPIO_GetTimeRtcUS;

//	gpio_pdm_pinusec=GPIO_GetTimeUS();	
//	gpio_pdm_pinusec=GPIO_GetTimeRtcUS();
	gpio_pdm_pinusec=GPIO_WavGetTimeUS();	
	
}

void GPIO_DummyClosePin(int pin)
{
	FILE *fd;
	int i, sz;

	if((pin<0) || (pin>=32))
		return;
	if(!gpio_pdm_outpin[pin])
		return;

//	GPIO_DummyUpdatePin(pin);
//	GPIO_DummyUpdatePins();

	fd=gpio_pdm_outpin[pin];
	sz=ftell(fd);
	fseek(fd, 40, 0);

	i=sz-44;
	fputc(i&0xFF, fd);			fputc((i>>8)&0xFF, fd);
	fputc((i>>16)&0xFF, fd);	fputc((i>>24)&0xFF, fd);
	
	fclose(fd);
	gpio_pdm_outpin[pin]=NULL;
}

void GPIO_DummyUpdatePins()
{
	s64 us, dt;
	int i;

//	us=GPIO_GetTimeUS();
//	us=GPIO_GetTimeRtcUS();
	us=GPIO_WavGetTimeUS();
	dt=us-gpio_pdm_pinusec;
	if(dt<=0)return;
	gpio_pdm_pinusec=us;
	
	for(i=0; i<32; i++)
		GPIO_DummyUpdatePinI(i, dt);
}

void GPIO_DummyUpdatePinI(int pin, s64 dt)
{
	byte tbuf[4096];
	FILE *fd;
//	s64 us, dt;
	int sz, sn, val;
	int i;
	
	if((pin<0) || (pin>=32))
		return;
	if(!gpio_pdm_outpin[pin])
		return;

	fd=gpio_pdm_outpin[pin];
	val=gpio_pdm_pinval[pin];
	
	sn=dt/22;
	if(sn<=0)return;

#if 1
	if(!gpio_pdm_outbuf[pin])
	{
		gpio_pdm_outbuf[pin]=frgl_malloc(1<<16);
		gpio_pdm_outrov[pin]=0;
	}

	i=gpio_pdm_outrov[pin];
	if((i+sn)<(1<<16))
	{
		memset(gpio_pdm_outbuf[pin]+i, val, sn);
		gpio_pdm_outrov[pin]=i+sn;
		return;
	}
	if(i>0)
	{
		fwrite(gpio_pdm_outbuf[pin], 1, i, fd);
		gpio_pdm_outrov[pin]=0;
	}
#endif

	if(sn>=4096)
	{
		memset(tbuf, val, 4096);
	}else if(sn>1)
	{
		memset(tbuf, val, sn);
	}else
	{
		tbuf[0]=val;
	}

	i=sn;
	while(i>=4096)
		{ fwrite(tbuf, 1, 4096, fd); i-=4096; }
	if(i>0)
		{ fwrite(tbuf, 1, i, fd); }

//	for(i=0; i<sn; i++)
//	{
//		fputc(val, fd);
//	}

	sz=ftell(fd);
	fseek(fd, 40, 0);

	i=sz-44;
	fputc(i&0xFF, fd);			fputc((i>>8)&0xFF, fd);
	fputc((i>>16)&0xFF, fd);	fputc((i>>24)&0xFF, fd);

	fseek(fd, sz, 0);
}

BTEIFGL_API void GPIO_SetPinInput(int pin)
{
}

BTEIFGL_API void GPIO_SetPinOutput(int pin)
{
	GPIO_DummyInitPin(pin);
}

BTEIFGL_API void GPIO_SetPinMode(int pin, int mode)
{
}

BTEIFGL_API void GPIO_SetPinModeAlt(int pin, int mode)
{
}

BTEIFGL_API void GPIO_InputPinSetValue(int pin, int val)
{
	if(val)
		{ gpio_pin_in_val|=(1<<pin); }
	else
		{ gpio_pin_in_val&=~(1<<pin); }
}

BTEIFGL_API int GPIO_PinGetValue(int pin)
{
	if(gpio_pin_in_val&(1<<pin))
		return(1);
	return(0);
}

BTEIFGL_API void GPIO_PinSetValue(int pin, int val)
{
	int i;

//	GPIO_DummyUpdatePins();

//	GPIO_DummyInitPin(pin);
//	GPIO_DummyUpdatePin(pin);

	i=val?192:64;
	if(gpio_pdm_pinval[pin]!=i)
		{ GPIO_DummyUpdatePins(); }

	gpio_pdm_pinval[pin]=i;

	if(val)
		{ gpio_pinoutmask|=(1<<pin); }
	else
		{ gpio_pinoutmask&=~(1<<pin); }
}

BTEIFGL_API int GPIO_PinGetOutMask(void)
{
	return(gpio_pinoutmask);
}

BTEIFGL_API int GPIO_PinSetOutMask(int bits)
{
	int i;
	
	for(i=0; i<32; i++)
	{
		GPIO_PinSetValue(i, (bits>>i)&1);
	}

//	gpio_pinoutmask=bits;
	return(0);
}

BTEIFGL_API int GPIO_PinSetOutMask8(int bits)
{
	return(GPIO_PinSetOutMask8A(bits));
}

BTEIFGL_API int GPIO_PinSetOutMask8A(int bits)
{
	int i;
	
	for(i=0; i<8; i++)
	{
		GPIO_PinSetValue(i, (bits>>i)&1);
	}
	return(0);
}

BTEIFGL_API int GPIO_PinSetOutMask8B(int bits)
{
	int i;
	
	for(i=0; i<8; i++)
	{
		GPIO_PinSetValue(8+i, (bits>>i)&1);
	}
	return(0);
}

BTEIFGL_API int GPIO_PinGetInBits(void)
{
	return(0);
}

BTEIFGL_API void GPIO_Init()
{
	static int init=0;
	if(init)return;
	init=1;

	printf("GPIO_Init(Dummy)\n");
}

BTEIFGL_API void GPIO_DeInit()
{
	int i;

	GPIO_DummyUpdatePins();
	
	for(i=0; i<32; i++)
		GPIO_DummyClosePin(i);
}

BTEIFGL_API void GPIO_I2C_Init()
{
	static int init=0;
	if(init)return;
	init=1;

	GPIO_Init();

	printf("GPIO_I2C_Init(Dummy)\n");
}

BTEIFGL_API int GPIO_I2C_PollDone()
{
	return(0);
}

BTEIFGL_API void GPIO_I2C_WaitDone()
{
}

BTEIFGL_API void GPIO_I2C_CmdReadDevice(int dev, int len,
	byte *data)
{
}

BTEIFGL_API void GPIO_I2C_CmdWriteDevice(int dev, int len,
	byte *data)
{
}

#if 0
// BTEIFGL_API BGBRPI_IORequest *GPIO_I2C_CmdReadDeviceSched(int dev,
	int len, byte *data,
	void *usrdata,
	void (*done)(BGBRPI_IORequest *iorq, void *data, int status))
{
	BGBRPI_IORequest *tmp;

	tmp=BGBRPI_AllocIORquest();
	tmp->data=usrdata;
	tmp->Done=done;

	if(dev==BGBRPI_MPU9250)
	{
		memset(data, 0, len);
		data[0]=0;

		done(tmp, usrdata, BGBRPI_STATUS_OK);
	}

	BGBRPI_FreeIORquest(tmp);

	return(NULL);
}

// BTEIFGL_API BGBRPI_IORequest *GPIO_I2C_CmdReadDeviceBaseSched(int dev,
	int base, int len, byte *data,
	void *usrdata,
	void (*done)(BGBRPI_IORequest *iorq, void *data, int status))
{
	BGBRPI_IORequest *tmp;

	tmp=BGBRPI_AllocIORquest();
	tmp->data=usrdata;
	tmp->Done=done;

	if(dev==BGBRPI_MPU9250)
	{
		memset(data, 0, len);
		data[0]=0;

		done(tmp, usrdata, BGBRPI_STATUS_OK);
	}

	BGBRPI_FreeIORquest(tmp);

	return(NULL);
}
#endif

#endif