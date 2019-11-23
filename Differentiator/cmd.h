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


#undef REC_R

#undef REC_L