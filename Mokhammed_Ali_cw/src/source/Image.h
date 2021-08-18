#include <stdint.h>
#include <cstdio>
#include <stdlib.h>

enum ImageType {
	PNG, JPG, BMP, TGA
};

struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;

	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType get_file_type(const char* filename);
	void print_img_info();
	Image& resize(int x, int y);

	Image& color_mask(float r, float g, float b);
	Image& change_color(int f_r, int f_g, int f_b, int to_r, int to_g, int to_b, int threshold = 0);
	Image& find_color(uint8_t srch_r, uint8_t srch_g, uint8_t srch_b, uint8_t border_r, uint8_t border_g, uint8_t border_b, uint8_t thickness);

	Image& overlay(const Image& source, int x, int y);
	Image& frame(int ptrn_num, int frame_size, int r, int g, int b);

	Image& crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
	Image& copy_area(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch, uint16_t to_x, uint16_t to_y);
};
