#include "E2CubicSpline.h"
#include <math.h>


_e2_cub_spline::_e2_cub_spline()
    : Trajectory(SPLINE)
    , m_dequeOfNodes()
    , m_iter_ergodic()
    , m_lastNode()
{
    m_iter_ergodic = m_dequeOfNodes.begin();
}


_e2_cub_spline::~_e2_cub_spline()
{

}

_e2_cub_spline::_e2_cub_spline(const _e2_cub_spline& object)
    : Trajectory(SPLINE)
    , m_dequeOfNodes()
    , m_iter_ergodic()
    , m_lastNode(object.m_lastNode)
{
    m_dequeOfNodes.assign(object.m_dequeOfNodes.begin(), object.m_dequeOfNodes.end());
    m_iter_ergodic = m_dequeOfNodes.begin();
}

_e2_cub_spline::_e2_cub_spline(const unsigned num,
                               const std::deque<double> &time,
                               const std::deque<double> &data,
                               const SPLINE_SECTION_TYPE &type /* = STATIC_SEC */)
    : Trajectory(SPLINE)
    , m_dequeOfNodes()
    , m_iter_ergodic()
    , m_lastNode()
{
    Create(num, time, data, type);
    m_iter_ergodic = m_dequeOfNodes.begin();
}

_e2_cub_spline& _e2_cub_spline::operator=(const _e2_cub_spline& object)
{
    m_dequeOfNodes.assign(object.m_dequeOfNodes.begin(), object.m_dequeOfNodes.end());
    m_iter_ergodic = m_dequeOfNodes.begin();
    m_lastNode = object.m_lastNode;
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

/*
//需要另外拼接段
bool _e2_cub_spline::CalculateSpline(const unsigned nodeAmount, const double *pTime, const double *pData, const SET_SPLINE_MODE &mode)
//注意
//最后一点的基函数参数也会计算并保存
{
    //分配差值空间
    double *const deltaT = new double[nodeAmount - 1]();
    double *const deltaX = new double[nodeAmount - 1]();

    //parameters of equation translation linear algebra
    //[A, B, C]*[D]^T=[E]^T
    double *const A = new double[nodeAmount]();
    double *const B = new double[nodeAmount]();
    double *const C = new double[nodeAmount]();
    double *const D = new double[nodeAmount]();
    double *const E = new double[nodeAmount]();

    //calculate the delta of X,Y
    for (unsigned i = 0; i < nodeAmount - 1; ++i)
    {
        deltaT[i] = pTime[i + 1] - pTime[i];
        deltaX[i] = pData[i + 1] - pData[i];
    }

   //设置起始点约束条件
   if(mode == STATIC || mode == START )
   {
       //Clamped ,固定边界，开始速度为0
       B[0] = 2.0 * deltaT[0];
       C[0] = deltaT[0];
       E[0] = 6.0 * deltaX[0] / deltaT[0];
   }
   else if(mode == MIDDLE || mode == END)
   {
       //Not-A-Knot，开始是非纽结结点
       double fac1(deltaT[0] / deltaT[1]);
       B[0] = deltaT[0] * fac1 - deltaT[1];
       C[0] = (2.0  * fac1 + 1) * (deltaT[0] + deltaT[1]);
       E[0] = 6.0 * (deltaX[1] / deltaT[1] - deltaX[0] / deltaT[0]) * fac1;

       if(B[0] == 0.0)//解三对角矩阵时，不能为0
       {
           B[0] = 1;
           E[0] += deltaT[0];//有错！！！！！！！！！！！！！！！！！！！！！！！！！！！
       }
   }
   else
       return false;//mode有误，返回false

    //设置中间点
    for (unsigned i = 1; i < nodeAmount - 1; ++i)
    {
        A[i] = deltaT[i - 1];
        B[i] = 2.0 * (deltaT[i - 1] + deltaT[i]);
        C[i] = deltaT[i];
        E[i] = 6.0 * (deltaX[i] / deltaT[i] - deltaX[i - 1] / deltaT[i - 1]);
    }

   //设置终止点约束条件
   if(mode == STATIC || mode == END)
   {
       //Clamped ,固定边界，结尾速度为0
       A[nodeAmount - 1] = deltaT[nodeAmount - 2];
       B[nodeAmount - 1] = 2.0 * deltaT[nodeAmount - 2];
       E[nodeAmount - 1] = -6.0 * deltaX[nodeAmount - 2] / deltaT[nodeAmount - 2];
   }
   else if(mode == START || mode == MIDDLE)
   {
       //Not-A-Knot，末尾是非纽结结点
       double fac(deltaT[nodeAmount - 2] / A[nodeAmount - 2]);
       A[nodeAmount - 1] = B[nodeAmount - 2] * fac + deltaT[nodeAmount - 3] + deltaT[nodeAmount - 2];
       B[nodeAmount - 1] = C[nodeAmount - 2] * fac - deltaT[nodeAmount - 3];
       E[nodeAmount - 1] = E[nodeAmount - 2] * fac;
    }

    //解三对角矩阵
    SolTridiagonalMatrices(nodeAmount, A, B, C, D, E);

    //开始结算结点的参数值
    SP_NODE node;
    for (unsigned i = 0; i < nodeAmount; ++i)
    {
        //基本点
        node.timeStamp = pTime[i];
        //方程参数
        if(i == nodeAmount - 1)//末尾点设置方法不同
        {
            //直接用最后一段的方程计算末尾点的B参数
            //subEqu是上一次的
            //只需要套用三次方方程
            node.spCubEqu.B = node.spCubEqu.B + 2.0 * node.spCubEqu.C * deltaT[i - 1] + 3.0 * node.spCubEqu.D * deltaT[i - 1] * deltaT[i - 1];
            node.spCubEqu.A = pData[i];
            node.spCubEqu.C = D[i] * 0.5;
            //subEqu.D不变
        }
        else
        {
            //calculate the paramers of equ
            node.spCubEqu.A = pData[i];
            node.spCubEqu.B = deltaX[i] / deltaT[i] - deltaT[i] * 0.5 * D[i] - deltaT[i] / 6.0 * (D[i + 1] - D[i]);
            node.spCubEqu.C = D[i] * 0.5;
            node.spCubEqu.D = (D[i + 1] - D[i]) / 6.0 / deltaT[i];
        }

        m_dequeOfNodes.push_back(node);
    }

    //释放内存
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] D;
    delete[] E;
    delete[] deltaT;
    delete[] deltaX;

    return true;
}
*/

bool _e2_cub_spline::CalculateSpline(unsigned nodeAmount, const std::deque<double> &time, const std::deque<double> &data, const SPLINE_SECTION_TYPE &type)
//************注意***************
//最后一点的基函数参数也会计算并保存
{
    if(type == MIDDLE_SEC || type == END_SEC)
        nodeAmount++;

    //分配差值空间
    double *const deltaT = new double[nodeAmount - 1]();
    double *const deltaX = new double[nodeAmount - 1]();

    //parameters of equation translation linear algebra
    //[A, B, C]*[D]^T=[E]^T
    double *const A = new double[nodeAmount]();
    double *const B = new double[nodeAmount]();
    double *const C = new double[nodeAmount]();
    double *const D = new double[nodeAmount]();
    double *const E = new double[nodeAmount]();

    //计算差值
    if(type == MIDDLE_SEC || type == END_SEC)//需要连接上一段
    {
       for (unsigned i = 0; i < nodeAmount - 1; ++i)
       {
           //将上一段spline的最后一点纳入计算范围
           if(i == 0)
           {
               deltaT[i] = time[i] - m_dequeOfNodes.back().timeStamp;
               deltaX[i] = data[i] - m_dequeOfNodes.back().spCubEqu.A;
           }
           else
           {
               deltaT[i] = time[i] - time[i - 1];
               deltaX[i] = data[i] - data[i - 1];
           }
       }
    }
    else
    {
        for (unsigned i = 0; i < nodeAmount - 1; ++i)
        {
            deltaT[i] = time[i + 1] - time[i];
            deltaX[i] = data[i + 1] - data[i];
        }
    }

    //设置起始点约束条件
    if(type == STATIC_SEC || type == START_SEC)
    {
        //Clamped ,固定边界，开始速度为0
        B[0] = 2.0 * deltaT[0];
        C[0] = deltaT[0];
        E[0] = 6.0 * deltaX[0] / deltaT[0];
    }
    else if(type == MIDDLE_SEC || type == END_SEC)
    {
        //Not-A-Knot，开始是非纽结结点
        B[0] = 2.0 * deltaT[0];
        C[0] = deltaT[0];
        E[0] = 6.0 * (deltaX[0] / deltaT[0] - m_dequeOfNodes.back().spCubEqu.B);
    }

    //设置中间点
    for (unsigned i = 1; i < nodeAmount - 1; ++i)
    {
        A[i] = deltaT[i - 1];
        B[i] = 2.0 * (deltaT[i - 1] + deltaT[i]);
        C[i] = deltaT[i];
        E[i] = 6.0 * (deltaX[i] / deltaT[i] - deltaX[i - 1] / deltaT[i - 1]);
    }

    //设置终止点约束条件
    if(type == STATIC_SEC || type == END_SEC)
    {
        //Clamped ,固定边界，结尾速度为0
        A[nodeAmount - 1] = deltaT[nodeAmount - 2];
        B[nodeAmount - 1] = 2.0 * deltaT[nodeAmount - 2];
        E[nodeAmount - 1] = -6.0 * deltaX[nodeAmount - 2] / deltaT[nodeAmount - 2];
    }
    else if(type == START_SEC || type == MIDDLE_SEC)
    {
       //Not-A-Knot，末尾是非纽结结点
       double fac(deltaT[nodeAmount - 2] / A[nodeAmount - 2]);
       A[nodeAmount - 1] = B[nodeAmount - 2] * fac + deltaT[nodeAmount - 3] + deltaT[nodeAmount - 2];
       B[nodeAmount - 1] = C[nodeAmount - 2] * fac - deltaT[nodeAmount - 3];
       E[nodeAmount - 1] = E[nodeAmount - 2] * fac;
    }

    //解三对角矩阵
    SolTridiagonalMatrices(nodeAmount, A, B, C, D, E);

    //开始结算结点的参数值
    SP_NODE node;
    for (unsigned i = 0; i < nodeAmount; ++i)
    {
        //设置结点TYPE
        if(i == 0 && (type == START_SEC || type == STATIC_SEC))
            node.type = START_NODE;
        else if(i == nodeAmount - 1 && (type == END_SEC || type == STATIC_SEC))
            node.type = END_NODE;
        else
            node.type = MIDDLE_NODE;

        //方程参数
        if(i == 0 && (type == MIDDLE_SEC || type == END_SEC))
        {
            //连接处直接修改上一个结点的参数
            m_dequeOfNodes.back().spCubEqu.C = D[i] * 0.5;
            m_dequeOfNodes.back().spCubEqu.D = (D[i + 1] - D[i]) / 6.0 / deltaT[i];
        }
        else
        {
            node.timeStamp = (type == MIDDLE_SEC || type == END_SEC) ? time[i - 1] : time[i];

            if(i == nodeAmount - 1)//末尾点设置方法不同
            {
                //直接用最后一段的方程计算末尾点的B参数
                //subEqu是上一次的
                //只需要套用三次方方程
                node.spCubEqu.B = node.spCubEqu.B + 2.0 * node.spCubEqu.C * deltaT[i - 1] + 3.0 * node.spCubEqu.D * deltaT[i - 1] * deltaT[i - 1];
                node.spCubEqu.A = (type == MIDDLE_SEC || type == END_SEC) ? data[i - 1] : data[i];
                node.spCubEqu.C = D[i] * 0.5;
                //subEqu.D不变
            }
            else
            {

                //calculate the paramers of equ
                node.spCubEqu.A = (type == MIDDLE_SEC || type == END_SEC) ? data[i - 1] : data[i];
                node.spCubEqu.B = deltaX[i] / deltaT[i] - deltaT[i] * 0.5 * D[i] - deltaT[i] / 6.0 * (D[i + 1] - D[i]);
                node.spCubEqu.C = D[i] * 0.5;
                node.spCubEqu.D = (D[i + 1] - D[i]) / 6.0 / deltaT[i];
            }

            m_dequeOfNodes.push_back(node);
        }
    }

    //释放内存
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] D;
    delete[] E;
    delete[] deltaT;
    delete[] deltaX;

    return true;
}



