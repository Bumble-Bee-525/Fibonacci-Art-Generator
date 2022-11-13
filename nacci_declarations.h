#include "BMP_datatypes_and_crap.h"
#include "stdbool.h"

//making a node struct for linked lists
typedef struct node
{
    int number;
    struct node *next;
    struct node *previous;
}
node;

//function that makes a linked list of fibonnaci numbers
node * fibonacci_generator(void);

//function that draws lines on images
void linemaker(int height, int width, RGBTRIPLE image[height][width], bool alpha_matte[height][width], float factor);

//function that shades an image
void shader(int height, int width, RGBTRIPLE image[height][width], bool alpha_matte[height][width], float factor);
