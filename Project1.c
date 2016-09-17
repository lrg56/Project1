#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pixel{
  unsigned char r,g,b;
}pixel;

typedef struct Image{
  char *Magic;
  int w, h;
  int MaxColor;
  Pixel *data;
}Image;

int check_bits(int r, int g, int b, int max, int min){
  if(((r > max) || (r < min)) || ((g > max) || (g < min)) || ((b > max) || (b < min))){
    return(1);
  }else{
    return(0);
  }

}

void read(char *fh, Image *image){
  char buffer[64];
  FILE *fp;
  int row, col;
  int r, g, b;
  int c;
  

  fpointer = fopen(filename, "r");

  if(fpointer == NULL){
    fprintf(stderr, "The file was unable to be opened.\n");
    fclose(fpointer);
    exit(1);
  }

  buffer[0] = fgetc(fpointer);
  buffer[1] = fgetc(fpointer);

  if(buffer[0] != 'P'){
    fprintf(stderr, "This file does not have a PPM format\n");
    exit(1);
  }

  if(buffer[1] == '3'){
    image->Magic = "P3";
  }else if(buffer[1] == '6'){
    image->Magic = "P6";
  }else{
    fprintf(stderr, "This file needs to be in P3 or P6 format\n");
    exit(1);
  }

  c = getc(fp);
  while(c == '#'){
    while(getc(fp) != '\n'){
      c = getc(fp);
    }
  }

  ungetc(c, fp);

  if(fscanf(fp, "%d %d", &image->w, &image->h) != 2){
    fprintf(stderr, "Invalid width or height when reading\n");
    exit(1);
  }

  if(fscanf(fp, "%d", &image->MaxColor) != 1){
    fprintf(stderr, "Invalid max color value when reading\n");
    exit(1);
  }

  if((image->MaxColor < 0) || (image->MaxColor > 255)){
    fprintf(stderr, "Invalid max color value range(>0 or <255)\n");
    exit(1);
  }

  image->data = malloc(sizeof(Pixel) * image->w * image->h);

  if(image->Magic[1] == '3'){
    for(row = 0; row < image->h; row++){
      for(col = 0; col < image->w; col++){
	fscanf(fp, "%d", &r);
        fscanf(fp, "%d", &g);
	fscanf(fp, "%d", &b);

	if(check_bits(r, g, b, 255, 0) == 1){
	  fprintf(stderr, "the color channel is not 8-bits.\n");
	  exit(-1);
	}

	image->data[(image->width) * row + col].r = r;
	image->data[(image->width) * row + col].g = g;
	image->data[(image->width) * row + col].b = b;
	
      }
    }
  }else if(image->Magic[1] == '6'){
    fgetc(fp);
    fread(image->data, sizeof(Pixel), image->w, image->h, fp);
  }else{
    fprintf(stderr, "There was an error reading the PPM file number(it should be P3 or P6)\n");
    exit(1);
  }
}

void write_P3(char *fh, Image *image){
  char buffer[64];
  int row, col;
  FILE *fp;

  fp = fopen(fh, "w");

  if(fp == NULL){
    fprintf(stderr, "The file did not open properly.\n");
    exit(1);
  }else{
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", image->w, image->h);
    fprintf(fp, "%d\n", image->MaxColor);

    for(row = 0; row < (image->h); row++){
      for(col = 0; col < (image->w); col++){
	sprintf(buffer, "%d", image->data[(image->w) * row + col].r);
	fprintf(fp, "%s\n", buffer);

	sprintf(buffer, "%d", image->data[(image->w) * row + col].g);
	fprintf(fp, "%s\n", buffer);

	sprintf(buffer, "%d", image->data[(image->w) * row + col].b);
	fprintf(fp, "%s\n", buffer);
      }
    }
    fclose(fpointer);
  }
}
void write_P6(char *fh, Image *image){
  FILE *fp;
  fp = fopen(fh, "wb");

  if(fp == NULL){
    fprintf(stderr, "The file did not open properly.\n");
    exit(1);
  }
  fprintf(fp, "P6\n");
  fprintf(fp, "%d %d\n", image->w, image->h);
  fprintf(fp, "%d\n", image->MaxColor);

  fwrite(image->data, sizeof(Pixel), image->w * image->h, fp);
  fclose(fp);
}


int main(int argc, char *argv[]){
  Image *PPM_image;
  PPM_image = (Image *)malloc(sizeof(Image));

  if(argc != 4){
    fprintf(stderr, "Incorrect number of arguments please check they are correct");
    exit(1);

  if(atoi(argv[1]) == 3){
    read(argv[2], PPM_image);
    write_P3(argv[3], PPM_image);
  }else if(atoi(argv[1]) == 3){
    read(argv[2], PPM_image);
    write_P6(argv[3], PPM_image);
  }else{
    fprintf(stderr, "Not a correct PPM conversion type.(P3 or P6)\n");
    exit(1);
  }
  return(0);
}
  
	
