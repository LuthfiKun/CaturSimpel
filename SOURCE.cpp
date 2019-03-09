//Daftar libray yang digunakan
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

//Inisiasi variabel ukuran window
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

//Inisiasi variabel ukuran tombol board
const int BIDAK_WIDTH = SCREEN_WIDTH * 1 / 8;
const int BIDAK_HEIGHT = SCREEN_HEIGHT * 1 / 8;

//Inisialisasi fungsi
int mousePosX(int x);
int mousePosY(int y);
bool init();
bool loadMedia();
SDL_Texture* renderTexture(std::string path);
void renderBoard();
bool cekBidak(SDL_Point oldPos, SDL_Point newPos);
bool gerakPion(SDL_Point oldPos, SDL_Point newPos);
bool gerakBenteng(SDL_Point oldPos, SDL_Point newPos);
bool gerakKuda(SDL_Point oldPos, SDL_Point newPos);
bool gerakMentri(SDL_Point oldPos, SDL_Point newPos);
bool gerakRatu(SDL_Point oldPos, SDL_Point newPos);
bool gerakRaja(SDL_Point oldPos, SDL_Point newPos);
bool hitamSkak();
bool putihSkak();
bool skak();
void close();

//Inisiasi variabel untuk front-end
SDL_Window* mainWindow = NULL;
SDL_Renderer* mainWindowRenderer = NULL;
SDL_Rect renderPlace;

//Inisiasi texture
SDL_Texture* hitamPion;
SDL_Texture* hitamBenteng;
SDL_Texture* hitamKuda;
SDL_Texture* hitamMentri;
SDL_Texture* hitamRatu;
SDL_Texture* hitamRaja;

SDL_Texture* putihPion;
SDL_Texture* putihBenteng;
SDL_Texture* putihKuda;
SDL_Texture* putihMentri;
SDL_Texture* putihRatu;
SDL_Texture* putihRaja;

//Inisiasi variabel untuk posisi bidak
int pos[8][8] = {
	{ 2, 1, 0, 0, 0, 0, -1, -2 },
	{ 3, 1, 0, 0, 0, 0, -1, -3 },
	{ 4, 1, 0, 0, 0, 0, -1, -4 },
	{ 5, 1, 0, 0, 0, 0, -1, -5 },
	{ 6, 1, 0, 0, 0, 0, -1, -6 },
	{ 4, 1, 0, 0, 0, 0, -1, -4 },
	{ 3, 1, 0, 0, 0, 0, -1, -3 },
	{ 2, 1, 0, 0, 0, 0, -1, -2 }
};

//Checkpoint gerak bidak
int savedPos[2][3] = {
	{ 0, 0, 0 },
	{ 0, 0, 0 },
};

bool putihLukir = true;
bool hitamLukir = true;
bool specialMove = false;

//Input mouse X axis
int mousePosX(int x) {
	int posX;

	if (SCREEN_WIDTH * 0 / 8 < x && x < (SCREEN_WIDTH * 0 / 8) + BIDAK_WIDTH) {
		posX = 0;
	}
	else if (SCREEN_WIDTH * 1 / 8 < x && x < (SCREEN_WIDTH * 1 / 8) + BIDAK_WIDTH) {
		posX = 1;
	}
	else if (SCREEN_WIDTH * 2 / 8 < x && x < (SCREEN_WIDTH * 2 / 8) + BIDAK_WIDTH) {
		posX = 2;
	}
	else if (SCREEN_WIDTH * 3 / 8 < x && x < (SCREEN_WIDTH * 3 / 8) + BIDAK_WIDTH) {
		posX = 3;
	}
	else if (SCREEN_WIDTH * 4 / 8 < x && x < (SCREEN_WIDTH * 4 / 8) + BIDAK_WIDTH) {
		posX = 4;
	}
	else if (SCREEN_WIDTH * 5 / 8 < x && x < (SCREEN_WIDTH * 5 / 8) + BIDAK_WIDTH) {
		posX = 5;
	}
	else if (SCREEN_WIDTH * 6 / 8 < x && x < (SCREEN_WIDTH * 6 / 8) + BIDAK_WIDTH) {
		posX = 6;
	}
	else if (SCREEN_WIDTH * 7 / 8 < x && x < (SCREEN_WIDTH * 7 / 8) + BIDAK_WIDTH) {
		posX = 7;
	}

	return posX;
}

