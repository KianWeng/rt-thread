#ifndef __KEY_H__
#define __KEY_H__

#define SW_RIGHT_MASK (1 << 6)
#define SW_DOWN_MASK (1 << 7)
#define SW_LEFT_MASK (1 << 8)
#define SW_UP_MASK (1 << 9)
#define SW_C_MASK (1 << 5)
#define SW_A_MASK (1 << 12)
#define SW_B_MASK (1 << 13)
#define SW_D_MASK (1 << 19)

enum KEY_NUM
{
    SW_RIGHT,
    SW_DOWN,
    SW_LEFT,
    SW_UP,
    SW_C,
    SW_A,
    SW_B,
    SW_D,
    SW_MAX
};


#endif
