#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#pragma warning (disable: 4996 4018)

#define SQTest(A, B, C, X1, X2, NROOTS) if (!TestSquare(A, B, C, X1, X2, NROOTS)) \
                                  return 0;
const double EPSILON = 1e-12;

const int MAXTRIES = 100;

const int MAXCOEFF = 3;

__inline int Compare( double a, double b );

int SolveSquare( double a, double b, double c, double *x1, double *x2 );

int SolveLinear( double a, double b, double *x );

int InputNumbers( const char promt[], double* values, size_t amount );

void FillNan( double* array, size_t size );

void ClearBuffer( void );

int TestSquare(double a, double b, double c, double x1, double x2, int nRoots);

int UnitTests( void );


enum roots
{
	SS_INF_ROOTS = -1,
	SS_ZERO_ROOTS, //0
	SS_ONE_ROOT,   //1
	SS_TWO_ROOTS   //2
};


int main( void )
{
	// Coefficients array
  double coeff[MAXCOEFF];

  // Filling array with NAN values
  FillNan(coeff, MAXCOEFF);

  if (UnitTests())
    printf("UnitTests sucess.\n");
  else
  {
    printf("UnitTests failed\n");
    return 1;
  }

  // Inputing the coefficients
  int OK = InputNumbers("Square equation solver program\n\n"
    "Input coefficients:\n", coeff, MAXCOEFF);
  assert(OK);

	// Roots
	double x1 = NAN, x2 = NAN;
	int nRoots = SolveSquare(coeff[0], coeff[1], coeff[2], &x1, &x2);

	switch (nRoots)
	{
	case SS_ZERO_ROOTS:
		printf("No roots in real.\n");
		break;
	case SS_ONE_ROOT:
		printf("x = %.4lg\n", x1);
		break;
	case SS_TWO_ROOTS:
		printf("x1 = %.4lg\nx2 = %.4lg\n", x1, x2);
		break;
	case SS_INF_ROOTS:
		printf("Infinity amount of roots\n");
		break;
	default:
		printf("main() : ERROR. Incorrect amount of roots: %d\n", nRoots);
		return 1;
	}

	return 0;
}

// doxygen
/**
 * Square equation solver function
 * \param [in]   a    First coefficient.
 * \param [in]   b    Second coefficient.
 * \param [in]   c    Third coefficient.
 * \param [out] *x1   Pointer to first root.
 * \param [out] *x2   Pointer to second root.
 * \return Number of roots
 * \note   Returns SS_INF_ROOTS if equation has infinity amount of roots,
 *                 SS_ZERO_ROOTS if equation hasn't any roots,
 *                 SS_ONE_ROOT if equation has one root,
 *                 SS_TWO_ROOTS if equation has two roots.
 */
int SolveSquare( double a, double b, double c, double *x1, double *x2 )
{
	/* checking if the coefficients is finite */
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));

	/* checking pointers */
	assert(x1 != NULL);
	assert(x2 != NULL);
	assert(x1 != x2);


	if (Compare(a, 0.0))
		return SolveLinear(b, c, x1);
	if (Compare(c, 0.0))
	{
		*x1 = 0;
		SolveLinear(a, b, x2);
    if (*x2 == 0)
      return SS_ONE_ROOT;
		return SS_TWO_ROOTS;
	}
	if (Compare(b, 0.0))
	{
		double x_square = -c / a;
		if (x_square > 0)
		{
			double abs_x = sqrt(x_square);
			*x1 =  abs_x;
			*x2 = -abs_x;
			return SS_TWO_ROOTS;
		}
		return SS_ZERO_ROOTS;
	}

	double d = b * b - 4 * a * c;

	if (d > 0)
	{
    double sqrt_d = sqrt(d);

		*x1 = (-b + sqrt_d) / (2 * a);
		*x2 = (-b - sqrt_d) / (2 * a);
		return SS_TWO_ROOTS;
	}
	if (Compare(d, 0.0))
	{
		*x1 = -b / (2 * a);
		return SS_ONE_ROOT;
	}

	return SS_ZERO_ROOTS;
} /* End of 'SolveSquare' function */

/**
 * Linear equation solver function
 * \param [in]   a     First coefficient.
 * \param [in]   b     Second coefficient.
 * \param [out] *x     Pointer to root.
 * \return Number of roots
 * \note   Returns SS_INF_ROOTS if equation has infinity amount of roots,
 *                 SS_ZERO_ROOTS if equation hasn't any roots,
 *                 SS_ONE_ROOT if equation has one root.
 */
int SolveLinear( double a, double b, double *x )
{
  assert(isfinite(a));
	assert(isfinite(b));
  assert(x != NULL);

	if (Compare(a, 0.0))
	{
		if (Compare(b, 0.0))
			return SS_INF_ROOTS;
		return SS_ZERO_ROOTS;
	}
	if (Compare(b, 0.0))
	{
		*x = 0;
		return SS_ONE_ROOT;
	}
	*x = -b / a;
	return SS_ONE_ROOT;
} /* End of 'SolveLinear' function */

