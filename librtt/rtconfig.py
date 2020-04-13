import os

# toolchains options
ARCH='arm'
CPU='cortex-m4'
CROSS_TOOL='gcc'

# bsp lib config
BSP_LIBRARY_TYPE = None

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')
if os.getenv('RTT_ROOT'):
    RTT_ROOT = os.getenv('RTT_ROOT')

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery, Keil MDK, IAR
if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = '/usr/bin'
elif CROSS_TOOL == 'keil':
    PLATFORM    = 'armcc'
    EXEC_PATH   = r'C:/Keil_v5'
elif CROSS_TOOL == 'iar':
    PLATFORM    = 'iar'
    EXEC_PATH   = r'C:/Program Files (x86)/IAR Systems/Embedded Workbench 8.0'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    CXX = PREFIX + 'g++'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'a'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    PLATFORM_CPPFLAGS    = ' -march=armv7-m'
    PLATFORM_CPPFLAGS   += ' -mword-relocations'
    PLATFORM_CPPFLAGS   += ' -fno-pic'
    PLATFORM_CPPFLAGS   += ' -mabi=aapcs-linux'
    AFLAGS_IMPLICIT_IT   = ' -Wa,-mimplicit-it=always'
    PF_CPPFLAGS_ARM	     = AFLAGS_IMPLICIT_IT + ' -mthumb -mthumb-interwork'
    PLATFORM_CPPFLAGS   += PF_CPPFLAGS_ARM
    PLATFORM_CPPFLAGS   += ' -mno-unaligned-access'
    
    PLATFORM_RELFLAGS  = ''
    PLATFORM_CPPFLAGS += ' -pipe'
    PLATFORM_RELFLAGS += ' -ffunction-sections -fdata-sections -fno-common -ffixed-r9'
    PLATFORM_RELFLAGS += ' -msoft-float'# -malignment-traps' + ' -mshort-load-bytes'
    PLATFORM_CPPFLAGS += PLATFORM_RELFLAGS

    asflags_y       = PLATFORM_CPPFLAGS
    ccflags_y       = PLATFORM_CPPFLAGS
    cppflags_y      = PLATFORM_CPPFLAGS

    CSTD_FLAG        = ' -std=gnu11'
    KBUILD_CFLAGS    = ' -Wall -Wstrict-prototypes -Wno-format-security' + CSTD_FLAG# + ' -fno-builtin -ffreestanding'
    KBUILD_CFLAGS	+= ' -fshort-wchar -fno-strict-aliasing'
    KBUILD_CFLAGS	+= ' -fno-PIE'
    KBUILD_CFLAGS   += ' -fno-stack-protector'
    KBUILD_CFLAGS   += ' -fno-delete-null-pointer-checks'
    KBUILD_CFLAGS	+= ' -g'
    KBUILD_CFLAGS   += ' -Wno-format-nonliteral'

    KBUILD_AFLAGS	= ' -fno-PIE'
    KBUILD_AFLAGS	+= ' -g'

    orig_c_flags   = KBUILD_CFLAGS + ccflags_y
    _c_flags       = orig_c_flags
    orig_a_flags   = KBUILD_AFLAGS + asflags_y 
    _a_flags       = orig_a_flags
    _cpp_flags     = cppflags_y

    c_flags        = _c_flags 
    a_flags        = _a_flags
    cpp_flags      = _cpp_flags
    ld_flags       = ' -r'

    #DEVICE = ' -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections'
    CFLAGS = c_flags + ' -Dgcc'
    AFLAGS = ' -c' + a_flags + ' -x assembler-with-cpp '#-Wa,-mimplicit-it=thumb '
    LFLAGS = ld_flags + PLATFORM_CPPFLAGS + ' -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T board/linker_scripts/link.lds'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0'
    else:
        CFLAGS += ' -O2'

    CXXFLAGS = CFLAGS 

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'

elif PLATFORM == 'armcc':
    # toolchains
    CC = 'armcc'
    CXX = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --cpu Cortex-M4.fp '
    CFLAGS = '-c ' + DEVICE + ' --apcs=interwork --c99'
    AFLAGS = DEVICE + ' --apcs=interwork '
    LFLAGS = DEVICE + ' --scatter "board\linker_scripts\link.sct" --info sizes --info totals --info unused --info veneers --list rtthread.map --strict'
    CFLAGS += ' -I' + EXEC_PATH + '/ARM/ARMCC/include'
    LFLAGS += ' --libpath=' + EXEC_PATH + '/ARM/ARMCC/lib'

    CFLAGS += ' -D__MICROLIB '
    AFLAGS += ' --pd "__MICROLIB SETA 1" '
    LFLAGS += ' --library_type=microlib '
    EXEC_PATH += '/ARM/ARMCC/bin/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -O2'

    CXXFLAGS = CFLAGS 
    CFLAGS += ' -std=c99'

    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET'

elif PLATFORM == 'iar':
    # toolchains
    CC = 'iccarm'
    CXX = 'iccarm'
    AS = 'iasmarm'
    AR = 'iarchive'
    LINK = 'ilinkarm'
    TARGET_EXT = 'out'

    DEVICE = '-Dewarm'

    CFLAGS = DEVICE
    CFLAGS += ' --diag_suppress Pa050'
    CFLAGS += ' --no_cse'
    CFLAGS += ' --no_unroll'
    CFLAGS += ' --no_inline'
    CFLAGS += ' --no_code_motion'
    CFLAGS += ' --no_tbaa'
    CFLAGS += ' --no_clustering'
    CFLAGS += ' --no_scheduling'
    CFLAGS += ' --endian=little'
    CFLAGS += ' --cpu=Cortex-M4'
    CFLAGS += ' -e'
    CFLAGS += ' --fpu=VFPv4_sp'
    CFLAGS += ' --dlib_config "' + EXEC_PATH + '/arm/INC/c/DLib_Config_Normal.h"'
    CFLAGS += ' --silent'

    AFLAGS = DEVICE
    AFLAGS += ' -s+'
    AFLAGS += ' -w+'
    AFLAGS += ' -r'
    AFLAGS += ' --cpu Cortex-M4'
    AFLAGS += ' --fpu VFPv4_sp'
    AFLAGS += ' -S'

    if BUILD == 'debug':
        CFLAGS += ' --debug'
        CFLAGS += ' -On'
    else:
        CFLAGS += ' -Oh'

    LFLAGS = ' --config "board/linker_scripts/link.icf"'
    LFLAGS += ' --entry __iar_program_start'

    CXXFLAGS = CFLAGS
    
    EXEC_PATH = EXEC_PATH + '/arm/bin/'
    POST_ACTION = 'ielftool --bin $TARGET rtthread.bin'
