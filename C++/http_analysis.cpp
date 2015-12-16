#include <cstdio>
#include <cstdlib>
#include <map>
#include <cstring>
#include <string>
#include <cstddef>

using namespace std;

const char buff[] = "POST /userloginex.php?action=login&cid=0&notice=0 HTTP/1.1\r\nHost: acm.hdu.edu.cn\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:39.0) Gecko/20100101 Firefox/39.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: http://acm.hdu.edu.cn/userloginex.php?action=login\r\nCookie: CNZZDATA1254072405=1655252587-1437546803-%7C1437959767; PHPSESSID=9tr1jmqbcrc3etacct1lqpvju7\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 45\r\n\r\n";

map<string, string> http_analysis(const char *buf, size_t length)
{
    size_t i = 0, j = 0, skip_n;
    char *str = (char *) malloc(length + 1);
    map<string, string> mp;
    string key;
    static const char *request_line[] = {
        "Method",
        "URL",
        "Version"
    };

    /*
     * Extract the request line
     * */
    for (int cnt = 0; cnt < 3; ++cnt) {
        sscanf(buf + i, "%s%n", str, &skip_n);
        i += skip_n;
        mp[request_line[cnt]] = str;
    }
    i += 2; // Skip "\r\n"

    /*
     * Time to handle header lines
     * */
    while (buf[i] != '\r') {
        j = 0;
        while (buf[i] != '\r') {
            if (buf[i] == ':') {
                str[j] = '\0';
                key = string(str);
                j = 0;
                i += 2; // Skip ": "
            } else {
                str[j++] = buf[i++];
            }
        }
        str[j] = '\0';
        i += 2;     // Skip "\r\n"
        mp[key] = str;
    }

    free(str);

    return mp;
}

int main(int argc, char **argv)
{
    map<string, string> mp = http_analysis(buff, sizeof(buff) / sizeof(*buff));
    map<string, string>::iterator it;
    for (it = mp.begin(); it != mp.end(); ++it) {
        printf("%s: %s\n", it->first.c_str(), it->second.c_str());
    }
    return 0;
}
