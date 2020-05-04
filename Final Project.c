#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include <time.h> 

// ECE 175: 31 Game
// Authors: Wesley Chiu, Natalie Hall

// Card definition: Suit, card value, pointer to next node and previous node (double-linked list)
typedef struct card_s {
	char suit[9];
	int value;
	struct card_s* next, * previous;
} card;

// Function Definitions
void createDeck();
void createCard(card* p, card** hl, card** hr, char suit[9], int value);

void resetHand(card** hl, card** hr); // Resets dealer or player hand
void dealCard(card* p, card** hl, card** hr);
void deleteCard(card* p, card** hl, card** hr);
card* findCard(card** hl, card** hr);	// Find random card, add to player/dealer hand and delete from deck

int checkDealerHand(card* hl, card* hr);
int checkPlayerHand(card* hl, card* hr);
int compareSums(card* dealer, card* player);
void printCard(card* printedCard);


card* deckStart = NULL;
card* deckEnd = NULL;

int main(void)
{
	printf("Playing 31: \n");
	int playerMoney = 1000;
	const int MINBET = 20;
	const int MAXBET = 200;
	int playerBet;
	int roundCounter = 1;
	bool roundEnd = false;

	// Initialize deck
	createDeck();

	// Set up player & dealer variables
	card* playerStart = NULL;
	card* playerEnd = NULL;
	card* dealerStart = NULL;
	card* dealerEnd = NULL;
	srand(time(0));

	while (playerMoney > MINBET)
	{
		printf("\nRound %d:", roundCounter);
		dealCard(dealerStart, &dealerStart, &dealerEnd);
		dealCard(playerStart, &playerStart, &playerEnd);
		printf("\nDealer: ");
		printf("*");
		printf("\nPlayer: ");
		printCard(playerStart);

		// Ask user for bet amount
		printf("\nPlace your bet (minimum 20, max 200): ");
		scanf("%d", &playerBet);
		while ((playerBet > 200) || (playerBet < 20))
		{
			printf("\nInvalid bet. Please try again: ");
			scanf("%d", &playerBet);
		}
		// Deal dealer hand
		while (checkDealerHand(dealerStart, dealerEnd) == 0)
		{
			dealCard(dealerStart, &dealerStart, &dealerEnd);
		}

		// Check outcome of dealer hand
		card* dealerIndex;
		dealerIndex = dealerStart;
		printf("\Dealer: ");
		switch (checkDealerHand(dealerStart, dealerEnd))
		{
		case 5:
			while (dealerIndex != NULL)		// Print all cards
			{
				printCard(dealerIndex);
				dealerIndex = dealerIndex->next;
			}
			printf("\nDealer hit 14. You have to hit 31 to win.");
			break;
		case 4:
			while (dealerIndex != NULL)		// Print all cards
			{
				printCard(dealerIndex);
				dealerIndex = dealerIndex->next;
			}
			playerMoney = playerMoney - playerBet;
			printf("\nDealer hit 31! Dealer wins.");
			roundEnd = true;
			break;
		case 3:
			while (dealerIndex->next != NULL)	// Don't print last card
			{
				printCard(dealerIndex);
				dealerIndex = dealerIndex->next;
			}
			printf("FD CARD");
			break;
		case -1:
			while (dealerIndex != NULL)
			{
				printCard(dealerIndex);
				dealerIndex = dealerIndex->next;
			}
			playerMoney = playerMoney + playerBet;
			printf("\nDealer went bust! You win");
			roundEnd = true;
			break;
		}

		if (roundEnd == false)	// Check if dealer already won/lost
		{
			// Player's turn 
			char playerChoice = 'h';
			card* playerIndex;
			while ((playerChoice == 'h') && (checkPlayerHand(playerStart, playerEnd) == 0))
			{
				printf("\nWould you like to hit or stand? (h or s): ");
				scanf("%*c%c", &playerChoice);
				while ((playerChoice != 'h') && (playerChoice != 's'))
				{
					printf("\nInvalid input. Please try again: ");
					scanf("%*c%c", &playerChoice);
				}
				playerIndex = playerStart;
				if (playerChoice == 'h')
				{
					dealCard(playerStart, &playerStart, &playerEnd);
					printf("\nPlayer: ");
					while (playerIndex != NULL)
					{
						printCard(playerIndex);
						playerIndex = playerIndex->next;
					}
				}
			}
			switch (checkPlayerHand(playerStart, playerEnd))
			{
			case 5:
				printf("\nPlayer hit 14!");
				playerMoney = playerMoney + playerBet;
				break;
			case 4:
				printf("\nPLayer hit 31!");
				playerMoney = playerMoney + playerBet;
				break;
			case -1:
				printf("\nPlayer goes bust!");
				playerMoney = playerMoney - playerBet;
				break;
			}
			if (playerChoice == 's')
			{
				int result = compareSums(dealerStart, playerStart);
			}
		}
		printf("\nPlayer money: %d", playerMoney);
		printf("\n");
		resetHand(&playerStart, &playerEnd);		// Resets dealer and player hand
		resetHand(&dealerStart, &dealerEnd);
		roundCounter++;
	}

	return 0;
}
void createDeck()
{
	int i,j;
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

}
void createCard(card* p, card** hl, card** hr, char suit[9], int value)
{
	card* temp;
	temp = (card*)malloc(sizeof(card));
	temp->value = value;
	strcpy(temp->suit, suit);
	if (*hl == NULL)
	{
		temp->previous = NULL;
		temp->next = NULL;
		*hl = temp;
		*hr = temp;
	}
	else if (p->next == NULL)
	{
		temp->previous = p;
		temp->next = NULL;
		p->next = temp;
		*hr = temp;
	}
	else
	{
		temp->next = p->next;
		temp->previous = p;
		p->next = temp;
		temp->next->previous = temp;
	}
}