//Input mouse Y axis
int mousePosY(int y) {
	int posY;

	if (SCREEN_HEIGHT * 0 / 8 < y && y < (SCREEN_HEIGHT * 0 / 8) + BIDAK_HEIGHT) {
		posY = 0;
	}
	else if (SCREEN_HEIGHT * 1 / 8 < y && y < (SCREEN_HEIGHT * 1 / 8) + BIDAK_HEIGHT) {
		posY = 1;
	}
	else if (SCREEN_HEIGHT * 2 / 8 < y && y < (SCREEN_HEIGHT * 2 / 8) + BIDAK_HEIGHT) {
		posY = 2;
	}
	else if (SCREEN_HEIGHT * 3 / 8 < y && y < (SCREEN_HEIGHT * 3 / 8) + BIDAK_HEIGHT) {
		posY = 3;
	}
	else if (SCREEN_HEIGHT * 4 / 8 < y && y < (SCREEN_HEIGHT * 4 / 8) + BIDAK_HEIGHT) {
		posY = 4;
	}
	else if (SCREEN_HEIGHT * 5 / 8 < y && y < (SCREEN_HEIGHT * 5 / 8) + BIDAK_HEIGHT) {
		posY = 5;
	}
	else if (SCREEN_HEIGHT * 6 / 8 < y && y < (SCREEN_HEIGHT * 6 / 8) + BIDAK_HEIGHT) {
		posY = 6;
	}
	else if (SCREEN_HEIGHT * 7 / 8 < y && y < (SCREEN_HEIGHT * 7 / 8) + BIDAK_HEIGHT) {
		posY = 7;
	}

	return posY;
}

//Inisiasi program
bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Gagal menginisiasi SDL! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		mainWindow = SDL_CreateWindow("Catur v0.28", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mainWindow == NULL) {
			printf("Gagal membuat window! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			mainWindowRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
			if (mainWindowRenderer == NULL) {
				printf("Gagal membuat renderer! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("Gagal menginisiasi SDL_image! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				else {
					SDL_SetRenderDrawColor(mainWindowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(mainWindowRenderer);

					SDL_RenderPresent(mainWindowRenderer);
				}
			}
		}
	}

	return success;
}

//Fungsi memuat media yang diperlukan
bool loadMedia() {
	bool success = true;

	//Memuat gambar bidak hitam
	hitamPion = renderTexture("img/hitamPion.png");
	hitamBenteng = renderTexture("img/hitamBenteng.png");
	hitamKuda = renderTexture("img/hitamKuda.png");
	hitamMentri = renderTexture("img/hitamMentri.png");
	hitamRatu = renderTexture("img/hitamRatu.png");
	hitamRaja = renderTexture("img/hitamRaja.png");

	//Memuat gambar bidak putih
	putihPion = renderTexture("img/putihPion.png");
	putihBenteng = renderTexture("img/putihBenteng.png");
	putihKuda = renderTexture("img/putihKuda.png");
	putihMentri = renderTexture("img/putihMentri.png");
	putihRatu = renderTexture("img/putihRatu.png");
	putihRaja = renderTexture("img/putihRaja.png");

	return success;
}

SDL_Texture* renderTexture(std::string path) {
	SDL_Texture* newTexture = NULL;
	
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Gagal memuat gambar %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(mainWindowRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Gagal membuat texture dari gambar %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

//Fungsi untuk merender papan dan bidak
void renderBoard() {

	//Render latar belakang
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if ((x + y) % 2 == 0) {
				SDL_Rect board = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
				SDL_SetRenderDrawColor(mainWindowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(mainWindowRenderer, &board);
			}
			else if ((x + y) % 2 != 0) {
				SDL_Rect board = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
				SDL_SetRenderDrawColor(mainWindowRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(mainWindowRenderer, &board);
			}
		}
	}

	//Render bidak
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (pos[x][y] > 0) {
				switch (pos[x][y]) {
				case 1:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, hitamPion, NULL, &renderPlace);
					break;
				case 2:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, hitamBenteng, NULL, &renderPlace);
					break;
				case 3:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, hitamKuda, NULL, &renderPlace);
					break;
				case 4:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, hitamMentri, NULL, &renderPlace);
					break;
				case 5:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, hitamRatu, NULL, &renderPlace);
					break;
				case 6:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, hitamRaja, NULL, &renderPlace);
					break;
				}
			}
			else {
				switch (pos[x][y]) {
				case -1:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, putihPion, NULL, &renderPlace);
					break;
				case -2:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, putihBenteng, NULL, &renderPlace);
					break;
				case -3:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, putihKuda, NULL, &renderPlace);
					break;
				case -4:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, putihMentri, NULL, &renderPlace);
					break;
				case -5:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, putihRatu, NULL, &renderPlace);
					break;
				case -6:
					renderPlace = { SCREEN_WIDTH * x / 8, SCREEN_HEIGHT * y / 8, BIDAK_WIDTH, BIDAK_HEIGHT };
					SDL_RenderCopy(mainWindowRenderer, putihRaja, NULL, &renderPlace);
					break;
				}
			}
		}
	}
}

