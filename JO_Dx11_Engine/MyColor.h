#pragma once
class MyColor
{

public:

	MyColor();
	MyColor(unsigned int val);
	MyColor(unsigned char r, unsigned char g, unsigned char b);
	MyColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	MyColor(const MyColor& src);

	MyColor& operator=(const MyColor& src);
	bool operator==(const MyColor& rhs) const;
	bool operator!=(const MyColor& rhs) const;

	constexpr unsigned char GetR() const;
	void SetR(unsigned char r);

	constexpr unsigned char GetG() const;
	void SetG(unsigned char g);

	constexpr unsigned char GetB() const;
	void SetB(unsigned char b);

	constexpr unsigned char GetA() const;
	void SetA(unsigned char a);

private:
	union
	{
		unsigned char rgba[4];
		unsigned int color;
	};


};

namespace Colors
{
	const Color UnloadedTextureColor(100, 100, 100);
	const Color UnhandledTextureColor(250, 0, 0);
}