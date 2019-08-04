/* Renderer Class */

#ifndef RENDERER_HPP
#define RENDERER_HPP

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

#include "pixmap.hpp"

#define PPM_FAILURE -1
#define PPM_SUCCESS 0

using namespace std;

class Renderer {

public:
	Renderer(int H, int W);
	Pixmap to_ppm(string text, int row, int cursor_position);

private:
	int height;
	int width;
};

#endif
