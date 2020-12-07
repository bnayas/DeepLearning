#include "pch.h"
#include "ConvLayer.h"


//void ConvLayer::AddFilter(float* X)
//{
//	float* ptr = new float[sizeof(X)];
//	for (int i = 0; i < sizeof(X); i++)
//		*(ptr+i) = *(X+i);
//	filters.push_back(ptr);
//}

void ConvLayer::setFiltersNumber(unsigned int num) 
{
	numOfFilters = num;
}

ConvLayer::ConvLayer(unsigned int inSize, unsigned int filter_size, unsigned int filter_pad, unsigned int filter_stride, unsigned int n_c,
	unsigned int inBind, unsigned int outBind, unsigned int filterBind) :
	Layer(inBind,outBind), f(filter_size), pad(filter_pad), stride(filter_stride), nc(n_c), m_FiltersBinding(filterBind), input_size(inSize), activation(None)
{

}

ConvLayer::ConvLayer(unsigned int inSize, unsigned int filter_size, unsigned int filter_pad, unsigned int filter_stride, unsigned int n_c, unsigned int filterBind) :
	Layer(), f(filter_size), pad(filter_pad), stride(filter_stride), nc(n_c), m_FiltersBinding(filterBind), input_size(inSize), activation(None)
{

}

ConvLayer::~ConvLayer()
{
	for (auto p : filters)
	{
		delete[] p;
	}
	filters.clear();

}

