#include <SFML/Graphics.hpp>
using namespace sf;

#ifndef Pocisk_h
#define Pocisk_h
class Pocisk{
private:
	CircleShape shape;
	//u- gora, d- dol, - -lewo, r- prawo
	char kierunek;
public:
	int X, Y;
	Pocisk(int X, int Y, char kierunek){
		shape.setFillColor(Color(255, 255, 255, 255));
		shape.setRadius(3);
		this->X = X;
		this->Y = Y;
		this->kierunek = kierunek;
	}
	void ruch(){
		switch (kierunek){
			case 'u':
				Y--;
				break;
			case 'd':
				Y++;
				break;
			case 'l':
				X--;
				break;
			case 'r':
				X++;
				break;
		}
	}
	void rysujpocisk(RenderWindow &window){
		shape.setPosition(Vector2f(X-3, Y-3));
		window.draw(shape);
	}
	bool kolizja(Mapa &mapa){
		if (X > 630 || X < 10||Y>390||Y<55) return true;
		if (mapa.mapa[((Y - 53) * 2) / 25][(X - 7) / 25] == 'X'){
			mapa.mapa[((Y - 53) * 2) / 25][(X - 7) / 25] = '-';
			return true;
		}
		return false;
	}
};
#endif