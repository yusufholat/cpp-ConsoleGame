#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <iomanip>

using namespace std;

const int genislik = 80;
const int yukseklik = 20;
char sahne[genislik + 10][yukseklik]; //sahnede +10 genislik bizim coplugumuz, yazdırırken kullanmayacagız

const int max_mermisayisi = 1000;
const int max_dusmansayisi = 1000;

char tuslar[256];
char gameover_karakter = 178;

int mermisayisi = 0;
int dusmansayisi = 0;
int oyun = 1;

int yokedilemeyen_dusman;


struct Ucak
{
	int x_orta;
	int y_orta;
	char karakter = 178;
	char roket = 175;
};
Ucak ucak;

struct Mermi
{
	int x;
	int y;
	char karakter = 254;
};
Mermi *mermiler = new Mermi[max_mermisayisi];
// Mermi mermiler[max_mermisayisi];

struct Dusman
{
	int x;
	int y;
	char karakter = 177;
	char karakterkose = 43;
};
Dusman *dusmanlar = new Dusman[max_dusmansayisi];
// Dusman dusmanlar[max_dusmansayisi];


void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), coord);
};
void kursoruGizle()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
};
void klavyeOku(char tuslar[])
{
	for (int x = 0; x < 256; x++)
	{
		tuslar[x] = (char)(GetAsyncKeyState(x) >> 8);
	}
}
void sahneyiCiz()
{
	for (int y = 0; y < yukseklik; y++)
	{
		for (int x = 0; x < genislik; x++)
		{
			cout << sahne[x][y];
		}
		cout << endl;
	}
}
void sahneyiTemizle()
{
	for (int y = 0; y < yukseklik; y++)
	{
		for (int x = 0; x < genislik + 10; x++)
		{
			sahne[x][y] = ' ';
		}
		cout << endl;
	}
};
void sinirlariOlustur()
{
	for (int x = 0; x < genislik; x++)
	{
		sahne[x][0] = 219;
		sahne[x][yukseklik - 1] = 219;
	}
	for (int y = 0; y < yukseklik; y++)
	{
		sahne[0][y] = 219;
		sahne[genislik - 1][y] = 219;
	}
}


///uçak fonksiyonları
//ucagi tutan kordinatları baslangıc olarak sahnenin ortasina ayarliyoruz
void ucakOlustur()
{
	ucak.x_orta = 3;
	ucak.y_orta = yukseklik / 2;
}

//ucak nesnesinin x ve y si, ucagın orta noktasini tutuyor. 
//buna gore sahneye ucagi yerlestiriyoruz
void ucagıSahneyeYerlestir()
{
	int x = ucak.x_orta;
	int y = ucak.y_orta;

	sahne[x - 2][y - 2] = ucak.roket;
	sahne[x - 1][y - 1] = ucak.karakter;
	sahne[x][y] = ucak.karakter;
	sahne[x - 1][y + 1] = ucak.karakter;
	sahne[x - 2][y + 2] = ucak.roket;
}

//klavyeden okudugumuz tuslara gore ucagin hareket sinirlarini da hesaba
//katarak ucagimizi hareket ettiriyoruz
void ucagiHareketEttir()
{
	klavyeOku(tuslar);

	if (tuslar['W'] != 0)
	{
		if (ucak.y_orta > 3)
			ucak.y_orta--;
	}
	if (tuslar['S'] != 0)
	{
		if (ucak.y_orta < yukseklik - 4)
			ucak.y_orta++;
	}
}


///memri fonksiyonları
//mermi, ucagin orta kordinatlarında üretiliyor
//x ve ysini buna göre ayarliyoruz
void mermiUret()
{
	klavyeOku(tuslar);

	if (tuslar['P'] != 0)
	{
		mermiler[mermisayisi].x = ucak.x_orta;
		mermiler[mermisayisi].y = ucak.y_orta;
		mermisayisi++;
	}
}

