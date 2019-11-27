#define REC_L rec_calc(nd->left)

#define REC_R rec_calc(nd->right)


DEF_OP('+', PLUS, 
  {
    return REC_L + REC_R;
  },
  {
    return dL + dR;
  }, 
  {
     if (nd->left->type == TYPE_NUMBER && Compare(nd->left->value))
    {
      delete nd->left;
      nd->left = nullptr;
      node *nd_prev = nd;
      nd = nd->right;
      free(nd_prev);
      symp_counter++;
      return nd;
    }

    if (nd->right->type == TYPE_NUMBER && Compare(nd->right->value))
    {
      delete nd->right;
      nd->right = nullptr;
      node *nd_prev = nd;
      nd = nd->left;
      free(nd_prev);
      symp_counter++;
      return nd;
    }
  })

DEF_OP('-', MINUS, 
  {
    return REC_L - REC_R;
  },
  {
    return dL - dR;
  },
  {
    if (nd->left->type == TYPE_NUMBER && Compare(nd->left->value))
    {
      delete nd->left;
      nd->left = nullptr;
      node *nd_prev = nd;
      nd = nd->right;
      free(nd_prev);
      symp_counter++;
      return nd;
    }
  })

DEF_OP('*', MUL, 
  {
    return REC_L * REC_R;
  },
  {
    return dL * cR + cL * dR;
  },
  {
    if ((nd->left->type == TYPE_NUMBER && Compare(nd->left->value)) ||
        (nd->right->type == TYPE_NUMBER && Compare(nd->right->value)))
    {
      delete nd->left;
      nd->left = nullptr;
      delete nd->right;
      nd->right = nullptr;
      nd->type = TYPE_NUMBER;
      nd->value = 0;
      symp_counter++;
      return nd;
    }
    if (nd->left->type == TYPE_NUMBER && nd->left->value == 1)
    {
      delete nd->left;
      nd->left = nullptr;
      node *nd_prev = nd;
      nd = nd->right;
      free(nd_prev);
      symp_counter++;
      return nd;
    }
    if (nd->right->type == TYPE_NUMBER && nd->right->value == 1)
    {
      delete nd->right;
      nd->right = nullptr;
      node *nd_prev = nd;
      nd = nd->left;
      free(nd_prev);
      symp_counter++;
      return nd;
    }
  })

DEF_OP('/', DIV,
  {
    double r = REC_R;
    TREE_ASSERT(r != 0, "Dividing by zero!!");
    
    return REC_L / r;
  },
  {
    return (dL * cR - cL * dR) / (cR * cR);
  },
  {
  })

#define _ ,

DEF_OP('^', POW,
  {
     double l = REC_L;
     double r = REC_R;
     TREE_ASSERT(!Compare(l) && !Compare(r), "0^0 undefined");
     TREE_ASSERT(l < 0, "negative base");

     return pow(l, r);
  },
  {
    bool IsL = _is_calc(nd.left)_
         IsR = _is_calc(nd.right);
    if (IsL && IsR)
      return *(new node(0));


    if (IsR)
    {
      return cR * (cL ^ (cR - *(new node(1)))) * dL;
    }
    if (IsL)
      return (cL ^ cR) * *(new node(TYPE_FUNC, "ln", 2, 5, nullptr, &cL));

    return (cL ^ cR) * ( cR * dL / cL + *(new node(TYPE_FUNC, "ln", 2, 5, nullptr, &cL)) * dR);
  },
  {
  })

#undef REC_R

#undef REC_L