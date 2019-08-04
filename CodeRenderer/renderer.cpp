/* Implementation of Renderer Class */

// C++ Headers
#include <fstream>
#include <list>
#include <iterator>

// Own Headers
#include "renderer.hpp"
#include "pixmap.hpp"

#define PATH "/home/milo/Schreibtisch/Code/Projects/CodeRender/test"

Renderer::Renderer(int H, int W) {

	this->height = H;
	this->width = W;
}

Pixmap Renderer::to_ppm(string text, int row) {

	Pixmap pixmap;	

	ifstream bit_file(PATH);

	if(bit_file.good() == false) {
	
		pixmap.set_rgb(-1, 0, 0, 'r');
		return pixmap;
	}

	for(int i = 0; i < text.length(); i++) {

		// if text[i] belongs to keyword -> get color
		// else get standard color

		// mapping color to RGB

		// select correct Bitmap File and open (based on text[i])
	}

	// saving color of letter...right now without syntax highlighting
	int r = 255; int g = 255; int b = 255;

	string line = ""; int line_index = 0;

	while(getline(bit_file, line)) {

		for(int i = 0; i < line.length(); i++) {

			if(line[i] == '0')
				continue;

			pixmap.set_rgb(r, line_index, i, 'r');
			pixmap.set_rgb(g, line_index, i, 'g');
			pixmap.set_rgb(b, line_index, i, 'b');																			
		}

		line_index += 1;
	}

	bit_file.close();		
	
	return pixmap;
}