void resetHand(card** hl, card** hr) // Resets dealer or player hand
{
	*hl = NULL;
	*hr = NULL;
}
void dealCard(card* p, card** hl, card** hr)
{
	card* temp;
	temp = (card*)malloc(sizeof(card));
	temp = findCard(&deckStart, &deckEnd);
	
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
card* findCard(card** hl, card** hr)
{
	int value = (rand() % 13) + 1;
	int nsuit = (rand() % 4) + 1;
	char suit[9];
	switch (nsuit)
	{
	case 1:
		strcpy(suit, "heart");
		break;
	case 2:
		strcpy(suit, "diamond");
		break;
	case 3:
		strcpy(suit, "spade");
		break;
	case 4:
		strcpy(suit, "club");
		break;
	}
	card* temp;
	temp = (card*)malloc(sizeof(card));
	temp->value = value;
	strcpy(temp->suit, suit);
	card* holder;
	card* deckIndex = *hl;
	while (deckIndex->next != NULL)
	{
		if ((deckIndex->value == value) && (strcmp(suit, (deckIndex->suit)) == 0))
		{
			//holder = temp;
			deleteCard(deckIndex, hl, hr);
			return temp;
		}
		deckIndex = deckIndex->next;
	}
	//temp = findCard(hl, hr);
}

int checkDealerHand(card* hl, card* hr)
{
	int sum = 0;
	int altSum = 0;	// altSum used for possibility of ace
	while (hl != NULL)	
	{
		if ((hl->value == 11) || (hl->value == 12) || (hl->value == 13))
		{
			sum += 10;
			altSum += 10;
		}
		else if (hl->value == 1)
		{
			sum += hl->value;
			altSum += 11;
		}
		else
		{
			sum += hl->value;
			altSum += hl->value;
		}
		hl = hl->next;
	}

	if ((sum == 14) || (altSum == 14))// If dealer hits 14
	{
		return 5;
	}
	else if ((sum == 31) || (altSum == 31))
	{
		return 4;
	}
	else if (((sum >= 27) && (sum < 31)) || ((altSum >= 27) && (altSum < 31)))	// All but one card revealed
	{
		return 3;
	}
	else if (sum > 31)	// Dealer goes bust
	{
		return -1;
	}
	else	// Dealer hasn't finished yet
	{
		return 0;
	}
}

int checkPlayerHand(card* hl, card* hr)
{
	int sum = 0;
	int altSum = 0;
	while (hl != NULL)	
	{
		if ((hl->value == 11) || (hl->value == 12) || (hl->value == 13))
		{
			sum += 10;
			altSum += 10;
		}
		else if (hl->value == 1)
		{
			sum += (hl->value);
			altSum += 11;
		}
		else
		{
			sum += hl->value;
			altSum += hl->value;
		}
		hl = hl->next;
	}
	if ((sum == 14) || (altSum == 14))	// If player hits 14
	{
		return 5;
	}
	else if ((sum == 31) || (altSum == 31))
	{
		return 4;
	}
	else if (sum > 31)	// Player goes bust
	{
		return -1;
	}
	else	// Player hasn't finished yet
	{
		return 0;
	}
}
int numCardsLeft(card* hl, card* hr)
{
	int counter = 0;
	while (hl != NULL)
	{
		hl = hl->next;
		counter++;
	}
	return counter;
}
int compareSums(card* dealer, card* player)
{
	int dealerSum = 0;
	int altDealerSum = 0;
	while (dealer != NULL)
	{
		if ((dealer->value == 11) || (dealer->value == 12) || (dealer->value == 13))
		{
			dealerSum += 10;
			altDealerSum += 10;
		}
		else if (dealer->value == 1)
		{
			dealerSum += (dealer->value);
			altDealerSum += 11;
		}
		else
		{
			dealerSum += dealer->value;
			altDealerSum += dealer->value;
		}
		dealer = dealer->next;
	}
	int playerSum = 0;
	int altPlayerSum = 0;
	while (player != NULL)
	{
		if ((player->value == 11) || (player->value == 12) || (player->value == 13))
		{
			playerSum += 10;
			altPlayerSum += 10;
		}
		else if (player->value == 1)
		{
			playerSum += (player->value);
			altPlayerSum += 11;
		}
		else
		{
			playerSum += player->value;
			altPlayerSum += player->value;
		}
		player = player->next;
	}
}

void printCard(card* printedCard)	// Function for printing an individual card
{
	// Print value
	if (printedCard->value == 1)
	{
		printf("A ");
	}
	else if (printedCard->value == 11)
	{
		printf("J ");
	}
	else if (printedCard->value == 12)
	{
		printf("Q ");
	}
	else if (printedCard->value == 13)
	{
		printf("K ");
	}
	else
	{
		printf("%d ", printedCard->value);

	}
	//Print suit
	if (strcmp(printedCard->suit, "heart") == 0)
	{
		printf("H\t");
	}
	else if (strcmp(printedCard->suit, "diamond") == 0)
	{
		printf("D\t");
	}
	else if (strcmp(printedCard->suit, "spade") == 0)
	{
		printf("S\t");
	}
	else if (strcmp(printedCard->suit, "club") == 0)
	{
		printf("C\t");
	}
}