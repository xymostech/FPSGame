#include "font.h"

/*
 * font_load
 *
 * Loads a font from a file name. Everything else is done
 * automatically!
 */
struct font* font_load(char *filename) {
	/* initialize the font structure */
	struct font *font = malloc(sizeof(*font));

	/* FreeType objects */
	FT_Library library;
	FT_Error error;
	FT_Face face;
	
	/* start up FreeType */
	error = FT_Init_FreeType(&library);
	if (error) {
		fprintf(stderr, "Error initializing freetype\n");
		free(font);
		return NULL;
	}

	/* load the file into FreeType */
	error = FT_New_Face(library, filename, 0, &face);
	if (error) {
		fprintf(stderr, "Error reading font: %s\n", filename);
		free(font);
		return NULL;
	}

	/* set our pixel size (I use 64 because it
	 * makes higher-quality glyphs */
	error = FT_Set_Pixel_Sizes(face, 0, 64);
	if (error) {
		fprintf(stderr, "Error setting sizes for font: %s\n", filename);
	}

	/* The characters that we want to load. This will hopefully be
	 * expanded at some later point. */
	char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int max_width = 0, max_height = 0;

	/* go through the characters */
	char *chr = chars;
	while (*chr) {
		/* get the unicode character index */
		FT_UInt index = FT_Get_Char_Index(face, *chr);

		/* render the glyph */
		error = FT_Load_Glyph(face, index, FT_LOAD_RENDER);
		if (error) {
			fprintf(stderr, "Error: Font %s can't load glyph: '%c'\n", filename, *chr);
			continue;
		}

		/* find the max_width and max_height based on the metrics */
		if (face->glyph->metrics.width > max_width) {
			max_width = face->glyph->metrics.width;
		}
		if (face->glyph->metrics.height > max_height) {
			max_height = face->glyph->metrics.height;
		}

		chr++;
	}

	/* calculate the overall max width and height */
	max_width /= 64;
	max_height /= 64;

	/*
	 * find a power of two buffer size to make OpenGL happy
	 *
	 * Note that we are storing all of the characters vertically
	 * which is not even close to the best arrangement but it
	 * makes it exceptionally easy to address each character when
	 * drawing. Maybe I'll come up with a better solution later
	 */
	int buffer_height = 1;
	while (buffer_height < max_height * strlen(chars)) {
		buffer_height <<= 1;
	}
	int buffer_width = 1;
	while (buffer_width < max_width) {
		buffer_width <<= 1;
	}

	/* some pre-calculation */
	int char_size = max_height * max_width;
	int row_size = max_width;

	/* create the buffer */
	unsigned char *buffer = malloc(buffer_height * buffer_width * 2);

	/* allocate char data for each of our characters */
	font->char_data = malloc(sizeof(*font->char_data)*strlen(chars));

	/* go through the characters again */
	chr = chars;
	int chr_num = 0;
	while (*chr) {
		/* refind the index and rerender */
		FT_UInt index = FT_Get_Char_Index(face, *chr);

		error = FT_Load_Glyph(face, index, FT_LOAD_RENDER);
		if (error) {
			fprintf(stderr, "Error: Font %s can't load glyph: '%c'\n", filename, *chr);
			continue;
		}

		/* go through each pixel of the character and copy the data
		 * into the buffer */
		int i, j;
		/* go through each row of the glyph */
		for (i=0; i<face->glyph->bitmap.rows; ++i) {
			/* find the position on the buffer where this
			 * character should go */
			unsigned char *set_buffer = buffer +
			    2 * chr_num * buffer_width * max_height +
			    i * buffer_width * 2;
			/* go through each column of the glyph */
			for (j=0; j<face->glyph->bitmap.width; ++j) {
				/* find the value of the glyph (0 - 255) at
				 * the current position */
				int val = face->glyph->bitmap.buffer
				              [i*face->glyph->bitmap.width+j];
				/* these are explained below at glTexImage2D */
				*(set_buffer++) = 255;
				*(set_buffer++) = val;
			}
		}

		/* store data on each character */
		font->char_data[chr_num].width = face->glyph->metrics.width/64;
		font->char_data[chr_num].height = face->glyph->metrics.height/64;
		font->char_data[chr_num].bearing_x = face->glyph->metrics.horiBearingX/64;
		font->char_data[chr_num].bearing_y = face->glyph->metrics.horiBearingY/64;
		font->char_data[chr_num].advance = face->glyph->metrics.horiAdvance/64;

		chr++;
		chr_num++;
	}

	/* place to store our texture */
	GLuint texture;

	/* generate a texture */
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	/* we want linear filtering for everything, so our fonts
	 * end up normally scaled. This might be a bad choice, and
	 * mipmaps might end up helping fonts look better, but this
	 * looks fine for now */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	 * generate the texture
	 *
	 * Notes: We are using GL_LUMINANCE8_ALPHA8, which means that in our
	 * buffer, the first byte represents the luminance (which in turn
	 * represents red, green, and blue), and the second byte represents
	 * alpha. We set luminance to 255 all the time, so everything should
	 * appear white, and we set the alpha to the value of the actual
	 * character. This creates very good looking fonts, and also has the
	 * benefit that each character can be colored easily (because it
	 * starts out white and can be blended, whereas black characters cannot
	 * be blended
	 */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8_ALPHA8,
	             buffer_width, buffer_height, 0,
	             GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buffer);

	/* the buffer is copied into OpenGL, so we don't need it any more */
	free(buffer);

	/* store our data into the font structure */
	font->tex = texture;

	font->char_width = max_width;
	font->char_height = max_height;

	font->buffer_width = buffer_width;
	font->buffer_height = buffer_height;

	return font;
}

/* simply free the texture and free up resources */
void font_delete(struct font *font) {
	glDeleteTextures(1, &font->tex);
	free(font->char_data);
	free(font);
}

/*
 * font_draw
 *
 * takes a string of characters, and draws them in sequence,
 * with correct spacing and sizing
 */
void font_draw(struct font *font, char *string, int size) {
	/* use our texture */
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

	/* cycle through each character */
	while (*string) {
		/*
		 * retrieve the index of the character.
		 * TODO: Make this work for things other than capital letters
		 */
		int chr = *string - 'A';

		/* retrieve our char data */
		struct font_char *char_data = &font->char_data[chr];

		glPushMatrix();

		/* translate according to bearing, and translate to the
		 * character's buffer position */
		glTranslatef(char_data->bearing_x, char_data->bearing_y, 0);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glTranslatef(0, chr * font->char_height, 0);
		glMatrixMode(GL_MODELVIEW);

		/* use QUADS because screw compatibility */
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

		/* translate according to the advance */
		glTranslatef(char_data->advance, 0, 0);

		++string;
	}

	/* and cleanup */
	glPopMatrix();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_TEXTURE_2D);
}
