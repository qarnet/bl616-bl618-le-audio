SDK_DEMO_PATH ?= .
BL_SDK_BASE ?= $(SDK_DEMO_PATH)/../..

export BL_SDK_BASE

CHIP ?= bl616
BOARD ?= bl616dk
CROSS_COMPILE ?= riscv64-unknown-elf-

# add custom cmake definition
#cmake_definition+=-Dxxx=sss
#cmake_definition+=-DCMAKE_EXPORT_COMPILE_COMMANDS=1

include $(BL_SDK_BASE)/project.build
