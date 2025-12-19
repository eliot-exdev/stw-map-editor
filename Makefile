#--- MorphOs ---#
CC_GCC=ppc-morphos-gcc-11
C_FLAGS_MOS_GCC=-Ofast -noixemul -mcpu=G4 -maltivec -mabi=altivec -DNDEBUG -D__MORPHOS__
LD_FLAGS_MOS_GCC=-lm -Lexdev-gfx
INCLUDES_MOS=-Iinclude -Iexdev-gfx/library/easing/include

exdev_gfx_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_mos_gcc.a

exdev_gfx_ui_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_mos_gcc.a

stw_map_editor: stw_map_editor_mos

stw_map_editor_mos_gcc: src/main.c exdev_gfx_ui_mos_gcc.a exdev_gfx_mos_gcc.a
	$(CC_GCC) -o ${@} ${INCLUDES_MOS} $(^) ${C_FLAGS_MOS_GCC} ${LD_FLAGS_MOS_GCC}

#--- clean ---#
.PHONY: clean
clean:
	$(RM) -f stw_map_editor_mos
