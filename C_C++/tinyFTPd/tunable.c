#include "tunable.h"

int tunable_pasv_enable = 1;
int tunable_port_enable = 1;
unsigned tunable_listen_port = 8086;
unsigned tunable_max_clients = 2000;
unsigned tunable_max_per_ip = 50;
unsigned tunable_accept_timeout = 60;
unsigned tunable_connect_timeout = 60;
unsigned tunable_idle_session_timeout = 300;
unsigned tunable_data_connection_timeout = 300;
unsigned tunable_local_umask = 077;
unsigned tunable_upload_max_rate = 0;
unsigned tunable_download_max_rate = 0;
const char *tunable_listen_addr = NULL;