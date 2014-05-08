#include <SFML/Graphics.hpp>
#include <fstream>
using namespace sf;
using namespace std;

#ifndef Mapa_h
#define Mapa_h
class Mapa {
private:
	//elementy planszy
	Texture texmur;
	Sprite mur;
public:
	//pierwsza wspolrzedna to wiersz, druga to kolumna
	char mapa[25][27];
	Mapa(){
		for (int i = 0; i < 27; i++){
			for (int j = 0; j < 25; j++){
				mapa[i][j] = '-';
			}
		}
		if (!texmur.loadFromFile("sciana.jpg")) printf("blad ladowania tekstury\n");
		mur.setTexture(texmur);
	}
	void wczytaniemapy(int poziom){
		ifstream plik;
		int i = 0;
		if (poziom == 1) plik.open("poziomy/pierwszy.lvl");
		if (plik.is_open()){
			while (!plik.eof()){
				plik.getline(mapa[i], 26);
				i++;
			}
		}
	}
	void rysowaniemapy(RenderWindow &window){
		for (int i = 0; i < 27; i++){
			for (int j = 0; j < 25; j++){
				if (mapa[i][j] == 'X') {
					mur.setPosition(j * 25 + 7, i * 12.5f + 53);
					window.draw(mur);
				}
			}
		}
	}
	void mapazserwera(char *buf){
		for (int i = 0; i < 27; i++){
			for (int j = 0; j < 25; j++){
				mapa[i][j] = buf[1+i + i*j];
			}
		}
	}
};
#endif