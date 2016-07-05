#pragma once

#include <deque>
#include "Trajectory.h"

/*
@brief: cubic equation
@author: JoMar
@date: 2016-07-02
@note: S(xi)=Ai+Bi(X-Xi)+Ci(X-Xi)2+Di(X-Xi)3+Ei(X-Xi)4+Fi(X-Xi)5
*/
typedef struct _cub_equ
{
	double A;
	double B;
	double C;
	double D;
    double E;
    double F;
    _cub_equ() : A(0), B(0), C(0), D(0), E(0), F(0){}
    ~_cub_equ(){}
    _cub_equ(const double newA, const double newB, const double newC, const double newD, const double newE, const double newF)
        : A(newA)
        , B(newB)
        , C(newC)
        , D(newD)
        , E(newE)
        , F(newF)
    {}
    _cub_equ(const _cub_equ& object)
        : A(object.A)
        , B(object.B)
        , C(object.C)
        , D(object.D)
        , E(object.E)
        , F(object.F)
    {}
	_cub_equ& operator=(const _cub_equ& object)
	{
		A = object.A;
		B = object.B;
		C = object.C;
		D = object.D;
        E = object.E;
        F = object.F;
		return *this;
	}
}
/*
@brief: cubic equation
@author: JoMar
@date: 2016-07-02
@note: S(xi)=Ai+Bi(X-Xi)+Ci(X-Xi)2+Di(X-Xi)3+Ei(X-Xi)4+Fi(X-Xi)5
*/
CUB_EQU;

/*
@brief: spline node
@author: JoMar
@date: 2016-07-05
*/
typedef struct _sp_node
{
    int timeStamp;
    CUB_EQU spCubEqu;
    NODE_TYPE type;
    _sp_node():timeStamp(0.0), spCubEqu(),type(START_NODE){}
    ~_sp_node(){}
    _sp_node(const int time, const CUB_EQU &cubEqu, const NODE_TYPE &nodeType)
        : timeStamp(time)
        , spCubEqu(cubEqu)
        , type(nodeType){}
    _sp_node(const _sp_node& object)
        : timeStamp(object.timeStamp)
        , spCubEqu(object.spCubEqu)
        , type(object.type){}
    _sp_node& operator=(const _sp_node &object)
    {
        timeStamp = object.timeStamp;
        spCubEqu = object.spCubEqu;
        type = object.type;
        return *this;
    }
}
/*
@brief: spline node
@author: JoMar
@date: 2016-07-05
*/
SP_NODE;

/*
@brief: set spline mode
@author: JoMar
@date: 2016-07-02
*/
enum SPLINE_SECTION_TYPE
{
    STATIC_SEC,
    START_SEC,
    MIDDLE_SEC,
    END_SEC
};

/*
@brief: get data mode
@author: JoMar
@date: 2016-07-02
*/
enum GET_DATA_MODE
{
    SEARCH,             //搜索时间参在整个spline中的位置
    ERGODIC_START,      //用默认指针遍历，并从头开始
    ERGODIC_CONTINUE    //用默认指针遍历
};