//Fungsi gerak bidak
bool cekBidak(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;

	if (pos[newPos.x][newPos.y] == 6 || pos[newPos.x][newPos.y] == -6) {
		printf("Gerak normal tidak valid!\n");
	}
	else {
		//Gerak pion
		if (pos[oldPos.x][oldPos.y] == 1 || pos[oldPos.x][oldPos.y] == -1) {
			if (gerakPion(oldPos, newPos)) {
				validMove = true;
			}
		}

		//Gerak benteng
		else if (pos[oldPos.x][oldPos.y] == 2 || pos[oldPos.x][oldPos.y] == -2) {
			if (gerakBenteng(oldPos, newPos)) {
				validMove = true;
			}
		}

		//Gerak kuda
		else if (pos[oldPos.x][oldPos.y] == 3 || pos[oldPos.x][oldPos.y] == -3) {
			if (gerakKuda(oldPos, newPos)) {
				validMove = true;
			}
		}

		//Gerak mentri
		else if (pos[oldPos.x][oldPos.y] == 4 || pos[oldPos.x][oldPos.y] == -4) {
			if (gerakMentri(oldPos, newPos)) {
				validMove = true;
			}
		}

		//Gerak ratu
		else if (pos[oldPos.x][oldPos.y] == 5 || pos[oldPos.x][oldPos.y] == -5) {
			if (gerakRatu(oldPos, newPos)) {
				validMove = true;
			}
		}

		//Gerak raja
		else if (pos[oldPos.x][oldPos.y] == 6 || pos[oldPos.x][oldPos.y] == -6) {
			if (gerakRaja(oldPos, newPos)) {
				validMove = true;
			}
		}
	}

	return validMove;
}

void cekLukir() {
	if (savedPos[0][2] == 6 || (savedPos[0][2] == 2 && savedPos[0][0] == 7 && savedPos[0][1] == 0)) {
		hitamLukir = false;
	}
	else if (savedPos[0][2] == -6 || (savedPos[0][2] == -2 && savedPos[0][0] == 7 && savedPos[0][1] == 7)) {
		putihLukir = false;
	}
}

bool gerakPion(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;
	
	if (pos[oldPos.x][oldPos.y] == -1) {
		if (oldPos.y == 6 && newPos.x == oldPos.x && newPos.y == oldPos.y - 2 && pos[newPos.x][newPos.y] == 0) {
			validMove = true;
		}
		else if (newPos.x == oldPos.x && newPos.y == oldPos.y - 1 && pos[newPos.x][newPos.y] == 0) {
			validMove = true;
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y - 1 && pos[newPos.x][newPos.y] != 0) {
			validMove = true;
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y - 1 && pos[newPos.x][newPos.y] != 0) {
			validMove = true;
		}
		else {
		}
	}
	if (pos[oldPos.x][oldPos.y] == 1) {
		if (oldPos.y == 1 && newPos.x == oldPos.x && newPos.y == oldPos.y + 2 && pos[newPos.x][newPos.y] == 0) {

			validMove = true;
		}
		else if (newPos.x == oldPos.x && newPos.y == oldPos.y + 1 && pos[newPos.x][newPos.y] == 0) {
			validMove = true;
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y + 1 && pos[newPos.x][newPos.y] != 0) {
			validMove = true;
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y + 1 && pos[newPos.x][newPos.y] != 0) {
			validMove = true;
		}
		else {
		}
	}

	return validMove;
}

