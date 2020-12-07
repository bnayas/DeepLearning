#pragma once
#include "pch.h"
#include "Layer.h"
#include "ConvLayer.h"

class NeuralNet
{
public:
	NeuralNet();
	~NeuralNet();

	std::vector<Layer*> layers;
	unsigned int initialDataSize[3];
	float* initialData;
	void SetInitialSize(unsigned int x, unsigned int y, unsigned int nc);
	void SetData(float data[], unsigned int m);
	void WriteFilter(unsigned int layer, float* data, unsigned int filter_num = 0);
	void WriteFilters(unsigned int layer, float* data, unsigned int filter_num, unsigned int filter_start = 0);
	void AddConvLayer(unsigned int f, unsigned int pad, unsigned int stride, unsigned int num_of_filters);
	float* Get(const std::string, unsigned int layer);
	void ForwardPropagation(float data[], unsigned int m);
//private:
	unsigned int current_m;
	unsigned int m_currentSize[3];
	unsigned int m_currentBind;
};

