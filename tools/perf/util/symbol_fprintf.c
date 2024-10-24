// SPDX-License-Identifier: GPL-2.0
#include <elf.h>
#include <inttypes.h>
#include <stdio.h>

#include "dso.h"
#include "map.h"
#include "symbol.h"
#include "srcline.h"

size_t symbol__fprintf(struct symbol *sym, FILE *fp)
{
	return fprintf(fp, " %" PRIx64 "-%" PRIx64 " %c %s\n",
		       sym->start, sym->end,
		       sym->binding == STB_GLOBAL ? 'g' :
		       sym->binding == STB_LOCAL  ? 'l' : 'w',
		       sym->name);
}

size_t __symbol__fprintf_sym_offs(const struct symbol *sym,
				  const struct addr_location *al,
				  bool unknown_as_addr, bool print_offsets,
				  bool print_line, FILE *fp)
{
	unsigned long offset;
	size_t length;

	if (sym) {
		length = fprintf(fp, "%s", sym->name);
		if (al && (print_offsets || print_line)) {
			if (print_line) {
				int ret = 0;
				char *srcline = map__srcline(al->map, al->addr, NULL);
				if (srcline != SRCLINE_UNKNOWN) {
					ret = fprintf(fp, "+%s", srcline);
					if (ret > 0)
						length += (size_t)ret;
				}
				zfree_srcline(&srcline);
				srcline = map__srcline(al->map, sym->start, NULL);
				if (srcline != SRCLINE_UNKNOWN) {
					ret = fprintf(fp, "+%s", srcline);
					if (ret > 0)
						length += (size_t)ret;
				}
				zfree_srcline(&srcline);
			} else {
				if (al->addr < sym->end)
					offset = al->addr - sym->start;
				else
					offset = al->addr - map__start(al->map) - sym->start;
				length += fprintf(fp, "+0x%lx", offset);
			}
		}
		return length;
	} else if (al && unknown_as_addr)
		return fprintf(fp, "[%#" PRIx64 "]", al->addr);
	else
		return fprintf(fp, "[unknown]");
}

size_t __symbol__fprintf_symname_offs(const struct symbol *sym,
				      const struct addr_location *al,
				      bool unknown_as_addr, bool print_offsets,
				      FILE *fp)
{
	return __symbol__fprintf_sym_offs(sym, al, unknown_as_addr,
					  print_offsets, false, fp);
}

size_t symbol__fprintf_symname_offs(const struct symbol *sym,
				    const struct addr_location *al,
				    FILE *fp)
{
	return __symbol__fprintf_symname_offs(sym, al, false, true, fp);
}

size_t __symbol__fprintf_symname(const struct symbol *sym,
				 const struct addr_location *al,
				 bool unknown_as_addr, FILE *fp)
{
	return __symbol__fprintf_symname_offs(sym, al, unknown_as_addr, false, fp);
}

size_t symbol__fprintf_symname(const struct symbol *sym, FILE *fp)
{
	return __symbol__fprintf_symname_offs(sym, NULL, false, false, fp);
}

size_t dso__fprintf_symbols_by_name(struct dso *dso,
				    FILE *fp)
{
	size_t ret = 0;

	for (size_t i = 0; i < dso->symbol_names_len; i++) {
		struct symbol *pos = dso->symbol_names[i];

		ret += fprintf(fp, "%s\n", pos->name);
	}
	return ret;
}

size_t symbol__fprintf_symline_offs(const struct symbol *sym,
				    const struct addr_location *al, FILE *fp)
{
	return __symbol__fprintf_sym_offs(sym, al, false, false, true, fp);
}