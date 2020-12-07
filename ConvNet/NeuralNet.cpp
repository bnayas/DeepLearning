#include "pch.h"
#include "NeuralNet.h"


NeuralNet::NeuralNet() :
	m_currentBind(0)
{

}

NeuralNet::~NeuralNet()
{
	for (auto p : layers)
	{
		
		delete p;
	}
	layers.clear();
}


void NeuralNet::SetInitialSize(unsigned int x, unsigned int y, unsigned int nc)
{
	m_currentSize[0] = x;
	m_currentSize[1] = y ;
	m_currentSize[2] = nc;
	initialDataSize[0] = x;
	initialDataSize[1] = y;
	initialDataSize[2] = nc;
}

void NeuralNet::AddConvLayer(unsigned int f, unsigned int pad, unsigned int stride, unsigned int num_of_filters)
{
	ConvLayer* layer = new ConvLayer(m_currentSize[0], f, pad, stride, m_currentSize[2], m_currentBind, m_currentBind + 5, m_currentBind + 1);
	layer->numOfFilters = num_of_filters;
	layer->CreateShaders();
	layer->fwd_prog->Bind();
	const std::string filter_name = "f";
	layer->fwd_prog->SetBuffer(filter_name, (f*f* m_currentSize[2] *num_of_filters) * sizeof(float), m_currentBind + 1);
	layers.push_back(layer);
	m_currentBind += 5;
	m_currentSize[0] = 1 + floor((m_currentSize[0] + 2 * layer->pad - layer->f) / layer->stride);
	m_currentSize[1] = 1 + floor((m_currentSize[1] + 2 * layer->pad - layer->f) / layer->stride);
	m_currentSize[2] = layer->numOfFilters;

}

void NeuralNet::SetData(float data[], unsigned int m)
{
	layers[0]->fwd_prog->Bind();
	layers[0]->fwd_prog->SetBuffer("A_prev",
		m * initialDataSize[0] * initialDataSize[1] *
		initialDataSize[2]*sizeof(float), 0);
	//layers[0]->prog->WriteBuffer("A_prev", data, m * initialDataSize[0] * initialDataSize[1] *
	//	initialDataSize[2] * sizeof(float), 0);
	layers[0]->fwd_prog->WriteBuffer<float>("A_prev", data, m * initialDataSize[0] * initialDataSize[1] *
		initialDataSize[2] *sizeof(float) , 0);
	current_m = m;
	ConvLayer* l = (ConvLayer*)layers[0];
	m_currentSize[0] = 1 + floor((initialDataSize[0] + 2 * l->pad - l->f) / l->stride);
	m_currentSize[1] = 1 + floor((initialDataSize[1] + 2 * l->pad - l->f) / l->stride);
	m_currentSize[2] = l->numOfFilters;
	l->fwd_prog->SetBuffer("A", (m_currentSize[0] * m_currentSize[1] * m_currentSize[2] * m) * sizeof(float), l->OutBind());

	for (unsigned int i = 1; i < layers.size(); i++)
	{
		ConvLayer* l = (ConvLayer*)layers[i];
		l->fwd_prog->Bind();
		l->fwd_prog->LinkBuffer("A_prev", "A", ((ConvLayer*)layers[i-1])->fwd_prog);
		m_currentSize[0] = 1 + floor((m_currentSize[0] + 2 * l->pad - l->f) / l->stride);
		m_currentSize[1] = 1 + floor((m_currentSize[1] + 2 * l->pad - l->f) / l->stride);
		m_currentSize[2] = l->numOfFilters;
		l->fwd_prog->SetBuffer("A", (m_currentSize[0] * m_currentSize[1] * m_currentSize[2] * m ) * sizeof(float), l->OutBind());
	}

}


void  NeuralNet::WriteFilter(unsigned int layer, float *data, unsigned int filter_num )
{
	ConvLayer* l = (ConvLayer *) layers[layer];
	l->fwd_prog->Bind();
	l->fwd_prog->BindBuffer("f");
	unsigned int size = sizeof(float) * (l->f) * (l->f) * (l->nc);
//	unsigned int offset = 	filter_num * size;
	unsigned int offset = filter_num * (l->f) * (l->f) * (l->nc) * sizeof(float);
	layers[layer]->fwd_prog->WriteBuffer<float>("f", data, size, offset);
	//float* d = new float[(l->f) * (l->f)];
	//for (unsigned int i = 0; i < (l->f) * (l->f); i++)
	//	d[i] = data[i];
	//delete[] d;

}
void NeuralNet::WriteFilters(unsigned int layer, float* data, unsigned int filter_num, unsigned int filter_start)
{
	ConvLayer* l = (ConvLayer*)layers[layer];
	l->fwd_prog->Bind();
	for (unsigned int i = filter_start; i < filter_start + filter_num; i++)
		WriteFilter(layer, data + i * (l->f) * (l->f) * (l->nc), i);
}

void NeuralNet::ForwardPropagation(float data[], unsigned int m)
{
	SetData(data, m);
	for (unsigned int i = 0; i < layers.size(); i++)
	{
		//ConvLayer* l = (ConvLayer*)layers[i];
		layers[i]->fwd_prog->Bind();
		layers[i]->fwd_prog->Call(m, 1, 1);
		GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		GLenum res = glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
	}
}
float* NeuralNet::Get(const std::string name, unsigned int layer)

{
	layers[layer]->fwd_prog->Bind();
	layers[layer]->fwd_prog->BindBuffer(name);
	GLCALL(return (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_MAP_READ_BIT));
}