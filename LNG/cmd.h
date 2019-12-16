#define REC_L rec_calc(nd->left)

#define REC_R rec_calc(nd->right)



DEF_OP('+', PLUS, 
  {
    return REC_L + REC_R;
  },
  {
    return dL + dR;
  })

DEF_OP('-', MINUS, 
  {
    return REC_L - REC_R;
  },
  {
    return dL - dR;
  })

DEF_OP('*', MUL, 
  {
    return REC_L * REC_R;
  },
  {
    return dL * cR + cL * dR;
  })

DEF_OP('/', DIV,
  {
    double r = REC_R;
    TREE_ASSERT(r != 0, "Dividing by zero!!");
    
    return REC_L / r;
  },
  {
    return (dL * cR - cL * dR) / (cR * cR);
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
    bool IsL = _find_var_tree(nd.left, var_num)_
         IsR = _find_var_tree(nd.right, var_num);
    if (IsL && IsR)
      return *(new node(0));


    if (IsR)
    {
      return cR * (cL ^ (cR - *(new node(1)))) * dL;
    }
    if (IsL)
      return (cL ^ cR) * *(new node(TYPE_FUNC, "ln", 2, 5, nullptr, &cL));

    return (cL ^ cR) * ( cR * dL / cL + *(new node(TYPE_FUNC, "ln", 2, 5, nullptr, &cL)) * dR);
  })

#undef REC_R

#undef REC_L

#undef _