bool _e2_cub_spline::StitchSpline(std::deque<SP_NODE>::iterator mix_iter)
{
    //三次方拼接，一阶导数连续
    const double dT((mix_iter + 1)->timeStamp - mix_iter->timeStamp);

    double F1((mix_iter + 1)->spCubEqu.A - mix_iter->spCubEqu.A - mix_iter->spCubEqu.B * dT);
    double F2((mix_iter + 1)->spCubEqu.B - mix_iter->spCubEqu.B);

    mix_iter->spCubEqu.D = (F2 - 2.0 * F1 / dT) / dT / dT;
    mix_iter->spCubEqu.C = F1 / dT / dT - mix_iter->spCubEqu.D * dT ;

    //五次方拼接，二阶导数平滑
//  //基础值
//  const double dT((mix_iter + 1)->timeStamp - mix_iter->timeStamp);
//  const double a(mix_iter->spCubEqu.A);
//  const double b(mix_iter->spCubEqu.B);
//  const double c(mix_iter->spCubEqu.C);
//  double &d(mix_iter->spCubEqu.D);
//  double &e(mix_iter->spCubEqu.E);
//  double &f(mix_iter->spCubEqu.F);
//
//  //中间变量
//  double dT_2(dT * dT);
//  double dT_3(dT_2 * dT);
//  double dT_4(dT_3 * dT);
//
//  double A1(dT_3);
//  double A2(3.0 * dT_2);
//  double A3(6.0 * dT);
//
//  double B1(dT_4);
//  double B2(4.0 * dT_3);
//  double B3(12.0 * dT_2);
//
//  double C1(dT_4 * dT);
//  double C2(5.0 * dT_4);
//  double C3(20.0 * dT_3);
//
//  double D1((mix_iter + 1)->spCubEqu.A - (a + b * dT + c * dT_2));
//  double D2((mix_iter + 1)->spCubEqu.B - (b + 2.0 * c * dT));
//  double D3(2.0 * ((mix_iter + 1)->spCubEqu.C - c));
//
//  //计算
//  double F1(A2 / A1);
//  B2 -= B1 * F1;
//  C2 -= C1 * F1;
//  D2 -= D1 * F1;
//
//  double F2(A3 / A1);
//  B3 -= B1 * F2;
//  C3 -= C1 * F2;
//  D3 -= D1 * F2;
//
//  double F3(B3 / B2);
//  C3 -= C2 * F3;
//  D3 -= D2 * F3;
//
//  f = D3 / C3;
//  e = (D2 - C2 * f) / B2;
//  d = (D1 - C1 * f - B1 * e) / A1;

    return true;
}