/**
 * Compare two numbers function
 * \param [in] a   first number.
 * \param [in] b   second number.
 * \return 1 if numbers are equal, 0 otherwise.
 * \note Accuracy defined by EPSILON constant.
 */
__inline int Compare( double a, double b )
{
	return (fabs(a - b) < EPSILON);
} /* End of 'Compare' function */

/**
 * Input numbers function
 * \param [in]    promt[]  String to print.
 * \param [out]  *values   Pointer to array with inputed values.
 * \param [in]    amount   Amount of values.
 * \return 1 if all is ok, 0 otherwise.
 */
int InputNumbers( const char promt[], double *values, size_t amount )
{
  assert(values != NULL);
  assert(amount != 0);
 
  int correct_input = 0, is_prev_correct = 1, incr = 0;

  printf(promt);

  for (int i = 0; i < MAXTRIES && correct_input < amount; i++)
  {

    if (is_prev_correct)
    {
      printf("Input [%d/%zd] value: ", correct_input + 1, amount);
      is_prev_correct = !is_prev_correct;
    }
    incr = scanf("%lg", values + correct_input);
    ClearBuffer();
    if (incr)
    {
      correct_input++;
      is_prev_correct = !is_prev_correct;
    }
    else
      printf("Incorrect input. Try again.\n");
  }

  return (correct_input == amount);
} /* End of 'InputNumbers' function */

/**
 * Fill array with NAN values function
 * \param [out]  *array  Pointer to array.
 * \param [in]    size   Array size.
 * \return  NONE.
 */
void FillNan( double* arr, size_t size )
{
  assert(arr != NULL);
  assert(size != 0);

  for (int i = 0; i < size; i++)
    arr[i] = (double)NAN;
} /* End of 'FillNan' function */

/**
 * Input numbers function (second variant).
 * \param [in]    promt[]  String to print.
 * \param [in]    amount   Amount of values.
 * \return Pointer to array with inputed values.
 * \warning Dynamic memory, it is necessary to use free() function.
 */
double * InputNumbers2( const char promt[], size_t amount )
{
  double *data = (double *)calloc(amount, sizeof(data[0]));
  int correct_input = 0, incr;

  for (int i = 0; i < MAXTRIES; i++)
  {
    printf("Input [%d/%zd] value: ", correct_input + 1, amount);
    incr = scanf("%lg", data + correct_input);
    ClearBuffer();
    if (incr)
      correct_input++;
  }

  return data;
} /* End of 'InputNumbers2' function */

/**
 * Clear input buffer fucntion.
 * \return  NONE.
 */
void ClearBuffer( void )
{
  char sym = getchar();

  while (sym != '\n')
    sym = getchar();
} /* End of 'ClearBuffer' function */

/**
 * \brief Test SolveSquare function (one test).
 * \param [in]   a    First coefficient.
 * \param [in]   b    Second coefficient.
 * \param [in]   c    Third coefficient.
 * \param [in] *x1    Expected root.
 * \param [in] *x2    Expected second root.
 * \return 1 if all is OK
 * \return 0 otherwise.
 */
int TestSquare( double a, double b, double c, double x1, double x2, int nRoots )
{
  double Roots[2];
  if (SolveSquare(a, b, c, Roots, Roots + 1) == nRoots)
    switch (nRoots)
    {
    case SS_ZERO_ROOTS:
      return 1;
    case SS_INF_ROOTS:
      return 1;
    case SS_ONE_ROOT:
      if (Roots[0] == x1)
        return 1;
      return 0;
    case SS_TWO_ROOTS:
      if ((Compare(Roots[0], x1)) && (Compare(Roots[1], x2)) ||
          (Compare(Roots[0], x2)) && (Compare(Roots[1], x1)))
        return 1;
      return 0;
    default:
      return 0;
    }
  return 1;
} /* End of 'TestSquare' function */

/**
 * \brief Unit tests for SolveSquare.
 * \return 1 if all is OK
 * \return 0 otherwise.
 */
int UnitTests( void )
{
  SQTest(1, 1, 1, 0, 0, SS_ZERO_ROOTS);

  SQTest(1, 5, 6, -2, -3, SS_TWO_ROOTS);

  SQTest(1, 0, 0, 0, 0, SS_ONE_ROOT);

  SQTest(0, 5, 6, -6 / 5.0, 0, SS_ONE_ROOT);

  SQTest(0, 0, 6, 0, 0, SS_ZERO_ROOTS);

  SQTest(0, 0, 0, 0, 0, SS_INF_ROOTS);

  SQTest(0, 0.0000001, 5, -5 / 0.0000001, 0, SS_ONE_ROOT);

  SQTest(1, 0, -2, sqrt(2), -sqrt(2), SS_TWO_ROOTS);

  return 1;
} /* End of 'UnitTests' function */