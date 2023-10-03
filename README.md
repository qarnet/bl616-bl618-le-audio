# Disclaimer
This project is far from finished. Edit: Also in need of more research, if this chip is able to support LE Audio. Something about the I2S clock needing to be synchronized with another clock.

# Setup
## 1.
Follow the environment setup from https://github.com/bouffalolab/bouffalo_sdk.git

## 2.
```
git clone https://github.com/bouffalolab/bouffalo_sdk.git
cd bouffalo_sdk
edit project.build and change COM to com path (e.g. /dev/ttyUSB0)
mkdir projects
cd projects
git clone git@github.com:qarnet/bl616-le-audio.git
```

# Supported CHIP
BL616/BL618

# Compile
```
make CHIP=bl616 BOARD=bl616dk
```

# Flash
```
make flash CHIP=chip_name COMX=xxx # xxx is your com name
```
Hold boot button, then press reset and finally let go of boot button
