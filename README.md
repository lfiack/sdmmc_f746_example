# How to use the SD Card with STM32F746G-Disco

1. Create project 
    * Board selector
    * No _Default Mode_
    * No MPU
2. Check for uSD_Detect (PC13, GPIO Input)
3. Activate SDMMC1 in Connectivity (Can be SDIO on some STM32)
    * SD 4 bits Wide Bus
    * SDMMCCLK clock divide factor 0 -> Let's check the clock configuration !
4. Activate HSE
    * Crystal/Ceramic Resonator
5. In Clock Configuration
    * SDMMC to 48MHz
6. Back in SDMMC1: 
    * NVIC Settings -> Enable interrupt
    * DMA Settings -> Add RX & TX
7. Hardware is configured, let's configure software
8. Activate the OS (should work without, but heh)
    * Middleware -> FreeRTOS, Interface CMSIS_V2
    * Default task stack -> 1024
    * Don't touch the rest for now
9. Activate FatFS
    * Select SD Card
    * In advanced settings: Check if DMA template is enabled
    * In platform settings : Select Detect_SDIO
10. Generate the code. Solve the FREERTOS warnings
11. Maybe test the printf and stuff
12. In the default task, you can test the following functions :

```C
f_mount(&fs, "", 1);    // Necessary before everything else

FRESULT sd_ls(const char *path)
{
	FRESULT res;
	DIR dir;
	FILINFO fno;

	res = f_opendir(&dir, path);  // Ouvre le répertoire
	if (res == FR_OK)
	{
		while (1)
		{
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
			{
				break; // Fin
			}
			printf("%s%s\r\n", fno.fname, (fno.fattrib & AM_DIR) ? "/" : "");
		}
		return f_closedir(&dir);
	}

	return res;
}

f_open(&f, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
f_write(&f, str, strlen(str), &bw);
f_close(&f);
```