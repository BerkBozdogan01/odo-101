// gorev0 — ODO-101
// Bu dosyayı sıfırdan sen dolduracaksın. Kolay gelsin.
#include <cmath>
#include <iostream>
#include <cassert>
using namespace std;

double NormalizeAngle(double x){
	x=fmod(x,2*M_PI);
 	while(x>=M_PI){x-=2*M_PI;}
 	while(x<-1*M_PI){x+=2*M_PI;}
 	return x;
}


//burda bağıl bir toplama yok, apsisle apsis toplanıyor;ordinatla ordinat.
struct Vec2{double x; double y;};

struct Pose2D{double x; double y; double theta;};

Vec2 topla(Vec2 a,Vec2 b){
	Vec2 s;
	s.x = a.x + b.x;
	s.y = a.y + b.y;
	return s;
}

Vec2 rotate(Vec2 v, double theta){
	Vec2 s;
	s.x = v.x*cos(theta) + v.y*(-1*sin(theta));
	s.y = v.x*sin(theta) + v.y*cos(theta);
	return s;
}
Pose2D compose(Pose2D a,Pose2D b){
	Pose2D s;
	s.x = a.x + b.x*cos(a.theta) + b.y*sin(a.theta)*(-1);
	s.y = a.y + b.x*sin(a.theta) + b.y*cos(a.theta);
	s.theta = NormalizeAngle(a.theta + b.theta);
	return s;
}

Pose2D inverse(Pose2D a){
	Pose2D m;
	m.theta = a.theta * -1;
	double alpha= atan2(a.y,a.x);
	double beta = a.theta - alpha;
	m.x = -1 * sqrt(a.x*a.x + a.y*a.y)*cos(beta);
	m.y = sqrt(a.x*a.x + a.y*a.y)*sin(beta);   
	return m;
}

double AngDiff(double a,double b){
	double d = a-b;
	return atan2(sin(d),cos(d));
}

bool approxEq(double a, double b, double eps){
	return fabs(a - b) < eps;
}

bool approxEqAngle(double a, double b, double eps){
	return fabs(AngDiff(a,b)) < eps;
}

int main() {
	
	// --- 1) approxEq'in kendisi: kayan nokta neden == ile karşılaştırılmaz
	assert(0.1 + 0.2 != 0.3);                          // gerçekten eşit değil
	assert(approxEq(0.1 + 0.2, 0.3, 1e-9));            // ama pratikte eşit

	// --- 2) NormalizeAngle, sınır: 5pi
	// 5pi = 2pi*2 + pi. [-pi,pi) konvansiyonunda +pi girdisi -pi'ye düşer.
	assert(approxEqAngle(NormalizeAngle(5*M_PI), M_PI, 1e-9));   // çemberde: geçer
	assert(NormalizeAngle(5*M_PI) < 0 && "konvansiyon [-pi,pi): +pi degil -pi");

	// --- 3) Aralık değişmezi: HER girdi için sonuç [-pi, pi) içinde olmalı
	// Tek örnek yerine süpürme — 4001 nokta, tek satır.
	for (int k = -2000; k <= 2000; ++k) {
		double r = NormalizeAngle(k * 0.00785398163);
		assert(r >= -M_PI && r < M_PI && "aralik disina cikti");
	}

	// --- 4) Kontrol kuralı (özellik testi): a + AngDiff(b,a) çemberde b'ye eşit
	for (int i = -50; i <= 50; ++i) {
		for (int j = -50; j <= 50; ++j) {
			double a = i * 0.37, b = j * 0.41;          // kasten sarılmamış, büyük değerler
			assert(approxEqAngle(a + AngDiff(b, a), b, 1e-9));
			assert(fabs(AngDiff(b, a)) <= M_PI + 1e-9 && "en kisa yay degil");
		}
	
	}
}