bool gerakBenteng(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;
	
	if (pos[oldPos.x][oldPos.y] == 2) {
		for (int i = 1; i < 8; i++) {
			if (newPos.x == oldPos.x && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x][oldPos.y + c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x][oldPos.y - c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else {
				continue;
			}
		}
	}
	if (pos[oldPos.x][oldPos.y] == -2) {
		for (int i = 1; i < 8; i++) {
			if (newPos.x == oldPos.x && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x][oldPos.y + c] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x][oldPos.y - c] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else {
				continue;
			}
		}
	}

	return validMove;
}

bool gerakKuda(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;

	if (pos[oldPos.x][oldPos.y] == 3) {
		if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y + 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y + 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y - 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y - 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 2 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 2 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 2 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 2 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
	}
	if (pos[oldPos.x][oldPos.y] == -3) {
		if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y + 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y + 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y - 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y - 2) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 2 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 2 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 2 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 2 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
	}

	return validMove;
}

bool gerakMentri(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;

	if (pos[oldPos.x][oldPos.y] == 4) {
		for (int i = 1; i < 8; i++) {
			if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y + c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y + c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y - c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y - c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else {
				continue;
			}
		}
	}
	if (pos[oldPos.x][oldPos.y] == -4) {
		for (int i = 1; i < 8; i++) {
			if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y + c] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y + c] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y - c] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y - c] < 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] > 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else {
				continue;
			}
		}
	}

	return validMove;
}

bool gerakRatu(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;

	if (pos[oldPos.x][oldPos.y] == 5) {
		for (int i = 1; i < 8; i++) {
			if (newPos.x == oldPos.x && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x][oldPos.y + c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x][oldPos.y - c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y + c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y + c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x + c][oldPos.y - c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i) {
				for (int c = 1; c < 8; c++) {
					if (pos[oldPos.x - c][oldPos.y - c] > 0) {
						printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
						i = 8;
						break;
					}
					else {
						if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
							validMove = true;
							i = 8;
							break;
						}
						else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] < 0) {
							validMove = true;
							i = 8;
							break;
						}
						else {
							continue;
						}
					}
				}
			}
			else {
				continue;
			}
		}
	}
	if (pos[oldPos.x][oldPos.y] == -5) {
		for (int i = 1; i < 8; i++) {
			for (int i = 1; i < 8; i++) {
				if (newPos.x == oldPos.x && newPos.y == oldPos.y + i) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x][oldPos.y + c] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x][oldPos.y - c] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x + c][oldPos.y] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x - c][oldPos.y] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x + c][oldPos.y + c] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x + c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x - c][oldPos.y + c] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y + i && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x + c][oldPos.y - c] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x + i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i) {
					for (int c = 1; c < 8; c++) {
						if (pos[oldPos.x - c][oldPos.y - c] < 0) {
							printf("Terhalang! di ( %d,%d )\n", oldPos.x - c, oldPos.y + c);
							i = 8;
							break;
						}
						else {
							if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] == 0) {
								validMove = true;
								i = 8;
								break;
							}
							else if (newPos.x == oldPos.x - i && newPos.y == oldPos.y - i && pos[newPos.x][newPos.y] > 0) {
								validMove = true;
								i = 8;
								break;
							}
							else {
								continue;
							}
						}
					}
				}
				else {
					continue;
				}
			}
		}
	}

	return validMove;
}

bool gerakRaja(SDL_Point oldPos, SDL_Point newPos) {
	bool validMove = false;

	if (pos[oldPos.x][oldPos.y] == 6) {
		if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] < 0) {
				validMove = true;
			}
		}
		else if (hitamLukir && oldPos.x == 4 && oldPos.y == 0 && newPos.x == 7 && newPos.y == 0) {
			if (pos[oldPos.x + 1][oldPos.y] == 0 && pos[oldPos.x + 2][oldPos.y] == 0) {
				if (!hitamSkak()) {
					validMove = true;
					specialMove = true;
				}
			}
		}
	}
	if (pos[oldPos.x][oldPos.y] == -6) {
		if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x + 1 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y + 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x - 1 && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (newPos.x == oldPos.x && newPos.y == oldPos.y - 1) {
			if (pos[newPos.x][newPos.y] == 0) {
				validMove = true;
			}
			else if (pos[newPos.x][newPos.y] > 0) {
				validMove = true;
			}
		}
		else if (hitamLukir && oldPos.x == 4 && oldPos.y == 7 && newPos.x == 7 && newPos.y == 7) {
			if (pos[oldPos.x + 1][oldPos.y] == 0 && pos[oldPos.x + 2][oldPos.y] == 0) {
				if (!putihSkak()) {
					validMove = true;
					specialMove = true;
				}
			}
		}
	}

	return validMove;
}

