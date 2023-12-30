#include <3ds.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

	printf("Welcome to 3DS-EAS!");
	printf("\n\nA - Start alert fetching process");
    printf("\nSTART - Quit the application");

    while(aptMainLoop()) {   
        const char *state_codes[] = {"AL", "AK", "AS", "AR", "AZ", "CA", "CO", "CT", "DE", "DC", "FL", "GA", "GU", "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "PR", "RI", "SC", "SD", "TN", "TX", "UT", "VT", "VI", "VA", "WA", "WV", "WI", "WY", "MP", "PW", "FM", "MH"};

        CURL *curl;
        CURLcode res;

        size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
            size_t realsize = size * nmemb;
            printf("%.*s", (int)realsize, (char *)contents);
            return realsize;
        }

        gspWaitForVBlank();
        hidScanInput();

        if(hidKeysDown() & KEY_START) {
            curl_easy_cleanup(curl);
            break;
        }

        if(hidKeysDown() & KEY_A) {
            bool isInList(const char *strstr) {
                for (int i = 0; i < sizeof(state_codes) / sizeof(state_codes[0]); i++) {
                    if (strcmp(strstr, state_codes[i]) == 0) {
                        return true;
                    }
                }
            }

            swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 3, 2);
			swkbdSetHintText(&swkbd, "Please enter a state's abbreviation.");

            if(strlen(strstr) > 2) {
                consoleClear();
                printf("Please enter the state's ABBREVIATION, not the state name.");
                sleep(3);
                aptMainLoop();
            } else {
                if(!isInList(strstr)) {
                    consoleClear();
                    printf("Please enter a VALID state abbreviation.");
                    sleep(3);
                    aptMainLoop();
            } else {
                curl = curl_easy_init();

                if(curl) {
                    const char *url = strcat("https://api.weather.gov/alerts/active?area=", strstr);
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

                    res = curl_easy_perform(curl);

                    if(res != CURLE_OK) {
                        consoleClear();
                        fprintf(stderr, "ERROR: Request failed: %s\n", curl_easy_strerror(res));
                        sleep(3);
                        aptMainLoop();
                    }

                    curl_easy_cleanup(curl);
            }
        }
    }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
    }

    gfxExit();
    return 0;
}