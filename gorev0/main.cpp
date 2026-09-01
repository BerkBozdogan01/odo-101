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
	m.theta = NormalizeAngle(a.theta * -1);
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

// Konu anlatımındaki tablolar derece — kod radyan. Çeviri: derece * pi/180.
double deg(double d){ return d * M_PI / 180.0; }

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

	const double eps = 1e-9;

	// --- 5) NormalizeAngle, Metot 1 tablosu
	assert(approxEq(NormalizeAngle(0.0),        0.0,      eps));
	assert(approxEq(NormalizeAngle(M_PI/2),     M_PI/2,   eps));
	assert(approxEq(NormalizeAngle(-M_PI),     -M_PI,     eps) && "sol uc dahil");
	assert(approxEq(NormalizeAngle(M_PI),      -M_PI,     eps) && "sag uc haric: +pi -> -pi");
	assert(approxEq(NormalizeAngle(3*M_PI/2),  -M_PI/2,   eps));
	assert(approxEq(NormalizeAngle(2*M_PI),     0.0,      eps));
	assert(approxEq(NormalizeAngle(3*M_PI),    -M_PI,     eps));
	assert(approxEq(NormalizeAngle(-3*M_PI),   -M_PI,     eps));
	assert(approxEq(NormalizeAngle(7*M_PI/2),  -M_PI/2,   eps));
	assert(approxEq(NormalizeAngle(-5*M_PI/2), -M_PI/2,   eps));
	assert(approxEq(NormalizeAngle(4*M_PI),     0.0,      eps));
	assert(approxEq(NormalizeAngle(10*M_PI),    0.0,      eps));
	assert(approxEq(NormalizeAngle(deg(200)),   deg(-160), eps));
	assert(approxEq(NormalizeAngle(deg(-200)),  deg(160),  eps));

	// --- 6) NormalizeAngle özellikleri: idempotentlik + "fark 2pi'nin tam kati" kurali
	for (int k = -2000; k <= 2000; ++k) {
		double x = k * 0.0123;
		double r = NormalizeAngle(x);
		assert(approxEq(NormalizeAngle(r), r, eps) && "idempotent degil");
		double tur = (x - r) / (2*M_PI);               // tam sayi olmali
		assert(approxEq(tur, round(tur), 1e-6) && "girdi-cikti farki 2pi'nin kati degil");
	}

	// --- 7) topla, Metot 2 tablosu
	{
		Vec2 s = topla({3,1},{2,4});
		assert(approxEq(s.x, 5, eps) && approxEq(s.y, 5, eps));
		s = topla({0,0},{7,-2});
		assert(approxEq(s.x, 7, eps) && approxEq(s.y, -2, eps));
		s = topla({5,5},{-5,-5});
		assert(approxEq(s.x, 0, eps) && approxEq(s.y, 0, eps));
	}

	// --- 8) rotate, Örnek M2.1 vektörleri
	{
		Vec2 r = rotate({1,0}, deg(90));               // (0,1)
		assert(approxEq(r.x, 0, eps) && approxEq(r.y, 1, eps) && "+x kuzeye donmeli (saat tersi!)");
		r = rotate({2,3}, deg(90));                    // 90 derece: (x,y) -> (-y,x)
		assert(approxEq(r.x, -3, eps) && approxEq(r.y, 2, eps));
		r = rotate({5,0}, deg(180));                   // (-5,0)
		assert(approxEq(r.x, -5, eps) && approxEq(r.y, 0, eps));
		r = rotate({3,0}, deg(30));                    // (3cos30, 3sin30)
		assert(approxEq(r.x, 3*cos(deg(30)), eps) && approxEq(r.y, 1.5, eps));
	}

	// --- 9) rotate özellikleri: uzunluk korunur + açı tam theta kadar artar
	// (uzunluk tek başına yetmez: ayna görüntüsü de uzunluk korur — açıyı da kontrol et)
	for (int i = -20; i <= 20; ++i) {
		for (int j = 1; j <= 20; ++j) {
			Vec2 v{ i*0.7, j*0.3 };
			double th = i*0.31 + j*0.17;
			Vec2 r = rotate(v, th);
			double lv = sqrt(v.x*v.x + v.y*v.y), lr = sqrt(r.x*r.x + r.y*r.y);
			assert(approxEq(lv, lr, eps) && "uzunluk korunmadi");
			assert(approxEqAngle(atan2(r.y,r.x), atan2(v.y,v.x) + th, 1e-9) && "aci theta kadar artmadi");
		}
	}
	// zincir: R(60)R(30) = R(90)  (Örnek 2.3)
	{
		Vec2 a = rotate(rotate({4,0}, deg(30)), deg(60));
		Vec2 b = rotate({4,0}, deg(90));
		assert(approxEq(a.x, b.x, eps) && approxEq(a.y, b.y, eps));
	}

	// --- 10) compose, Metot 4 tablosu
	{
		Pose2D p = compose({0,0,0},{1,0,0});
		assert(approxEq(p.x,1,eps) && approxEq(p.y,0,eps) && approxEqAngle(p.theta,0,eps));

		p = compose({0,0,deg(90)},{2,0,0});
		assert(approxEq(p.x,0,eps) && approxEq(p.y,2,eps) && approxEqAngle(p.theta,deg(90),eps));

		p = compose({2,1,deg(90)},{4,0,0});            // Örnek 4.3: ileri gitmek
		assert(approxEq(p.x,2,eps) && approxEq(p.y,5,eps));

		p = compose({2,1,deg(90)},{0,3,0});            // Örnek 4.4: sola kayma -x'e gider
		assert(approxEq(p.x,-1,eps) && approxEq(p.y,1,eps));

		p = compose({5,5,deg(180)},{3,0,0});           // Örnek 4.5
		assert(approxEq(p.x,2,eps) && approxEq(p.y,5,eps) && approxEqAngle(p.theta,deg(180),eps));

		p = compose({1,2,deg(30)},{3,0,deg(45)});      // Örnek 4.1 (birim test)
		assert(approxEq(p.x, 1 + 3*cos(deg(30)), eps));
		assert(approxEq(p.y, 2 + 3*sin(deg(30)), eps));
		assert(approxEqAngle(p.theta, deg(75), eps));
		// Dikkat: theta_b trigonometriye girmez, sadece toplama girer (Örnek 4.1 notu)

		p = compose({0,0,deg(170)},{0,0,deg(40)});     // Örnek M4.1: sarma devreye girer
		assert(approxEq(p.theta, deg(-150), eps) && "170+40=210 sarilmali: -150");
	}

	// --- 11) compose özellikleri
	{
		Pose2D a{1,2,deg(30)}, b{3,0,deg(45)}, e{0,0,0};

		// birim eleman: a (+) e = a  ve  e (+) a = a
		Pose2D p = compose(a,e);
		assert(approxEq(p.x,a.x,eps) && approxEq(p.y,a.y,eps) && approxEqAngle(p.theta,a.theta,eps));
		p = compose(e,a);
		assert(approxEq(p.x,a.x,eps) && approxEq(p.y,a.y,eps) && approxEqAngle(p.theta,a.theta,eps));

		// degismeli DEGIL: a(+)b ile b(+)a konumlari farkli (Örnek 4.2) — ama theta ayni
		Pose2D ab = compose(a,b), ba = compose(b,a);
		assert(!(approxEq(ab.x,ba.x,1e-6) && approxEq(ab.y,ba.y,1e-6)) && "degismeli cikmamali");
		assert(approxEqAngle(ab.theta, ba.theta, eps));

		// birlesmeli: (a(+)b)(+)c = a(+)(b(+)c) — süpürme
		for (int i = -5; i <= 5; ++i) {
			Pose2D c{ i*0.9, -i*0.4, i*0.7 };
			Pose2D L = compose(compose(a,b),c);
			Pose2D R = compose(a,compose(b,c));
			assert(approxEq(L.x,R.x,eps) && approxEq(L.y,R.y,eps) && approxEqAngle(L.theta,R.theta,eps));
		}

		// Örnek M4.2 — eşkenar üçgen: 3 kez (1,0,120) baslangica dondurmeli
		Pose2D t{0,0,0}, adim{1,0,deg(120)};
		for (int i = 0; i < 3; ++i) t = compose(t, adim);
		assert(approxEq(t.x,0,eps) && approxEq(t.y,0,eps) && approxEqAngle(t.theta,0,eps) && "ucgen kapanmadi");

		// Alistirma 19 — kare: 4 kez (2,0,90) baslangica dondurmeli
		Pose2D k{0,0,0}, kadim{2,0,deg(90)};
		for (int i = 0; i < 4; ++i) k = compose(k, kadim);
		assert(approxEq(k.x,0,eps) && approxEq(k.y,0,eps) && approxEqAngle(k.theta,0,eps) && "kare kapanmadi");
	}

	// --- 12) inverse, Metot 5 tablosu
	{
		Pose2D p = inverse({0,0,0});                   // birim elemanin tersi kendisi
		assert(approxEq(p.x,0,eps) && approxEq(p.y,0,eps) && approxEqAngle(p.theta,0,eps));

		p = inverse({5,3,0});                          // theta=0: sadece isaret
		assert(approxEq(p.x,-5,eps) && approxEq(p.y,-3,eps) && approxEqAngle(p.theta,0,eps));

		p = inverse({3,0,deg(90)});                    // Örnek 5.3: (-3,0) DEGIL
		assert(approxEq(p.x,0,eps) && approxEq(p.y,3,eps) && approxEqAngle(p.theta,deg(-90),eps));

		p = inverse({0,4,deg(90)});
		assert(approxEq(p.x,-4,eps) && approxEq(p.y,0,eps) && approxEqAngle(p.theta,deg(-90),eps));

		p = inverse({1,2,deg(30)});                    // Örnek 5.1 (ana örnek), kapali form
		assert(approxEq(p.x, -1*cos(deg(30)) - 2*sin(deg(30)), eps));
		assert(approxEq(p.y,  1*sin(deg(30)) - 2*cos(deg(30)), eps));
		assert(approxEqAngle(p.theta, deg(-30), eps));

		p = inverse({2,0,deg(180)});                   // Örnek M5.1: konum ayni kalir!
		assert(approxEq(p.x,2,eps) && approxEq(p.y,0,eps) && approxEqAngle(p.theta,deg(180),eps));
	}

	// --- 13) inverse özellikleri: a (+) (-)a = birim  ve  (-)(-)a = a  (süpürme)
	for (int i = -10; i <= 10; ++i) {
		Pose2D a{ i*0.8, -i*0.5, NormalizeAngle(i*0.6) };
		Pose2D geri = compose(a, inverse(a));          // Kavram 5 tanimi
		assert(approxEq(geri.x,0,eps) && approxEq(geri.y,0,eps) && approxEqAngle(geri.theta,0,eps));
		Pose2D cift = inverse(inverse(a));             // Örnek M5.2: isaret hatasini aninda yakalar
		assert(approxEq(cift.x,a.x,eps) && approxEq(cift.y,a.y,eps) && approxEqAngle(cift.theta,a.theta,eps));
	}

	// --- 14) AngDiff, Metot 6 tablosu
	assert(approxEq(AngDiff(deg(80),  deg(30)),   deg(50),  eps));
	assert(approxEq(AngDiff(deg(45),  deg(45)),   0.0,      eps));
	assert(approxEq(AngDiff(deg(10),  deg(350)),  deg(20),  eps) && "en kisa yol +20, -340 degil");
	assert(approxEq(AngDiff(deg(350), deg(10)),   deg(-20), eps));
	assert(approxEq(AngDiff(deg(-179),deg(179)),  deg(2),   eps));
	assert(approxEq(AngDiff(deg(179), deg(-179)), deg(-2),  eps));
	assert(approxEq(AngDiff(deg(5),   deg(355)),  deg(10),  eps));
	// sinir vakasi (Örnek M6.1): sonuc +pi VEYA -pi — bu tolerans degil KONVANSIYON meselesi,
	// o yuzden isarete degil buyuklugune assert yaziyoruz
	assert(approxEq(fabs(AngDiff(0.0, deg(180))), M_PI, eps));

	// --- 15) Alistirma 20 — tabela govde cerceveinde: topla + rotate birlikte
	// Dunya: tabela (5,5), arac (2,1,90). p_govde = R(-theta) * (p_dunya - t)
	{
		Vec2 fark = topla({5,5}, {-2,-1});
		Vec2 pg = rotate(fark, deg(-90));
		assert(approxEq(pg.x, 4, eps) && approxEq(pg.y, -3, eps));
		double bearing = atan2(pg.y, pg.x);            // burna gore aci
		assert(approxEq(bearing, atan2(-3.0, 4.0), eps));
	}

	// --- 16) SINIF DEGISMEZI (Metot 3): compose/inverse ciktisinin theta'si HEP [-pi,pi) icinde
	// "sar, sonra sakla" — bu blok en sonda, cunku inverse su an theta'yi sarmiyor olabilir
	for (int i = -30; i <= 30; ++i) {
		Pose2D a{ i*0.3, i*0.2, NormalizeAngle(i*0.5) };
		Pose2D c = compose(a, {1.0, -0.5, NormalizeAngle(i*0.9)});
		assert(c.theta >= -M_PI && c.theta < M_PI && "compose degismezi ihlal");
		Pose2D v = inverse(a);
		assert(v.theta >= -M_PI && v.theta < M_PI && "inverse degismezi ihlal");
	}
	// tam sinir ucu: theta=-pi kanonik bir girdi (sol uc dahil). Tersi -(-pi)=+pi olur
	// ve +pi aralikta DEGIL — inverse'un theta'yi sarmasi gerekir.
	{
		Pose2D v = inverse({2,0,-M_PI});
		assert(v.theta >= -M_PI && v.theta < M_PI && "inverse(-pi) -> +pi: sar, sonra sakla");
	}

	cout << "gorev0: tum assertler gecti\n";
	return 0;
}
