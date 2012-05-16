#include "texture.h"

unsigned char* load_png_data(char *filename, int *w, int *h) {
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		return NULL;
	}
	png_byte header[8];
	fread(header, 1, 8, fp);
	if(png_sig_cmp(header, 0, 8)) {
		return NULL;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr) {
		return NULL;
	}
	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return NULL;
	}
	
	png_infop end_ptr = png_create_info_struct(png_ptr);
	if(!end_ptr) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return NULL;
	}
	
	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
		fclose(fp);
		return NULL;
	}
	
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	
	png_read_info(png_ptr, info_ptr);
	
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	
	switch(png_get_color_type(png_ptr, info_ptr)) {
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png_ptr);
			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			if(bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
			if(bit_depth==16) png_set_strip_16(png_ptr);
			png_set_gray_to_rgb(png_ptr);
			break;
		case PNG_COLOR_TYPE_RGB:
			if(bit_depth <  8 ) png_set_packing(png_ptr);
			if(bit_depth == 16) png_set_strip_16(png_ptr);
			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
			break;
		default:
			break;
	}
	
	png_read_update_info(png_ptr, info_ptr);
	
	*w = png_get_image_width(png_ptr, info_ptr);
	*h = png_get_image_height(png_ptr, info_ptr);
	int rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);
	
	png_bytep data = malloc((*h)*rowbytes*sizeof(*data));
	
	png_bytep row_pointers[*h];
	
	int i;
	for(i=0; i<(*h); ++i) {
		row_pointers[i] = &data[i*rowbytes];
	}
	
	png_read_image(png_ptr, row_pointers);
	
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);

	fclose(fp);

	return data;
}

struct texture* texture_load(char *filename) {
	
}

void texture_delete(struct texture *texture) {
	
}

void texture_use(struct texture *texture) {
	
}