//Fungsi cek skak
bool hitamSkak() {
	bool checkmate = false;

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (pos[x][y] == 6) {
				for (int i = 1; x - i >= 0 && x - i <= 7 && y + i >= 0 && y + i <= 7; i++) {
					if (pos[x - i][y + i] == 0) {
						continue;
					}
					else if (pos[x - i][y + i] == -4 || pos[x - i][y + i] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x + i >= 0 && x + i <= 7 && y + i >= 0 && y + i <= 7; i++) {
					if (pos[x + i][y + i] == 0) {
						continue;
					}
					else if (pos[x + i][y + i] == -4 || pos[x + i][y + i] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x + i >= 0 && x + i <= 7 && y - i >= 0 && y - i <= 7; i++) {
					if (pos[x + i][y - i] == 0) {
						continue;
					}
					else if (pos[x + i][y - i] == -4 || pos[x + i][y - i] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x - i >= 0 && x - i <= 7 && y - i >= 0 && y - i <= 7; i++) {
					if (pos[x - i][y - i] == 0) {
						continue;
					}
					else if (pos[x - i][y - i] == -4 || pos[x - i][y - i] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x + i >= 0 && x + i <= 7 && y >= 0 && y <= 7; i++) {
					if (pos[x + i][y] == 0) {
						continue;
					}
					else if (pos[x + i][y] == -2 || pos[x + i][y] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x - i >= 0 && x - i <= 7 && y >= 0 && y <= 7; i++) {
					if (pos[x - i][y] == 0) {
						continue;
					}
					else if (pos[x - i][y] == -2 || pos[x - i][y] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x >= 0 && x <= 7 && y - i >= 0 && y - i<= 7; i++) {
					if (pos[x][y - i] == 0) {
						continue;
					}
					else if (pos[x][y - i] == -2 || pos[x][y - 1] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x >= 0 && x <= 7 && y + i >= 0 && y + i <= 7; i++) {
					if (pos[x][y + i] == 0) {
						continue;
					}
					else if (pos[x][y + i] == -2 || pos[x][y + i] == -5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				if (x + 1 >= 0 && x + 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x + 1][y + 1] == -1) {
						checkmate = true;
					}
				}
				if (x - 1 >= 0 && x - 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x - 1][y + 1] == -1) {
						checkmate = true;
					}
				}
				if (x + 1 >= 0 && x + 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x + 1][y + 2] == -3) {
						checkmate = true;
					}
				}
				if (x - 1 >= 0 && x - 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x - 1][y + 2] == -3) {
						checkmate = true;
					}
				}
				if (x + 1 >= 0 && x + 1 <= 7 && y - 2 >= 0 && y - 2 <= 7) {
					if (pos[x + 1][y - 2] == -3) {
						checkmate = true;
					}
				}
				if (x - 1 >= 0 && x - 1 <= 7 && y - 2 >= 0 && y - 2 <= 7) {
					if (pos[x - 1][y - 2] == -3) {
						checkmate = true;
					}
				}
				if (x + 2 >= 0 && x + 2 <= 7 && y + 1 >= 0 && y + 1 <= 7) {
					if (pos[x + 2][y + 1] == -3) {
						checkmate = true;
					}
				}
				if (x - 2 >= 0 && x - 2 <= 7 && y + 1 >= 0 && y + 1 <= 7) {
					if (pos[x - 2][y + 1] == -3) {
						checkmate = true;
					}
				}
				if (x + 2 >= 0 && x + 2 <= 7 && y - 1 >= 0 && y - 1 <= 7) {
					if (pos[x + 2][y - 1] == -3) {
						checkmate = true;
					}
				}
				if (x - 2 >= 0 && x - 2 <= 7 && y - 1 >= 0 && y - 1 <= 7) {
					if (pos[x - 2][y - 1] == -3) {
						checkmate = true;
					}
				}
			}
			else {
				continue;
			}
		}
	}

	if (checkmate == true) {
		printf("Hitam skak\n");
	}

	return checkmate;
}

