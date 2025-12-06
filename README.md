# settle the world - map editor

Simple map editor for the strategy game [Settle the World](https://theotheoderich.itch.io/settle-the-world).

## Copyright

The stw-map-editor and exdev-gfx library are copyright by Andre "eliot" Geisler (andre@exdev.de).  
The stw-map-editor uses assets from the game Settle the World.  
These assets are copyright by Christian "theotheoderich" Wiegel.  

## Checkout

```shell
    git clone git@github.com:eliot-exdev/stw-map-editor.git
    cd stw-map-editor
    git submodule update --init
```

## Build

```shell
    mkdir build
    cd build
    cmake ../ -DCMAKE_BUILD_TYPE=Release
    cmake --build . --parallel 4
```
