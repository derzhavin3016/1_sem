
#define _ ,

#define DATA_SIZE sizeof(int)

#define STK_PUSH(VALUE) stk.Push(VALUE)

#define STK_POP(PTR)  stk.Pop(PTR)

#define GET_FST_SEC_VALUES   int first = 0 _ second = 0;                    \
                             if (!STK_POP(&first) || !STK_POP(&second))     \
                               return false;                  

#define GET_VALUE   int value = 0;                                          \
                    if (!STK_POP(&value))                                   \
                      return false;

#define CODE_INT *((int *)code_ptr)

#define GET_REG(STR, CHECK, CMD)   else if (sscanf(prog[PC].str + pos, STR, &reg_push, &reg_check) == 2 &&                    \
                                            toupper(reg_check) == CHECK)                                                      \
                                   {                                                                                          \
                                     *(bptr - 1) = CMD;                                                                       \
                                     *bptr++ = toupper(reg_push) - 'A' + 1;                                                   \
                                     buf_out_size++;                                                                          \
                                   }

DEF_CMD(PUSH, 1, 2,
  {
    sprintf(bptr_o, "%d", CODE_INT / ACCURACY);
    bptr_o += DATA_SIZE;
    code_ptr += DATA_SIZE;
  },
  {
    double num_push = 0;
    char reg_push = 0 _ reg_check = 0;
     

    if (sscanf(prog[PC].str + pos, " %lg", &num_push) == 1)
    {
      *((int *)bptr) = (int)num_push * ACCURACY;
      bptr = (char *)((int *)bptr + 1);
      buf_out_size += DATA_SIZE;
    }
    else if (sscanf(prog[PC].str + pos, " %c%c", &reg_push, &reg_check) == 2 &&
             toupper(reg_check) == 'X')
    {
      *(bptr - 1) = ASM_CMD_PUSH_REG;
      *bptr++ = toupper(reg_push) - 'A' + 1;
      buf_out_size++;
    }
    else if (sscanf(prog[PC].str + pos, " %c%c", &reg_check, &reg_push) == 2 &&
             reg_check == '[')
    {
      *(bptr - 1) = ASM_CMD_PUSH_RAM;
      *bptr++ = toupper(reg_push) - 'A' + 1;
      buf_out_size++;
    }
    else
    {
      printf("Error. No argument for push command in line %d" _ PC + 1);
      return false;
    }
  },
  {
    STK_PUSH(CODE_INT);
    code_ptr += DATA_SIZE;
  })

#define PUT_REG {                                                        \
                  sprintf(bptr_o _ "%cX" _ 'A' + *code_ptr - 1);         \
                  bptr_o += 2;                                           \
                  code_ptr++;                                            \
                }

DEF_CMD(PUSH_REG, 11, 2,
  {
      sprintf(bptr_o - 5 _ "    ");
      bptr_o -= 4;
      PUT_REG;
  },
  {
  },
  {
    STK_PUSH(regs[*code_ptr - 1]);
    code_ptr++;
  })

#define PUT_RAM {                                                          \
                  sprintf(bptr_o _ "[%cX]" _ 'A' + *code_ptr - 1);         \
                  bptr_o += 4;                                             \
                  code_ptr++;                                              \
                }

DEF_CMD(PUSH_RAM, 12, 2,
  {
      sprintf(bptr_o - 5 _ "    ");
      bptr_o -= 4;
      PUT_RAM;
  },
  {
  },
  {
    size_t num = regs[*code_ptr - 1];
    num = Clamp(num, (size_t)0, RAM_SIZE - 1);
    
    STK_PUSH(RAM[num]);
    code_ptr++;
  })


DEF_CMD(ADD, 2, 1, {}, {},
  {
    GET_FST_SEC_VALUES;
    STK_PUSH(first + second);
  })

DEF_CMD(POP, 3, 2,
  {
    PUT_REG;
  },
  {
    char reg_pop = 0 _ reg_check = 0;
    if ((sscanf(prog[PC].str + pos _ " %c%c" _ &reg_pop _ &reg_check) == 2) &&
             toupper(reg_check) == 'X')
    {
      *bptr++ = toupper(reg_pop) - 'A' + 1;
      buf_out_size++;
    }
    else if (sscanf(prog[PC].str + pos _ " %c%c" _ &reg_check _ &reg_pop) == 2 &&
             reg_check == '[')
    {
      *(bptr - 1) = ASM_CMD_POP_RAM;
      *bptr++ = toupper(reg_pop) - 'A' + 1;
      buf_out_size++;
    }
    else
    {
      printf("Error. No arguments for pop command in line %d" _ PC + 1);
      return false;
    }
  },
  {
    if (!STK_POP(regs + *code_ptr - 1))
      return false;
    code_ptr++;
  })

DEF_CMD(POP_RAM, 31, 2, 
  {
    sprintf(bptr_o - 5 _ "    ");
    bptr_o -= 4;
    PUT_RAM;
  },
  {
  },
  {
    size_t num = regs[*code_ptr - 1];
    num = Clamp(num, (size_t)0, RAM_SIZE - 1);
    
    GET_VALUE;
    RAM[num] = value;
    code_ptr++;
  })
#undef PUT_RAM

