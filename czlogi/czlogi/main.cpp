#include <SFML/Graphics.hpp>
#include <iostream>
#include "Mapa.h"
#include "Gracz.h"
#include "Pocisk.h"
#include <cstdio>
#include <cstdlib>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace sf;
using namespace std;
//mapa 25x27
int main()
{
	std::string komenda;
	cout << "chcesz zalozyc serwer (komenda 'serwer', czy dolaczyc do istniejacego (komenda 'klient portserwera')?" << endl;
	cin >> komenda;
	if (komenda == "serwer"){
		//wczytanie mapy
		Mapa mapa;
		mapa.wczytaniemapy(1);
		//wczytanie gracza
		Gracz *gracze[4];
		for (int i = 0; i < 4; i++){
			gracze[i] = NULL;
		}
		//tablica pociskow
		Pocisk *pociski[40];
		for (int i = 0; i < 40; i++){
			pociski[i] = NULL;
		}
		//postawienie serwera

	}
	else {
		if (komenda == "klient") {
			//odczytanie portu
			cin >> komenda;
			int port = atoi(komenda.c_str());
			//pociski
			CircleShape pocisk;
			pocisk.setFillColor(Color(255, 255, 255, 255));
			pocisk.setRadius(3);
			Vector2f polozeniepociskow[40];
			for (int i = 0; i < 40; i++){
				polozeniepociskow[i].x = 0;
				polozeniepociskow[i].y = 0;
			}
			//inni gracze
			Sprite *gracze[3];
			gracze[0] = NULL;
			gracze[1] = NULL;
			gracze[2] = NULL;
			//gracz
			Gracz *gracz1;
			//mapa
			Mapa mapa;
			///////////////////////////////////////////////////////////////////////////////////
			//laczenie z serwerem
			WSADATA wsas;
			int result;
			WORD wersja;
			wersja = MAKEWORD(2, 2);
			result = WSAStartup(wersja, &wsas);
			if (result != NO_ERROR) printf("Initialization error.\n");
			SOCKET s;
			s = socket(AF_INET, SOCK_STREAM, 0);
			if (s == INVALID_SOCKET)
			{
				printf("Error creating socket: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}
			sockaddr_in sa;
			memset(&sa, 0, sizeof(sa));
			sa.sin_family = AF_INET;
			//sa.sin_addr.s_addr = htonl(INADDR_ANY);
			sa.sin_addr.s_addr = inet_addr("127.0.0.1");
			sa.sin_port = htons(port);
			if (connect(s, (SOCKADDR *)& sa, sizeof(sa)) == SOCKET_ERROR)
			{
				printf("Failed to connect.\n");
				WSACleanup();
				return 1;
			}
			char sendbuf[] = "polacz";
			send(s, sendbuf, strlen(sendbuf), 0);
			//odbieranie danych
			char buf[936]; //1(id)+25*27(mapa)+3*80(3 chary na wspolrzedna*2 wspolrzedne 40 pociskow)+3*6+2(pozycja graczy, id i kierunek)=936
			while (recv(s, buf, 934, 0) > 0)
			{
				printf("\n%s", buf);
			};
			if (buf[0] == -1){
				cout << "brak miejsca" << endl;
				system("pause");
				return 0;
			}
			else {
				gracz1 = new Gracz(buf[0] - 48);
				mapa.mapazserwera(buf);//1+25*27=676
				for (int i = 0; i < 40; i++){
					char x[3], y[3];
					x[0] = buf[676 + 6 * i];
					x[1] = buf[677 + 6 * i];
					x[2] = buf[678 + 6 * i];
					y[0] = buf[679 + 6 * i];
					y[1] = buf[680 + 6 * i];
					y[2] = buf[681 + 6 * i];
					polozeniepociskow[i].x = atoi(x);
					polozeniepociskow[i].y = atoi(y);
				}//1+25*27+3*80=916
				for (int i = 0, j = i; i < 3; i++, j++){
					if (buf[916+8*i] != -1){
						if (i == buf[0] - 48) j = i + 1;
						gracze[i] = new Sprite();
						gracze[i]->setTexture(gracz1->texczolg);
						gracze[i]->setTextureRect(IntRect(0/*+26 %206*/, j * 26, 25, 25));
						switch (buf[917 + i * 8]){
						case 'u':
							gracze[i]->setRotation(270);
							break;
						case 'd':
							gracze[i]->setRotation(90);
							break;
						case 'l':
							gracze[i]->setRotation(180);
							break;
						case 'r':
							gracze[i]->setRotation(0);
							break;
						}
						char x[3], y[3];
						x[0] = buf[918 + i * 8];
						x[1] = buf[919 + i * 8];
						x[2] = buf[920 + i * 8];
						y[0] = buf[921 + i * 8];
						y[1] = buf[922 + i * 8];
						y[2] = buf[923 + i * 8];
						gracze[i]->setPosition(Vector2f(atoi(x), atoi(y)));
					}
				}
			}
			//okno gry
			RenderWindow window(VideoMode(640, 480), "Czolgi");
			//otoczka mapy 
			RectangleShape rectangle(Vector2f(625, 337));
			rectangle.setFillColor(Color(0, 0, 0, 0));
			rectangle.setOutlineColor(Color(0, 255, 0, 255));
			rectangle.setOutlineThickness(5);
			rectangle.setPosition(Vector2f(7, 53));
			//g³ówna petla programu
			while (window.isOpen())
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
					if (event.type == Event::KeyPressed){
						gracz1->ruch(event, mapa);
						if (event.key.code == Keyboard::Space) {
							gracz1->strzal();
						}
					}
				}
				window.clear();
				int i;
				for (i = 0; i < 40; i++){
					pocisk.setPosition(Vector2f(polozeniepociskow[i].x, polozeniepociskow[i].y));
					window.draw(pocisk);
				}
				gracz1->rysujgracza(window);
				window.draw(rectangle);
				for (int i = 0; i < 3; i++){
					if (gracze[i] != NULL) window.draw(*gracze[i]);
				}
				mapa.rysowaniemapy(window);
				window.display();
			}
		}
	}
	return 0;
}


//serwer wysyla mape, pozycje graczy, pociskow
//gracz wysyla pozycje i czy oddal strzal
//gracz ma: sprite'y na graczy i circleshape na pocisk i zbior punktow gdzie je malowac