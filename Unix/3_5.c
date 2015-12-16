#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

const int BUFFSIZE = 4096;

int main() {
	int 	n;
	char 	buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			printf("write error!\n");

	if (n < 0) printf("read error!\n");

	exit(0);
}
