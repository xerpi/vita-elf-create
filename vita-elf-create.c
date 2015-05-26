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

	// elf_print_ehdr(&ehdr);

	Elf32_Phdr *phdr;
	if (!elf_load_phdr(fp, &ehdr, &phdr)) {
		fprintf(stderr, "ERROR: loading ELF Program Header\n");
		goto exit_close;
	}

	Elf32_Shdr *shdr;
	if (!elf_load_shdr(fp, &ehdr, &shdr)) {
		fprintf(stderr, "ERROR: loading ELF Section Header\n");
		goto exit_free_phdr;
	}


	// Print sections
	// Load entire String Table Section
	char *shstrtab = elf_load_shstrtab(fp, &ehdr, shdr);
	int i;
	for (i = 0; i < ehdr.e_shnum; i++) {
		printf("Section %i: %s\n", i, (shstrtab + shdr[i].sh_name));
	}


	free(shstrtab);

	elf_free_shdr(&shdr);
	elf_free_phdr(&phdr);
	fclose(fp);
	return EXIT_SUCCESS;

exit_free_shdr:
	elf_free_shdr(&shdr);
exit_free_phdr:
	elf_free_phdr(&phdr);
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
