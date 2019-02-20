#include <SDL.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	// állítsuk be, hogy kilépés elõtt hívja meg a rendszer az exitProgram() függvényt
	//atexit( exitProgram );

	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjünk ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							640,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							350,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == nullptr)
	{
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: töröljük az ablak háttérszínét és várjunk 2 másodpercet
	//

	// aktuális rajzolási szín legyen fekete és töröljük az aktuális rajzolási színnel az ablak kliensterületét
	SDL_SetRenderDrawColor(	ren,	// melyik renderelõnek állítjuk be az aktuális rajzolási színét
							0,	// vörös intenzitás - 8 bites elõjel nélküli egész szám
							0,		// zöld intenzitás - 8 bites elõjel nélküli egész szám
							0,		// kék intenzitás - 8 bites elõjel nélküli egész szám
							255);	// átlátszóság - 8 bites elõjel nélküli egész szám
	SDL_RenderClear(ren);

	// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
	SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
							0,		// piros
							255,	// zöld
							0,		// kék
							255);	// átlátszatlanság

	SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
						10, 10, // vonal kezdõpontjának (x,y) koordinátái
						10, 60);// vonal végpontjának (x,y) koordinátái

	// 1. feladat: egészítsük ki a fenti vonalat egy H betûvé!

	SDL_RenderDrawLine(ren,
		10, 35,
		35, 35);

	SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
		35, 10, // vonal kezdõpontjának (x,y) koordinátái
		35, 60);

	// ...

	// 2. feladat: írjuk ki a "HELLO" szöveget a képernyõre! Ehhez használható a
	// SDL_RenderDrawLines( <renderer ptr>, <SDL_Point tömb>, <pontok száma>); parancs!

	static SDL_Point letterE[7] = {
		{70, 10},
		{45, 10},
		{45, 35},
		{70, 35},
		{45, 35},
		{45, 60},
		{70, 60}
	};

	SDL_RenderDrawLines(ren, letterE, 7);

	static SDL_Point letterL1[3] = {
		{80, 10},
		{80, 60},
		{105, 60}
	};

	SDL_RenderDrawLines(ren, letterL1, 3);

	static SDL_Point letterL2[3] = {
		{115, 10},
		{115, 60},
		{140, 60}
	};

	SDL_RenderDrawLines(ren, letterL2, 3);

	SDL_Rect* rect = new SDL_Rect();
	rect->x = 150;
	rect->y = 10;
	rect->w = 26;
	rect->h = 51;

	SDL_RenderDrawRect(ren, rect);

	// 3. feladat: 2 másodpercenként változzon a háttér színe! Elõször legyen piros, aztán zöld és végül kék,
	// majd lépjen ki a program!

	SDL_RenderPresent(ren);
	SDL_Delay(4000);
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
	SDL_Delay(2000);
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
	SDL_Delay(2000);
	SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
	SDL_RenderClear(ren);

	// jelenítsük meg a backbuffer tartalmát
	SDL_RenderPresent(ren);

	// várjunk 2 másodpercet
	exitProgram();

	//
	// 4. lépés: lépjünk ki

	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}