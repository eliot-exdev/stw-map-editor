#--- MorphOs ---#
CC_GCC=ppc-morphos-gcc-11
C_FLAGS_MOS_GCC=-Ofast -noixemul -mcpu=G4 -maltivec -mabi=altivec -DNDEBUG -D__MORPHOS__
LD_FLAGS_MOS_GCC=-lm

INCLUDES_MOS=-Iinclude -Iexdev-gfx/library/include

#--- AmigaOs 060 ---#
CC=vc
C_FLAGS_060=+aos68k -c99 -fpu=68060 -cpu=68060 -speed -final -DNDEBUG -D__AMIGA__
LD_FLAGS_060=-lm060

#--- AmigaOs 030 fpu ---#
C_FLAGS_030_FPU=+aos68k -c99 -fpu=68881 -cpu=68030 -speed -final -DNDEBUG -D__AMIGA__
LD_FLAGS_030_FPU=-lm881

INCLUDES_AOS=-Iinclude -Iexdev-gfx/library/include

#--- Libraries MorphOs ---#
exdev-gfx/exdev_gfx_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_mos_gcc.a

exdev-gfx/exdev_gfx_ui_mos_gcc.a:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_mos_gcc.a

#--- Libraries AmigaOs ---#
exdev-gfx/exdev_gfx_aos_060.lib:
	$(MAKE) -C exdev-gfx exdev_gfx_aos_060.lib

exdev-gfx/exdev_gfx_aos_060_c2p.lib:
	$(MAKE) -C exdev-gfx exdev_gfx_aos_060_c2p.lib

exdev-gfx/exdev_gfx_aos_030_fpu_c2p.lib:
	$(MAKE) -C exdev-gfx exdev_gfx_aos_030_fpu_c2p.lib

exdev-gfx/exdev_gfx_ui_aos_060.lib:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_aos_060.lib

exdev-gfx/exdev_gfx_ui_aos_030_fpu.lib:
	$(MAKE) -C exdev-gfx exdev_gfx_ui_aos_030_fpu.lib

#--- Editor ---#
stw_map_editor_mos_gcc: src/main.c src/ui_map.c src/ui_tile.c src/settle_the_world_util.c src/ui_status.c exdev-gfx/exdev_gfx_ui_mos_gcc.a exdev-gfx/exdev_gfx_mos_gcc.a
	$(CC_GCC) -o ${@} ${INCLUDES_MOS} $(^) ${C_FLAGS_MOS_GCC} ${LD_FLAGS_MOS_GCC}

stw_map_editor_aos_060: src/main.c src/ui_map.c src/ui_tile.c src/settle_the_world_util.c src/ui_status.c exdev-gfx/exdev_gfx_ui_aos_060.lib exdev-gfx/exdev_gfx_aos_060.lib
	$(CC) -o ${@} ${INCLUDES_AOS} $(^) ${C_FLAGS_060} ${LD_FLAGS_060}

stw_map_editor_aos_060_c2p: src/main.c src/ui_map.c src/ui_tile.c src/settle_the_world_util.c src/ui_status.c exdev-gfx/exdev_gfx_ui_aos_060.lib exdev-gfx/exdev_gfx_aos_060_c2p.lib
	$(CC) -o ${@} ${INCLUDES_AOS} $(^) ${C_FLAGS_060} -DLOW_RESOLUTION ${LD_FLAGS_060} -LWork:workspace/c2plib/sdk -lc2p

stw_map_editor_aos_030_fpu_c2p: src/main.c src/ui_map.c src/ui_tile.c src/settle_the_world_util.c src/ui_status.c exdev-gfx/exdev_gfx_ui_aos_060.lib exdev-gfx/exdev_gfx_aos_060_c2p.lib
	$(CC) -o ${@} ${INCLUDES_AOS} $(^) ${C_FLAGS_030_FPU} -DLOW_RESOLUTION ${LD_FLAGS_030_FPU} -LWork:workspace/c2plib/sdk -lc2p

stw_map_editor: stw_map_editor_mos_gcc stw_map_editor_aos_060 stw_map_editor_aos_060_c2p

all: stw_map_editor

#--- dist ---#
dist: stw_map_editor
	$(RM) -rf ram:stw-map-editor
	$(RM) -f ram:stw-map-editor.lha
	mkdir ram:stw-map-editor
	mkdir ram:stw-map-editor/assets
	mkdir ram:stw-map-editor/maps
	cp -av assets/tiles_8bit.dat assets/tiles_8bit.pal ram:stw-map-editor/assets/
	cp -av stw_map_editor_mos_gcc stw_map_editor_aos_060 stw_map_editor_aos_060_c2p ram:stw-map-editor/
	cp maps/*.map ram:stw-map-editor/maps/
	cp dist/stw_map_editor.readme ram:stw-map-editor/
	lha a -r ram:stw-map-editor.lha ram:stw-map-editor

#--- clean ---#
.PHONY: clean
clean:
	$(MAKE) -C exdev-gfx clean
	$(RM) -f stw_map_editor_mos_gcc stw_map_editor_aos_060 stw_map_editor_aos_060_c2p
