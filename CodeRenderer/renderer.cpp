/* Implementation of Renderer Class */

// C++ Headers
#include <fstream>
#include <list>
#include <iterator>

// Own Headers
#include "renderer.hpp"
#include "pixmap.hpp"

Renderer::Renderer(int H, int W) {

	this->height = H;
	this->width = W;
}

Pixmap Renderer::to_ppm(string text, int row) {

	Pixmap pixmap;	
	
	ifstream bit_file;

	string line = ""; int line_index = 0;

	int r = 255; int g = 255; int b = 255;

	for(int i = 0; i < text.length(); i++) {

		// if text[i] belongs to keyword -> get color
		// else get standard color		

		string font_path = "/home/alex/dragon/font/";
		font_path += text[i];
		font_path += ".dragonchar";

		bit_file.open(font_path);

		if(bit_file.good() == false) {

			pixmap.set_rgb(-1, 0, 0, 'r');
			return pixmap;
		}		

		while(getline(bit_file, line)) {

			for(int j = 0; j < line.length(); j++) {

				if(line[j] == '0')
					continue;

				pixmap.set_rgb(r, line_index, (i*21) + j, 'r');
				pixmap.set_rgb(g, line_index, (i*21) + j, 'g');
				pixmap.set_rgb(b, line_index, (i*21) + j, 'b');
			}

			line_index += 1;
		}

		bit_file.close();
	}		
	
	return pixmap;
}