//tum mermileri dolanarak sahneye ciziyoruz
void mermiyiSahneyeYerlestir()
{
	for (int i = 0; i < mermisayisi; i++)
	{
		int m = mermiler[i].x;
		int k = mermiler[i].y;
		sahne[m][k] = mermiler[i].karakter;
	}
}

//mermi hareketi sahnenin copluk kisminda kisitlaniyor.
//sahneden taşması engellenmek için
void mermiyiHareketEttir()
{
	for (int i = 0; i < max_mermisayisi; i++)
	{
		if (mermiler[i].x == genislik + 9)
			mermiler[i].x = genislik + 9;
		else
			mermiler[i].x++;
	}
}


///dusman fonksiyonları
//dusmanin y'si random atanıyor her dusman urettigimizde 
//bir sonraki dusmana gecmek icin sayiyi arttiriyoruz
void dusmanUret()
{
	int randy = 2 + rand() % (yukseklik - 4);

	dusmanlar[dusmansayisi].x = genislik - 3;
	dusmanlar[dusmansayisi].y = randy;

	dusmansayisi++;
}

//dusmanin tuttugu kordinata gore sahneye dusmanlari ciziyoruz
void dusmanSahneyeYerlestir()
{
	for (int i = 0; i < dusmansayisi; i++)
	{
		int x = dusmanlar[i].x;
		int y = dusmanlar[i].y;

		sahne[x - 1][y - 1] = dusmanlar[i].karakterkose;
		sahne[x][y - 1] = dusmanlar[i].karakter;
		sahne[x + 1][y - 1] = dusmanlar[i].karakterkose;

		sahne[x - 1][y] = dusmanlar[i].karakter;
		//	sahne[x][y] = dusmanlar[i].karakter; //dusmanın orta karakteri bos.
		sahne[x + 1][y] = dusmanlar[i].karakter;

		sahne[x - 1][y + 1] = dusmanlar[i].karakterkose;
		sahne[x][y + 1] = dusmanlar[i].karakter;
		sahne[x + 1][y + 1] = dusmanlar[i].karakterkose;
	}
}

//dusman[i].x 0 kordinatina gelince copluge atiliyor
void dusmaniHareketEttir()
{
	for (int i = 0; i < max_dusmansayisi; i++)
	{
		if (dusmanlar[i].x == 2)
		{
			yokedilemeyen_dusman++;
			dusmanlar[i].x = genislik + 5;
			dusmanlar[i].y = 1;

		}
		else if (dusmanlar[i].x == genislik + 5)
			dusmanlar[i].x = genislik + 5;
		else
			dusmanlar[i].x--;
	}
}


///diger fonksiyonlar
//mermi ve dusmanin carpisacagi konumlara göre copluge atiyoruz
void carpismaKontrol(int *skorptr)
{
	for (int i = 0; i < max_mermisayisi; i++)
	{
		for (int j = 0; j < max_dusmansayisi; j++)
		{
			if (mermiler[i].x == dusmanlar[j].x - 1 && mermiler[i].y == dusmanlar[j].y - 1 ||
				mermiler[i].x == dusmanlar[j].x - 1 && mermiler[i].y == dusmanlar[j].y ||
				mermiler[i].x == dusmanlar[j].x - 1 && mermiler[i].y == dusmanlar[j].y + 1)
			{
				mermiler[i].x = genislik + 5;
				dusmanlar[j].x = genislik + 5;
				dusmanlar[j].y = 1;
				(*skorptr)++;
			}
		}

	}
}

