#version 430 core


layout(local_size_x = 6, local_size_y = 2) in;
layout(std430, binding = 6) buffer shader_data
{
	float W[];
};
layout(std430, binding = 5) buffer shader_output
{
	float A[];
};

//uniform mat3 f;

void main()
{

	A[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_WorkGroupSize.x] = 3 + W[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_WorkGroupSize.x];
	W[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_WorkGroupSize.x] += 1;
}