#!/bin/sh

echo "0) make sure all changes are committed"
diff=$(git diff | wc -l)
if [ $diff -gt 0 ]; then
        echo "You have diff"
        exit 1
fi

echo "1) make sure you've incremented the version number in Main/Constants.h"
read yes

builddir=/tmp/arduino_build_release
cachedir=/tmp/arduino_cache_release

mkdir "$builddir"

now=$(date +%Y%m%d_%H%M%S)
cp Main/config.h Main/config.h_"$now"
git checkout -- Main/config.h

/home/user/arduino-1.8.13/arduino-builder -compile -logger=machine -hardware /home/user/arduino-1.8.13/hardware -hardware /home/user/.arduino15/packages -tools /home/user/arduino-1.8.13/tools-builder -tools /home/user/arduino-1.8.13/hardware/tools/avr -tools /home/user/.arduino15/packages -built-in-libraries /home/user/arduino-1.8.13/libraries -libraries /home/user/Arduino/libraries -fqbn=esp32:esp32:esp32:JTAGAdapter=default,PSRAM=disabled,PartitionScheme=default,CPUFreq=240,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,LoopCore=1,EventsCore=1,DebugLevel=info,EraseFlash=none -ide-version=10813 -build-path "$builddir" -warnings=default -build-cache "$cachedir" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.xtensa-esp32-elf-gcc.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.xtensa-esp32-elf-gcc-esp-2021r2-patch5-8.4.0.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.xtensa-esp-elf-gdb.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp-elf-gdb/11.2_20220823 -prefs=runtime.tools.xtensa-esp-elf-gdb-11.2_20220823.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp-elf-gdb/11.2_20220823 -prefs=runtime.tools.openocd-esp32.path=/home/user/.arduino15/packages/esp32/tools/openocd-esp32/v0.12.0-esp32-20230419 -prefs=runtime.tools.openocd-esp32-v0.12.0-esp32-20230419.path=/home/user/.arduino15/packages/esp32/tools/openocd-esp32/v0.12.0-esp32-20230419 -prefs=runtime.tools.mklittlefs.path=/home/user/.arduino15/packages/esp32/tools/mklittlefs/3.0.0-gnu12-dc7f933 -prefs=runtime.tools.mklittlefs-3.0.0-gnu12-dc7f933.path=/home/user/.arduino15/packages/esp32/tools/mklittlefs/3.0.0-gnu12-dc7f933 -prefs=runtime.tools.dfu-util.path=/home/user/.arduino15/packages/arduino/tools/dfu-util/0.11.0-arduino5 -prefs=runtime.tools.dfu-util-0.11.0-arduino5.path=/home/user/.arduino15/packages/arduino/tools/dfu-util/0.11.0-arduino5 -prefs=runtime.tools.mkspiffs.path=/home/user/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.mkspiffs-0.2.3.path=/home/user/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.xtensa-esp32s3-elf-gcc.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.xtensa-esp32s3-elf-gcc-esp-2021r2-patch5-8.4.0.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.riscv32-esp-elf-gdb.path=/home/user/.arduino15/packages/esp32/tools/riscv32-esp-elf-gdb/11.2_20220823 -prefs=runtime.tools.riscv32-esp-elf-gdb-11.2_20220823.path=/home/user/.arduino15/packages/esp32/tools/riscv32-esp-elf-gdb/11.2_20220823 -prefs=runtime.tools.xtensa-esp32s2-elf-gcc.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32s2-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.xtensa-esp32s2-elf-gcc-esp-2021r2-patch5-8.4.0.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32s2-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.esptool_py.path=/home/user/.arduino15/packages/esp32/tools/esptool_py/4.5.1 -prefs=runtime.tools.esptool_py-4.5.1.path=/home/user/.arduino15/packages/esp32/tools/esptool_py/4.5.1 -prefs=runtime.tools.riscv32-esp-elf-gcc.path=/home/user/.arduino15/packages/esp32/tools/riscv32-esp-elf-gcc/esp-2021r2-patch5-8.4.0 -prefs=runtime.tools.riscv32-esp-elf-gcc-esp-2021r2-patch5-8.4.0.path=/home/user/.arduino15/packages/esp32/tools/riscv32-esp-elf-gcc/esp-2021r2-patch5-8.4.0 -verbose /home/user/Arduino/lightning-piggy/Main/Main.ino

echo "Done, see:"
ls -al "$builddir"/Main.ino.bin


exit


# building 1.x like this:

# /home/user/arduino-1.8.13/arduino-builder -compile -logger=machine -hardware /home/user/arduino-1.8.13/hardware -hardware /home/user/.arduino15/packages -tools /home/user/arduino-1.8.13/tools-builder -tools /home/user/arduino-1.8.13/hardware/tools/avr -tools /home/user/.arduino15/packages -built-in-libraries /home/user/arduino-1.8.13/libraries -libraries /home/user/Arduino/libraries -fqbn=esp32:esp32:esp32:PSRAM=disabled,PartitionScheme=default,CPUFreq=240,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=warn -vid-pid=1A86_55D4 -ide-version=10813 -build-path /tmp/arduino_build_7931 -warnings=default -build-cache /tmp/arduino_cache_351155 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.xtensa-esp32-elf-gcc.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-97-gc752ad5-5.2.0 -prefs=runtime.tools.xtensa-esp32-elf-gcc-1.22.0-97-gc752ad5-5.2.0.path=/home/user/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-97-gc752ad5-5.2.0 -prefs=runtime.tools.esptool_py.path=/home/user/.arduino15/packages/esp32/tools/esptool_py/3.0.0 -prefs=runtime.tools.esptool_py-3.0.0.path=/home/user/.arduino15/packages/esp32/tools/esptool_py/3.0.0 -prefs=runtime.tools.mkspiffs.path=/home/user/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.mkspiffs-0.2.3.path=/home/user/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -verbose /home/user/Arduino/lightning-piggy/Main/Main.ino
