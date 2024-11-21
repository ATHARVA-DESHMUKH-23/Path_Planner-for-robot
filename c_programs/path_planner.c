
/*
* EcoMender Bot (EB): Task 2B Path Planner
*
* This program computes the valid path from the start point to the end point.
* Make sure you don't change anything outside the "Add your code here" section.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#define V 32

#ifdef __linux__ // for host pc

    #include <stdio.h>

    void _put_byte(char c) { putchar(c); }

    void _put_str(char *str) {
        while (*str) {
            _put_byte(*str++);
        }
    }

    void print_output(uint8_t num) {
        if (num == 0) {
            putchar('0'); // if the number is 0, directly print '0'
            _put_byte('\n');
            return;
        }

        if (num < 0) {
            putchar('-'); // print the negative sign for negative numbers
            num = -num;   // make the number positive for easier processing
        }

        // convert the integer to a string
        char buffer[20]; // assuming a 32-bit integer, the maximum number of digits is 10 (plus sign and null terminator)
        uint8_t index = 0;

        while (num > 0) {
            buffer[index++] = '0' + num % 10; // convert the last digit to its character representation
            num /= 10;                        // move to the next digit
        }
        // print the characters in reverse order (from right to left)
        while (index > 0) { putchar(buffer[--index]); }
        _put_byte('\n');
    }

    void _put_value(uint8_t val) { print_output(val); }

#else  // for the test device

    void _put_value(uint8_t val) { }
    void _put_str(char *str) { }

#endif


// main function
int main(int argc, char const *argv[]) {

    #ifdef __linux__

        const uint8_t START_POINT   = atoi(argv[1]);
        const uint8_t END_POINT     = atoi(argv[2]);
        uint8_t NODE_POINT          = 0;
        uint8_t CPU_DONE            = 0;

    #else
        // Address value of variables for RISC-V Implementation
        #define START_POINT         (* (volatile uint8_t * ) 0x02000000)
        #define END_POINT           (* (volatile uint8_t * ) 0x02000004)
        #define NODE_POINT          (* (volatile uint8_t * ) 0x02000008)
        #define CPU_DONE            (* (volatile uint8_t * ) 0x0200000c)

    #endif

    // array to store the planned path
    uint8_t path_planned[32];
    // index to keep track of the path_planned array
    uint8_t idx = 0;

    /* Functions Usage

    instead of using printf() function for debugging,
    use the below function calls to print a number, string or a newline

    for newline: _put_byte('\n');
    for string:  _put_str("your string here");
    for number:  _put_value(your_number_here);

    Examples:
            _put_value(START_POINT);
            _put_value(END_POINT);
            _put_str("Hello World!");
            _put_byte('\n');
    */

    // ############# Add your code here #############
    // prefer declaring variable like this
   #ifdef __linux__
        uint32_t graph[V] = { 
    0b00000000000000000000010001000010, // Node 0: connected to Nodes 1,6 and 10
    0b00000000000000000000100000000101, // Node 1: connected to Nodes 11,0,2
    0b00000000000000000000000000111000, // Node 2: connected to Nodes 3,4,5
    0b00000000000000000000000000000100, // Node 3: connected to Node 2
    0b00000000000000000000000000000100, // Node 4: connected to Nodes 2
    0b00000000000000000000000000000100, // Node 5: connected to Nodes 2
    0b00000000000000000000001110000001, // Node 6: connected to Nodes 7,8,9
    0b00000000000000000000000001000000, // Node 7: connected to Node 6
    0b00000000000000000000000001000000, // Node 8: connected to Node 6
    0b00000000000000000000000001000000, // Node 9: connected to Node 6
    0b00001101000000000000100000000001, // Node 10: connected to Nodes 0,11,25,26
    0b00000000000010000001010000000010, // Node 11: connected to Nodes 12,1.10.19
    0b00000000000000000110100000000000, // Node 12: connected to Node 11,13,14
    0b00000000000000000001000000000000, // Node 13: connect to Node 12
    0b00000000000000011001000000000000, // Node 14: connected to Node 16,15,12
    0b00000000000000000100000000000000, // Node 15: connected to Node 14
    0b00000000000001100100000000000000, // Node 16: connected to Node 14,17,18
    0b00000000000000010010000000000000, // Node 17: connected to Node 16
    0b00000000001010010000000000000000, // Node 18: connected to Node 21,19,16
    0b00000000000101000000100000000000, // Node 19: connected to Node 20,18,11
    0b00000000000010000000000000000000, // Node 20: connected to Nodes 19
    0b00000000110001000000000000000000, // Node 21: connected to Nodes 18,22 and 23
    0b00000000001000000000000000000000, // Node 22: connected to Node 21
    0b01000001001000000000000000000000, // Node 23: connected to Nodes 21,24 and 30
    0b00000010100000000000010000000000, // Node 24: connected to Node 25,10,23
    0b00000001000000000000000000000000, // Node 25: connected to Nodes 24
    0b00011000000000000000010000000000, // Node 26: connected to Node 10,28, 27
    0b00000100000000000000000000000000, // Node 27: connected to Nodes 26
    0b01100100000000000000000000000000, // Node 28: connected to Node 29,30,26
    0b00010000000000000000000000000000, // Node 29: connected to Nodes 28
    0b10010000100000000000000000000000, // Node 30: connected to Nodes 23,28 and 31
    0b01000000000000000000000000000000  // Node 31: connected to Node 30
};

    #else
        uint32_t *graph = (uint32_t *) 0x02000010;
         *(graph + 0x00) = 0b00000000000000000000010001000010; // Node 0
         *(graph + 0x01) = 0b00000000000000000000100000000101; // Node 1
         *(graph + 0x02) = 0b00000000000000000000000000111000; // Node 2
         *(graph + 0x03) = 0b00000000000000000000000000000100; // Node 3
         *(graph + 0x04) = 0b00000000000000000000000000000100; // Node 0
         *(graph + 0x05) = 0b00000000000000000000000000000100; // Node 1
         *(graph + 0x06) = 0b00000000000000000000001110000001; // Node 2
         *(graph + 0x07) = 0b00000000000000000000000001000000; // Node 3
         *(graph + 0x08) = 0b00000000000000000000000001000000; // Node 0
         *(graph + 0x09) = 0b00000000000000000000000001000000; // Node 1
         *(graph + 0x0a) = 0b00001101000000000000100000000001; // Node 2
         *(graph + 0x0b) = 0b00000000000010000001010000000010; // Node 3
         *(graph + 0x0c) = 0b00000000000000000110100000000000; // Node 0
         *(graph + 0x0d) = 0b00000000000000000001000000000000; // Node 1
         *(graph + 0x0e) = 0b00000000000000011001000000000000; // Node 2
         *(graph + 0x0f) = 0b00000000000000000100000000000000; // Node 3
         *(graph + 0x10) = 0b00000000000001100100000000000000; // Node 0
         *(graph + 0x11) = 0b00000000000000010010000000000000; // Node 1
         *(graph + 0x12) = 0b00000000001010010000000000000000; // Node 2
         *(graph + 0x13) = 0b00000000000101000000100000000000; // Node 3
         *(graph + 0x14) = 0b00000000000010000000000000000000; // Node 0
         *(graph + 0x15) = 0b00000000110001000000000000000000; // Node 1
         *(graph + 0x16) = 0b00000000001000000000000000000000; // Node 2
         *(graph + 0x17) = 0b01000001001000000000000000000000; // Node 3
         *(graph + 0x18) = 0b00000010100000000000010000000000; // Node 0
         *(graph + 0x19) = 0b00000001000000000000000000000000; // Node 1
         *(graph + 0x1a) = 0b00011000000000000000010000000000; // Node 2
         *(graph + 0x1b) = 0b00000100000000000000000000000000; // Node 3
         *(graph + 0x1c) = 0b01100100000000000000000000000000; // Node 0
         *(graph + 0x1d) = 0b00010000000000000000000000000000; // Node 1
         *(graph + 0x1e) = 0b10010000100000000000000000000000; // Node 2
         *(graph + 0x1f) = 0b01000000000000000000000000000000; // Node 3


    #endif

 uint8_t stack[20]; // Stack pointer to hold at most 20 nodes
    uint32_t visited = 0; // Visited array as a single 32-bit integer
    int top = -1; // Stack top pointer

    stack[++top] = START_POINT; // Push start node
    visited |= (1 << START_POINT); // Mark start node as visited

    while (top >= 0) {
        uint8_t node = stack[top];
        if (idx == 0 || path_planned[idx - 1] != node) {
            path_planned[idx++] = node; // Add to path
        }
        if (node == END_POINT) break; // Exit on reaching end node

        bool has_unvisited = false;
        for (uint8_t i = 0; i < V; i++) {
            if ((graph[node] & (1 << i)) && !(visited & (1 << i))) {
                stack[++top] = i;
                visited |= (1 << i);
                has_unvisited = true;
                break;
            }
        }
        if (!has_unvisited) {
            top--; // Backtrack
            idx--; // Remove from path
        }
    }

    // ##############################################

    // the node values are written into data memory sequentially.
    for (int i = 0; i < idx; ++i) {
        NODE_POINT = path_planned[i];
    }
    // Path Planning Computation Done Flag
    CPU_DONE = 1;

    #ifdef __linux__    // for host pc

        _put_str("######### Planned Path #########\n");
        for (int i = 0; i < idx; ++i) {
            _put_value(path_planned[i]);
        }
        _put_str("################################\n");

    #endif

    return 0;
}