bool _e2_cub_spline::Create(
        const unsigned num,
        const std::deque<double> &time,
        const std::deque<double> &data,
        const SPLINE_SECTION_TYPE type /* = STATIC_SEC */)
{
//  if(mode == MIDDLE || mode == END)
//  {
//      if(m_dequeOfNodes.size() < 1)//需要拼接的时候发现spline没有初始化，返回false
//          return false;
//
//      if(num < 3)
//          return false;//特殊处理！！！！！！！
//
//        std::deque<SP_NODE>::iterator mix_iter = m_dequeOfNodes.end() - 1;
//
//      if(!CalculateSpline(num, pTime, pData, mode))
//          return false;
//
//        if(!StitchSpline(mix_iter))
//            return false;
//  }
//  else
//  {
//      if(num < 3)
//          return false;
//
//      if(!CalculateSpline(num, pTime, pData, mode))
//          return false;
//  }

    if(type == STATIC_SEC || type == START_SEC)
    {
        if(num < 3) return false;
    }
    else
    {
        if(num < 2) return false;
    }

    if(!CalculateSpline(num, time, data, type))
        return false;

    //最后点备份
    m_lastNode = m_dequeOfNodes.back();

    return true;
}

bool _e2_cub_spline::GetFirstData(
        double *const time/* = nullptr */,
        double *const pos /* = nullptr */,
        double *const vel /* = nullptr */,
        double *const acc /* = nullptr */)
{
    if(m_dequeOfNodes.size() < 1)//如果没有结点，没有初始化spline，返回flase
        return false;

    if(!(time || pos || vel || acc))//如果没有传入参数，返回flase
        return false;

    if(time)
        *time = m_dequeOfNodes.front().timeStamp;

    if(pos)
        *pos = m_dequeOfNodes.front().spCubEqu.A;

    if(vel)
        *vel = m_dequeOfNodes.front().spCubEqu.B;

    if(acc)
        *acc = 2.0 * m_dequeOfNodes.front().spCubEqu.C;

    return true;
}