DEF_CMD(SUB, 4, 1, {}, {},
  {
    GET_FST_SEC_VALUES;
    STK_PUSH(first - second);
  })

DEF_CMD(MUL, 5, 1, {}, {},
  {
    GET_FST_SEC_VALUES;
    STK_PUSH((int)(((double)first / ACCURACY) * second));
  })

DEF_CMD(DIV, 6, 1, {}, {},
  {
    GET_FST_SEC_VALUES;
    if (second == 0)
    {
      printf("ERROR!!! Dividing by zero.\n");
      return false;
    }
    STK_PUSH((int)(ACCURACY * ((double)first / second)));
  })

DEF_CMD(OUT, 7, 1, {}, {},
  {
    GET_VALUE;
    printf("out : %lg\n", (double)value * 1.0 / ACCURACY);
  })

DEF_CMD(SQRT, 8, 1, {}, {},
  {
    int value = 0;
    
   if (!STK_POP(&value))
     return false;
    if (value < 0)
    {
      printf("ERROR!!! Square root from neg value.\n");
      return false;
    }
    STK_PUSH((int)(sqrt(((double)value * 1.0 / ACCURACY)) * ACCURACY));
  })

DEF_CMD(SIN, 9, 1, {}, {},
  {
    GET_VALUE;
    STK_PUSH(sin((int)((double)value * 1.0 / ACCURACY) * ACCURACY));
  })

DEF_CMD(COS, 10, 1, {}, {},
  {
    GET_VALUE;
    STK_PUSH(cos((int)((double)value * 1.0 / ACCURACY) * ACCURACY));
  })

#define JMP_DISASM  sprintf(bptr_o, "%d", CODE_INT);   \
                    bptr_o += DATA_SIZE;               \
                    code_ptr += DATA_SIZE;             

#define JMP_ASM  if (sscanf(prog[PC].str + pos _ " %s" _ promt) == 1)    \
                    {                                                       \
                      int line_mark = FindMark(promt);                      \
                      if (line_mark == -2 && IsAsm)                          \
                      {                                                     \
                        printf("Mark was not find : %s\n", promt);          \
                        return false;                                       \
                      }                                                     \
                      if (!IsAsm)                                           \
                        line_mark = -1;                                     \
                      *((int *)bptr) = line_mark;                           \
                      bptr = (char *)((int *)bptr + 1);                     \
                      buf_out_size += DATA_SIZE;                            \
                    }

#define DEF_JMP(cmd, num, cond)         DEF_CMD(cmd, num, 2,                               \
                                        {                                                  \
                                          JMP_DISASM;                                      \
                                        },                                                 \
                                        {                                                  \
                                          JMP_ASM;                                         \
                                        },                                                 \
                                        {                                                  \
                                          if (jmp_counter++ >= MAX_JUMPS)                  \
                                          {                                                \
                                            printf("Seems like infinity jumps\n");         \
                                            return false;                                  \
                                          }                                                \
                                          GET_FST_SEC_VALUES;                              \
                                           if (cond)                                        \
                                            code_ptr = (char *)(code + *((int *)code_ptr));          \
                                          else                                             \
                                            code_ptr += DATA_SIZE;                         \
                                        })


DEF_CMD(JMP, 20, 2,
{
  JMP_DISASM;
},
{
  JMP_ASM;
},
{
  if (jmp_counter++ >= MAX_JUMPS)
  {
    printf("Seems like infinity jumps\n");
    return false;
  }
  code_ptr = (char *)(code + *((int *)code_ptr));
})

DEF_JMP(JA, 21, first > second)

DEF_JMP(JAE, 22, first >= second)

DEF_JMP(JB, 23, first < second)

DEF_JMP(JBE, 24, first <= second)

DEF_JMP(JE, 25, first == second)

DEF_JMP(JNE, 26, first != second)

DEF_CMD(CALL, 40, 2,
  {
    JMP_DISASM;
  },
  {
    JMP_ASM;
  },
  {
    if (func_stk.Size() >= FUNC_REC)
    {
      printf("Function stack overflow\n");
      return false;
    }
    int mark = code_ptr + DATA_SIZE - code;
    func_stk.Push(mark);
    code_ptr = code + *((int *)code_ptr);
  })

DEF_CMD(RET, 41, 1, {}, {},
  {
    int value = 0;
    if (!func_stk.Pop(&value))
      return false;
    code_ptr = code + value;
  })


    DEF_CMD(IN, 42, 1, {}, {},
  {
    double value = 0;

    scanf("%lg", &value);
    STK_PUSH((int)(value * ACCURACY));
  })

DEF_CMD(POW, 43, 1, {}, {},
  {
    GET_FST_SEC_VALUES;
    STK_PUSH(pow((double)first * 1.0 / ACCURACY, (double)second * 1.0 / ACCURACY) * ACCURACY);
  })

DEF_CMD(END, -1, 1, {}, {},
  {
    return true;
  })


#undef _

#undef DATA_SIZE

#undef STK_PUSH

#undef STK_POP
 
#undef GET_FST_SEC_VALUES

#undef CODE_INT

#undef PUT_REG

#undef JMP_DISASM

#undef JMP_ASM

#undef DEF_JMP