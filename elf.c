#include "elf.h"

int elf_read_ehdr(FILE *fp, Elf32_Ehdr *ehdr)
{
	fseek(fp, 0, SEEK_SET);
	size_t n = fread(ehdr, 1, sizeof(*ehdr), fp);

	if (n != sizeof(*ehdr)) {
		fprintf(stderr, "Could not read from file\n");
		return 0;
	}

	if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
	    ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
	    ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
	    ehdr->e_ident[EI_MAG3] != ELFMAG3) {
		fprintf(stderr, "Not an ELF file\n");
		return 0;
	}

	if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
		fprintf(stderr, "Not a 32 bit ELF\n");
		return 0;
	}

	if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
		fprintf(stderr, "Not a little endian ELF\n");
		return 0;
	}

	if (ehdr->e_type != ET_EXEC) {
		fprintf(stderr, "Not an executable ELF\n");
		return 0;
	}

	if (ehdr->e_machine != EM_ARM) {
		fprintf(stderr, "Not an ARM ELF\n");
		return 0;
	}

	return 1;
}
