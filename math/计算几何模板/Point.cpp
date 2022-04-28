#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-8;
const int inf = 0x3f3f3f3f;
const int Inf = INT_MAX;
const double PI = 3.14159265358979323846264338;

int sgn(double x) { return abs(x) < eps ? 0 : (x > 0 ? 1 : -1); }
long long gcd(long long a, long long b) { return b == 0 ? a : gcd(b, a % b); }

//
struct Point // Point & Vector
{
	double x, y;
	Point(const double &x = 0, const double &y = 0) : x(x), y(y) {}
	friend Point operator+(const Point &a, const Point &b){
		return Point(a.x + b.x, a.y + b.y);
	}
	friend Point operator-(const Point &a, const Point &b){
		return Point(a.x - b.x, a.y - b.y);
	}
	friend Point operator*(const double &c, const Point &a){
		return Point(c * a.x, c * a.y);
	}
	friend Point operator*(const Point &a, const double &c){
		return Point(c * a.x, c * a.y);
	}
	friend Point operator/(const Point &a, const double &c){
		return Point(a.x / c, a.y / c);
	}
	friend Point rotate(const Point &v, double theta){ // 向量逆时针旋转theta弧度
		return Point(v.x * cos(theta) - v.y * sin(theta),
					 v.x * sin(theta) + v.y * cos(theta));
	}
	friend Point rotateAroundPoint(Point &v, Point &p, double theta){
		return rotate(v - p, theta) + p;
	}
	friend bool operator==(const Point &a, const Point &b){
		return !sgn(a.x - b.x) && !sgn(a.y - b.y);
	}
	friend bool operator<(const Point &a, const Point &b){
		return sgn(a.x - b.x) < 0 || (!sgn(a.x - b.x) && sgn(a.y - b.y) < 0);
	}
	double norm() { return sqrt(x * x + y * y); }	 // 向量模
	friend double det(const Point &a, const Point &b){ // 向量叉积
		return a.x * b.y - a.y * b.x;
	}
	friend double dot(const Point &a, const Point &b){ // 向量点积
		return a.x * b.x + a.y * b.y;
	}
	friend double dis(const Point &a, const Point &b){// 两点间距离
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
	friend Point intersection(Point u1, Point u2, Point v1, Point v2){ // 线段交点, 线段有交点才可用
		return u1 + (u2 - u1) * det(u1 - v1, v1 - v2) / det(u1 - u2, v1 - v2);
	}
	double arg() { return atan2(y, x); } // 返回弧度
	friend double arg_2(Point u, Point v){
		return acos(dot(u, v) / (u.norm() * v.norm()));
	} // 两向量之间的夹角
	friend double arg_3(const Point &a, const Point &b, const Point &c){
		return arg_2(a - b, c - b);
	} // ∠abc
};
int main()
{
	
}