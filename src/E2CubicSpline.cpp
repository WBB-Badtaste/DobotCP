#include "E2CubicSpline.h"
#include <math.h>


_e2_cub_spline::_e2_cub_spline(): pointAmount(0), subAmount(0), nodeArray(nullptr), equArray(nullptr), subCurveLen(nullptr)
{
}


_e2_cub_spline::~_e2_cub_spline()
{
	DeleteArray();
}

_e2_cub_spline::_e2_cub_spline(const _e2_cub_spline& object) : pointAmount(object.pointAmount), subAmount(object.subAmount), nodeArray(nullptr), equArray(nullptr), subCurveLen(nullptr)
{
	DeleteArray();
	CreateArray();
	CopyArray(object);
}

_e2_cub_spline::_e2_cub_spline(const unsigned num, E2_POINT *points) : pointAmount(num), subAmount(num - 1), nodeArray(nullptr), equArray(nullptr), subCurveLen(nullptr)
{
	CreateArray();
	CopyNodeArray(points);
	CubicSplineInterpolationFront();
}

_e2_cub_spline& _e2_cub_spline::operator=(const _e2_cub_spline& object)
{
	DeleteArray();
	CreateArray();
	pointAmount = object.pointAmount;
	subAmount = object.subAmount;
	CopyArray(object);
	boundInfo = object.boundInfo;
	return *this;
}

void _e2_cub_spline::DeleteArray()
{
	if (nodeArray)
	{
		delete[] nodeArray;
		nodeArray = 0;
	}
	if (equArray)
	{
		delete[] equArray;
		equArray = 0;
	}
	if (subCurveLen)
	{
		delete[] subCurveLen;
		subCurveLen = 0;
	}
}

void _e2_cub_spline::CreateArray()
{
	nodeArray = new E2_POINT[pointAmount]();
	equArray = new CUB_EQU[subAmount]();
	subCurveLen = new double[subAmount]();
}

void _e2_cub_spline::CopyNodeArray(const E2_POINT* const &points)
{
	for (unsigned i = 0; i < pointAmount; ++i)
		nodeArray[i] = points[i];
}

void _e2_cub_spline::CopyArray(const _e2_cub_spline& object)
{
	CopyNodeArray(object.nodeArray);
	for (unsigned i = 0; i < subAmount; ++i)
	{
		equArray[i] = object.equArray[i];
		subCurveLen[i] = object.subCurveLen[i];
	}
}

void _e2_cub_spline::SolTridiagonalMatrices(const unsigned n, const double* A, const double* B, double* C, double *X, double* Y)
{
	//forward elimination
	C[0] /= B[0];
	Y[0] /= B[0];

    for (unsigned i = 1; i < n; i++)
	{
		double tmp(B[i] - A[i] * C[i - 1]);
		C[i] /= tmp;
		Y[i] = (Y[i] - A[i] * Y[i - 1]) / tmp;
	}

	//backward substitution
	X[n - 1] = Y[n - 1];
	for (int i = n - 2; i >= 0; i--)
		X[i] = Y[i] - C[i] * X[i + 1];
}

void _e2_cub_spline::CubicSplineInterpolationFront()
{
	if (subAmount != pointAmount - 1)
	{
		return;//error
	}

	double *deltaX = new double[subAmount]();
	double *deltaY = new double[subAmount]();

	//parameters of equation translation linear algebra
	//[A, B, C]*[D]^T=[E]^T
	double *A = new double[pointAmount]();
	double *B = new double[pointAmount]();
	double *C = new double[pointAmount]();
	double *D = new double[pointAmount]();
	double *E = new double[pointAmount]();

	//calculate the delta of X,Y
	for (unsigned i = 0; i < subAmount; ++i)
	{
		deltaX[i] = nodeArray[i + 1].x - nodeArray[i].x;
		deltaY[i] = nodeArray[i + 1].y - nodeArray[i].y;
	}

	//calculate the matrix
	for (unsigned i = 1; i < subAmount; ++i)
	{
		A[i] = deltaX[i - 1];
		B[i] = 2 * (deltaX[i - 1] + deltaX[i]);
		C[i] = deltaX[i];
        E[i] = 6.0 * (deltaY[i] / deltaX[i] - deltaY[i - 1] / deltaX[i - 1]);
	}

	//set up the spline condition
	//Natural begin point
	B[0] = 1;
	C[0] = 0;
	E[0] = 0;
	//Not-A-Knot end point
	double fac(deltaX[subAmount - 1] / A[subAmount - 1]);
	A[subAmount] = B[subAmount - 1] * fac + deltaX[subAmount - 2] + deltaX[subAmount - 1];
	B[subAmount] = C[subAmount - 1] * fac - deltaX[subAmount - 2];
	E[subAmount] = E[subAmount - 1] * fac;

	SolTridiagonalMatrices(pointAmount, A, B, C, D, E);

	for (unsigned i = 0; i < subAmount; i++)
	{
		//calculate the paramers of equ
		equArray[i].A = nodeArray[i].y;
        equArray[i].B = deltaY[i] / deltaX[i] - deltaX[i] * 0.5 * D[i] - deltaX[i] / 6.0 * (D[i + 1] - D[i]);
		equArray[i].C = D[i] * 0.5;
		equArray[i].D = (D[i + 1] - D[i]) / 6.0 / deltaX[i];

		//calculate the len of the sub
        subCurveLen[i]
			= equArray[i].A * deltaX[i]
			+ equArray[i].B * pow(deltaX[i], 2) * 0.5
			+ equArray[i].C * pow(deltaX[i], 3) / 3
            + equArray[i].D * pow(deltaX[i], 4) / 4
			- nodeArray[i].y;

		if (i)
			subCurveLen[i] += subCurveLen[i - 1];//the sum len form start point
	}

	delete[] deltaX;
	delete[] deltaY;
	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;
	delete[] E;
}