bool putihSkak() {
	bool checkmate = false;

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (pos[x][y] == -6) {
				for (int i = 1; x - i >= 0 && x - i <= 7 && y + i >= 0 && y + i <= 7; i++) {
					if (pos[x - i][y + i] == 0) {
						continue;
					}
					else if (pos[x - i][y + i] == 4 || pos[x - i][y + i] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x + i >= 0 && x + i <= 7 && y + i >= 0 && y + i <= 7; i++) {
					if (pos[x + i][y + i] == 0) {
						continue;
					}
					else if (pos[x + i][y + i] == 4 || pos[x + i][y + i] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x + i >= 0 && x + i <= 7 && y - i >= 0 && y - i <= 7; i++) {
					if (pos[x + i][y - i] == 0) {
						continue;
					}
					else if (pos[x + i][y - i] == 4 || pos[x + i][y - i] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x - i >= 0 && x - i <= 7 && y - i >= 0 && y - i <= 7; i++) {
					if (pos[x - i][y - i] == 0) {
						continue;
					}
					else if (pos[x - i][y - i] == 4 || pos[x - i][y - i] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x + i >= 0 && x + i <= 7 && y >= 0 && y <= 7; i++) {
					if (pos[x + i][y] == 0) {
						continue;
					}
					else if (pos[x + i][y] == 2 || pos[x + i][y] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x - i >= 0 && x - i <= 7 && y >= 0 && y <= 7; i++) {
					if (pos[x - i][y] == 0) {
						continue;
					}
					else if (pos[x - i][y] == 2 || pos[x - i][y] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x >= 0 && x <= 7 && y - i >= 0 && y - i <= 7; i++) {
					if (pos[x][y - i] == 0) {
						continue;
					}
					else if (pos[x][y - i] == 2 || pos[x][y - 1] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				for (int i = 1; x >= 0 && x <= 7 && y + i >= 0 && y + i <= 7; i++) {
					if (pos[x][y + i] == 0) {
						continue;
					}
					else if (pos[x][y + i] == 2 || pos[x][y + i] == 5) {
						checkmate = true;
						break;
					}
					else {
						break;
					}
				}
				if (x + 1 >= 0 && x + 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x + 1][y + 1] == 1) {
						checkmate = true;
					}
				}
				if (x - 1 >= 0 && x - 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x - 1][y + 1] == 1) {
						checkmate = true;
					}
				}
				if (x + 1 >= 0 && x + 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x + 1][y + 2] == 3) {
						checkmate = true;
					}
				}
				if (x - 1 >= 0 && x - 1 <= 7 && y + 2 >= 0 && y + 2 <= 7) {
					if (pos[x - 1][y + 2] == 3) {
						checkmate = true;
					}
				}
				if (x + 1 >= 0 && x + 1 <= 7 && y - 2 >= 0 && y - 2 <= 7) {
					if (pos[x + 1][y - 2] == 3) {
						checkmate = true;
					}
				}
				if (x - 1 >= 0 && x - 1 <= 7 && y - 2 >= 0 && y - 2 <= 7) {
					if (pos[x - 1][y - 2] == 3) {
						checkmate = true;
					}
				}
				if (x + 2 >= 0 && x + 2 <= 7 && y + 1 >= 0 && y + 1 <= 7) {
					if (pos[x + 2][y + 1] == 3) {
						checkmate = true;
					}
				}
				if (x - 2 >= 0 && x - 2 <= 7 && y + 1 >= 0 && y + 1 <= 7) {
					if (pos[x - 2][y + 1] == 3) {
						checkmate = true;
					}
				}
				if (x + 2 >= 0 && x + 2 <= 7 && y - 1 >= 0 && y - 1 <= 7) {
					if (pos[x + 2][y - 1] == 3) {
						checkmate = true;
					}
				}
				if (x - 2 >= 0 && x - 2 <= 7 && y - 1 >= 0 && y - 1 <= 7) {
					if (pos[x - 2][y - 1] == 3) {
						checkmate = true;
					}
				}
			}
			else {
				continue;
			}
		}
	}

	if (checkmate == true) {
		printf("Putih skak\n");
	}

	return checkmate;
}

