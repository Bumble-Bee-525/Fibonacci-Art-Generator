#include "nacci_declarations.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

/*
Possible numbers: 0, 1, 1, 2, 3, 5, 8
Paper dimensions: 21.5cm by 21.5 cm
Pixel dimensions: 812.6 by 812.6
1cm = 37.7 pixels
Printer is not capable of printing 21.5 by 21.5 cm
model is HL L2320D
Took possible numbers,and arranged them so their sum is 43, however the paper is half of 43 cm. All elements in the list were halved for their final dimensions in cm to fit on the paper
For this project, maybe use 43 instead to keep simple.
TODO:
alpha matte DONE
shade DONE
print&free numbers DONE
scramble numbers DONE
*/

//function to generate a list of numbers
node * fibonacci_generator(void)
{
    // fibonacci numbers is a list of options
    int fibonacci_numbers[] = {8, 5, 3, 2, 1};
    //middle numbers is a selection of numbers at center
    int middlenumbers[] = {5, 3, 1};
    //declare variables
    int sum_goal;
    int sum_current;

    //generate a random number to be the middle number
    srand(time(NULL));
    int random_index = (rand() %(3));
    int seednumber = middlenumbers[random_index];
    printf("Middle number: %i\n", seednumber);

    //set up sum goals
    sum_goal = 43 - seednumber;
    sum_current = 0;

    //initialize a linked list
    // point_list is sentinel node
    node *point_list = malloc(sizeof(node));
    point_list -> previous = NULL;
    //this pair of nodes work in tandem
    node *next_point = NULL;
    node *current_point = point_list;

    //while the sum of the fibbonacci sequence isn't 43
    while (sum_current < sum_goal)
    {
        //for each possible fibbonacci number
        for (int current_fib_num = 0; current_fib_num < 5; current_fib_num++)
        {
            //if adding the number on both sides wouldn't go greater than 43, add it
            if ((2 * fibonacci_numbers[current_fib_num]) + sum_current <= sum_goal)
            {
                //adjust current sum of the array
                sum_current += 2 * fibonacci_numbers[current_fib_num];
                //add number to array
                current_point -> number = fibonacci_numbers[current_fib_num];
                //create a new point
                next_point = malloc(sizeof(node));
                //link current point to new point and vice versa
                current_point -> next = next_point;
                next_point -> previous = current_point;
                //with all links attached, move forward
                current_point = next_point;
                break;
            }
        }
    }
    //randomly swap a pair of numbers
    int random_index2;
    node * other_point;
    int node1value;

    //random pair to be swapped
    srand(time(NULL) * 3);
    random_index = (rand() %(3));
    srand(time(NULL) / 3);
    random_index2 = (rand() %(3));
    //start at beginning
    current_point = point_list;
    other_point = point_list;
    //if it is a pair and not the same
    if (random_index != random_index2)
    {
        //travel to node1
        for (int yeet = 0; yeet < random_index; yeet++)
        {
            current_point = current_point -> next;
        }
        //travel to node 2
        for (int bradley = 0; bradley < random_index2; bradley++)
        {
            other_point = other_point -> next;
        }
        //remember node 1 value
        node1value = current_point -> number;
        //node 1 value = node 2 value
        current_point -> number = other_point -> number;
        //node 2 value = node 1 copy value
        other_point -> number = node1value;
    }

    //after loop exits, send current_point to left half, and set next_point to middle number
    current_point = next_point -> previous;
    next_point -> number = seednumber;

    //now we must add the other side of the array to be symetrical
    while (current_point != NULL)
    {
        next_point -> next = malloc(sizeof(node));
        next_point -> next -> previous = next_point;
        next_point = next_point -> next;
        next_point -> number = current_point -> number;
        current_point = current_point -> previous;
    }

    next_point -> next = NULL;

    //adds 0 the end
    next_point = malloc(sizeof(node));

    next_point -> number = 0;
    next_point -> next = point_list;
    point_list -> previous = next_point;
    next_point -> previous = NULL;
    point_list = next_point;

    //return
    return point_list;
}

