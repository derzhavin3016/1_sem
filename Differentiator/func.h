
#define REC_R rec_calc(nd->right)


#define COS *(new node(TYPE_FUNC, "cos", 3, 2, nullptr, &cR ))
#define SIN *(new node(TYPE_FUNC, "sin", 3, 1, nullptr, &cR ))
#define CH  *(new node(TYPE_FUNC, "ch", 2, 8, nullptr, &cR ))
#define SH  *(new node(TYPE_FUNC, "sh", 2, 7, nullptr, &cR ))

#define ONE *(new node(1))

#define MINUS_ONE *(new node(-1))

DEF_FNC(sin, 1, 
  {
    return COS * dR;
  },
  {
    return sin(REC_R);
  })

DEF_FNC(cos, 2, 
  {
    return (MINUS_ONE * SIN) * dR;
  },
  {
    return cos(REC_R);
  })

DEF_FNC(tg, 3, 
  {
    return (ONE / (COS * COS)) * dR;
  },
  {
    double co = cos(REC_R)

    TREE_ASSERT(!Compare(co), "tangens dont' defined");
    return tan(REC_R);
  })

DEF_FNC(ctg, 4, 
  {
    return (MINUS_ONE / (SIN * SIN)) * dR;
  },
  {
    double res = REC_R;
    double si = sin(res);

    TREE_ASSERT(!Compare(si), "cotangens dont' defined")
    return cos(res) / si;
  })

DEF_FNC(ln, 5, 
  {
    return (ONE / cR) * dR;
  },
  {
    double res = REC_R;
    TREE_ASSERT(res > 0, "Log argument don't positive");

    return log(REC_R);
  })

DEF_FNC(sqrt, 6, 
  {
    return (ONE / (*(new node(2)) * cR)) * dR;
  },
  {
    double res = REC_R;
    TREE_ASSERT(res >= 0, "Sqrt argument negative");

    return sqrt(REC_R);
  })

DEF_FNC(sh, 7, 
  {
    return CH * dR;
  },
  {
    return sinh(REC_R);
  })

DEF_FNC(ch, 8, 
  {
    return SH * dR;
  },
  {
    return cosh(REC_R);
  })

DEF_FNC(th, 9, 
  {
    return (ONE / (CH * CH)) * dR;
  },
  {
    return tanh(REC_R);
  })

DEF_FNC(cth, 10, 
  {
    return (MINUS_ONE / (SH * SH)) * dR;
  },
  {
    double res = REC_R;

    TREE_ASSERT(!Compare(res), "Hyperbolic cotangens dont' defined")
    return 1 / tanh(res);
  })

DEF_FNC(exp, 11, 
  {
    return *(new node(TYPE_FUNC, "exp", 3, 11, nullptr, &cR )) * dR;
  },
  {
    return exp(REC_R);
  })
