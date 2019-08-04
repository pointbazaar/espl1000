/* Implementation of Pixmap Class */

// C Headers
#include <cstring>

// C++ Headers
#include <fstream>

// Own Headers
#include "pixmap.hpp"

Pixmap::Pixmap() {

	for(int i = 0; i < PIXMAP_HEIGHT; i++) {

		for(int j = 0; j < PIXMAP_WIDTH; j++) {

			int col = j % 3;

			switch(col) {

				case 0: this->field[i][j] = DEFAULT_R; break;
				case 1: this->field[i][j] = DEFAULT_G; break;
				case 2: this->field[i][j] = DEFAULT_B; break;
				default: break;
			}
		}
	}
	
}

int Pixmap::set_rgb(int value, int x, int y, char rgb_mode) {

	if(!(x >= 0 && x < PIXMAP_HEIGHT))
		return PIXEL_SET_FAILURE;

	if(!(y >= 0 && y < PIXMAP_WIDTH))
		return PIXEL_SET_FAILURE;

	switch(rgb_mode) {

		case 'r': this->field[x][ (y*3) + 0 ] = value; break;
		case 'g': this->field[x][ (y*3) + 1 ] = value; break;
		case 'b': this->field[x][ (y*3) + 2 ] = value; break;
		default: return PIXEL_SET_FAILURE;
	}

	return PIXEL_SET_SUCCESS;
}

int Pixmap::save(string path) {
	
	ofstream write_file(path, ios::out | ios::trunc);

	string line = "";

	if(write_file.good() == false)
		return -1;

	write_file << "P3" << "\n" << PIXMAP_WIDTH << " " << PIXMAP_HEIGHT << "\n" << 255 << "\n";

	for(int i = 0; i < PIXMAP_HEIGHT; i++) {
		
		for(int j = 0; j < PIXMAP_WIDTH; j++) {
		
			write_file << setfill(' ') << setw(3) << this->field[i][j] << " ";

			if(j % 3 == 2)
				write_file << "  ";
		}

		write_file << "\n";
	}

	write_file.close();
}

void Pixmap::show() {

	cout << "P3" << "\n" << PIXMAP_WIDTH << " " << PIXMAP_HEIGHT << "\n" << 255 << "\n";

	for(int i = 0; i < PIXMAP_HEIGHT; i++) {

		for(int j = 0; j < PIXMAP_WIDTH; j++) {
		
			cout << setfill(' ') << setw(3) << this->field[i][j] << " ";

			if(j % 3 == 2)
				cout << "  ";
		}

		cout << "\n";
	}
}