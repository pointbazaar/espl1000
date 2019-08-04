/* Pixmap Class */

#ifndef PIXMAP_HPP
#define PIXMAP_HPP

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

#define PIXEL_SET_FAILURE -1
#define PIXEL_SET_SUCCESS 0

#define PIXMAP_HEIGHT 7
#define PIXMAP_WIDTH 21
#define DEFAULT_R 18
#define DEFAULT_G 30
#define DEFAULT_B 49

using namespace std;

class Pixmap {

public:
	Pixmap();

	int set_rgb(int value, int x, int y, char rgb_mode);
	int save(string path);
	void show();

private:
	int field[PIXMAP_HEIGHT][PIXMAP_WIDTH];
	
};

#endif