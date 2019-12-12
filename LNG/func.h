
#define REC_R rec_calc(nd->right)


#define COS *(new node(TYPE_FUNC, "cos", 3, 2, nullptr, &cR ))
#define SIN *(new node(TYPE_FUNC, "sin", 3, 1, nullptr, &cR ))
#define CH  *(new node(TYPE_FUNC, "ch", 2, 8, nullptr, &cR ))
#define SH  *(new node(TYPE_FUNC, "sh", 2, 7, nullptr, &cR ))

DEF_FNC(sin, 1, 
  {
    return COS * dR;
  },
  {
    return sin(REC_R);
  })

DEF_FNC(cos, 2, 
  {
    return (*(new node(-1)) * SIN) * dR;
  },
  {
    return cos(REC_R);
  })

DEF_FNC(tg, 3, 
  {
    return (*(new node(1)) / (COS * COS)) * dR;
  },
  {
    double co = cos(REC_R)

    TREE_ASSERT(!Compare(co), "tangens dont' defined");
    return tan(REC_R);
  })

DEF_FNC(ctg, 4, 
  {
    return (*(new node(-1)) / (SIN * SIN)) * dR;
  },
  {
    double res = REC_R;
    double si = sin(res);

    TREE_ASSERT(!Compare(si), "cotangens dont' defined")
    return cos(res) / si;
  })

DEF_FNC(ln, 5, 
  {
    return (*(new node(1)) / cR) * dR;
  },
  {
    double res = REC_R;
    TREE_ASSERT(res > 0, "Log argument don't positive");

    return log(REC_R);
  })

DEF_FNC(sqrt, 6, 
  {
    return (*(new node(1)) / (*(new node(2)) * cR)) * dR;
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
    return (*(new node(1)) / (CH * CH)) * dR;
  },
  {
    return tanh(REC_R);
  })

DEF_FNC(cth, 10, 
  {
    return (*(new node(-1)) / (SH * SH)) * dR;
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