bool skak() {
	bool checkmate = false;

	if (putihSkak() || hitamSkak()) {
		checkmate = true;
	}

	return checkmate;
}

//Fungsi keluar
void close() {
	SDL_DestroyTexture(hitamPion);
	SDL_DestroyTexture(hitamBenteng);
	SDL_DestroyTexture(hitamKuda);
	SDL_DestroyTexture(hitamMentri);
	SDL_DestroyTexture(hitamRatu);
	SDL_DestroyTexture(hitamRaja);
	SDL_DestroyTexture(putihPion);
	SDL_DestroyTexture(putihBenteng);
	SDL_DestroyTexture(putihKuda);
	SDL_DestroyTexture(putihMentri);
	SDL_DestroyTexture(putihRatu);
	SDL_DestroyTexture(putihRaja);

	hitamPion = NULL;
	hitamBenteng = NULL;
	hitamKuda = NULL;
	hitamMentri = NULL;
	hitamRatu = NULL;
	hitamRaja = NULL;
	putihPion = NULL;
	putihBenteng = NULL;
	putihKuda = NULL;
	putihMentri = NULL;
	putihRatu = NULL;
	putihRaja = NULL;

	SDL_DestroyRenderer(mainWindowRenderer);
	mainWindowRenderer = NULL;
	SDL_DestroyWindow(mainWindow);
	mainWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

//Fungsi utama. Main dan secondary loop disini.
int main(int argc, char* args[]) {
	if (!init()) {
		printf("Gagal menginisiasi program!\n");
	}
	else {
		if (!loadMedia()) {
			printf("Gagal memuat media!\n");
		}
		else {
			bool quit = false;
			SDL_Event mainInput;
			SDL_Point mInput, oldPos, newPos;
			bool whiteTurn = true;
			bool blackTurn = false;
			oldPos = { -1, -1 };
			newPos = { -1, -1 };

			while (!quit && (whiteTurn || blackTurn)) {
				SDL_RenderClear(mainWindowRenderer);
				renderBoard();
				SDL_RenderPresent(mainWindowRenderer);
				while (SDL_PollEvent(&mainInput) != 0) {
					if (mainInput.type == SDL_QUIT) {
						quit = true;
						whiteTurn = false;
						blackTurn = false;
					}
					if (mainInput.type == SDL_MOUSEBUTTONDOWN) {
						SDL_GetMouseState(&mInput.x, &mInput.y);
						oldPos.x = mousePosX(mInput.x);
						oldPos.y = mousePosY(mInput.y);
						mInput = { -1, -1 };
						//Jika kotak yang diklik tidak ada bidak.
						if (pos[oldPos.x][oldPos.y] == 0) {
							printf("Bidak tidak terdeteksi\n");
							oldPos = { -1, -1 };

							continue;
						}
						else {
							//Jika mengklik kotak berisi bidak putih sedang sekarang giliran hitam.
							if (pos[oldPos.x][oldPos.y] < 0 && !whiteTurn) {
								printf("Sekarang giliran bidak hitam\n");
								oldPos = { -1, -1 };

								continue;
							}
							//Jika mengklik kotak berisi bidak hitam sedang sekarang giliran putih.
							else if (pos[oldPos.x][oldPos.y] > 0 && !blackTurn) {
								printf("Sekarang giliran bidak putih\n");
								oldPos = { -1, -1 };

								continue;
							}
							else {
								while (whiteTurn || blackTurn) {
									while (SDL_PollEvent(&mainInput) != 0) {
										if (mainInput.type == SDL_QUIT) {
											whiteTurn = false;
											blackTurn = false;
											quit = true;
										}
										if (mainInput.type == SDL_MOUSEBUTTONDOWN) {
											SDL_GetMouseState(&mInput.x, &mInput.y);
											newPos.x = mousePosX(mInput.x);
											newPos.y = mousePosY(mInput.y);
											mInput = { -1, -1 };
											//Jika input pertama == input kedua, batal gerak.
											if (newPos.x == oldPos.x && newPos.y == oldPos.y) {
												printf("Bidak tidak bergerak\n");
												oldPos = { -1, -1 };
												newPos = { -1, -1 };

												break;
											}
											else {

												break;
											}
										}
									}
									if (newPos.x >= 0 || newPos.y >= 0) {
										//Jika input kedua tidak cocok dengan gerak bidak yang diperbolehkan pada input pertama.
										if (!cekBidak(oldPos, newPos)) {
											printf("Gerak bidak tidak valid\n");

											break;
										}
										else {
											if (specialMove) {
												savedPos[0][0] = { oldPos.x };
												savedPos[0][1] = { oldPos.y };
												savedPos[0][2] = { pos[oldPos.x][oldPos.y] };

												savedPos[1][0] = { newPos.x };
												savedPos[1][1] = { newPos.y };
												savedPos[1][2] = { pos[newPos.x][newPos.y] };

												pos[newPos.x - 1][newPos.y] = { pos[oldPos.x][oldPos.y] };
												pos[oldPos.x + 1][oldPos.y] = { pos[newPos.x][newPos.y] };
												pos[newPos.x][newPos.y] = { 0 };
												pos[oldPos.x][oldPos.y] = { 0 };

												specialMove = false;
											}
											else {
												savedPos[0][0] = { oldPos.x };
												savedPos[0][1] = { oldPos.y };
												savedPos[0][2] = { pos[oldPos.x][oldPos.y] };

												savedPos[1][0] = { newPos.x };
												savedPos[1][1] = { newPos.y };
												savedPos[1][2] = { pos[newPos.x][newPos.y] };

												pos[newPos.x][newPos.y] = { pos[oldPos.x][oldPos.y] };
												pos[oldPos.x][oldPos.y] = { 0 };

												oldPos = { -1, -1 };
												newPos = { -1, -1 };
											}
											if (!putihSkak() && whiteTurn && !blackTurn) {
												whiteTurn = false;
												blackTurn = true;
												printf("Bidak digerakkan dari ( %d, %d ) ke ( %d, %d )\n", savedPos[0][0], savedPos[0][1], savedPos[1][0], savedPos[1][1]);
												if (savedPos[1][1] == 0 && savedPos[0][2] == -1) {
													while (pos[savedPos[1][0]][savedPos[1][1]] == -1) {
														printf("Promosi bidak Pion (Benteng == -2; Kuda == -3; Mentri = -4; Ratu == -5): ");
														scanf_s("%d", &pos[savedPos[1][0]][savedPos[1][1]], 1);
														if (pos[savedPos[1][0]][savedPos[1][1]] <= -6 || pos[savedPos[1][0]][savedPos[1][1]] >= -1) {
															pos[savedPos[1][0]][savedPos[1][1]] = -1;
															continue;
														}
													}
												}
												printf("Berganti giliran ke hitam\n");
												cekLukir();
												break;
											}
											else if (!hitamSkak() && !whiteTurn && blackTurn) {
												whiteTurn = true;
												blackTurn = false;
												printf("Bidak digerakkan dari ( %d, %d ) ke ( %d, %d )\n", savedPos[0][0], savedPos[0][1], savedPos[1][0], savedPos[1][1]);
												cekLukir();
												if (savedPos[1][1] == 7 && savedPos[0][2] == 1) {
													while (pos[savedPos[1][0]][savedPos[1][1]] == 1) {
														printf("Promosi bidak Pion (Benteng == 2; Kuda == 3; Mentri = 4; Ratu == 5): ");
														scanf_s("%d", &pos[savedPos[1][0]][savedPos[1][1]], 1);
														if (pos[savedPos[1][0]][savedPos[1][1]] <= 1 || pos[savedPos[1][0]][savedPos[1][1]] >= 6) {
															pos[savedPos[1][0]][savedPos[1][1]] = 1;
															continue;
														}
													}
												}
												printf("Berganti giliran ke putih\n");
												break;
											}
											else {
												printf("Gerak tidak valid! SKAK!\n");
												pos[savedPos[0][0]][savedPos[0][1]] = { savedPos[0][2] };
												pos[savedPos[1][0]][savedPos[1][1]] = { savedPos[1][2] };
												break;
											}
										}
									}
									else {
										if (oldPos.x < 0 || oldPos.y < 0) {
											break;
										}
									}
								}
								if (skak()) {
									printf("Skak lur!\n");
								}
								newPos = { -1, -1 };
							}
						}
					}
				}
			}
		}

		close();

		return 0;
	}
}