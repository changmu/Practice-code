all: Sniffer

LINK = -lpcap

Sniffer: sniffer.c
	gcc $< -o $@ $(LINK)

.PHONY: clean
clean:
	-rm -f Sniffer
