#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("\x1b[9;20HWelcome to 3DS-EAS!");
	printf("\x1b[30;16H\nA - Begin alert fetching process");
    printf("\x1b[44;12H\nSTART - Quit the application");

	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) {
            break;
        } 

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
