#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <Vortex.h>

//This class contain value and suit of card.
class Card{
public:
    enum Suit {
        HEART,
        CLUBS,
        PEAKS,
        DIAMONDS,
        SUITS
    };

    enum Value {
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE,
        VALUES
    };
private:
    Suit m_suit;
    Value m_value;
public:
    Card(Value value = Value::VALUES, Suit suit = Suit::SUITS) : m_value(value), m_suit(suit) {}
    Card() {}

    void PrintCard() const{
        switch (m_value) {
        case TWO:
            std::cout << "2";
            break;
        case THREE:
            std::cout << "3";
            break;
        case FOUR:
            std::cout << "4";
            break;
        case FIVE:
            std::cout << "5";
            break;
        case SIX:
            std::cout << "6";
            break;
        case SEVEN:
            std::cout << "7";
            break;
        case EIGHT:
            std::cout << "8";
            break;
        case NINE:
            std::cout << "9";
            break;
        case TEN:
            std::cout << "10";
            break;
        case JACK:
            std::cout << "J";
            break;
        case QUEEN:
            std::cout << "Q";
            break;
        case KING:
            std::cout << "K";
            break;
        case ACE:
            std::cout << "A";
            break;
        }
        switch (m_suit) {
        case HEART:
            std::cout << "H";
            break;
        case CLUBS:
            std::cout << "C";
            break;
        case PEAKS:
            std::cout << "P";
            break;
        case DIAMONDS:
            std::cout << "D";
            break;
        }
    }

    int getCardValue()const {
        switch (m_value)
        {
        case TWO:		return 2;
        case THREE:		return 3;
        case FOUR:		return 4;
        case FIVE:		return 5;
        case SIX:		return 6;
        case SEVEN:		return 7;
        case EIGHT:		return 8;
        case NINE:		return 9;
        case TEN:		return 10;
        case JACK:	    return 10;
        case QUEEN:	    return 10;
        case KING:	    return 10;
        case ACE:		return 11;
        }

        return 0;
    }
};

//Here keeps full deck and methods to operate with it
class Deck {
    std::vector <Card> m_deck;
    int m_cardIndex = 0;

    void swapCard(Card& card1, Card& card2) {
        Card tempcard = card1;
        card1 = card2;
        card2 = tempcard;
    }
    PRNG generator;
    

public:
    Deck() {
        for (int j = 0; j < Card::Value::VALUES; j++) {
            for (int k = 0; k < Card::Suit::SUITS; k++) {
                m_deck.push_back(Card(static_cast<Card::Value>(j), static_cast<Card::Suit>(k)));
            }
        }
        generator.initGenerator(generator);
    }

    void printDeck() {
        for (const auto& a : m_deck) {
            a.PrintCard();
            std::cout << ' ';
        }
        std::cout << '\n';
    }

    void shuffleDeck() {
        
        for (auto& a : m_deck) {
            swapCard(a, m_deck[generator.getRandomIndex(generator, 51)]);
        }
        m_cardIndex = 0;
    }

    const Card& dealCard() {
        assert(m_cardIndex < 52);
        return m_deck[m_cardIndex++];
    }

    std::vector <Card> GetDeck() {
        return m_deck;
    }

};

int CheckACE(int& a, int& b) {
    if (a == 11 && b < 11) {
        a = 11;
        std::cout << "ACE is now equal:" << a << std::endl;
        return a;
    }
    else if (a == 11 && b > 11) {
        a = 1;
        std::cout << "ACE is now equal:" << a << std::endl;
        return a;
    }
    else return a;
}
//This method starts by method PlayOrNot
int playBlackjack(std::vector <Card> & deck) {
    Card* cardPtr;
    cardPtr = &deck[0];
    int PlayerResult[10];
    int CompResult[10];
    //Opponent shows the first card
    CompResult[0] = cardPtr->getCardValue();
    *cardPtr++;
    std::string choice;
    CompResult[9] = CompResult[0];
    //The game checks if there is ACE and if it is compare with total to choose its correct value
    CompResult[0] = CheckACE(CompResult[0], CompResult[9]);
    std::cout << "Dealer's first card: " << CompResult[0] << " in total " << CompResult[9] << std::endl;
    //It's players turn now
    PlayerResult[0] = cardPtr->getCardValue();
    *cardPtr++;
    //Checking for ACE
    PlayerResult[0] = CheckACE(PlayerResult[0], PlayerResult[9]);
    PlayerResult[1] = cardPtr->getCardValue();
    *cardPtr++;
    PlayerResult[9] = PlayerResult[0];
    //Checking for ACE
    PlayerResult[1] = CheckACE(PlayerResult[1], PlayerResult[9]);
    PlayerResult[9] += PlayerResult[1];
    //Below shows cards got by player.
    std::cout << "Your hand " << PlayerResult[0] << " " << PlayerResult[1] << " in total: " << PlayerResult[9] << std::endl;
    //Here starts loop with checking if player want to hit or stand card
    do {
        std::cout << "Do you hit(h) or stand(s)?";
        std::cin >> choice;
        std::transform(choice.begin(), choice.end(), choice.begin(),
            [](unsigned char c) { return std::tolower(c); });
        int k = 2;
        if (choice == "s") {
            std::cout << "You decided to stand, your total score is " << PlayerResult[9] << std::endl;
        }
        else if (choice == "h") {
            PlayerResult[k] = cardPtr->getCardValue();
            *cardPtr++;
            CheckACE(PlayerResult[k], PlayerResult[9]);
            PlayerResult[9] += PlayerResult[k];
            std::cout << "Your card is " << PlayerResult[k] << " in total: " << PlayerResult[9] << std::endl;
            if (PlayerResult[9] > 21) return 0;
            k++;
        }
    } while (choice != "s");
    //After player's choice starts dealer's turn
    std::cout << "It's dealer's turn now" << std::endl;
    int j = 1;
    do {
        CompResult[j] = cardPtr->getCardValue();
        *cardPtr++;
        CheckACE(CompResult[j], CompResult[9]);
        CompResult[9] += CompResult[j];
        std::cout << "Dealer's card " << CompResult[j] << " in total " << CompResult[9] << std::endl;
    } while (CompResult[9] < 17);
    cardPtr = nullptr;
    //After whole cycle of game starts checking for getting winner
    if (CompResult[9] > 21) return 1;
    if (CompResult[9] > PlayerResult[9]) return 0;
    else return 1;
    //Here the game checks for draw
    if (CompResult[9] == PlayerResult[9]) return 2;
}

void PlayOrNot(Deck& cdeck) {
    std::vector <Card> deck;
    char choice;
    do {
        deck = cdeck.GetDeck();
        choice = ' ';
        //Here starts the game
        int resultofgame = playBlackjack(deck);
        if (resultofgame == 0) std::cout << "You lose" << std::endl;
        if (resultofgame == 1) std::cout << "You won" << std::endl;
        if (resultofgame == 2) std::cout << "It's a draw" << std::endl;
        //At the end the game asks player if he wants to continue or end game
        do {
            std::cout << "Do you want to play again? y/n ";
            std::cin >> choice;
            choice = (char)tolower(choice);
            system("cls");
        } while (choice != 'n' && choice != 'y');
        if (choice == 'y') {
            cdeck.shuffleDeck();
        }
    } while (choice != 'n');
}

int main() {
    //Creating deck
    Deck deck;
    //Shuffling deck
    deck.shuffleDeck();
    //Starting game throug looped block
    PlayOrNot(deck);

    
    return 0;
}

