#include<stdio.h>
/*
unsigned char:  0 to 255        (only positive + zero)
signed char:  -128 to 127       (negative + zero + positive)
*/
int main() {
    //=================================
    // Unsigned << >> Shift - 0 fill
    //=================================
    unsigned char u = 0x95; // 1001 0101 =149, limit is only (0-255)
    unsigned char s = 3; //small value for left shift becuase over flow reason only 8 bit cover 

    printf("==== UNSIGNED Right Shift ===\n");
    printf("Original : %d (1001 0101)\n",u); //149
  
    unsigned char u_RightShift = u >> 1;
    printf(">> 1(1001 0101) : %d \n",u_RightShift);//74, right shift means 1 shift gone so divided by 2 compare to decimal like 230/10 = 23 this is right shift 1 value gone base value binary %2

      // ==========================
    /*
    Use behind right shift (1001 0101) 0 1001 010 = 0100 1010

    = 0×128 + 1×64 + 0×32 + 0×16 + 1×8 + 0×4 + 1×2 + 0×1
    = 0 + 64 + 0 + 0 + 8 + 0 + 2 + 0
    = 74 ✓
    149 ÷ 2 = 74 — correct! 
*/
// =====================================

      // ===============================

    printf("=== UNSIGNED Left Shift ===\n");
    unsigned char s_LeftShift = s << 1;
    printf("<< 1 (1001 0101) : %d \n",s_LeftShift);// 42,left gone but add right value like 0 this same as decimal like 23x10 = 230, right 0 added same as binary base value 2 

     // =====================================
     /*
    Use behind left shift (0000 0011) 000 00110 = 0000 0110

    = 0×128 + 0×64 + 0×32 + 0×16 + 0×8 + 1×4 + 1×2 + 0×1
    = 0 + 0 + 0 + 0 + 0 + 4 + 2 + 0
    = 6 ✓
    3 x 2 = 6  — correct! //because binary base 2 
  */
  // =====================================

    printf("Check Right Shift : 149 / 2 = %d\n",149 / 2);
    printf("Check Left value  : 6 x 2 = %d\n",3 * 2);

    // ==================================
    // Signed << >> shift MSB Copy
    // =================================

    signed char s = (signed char)0x95; //1001 0101 = -107,because signed start with (-128 to 127)

    /*
    signed char s = 1001 0101
                ↑
                MSB = 1, weight = -128
       Value = -128 + 0 + 0 + 16 + 0 + 4 + 0 + 1 = -107
    */

    printf("=== Signed Right Shift ===\n");
    signed char s_Rightshift = s >> 1;
    printf(" >> 1 (1001 0101) : %d \n",s_Rightshift);

     // =====================================
     /*
    Use behind Right shift (1001 0101) 11001 010 = 1100 1010

    MSB-ரோட weight = -128 (negative! இதுதான் minus-ரோட source)

     = 1×(-128) + 1×64 + 0×32 + 0×16 + 1×8 + 0×4 + 1×2 + 0×1
     = -128  + 64 + 0  + 0 + 8 + 0 + 2  + 0
     = -128 + 74
     = -54
    -107 / 2 = 53.5  — correct! //because binary base 2 
  */
  // =====================================

   printf("==== Signed Left Shift ====\n ");
   signed char s_LeftShift = s << 1;
   printf(" << 1 (1001 0101)  :  %d\n",s_LeftShift);

   // =====================================
     /*
    Use behind Left shift (1001 0101) 001 01010 = 0010 1010
     = 0×(-128) + 0×64 + 1×32 + 0×16 + 1×8 + 0×4 + 1×2 + 0×1
     = 0  + 0 + 32  + 0 + 8 + 0 + 2  + 0
     = 42
    -107 x 2 = -214  — correct! //because binary base 2 
  */
  // =====================================







}
