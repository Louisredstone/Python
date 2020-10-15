#include "LeNet.h"
#include <stdlib.h>
#include <iomanip>
/*
float expf(float x) {
 x = 1.0 + x / 1024;
 x *= x; x *= x; x *= x; x *= x;
 x *= x; x *= x; x *= x; x *= x;
 x *= x; x *= x;
 return x;
}
*/
float Conv_5x5(float input[25], float kernel[25]){
	int x,y;
	float result = 0;
	for(y = 0; y < 5; y++){
		for(x = 0; x < 5; x++){
			result += input[x+y*5] * kernel[x+y*5];
		}
	}
	return result;
}

//kernel 5x5x6 = 25x6 = 150
void ConvLayer_1(float input[1024],float * C1_value,float * weights){
	int i_y,i_x,matrix_y,matrix_x;
	int k_num,mat_i = 0;
	top_loop:for(int k_num = 0; k_num < 6; k_num+=1){
		//TODO 内存kernel
		float matrix_2[25];
		for(mat_i = 0;mat_i<25;mat_i++){
			matrix_2[mat_i] = weights[mat_i + k_num*25];
		}
		i_y_loop:for(i_y = 0; i_y < 28; i_y++){
			for(i_x = 0; i_x < 28; i_x++){
				float matrix[25];
				int pic_value_index = i_x + i_y * 32;  //the top left position of a kernel in input matrix  that mapped from the output kernel
				matrix_loop:for(matrix_y = 0; matrix_y <5; matrix_y++){
					caculate:for(matrix_x = 0; matrix_x <5; matrix_x++){
//						图片索引  0 ~ 24  the pixel position of convolution kernel
						int matrix_index = matrix_x + matrix_y * 5;
//						图片像素索引 0 ~ 1024,与matrix_x,matrix_y相关,x、y=32
						int input_value_index = pic_value_index + matrix_x + matrix_y * 32;
						matrix[matrix_index] = input[input_value_index];    //to restore the pixels that generate the output pixel matrix from input pixels
					}
				}
				int out_pic_index = i_x + i_y * 28 + k_num * 784;
				C1_value[out_pic_index] = Conv_5x5(matrix,matrix_2);   // matrix is the convolution pixels from input array, matrix_2 is one of the kernel from six dimension kernels
			}
		}
	}
}

float AvgPool_2x2(float input[4]){
	float res = 0;
	int i;
	for(i = 0; i < 4 ; i++){
		res += input[i];
	}
	res /= 4;
	return res;
}
/*float sigmoid(float x){
#pragma HLS INLINE
	float exp2x = expf(2*x)+1;
	//return (exp2x-2)/(exp2x);
	//cout<<"x is %f %f"<<x<<expf(x)<<'\n';
	return (expf(2*x)-1)/(expf(2*x)+1);
}*/
float sigmoid(float x)
{
	if(x>=0)
		return x;
	else
		return 0;
}

void AvgpoolLayer_2(float input[4704],float *A2_value){   // the width of a2_value is 1176
	int k_num,i_y,i_x,matrix_x,matrix_y;
	int count = 0;
	for(k_num = 0; k_num < 6; k_num++){
		for(i_y = 0; i_y < 27; i_y+=2){
			for(i_x = 0;  i_x < 27; i_x+=2){
				float matrix[4];   //the kernel that is being  pooling  right now
				int index_now = i_x + i_y * 28 + k_num * 784;
				for(matrix_y = 0; matrix_y < 2; matrix_y++){
					for(matrix_x = 0; matrix_x < 2; matrix_x++){
						int input_index = index_now + matrix_x + matrix_y * 28 ;
						matrix[matrix_x + matrix_y*2] = input[input_index];
						//cout<<"input is "<<input[input_index];
					}
				}
				A2_value[count] = sigmoid(AvgPool_2x2(matrix));
				//cout<<"avgpool is "<<matrix[0]<<matrix[1]<<matrix[2]<<matrix[3]<<'\n';
				//A2_value[count] = AvgPool_2x2(matrix);
				count++;  // the max of count is 14*14*6 = 1176
			}
		}
	}
}

//kernel 5x5x6x16 = 25x6x16 =2400
void ConvLayer_3(float input[1176],float *C3_value,float * weights){   //weights is 5*5*16 , input is 14*14*6
	int k_num,nk_num,i_y,i_x,matrix_x,matrix_y;
	int mat_i;
    for(nk_num = 0; nk_num < 16; nk_num++){  //the third dimension of C3_value
		for(i_y = 0; i_y < 10; i_y++){		 //the second dimension of C3_value
			for(i_x = 0; i_x < 10; i_x++){	  //the first dimension of C3_value
				float res = 0;
				float res_total_6 = 0;
				float matrix[25];
				int index_now = i_x + i_y * 10 + nk_num * 100;  //the index of output C3_value right now
				for(k_num = 0; k_num < 6; k_num++){  //the number of kernel page
					int index_now_input = i_x + i_y * 14 + k_num * 196;
					float matrix_2[25]; //the kernel pixels right now
					for(mat_i = 0;mat_i<25;mat_i++){
						int weights_index = mat_i + k_num*25 + (nk_num)*150;
						matrix_2[mat_i] = weights[weights_index + 150];
					}
					for(matrix_y = 0; matrix_y <5; matrix_y++){
						for(matrix_x = 0; matrix_x <5; matrix_x++){
							int matrix_index = matrix_x + matrix_y * 5;
							int input_value_index = index_now_input + matrix_x + matrix_y * 14;
							matrix[matrix_index] = input[input_value_index];
						}
					}
					res_total_6 += Conv_5x5(matrix,matrix_2);
				}
				C3_value[index_now] = res_total_6;
			}
		}
	}
}

