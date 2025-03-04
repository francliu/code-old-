/*************************************************************************
> File Name: matrix.h
> Author: 刘建飞
> Mail: liujianfei526@163.com
> Created Time: 2015年04月06日 星期一 23时09分51秒
************************************************************************/
#include<iostream>
#include<vector>
#include<map>
#define N 10
using namespace std;
class matrix
{
public:
	//功能: 采用部分主元的高斯消去法求方阵A的逆矩阵B
	//入口参数: 输入方阵，输出方阵，方阵阶数
	bool Gauss(float A[][N], float B[][N], int n)
	{
		int i, j, k;
		float max, temp;
		float t[N][N];                //临时矩阵
		//将A矩阵存放在临时矩阵t[n][n]中
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				t[i][j] = A[i][j];
			}
		}
		//初始化B矩阵为单位阵
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				B[i][j] = (i == j) ? (float)1 : 0;
			}
		}
		for (i = 0; i < n; i++)
		{
			//寻找主元
			max = t[i][i];
			k = i;
			for (j = i + 1; j < n; j++)
			{
				if (fabs(t[j][i]) > fabs(max))
				{
					max = t[j][i];
					k = j;
				}
			}
			//如果主元所在行不是第i行，进行行交换
			if (k != i)
			{
				for (j = 0; j < n; j++)
				{
					temp = t[i][j];
					t[i][j] = t[k][j];
					t[k][j] = temp;
					//B伴随交换
					temp = B[i][j];
					B[i][j] = B[k][j];
					B[k][j] = temp;
				}
			}
			//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
			if (t[i][i] == 0)
			{
				cout << "There is no inverse matrix!";
				return false;
			}
			//消去A的第i列除去i行以外的各行元素
			temp = t[i][i];
			for (j = 0; j < n; j++)
			{
				t[i][j] = t[i][j] / temp;        //主对角线上的元素变为1
				B[i][j] = B[i][j] / temp;        //伴随计算
			}
			for (j = 0; j < n; j++)        //第0行->第n行
			{
				if (j != i)                //不是第i行
				{
					temp = t[j][i];
					for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素
					{
						t[j][k] = t[j][k] - t[i][k] * temp;
						B[j][k] = B[j][k] - B[i][k] * temp;
					}
				}
			}
		}
		return true;
	}
	void print(float p[N][N], int n)
	{
		int i, j;
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				printf("%.1f ", p[i][j]);
			}
		}
	}
	void dialog(float D[N][N], float Node[N])
	{

		//初始化D矩阵
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				D[i][j] = (i == j) ? (float)(Node[i]) : 0;
			}
		}
	}
	//矩阵相乘
	void muti_matric(float temp[N][N], float invD[N][10], float vertex[N][N])
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				float num = 0;
				for (int k = 0; k < N; k++)
				{
					num += vertex[i][k] * invD[k][j];
				}
				temp[i][j] = num;
				//cout<<temp[i][j]<<" ";
			}
			//cout<<endl;
		}
	}
	/**
	* @brief 求实对称矩阵的特征值及特征向量的雅克比法
	* 利用雅格比(Jacobi)方法求实对称矩阵的全部特征值及特征向量
	* @param pMatrix				长度为n*n的数组，存放实对称矩阵
	* @param nDim					矩阵的阶数
	* @param pdblVects				长度为n*n的数组，返回特征向量(按列存储)
	* @param dbEps					精度要求
	* @param nJt					整型变量，控制最大迭代次数
	* @param pdbEigenValues			特征值数组
	* @return
	*/
	bool JacbiCor(double * pMatrix, int nDim, double *pdblVects, double *pdbEigenValues, double dbEps, int nJt)
	{
		for (int i = 0; i < nDim; i++)
		{
			pdblVects[i*nDim + i] = 1.0f;
			for (int j = 0; j < nDim; j++)
			{
				if (i != j)
					pdblVects[i*nDim + j] = 0.0f;
			}
		}

		int nCount = 0;		//迭代次数
		while (1)
		{
			//在pMatrix的非对角线上找到最大元素
			double dbMax = pMatrix[1];
			int nRow = 0;
			int nCol = 1;
			for (int i = 0; i < nDim; i++)			//行
			{
				for (int j = 0; j < nDim; j++)		//列
				{
					double d = fabs(pMatrix[i*nDim + j]);

					if ((i != j) && (d> dbMax))
					{
						dbMax = d;
						nRow = i;
						nCol = j;
					}
				}
			}

			if (dbMax < dbEps)     //精度符合要求
				break;

			if (nCount > nJt)       //迭代次数超过限制
				break;

			nCount++;

			double dbApp = pMatrix[nRow*nDim + nRow];
			double dbApq = pMatrix[nRow*nDim + nCol];
			double dbAqq = pMatrix[nCol*nDim + nCol];

			//计算旋转角度
			double dbAngle = 0.5*atan2(-2 * dbApq, dbAqq - dbApp);
			double dbSinTheta = sin(dbAngle);
			double dbCosTheta = cos(dbAngle);
			double dbSin2Theta = sin(2 * dbAngle);
			double dbCos2Theta = cos(2 * dbAngle);

			pMatrix[nRow*nDim + nRow] = dbApp*dbCosTheta*dbCosTheta +
				dbAqq*dbSinTheta*dbSinTheta + 2 * dbApq*dbCosTheta*dbSinTheta;
			pMatrix[nCol*nDim + nCol] = dbApp*dbSinTheta*dbSinTheta +
				dbAqq*dbCosTheta*dbCosTheta - 2 * dbApq*dbCosTheta*dbSinTheta;
			pMatrix[nRow*nDim + nCol] = 0.5*(dbAqq - dbApp)*dbSin2Theta + dbApq*dbCos2Theta;
			pMatrix[nCol*nDim + nRow] = pMatrix[nRow*nDim + nCol];

			for (int i = 0; i < nDim; i++)
			{
				if ((i != nCol) && (i != nRow))
				{
					int u = i*nDim + nRow;	//p
					int w = i*nDim + nCol;	//q
					dbMax = pMatrix[u];
					pMatrix[u] = pMatrix[w] * dbSinTheta + dbMax*dbCosTheta;
					pMatrix[w] = pMatrix[w] * dbCosTheta - dbMax*dbSinTheta;
				}
			}

			for (int j = 0; j < nDim; j++)
			{
				if ((j != nCol) && (j != nRow))
				{
					int u = nRow*nDim + j;	//p
					int w = nCol*nDim + j;	//q
					dbMax = pMatrix[u];
					pMatrix[u] = pMatrix[w] * dbSinTheta + dbMax*dbCosTheta;
					pMatrix[w] = pMatrix[w] * dbCosTheta - dbMax*dbSinTheta;
				}
			}

			//计算特征向量
			for (int i = 0; i < nDim; i++)
			{
				int u = i*nDim + nRow;		//p
				int w = i*nDim + nCol;		//q
				dbMax = pdblVects[u];
				pdblVects[u] = pdblVects[w] * dbSinTheta + dbMax*dbCosTheta;
				pdblVects[w] = pdblVects[w] * dbCosTheta - dbMax*dbSinTheta;
			}

		}

		//对特征值进行排序以及重新排列特征向量,特征值即pMatrix主对角线上的元素
		std::map<double, int> mapEigen;
		for (int i = 0; i < nDim; i++)
		{
			pdbEigenValues[i] = pMatrix[i*nDim + i];

			mapEigen.insert(make_pair(pdbEigenValues[i], i));
		}

		double *pdbTmpVec = new double[nDim*nDim];
		std::map<double, int>::reverse_iterator iter = mapEigen.rbegin();
		for (int j = 0; iter != mapEigen.rend(), j < nDim; ++iter, ++j)
		{
			for (int i = 0; i < nDim; i++)
			{
				pdbTmpVec[i*nDim + j] = pdblVects[i*nDim + iter->second];
			}

			//特征值重新排列
			pdbEigenValues[j] = iter->first;
		}

		//设定正负号
		for (int i = 0; i < nDim; i++)
		{
			double dSumVec = 0;
			for (int j = 0; j < nDim; j++)
				dSumVec += pdbTmpVec[j * nDim + i];
			if (dSumVec<0)
			{
				for (int j = 0; j < nDim; j++)
					pdbTmpVec[j * nDim + i] *= -1;
			}
		}

		memcpy(pdblVects, pdbTmpVec, sizeof(double)*nDim*nDim);
		delete[]pdbTmpVec;

		return 1;
	}
	void mutex(float I[N][N], float vertex[N][N], float a, int n)
	{

		//初始化I矩阵为单位阵
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				I[i][j] = (i == j) ? (float)1 : 0;
			}
		}
		//矩阵相减
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				I[i][j] = I[i][j] - a*vertex[i][j];
			}
		}
		//print(I,n);
	}
};
