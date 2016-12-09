/*
 * DESCRIPTION:
 *  Simple program to shuffle a deck of cards in the following manner:
 *    1. Take the top card off the deck and set it on the table
 *    2. Take the next card off the top and put it on the bottom of the deck in your hand.
 *    3. Continue steps 1 and 2 until all cards are on the table. This is a round.
 *    4. Pick up the deck from the table and repeat steps 1-3 until the deck is in the original order.
 *
 * INPUT: number_of_cards_in_deck
 * OUTPUT: number_of_shuffle_rounds (until the deck is in the original order)
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

#include "CommsNode.h"

void print_help_and_exit(const char* argv0);

int main(int argc, char **argv) {

	std::string filename;

	// Parse arguments using GNU getopt
	int opt;
	int n = -1;
	while ((opt = getopt(argc, argv, "hn:f:")) != -1) {
		switch (opt) {
		case 'h':
			print_help_and_exit(argv[0]);
			break;
		case 'n':
			n = atoi(optarg);
			break;
		case 'f':
			filename = std::string(optarg);
			break;
		default:
			print_help_and_exit(argv[0]);
			return 0;
		}
	}

	// Check if -n was entered and parsed properly
	if(n == -1){
		print_help_and_exit(argv[0]);
	}
	else if(n < 0 || n == 0){
		printf("Deck size is 0, negative or could not be parsed, exiting program\n");
		return 0;
	}

	// Get the # of turns needed for shuffling
	printf("Number of turns needed for shuffling %d cards is %d\n", n, 2);
	return 0;
}

void print_help_and_exit(const char* argv0){
	printf("Usage: %s [-h] [-n] NUM_CARDS [-o] FILE\n", argv0);
	printf("       -n NUM_CARDS  (required) number of cards to shuffle\n");
	printf("       -f FILE       (optional) output csv file\n");
	printf("       -h            print this help menu and exit\n");
	exit(0);
}
