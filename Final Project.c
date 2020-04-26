#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

// ECE 175: 31 Game
// Authors: Wesley Chiu, Natalie Hall

// Card definition: Suit, card value, pointer to next node and previous node (double-linked list)
typedef struct card_s {
	char suit[9];
	int value;
	struct card_s* next, * previous;
} card;

// Function Definitions
void createCard(card* p, card** hl, card** hr, char suit[9], int value);
void shuffleDeck(card** hl, card** hr); // Shuffles deck

void resetHand(card** hl, card** hr); // Resets dealer or player hand
void dealCard(card* p, card** hl, card** hr);
void deleteCard(card* p, card** hl, card** hr);
card* findCard(card** hl, card** hr);

int checkDealerHand(card* hl, card* hr);
int checkPlayerHand(card* hl, card* hr);

void printCard(card** printedCard);

int main(void)
{
	int playerMoney = 1000;
	card* deckStart = NULL;
	card* deckEnd = NULL;
	int i, j;
	
	// Initialize deck
	for (i = 1; i < 14;i++)	// Initialize each number value
	{
		for (j = 0; j < 2;j++)	// Double the amount since its 104 cards
		{
			createCard(deckStart, &deckStart, &deckEnd, "heart", i);
			createCard(deckStart, &deckStart, &deckEnd, "diamond", i);
			createCard(deckStart, &deckStart, &deckEnd, "spade", i);
			createCard(deckStart, &deckStart, &deckEnd, "club", i);

		}
	}

	card* playerStart = NULL;
	card* playerEnd = NULL;
	card* dealerStart = NULL;
	card* dealerEnd = NULL;

}

void dealCard(card* p, card** hl, card** hr)
{
	card* temp;
	temp = (card*)malloc(sizeof(card));
	if (*hl == NULL)	// if adding to beginning of deck/hand
	{
		*hl = temp;
		*hr = temp;
		temp->next = NULL;
		temp->previous = NULL;
	}
	else if (p->next == NULL) // if adding at the end of deck/hand
	{ 
		p->next = temp;
		temp->previous = p;
		*hr = temp;
		temp->next = NULL;
	}
	else		 // if adding to the middle of the deck/hand
	{
		temp->next = p->next;
		p->next->previous = temp;
		p->next = temp;
		temp->previous = p;
	}
}

void deleteCard(card* p, card** hl, card** hr) {
	if (p == *hl) 		// if deleting the first element  
	{
		*hl = p->next; 	// update the left head pointer
	}
	else
	{
		p->previous->next = p->next;
	}

	if (p == *hr)		// if deleting the last element 
	{
		*hr = p->previous; 	// update right head pointer
	}
	else
	{
		p->next->previous = p->previous;
	}

	free(p); // free memory 
}

int checkDealerHand(card* hl, card* hr)
{
	int sum = 0;
	int altSum;
	while (hl != NULL)	// still need to add condition for when ace is dealt
	{
		if ((hl->value == 11) || (hl->value == 11) || (hl->value == 11))
		{
			sum += 10;
		}
		else
		{
			sum += hl->value;
		}
		hl = hl->next;
	}
	if (sum == 14)	// If dealer hits 14
	{
		return 5;
	}
	else if (sum == 31)
	{
		return 4;
	}
	else if ((sum >= 27) && (sum < 31))	// All but one card revealed
	{
		return 3;
	}
	else if (sum > 31)	// Dealer goes bust
	{
		return -1
	}
	else	// Dealer hasn't finished yet
	{
		return 0;
	}
}

int checkPlayerHand(card* hl, card* hr)
{
	int sum = 0;
	int altSum;
	while (hl != NULL)	// still need to add condition for when ace is dealt
	{
		if ((hl->value == 11) || (hl->value == 11) || (hl->value == 11))
		{
			sum += 10;
		}
		else
		{
			sum += hl->value;
		}
		hl = hl->next;
	}
	if (sum == 14)	// If player hits 14
	{
		return 5;
	}
	else if (sum == 31)
	{
		return 4;
	}
	else if (sum > 31)	// Player goes bust
	{
		return -1
	}
	else	// Player hasn't finished yet
	{
		return 0;
	}
}
