# Ed-2000
Arduino &amp; GameTrak based expressive midi controller

>This is our playground for all our code and experiments, we are in the progress of making separate git repositories for released code. Have a look at the [Edt-2000 organisation](https://github.com/Edt-2000) for all the repositories.

## TODO:

- Edt-DOSCMX addresses to /DP and /DZ
- Watchdog on OSC slaves
- Disable stacking of animations in Dispedter

## Pinout

FastLED:

- VCC = BR = 1
- CI = GR = 2
- DI = OR = 3
- GR = GL = 4

RGBLED:

- + = BL = 1
- B = BR = 2
- R = OR = 3
- G = GR = 4

## OSC lighting commands

start = 0 - 127 (0 = begin, 127 = end)
end = 0 - 127

h, s, v = 0 - 255
duration, intensity, speed = 0 - 225


### Single color, solid
`/address 1 [start: int] [end: int] [h: int] [s: int] [v: int] [duration (required with l == 0): int]`

- `l == 0` dims using pulse

### Single color, pulse
`/address 2 [start: int] [end: int] [h: int] [s: int] [v: int] [duration: int]`

### Single color, spark
`/address 7 [start: int] [end: int] [h: int] [s: int] [v: int] [duration: int]`

### Dual color, solid
`/address 9 [start: int] [end: int] [h1: int] [h2: int] [duration (required with l == 0): int]`

- `l == 0` dims using pulse
- `s` and `v` are 255 by default

### Dual color, pulse
`/address 10 [start: int] [end: int] [h1: int] [h2: int] [duration: int]`

### Dual color, spark
`/address 11 [start: int] [end: int] [h1: int] [h2: int] [duration: int]`

### Rainbow color, solid
`/address 2 [start: int] [end: int] [h start: int] [delta h: int] [duration (required with delta h == 0): int]`

- `delta h == 0` dims using pulse
- `delta h == 127` = full rainbow
- `delta h == 255` = two full rainbows

### Rainbow color, pulse
`/address 2 [start: int] [end: int] [h start: int] [delta h: int] [duration: int]`

### Rainbow color, spark
`/address 8 [start: int] [end: int] [h start: int] [delta h: int] [duration: int]`

### VU Meter
`/address 4 [start: int] [end: int] [center: int] [h start: int] [delta h: int] [intensity: int]`

### Twinkle
`/address 5 [start: int] [end: int] [h: int] [intensity: int]`

### Strobo
`/address 6 [h: int] [fps: int]`

- `h == 255` yields white strobo
- `fps == 0` stops strobo

### Chase
`/address 12 [hue: int] [speed: int] [style: int]`

- `style == 0` default chase
- `style == 1` default reverse chase
- `style == 2` long tail chase
- `style == 3` long tail reverse chase

### Bash
`/address 13 [hue: int] [intensity: int]`

## License

Details about the licensing can be found in the LICENSE file.

- Please use this code for your own DIY projects
- When you make interesting changes and additions; send us a message and/or make a Pull Request
- Send us a link to your blog if you have one, we would love others to use our work to create cool live performances
