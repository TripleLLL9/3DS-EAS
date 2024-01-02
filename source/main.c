#include <3ds.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

	printf("Welcome to 3DS-EAS!");
	printf("\n\nA - Start alert fetching process");
    printf("\nSTART - Quit the application");

    curl_global_init(CURL_GLOBAL_DEFAULT);

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
            curl_global_cleanup();
            curl_easy_cleanup(curl);
            break;
        }

        if(hidKeysDown() & KEY_A) {
            bool isInList(const char *mybuf) {
                for (int i = 0; i < sizeof(state_codes) / sizeof(state_codes[0]); i++) {
                    if (strcmp(mybuf, state_codes[i]) == 0) {
                        return true;
                    }
                }

                return false;
            }

            static SwkbdState swkbd;
            static char mybuf[60];
            swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 3, -1);
            swkbdSetInitialText(&swkbd, mybuf);
            swkbdSetHintText(&swkbd, "Please enter a state's abbreviation.");

            SwkbdButton button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));

            if(strlen(mybuf) > 2) {
                consoleClear();
                printf("Please enter the state's ABBREVIATION, not the state name.");
                sleep(3);
                aptMainLoop();
            } else {
                if(!isInList(mybuf)) {
                    consoleClear();
                    printf("Please enter a VALID state abbreviation.");
                    sleep(3);
                    aptMainLoop();
            } else {              
                // printf(system("curl -X GET https://api.weather.gov/alerts/active?state=CA -H 'User-Agent: curl'"));
                consoleClear();
                
                CURL *curl;
                CURLcode res;

                curl = curl_easy_init();
                if (curl) {
                    const char *url = "https://api.weather.gov/alerts/active?area=CA"; // placeholder url for when i actually get curl working

                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

                    struct curl_slist *headers = NULL;
                    headers = curl_slist_append(headers, "User-Agent: curl");
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK) {
                        fprintf(stderr, "\nERROR: curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    }

                    curl_slist_free_all(headers);
                    curl_easy_cleanup(curl);
                }
            }
        }

    gfxFlushBuffers();
    gfxSwapBuffers();

    }
}

    gfxExit();
    return 0;

}