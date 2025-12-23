#--- MorphOs ---#
CC_GCC=ppc-morphos-gcc-11
C_FLAGS_MOS_GCC=-Ofast -noixemul -mcpu=G4 -maltivec -mabi=altivec -DNDEBUG -D__MORPHOS__
LD_FLAGS_MOS_GCC=-lm
INCLUDES_MOS=-Iinclude -Iexdev-gfx/library/include

#--- AmigaOs ---#
CC=vc
C_FLAGS_060=+aos68k -c99 -fpu=68060 -cpu=68060 -speed -final -DNDEBUG -D__AMIGA__
LD_FLAGS_060=-lm060
INCLUDES_AOS=-Ilibrary/include -Ilibrary/easing/include

#--- Libraries ---#
exdev-gfx/exdev_gfx_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_mos_gcc.a

exdev-gfx/exdev_gfx_ui_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_mos_gcc.a

exdev-gfx/exdev_gfx_aos_060.a:
	$(MAKE) -C exdev-gfx exdev_gfx_aos_060.a

exdev-gfx/exdev_gfx_aos_060_c2p.a:
	$(MAKE) -C exdev-gfx exdev_gfx_aos_060_c2p.a

exdev-gfx/exdev_gfx_ui_aos_060.a:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_aos_060.a

EXDEV_GFX_AOS_OBJECTS=exdev-gfx/library/src/args.o exdev-gfx/library/src/color.o exdev-gfx/library/src/events.o exdev-gfx/library/src/font.o exdev-gfx/library/src/framebuffer.o exdev-gfx/library/src/framebuffer_8bit.o exdev-gfx/library/src/framebuffer_rgba.o exdev-gfx/library/src/heightmap.o exdev-gfx/library/src/helper.o exdev-gfx/library/src/julia.o exdev-gfx/library/src/matrix.o exdev-gfx/library/src/palette.o exdev-gfx/library/src/vertex2d.o exdev-gfx/library/src/vertex3d.o exdev-gfx/library/src/voxelspace.o exdev-gfx/library/src_amiga/exdev_base_amiga.o exdev-gfx/library/src_amiga/helper_amiga.o exdev-gfx/library/src_amiga/window_amiga.o
EXDEV_GFX_UI_AOS_OBJECTS=exdev-gfx/library/src/ui/ui_application.o exdev-gfx/library/src/ui/ui_component.o exdev-gfx/library/src/ui/ui_component_list.o exdev-gfx/library/src/ui/ui_scroll.o exdev-gfx/library/src/ui/ui_horizontal_scroll_bar.o exdev-gfx/library/src/ui/ui_vertical_scroll_bar.o exdev-gfx/library/src/ui/ui_icon.o

#--- Editor ---#
stw_map_editor_mos_gcc: src/main.c exdev-gfx/exdev_gfx_ui_mos_gcc.a exdev-gfx/exdev_gfx_mos_gcc.a
	$(CC_GCC) -o ${@} ${INCLUDES_MOS} $(^) ${C_FLAGS_MOS_GCC} ${LD_FLAGS_MOS_GCC}

stw_map_editor_aos_060: src/main.c exdev-gfx/exdev_gfx_ui_aos_060.a exdev-gfx/exdev_gfx_aos_060.a
	$(CC) -o ${@} ${INCLUDES_MOS} src/main.c ${EXDEV_GFX_AOS_OBJECTS} ${EXDEV_GFX_UI_AOS_OBJECTS} ${C_FLAGS_060} ${LD_FLAGS_060}

stw_map_editor_aos_060_c2p: src/main.c exdev-gfx/exdev_gfx_ui_aos_060.a exdev-gfx/exdev_gfx_aos_060_c2p.a
	$(CC) -o ${@} ${INCLUDES_MOS} src/main.c ${EXDEV_GFX_AOS_OBJECTS} ${EXDEV_GFX_UI_AOS_OBJECTS} ${C_FLAGS_060} ${LD_FLAGS_060}

stw_map_editor: stw_map_editor_mos_gcc stw_map_editor_aos_060

all: stw_map_editor

#--- clean ---#
.PHONY: clean
clean:
	$(MAKE) -C exdev-gfx clean
	$(RM) -f stw_map_editor_mos_gcc
