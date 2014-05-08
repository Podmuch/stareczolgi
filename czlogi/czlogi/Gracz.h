#include <SFML/Graphics.hpp>
#include "Pocisk.h"
using namespace sf;

#ifndef Gracz_h
#define Gracz_h
class Gracz{
public:
	Texture texczolg;
	int X, Y;
	Sprite czolg;
	Gracz(int nr){
		if (!texczolg.loadFromFile("sprite.jpg")) printf("blad ladowania tekstury\n");
		czolg.setTexture(texczolg);
		czolg.setTextureRect(IntRect(0/*+26 %206*/, nr*26, 25, 25));
		X = 320;
		Y = 380;

	}
	void rysujgracza(RenderWindow &window){
		czolg.setPosition(X-12,Y-12);
		window.draw(czolg);
	}
	void ruch(Event &event, Mapa &mapa){
		switch (event.key.code){
		case Keyboard::Up:
			if (mapa.mapa[((Y - 53) * 2) / 25 - 3][(X - 7) / 25] != 'X'&&Y>90){
				Y--;
			}
			if (czolg.getRotation() == 0 || czolg.getRotation() == 90) Y += 25;
			if (czolg.getRotation() == 180 || czolg.getRotation() == 90) X -= 25;
			czolg.setRotation(270);
			break;
		case Keyboard::Down:
			if (mapa.mapa[((Y - 53) * 2) / 25 + 1][(X - 7) / 25 - 1] != 'X'&&Y<380){
				Y++;
			}
			if (czolg.getRotation() == 180 || czolg.getRotation() == 270) Y -= 25;
			if (czolg.getRotation() == 0 || czolg.getRotation() == 270) X += 25;
			czolg.setRotation(90);
			break;
		case Keyboard::Left:
			if (mapa.mapa[((Y - 53) * 2) / 25-2][(X +6) / 25 -2] != 'X'&&X>45){
				X--;
			}
			if (czolg.getRotation() == 90 || czolg.getRotation() == 0) Y += 25;
			if (czolg.getRotation() == 270 || czolg.getRotation() == 0) X += 25;
			czolg.setRotation(180);
			break;
		case Keyboard::Right:
			if (mapa.mapa[((Y - 53) * 2) / 25][(X + 6) / 25] != 'X'&&X<620){
				X++;
			}
			if (czolg.getRotation() == 270 || czolg.getRotation() == 180) Y -= 25;
			if (czolg.getRotation() == 90 || czolg.getRotation() == 180) X -= 25;
			czolg.setRotation(0);
			break;
		}
	}
	Pocisk *strzal(){
		Pocisk *nowy=NULL;
		if (czolg.getRotation() == 0)  nowy=new Pocisk(X + 17, Y , 'r');
		if (czolg.getRotation() == 90) nowy = new Pocisk(X -25, Y + 17, 'd');
		if (czolg.getRotation() == 180) nowy = new Pocisk(X-42, Y-25, 'l');
		if (czolg.getRotation() == 270) nowy = new Pocisk(X, Y-42, 'u');
		return nowy;
	}
};
#endif