//düşmanlar uçağa çarptığında oyunu sıfırlıyor. (oyun, while için dönmesini sağlıyor)
bool ucakcarpma()
{
	//düşmanın uçağa çarpması için kosullar kontrol ediliyor
	for (int i = 0; i < max_dusmansayisi; i++)
	{
		if (dusmanlar[i].x - 1 == ucak.x_orta - 2 && dusmanlar[i].y + 1 == ucak.y_orta - 2
			|| dusmanlar[i].x - 1 == ucak.x_orta - 2 && dusmanlar[i].y == ucak.y_orta - 2
			|| dusmanlar[i].x - 1 == ucak.x_orta - 1 && dusmanlar[i].y + 1 == ucak.y_orta - 1
			|| dusmanlar[i].x - 1 == ucak.x_orta - 1 && dusmanlar[i].y == ucak.y_orta - 1
			|| dusmanlar[i].x - 1 == ucak.x_orta && dusmanlar[i].y - 1 == ucak.y_orta
			|| dusmanlar[i].x - 1 == ucak.x_orta && dusmanlar[i].y == ucak.y_orta
			|| dusmanlar[i].x - 1 == ucak.x_orta && dusmanlar[i].y + 1 == ucak.y_orta
			|| dusmanlar[i].x - 1 == ucak.x_orta - 1 && dusmanlar[i].y - 1 == ucak.y_orta + 1
			|| dusmanlar[i].x - 1 == ucak.x_orta - 1 && dusmanlar[i].y == ucak.y_orta + 1
			|| dusmanlar[i].x - 1 == ucak.x_orta - 2 && dusmanlar[i].y == ucak.y_orta + 2
			|| dusmanlar[i].x - 1 == ucak.x_orta - 2 && dusmanlar[i].y - 1 == ucak.y_orta + 2)
		{
			oyun = 0;
			dusmanlar[i].x = genislik + 5;
			dusmanlar[i].y = 1;
		}
	}
	return oyun;
}

//game over yazısını yazdıran fonksiyon
void oyunBitti()
{
	for (int i = 0; i < yukseklik; i++)
	{
		for (int j = 0; j < genislik; j++)
		{
			if (i == 0 && (j >= 7 && j <= 11 || j >= 17 && j <= 19 || j == 24 || j == 30 || j >= 33 && j <= 39))
				sahne[j][i] = gameover_karakter;

			if (i == 1 && (j == 6 || j == 12 || j == 16 || j == 20 || j >= 24 && j <= 25 || j >= 29 && j <= 30 || j == 33))
				sahne[j][i] = gameover_karakter;

			if (i == 2 && (j == 6 || j == 15 || j == 21 || j >= 24 && j <= 26 || j >= 28 && j <= 30 || j == 33))
				sahne[j][i] = gameover_karakter;

			if (i == 3 && (j == 6 || j == 15 || j == 21 || j == 24 || j >= 26 && j <= 28 || j == 30 || j == 33))
				sahne[j][i] = gameover_karakter;

			if (i == 4 && (j == 6 || j >= 9 && j <= 11 || j >= 15 && j <= 21 || j == 24 || j == 27 || j == 30 || j >= 33 && j <= 37))
				sahne[j][i] = gameover_karakter;

			if (i == 5 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 30 || j == 33))
				sahne[j][i] = gameover_karakter;

			if (i == 6 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 30 || j == 33))
				sahne[j][i] = gameover_karakter;

			if (i == 7 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 30 || j == 33))
				sahne[j][i] = gameover_karakter;

			if (i == 8 && (j >= 7 && j <= 11 || j == 15 || j == 21 || j == 24 || j == 30 || j >= 33 && j <= 39))
				sahne[j][i] = gameover_karakter;
			if (i == 9)
				i = 9;

			if (i == 10 && (j >= 7 && j <= 11 || j == 15 || j == 21 || j >= 24 && j <= 30 || j >= 33 && j <= 38))
				sahne[j][i] = gameover_karakter;

			if (i == 11 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 33 || j == 39))
				sahne[j][i] = gameover_karakter;

			if (i == 12 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 33 || j == 39))
				sahne[j][i] = gameover_karakter;

			if (i == 13 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 33 || j == 39))
				sahne[j][i] = gameover_karakter;

			if (i == 14 && (j == 6 || j == 12 || j == 15 || j == 21 || j >= 24 && j <= 28 || j >= 33 && j <= 38))
				sahne[j][i] = gameover_karakter;

			if (i == 15 && (j == 6 || j == 12 || j == 15 || j == 21 || j == 24 || j == 33 || j >= 35 && j <= 36))
				sahne[j][i] = gameover_karakter;

			if (i == 16 && (j == 6 || j == 12 || j == 16 || j == 20 || j == 24 || j == 33 || j == 37))
				sahne[j][i] = gameover_karakter;

			if (i == 17 && (j == 6 || j == 12 || j == 17 || j == 19 || j == 24 || j == 33 || j == 38))
				sahne[j][i] = gameover_karakter;

			if (i == 18 && (j >= 7 && j <= 11 || j == 18 || j >= 24 && j <= 30 || j == 33 || j == 39))
				sahne[j][i] = gameover_karakter;
		}
	}
}

