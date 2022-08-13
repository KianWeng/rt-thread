import os

# toolchains options
ARCH='arm'
CPU='cortex-m0'
CROSS_TOOL='gcc'

if os.getenv('RTT_CC'):
	CROSS_TOOL = os.getenv('RTT_CC')

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery, Keil MDK, IAR
if  CROSS_TOOL == 'gcc':
	PLATFORM 	= 'gcc'
	EXEC_PATH 	= r'/usr/local/arm-elf/bin'
elif CROSS_TOOL == 'keil':
	PLATFORM 	= 'armcc'
	EXEC_PATH 	= r'C:/Keil'
elif CROSS_TOOL == 'iar':
    print('================ERROR============================')
    print('Not support iar yet!')
    print('=================================================')
    exit(0)

if os.getenv('RTT_EXEC_PATH'):
	EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    DEVICE = ' -mcpu=cortex-m0plus -mfloat-abi=soft -mthumb -mapcs -fno-common -ffunction-sections -fdata-sections -fno-builtin -ffreestanding -Wall'
    CFLAGS = DEVICE + ' -O0 -MMD -MP'
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -D__STARTUP_CLEAR_BSS'
    LFLAGS = DEVICE + ' -static --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections,-Map=rtthread-kl26z4.map,-cref,-u,Reset_Handler -T KL26Zxxx4.ld'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -g -DDEBUG'
        AFLAGS += ' -g -DDEBUG'
        LFLAGS += ' -g'
    else:
        CFLAGS += ' -O2 -DNDEBUG'
        AFLAGS += '-DNDEBUG'
        LFLAGS += ''

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'

elif PLATFORM == 'armcc':
    # toolchains
    CC = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --cpu Cortex-M0+ '
    CFLAGS = DEVICE + ' --c99 --apcs=interwork'
    AFLAGS = DEVICE
    LFLAGS = DEVICE + ' --info sizes --info totals --info unused --info veneers --list rtthread-kl26z4.map --scatter KL26Z.sct'

    LFLAGS += ' --keep *.o(.rti_fn.*)   --keep *.o(FSymTab) --keep *.o(VSymTab)' 

    EXEC_PATH += '/ARM/ARMCC/bin'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -O2'

    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET'
