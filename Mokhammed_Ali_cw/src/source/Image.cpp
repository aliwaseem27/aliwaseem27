#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"


Image::Image(const char* filename) {
	if(read(filename)) {
		printf("\e[0;32mRead successfully\e[0m %s\n", filename);
		size = w*h*channels;
	}
	else {
		printf("\e[1;31mFailed to read\e[0m %s\n", filename);
		abort();
	}
}


Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
	size = w*h*channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
	memcpy(data, img.data, size);
}

Image::~Image() {
	stbi_image_free(data);
}

bool Image::read(const char* filename) {
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data != NULL;
}


bool Image::write(const char* filename) {
	ImageType type = get_file_type(filename);
	int success;
  switch (type) {
    case PNG:
      success = stbi_write_png(filename, w, h, channels, data, w*channels);
      break;
    case BMP:
      success = stbi_write_bmp(filename, w, h, channels, data);
      break;
    case JPG:
      success = stbi_write_jpg(filename, w, h, channels, data, 100);
      break;
    case TGA:
      success = stbi_write_tga(filename, w, h, channels, data);
      break;
  }
  return success != 0;
}


ImageType Image::get_file_type(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if(ext != nullptr) {
		if(strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if(strcmp(ext, ".jpg") == 0) {
			return JPG;
		}
		else if(strcmp(ext, ".bmp") == 0) {
			return BMP;
		}
		else if(strcmp(ext, ".tga") == 0) {
			return TGA;
		}
	}
	return PNG;
}

void Image::print_img_info(){
	printf(" ------------------\n");
	printf("| Width    | %5d |\n", w);
	printf("| Height   | %5d |\n", h);
	printf("| Channels | %5d |\n", channels);
	printf(" ------------------\n");
}


Image& Image::resize(int x, int y){

	int new_size = x * y * channels;
	uint8_t* extra_memory = new uint8_t[new_size];
	int success; 

	success = stbir_resize_uint8(data, w, h, 3, extra_memory, x, y, 0, channels);
	if(success){
		printf("memory allocated successfully\n");
	}
	printf("w => %d | h => %d | x => %d | y => %d | channels => %d \n", w, h, x, y, channels);

	w = x;
	h = y;
	size = new_size;

	delete[] extra_memory;
	write("new_sized_pattern.png");

	return *this;
}


Image& Image::color_mask(float r, float g, float b) {
	if(channels < 3) {
		printf("\e[31m[ERROR] Color mask requires at least 3 channels, but this image has %d channels\e[0m\n", channels);
	}
	else {
		for(int i = 0;i < size;i+=channels) {
			data[i] *= r;
			data[i+1] *= g;
			data[i+2] *= b;
		}
	}
	return *this;
}


Image& Image::change_color(int f_r, int f_g, int f_b, int to_r, int to_g, int to_b, int threshold){
	if(channels < 3) {
		printf("\e[31m[ERROR] Color mask requires at least 3 channels, but this image has %d channels\e[0m\n", channels);
	}
	else {
		int D_r, D_g, D_b;
		for(int i = 0; i < size; i += channels){

			D_r = abs(f_r - data[i]);
			D_g = abs(f_g - data[i+1]);
			D_b = abs(f_b - data[i+2]);

			if (D_r <= threshold && D_g <= threshold && D_b <= threshold){
				data[i] = BYTE_BOUND(to_r);
				data[i+1] = BYTE_BOUND(to_g);
				data[i+2] = BYTE_BOUND(to_b);
			}
		}
	}
	return *this;
}


Image& Image::find_color(uint8_t srch_r, uint8_t srch_g, uint8_t srch_b, uint8_t border_r, uint8_t border_g, uint8_t border_b, uint8_t thickness){
	int i, px, n;
	for(i = 0; i < size; i += channels){ // for every pixell in image
		if(data[i] == srch_r && data[i+1] == srch_g && data[i+2] == srch_b){ // if pixel equal color
			
			for(n = 1; n < thickness+1; n++){  // Upper Border
				px = i - (w * channels * n);   // Upper pixel
				if(px < 0){break;}
				if(data[px] == srch_r && data[px+1] == srch_g && data[px+2] == srch_b){break;}

				data[px]   = border_r;
				data[px+1] = border_g;
				data[px+2] = border_b;
			}

			for(n = 1; n < thickness+1; n++){  // Lower Border
				px = i + (w * channels * n);   // Lower pixel
				if(px > size){break;}
				if(data[px] == srch_r && data[px+1] == srch_g && data[px+2] == srch_b){break;}

				data[px]   = border_r;
				data[px+1] = border_g;
				data[px+2] = border_b;
			}

			for(n = 1; n < thickness+1; n++){  // Left Border
				px = i - (channels * n);       // Left pixel
				if(i % w == 0){break;}
				if(data[px] == srch_r && data[px+1] == srch_g && data[px+2] == srch_b){break;}

				data[px]   = border_r;
				data[px+1] = border_g;
				data[px+2] = border_b;
			}

			for(n = 1; n < thickness+1; n++){  // Right Border
				px = i + (channels * n);       // Right pixel
				if(px % w == 0){break;}
				if(data[px] == srch_r && data[px+1] == srch_g && data[px+2] == srch_b){break;}

				data[px]   = border_r;
				data[px+1] = border_g;
				data[px+2] = border_b;
			}
		}
	}
	return *this;
}


Image& Image::frame(int ptrn_num, int frame_size, int r, int g, int b){

	int edge;
	if(frame_size == 0){
		edge = 50;
	} else {
		edge = frame_size * 100;
	}

	char *pattern_choice = new char[29];
	char pattern_type[][3] = {"BF", "_1", "_2", "_3", "_4"};
	char pattern_size[][9] = {"_050.png","_100.png","_200.png","_300.png"};

	strncat(pattern_choice,"./patterns/pattern",29);
	strncat(pattern_choice, pattern_type[ptrn_num], 29);
	strncat(pattern_choice, pattern_size[frame_size], 29);

	Image pattern(pattern_choice);
	Image pattern_copy(pattern);
	pattern_copy.change_color(0,0,0,r,g,b);
	// pattern_copy.resize(edge, edge);

	int copies_x = w / edge;
	int copies_y = h / edge;
	int offset_x = w % edge * 0.5;
	int offset_y = h % edge * 0.5;

	if (w%edge == 0){
		copies_x = copies_x - 1;
		offset_x += edge/2;
	}
	if (h%edge == 0){
		copies_y = copies_y - 1;
		offset_y += edge/2;
	}	


	int n;
	for(n = 0; n < copies_x; n++){ // Upper Frame
		overlay(pattern_copy, offset_x + (n * edge), offset_y);
	}
	for(n = 0; n < copies_x; n++){ // Lower Frame
		overlay(pattern_copy, offset_x + (n * edge), offset_y + (copies_y-1) * edge);
	}
	for(n = 1; n < copies_y-1; n++){ // Left Frame
		overlay(pattern_copy, offset_x, offset_y + (n * edge));
	}
	for(n = 1; n < copies_y-1; n++){ // Right Frame
		overlay(pattern_copy, offset_x + (copies_x-1) * edge , offset_y + (n * edge));
	}
	return *this;
}


Image& Image::overlay(const Image& source, int x, int y) {

	uint8_t* srcPx;
	uint8_t* dstPx;

	for(int sy = 0;sy < source.h;++sy) {
		if(sy + y < 0) {continue;}
		else if(sy + y >= h) {break;}
		for(int sx = 0;sx < source.w;++sx) {
			if(sx + x < 0) {continue;}
			else if(sx + x >= w) {break;}
			srcPx = &source.data[(sx + sy * source.w) * source.channels];
			dstPx = &data[(sx + x + (sy + y) * w) * channels];

			float srcAlpha = source.channels < 4 ? 1 : srcPx[3] / 255.f;
			float dstAlpha = channels < 4 ? 1 : dstPx[3] / 255.f;

			if(srcAlpha > .99 && dstAlpha > .99) {
				if(source.channels >= channels) {
					memcpy(dstPx, srcPx, channels);
				}
				else {
					// In case our source image is grayscale and the dest one isnt
					memset(dstPx, srcPx[0], channels);
				}
			}
			else {
				float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
				if(outAlpha < .01) {
					memset(dstPx, 0, channels);
				}
				else {
					for(int chnl = 0;chnl < channels;++chnl) {
						dstPx[chnl] = (uint8_t)BYTE_BOUND((srcPx[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
					}
					if(channels > 3) {dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f);}
				}
			}
			
		}

	}
	return *this;
}


Image& Image::crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch) {
	size = cw * ch * channels;
	uint8_t* croppedImage = new uint8_t[size];
	memset(croppedImage, 0, size);

	for(uint16_t y = 0;y < ch;++y) {
		if(y + cy >= h) {break;}
		for(uint16_t x = 0;x < cw;++x) {
			if(x + cx >= w) {break;}
			memcpy(&croppedImage[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}

	w = cw;
	h = ch;	

	delete[] data;
	data = croppedImage;
	croppedImage = nullptr;

	return *this;
}


Image& Image::copy_area(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch, uint16_t to_x, uint16_t to_y){
	size = cw * ch * channels;
	uint8_t* copied_area = new uint8_t[size];
	memset(copied_area, 0, size);

	for(uint16_t y = 0;y < ch;++y) {  // From Image to Temp
		if(y + cy >= h) {break;}
		for(uint16_t x = 0;x < cw;++x) {
			if(x + cx >= w) {break;}
			memcpy(&copied_area[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}

	for(uint16_t y = 0;y < ch;++y) {  // From Temp to Image
		if(y + to_y >= h) {break;}
		for(uint16_t x = 0;x < cw;++x) {
			if(x + to_x >= w) {break;}
			memcpy(&data[(x + to_x + (y + to_y) * w) * channels], &copied_area[(x + y * cw) * channels], channels);
		}
	}
	return *this;
}