int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);

	srand(time(NULL));
	kursoruGizle();

	int skor = 0;
	int *skorptr;
	skorptr = &skor;


	int zorluk;
	char zorluk_derece;

	cout << "Oyunun zorlugunu seciniz:" << endl;
	cout << "Kolay-> 1 / Orta-> 2 / Zor-> 3" << endl;
	cin >> zorluk;

	if (zorluk == 1)
		zorluk_derece = 17;
	else if (zorluk == 2)
		zorluk_derece = 9;
	else if (zorluk == 3)
		zorluk_derece = 4;
	else
	{
		zorluk = 2;
		zorluk_derece = 9;
		cout << "zorluk Orta olarak ayarlandi." << endl;
		system("pause");
	}


	int can = 3;

	int dongu = 0;
	while (oyun && can)
	{
		sahneyiTemizle();
		sinirlariOlustur();
		gotoxy(0, 0);

		if (dongu == 0)
			ucakOlustur();

		ucagıSahneyeYerlestir();
		ucagiHareketEttir();

		mermiUret();
		mermiyiSahneyeYerlestir();

		if (dongu % zorluk_derece == 0)
			dusmanUret();

		dusmanSahneyeYerlestir();

		mermiyiHareketEttir();
		carpismaKontrol(skorptr);
		dusmaniHareketEttir();
		carpismaKontrol(skorptr);

		sahneyiCiz();
		oyun = ucakcarpma();

		if (oyun == 0)
		{
			can--;
			oyun = 1;
		}

		if (zorluk == 1)
			cout << setw(10) << left << "Zorluk" << ":" << "Kolay" << endl;
		else if (zorluk == 2)
			cout << setw(10) << left << "Zorluk" << ":" << "Orta" << endl;
		else if (zorluk == 3)
			cout << setw(10) << left << "Zorluk" << ":" << "Zor" << endl;

		cout << setw(10) << left << "Kalan Can" << ":" << can << endl;
		cout << setw(10) << left << "Skor: " << ":" << *skorptr;

		if (mermisayisi == max_mermisayisi)
		{
			oyun = 0;
			can = 0;
		}
		gotoxy(0, 0);
		dongu++;
	}

	sahneyiTemizle();
	gotoxy(0, 0);
	oyunBitti();

	sahneyiCiz();


	cout << endl << endl << endl;

	double _skor = *skorptr;
	double _mermisayisi = mermisayisi;
	double _yokedilemeyen_dusman = yokedilemeyen_dusman;

	//basari orani kacirilan dusman sayisina baglidir
	double basari_orani = (_skor - yokedilemeyen_dusman) / _mermisayisi * 100;

	if (basari_orani < 0)
		basari_orani = 0;

	if (mermisayisi == 0 && yokedilemeyen_dusman == 0)
		basari_orani = 0;

	cout << setw(10) << left << "Basari oraniniz" << ":" << basari_orani << endl;

	if (mermisayisi == max_mermisayisi)
		cout << "MERMINIZ BITTI!" << endl;

	cout << endl << endl;

	cout << "Devam etmek icin herhangi bir tusa basiniz:";

	char bitis;
	cin >> bitis;
	
	system("pause");

	return 0;
}