void AvgpoolLayer_4(float input[1600],float *A4_value){
	int k_num,i_y,i_x,matrix_x,matrix_y;
	int count = 0;
	for(k_num = 0; k_num < 16; k_num++){
		for(i_y = 0; i_y < 10; i_y+=2){
			for(i_x = 0;  i_x < 10; i_x+=2){
				float matrix[4];
				int index_now = i_x + i_y * 10 + k_num * 100;
				for(matrix_y = 0; matrix_y < 2; matrix_y++){
					for(matrix_x = 0; matrix_x < 2; matrix_x++){
						int input_index = index_now + matrix_x + matrix_y * 10 ;
						matrix[matrix_x + matrix_y*2] = input[input_index];
					}
				}
				A4_value[count] = sigmoid(AvgPool_2x2(matrix));
				count++;
			}
		}
	}
}

//kernel 400x120 = 48000
void FullyConnLayer_5(float input[400],float *F5_value,float * weights){  //weights is 400*200 matrix, F5_value is 1*120 matrix  , weights constant of the weights of con1 conv3 conv5 con6
	int i_y,i_x;
	for(i_y = 0; i_y < 120; i_y++){
		float res = 0;
		for(i_x = 0;  i_x < 400; i_x++){
			int index = i_x + i_y * 120;
			res += input[i_x] * weights[index + 2550];
		}
		F5_value[i_y] = res;
	}
}
//kernel 84x120 = 10080
void FullyConnLayer_6(float input[120],float *F6_value,float * weights){
	int i_y,i_x;
	for(i_y = 0; i_y < 84; i_y++){
		float res = 0;
		for(i_x = 0;  i_x < 120; i_x++){
			int index = i_x + i_y * 120;
			res += input[i_x] * weights[index + 50550];
		}
		F6_value[i_y] = res;
	}
}

//kernel 10x84 = 840
void FullyConnLayer_7(float input[84],float *F6_value,float * weights){
	int i_y,i_x;
	for(i_y = 0; i_y < 10; i_y++){
		float res = 0;
		for(i_x = 0;  i_x < 84; i_x++){
			int index = i_x*10 + i_y;
			res += input[i_x] * weights[index + 60630];
			//res = weights[index + 60630];
			//res = 0;
		}
		F6_value[i_y] = res;
	}
}

int Softmax_1_8(float input[10]){
	int index;
	int max_index=0;
	for(index=0;index<10;index++)
	{
		if(input[index]>input[max_index])
			max_index=index;
	}
	return max_index;
}


void LeNet(volatile float *addrMaster,int* r){
#pragma HLS INTERFACE m_axi depth=62494 port=addrMaster offset=slave bundle=MASTER_BUS
#pragma HLS INTERFACE s_axilite port=r bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	// 32x32 iamge
	float photo[1024];
	//layer1 weights  5x5x6 = 25x6 = 150
	//layer3 weights  5x5x6x16 = 25x6x16 =2400
	//layer5 weights 400x120 = 48000
	//layer6 weights 84x120 = 10080
	//layer7 weights 10x120 = 840
	float data[62494];
	//The output of each layer
	float C1_value[4704];
	float A2_value[1176];
	float C3_value[1600];
	float A4_value[400];
	float F5_value[120];
	float F6_value[84];
	float F7_value[10];
	float f7;
	int loop1_i;
	//memory copy from BRAM to FPGA's RAM
	memcpy(data,(const float*)addrMaster,62494*sizeof(float));
	//get the image data
	for(loop1_i = 0; loop1_i<1024; loop1_i++){
		photo[loop1_i] = data[loop1_i+61470];
	}
	//calulation of each layer

	ConvLayer_1(photo,C1_value,data);
	AvgpoolLayer_2(C1_value,A2_value);
	ConvLayer_3(A2_value,C3_value,data);
	AvgpoolLayer_4(C3_value,A4_value);
	FullyConnLayer_5(A4_value,F5_value,data);
	FullyConnLayer_6(F5_value,F6_value,data);
	FullyConnLayer_7(F6_value,F7_value,data);
	//for(int i=0;i<4704;i++)
	//	cout<<"c1 value is"<<C1_value[i]<<'\n';
	*r = Softmax_1_8(F7_value);

}
