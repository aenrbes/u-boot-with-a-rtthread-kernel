
RT_KERNEL_BUILD = y

obj-y += rt-thread/src/
obj-y += rt-thread/libcpu/arm/
obj-y += rt-thread/components/
obj-y += libraries/HAL_Drivers/
obj-y += libraries/STM32F4xx_HAL/
obj-y += board/
obj-y += applications/

subdir-ccflags-y += -Ilibrtt
subdir-ccflags-y += -Ilibrtt/rt-thread/include
subdir-ccflags-y += -Ilibrtt/rt-thread/components/drivers/include
subdir-ccflags-y += -Ilibrtt/rt-thread/components/libc/compilers/minilibc
subdir-ccflags-y += -Ilibrtt/libraries/HAL_Drivers/config
subdir-ccflags-y += -Ilibrtt/libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc
subdir-ccflags-y += -Ilibrtt/libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Include
subdir-ccflags-y += -Ilibrtt/libraries/STM32F4xx_HAL/CMSIS/Include
subdir-ccflags-y += -Ilibrtt/libraries/HAL_Drivers
subdir-ccflags-y += -Ilibrtt/board
subdir-ccflags-y += -Ilibrtt/board/CubeMX_Config/Inc
subdir-ccflags-y += -Ilibrtt/board/ports
subdir-ccflags-y += -DSTM32F429xx
subdir-ccflags-y += -DUSE_HAL_DRIVER
subdir-ccflags-y += -DRT_USING_NOLIBC
subdir-ccflags-y += -D__SOFTFP__
SUBDIR_CFLAGS_REMOVE = -Werror=date-time
