#pragma once

/*
@brief: spline profile data
@author: JoMar
@date: 2016-06-23
*/
typedef struct _sp_profile
{
    double x;
    double y;
    double dy;
    double ddy;
}
/*
@brief: spline profile data
@author: JoMar
@date: 2016-06-23
*/
SP_PROIFILE;

/*
@brief: 2 dimensional point
@author: JoMar
@date: 2016-06-16
*/
typedef struct _e2_point
{
	double x;
	double y;
	_e2_point() : x(0), y(0){};
	_e2_point(const double newX, const double newY) : x(newX), y(newY){};
	_e2_point(const _e2_point& object) : x(object.x), y(object.y){};
	_e2_point& operator=(const _e2_point& object)
	{
		x = object.x;
		y = object.y;
		return *this;
	}
}
/*
@brief: 2 dimensional point
@author: JoMar
@date: 2016-06-16
*/
E2_POINT;

/*
@brief: cubic equation
@author: JoMar
@date: 2016-06-16
@note: S(xi)=Ai+Bi(X-Xi)+Ci(X-Xi)2+Di(X-Xi)3
*/
typedef struct _cub_equ
{
	double A;
	double B;
	double C;
	double D;
	_cub_equ() : A(0), B(0), C(0), D(0){};
	~_cub_equ(){};
	_cub_equ(const double newA, const double newB, const double newC, const double newD) : A(newA), B(newB), C(newC), D(newD){};
	_cub_equ(const _cub_equ& object) : A(object.A), B(object.B), C(object.C), D(object.D){};
	_cub_equ& operator=(const _cub_equ& object)
	{
		A = object.A;
		B = object.B;
		C = object.C;
		D = object.D;
		return *this;
	}
}
/*
@brief: cubic equation
@author: JoMar
@date: 2016-06-16
@note: S(x)=A(x-x0)3+B(x-x0)2+C(x+x0)+D
*/
CUB_EQU;

/*
@brief: boundary information of section of spline
@author: JoMar
@date: 2016-06-16
*/
typedef struct _boundary_info
{
	double lsatSecondDerivative;
	E2_POINT lastPoint;

	_boundary_info() : lsatSecondDerivative(0){};
	_boundary_info(const _boundary_info& object) : lsatSecondDerivative(object.lsatSecondDerivative), lastPoint(object.lastPoint){};
	_boundary_info& operator=(const _boundary_info& object)
	{
		lsatSecondDerivative = object.lsatSecondDerivative;
		lastPoint = object.lastPoint;
		return *this;
	}
}
/*
@brief: boundary information of section of spline
@author: JoMar
@date: 2016-06-16
*/
BOUNDARY_INFO;

/*
@brief: 2 dimensional cubic spline
@author: JoMar
@date: 2016-06-16
*/
typedef class _e2_cub_spline
{
public:
	unsigned pointAmount;
	unsigned subAmount;
	E2_POINT *nodeArray;//nodes
	CUB_EQU *equArray;//the equ paramers
	double *subCurveLen;//spline len from start point to node

	_e2_cub_spline();

	virtual ~_e2_cub_spline();

	_e2_cub_spline(const unsigned num, E2_POINT *points);

	_e2_cub_spline(const _e2_cub_spline& object);

	_e2_cub_spline& operator=(const _e2_cub_spline& object);

	/*
	@brief: get the point by spline len
	@param[in]: len
	@param[out]: point
	@author: JoMar
	@date: 2016-06-23
	*/
	bool GetPointByLen(const double &len, E2_POINT &point);

	/*
	@brief: get the point by step of x
	@param[in]: step
	@param[out]: point
	@author: JoMar
	@date: 2016-06-23
	*/
    bool GetPointByStep(const double &step, SP_PROIFILE *point, unsigned &pointNum);

    /*
    @brief: get y by x
    @param[in]: x
    @param[out]: point
    @author: JoMar
    @date: 2016-06-24
    */
    bool GetY(const double &x, SP_PROIFILE &y);
	
private:
	BOUNDARY_INFO boundInfo;

	inline void DeleteArray();

	inline void CreateArray();

	inline void CopyNodeArray(const E2_POINT* const &points);

	inline void CopyArray(const _e2_cub_spline& object);

	void CubicSplineInterpolationFront();

	void CubicSplineInterpolationContinue();

	/*
	@brief: sol Tridiagonal Matrices Parameters
	@author: JoMar
	@date: 2016-06-15
	@note:
	[B0, C0, 0 , 0 , ... 0   , 0   , 0   ]
	[A1, B1, C1, 0 , ... 0   , 0   , 0   ]
	[0,  A2, B2, C2, ... 0   , 0   , 0   ]
	[0,  0 , 0 , 0 , ... An-1, Bn-1, Cn-1]
	[0,  0 , 0 , 0 , ... 0   , An  , Bn  ]
	*/
	void SolTridiagonalMatrices(const unsigned n, const double* A, const double* B, double* C, double *X, double* Y);

	/*
	@brief: Sol Quartic Equation
	@author: JoMar
	@date: 2016-06-23
	@note: ax4+bx3+cx2+dx+e=0
	*/
	void SolQuarticEquation(const double a, const double b, const double c, const double d, const double e, double &x);
}
/*
@brief: 2 dimensional cubic spline
@author: JoMar
@date: 2016-06-16
*/
E2_CUB_SPLINE;

