#include "MyColor.h"

MyColor::MyColor() :color(0)
{
}

MyColor::MyColor(unsigned int val) : color(val)
{
}

MyColor::MyColor(unsigned char r, unsigned char g, unsigned char b) : MyColor(r, g, b, 255)
{
}

MyColor::MyColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)  
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

MyColor::MyColor(const MyColor & src) :color(src.color)
{
}

// 오퍼레이터 대입
MyColor & MyColor::operator=(const MyColor & src)
{
	this->color = src.color;
	return *this;
}


// 오퍼레이터 색깔비교
bool MyColor::operator==(const MyColor & rhs) const
{
	return (this->color == rhs.color);
}

bool MyColor::operator!=(const MyColor & rhs) const
{
	return !(*this == rhs);
}

constexpr unsigned char MyColor::GetR() const
{
	return this->rgba[0];
}

void MyColor::SetR(unsigned char r)
{
	this->rgba[0] = r;
}

constexpr unsigned char MyColor::GetG() const
{
	return this->rgba[1];
}

void MyColor::SetG(unsigned char g)
{
	this->rgba[1] = g;
}

constexpr unsigned char MyColor::GetB() const
{
	return this->rgba[2];
}

void MyColor::SetB(unsigned char b)
{
	this->rgba[2] = b;
}

constexpr unsigned char MyColor::GetA() const
{
	return this->rgba[3];
}

void MyColor::SetA(unsigned char a)
{
	this->rgba[3] = a;
}
