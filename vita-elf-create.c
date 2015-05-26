#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"

#define FSTUBS_SECTION ".vitalink.fstubs"
#define VSTUBS_SECTION ".vitalink.vstubs"

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


	// Load entire String Table Section
	char *shstrtab = elf_load_shstrtab(fp, &ehdr, shdr);
	int fstubs_found = 0;
	int vstubs_found = 0;
	int i;
	for (i = 0; i < ehdr.e_shnum; i++) {
		const char *shstr = shstrtab + shdr[i].sh_name;
		if (!fstubs_found) {
			if (strcmp(FSTUBS_SECTION, shstr) == 0) {
				fstubs_found = 1;
			}
		}
		if (!vstubs_found) {
			if (strcmp(VSTUBS_SECTION, shstr) == 0) {
				vstubs_found = 1;
			}
		}
	}

	if (!fstubs_found && !vstubs_found) {
		fprintf(stderr, "ERROR: the requiered sections "
			FSTUBS_SECTION " and " VSTUBS_SECTION
			" were not found.\n");
		goto exit_free_shstrtab;
	}

	if (!fstubs_found) {
		printf("WARNING: section: " FSTUBS_SECTION
			" was not found");
	}
	if (!vstubs_found) {
		printf("WARNING: section: " VSTUBS_SECTION
			" was not found");
	}


	free(shstrtab);

	elf_free_shdr(&shdr);
	elf_free_phdr(&phdr);
	fclose(fp);
	return EXIT_SUCCESS;

exit_free_shstrtab:
	free(shstrtab);
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
