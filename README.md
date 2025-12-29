# settle the world - map editor

Simple map editor for the Amiga strategy game [Settle the World](https://theotheoderich.itch.io/settle-the-world).

## Copyright notice

The stw-map-editor and exdev-gfx library are copyright by Andre "eliot" Geisler (andre@exdev.de).  
The stw-map-editor uses assets from the game Settle the World.  
These assets are copyright by Christian "theotheoderich" Wiegel.  

## Goals

- Implement a map editor for "Settle the World"
- Support load, modify and store maps 
- Support different target platforms
  - AmigaOs 3.x (68k, RTG and AGA)
  - MorphOs 3.x (ppc)
  - MacOs X (arm64)
  - Ubuntu Linux (amd64, arm64)
- Implement in C99
- Few dependencies as possible

## Checkout

```shell
    git clone git@github.com:eliot-exdev/stw-map-editor.git
    cd stw-map-editor
    git submodule update --init
```

## Dependencies

- [vbcc](http://sun.hasenbraten.de/vbcc) and NDK (for AmigaOs build)
- gcc and SDK (for MorphOs build)
- [c2p library](https://aminet.net/package/dev/misc/c2plib) (for AGA)

## Build

### Compile for MacOs X and Linux
```shell
    mkdir build
    cd build
    cmake ../ -DCMAKE_BUILD_TYPE=Release
    cmake --build . --parallel 4
```

### Cross compile for MorphOs on linux
```shell
    mkdir build-mos
    cd build-mos
    cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../exdev-gfx/cmake/morphos-ppc.toolchain
    cmake --build . --parallel 4
```
