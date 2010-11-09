# Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

# Files from Libiberty not located in libiberty/.
EXTRA_DIST +=								\
  config.guess config.sub install-sh					\
									\
  config/acx.m4 config/warnings.m4					\
									\
  include/alloca-conf.h include/ansidecl.h include/aout/adobe.h		\
  include/aout/aout64.h include/aout/ar.h include/aout/ChangeLog	\
  include/aout/dynix3.h include/aout/encap.h include/aout/host.h	\
  include/aout/hp.h include/aout/hp300hpux.h include/aout/hppa.h	\
  include/aout/ranlib.h include/aout/reloc.h include/aout/stab.def	\
  include/aout/stab_gnu.h include/aout/sun4.h include/bfdlink.h		\
  include/binary-io.h include/bout.h include/cgen/basic-modes.h		\
  include/cgen/basic-ops.h include/cgen/bitset.h			\
  include/cgen/ChangeLog include/ChangeLog include/ChangeLog-9103	\
  include/coff/alpha.h include/coff/apollo.h include/coff/arm.h		\
  include/coff/aux-coff.h include/coff/ChangeLog			\
  include/coff/ChangeLog-9103 include/coff/ecoff.h			\
  include/coff/external.h include/coff/go32exe.h include/coff/h8300.h	\
  include/coff/h8500.h include/coff/i386.h include/coff/i860.h		\
  include/coff/i960.h include/coff/ia64.h include/coff/internal.h	\
  include/coff/m68k.h include/coff/m88k.h include/coff/mcore.h		\
  include/coff/mips.h include/coff/mipspe.h include/coff/or32.h		\
  include/coff/pe.h include/coff/powerpc.h include/coff/rs6000.h	\
  include/coff/rs6k64.h include/coff/sh.h include/coff/sparc.h		\
  include/coff/sym.h include/coff/symconst.h include/coff/ti.h		\
  include/coff/tic30.h include/coff/tic4x.h include/coff/tic54x.h	\
  include/coff/tic80.h include/coff/w65.h include/coff/we32k.h		\
  include/coff/x86_64.h include/coff/xcoff.h include/coff/z80.h		\
  include/coff/z8k.h include/COPYING include/COPYING3			\
  include/demangle.h include/dis-asm.h include/dwarf2.h			\
  include/dyn-string.h include/elf/alpha.h include/elf/arc.h		\
  include/elf/arm.h include/elf/avr.h include/elf/bfin.h		\
  include/elf/ChangeLog include/elf/ChangeLog-9103			\
  include/elf/common.h include/elf/cr16.h include/elf/cr16c.h		\
  include/elf/cris.h include/elf/crx.h include/elf/d10v.h		\
  include/elf/d30v.h include/elf/dlx.h include/elf/dwarf.h		\
  include/elf/external.h include/elf/fr30.h include/elf/frv.h		\
  include/elf/h8.h include/elf/hppa.h include/elf/i370.h		\
  include/elf/i386.h include/elf/i860.h include/elf/i960.h		\
  include/elf/ia64.h include/elf/internal.h include/elf/ip2k.h		\
  include/elf/iq2000.h include/elf/lm32.h include/elf/m32c.h		\
  include/elf/m32r.h include/elf/m68hc11.h include/elf/m68k.h		\
  include/elf/mcore.h include/elf/mep.h include/elf/microblaze.h	\
  include/elf/mips.h include/elf/mmix.h include/elf/mn10200.h		\
  include/elf/mn10300.h include/elf/moxie.h include/elf/msp430.h	\
  include/elf/mt.h include/elf/openrisc.h include/elf/or32.h		\
  include/elf/pj.h include/elf/ppc.h include/elf/ppc64.h		\
  include/elf/reloc-macros.h include/elf/rx.h include/elf/s390.h	\
  include/elf/score.h include/elf/sh.h include/elf/sparc.h		\
  include/elf/spu.h include/elf/tic6x-attrs.h include/elf/tic6x.h	\
  include/elf/v850.h include/elf/vax.h include/elf/vxworks.h		\
  include/elf/x86-64.h include/elf/xc16x.h include/elf/xstormy16.h	\
  include/elf/xtensa.h include/fibheap.h include/filenames.h		\
  include/floatformat.h include/fnmatch.h include/fopen-bin.h		\
  include/fopen-same.h include/fopen-vms.h include/gdb/callback.h	\
  include/gdb/ChangeLog include/gdb/fileio.h include/gdb/remote-sim.h	\
  include/gdb/signals.def include/gdb/signals.h include/gdb/sim-arm.h	\
  include/gdb/sim-cr16.h include/gdb/sim-d10v.h include/gdb/sim-frv.h	\
  include/gdb/sim-h8300.h include/gdb/sim-lm32.h			\
  include/gdb/sim-m32c.h include/gdb/sim-ppc.h include/gdb/sim-rx.h	\
  include/gdb/sim-sh.h include/gdbm.h include/getopt.h			\
  include/hashtab.h include/hp-symtab.h include/ieee.h			\
  include/libiberty.h include/lto-symtab.h include/MAINTAINERS		\
  include/md5.h include/nlm/alpha-ext.h include/nlm/ChangeLog		\
  include/nlm/common.h include/nlm/external.h include/nlm/i386-ext.h	\
  include/nlm/internal.h include/nlm/ppc-ext.h				\
  include/nlm/sparc32-ext.h include/oasys.h include/objalloc.h		\
  include/obstack.h include/opcode/alpha.h include/opcode/arc.h		\
  include/opcode/arm.h include/opcode/avr.h include/opcode/bfin.h	\
  include/opcode/cgen.h include/opcode/ChangeLog			\
  include/opcode/ChangeLog-9103 include/opcode/convex.h			\
  include/opcode/cr16.h include/opcode/cris.h include/opcode/crx.h	\
  include/opcode/d10v.h include/opcode/d30v.h include/opcode/dlx.h	\
  include/opcode/h8300.h include/opcode/hppa.h include/opcode/i370.h	\
  include/opcode/i386.h include/opcode/i860.h include/opcode/i960.h	\
  include/opcode/ia64.h include/opcode/m68hc11.h include/opcode/m68k.h	\
  include/opcode/m88k.h include/opcode/mips.h include/opcode/mmix.h	\
  include/opcode/mn10200.h include/opcode/mn10300.h			\
  include/opcode/moxie.h include/opcode/msp430.h include/opcode/np1.h	\
  include/opcode/ns32k.h include/opcode/or32.h include/opcode/pdp11.h	\
  include/opcode/pj.h include/opcode/pn.h include/opcode/ppc.h		\
  include/opcode/pyr.h include/opcode/rx.h include/opcode/s390.h	\
  include/opcode/score-datadep.h include/opcode/score-inst.h		\
  include/opcode/sparc.h include/opcode/spu-insns.h			\
  include/opcode/spu.h include/opcode/tahoe.h include/opcode/tic30.h	\
  include/opcode/tic4x.h include/opcode/tic54x.h			\
  include/opcode/tic6x-control-registers.h				\
  include/opcode/tic6x-insn-formats.h					\
  include/opcode/tic6x-opcode-table.h include/opcode/tic6x.h		\
  include/opcode/tic80.h include/opcode/v850.h include/opcode/vax.h	\
  include/os9k.h include/partition.h include/plugin-api.h		\
  include/progress.h include/safe-ctype.h include/sha1.h		\
  include/som/aout.h include/som/ChangeLog include/som/clock.h		\
  include/som/internal.h include/som/lst.h include/som/reloc.h		\
  include/sort.h include/splay-tree.h include/symcat.h			\
  include/vms/ChangeLog include/vms/dcx.h include/vms/dmt.h		\
  include/vms/dsc.h include/vms/dst.h include/vms/eeom.h		\
  include/vms/egps.h include/vms/egsd.h include/vms/egst.h		\
  include/vms/egsy.h include/vms/eiaf.h include/vms/eicp.h		\
  include/vms/eidc.h include/vms/eiha.h include/vms/eihd.h		\
  include/vms/eihi.h include/vms/eihs.h include/vms/eihvn.h		\
  include/vms/eisd.h include/vms/emh.h include/vms/eobjrec.h		\
  include/vms/esdf.h include/vms/esdfm.h include/vms/esdfv.h		\
  include/vms/esgps.h include/vms/esrf.h include/vms/etir.h		\
  include/vms/internal.h include/vms/lbr.h include/vms/prt.h		\
  include/vms/shl.h include/xregex.h include/xregex2.h			\
  include/xtensa-config.h include/xtensa-isa-internal.h			\
  include/xtensa-isa.h

# These scripts are required by Libiberty's build system.
EXTRA_DIST += mkinstalldirs move-if-change