bool _e2_cub_spline::GetLastData(
        double *const time/* = nullptr */,
        double *const pos /* = nullptr */,
        double *const vel /* = nullptr */,
        double *const acc /* = nullptr */)
{
    if(m_dequeOfNodes.size() < 1)//如果没有结点，没有初始化spline，返回flase
        return false;

    if(!(time || pos || vel || acc))//如果没有传入参数，返回flase
        return false;

    if(time)
        *time = m_lastNode.timeStamp;

    if(pos)
        *pos = m_lastNode.spCubEqu.A;

    if(vel)
        *vel = m_lastNode.spCubEqu.B;

    if(acc)
        *acc = 2.0 * m_lastNode.spCubEqu.C;

    return true;
}


bool _e2_cub_spline::GetDataByTimeStamp(
        const double timeStamp,
        double *const pos /* = nullptr */,
        double *const vel /* = nullptr */,
        double *const acc /* = nullptr */,
        GET_DATA_MODE mode /* = SEARCH */)
//*****特别注意*****
//最后一个点可能没有获取
{
    if(m_dequeOfNodes.size() < 1)//如果没有结点，没有初始化spline，返回flase
        return false;

    if(!(pos || vel || acc))//如果没有传入参数，返回flase
        return false;

    std::deque<SP_NODE>::iterator iter;

    //如果时间戳超出spline范围，返回flase
    if(timeStamp < m_dequeOfNodes.front().timeStamp || timeStamp > m_dequeOfNodes.back().timeStamp)
        return false;

    if(mode == SEARCH)//搜索时间戳对应哪一个结点的函数
    {
        for(iter = m_dequeOfNodes.begin(); iter != m_dequeOfNodes.end() - 1; ++iter)
        {
            if(timeStamp <= (iter + 1)->timeStamp)
                break;
        }
    }
    else if(mode == ERGODIC_START)//使用指针开始快速搜索
    {
        iter = m_iter_ergodic = m_dequeOfNodes.begin();//重置指针
    }
    else if(mode == ERGODIC_CONTINUE)//使用指针继续快速搜索
    {
        if(timeStamp > (m_iter_ergodic + 1)->timeStamp)//时间戳已经进入下一结点范围
            if(++m_iter_ergodic == m_dequeOfNodes.end())//当前已经是最后一段
                return false;
        iter = m_iter_ergodic;
    }
    else
        return false;//取值模式有误
    
    const CUB_EQU &cubEqu(iter->spCubEqu);//使用常量引用，防止误修改  
    const double &dT(timeStamp - iter->timeStamp);//使用常量引用，防止误修改

    //计算数据
    double dT_2(dT * dT);
    double dT_3(dT_2 * dT);
    double dT_4(dT_3 * dT);

    if(pos)
        *pos = cubEqu.A
             + cubEqu.B * dT
             + cubEqu.C * dT_2
             + cubEqu.D * dT_3
             + cubEqu.E * dT_4
             + cubEqu.F * dT_4 * dT;

    if(vel)
        *vel = cubEqu.B
             + 2.0 * cubEqu.C * dT
             + 3.0 * cubEqu.D * dT_2
             + 4.0 * cubEqu.E * dT_3
             + 5.0 * cubEqu.F * dT_4;

    if(acc)
        *acc = 2.0  * cubEqu.C
             + 6.0  * cubEqu.D * dT
             + 12.0 * cubEqu.E * dT_2
             + 20.0 * cubEqu.F * dT_3;

    return true;
}

int _e2_cub_spline::FirstNodeTimeStamp()
{
    return m_dequeOfNodes.front().timeStamp;
}

int _e2_cub_spline::LastNodeTimeStamp()
{
    return m_lastNode.timeStamp;
}

NODE_TYPE _e2_cub_spline::LastNodeType()
{
    return m_lastNode.type;
}

//bool _e2_cub_spline::SynReadData(SP_NODE &node)
//{
//    return true;
//}
//
//bool _e2_cub_spline::SynWriteData(const SP_NODE &node)
//{
//    return true;
//}