/*
@brief: 2 dimensional cubic spline
@author: JoMar
@date: 2016-06-16
*/
typedef class _e2_cub_spline : public Trajectory
{
public:

	_e2_cub_spline();

	virtual ~_e2_cub_spline();

    _e2_cub_spline(const unsigned num,
                   const std::deque<double> &time,
                   const std::deque<double> &data,
                   const SPLINE_SECTION_TYPE &type = STATIC_SEC);

	_e2_cub_spline(const _e2_cub_spline& object);

	_e2_cub_spline& operator=(const _e2_cub_spline& object);


    /*
    @brief: get data by time
    @param[in]: t - time
    @param[out]: pos - position
    @param[out]: vel - velocity
    @param[out]: acc - acceleration
    @author: JoMar
    @date: 2016-07-02
    */
    bool GetDataByTimeStamp(
            const double t,
            double *const pos = nullptr,
            double *const vel = nullptr,
            double *const acc = nullptr,
            GET_DATA_MODE mode = SEARCH);

    /*
    @brief: get first data
    @author: JoMar
    @param[out]: time
    @param[out]: pos - position
    @param[out]: vel - velocity
    @param[out]: acc - acceleration
    @date: 2016-07-02
    */
    bool GetFirstData(
            double *const time = nullptr,
            double *const pos = nullptr,
            double *const vel = nullptr,
            double *const acc = nullptr);

    /*
    @brief: get last data
    @author: JoMar
    @param[out]: time
    @param[out]: pos - position
    @param[out]: vel - velocity
    @param[out]: acc - acceleration
    @date: 2016-07-02
    */
    bool GetLastData(
            double *const time = nullptr,
            double *const pos = nullptr,
            double *const vel = nullptr,
            double *const acc = nullptr);

    /*
    @brief: get last node type quickly
    @author: JoMar
    @date: 2016-07-05
    */
    NODE_TYPE LastNodeType();

    /*
    @brief: get first node time stamp quickly
    @author: JoMar
    @date: 2016-07-05
    */
    int FirstNodeTimeStamp();

    /*
    @brief: get last node time stamp quickly
    @author: JoMar
    @date: 2016-07-05
    */
    int LastNodeTimeStamp();
	
    /*
    @brief: setup the spline profile
    @param[in]: num - number of datas
    @param[in]: pTime - time stamp
    @param[in]: pData
    @param[in]: mode
    @author: JoMar
    @date: 2016-06-27
    */
    bool Create(
            const unsigned num,
            const std::deque<double> &time,
            const std::deque<double> &data,
            const SPLINE_SECTION_TYPE type = STATIC_SEC);



private:

    /*
    @brief: deque of nodes
    @author: JoMar
    @date: 2016-07-02
    */
    std::deque<SP_NODE> m_dequeOfNodes;

    /*
    @brief: the iterator used for ergodic the deque quickly
    @author: JoMar
    @date: 2016-07-02
    */
    std::deque<SP_NODE>::iterator m_iter_ergodic;  

	/*
	@brief: sol Tridiagonal Matrices Parameters
	@author: JoMar
	@date: 2016-06-15
	@note:
    [B0, C0, 0 , 0 , . . . 0   , 0   , 0   , 0   ]
    [A1, B1, C1, 0 , . . . 0   , 0   , 0   , 0   ]
    [0 , A2, B2, C2, . . . 0   , 0   , 0   , 0   ]
    [.   .   .   .         .     .     .     .   ]
    [.   .   .   .         .     .     .     .   ]
    [.   .   .   .         .     .     .     .   ]
    [0 , 0 , 0 , 0 , . . . An-2, Bn-2, Cn-2, 0   ]
    [0 , 0 , 0 , 0 , . . . 0   , An-1, Bn-1, Cn-1]
    [0 , 0 , 0 , 0 , . . . 0   , 0   , An  , Bn  ]
	*/
	void SolTridiagonalMatrices(const unsigned n, const double* A, const double* B, double* C, double *X, double* Y);

    /*
    @brief: Calculate new spline
    @author: JoMar
    @date: 2016-07-02
    */
    bool CalculateSpline(unsigned num, const std::deque<double> &time, const std::deque<double> &data, const SPLINE_SECTION_TYPE &type);

    /*
    @brief: Stitch new spline
    @author: JoMar
    @date: 2016-07-02
    */
    bool StitchSpline(std::deque<SP_NODE>::iterator mix_iter);

//    /*
//    @brief: write datas synchronously
//    @author: JoMar
//    @date: 2016-07-02
//    */
//    inline bool SynWriteData(const SP_NODE &node);
//
//    /*
//    @brief: read datas synchronously
//    @author: JoMar
//    @date: 2016-07-02
//    */
//    inline bool SynReadData(SP_NODE &node);


}
/*
@brief: 2 dimensional cubic spline
@author: JoMar
@date: 2016-06-16
*/
E2_CUB_SPLINE;

