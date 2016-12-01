#ifndef _HOP_H
#define _HOP_H

namespace hop
{
	void hopDisplay();
};

class HopSide
{

private:

	float m_eye[3];
	float m_at[3];

	HopSide(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ);

	friend void hop::hopDisplay();

public:

	void getViewMatrix(float out[]);
	void getProjectionMatrix(float out[]);
	void getViewProjectionMatrix(float out[]);
};

class Hop
{

public:

	static void setBeforeDisplayFunc(void(*beforeCallBack)());
	static void setAfterDisplayFunc(void(*afterCallBack)());

	static void setDisplayFunc(void(*displayCallBack)(HopSide));
	static void setReshapeFunc(void(*reshapeCallBack)(int, int));


	static float getSize();
	static float getX();
	static float getY();
	static float getZ();

	static void setSize(float size);
	static void setPosition(float x, float y, float z);

private:

	static float s_size;
	static float s_x;
	static float s_y;
	static float s_z;

	Hop();
};

#endif