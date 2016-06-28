#include "E2CubicSpline.h"
#include <math.h>


_e2_cub_spline::_e2_cub_spline()
{
}


_e2_cub_spline::~_e2_cub_spline()
{

}

_e2_cub_spline::_e2_cub_spline(const _e2_cub_spline& object)
{
    dequeOfEqus.assign(object.dequeOfEqus.begin(), object.dequeOfEqus.end());
    dequeOfNodes.assign(object.dequeOfNodes.begin(), object.dequeOfNodes.end());
}

_e2_cub_spline::_e2_cub_spline(const unsigned num, E2_POINT *points)
{
    CubicSplineInterpolation(num, points);
}

_e2_cub_spline& _e2_cub_spline::operator=(const _e2_cub_spline& object)
{
    dequeOfEqus.clear();
    dequeOfEqus.assign(object.dequeOfEqus.begin(), object.dequeOfEqus.end());
    dequeOfNodes.clear();
    dequeOfNodes.assign(object.dequeOfNodes.begin(), object.dequeOfNodes.end());
	return *this;
}

//解三对角矩阵
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

bool _e2_cub_spline::CubicSplineInterpolation(const unsigned num, E2_POINT *points, bool bContinue /* = false */)
{
    std::deque<E2_POINT>::iterator iter_point_calcMask;
    CUB_EQU &lastCub(dequeOfEqus.back());

    if(bContinue)
    {
        iter_point_calcMask = dequeOfNodes.end();//指向上一段最后元素
        for(unsigned i = 0; i < num; ++i)
            dequeOfNodes.push_back(points[i]);//拼接数据
    }
    else
    {
        dequeOfNodes.assign(points, points + num);//重置数组
        iter_point_calcMask = dequeOfNodes.begin();//指向新的数据第一位
    }

    unsigned nodeAmount(num);//能不要吗？
    unsigned subAmount(nodeAmount - 1);//能不要吗？

    //分配差值空间
    double *deltaX = new double[subAmount](); //能不能组合？
    double *deltaY = new double[subAmount](); //能不能组合？

    //calculate the delta of X,Y
    for (unsigned i = 0; i < nodeAmount; ++i)
    {
        deltaX[i] = (iter_point_calcMask + 1)->x - iter_point_calcMask->x;
        deltaY[i] = (iter_point_calcMask + 1)->y - iter_point_calcMask->y;

        iter_point_calcMask++;
    }

    //parameters of equation translation linear algebra
    //[A, B, C]*[D]^T=[E]^T
    double *A = new double[nodeAmount]();
    double *B = new double[nodeAmount]();
    double *C = new double[nodeAmount]();
    double *D = new double[nodeAmount]();
    double *E = new double[nodeAmount]();

    //set up the spline condition
    if(bContinue)
    {
        //Clamped
        B[0] = 2 * deltaX[0];
        C[0] = deltaX[0];
        E[0] = 6.0 * (deltaY[0] / deltaX[0] - lastCub.B);
    }
    else
    {
        //Natural begin point
        B[0] = 1;
        C[0] = 0;
        E[0] = 0;
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
    //Not-A-Knot end point
    double fac(deltaX[subAmount - 1] / A[subAmount - 1]);
    A[subAmount] = B[subAmount - 1] * fac + deltaX[subAmount - 2] + deltaX[subAmount - 1];
    B[subAmount] = C[subAmount - 1] * fac - deltaX[subAmount - 2];
    E[subAmount] = E[subAmount - 1] * fac;

    //解三对角矩阵
    SolTridiagonalMatrices(nodeAmount, A, B, C, D, E);

    CUB_EQU subEqu;
    for (unsigned i = 0; i < subAmount; ++i)
    {
        //calculate the paramers of equ
        subEqu.A = dequeOfNodes[i].y;
        subEqu.B = deltaY[i] / deltaX[i] - deltaX[i] * 0.5 * D[i] - deltaX[i] / 6.0 * (D[i + 1] - D[i]);
        subEqu.C = D[i] * 0.5;
        subEqu.D = (D[i + 1] - D[i]) / 6.0 / deltaX[i];

        dequeOfEqus.push_back(subEqu);
    }

    delete[] deltaX;
    delete[] deltaY;
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] D;
    delete[] E;

    return true;
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

        if (point[pointIndex].x > dequeOfNodes[cubIndex + 1].x && ++cubIndex == dequeOfEqus.size())//整理一下逻辑
        {
            point[pointIndex].x = dequeOfNodes[cubIndex--].x;
        }

        deltaX = point[pointIndex].x - dequeOfNodes[cubIndex].x;

        CUB_EQU &cubEqu(dequeOfEqus[cubIndex]);
        point[pointIndex].y = cubEqu.A + cubEqu.B * deltaX + cubEqu.C * deltaX * deltaX +cubEqu.D * deltaX * deltaX *deltaX;
        point[pointIndex].dy = cubEqu.B + 2 * cubEqu.C * deltaX + 3 * cubEqu.D * deltaX * deltaX;
        point[pointIndex].ddy = 2 * cubEqu.C + 6 * cubEqu.D * deltaX;

        if(point[pointIndex].x == dequeOfNodes[cubIndex + 1].x)//不能删
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

    if(dequeOfNodes.size() < 1)
        return false;

    if(x < dequeOfNodes.front().x || x > dequeOfNodes.back().x)
        return false;

    if(x == dequeOfNodes.back().x)
    {
        cubIndex = dequeOfEqus.size() - 2;
    }
    else
    {
        for(; cubIndex < dequeOfEqus.size(); )
        {
            if(dequeOfNodes[++cubIndex].x > x)
            {
                cubIndex--;
                break;
            }
        }
    }

    double deltaX(x - dequeOfNodes[cubIndex].x);

    CUB_EQU &cubEqu(dequeOfEqus[cubIndex]);
    y = cubEqu.A + cubEqu.B * deltaX + cubEqu.C * deltaX * deltaX + cubEqu.D * deltaX * deltaX *deltaX;

    return true;
}

bool _e2_cub_spline::GetYFull(const double &x, SP_PROIFILE &y)
{
    unsigned cubIndex(0);

    if(dequeOfNodes.size() < 1)
        return false;

    if(x < dequeOfNodes.front().x || x > dequeOfNodes.back().x)
        return false;

    if(x ==  dequeOfNodes.back().x)
    {
        cubIndex = dequeOfEqus.size() - 2;
    }
    else
    {
        for(; cubIndex < dequeOfEqus.size(); )
        {
            if(dequeOfNodes[++cubIndex].x > x)
            {
                cubIndex--;
                break;
            }
        }
    }

    double deltaX(x - dequeOfNodes[cubIndex].x);

    CUB_EQU &cubEqu(dequeOfEqus[cubIndex]);
    y.y = cubEqu.A + cubEqu.B * deltaX + cubEqu.C * deltaX * deltaX + cubEqu.D * deltaX * deltaX *deltaX;
    y.dy = cubEqu.B + 2 * cubEqu.C * deltaX + 3 * cubEqu.D * deltaX * deltaX;
    y.ddy = 2 * cubEqu.C + 6 * cubEqu.D * deltaX;

    return true;
}
