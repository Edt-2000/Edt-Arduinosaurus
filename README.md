# Ed-2000
Arduino &amp; GameTrak based expressive midi controller

>This is our playground for all our code and experiments, we are in the progress of making separate git repositories for released code. Have a look at the [Edt-2000 organisation](https://github.com/Edt-2000) for all the repositories.

## OSC lighting commands

start = 0 - 127 (0 = begin, 127 = end)
end = 0 - 127

### Single color, solid
/address 1 [start: int] [end: int] [h: int] [s: int] [l: int] [duration (only required with l == 0): int]

l == 0 dims using pulse

### Single color, pulse
/address 2 [start: int] [end: int] [h: int] [s: int] [l: int] [duration: int]

### Rainbow color, solid
/address 3 [start: int] [end: int] [h start: int] [delta h: int] [duration (only required with delta h == 0): int]

delta h == 0 dims using pulse
delta h == 127 = full rainbow
delta h == 255 = two full rainbows

### Rainbow color, pulse
/address 4 [start: int] [end: int] [h start: int] [delta h: int] [duration: int]

### VU Meter
/address 100 [start: int] [end: int] [center: int] [h start: int] [delta h: int] [intensity: int]

### Twinkle
/address 101 [start: int] [end: int] [h: int] [saturation: int]

### Strobo
/address 200 [h: int] [fps: int]

h == 0 yields white strobo
fps == 0 stops strobo


## License

Details about the licensing can be found in the LICENSE file.

* Please use this code for your own DIY projects
* When you make interesting changes and additions; send us a message and/or make a Pull Request
* Send us a link to your blog if you have one, we would love others to use our work to create cool live performances
