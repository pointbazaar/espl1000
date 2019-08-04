/* Pixmap Class */

#ifndef PIXMAP_HPP
#define PIXMAP_HPP

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

#define PIXEL_SET_FAILURE -1
#define PIXEL_SET_SUCCESS 0

//means 40 characters maximum, per line
#define BP_HEIGHT 7
#define BP_WIDTH 280

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
	int field[BP_HEIGHT][BP_WIDTH * 3];
	
};

#endif