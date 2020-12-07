#pragma once
#include "pch.h"
#include "Layer.h"


class ConvLayer :
    public Layer
{
public:
	ConvLayer(unsigned int inSize, unsigned int filter_size, unsigned int filter_pad, unsigned int filter_stride, unsigned int n_c,
		unsigned int inBind, unsigned int outBind, unsigned int filterBind = 9);
	ConvLayer(unsigned int inSize, unsigned int filter_size, unsigned int filter_pad, unsigned int filter_stride, unsigned int n_c, unsigned int filterBind = 9);
	~ConvLayer();
	void CreateShaders() override;
	void Bind();
	// void AddFilter(float* X);
	void setFiltersNumber(unsigned int num);
	std::vector<float *> filters;
	unsigned int numOfFilters;
	unsigned int f;
	unsigned int pad;
	unsigned int stride;
	unsigned int nc;
	unsigned int m_FiltersBinding;
	unsigned int input_size;
	bool initialLayer;
	Activation activation;
};

