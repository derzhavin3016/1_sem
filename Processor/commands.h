
#define _ ,

DEF_CMD(PUSH, 1, 2,
  {
    sprintf(bptr_o, "%d", *((int *)(bptr_in)) / 1000);
    bptr_o += sizeof(int);
    bptr_in += sizeof(int);
  },
  {
    int num_push = 0;
    if (sscanf(prog[PC].str + pos _ " %d" _ &num_push) == 1)
    {
      *((int *)bptr) = num_push * 1000;
      bptr = (char *)((int *)bptr + 1);
      buf_out_size += sizeof(int);
    }
    else
    {
      printf("Error. No argument for push command in line %d\n" _ PC + 1);
      return false;
    }
  };)

DEF_CMD(ADD, 2, 1, {}, {})

DEF_CMD(POP, 3, 1, {}, {})

DEF_CMD(END, 0, 1, {}, break;)
