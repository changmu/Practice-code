#include "myLinux.h"
#include <curl/curl.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <url>\n", basename(argv[0]));
        return 0;
    }

    CURLcode ret;
    CURL *curl = curl_easy_init();
    assert(curl != NULL);

    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return 0;
}
