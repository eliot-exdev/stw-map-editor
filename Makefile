#--- MorphOs ---#
CC_GCC=ppc-morphos-gcc-11
C_FLAGS_MOS_GCC=-Ofast -noixemul -mcpu=G4 -maltivec -mabi=altivec -DNDEBUG -D__MORPHOS__
LD_FLAGS_MOS_GCC=-lm
INCLUDES_MOS=-Iinclude -Iexdev-gfx/library/include

exdev-gfx/exdev_gfx_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_mos_gcc.a

exdev-gfx/exdev_gfx_ui_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_mos_gcc.a

stw_map_editor_mos_gcc: src/main.c exdev-gfx/exdev_gfx_ui_mos_gcc.a exdev-gfx/exdev_gfx_mos_gcc.a
	$(CC_GCC) -o ${@} ${INCLUDES_MOS} $(^) ${C_FLAGS_MOS_GCC} ${LD_FLAGS_MOS_GCC}

stw_map_editor: stw_map_editor_mos_gcc

all: stw_map_editor

#--- clean ---#
.PHONY: clean
clean:
	$(MAKE) -C exdev-gfx clean
	$(RM) -f stw_map_editor_mos
