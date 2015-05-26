#include <stdio.h>
#include <stdlib.h>
#include "elf.h"

void usage();

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage();
		goto exit_failure;
	}

	FILE *fp = fopen(argv[1], "rb");

	Elf32_Ehdr ehdr;
	if (!elf_read_ehdr(fp, &ehdr)) {
		fprintf(stderr, "ERROR: loading ELF\n");
		goto exit_close;
	}



	fclose(fp);
	return EXIT_SUCCESS;
exit_close:
	fclose(fp);
exit_failure:
	return EXIT_FAILURE;
}


void usage()
{
	fprintf(stderr,
		"vita-elf-create by xerpi\n"
		"usage:\n\tvita-elf-create in.elf\n"
	);
}
