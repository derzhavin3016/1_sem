
#define VAL(node) node->value

#define TYPE(node) node->type

#define LN nd->left

#define RN nd->right

#define LV VAL(LN)

#define RV VAL(RN)

#define RT TYPE(RN)

#define LT TYPE(LN)

#define DEL(nde)                   \
                     delete nde;   \
                     nde = nullptr;

#define CHANGE_ND(nde)  node *nd_prev = nd;  \
                         nd = nde;           \
                         free(nd_prev);

#define SET_ND(nd_set, val, type)   \
  VAL(nd_set) = val;                \
  TYPE(nd_set) = type; 

#define END      \
  symp_counter++; \
  return nd;

#define ND_CMP(node, val) (TYPE(node) == TYPE_NUMBER && (VAL(node)) == val)

#define IF_ND_ZERO(node) (TYPE(node) == TYPE_NUMBER && Compare(VAL(node)))

#define KILL_ZERO(node_check, node_new)          \
if (IF_ND_ZERO(node_check))                      \
    {                                            \
      DEL(node_check);                           \
      CHANGE_ND(node_new);                       \
      END;                                       \
    }

#define KILL_ONE(node_check, node_new)           \
  if (ND_CMP(node_check, 1))                     \
    {                                            \
      DEL(node_check);                           \
      CHANGE_ND(node_new);                       \
      END;                                       \
    }

DEF_SMP('+',
  {
    KILL_ZERO(LN, RN);

    KILL_ZERO(RN, LN);
  })

DEF_SMP('-',
  {
    KILL_ZERO(RN, LN);
  })

DEF_SMP('*',
  {
    if (IF_ND_ZERO(LN) || IF_ND_ZERO(RN))
    {
      DEL(LN);
      DEL(RN);
      SET_ND(nd, 0, TYPE_NUMBER);
      END;
    }
    
    KILL_ONE(RN, LN);

    KILL_ONE(LN, RN);
  })

DEF_SMP('/',
  {
    KILL_ZERO(LN, RN);

    KILL_ONE(RN, LN);
  })

#define _ ,

DEF_SMP('^', 
  {
    if (ND_CMP(LN, 1) || IF_ND_ZERO(LN))
    {
      DEL(LN);
      DEL(RN);
      SET_ND(nd, 1, TYPE_NUMBER);
      END;
    }

    KILL_ZERO(LN, RN);

    KILL_ONE(RN, LN);
  })

#undef VAL

#undef TYPE

#undef LN

#undef RN

#undef LV

#undef RV

#undef RT

#undef LT

#undef DEL

#undef CHANGE_ND

#undef SET_ND

#undef END

#undef ND_CMP

#undef IF_ND_ZERO

#undef KILL_ZERO

#undef KILL_ONE 