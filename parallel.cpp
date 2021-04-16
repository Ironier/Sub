#include"mpi.h"
#include<vector>
#include<iostream>
#include<cmath>
using namespace std;
class calculator
{
	public:
		vector<vector<float>> matrix;
		vector<float> solution;
		vector<float> value;
		void GaussMethod(int argc,char *argv[])
		{
			MPI_Status status;
			float **A=new float*[matrix.size()];
			for(int i=0;i<matrix.size();i++)
				A[i]=new float[matrix.size()];
			for(int i=0;i<matrix.size();i++)
				for(int j=0;j<matrix.size();j++)
					A[i][j]=matrix.at(i).at(j);
			int self,size;
                        MPI_Status s;  
		        MPI_Init(&argc,&argv);
                        MPI_Comm_rank(MPI_COMM_WORLD,&self);  
                        MPI_Comm_size(MPI_COMM_WORLD,&size);
			float *B=new float[matrix.size()];
			float *f=new float[matrix.size()];
			for(int i=0;i<matrix.size();i++)
				B[i]=value.at(i);
			int m=matrix.size()/size;
			if(m!=0)
				m++;
			float **a=new float*[matrix.size()];
			for(int i=0;i<matrix.size();i++)
				a[i]=new float[m];
			for(int i=0;i<matrix.size();i++)
			{
			if(self==0)
			{
					for(int j=0;j<matrix.size();j++)
					{
						if(j%m!=0)
						{
							int i1=j%m;
							int i2=j/m+1;
							MPI_Send(&A[i][j],1,MPI_FLOAT,i1,i2,MPI_COMM_WORLD);
						}
					}
			}
			else
			{
				for(int j=0;j<m;j++)
					MPI_Recv(&a[i][j],1,MPI_FLOAT,0,j+1,MPI_COMM_WORLD,&status);
			}
			}
			for(int i=0;i<m;i++)
				for(int j=0;j<size;j++)
				{
					int v;
					int l;
					int ratio;
					if(self==j)
					{
						 v=i*size+j;
						 if(v>=size)
							 break;
						 int Imax=a[v][i];
						 l=v;
						 for(int k=v+1;k<matrix.size();k++)
						{
							if(fabs(a[k][i])>Imax)
							{
								Imax=a[k][i];
								l=k;
							}
						}
						 if(l!=v)
						 {
							 for(int t=0;t<m;t++)
							 {
								 int temp=a[v][t];
								 a[v][t]=a[l][t];
								 a[l][t]=temp;
							 }
						 }
						 for(int k=i+1;k<m;k++)
							 a[v][k]/=a[v][i];
						 a[v][i]=1;
						 B[v]/=a[v][i];
						 for(int k=v+1;k<matrix.size();k++)
						 {
							 f[k]=a[k][i];
							 B[k]-=a[k][i]*B[v];
							 a[k][i]=0;
						 }
						 for(int k=i+1;k<m;k++)
						 {
							for(int w=v+1;w<matrix.size();w++)
								a[w][k]-=f[w]*a[v][k];
						 }
						 MPI_Bcast(&a[v][i],1,MPI_FLOAT,self,MPI_COMM_WORLD);
						 MPI_Bcast(f,matrix.size(),MPI_FLOAT,self,MPI_COMM_WORLD);
						 MPI_Bcast(&l,1,MPI_INT,self,MPI_COMM_WORLD);
					}
					else
					{
						v=i*size+j;
						if(v>=size)
							break;
						MPI_Bcast(&ratio,1,MPI_FLOAT,j,MPI_COMM_WORLD);
                                                MPI_Bcast(f,matrix.size(),MPI_FLOAT,j,MPI_COMM_WORLD);
                                                MPI_Bcast(&l,1,MPI_INT,j,MPI_COMM_WORLD);
						if(l!=v)
                                                 {
                                                         for(int t=0;t<m;t++)
                                                         {
                                                                 int temp=a[v][t];
                                                                 a[v][t]=a[l][t];
                                                                 a[l][t]=temp;
                                                         }

                                                 }
						if(self<j)
						{
							for(int k=i+1;k<m;k++)
							{
								a[v][k]/=ratio;
								for(int w=v+1;w<matrix.size();w++)
								{
									a[w][k]-=f[w]*a[v][k];
								}
							}
						}
						if(self>j)
                                                {
                                                        for(int k=i;k<m;k++)
                                                        {
                                                                a[v][k]/=ratio;
                                                                for(int w=v+1;w<matrix.size();w++)
                                                                {
                                                                        a[w][k]-=f[w]*a[v][k];
                                                                }
                                                        }
                                                }

					}
				}
			for(int i=m-1;i>=0;i--)
				for(int j=size-1;j>=0;j--)
				{
						int v=i*size+j;
						if(v>=size)
							continue;
						for(int k=v-1;k>=0;k++)
						{
							B[k]-=a[k][i]*B[v];
							a[k][i]=0;
						}
				}
			if(self==0)
			{
				for(int i=0;i<matrix.size();i++)
				solution.push_back(B[i]);
			
			}
		MPI_Finalize();
		delete A;
                delete a;
                delete f;
		delete B;


		}
};
int main(int argc,char **argv)
{
	return 0;
}
