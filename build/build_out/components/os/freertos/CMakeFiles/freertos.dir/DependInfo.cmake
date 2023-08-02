
# Consider dependencies only in project.
set(CMAKE_DEPENDS_IN_PROJECT_ONLY OFF)

# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/portable/GCC/RISC-V/common/portASM.S" "/home/ubuntuvm/Programs/bouffalo_sdk/projects/ble-audio/build/build_out/components/os/freertos/CMakeFiles/freertos.dir/portable/GCC/RISC-V/common/portASM.S.o"
  )
set(CMAKE_ASM_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_ASM
  "ARCH_RISCV"
  "BFLB_BLE"
  "BFLB_TIMESTAMP_TIMEZONE=8"
  "BFLB_USE_HAL_DRIVER"
  "BFLB_USE_ROM_DRIVER"
  "BL616"
  "CFG_BLE"
  "CFG_BLE_ENABLE"
  "CFG_BLE_STACK_DBG_PRINT"
  "CFG_BLE_TX_BUFF_DATA=2"
  "CFG_BT_RESET"
  "CFG_CON=1"
  "CFG_FREERTOS"
  "CONFIG_BLE_TP_SERVER"
  "CONFIG_BLE_TP_TEST"
  "CONFIG_BT_ASSERT"
  "CONFIG_BT_BONDABLE"
  "CONFIG_BT_BROADCASTER"
  "CONFIG_BT_CONN"
  "CONFIG_BT_GAP_PERIPHERAL_PREF_PARAMS"
  "CONFIG_BT_GATT_CLIENT"
  "CONFIG_BT_GATT_DIS_FW_REV"
  "CONFIG_BT_GATT_DIS_HW_REV"
  "CONFIG_BT_GATT_DIS_PNP"
  "CONFIG_BT_GATT_DIS_SERIAL_NUMBER"
  "CONFIG_BT_GATT_DIS_SW_REV"
  "CONFIG_BT_GATT_DYNAMIC_DB"
  "CONFIG_BT_GATT_SERVICE_CHANGED"
  "CONFIG_BT_HCI_VS_EVT_USER"
  "CONFIG_BT_KEYS_OVERWRITE_OLDEST"
  "CONFIG_BT_KEYS_SAVE_AGING_COUNTER_ON_PAIRING"
  "CONFIG_BT_PERIPHERAL"
  "CONFIG_BT_SETTINGS_CCC_LAZY_LOADING"
  "CONFIG_BT_SETTINGS_USE_PRINTK"
  "CONFIG_BT_SIGNING"
  "CONFIG_BT_SMP"
  "CONFIG_FREERTOS"
  "CONFIG_IRQ_NUM=80"
  "CONFIG_LOG_LEVEL=3"
  "CONFIG_SHELL"
  "CONFIG_TLSF"
  "RFPARAM_BL616"
  "WL_BL616=1"
  "WL_BL618M=0"
  "WL_WB03=0"
  "configSTACK_ALLOCATION_FROM_SEPARATE_HEAP=1"
  "default_interrupt_handler=freertos_risc_v_trap_handler"
  "portasmHANDLE_INTERRUPT=interrupt_entry"
  )

# The include file search paths:
set(CMAKE_ASM_TARGET_INCLUDE_PATH
  "/home/ubuntuvm/Programs/bouffalo_sdk/bsp/board/bl616dk/."
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/libc/newlib/."
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/libc/."
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/mm/."
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/mm/tlsf/."
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/portable/GCC/RISC-V/common"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/portable/GCC/RISC-V/common/chip_specific_extensions/RV32I_CLINT_no_extensions"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/shell/."
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/utils/log"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/utils/ring_buffer"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/utils/bflb_block_pool"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/utils/bflb_timestamp"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/utils/getopt"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/btblecontroller/btble_inc"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/btblecontroller/btblecontroller_port"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/port/include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/common"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/common/include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/common/include/zephyr"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/common/include/misc"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/common/tinycrypt/include/tinycrypt"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/hci_onchip"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/bl_hci_wrapper"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/host"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/include/bluetooth"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/include/drivers/bluetooth"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/profiles"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/cli_cmds"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/wireless/bluetooth/blestack/src/services"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/include/arch"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/include/arch/risc-v/t-head"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/include/arch/risc-v/t-head/Core/Include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/include/arch/risc-v/t-head/csi_dsp/include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/config/bl616"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/lhal/src/flash"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/soc/bl616/std/include"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/soc/bl616/std/include/hardware"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/soc/bl616/rf/inc"
  "/home/ubuntuvm/Programs/bouffalo_sdk/drivers/rfparam/Inc"
  "/home/ubuntuvm/Programs/bouffalo_sdk/projects/ble-audio/."
  )

# The set of dependency files which are needed:
set(CMAKE_DEPENDS_DEPENDENCY_FILES
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/croutine.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/croutine.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/croutine.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/event_groups.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/event_groups.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/event_groups.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/freertos_port.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/freertos_port.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/freertos_port.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/list.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/list.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/list.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/portable/GCC/RISC-V/common/port.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/portable/GCC/RISC-V/common/port.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/portable/GCC/RISC-V/common/port.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/portable/MemMang/heap_3.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/portable/MemMang/heap_3.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/portable/MemMang/heap_3.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/queue.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/queue.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/queue.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/tasks.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/tasks.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/tasks.c.o.d"
  "/home/ubuntuvm/Programs/bouffalo_sdk/components/os/freertos/timers.c" "build_out/components/os/freertos/CMakeFiles/freertos.dir/timers.c.o" "gcc" "build_out/components/os/freertos/CMakeFiles/freertos.dir/timers.c.o.d"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