void ConvLayer::CreateShaders()
{
	std::string inSize = std::to_string(input_size);
	std::string outSize = std::to_string(int(1 + ((input_size + 2 * pad - f) / stride)));
	std::string inBind = std::to_string(m_InputBinding);
	std::string outBind = std::to_string(m_OutputBinding);
	std::string filterBind = std::to_string(m_FiltersBinding);
//	std::string filNum = std::to_string(filters.size());
	std::string filNum = std::to_string(numOfFilters);
	fwd_source_code = "#version ";
	fwd_source_code += std::to_string(OpenGL_MajorVersion) + std::to_string(OpenGL_MinorVersion) + "0 core \n";
	fwd_source_code += "\n";
	fwd_source_code += "layout(local_size_x = " + outSize + ", local_size_y = "
		+ outSize + ", local_size_z = " + filNum + ") in;\n";
	fwd_source_code += "\n";
	fwd_source_code += "layout(std430, binding = " + inBind +
		") buffer input_var\n";
	fwd_source_code += "{\n";
	fwd_source_code += "float A_prev[]["+std::to_string(nc)+"][" + inSize + "][" + inSize + "]; ";
	fwd_source_code += "};\n";
	fwd_source_code += "\n";
//	fwd_source_code += "layout(std430, binding = 1) buffer result\n";
//	fwd_source_code += "{\n";
//	fwd_source_code += "float Z[" + outSize + "]; ";
//	fwd_source_code += "};\n";
	fwd_source_code += "layout(std430, binding = " + outBind + ") buffer result_pass\n";
	fwd_source_code += "{\n";
	fwd_source_code += "float A[][" + filNum + "][" + outSize + "][" + outSize + "]; ";
	fwd_source_code += "};\n";
	fwd_source_code += "layout(std430, binding ="+ filterBind + ") buffer filters\n";
	fwd_source_code += "{\n";
	fwd_source_code += "float f[" + filNum + "][" + std::to_string(nc) + "][" + std::to_string(f) + "][" + std::to_string(f) + "]; ";
	fwd_source_code += "};\n";
	fwd_source_code += "\n\n";

	fwd_source_code += "void main()\n";
	fwd_source_code += "{\n";
	fwd_source_code += "uint y0 = gl_LocalInvocationID.y * " +
		std::to_string(stride) + ";\n";
	fwd_source_code += "uint x0 = gl_LocalInvocationID.x * " + std::to_string(stride) + ";\n";
	fwd_source_code += "float temp = 0;\n";
	fwd_source_code += "for (uint x = 0; x < " + std::to_string(f) + "; x++)   \n";
	fwd_source_code += "for (uint y = 0; y < " + std::to_string(f) + "; y++)   \n";
	fwd_source_code += "for (uint z = 0; z < " + std::to_string(nc) + "; z++)   \n";
	fwd_source_code += "				temp += A_prev[gl_WorkGroupID.x][z][y + y0][x + x0] * f[gl_LocalInvocationID.z][z][y][x];\n";
	fwd_source_code += " \n";
	// source_code += "	}\n";
	if (activation == None)
	fwd_source_code += "		A[gl_WorkGroupID.x][gl_LocalInvocationID.z][gl_LocalInvocationID.y][gl_LocalInvocationID.x] = temp; \n";
	else if (activation == Relu)
		fwd_source_code += "		A[gl_WorkGroupID.x][gl_LocalInvocationID.z][gl_LocalInvocationID.y][gl_LocalInvocationID.x] = max(temp,0); \n";
	fwd_source_code += " } \n";

	fwd_prog = new Shader(fwd_source_code, true, true);
	
	//bck_source_code = "#version ";
	//bck_source_code += std::to_string(OpenGL_MajorVersion) + std::to_string(OpenGL_MinorVersion) + "0 core \n";
	//bck_source_code += "\n";
	//bck_source_code += "layout(local_size_x = " + outSize + ", local_size_y = "
	//+ outSize + ", local_size_z = " + filNum + ") in;\n";
	//bck_source_code += "\n";
	//bck_source_code += "layout(std430, binding = " + inBind +
	//") buffer input_var\n";
	//bck_source_code += "{\n";
	//bck_source_code += "float A_prev[][" + std::to_string(nc) + "][" + inSize + "][" + inSize + "]; ";
	//bck_source_code += "};\n";
	//bck_source_code += "\n";
	//bck_source_code += "layout(std430, binding = 1) buffer result\n";
	//bck_source_code += "{\n";
	//bck_source_code += "float Z[" + outSize + "]; ";
	//bck_source_code += "};\n";
	//bck_source_code += "layout(std430, binding = " + outBind + ") buffer result_pass\n";
	//bck_source_code += "{\n";
	//bck_source_code += "float A[][" + filNum + "][" + outSize + "][" + outSize + "]; ";
	//bck_source_code += "};\n";
	//bck_source_code += "layout(std430, binding =" + filterBind + ") buffer filters\n";
	//bck_source_code += "{\n";
	//bck_source_code += "float f[" + filNum + "][" + std::to_string(nc) + "][" + std::to_string(f) + "][" + std::to_string(f) + "]; ";
	//bck_source_code += "};\n";
	//bck_source_code += "\n\n";
	//
	//bck_source_code += "void main()\n";
	//bck_source_code += "{\n";
	//bck_source_code += "uint y0 = gl_LocalInvocationID.y * " +
	//std::to_string(stride) + ";\n";
	//bck_source_code += "uint x0 = gl_LocalInvocationID.x * " + std::to_string(stride) + ";\n";
	//bck_source_code += "float temp = 0;\n";
	//bck_source_code += "for (uint x = 0; x < " + std::to_string(f) + "; x++)   \n";
	//bck_source_code += "for (uint y = 0; y < " + std::to_string(f) + "; y++)   \n";
	//bck_source_code += "for (uint z = 0; z < " + std::to_string(nc) + "; z++)   \n";
	//bck_source_code += "				temp += A_prev[gl_WorkGroupID.x][z][y + y0][x + x0] * f[gl_LocalInvocationID.z][z][y][x];\n";
	//bck_source_code += " \n";
	//bck_source_code += "	}\n";
	//bck_source_code += "		A[gl_WorkGroupID.x][gl_LocalInvocationID.z][gl_LocalInvocationID.y][gl_LocalInvocationID.x] = temp; \n";
	//bck_source_code += " } \n";

	//bck_prog = new Shader(bck_source_code, true, true);

}

void ConvLayer::Bind()
{
	//const std::string new_name = "A_prev";
	//prog->SetBuffer<float[32]>(new_name, &A, 0);
	//prog->WriteBuffer("A_prev", A);

	//prog->Bind();
	//const std::string filter_name = "f";
	//prog->SetBuffer(filter_name, sizeof(float) * filters.size() * (f * f), m_FiltersBinding);
	//unsigned offset = 0;
	//for (auto v : filters)
	//{
	//	prog->WriteBuffer("f", v, offset);
	//	offset += sizeof(v);
	//}
	
}
