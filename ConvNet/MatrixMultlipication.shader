#version 430 core


layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 0) buffer data1
{
	float A[];
};
layout(std430, binding = 1) buffer data2
{
	float B[];
};
layout(std430, binding = 5) buffer shader_output
{
	float X[];
};
layout(std430, binding = 6) buffer shader_output2
{
	float X2[];
};

//uniform mat3 f;

void main()
{
	
	uint i = gl_WorkGroupID.y;
	uint j = gl_WorkGroupID.x;
	uint d = gl_NumWorkGroups.x;
	float temp = 0;
	for (uint k = 0; k < A.d; k++)
	{
		temp += A[k + i * d]* B[j + k * d];
	}
			X[j + i * d] = temp;
	
}

