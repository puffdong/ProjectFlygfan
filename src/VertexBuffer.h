#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // Internal rendering ID used in OpenGL

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};