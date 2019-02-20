#include <SDL.h>
#include <iostream>
#include <math.h>

class MyCircle
{
public:

	int radius;
	int x;
	int y;
	int new_x;
	int new_y;
	int old_x;
	int old_y;
	float step;

	MyCircle() {
		this->radius = 50;
		this->x = 0;
		this->y = 0;
		this->new_x = 0;
		this->new_y = 0;
		this->old_x = 0;
		this->old_y = 0;
		this->step = (M_PI * 2) / 50;
	}
};

void exitProgram()
{
	std::cout << "Kilepeshez nyomj meg egy billentyut..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
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
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
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
	// 4. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;

	float elapsed = SDL_GetTicks();
	bool rectColor = true;

	MyCircle circle = MyCircle();
	/*circle.radius = 50;
	circle.x = mouseX;
	circle.y = mouseY;
	circle.new_x = 0;
	circle.new_y = 0;
	circle.old_x = 0;
	circle.old_y = 0;
	circle.step = (M_PI * 2) / 50;*/

	while (!quit)
	{
		// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
					quit = true;
				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				// egérgomb felengedésének eseménye; a felengedett gomb a ev.button.button -ban található
				// a lehetséges gombok a következõek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				if (ev.button.button == SDL_BUTTON_LEFT) {
					rectColor = true;
					std::cout << "Left" << std::endl;
				}
				if (ev.button.button == SDL_BUTTON_RIGHT) {
					rectColor = false;
					std::cout << "Right" << std::endl;
				}
				break;
			}
		}

		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
								0,		// piros
								255,	// zöld
								0,		// kék
								255);	// átlátszatlanság

		SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
							0, 0, // vonal kezdõpontjának (x,y) koordinátái
							mouseX, mouseY);// vonal végpontjának (x,y) koordinátái
		
		// definiáljunk egy (mouseX, mouseY) középpontó, tengelyekkel párhuzamos oldalú
		// 20x20-as négyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - 10;
		cursor_rect.y = mouseY - 10;
		cursor_rect.w = 20;
		cursor_rect.h = 20;
		// legyen a kitöltési szín piros
		/*SDL_SetRenderDrawColor( ren, 255, 0, 0, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);*/

		// 1. feladat: az eltelt idõ függvényében periodikusan nõjjön és csökkenjen
		//    az egérmutató középpontjával kirajzolt négyszög

		float ticks = (SDL_GetTicks() - elapsed) / 1000;
		float x = cos(ticks);
		if (x < 0) x *= -1;
		//std::cout << x << std::endl;
		cursor_rect.w = 30 * x;
		cursor_rect.h = 30 * x;


		// 2. feladat: ha a user a bal egérgombot nyomja meg akkor a téglalap színe váltson pirosra,
		//    ha a jobb egérgombot, akkor kékre

		// A kod a kattintas erzekelesre a switch case-ben van feljebb ^^^^^^ (SDL_MOUSEBUTTONUP resz)
		if (rectColor) {
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		}
		else {
			SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
		}

		SDL_RenderFillRect(ren, &cursor_rect);

		// 3. feladat: rajzolj ki egy 50 sugarú körvonalat az egérmutató köré!
		// segítség: használd a SDL_RenderDrawLines()-t

		circle.x = mouseX;
		circle.y = mouseY;
		

		for (float theta = 0; theta < (M_PI * 2); theta += circle.step)
		{
			circle.new_x = circle.x + (circle.radius * cos(theta));
			circle.new_y = circle.y + (circle.radius * sin(theta));

			SDL_RenderDrawLine(ren, circle.old_x, circle.old_y,
				circle.new_x, circle.new_y);

			circle.old_x = circle.new_x;
			circle.old_y = circle.new_y;

		}

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}



	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}