void linemaker(int height, int width, RGBTRIPLE image[height][width], bool alpha_matte[height][width], float factor)
{
    //set up list of nacci numbers
    int scalefactor = width / 43;
    node *list_anchor = fibonacci_generator();
    node *list_rider = list_anchor;

    //loop through list, and every line associated with it
    int startspot = 0;
    int x;
    int y;

    // for each fibonacciu number in the sequence
    while (list_rider != NULL)
    {
        startspot += ((list_rider -> number) * scalefactor);
        x = startspot;
        y = 0;

        // increment x and y until line is drawn
        while(x < width)
        {
            //1
            image[y][x].rgbtRed *= factor;
            image[y][x].rgbtGreen *= factor;
            image[y][x].rgbtBlue *= factor;
            alpha_matte[y][x] = true;

            // 2
            image[height - y - 1][x].rgbtRed *= factor;
            image[height - y - 1][x].rgbtGreen *= factor;
            image[height - y - 1][x].rgbtBlue *= factor;
            alpha_matte[height - y - 1][x] = true;

            //3
            image[y][width - x - 1].rgbtRed *= factor;
            image[y][width - x - 1].rgbtGreen *= factor;
            image[y][width - x - 1].rgbtBlue *= factor;
            alpha_matte[y][width - x - 1] = true;

            //4
            image[height - y - 1][width - x - 1].rgbtRed *= factor;
            image[height - y - 1][width - x - 1].rgbtGreen *= factor;
            image[height - y - 1][width - x - 1].rgbtBlue *= factor;
            alpha_matte[height - y - 1][width - x - 1] = true;

            x++;
            y++;
        }

        list_rider = list_rider -> next;
    }

    //free the list while printing it one last time
    printf("your lucky numbers are. . .\n");
    list_rider = list_anchor;
    while ((list_rider -> next) != NULL)
    {
        printf("%i, ", list_rider -> number);
        list_rider = list_rider -> next;
        free(list_rider -> previous);
    }
    printf("%i, \n", list_rider -> number);
    free(list_rider);
}

void shader(int height, int width, RGBTRIPLE image[height][width], bool alpha_matte[height][width], float factor)
{
    int x;
    unsigned int y;
    bool pixel_matte;

    //scan through x axis
    for (unsigned int startspot = 1; startspot < width; startspot++)
    {
        //move diagonally each iterance for each pixel
        x = startspot;
        y = 1;
        pixel_matte = false;

        while (x > 0)
        {
            // if boundary detected in lower L, swap
            //  down 1                      down 1 right 1                   right 1
            if ((alpha_matte[y - 1][x] && alpha_matte[y][x + 1]) || alpha_matte[y - 1][x + 1])
            {
                pixel_matte = !pixel_matte;
            }
            //change pixel color
            if (pixel_matte)
            {
                //left side
                image[y][x].rgbtRed *= factor;
                image[y][x].rgbtGreen *= factor;
                image[y][x].rgbtBlue *= factor;

                //opposite side change
                image[height - y - 1][width - x - 1].rgbtRed *= factor;
                image[height - y - 1][width - x - 1].rgbtGreen *= factor;
                image[height - y - 1][width - x - 1].rgbtBlue *= factor;
            }
            //update variables
            x += -1;
            y++;
        }
    }
    //somehwere there is some bug that i'm too lazy to fix so here's a bandaid solution instead
    pixel_matte = false;
    for (int bruh = 1; bruh < height; bruh++)
    {
        if (alpha_matte[bruh - 1][0])
        {
            pixel_matte = !pixel_matte;
        }

        if (pixel_matte)
        {
            //left side
            image[bruh][0].rgbtRed *= factor;
            image[bruh][0].rgbtGreen *= factor;
            image[bruh][0].rgbtBlue *= factor;

            //opposite side change
            image[bruh][width-1].rgbtRed *= factor;
            image[bruh][width-1].rgbtGreen *= factor;
            image[bruh][width-1].rgbtBlue *= factor;
        }
    }
}