void _e2_cub_spline::CubicSplineInterpolationContinue()
{

}


bool _e2_cub_spline::GetPointByLen(const double &len, E2_POINT &point)
{
	if (len > subCurveLen[subAmount - 1])
		return false;

	if (len == subCurveLen[subAmount - 1])
		point = nodeArray[pointAmount - 1];
	else
	{
		unsigned i = 0;

		for (; i < subAmount; ++i)
		{
			if (subCurveLen[i] < len && len < subCurveLen[i + 1])
				break;
		}
		
		double subLen(len - subCurveLen[i]);
	}
	
	return true;
}

void _e2_cub_spline::SolQuarticEquation(const double a, const double b, const double c, const double d, const double e, double &x)
{
	const double P((c*c + 12 * a * e - 3 * d * b) / 9);
	const double Q((27 * a *d *d + 2 * c * c * c + 27 * b * b * e - 72 * a * c * e - 9 * b * c * d));
	const double D(sqrt(Q * Q - P * P * P));
	const double tmp1(pow(Q + D, 1 / 3));
}

bool _e2_cub_spline::GetPointByStep(const double &step, SP_PROIFILE *point, unsigned &pointNum)
{
    unsigned cubIndex(0), pointIndex(0);
    double deltaX(0.0);
	while (1)
	{
        if(pointIndex)
            point[pointIndex].x = point[pointIndex - 1].x + step;
        else
            point[pointIndex].x = 0;

        if (point[pointIndex].x > nodeArray[cubIndex + 1].x && ++cubIndex == subAmount)//整理一下逻辑
		{
            point[pointIndex].x = nodeArray[cubIndex--].x;
		}

        deltaX = point[pointIndex].x - nodeArray[cubIndex].x;

		point[pointIndex].y = equArray[cubIndex].A + equArray[cubIndex].B * deltaX + equArray[cubIndex].C * deltaX * deltaX + equArray[cubIndex].D * deltaX * deltaX *deltaX;
        point[pointIndex].dy = equArray[cubIndex].B + 2 * equArray[cubIndex].C * deltaX + 3 * equArray[cubIndex].D * deltaX * deltaX;
        point[pointIndex].ddy = 2 * equArray[cubIndex].C + 6 * equArray[cubIndex].D * deltaX;

        if(point[pointIndex].x == nodeArray[cubIndex + 1].x)//不能删
        {
            pointIndex++;
            break;
        }

        pointIndex++;
    }
    pointNum = pointIndex;
	return true;
}

bool _e2_cub_spline::GetY(const double &x, double &y)
{
    unsigned cubIndex(0);

    if(x < nodeArray[0].x || x > nodeArray[pointAmount - 1].x)
        return false;

    if(x == nodeArray[pointAmount - 1].x)
    {
        cubIndex = subAmount-2;
    }
    else
    {
        for(; cubIndex < subAmount; )
        {
            if(nodeArray[++cubIndex].x > x)
            {
                cubIndex--;
                break;
            }
        }
    }

    double deltaX(x - nodeArray[cubIndex].x);

    y = equArray[cubIndex].A + equArray[cubIndex].B * deltaX + equArray[cubIndex].C * deltaX * deltaX + equArray[cubIndex].D * deltaX * deltaX *deltaX;

    return true;
}

bool _e2_cub_spline::GetYFull(const double &x, SP_PROIFILE &y)
{
    unsigned cubIndex(0);

    if(x < nodeArray[0].x || x > nodeArray[pointAmount - 1].x)
        return false;

    if(x == nodeArray[pointAmount - 1].x)
    {
        cubIndex = subAmount-2;
    }
    else
    {
        for(; cubIndex < subAmount; )
        {
            if(nodeArray[++cubIndex].x > x)
            {
                cubIndex--;
                break;
            }
        }
    }

    double deltaX(x - nodeArray[cubIndex].x);

    y.y = equArray[cubIndex].A + equArray[cubIndex].B * deltaX + equArray[cubIndex].C * deltaX * deltaX + equArray[cubIndex].D * deltaX * deltaX *deltaX;
    y.dy = equArray[cubIndex].B + 2 * equArray[cubIndex].C * deltaX + 3 * equArray[cubIndex].D * deltaX * deltaX;
    y.ddy = 2 * equArray[cubIndex].C + 6 * equArray[cubIndex].D * deltaX;

    return true;
}
