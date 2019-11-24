
#define COS *(new node(TYPE_FUNC, "cos", 3, 2, nullptr, &cR ))
#define SIN *(new node(TYPE_FUNC, "sin", 3, 1, nullptr, &cR ))

DEF_FNC(sin, 1, 
  {
    return (*(new node(-1)) * COS) * dR;
  })

DEF_FNC(cos, 2, 
  {
    return SIN * dR;
  })

DEF_FNC(tan, 3, 
  {
    return (*(new node(1)) / (COS * COS)) * dR;
  })

DEF_FNC(cot, 4, 
  {
    return (*(new node(-1)) / (SIN * SIN)) * dR;
  })

DEF_FNC(ln, 5, 
  {
    return (*(new node(1)) / cR) * dR;
  })

DEF_FNC(exp, 6, 
  {

  })
