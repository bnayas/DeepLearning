#version 430 core


layout(local_size_x = 3, local_size_y = 3, local_size_z = 2) in;
 
layout(std430, binding = 0) buffer data1
{
	float A[][4][4];
};
layout(std430, binding = 5) buffer shader_output
{
	float  X[][gl_WorkGroupSize.z][gl_WorkGroupSize.x][gl_WorkGroupSize.y];
};
layout(std430, binding = 3) buffer filters
{
	float f[gl_WorkGroupSize.z][2][2];
};

uniform uint stride;

void main()
{
	uint y0 = gl_LocalInvocationID.y * stride;
	uint x0 = gl_LocalInvocationID.x * stride ;
	float temp = 0;
	for (uint x = 0; x < 2; x++)
		for (uint y = 0; y < 2; y++)
			temp += A[gl_WorkGroupID.x][y0 + y][x0 + x] * f[gl_LocalInvocationID.z][x][y];				
	X[gl_WorkGroupID.x][gl_LocalInvocationID.z][gl_LocalInvocationID.y][gl_LocalInvocationID.x] =
		temp;

}

