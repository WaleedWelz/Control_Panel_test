/*  guard to protect my file  */
#ifndef   BIT_MATH_H
#define   BIT_MATH_H

#define   CLR_BIT( REG , BIT_NUM )       (REG &= ~( 1 << BIT_NUM ))
#define   SET_BIT( REG , BIT_NUM )       (REG |=  ( 1 << BIT_NUM ))
#define   TOG_BIT( REG , BIT_NUM )       (REG ^=  ( 1 << BIT_NUM ))
#define   GET_BIT( REG , BIT_NUM)       ((REG>>BIT_NUM)& 1) 

#define   REG_VALUE(REG , Value)      (REG = Value)

#define  S_LOW(REG)                (REG|=0x0f)
#define  C_LOW(REG)                (REG&=0xf0)
#define  S_HIGH(REG)               (REG|=0xf0)
#define  C_HIGH(REG)                (REG&=0x0f)


#endif   /*   End of guard */