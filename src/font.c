#include "font.h"

struct font* font_load(char *filename) {
	struct font *font = malloc(sizeof(*font));

	FT_Library library;
	FT_Error error;
	FT_Face face;
	
	error = FT_Init_FreeType(&library);
	if (error) {
		fprintf(stderr, "Error initializing freetype\n");
		free(font);
		return NULL;
	}

	error = FT_New_Face(library, filename, 0, &face);
	if (error) {
		fprintf(stderr, "Error reading font: %s\n", filename);
		free(font);
		return NULL;
	}

	error = FT_Set_Pixel_Sizes(face, 0, 64);
	if (error) {
		fprintf(stderr, "Error setting sizes for font: %s\n", filename);
	}

	char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int max_width = 0, max_height = 0;

	char *chr = chars;
	while (*chr) {
		FT_UInt index = FT_Get_Char_Index(face, *chr);

		error = FT_Load_Glyph(face, index, FT_LOAD_RENDER);
		if (error) {
			fprintf(stderr, "Error: Font %s can't load glyph: '%c'\n", filename, *chr);
			continue;
		}

		if (face->glyph->metrics.width > max_width) {
			max_width = face->glyph->metrics.width;
		}
		if (face->glyph->metrics.height > max_height) {
			max_height = face->glyph->metrics.height;
		}

		chr++;
	}

	max_width /= 64;
	max_height /= 64;

	int buffer_height = 1;
	while (buffer_height < max_height * strlen(chars)) {
		buffer_height <<= 1;
	}
	int buffer_width = 1;
	while (buffer_width < max_width) {
		buffer_width <<= 1;
	}

	int char_size = max_height * max_width;
	int row_size = max_width;

	unsigned char *buffer = malloc(buffer_height * buffer_width * 2);

	font->char_data = malloc(sizeof(*font->char_data)*strlen(chars));

	chr = chars;
	int chr_num = 0;
	while (*chr) {
		FT_UInt index = FT_Get_Char_Index(face, *chr);

		error = FT_Load_Glyph(face, index, FT_LOAD_RENDER);
		if (error) {
			fprintf(stderr, "Error: Font %s can't load glyph: '%c'\n", filename, *chr);
			continue;
		}

		int i, j;
		for (i=0; i<face->glyph->bitmap.rows; ++i) {
			unsigned char *set_buffer = buffer +
			    2 * chr_num * buffer_width * max_height +
			    i * buffer_width * 2;
			for (j=0; j<face->glyph->bitmap.width; ++j) {
				int val = face->glyph->bitmap.buffer
				              [i*face->glyph->bitmap.width+j];
				*(set_buffer++) = 255;
				*(set_buffer++) = val;
			}
		}

		font->char_data[chr_num].width = face->glyph->metrics.width/64;
		font->char_data[chr_num].height = face->glyph->metrics.height/64;
		font->char_data[chr_num].bearing_x = face->glyph->metrics.horiBearingX/64;
		font->char_data[chr_num].bearing_y = face->glyph->metrics.horiBearingY/64;
		font->char_data[chr_num].advance = face->glyph->metrics.horiAdvance/64;

		chr++;
		chr_num++;
	}

	GLuint texture;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8_ALPHA8, buffer_width, buffer_height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	font->tex = texture;

	font->char_width = max_width;
	font->char_height = max_height;

	font->buffer_width = buffer_width;
	font->buffer_height = buffer_height;

	return font;
}

void font_delete(struct font *font) {
	glDeleteTextures(1, &font->tex);
	free(font->char_data);
	free(font);
}

void font_draw(struct font *font, char *string, int size) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font->tex);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	/* scale texture so pixels can be addressed correctly */
	glScalef(1.0/font->buffer_width, 1.0/font->buffer_height, 1);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	/* scale so that the font is drawn approximately size pixels tall */
	glScalef(size/(float)font->char_height, size/(float)font->char_height, 1);

	while (*string) {
		int chr = *string - 'A';

		struct font_char *char_data = &font->char_data[chr];

		glPushMatrix();

		glTranslatef(char_data->bearing_x, char_data->bearing_y, 0);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glTranslatef(0, chr * font->char_height, 0);
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);
			glTexCoord2i(0, char_data->height);
			glVertex2f(0, -char_data->height);

			glTexCoord2i(char_data->width, char_data->height);
			glVertex2f( char_data->width, -char_data->height);

			glTexCoord2i(char_data->width, 0);
			glVertex2f( char_data->width,  0);

			glTexCoord2i(0, 0);
			glVertex2f(0,  0);
		glEnd();

		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glTranslatef(char_data->advance, 0, 0);

		++string;
	}

	glPopMatrix();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_TEXTURE_2D);
}
