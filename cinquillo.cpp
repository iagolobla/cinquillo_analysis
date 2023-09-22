#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <stdlib.h>
#include <string>

//#define DEBUG
#ifdef DEBUG 
#define D(x) x
#else 
#define D(x)
#endif

using namespace std;

/*  Card Class
    - Has a card Number
    - Has a card Suit
*/
enum Card_Suit{
    OROS = 0,
    BASTOS = 1,
    ESPADAS = 2,
    COPAS = 3
};
constexpr const char* CardSuit_ToStr(Card_Suit c) throw()
{
    switch (c)
    {
        case Card_Suit::OROS: return "O";
        case Card_Suit::BASTOS: return "B";
        case Card_Suit::ESPADAS: return "E";
        case Card_Suit::COPAS: return "C";
        default: throw std::invalid_argument("Unimplemented item");
    }
}

class Card{
    public:
        int number; //From 1 to 10
        Card_Suit suit;

        //Constructor
        Card(int number, Card_Suit suit){
            this->number=number;
            this->suit=suit;
        }

        bool operator==(Card const & other) const {
            return this->_equals(other);
        }

        bool operator!=(Card const & other) const {
            return !(*this == other);
        }

        //Destructor
        ~Card(){
            this->number=-1;
        }

    protected:
        virtual bool _equals(Card const & other) const
        {
            if (typeid(*this) != typeid(other)) return false;
            return (this->number == other.number && this->suit == other.suit);
        }
        
};

ostream& operator << (ostream &os, const Card &c) {
    return (os << c.number << CardSuit_ToStr(c.suit));
}

/* Deck Class
    - Has a set of cards
*/
class Deck{
    public:
        static const int SIZE = 40; //Deck size
        vector<Card> cards;
        int five_oros_index=-1;

        //Constructor
        Deck(){
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            cards = vector<Card>();

            //Oros suit
            this->cards.push_back(Card(1,Card_Suit::OROS));
            this->cards.push_back(Card(2,Card_Suit::OROS));
            this->cards.push_back(Card(3,Card_Suit::OROS));
            this->cards.push_back(Card(4,Card_Suit::OROS));
            this->cards.push_back(Card(5,Card_Suit::OROS));
            this->cards.push_back(Card(6,Card_Suit::OROS));
            this->cards.push_back(Card(7,Card_Suit::OROS));
            this->cards.push_back(Card(8,Card_Suit::OROS));
            this->cards.push_back(Card(9,Card_Suit::OROS));
            this->cards.push_back(Card(10,Card_Suit::OROS));

            //Bastos suit
            this->cards.push_back(Card(1,Card_Suit::BASTOS));
            this->cards.push_back(Card(2,Card_Suit::BASTOS));
            this->cards.push_back(Card(3,Card_Suit::BASTOS));
            this->cards.push_back(Card(4,Card_Suit::BASTOS));
            this->cards.push_back(Card(5,Card_Suit::BASTOS));
            this->cards.push_back(Card(6,Card_Suit::BASTOS));
            this->cards.push_back(Card(7,Card_Suit::BASTOS));
            this->cards.push_back(Card(8,Card_Suit::BASTOS));
            this->cards.push_back(Card(9,Card_Suit::BASTOS));
            this->cards.push_back(Card(10,Card_Suit::BASTOS));

            //Espadas suit
            this->cards.push_back(Card(1,Card_Suit::ESPADAS));
            this->cards.push_back(Card(2,Card_Suit::ESPADAS));
            this->cards.push_back(Card(3,Card_Suit::ESPADAS));
            this->cards.push_back(Card(4,Card_Suit::ESPADAS));
            this->cards.push_back(Card(5,Card_Suit::ESPADAS));
            this->cards.push_back(Card(6,Card_Suit::ESPADAS));
            this->cards.push_back(Card(7,Card_Suit::ESPADAS));
            this->cards.push_back(Card(8,Card_Suit::ESPADAS));
            this->cards.push_back(Card(9,Card_Suit::ESPADAS));
            this->cards.push_back(Card(10,Card_Suit::ESPADAS));

            //Copas
            this->cards.push_back(Card(1,Card_Suit::COPAS));
            this->cards.push_back(Card(2,Card_Suit::COPAS));
            this->cards.push_back(Card(3,Card_Suit::COPAS));
            this->cards.push_back(Card(4,Card_Suit::COPAS));
            this->cards.push_back(Card(5,Card_Suit::COPAS));
            this->cards.push_back(Card(6,Card_Suit::COPAS));
            this->cards.push_back(Card(7,Card_Suit::COPAS));
            this->cards.push_back(Card(8,Card_Suit::COPAS));
            this->cards.push_back(Card(9,Card_Suit::COPAS));
            this->cards.push_back(Card(10,Card_Suit::COPAS));

            shuffle(begin(cards), end(cards), default_random_engine(seed));

            //Search for the 5 of Oros to store it's index
            this->five_oros_index=this->getIndex(Card(5,Card_Suit::OROS));
        }

        Card* get(int card_index){
            return &(this->cards[card_index]);
        }

        //Returns the index of a card in the deck. Returns -1 if the card is not in the deck
        int getIndex(Card card){
            for(int i=0;i<this->cards.size();i++){
                if(card == this->cards[i]){
                    return i;
                }
            }
            cerr << "The searched card doesn't exist in the deck: " << card;
            return -1;
        }

        //Destructor
        ~Deck(){
            this->cards.clear();
        }
};

/* Table Class
    - Has 4 piles of cards
    - Each pile of cards is defined by it's lowest and highest card
    - Each pile of cards can only be of one suit of cards
*/
class Table{
    class Pile{
        Deck* deck;
        public:
            Card_Suit suit;
            int highest_card=-1;    //Reference index (in the deck) of the highest card in the pile
            int lowest_card=-1;     //Reference index (in the deck) of the lowest card in the pile
            
            //Constructor
            Pile(Deck* deck, Card_Suit suit){
                this->suit=suit;
                this->deck=deck;
                this->highest_card=-1;
                this->lowest_card=-1;
            }

            //Destructor
            ~Pile(){
                this->deck=nullptr;
            }

            //Adds card to the pile if possible, if not, returns false
            bool addCard(int card_index){
                Card* card = deck->get(card_index);   //Retrieve card

                //If the cards suit is not the same, it can't be added to the pile
                if(card->suit != this->suit){
                    cerr << "Tried to add a card to a pile with a different suit" << endl;
                    return false;
                }

                //If the pile is empty and the card is a 5
                if(this->highest_card == -1 && this->lowest_card == -1 && card->number == 5){
                    this->highest_card = card_index;
                    this->lowest_card = card_index;
                    return true;
                }

                //If the pile has cards and the card is the one above the highest
                if(card->number == this->deck->get(highest_card)->number+1){
                    this->highest_card=card_index;
                    return true;
                }

                //If the pile has cards and the card is the one below the lowest
                if(card->number == this->deck->get(lowest_card)->number-1){
                    this->lowest_card=card_index;
                    return true;
                }

                //If any of this can be done, the card cannot be added to the pile
                return false;
            }

            //Get addable cards to the pile
            vector<int> getAddableCards(){
                vector<int> result;
                //If the pile is empty, the only addable card is the 5 of that suit
                if(this->highest_card == -1 && this->lowest_card == -1){
                    //Search the card in the Deck and add the index to the result
                    result.push_back(deck->getIndex(Card(5,this->suit)));
                }else{    
                    //The pile is not empty, get the card above the highest and the one below the lowest
                    Card* high_card=deck->get(highest_card);
                    Card* low_card=deck->get(lowest_card);
                    
                    //Check the card above the highest
                    if(high_card->number < 10){
                        result.push_back(deck->getIndex(Card(high_card->number+1, this->suit)));
                    }
                    
                    //Check the card below the lowest
                    if(low_card->number > 1){
                        result.push_back(deck->getIndex(Card(low_card->number-1, this->suit)));
                    }
                }

                return result;
            }
    };

    private:
        Pile* piles[4];

    public:
        Deck* deck;

        //Constructor
        Table(Deck* deck){
            this->deck=deck;    //Copy the reference to the deck

            //Create the Piles
            piles[Card_Suit::OROS]=new Pile(this->deck,Card_Suit::OROS);
            piles[Card_Suit::BASTOS]=new Pile(this->deck,Card_Suit::BASTOS);
            piles[Card_Suit::ESPADAS]=new Pile(this->deck,Card_Suit::ESPADAS);
            piles[Card_Suit::COPAS]=new Pile(this->deck,Card_Suit::COPAS);
        }

        ~Table(){
            delete(piles[Card_Suit::OROS]);
            delete(piles[Card_Suit::BASTOS]);
            delete(piles[Card_Suit::ESPADAS]);
            delete(piles[Card_Suit::COPAS]);
        }

        //Function to get the playable cards in the current state
        vector<int> getPlayableCards(){
            vector<int> result;

            //Iterate through the piles
            for(int i=0;i<4;i++){
                //Add the addable cards of each pile to the result vector
                for(int j : piles[i]->getAddableCards()){
                    result.push_back(j);
                }
            }

            return result;
        }

        //Function to add cards to the table
        bool addCardToTable(int card_index){
            //Try to add card to the right card
            return piles[this->deck->get(card_index)->suit]->addCard(card_index);
        }

        //Function to print the state of the table
        void printTable(){
            //Top Line
            cout << "-----------------------------------------" << endl;

            //Line 10
            cout << "|\t" << (piles[Card_Suit::OROS]->highest_card != -1 && deck->get(piles[Card_Suit::OROS]->highest_card)->number==10?"10O":" - ") << "\t" << (piles[Card_Suit::BASTOS]->highest_card != -1 && deck->get(piles[Card_Suit::BASTOS]->highest_card)->number==10?"10B":" - ") << "\t" << (piles[Card_Suit::ESPADAS]->highest_card != -1 && deck->get(piles[Card_Suit::ESPADAS]->highest_card)->number==10?"10E":" - ") << "\t" << (piles[Card_Suit::COPAS]->highest_card != -1 && deck->get(piles[Card_Suit::COPAS]->highest_card)->number==10?"10C":" - ") << "\t|" << endl;

            //Line 9
            cout << "|\t" << (piles[Card_Suit::OROS]->highest_card != -1 && deck->get(piles[Card_Suit::OROS]->highest_card)->number>=9?" 9O":" - ") << "\t" << (piles[Card_Suit::BASTOS]->highest_card != -1 && deck->get(piles[Card_Suit::BASTOS]->highest_card)->number>=9?" 9B":" - ") << "\t" << (piles[Card_Suit::ESPADAS]->highest_card != -1 && deck->get(piles[Card_Suit::ESPADAS]->highest_card)->number>=9?" 9E":" - ") << "\t" << (piles[Card_Suit::COPAS]->highest_card != -1 && deck->get(piles[Card_Suit::COPAS]->highest_card)->number>=9?" 9C":" - ") << "\t|" << endl;

            //Line 8
            cout << "|\t" << (piles[Card_Suit::OROS]->highest_card != -1 && deck->get(piles[Card_Suit::OROS]->highest_card)->number>=8?" 8O":" - ") << "\t" << (piles[Card_Suit::BASTOS]->highest_card != -1 && deck->get(piles[Card_Suit::BASTOS]->highest_card)->number>=8?" 8B":" - ") << "\t" << (piles[Card_Suit::ESPADAS]->highest_card != -1 && deck->get(piles[Card_Suit::ESPADAS]->highest_card)->number>=8?" 8E":" - ") << "\t" << (piles[Card_Suit::COPAS]->highest_card != -1 && deck->get(piles[Card_Suit::COPAS]->highest_card)->number>=8?" 8C":" - ") << "\t|" << endl;

            //Line 7
            cout << "|\t" << (piles[Card_Suit::OROS]->highest_card != -1 && deck->get(piles[Card_Suit::OROS]->highest_card)->number>=7?" 7O":" - ") << "\t" << (piles[Card_Suit::BASTOS]->highest_card != -1 && deck->get(piles[Card_Suit::BASTOS]->highest_card)->number>=7?" 7B":" - ") << "\t" << (piles[Card_Suit::ESPADAS]->highest_card != -1 && deck->get(piles[Card_Suit::ESPADAS]->highest_card)->number>=7?" 7E":" - ") << "\t" << (piles[Card_Suit::COPAS]->highest_card != -1 && deck->get(piles[Card_Suit::COPAS]->highest_card)->number>=7?" 7C":" - ") << "\t|" << endl;

            //Line 6
            cout << "|\t" << (piles[Card_Suit::OROS]->highest_card != -1 && deck->get(piles[Card_Suit::OROS]->highest_card)->number>=6?" 6O":" - ") << "\t" << (piles[Card_Suit::BASTOS]->highest_card != -1 && deck->get(piles[Card_Suit::BASTOS]->highest_card)->number>=6?" 6B":" - ") << "\t" << (piles[Card_Suit::ESPADAS]->highest_card != -1 && deck->get(piles[Card_Suit::ESPADAS]->highest_card)->number>=6?" 6E":" - ") << "\t" << (piles[Card_Suit::COPAS]->highest_card != -1 && deck->get(piles[Card_Suit::COPAS]->highest_card)->number>=6?" 6C":" - ") << "\t|" << endl;

            //Line 5
            cout << "|\t" << (piles[Card_Suit::OROS]->highest_card != -1 && deck->get(piles[Card_Suit::OROS]->highest_card)->number>=5?" 5O":" - ") << "\t" << (piles[Card_Suit::BASTOS]->highest_card != -1 && deck->get(piles[Card_Suit::BASTOS]->highest_card)->number>=5?" 5B":" - ") << "\t" << (piles[Card_Suit::ESPADAS]->highest_card != -1 && deck->get(piles[Card_Suit::ESPADAS]->highest_card)->number>=5?" 5E":" - ") << "\t" << (piles[Card_Suit::COPAS]->highest_card != -1 && deck->get(piles[Card_Suit::COPAS]->highest_card)->number>=5?" 5C":" - ") << "\t|" << endl;

            //Line 4
            cout << "|\t" << (deck->get(piles[Card_Suit::OROS]->lowest_card)->number<=4 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 4O":" - ") << "\t" << (deck->get(piles[Card_Suit::BASTOS]->lowest_card)->number<=4 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 4B":" - ") << "\t" << (deck->get(piles[Card_Suit::ESPADAS]->lowest_card)->number<=4 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 4E":" - ") << "\t" << (deck->get(piles[Card_Suit::COPAS]->lowest_card)->number<=4 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 4C":" - ") << "\t|" << endl;

            //Line 3
            cout << "|\t" << (deck->get(piles[Card_Suit::OROS]->lowest_card)->number<=3 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 3O":" - ") << "\t" << (deck->get(piles[Card_Suit::BASTOS]->lowest_card)->number<=3 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 3B":" - ") << "\t" << (deck->get(piles[Card_Suit::ESPADAS]->lowest_card)->number<=3 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 3E":" - ") << "\t" << (deck->get(piles[Card_Suit::COPAS]->lowest_card)->number<=3 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 3C":" - ") << "\t|" << endl;

            //Line 2
            cout << "|\t" << (deck->get(piles[Card_Suit::OROS]->lowest_card)->number<=2 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 2O":" - ") << "\t" << (deck->get(piles[Card_Suit::BASTOS]->lowest_card)->number<=2 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 2B":" - ") << "\t" << (deck->get(piles[Card_Suit::ESPADAS]->lowest_card)->number<=2 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 2E":" - ") << "\t" << (deck->get(piles[Card_Suit::COPAS]->lowest_card)->number<=2 && deck->get(piles[Card_Suit::OROS]->lowest_card)->number > 0?" 2C":" - ") << "\t|" << endl;

            //Line 1
            cout << "|\t" << (deck->get(piles[Card_Suit::OROS]->lowest_card)->number==1?" 1O":" - ") << "\t" << (deck->get(piles[Card_Suit::BASTOS]->lowest_card)->number==1?" 1B":" - ") << "\t" << (deck->get(piles[Card_Suit::ESPADAS]->lowest_card)->number==1?" 1E":" - ") << "\t" << (deck->get(piles[Card_Suit::COPAS]->lowest_card)->number==1?" 1C":" - ") << "\t|" << endl;

            //Bottom Line
            cout << "-----------------------------------------" << endl;
        }
};

/* Player Class
    - Has a hand of cards
    - Can play cards of its hand onto the table
        - Option 1: Apply game strategy and play the best possible card
        - Option 2: Play a random card from the possible
        - Option 3: Apply reverse game strategy and play the worst possible card
    - If it can't play any card, it must pass
*/
enum Player_Strategy{
    SMART_STRATEGY=0,
    RANDOM_STRATEGY=1,
    DUMB_STRATEGY=2
};
constexpr const char* PlayerStrategy_ToStr(Player_Strategy ps) throw()
{
    switch (ps)
    {
        case Player_Strategy::SMART_STRATEGY: return "SMART_STRATEGY";
        case Player_Strategy::RANDOM_STRATEGY: return "RANDOM_STRATEGY";
        case Player_Strategy::DUMB_STRATEGY: return "DUMB_STRATEGY";
        default: throw std::invalid_argument("Unimplemented item");
    }
}

ostream& operator << (ostream &os, const Player_Strategy ps) {
    return (os << PlayerStrategy_ToStr(ps));
}
class Player{
    public:

        Deck* deck;
        Table* table;
        Player_Strategy strategy=Player_Strategy::RANDOM_STRATEGY;
        vector<int> hand;   //Card Positions in the deck
        int n_initial_cards=0;

        //Player statistics
        int turns_passed=0;

        //Constructor
        Player(Deck* deck, Table* table, Player_Strategy strategy, vector<int> hand){
            this->deck=deck;
            this->table=table;
            this->strategy=strategy;
            this->hand = vector<int>(hand);
            this->n_initial_cards = this->hand.size();
        }

        //Destructor
        ~Player(){
            this->hand.clear();
            deck=nullptr;
            table=nullptr;
        }

        /*  Function to play a turn
            - Returns true if the player won the game, false otherwise.
        */
        bool play(){
            if(this->strategy==Player_Strategy::RANDOM_STRATEGY){
                return this->play_random();
            }else if(this->strategy==Player_Strategy::SMART_STRATEGY){
                return this->play_smart();
            }else if(this->strategy==Player_Strategy::DUMB_STRATEGY){
                return this->play_dumb();
            }
            return false;
        }

        //Function to play this player's hand
        void printHand(){
            cout << "[";
            int h;
            for(h=0;h<this->hand.size()-1;h++){
                cout << *(this->deck->get(this->hand[h])) << ", ";
            }
            cout << *(this->deck->get(this->hand[h])) << "]" << endl;
        }
    
    private:
        //Function to play a turn with smart strategy
        bool play_smart(){
            //Check if the player has the 5 of Oros to play
            for(int i=0;i<this->hand.size();i++){
                if(this->hand[i] == deck->five_oros_index){
                    D(cout << "Play " << *(this->deck->get(this->hand[i])) << endl);
                    this->play_card(i); //Play the card
                    return (this->hand.size() == 0);  //Return if player won
                }
            }

            //Cross-Check the cards in the hand with the ones that can be played 
            vector<int> available_options;
            for(int t : table->getPlayableCards()){
                for(int h=0;h<this->hand.size();h++){
                    if(t == this->hand[h]){ //If there's a match and therefore, a card in the hand can be played
                        available_options.push_back(h); //Add it to the available_options vector
                    }
                }
            }

            //If there's no available options, the player must pass
            if(available_options.size() == 0){
                this->turns_passed++;
                D(cout << "Pass" << endl);
                return false;
            }

            //If there's only one choice
            if(available_options.size() == 1){
                D(cout << "Play " << *(this->deck->get(this->hand[available_options[0]])) << endl);
                this->play_card(available_options[0]);
                return (this->hand.size() == 0);    //Return if player won
            }

            //If there's more than one choice, the options must be evaluated
            int max_score=0;
            int max_score_card_hand_index=-1;
            for(int i : available_options){ //Evaluate each option and pick the one with the highest score
                int card_score=this->evaluateCard(this->hand[i]);
                if(card_score > max_score){
                    max_score=card_score;
                    max_score_card_hand_index=i;
                }
            }

            //Play the card with the best score
            D(cout << "Play " << *(this->deck->get(this->hand[max_score_card_hand_index])) << endl);
            this->play_card(max_score_card_hand_index);
            return (this->hand.size() == 0);    //Return if player won
        }

        //Function to play a turn with dumb strategy
        bool play_dumb(){
            //Check if the player has the 5 of Oros to play
            for(int i=0;i<this->hand.size();i++){
                if(this->hand[i] == deck->five_oros_index){
                    D(cout << "Play " << *(this->deck->get(this->hand[i])) << endl);
                    this->play_card(i); //Play the card
                    return (this->hand.size() == 0);  //Return if player won
                }
            }

            //Cross-Check the cards in the hand with the ones that can be played 
            vector<int> available_options;
            for(int t : table->getPlayableCards()){
                for(int h=0;h<this->hand.size();h++){
                    if(t == this->hand[h]){ //If there's a match and therefore, a card in the hand can be played
                        available_options.push_back(h); //Add it to the available_options vector
                    }
                }
            }

            //If there's no available options, the player must pass
            if(available_options.size() == 0){
                this->turns_passed++;
                D(cout << "Pass" << endl);
                return false;
            }

            //If there's only one choice
            if(available_options.size() == 1){
                D(cout << "Play " << *(this->deck->get(this->hand[available_options[0]])) << endl);
                this->play_card(available_options[0]);
                return (this->hand.size() == 0);    //Return if player won
            }

            //If there's more than one choice, the options must be evaluated
            int min_score=2200;
            int min_score_card_hand_index=-1;
            for(int i : available_options){ //Evaluate each option and pick the one with the lowest score
                int card_score=this->evaluateCard(this->hand[i]);
                if(card_score < min_score){
                    min_score=card_score;
                    min_score_card_hand_index=i;
                }
            }

            //Play the card with the worst score
            D(cout << "Play " << *(this->deck->get(this->hand[min_score_card_hand_index])) << endl);
            this->play_card(min_score_card_hand_index);
            return (this->hand.size() == 0);    //Return if player won
        }

        //Function to play a turn with random strategy
        bool play_random(){
            //Check if the player has the 5 of Oros to play
            for(int i=0;i<this->hand.size();i++){
                if(this->hand[i] == deck->five_oros_index){
                    D(cout << "Play " << *(this->deck->get(this->hand[i])) << endl);
                    this->play_card(i); //Play the card
                    return (this->hand.size() == 0);  //Return if player won
                }
            }

            //Cross-Check the cards in the hand with the ones that can be played 
            vector<int> available_options;
            for(int t : table->getPlayableCards()){
                for(int h=0;h<this->hand.size();h++){
                    if(t == this->hand[h]){ //If there's a match and therefore, a card in the hand can be played
                        available_options.push_back(h); //Add it to the available_options vector
                    }
                }
            }

            //If there's no available options, the player must pass
            if(available_options.size() == 0){
                this->turns_passed++;
                D(cout << "Pass" << endl);
                return false;
            }

            //If there's only one choice
            if(available_options.size() == 1){
                D(cout << "Play " << *(this->deck->get(this->hand[available_options[0]])) << endl);
                this->play_card(available_options[0]);
                return (this->hand.size() == 0);    //Return if player won
            }

            //Choose a random card to play from the available options
            srand(std::chrono::system_clock::now().time_since_epoch().count());
            int card_hand_index=available_options[rand() % available_options.size()];

            //Play the random card
            D(cout << "Play " << *(this->deck->get(this->hand[card_hand_index])) << endl);
            this->play_card(card_hand_index);
            return (this->hand.size() == 0);    //Return if player won
        }

        //Function to play a card from the hand: Adds the card to the table and removes it from the hand
        void play_card(int card_hand_index){
            int card_index=this->hand[card_hand_index];
            if(!table->addCardToTable(card_index)){
                cerr << "Error the card can't be played: " << deck->get(card_index) << endl;
                return;
            }
            this->hand.erase(this->hand.begin()+card_hand_index);
        }

        /*Function to assign a score to a card that indicates how good is it to play in the current table
            - The best choices are always 1s and 10s as they don't open any new spots
            - The next best choices are the ones that only open spots to the player
                - That means that the player has the cards that would go in the opened spots in its hand
            - It's better to open 1 spot to other players than 2 spots
                - If the card is a 5, two spots must be checked, the 4 and the 6
            - Opening the same amount of spots to other players, it's better to play the card that open spots in a direction where the player has more cards
                - If the card is a 6O and opens the spot 7O, it's better to play it as the player has a bigger amount of OROS above 5.
                - If the card is a 4O and opens the spot 3O, it's better to play it as the player has a bigger amount of OROS below 5.
            - Opening the same amount of spots to other players and having the same amount of cards of that suit in that direction, it's better to play the cards further from 5
        */
        int evaluateCard(int card_index){
            Card* card = this->deck->get(card_index);
            Card* card_aux=nullptr;
            int opened_spots=-1;
            int cards_from_suit=0;
            int base_score = 2200;  //More than the highest score that can be achieved

            //If the card is a 5, 2 spots must be checked
            if(card->number == 5){
                opened_spots=2; //Max opened spots
                //Get indexes of the adjacent cards
                int above_card_index = this->deck->getIndex(Card(card->number+1,card->suit));
                int below_card_index = this->deck->getIndex(Card(card->number-1,card->suit));

                for(int i : this->hand){
                    //Check if the player has those cards
                    if(i == above_card_index || i == below_card_index){
                        opened_spots--;
                    }

                    //Check how many cards of the same suit does the player have
                    card_aux=deck->get(i);
                    if(card_aux->suit == card->suit && card_aux->number != card->number){
                        cards_from_suit++;
                    }
                }
            }else{
                opened_spots=1; //Max opened spots
                //The card would only open one spot
                if(card->number > 5){
                    //If the card is bigger than 5 and not 10, check the card above
                    if(card->number == 10){
                        opened_spots=0;
                    }else{
                        int above_card_index = this->deck->getIndex(Card(card->number+1,card->suit));
                    
                        for(int i : this->hand){
                            //Check if the player has that card
                            if(i == above_card_index){
                                opened_spots--;
                            }
                            //Check how many cards of the same suit above the card number does the player have
                            card_aux=deck->get(i);
                            if(card_aux->suit == card->suit && card_aux->number > card->number){
                                cards_from_suit++;
                            }
                        }
                    }                       
                }else{
                    //If the card is lower than 5 and not 1, check the card below
                    if(card->number == 1){
                        opened_spots=0;
                    }else{
                        int below_card_index = this->deck->getIndex(Card(card->number-1,card->suit));

                        for(int i : this->hand){
                            //Check if the player has that card
                            if(i == below_card_index){
                                opened_spots--;
                            }

                            //Check how many cards of the same suit below the card number does the player have
                            card_aux=deck->get(i);
                            if(card_aux->suit == card->suit && card_aux->number < card->number){
                                cards_from_suit++;
                            }
                        }
                    }
                }
            }

            //Calculate card score
            int score = base_score - opened_spots*1000 - (10-cards_from_suit)*10 - (5-abs(5-card->number));

            //Clean the pointers
            card_aux=nullptr;
            card=nullptr;

            return score;
        }
};

/* Game Class
    - Has a number of Players (From 3 to 6)
    - Has a Table
    - Has a Deck
    - Has a Type that can be:
        - Smart Game: All Players using smart strategy
        - Random Game: All Players using random cards
        - Dumb Game: All Players using dumb strategy
        - Smart Random Game: Player 1 using smart strategy and the rest using random game strategy
        - Dumb Random Game: Player 1 using dumb strategy and the rest using random game strategy
        - Smart Dumb Game: Player 1 using smart strategy and the rest using dumb game strategy
        - Dumb Smart Game: Player 1 using dumb strategy and the rest using smart game strategy
    - Has a calculated Result: Winning player; Number of Turns
    - Can deal cards to the Players
*/
enum Game_Type{
    SMART_GAME = 0,
    RANDOM_GAME = 1,
    DUMB_GAME = 2,
    SMART_RANDOM_GAME = 3,
    DUMB_RANDOM_GAME = 4,
    SMART_DUMB_GAME = 5,
    DUMB_SMART_GAME = 6
};
constexpr const char* GameType_ToStr(Game_Type gs) throw()
{
    switch (gs)
    {
        case Game_Type::SMART_GAME: return "SMART_GAME";
        case Game_Type::RANDOM_GAME: return "RANDOM_GAME";
        case Game_Type::DUMB_GAME: return "DUMB_GAME";
        case Game_Type::SMART_RANDOM_GAME: return "SMART_RANDOM_GAME";
        case Game_Type::DUMB_RANDOM_GAME: return "DUMB_RANDOM_GAME";
        case Game_Type::SMART_DUMB_GAME: return "SMART_DUMB_GAME";
        case Game_Type::DUMB_SMART_GAME: return "DUMB_SMART_GAME";
        default: throw std::invalid_argument("Unimplemented item");
    }
}
ostream& operator << (ostream &os, const Game_Type gs) {
    return (os << GameType_ToStr(gs));
}

class Game{
    private:
        Deck* own_deck=nullptr;
    public:
        unsigned seed;
        int NumPlayers;
        Game_Type GameType;
        Deck* deck;
        Table* table;
        vector<Player> players;

        //Game development statistics
        int turn=-1;
        int first_player=-1;
        int current_player=-1;
        int winner_player=-1;

        //Constructor standard
        Game(int NumPlayers, Game_Type GameType){
            this->NumPlayers=NumPlayers;
            this->GameType=GameType;
            
            //Create Deck
            this->own_deck=new Deck();
            this->deck=this->own_deck;

            //Create Table
            table = new Table(this->deck);

            //Prepare players hands
            int cards_per_player = Deck::SIZE / NumPlayers;
            int leftover_cards = Deck::SIZE % NumPlayers;
            
            vector<int> player_hand[NumPlayers];
            
            //Fill each hand
            for(int i=0;i<NumPlayers;i++){
                //Generate the right card indexes
                for(int j=leftover_cards+i*cards_per_player;j<leftover_cards+(i+1)*cards_per_player;j++){
                    if(j == deck->five_oros_index){
                        first_player=i;
                    }
                    player_hand[i].push_back(j);    //Add Card indexes to each player hand
                }
            }
            //Deal the leftover cards
            this->seed=std::chrono::system_clock::now().time_since_epoch().count();
            srand(this->seed);
            for(int i=0;i<leftover_cards;i++){
                //Get a random player
                int player = rand() % NumPlayers;
                //Check if player already took a leftover card and pick another one
                while(player_hand[player].size() > cards_per_player){
                    player = (player + 1) % NumPlayers;
                }
                player_hand[player].push_back(i);
                if(i == deck->five_oros_index){
                    first_player=player;
                }
            }
            current_player=first_player;    //Set the current player to the first

            //Create Players depending on strategy
            if(this->GameType == Game_Type::RANDOM_GAME){
                for(int i=0;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::RANDOM_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::SMART_GAME){
                for(int i=0;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::DUMB_GAME){
                for(int i=0;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::SMART_RANDOM_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::RANDOM_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::DUMB_RANDOM_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::RANDOM_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::SMART_DUMB_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::DUMB_SMART_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[i]));
                }
            }
        }

        //Constructor finited options
        Game(int NumPlayers, Game_Type GameType, Deck* deck, unsigned seed){
            this->NumPlayers=NumPlayers;
            this->GameType=GameType;
            this->seed=seed;
            
            //Link Deck
            this->deck=deck;

            //Create Table
            table = new Table(this->deck);

            //Prepare players hands
            int cards_per_player = Deck::SIZE / NumPlayers;
            int leftover_cards = Deck::SIZE % NumPlayers;
            
            vector<int> player_hand[NumPlayers];
            
            //Fill each hand
            for(int i=0;i<NumPlayers;i++){
                //Generate the right card indexes
                for(int j=leftover_cards+i*cards_per_player;j<leftover_cards+(i+1)*cards_per_player;j++){
                    if(j == deck->five_oros_index){
                        first_player=i;
                    }
                    player_hand[i].push_back(j);    //Add Card indexes to each player hand
                }
            }
            //Deal the leftover cards
            srand(this->seed);
            for(int i=0;i<leftover_cards;i++){
                //Get a random player
                int player = rand() % NumPlayers;
                //Check if player already took a leftover card and pick another one
                while(player_hand[player].size() > cards_per_player){
                    player = (player + 1) % NumPlayers;
                }
                player_hand[player].push_back(i);
                if(i == deck->five_oros_index){
                    first_player=player;
                }
            }
            current_player=first_player;    //Set the current player to the first

            //Create Players depending on strategy
            if(this->GameType == Game_Type::RANDOM_GAME){
                for(int i=0;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::RANDOM_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::SMART_GAME){
                for(int i=0;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::DUMB_GAME){
                for(int i=0;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::SMART_RANDOM_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::RANDOM_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::DUMB_RANDOM_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::RANDOM_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::SMART_DUMB_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[i]));
                }
            }else if(this->GameType == Game_Type::DUMB_SMART_GAME){
                this->players.push_back(Player(deck,table,Player_Strategy::DUMB_STRATEGY,player_hand[0]));
                for(int i=1;i<NumPlayers;i++){
                    this->players.push_back(Player(deck,table,Player_Strategy::SMART_STRATEGY,player_hand[i]));
                }
            }
        }

        //Destructor
        ~Game(){
            this->players.clear();
            delete(table);
            table=nullptr;
            delete(this->own_deck);
            this->own_deck=nullptr;
            this->deck=nullptr;
        }

        /* Function to play a turn
            -Returns true if the game ended, false otherwise
        */
        bool playTurn(){
            D(cout << "----Turn " << this->turn+1 << "----" << endl);
            D(this->table->printTable());
            D(cout << "Player " << this->current_player << " Hand: ");
            D(this->players[this->current_player].printHand());
            D(cout << "Player " << this->current_player << ": ");
            bool result=this->players[this->current_player].play();
            this->turn++;   //Mark turn played
            if(result){ //If the player won the game
                this->winner_player=this->current_player;   //Mark current player as the winner
            }else{
                this->current_player = (this->current_player + 1) % this->NumPlayers;   //Update current player
            }
            D(cout << "---------------" << endl);
            return result;
        }

        //Function to play an entire game
        void playGame(){
            //Play every turn
            while(!playTurn());

            D(cout << "----Game Finished----" << endl);
            D(cout << " - Winner: Player " << this->winner_player << endl);
            D(cout << "---------------------" << endl);

            //When the game is finished print the statistics
            D(this->printStatistics());
        }

        //Function to print the game statistics
        void printStatistics(){
            cout << "----------------Game Statistics----------------" << endl;
            cout << "\t- Type of Game: " << this->GameType << endl;
            cout << "\t- Number of Players: " << this->NumPlayers << endl;
            cout << "\t- Winner: Player " << this->winner_player << endl;
            cout << "\t- Total Turns: " << this->turn << endl;
            cout << "\t- First Player: " << this->first_player << endl;
            cout << "\t- Player Stats: " << endl;
            for(int i = 0;i<this->NumPlayers;i++){
                cout << "\t\t- Player " << i << ":" << endl;
                cout << "\t\t\t- Strategy Played: " << this->players[i].strategy << endl;
                cout << "\t\t\t- Started with: " << this->players[i].n_initial_cards << " cards" << endl;
                cout << "\t\t\t- Skipped Turns: " << this->players[i].turns_passed << endl;
                cout << "\t\t\t- Remaining Cards: " << this->players[i].hand.size() << endl;
            }
            
            cout << "-----------------------------------------------" << endl;
        }

        static void printCsvHeader(){
            cout << "Game Seed;Number of Players;Type of Game;Total Turns;First Player;Winner;Player 0 Strategy;Player 0 Initial Cards;Player 0 Skipped Turns;Player 0 Remaining Cards;Player 1 Strategy;Player 1 Initial Cards;Player 1 Skipped Turns;Player 1 Remaining Cards;Player 2 Strategy;Player 2 Initial Cards;Player 2 Skipped Turns;Player 2 Remaining Cards;Player 3 Strategy;Player 3 Initial Cards;Player 3 Skipped Turns;Player 3 Remaining Cards;Player 4 Strategy;Player 4 Initial Cards;Player 4 Skipped Turns;Player 4 Remaining Cards;Player 5 Strategy;Player 5 Initial Cards;Player 5 Skipped Turns;Player 5 Remaining Cards;" << endl;
        }

        /* Function to Print Stats in CSV Format following this format:
        Game Seed;Number of Players;Type of Game;Total Turns;First Player;Winner;Player 0 Strategy;Player 0 Initial Cards;Player 0 Skipped Turns;Player 0 Remaining Cards;Player 1 Strategy;Player 1 Initial Cards;Player 1 Skipped Turns;Player 1 Remaining Cards;Player 2 Strategy;Player 2 Initial Cards;Player 2 Skipped Turns;Player 2 Remaining Cards;Player 3 Strategy;Player 3 Initial Cards;Player 3 Skipped Turns;Player 3 Remaining Cards;Player 4 Strategy;Player 4 Initial Cards;Player 4 Skipped Turns;Player 4 Remaining Cards;Player 5 Strategy;Player 5 Initial Cards;Player 5 Skipped Turnn;Player 5 Remaining Cards;
        */
        void printCsvStats(){
            //Print Header
            //cout << "Game Seed;Number of Players;Type of Game;Total Turns;First Player;Winner;Player 0 Strategy;Player 0 Initial Cards;Player 0 Skipped Turns;Player 0 Remaining Cards;Player 1 Strategy;Player 1 Initial Cards;Player 1 Skipped Turns;Player 1 Remaining Cards;Player 2 Strategy;Player 2 Initial Cards;Player 2 Skipped Turns;Player 2 Remaining Cards;Player 3 Strategy;Player 3 Initial Cards;Player 3 Skipped Turns;Player 3 Remaining Cards;Player 4 Strategy;Player 4 Initial Cards;Player 4 Skipped Turns;Player 4 Remaining Cards;Player 5 Strategy;Player 5 Initial Cards;Player 5 Skipped Turns;Player 5 Remaining Cards;" << endl;
            cout << this->seed << ";";                              //Print the game seed
            cout << this->NumPlayers << ";";                        //Print the number of players in the game
            cout << this->GameType << ";";                          //Print the type of game
            cout << this->turn << ";";                              //Print the total number of turns of the game
            cout << this->first_player << ";";                      //Print the First Player Id
            cout << this->winner_player << ";";                     //Print the Winner of the Game Id
            for(int i = 0;i<this->NumPlayers;i++){
                cout << this->players[i].strategy << ";";           //Print the Strategy that player used
                cout << this->players[i].n_initial_cards << ";";    //Print the Number of Cards that player started with
                cout << this->players[i].turns_passed << ";";       //Print the Number of Turns that player had to skip
                cout << this->players[i].hand.size() << ";";        //Print the Remaining cards of that player
            }
            cout << endl;
        }
};

/* Idea for the statistics
    - Set different Number of Players (P): Get a collection of different numbers of players (N)
        - Cinquillo can be played from 3 to 6 players so P can be 3,4,5 or 6 and N should be equal to 4.
    - For each different P: Prepare a big number of different initial settings (M) (different initial cards for each player)
    - With the same cards (N,M) run 3 type of games
        - 1 Smart Game
        - 1 Dumb Game
        - Several (Rr) Random games (To analize the random choices)
        - Several (Rsr) Smart Random Games (To analize the random choices)
        - Several (Rdr) Dumb Random Games (To analize the random choices)
        - 1 Smart Dumb Game
        - 1 Dumb Smart Game
    - This way, the number of results will be (N*M*(1+1+Rr+Rsr+Rdr+1+1))
*/

void run_statistics(){
    Deck* deck = nullptr;
    Game* game=nullptr;
    unsigned seed = 0;
    int n_initial_settings = 1000;
    int n_random_simulations = 100;

    Game::printCsvHeader();

    //Iterate through different number of players
    for(int num_players=3;num_players<=6;num_players++){
        //Try different deck shuffles
        for(int i=0;i<n_initial_settings;i++){
            //Prepare a deck and a seed
            seed = std::chrono::system_clock::now().time_since_epoch().count();
            deck=new Deck();

            //Play 1 Smart Game
            game = new Game(num_players,Game_Type::SMART_GAME,deck,seed);
            game->playGame();
            game->printCsvStats();
            delete(game);
            game=nullptr;

            //Play 1 Dumb Game
            game = new Game(num_players,Game_Type::DUMB_GAME,deck,seed);
            game->playGame();
            game->printCsvStats();
            delete(game);
            game=nullptr;

            //Play 1 Smart Dumb Game
            game = new Game(num_players,Game_Type::SMART_DUMB_GAME,deck,seed);
            game->playGame();
            game->printCsvStats();
            delete(game);
            game=nullptr;

            //Play 1 Dumb Smart Game
            game = new Game(num_players,Game_Type::DUMB_SMART_GAME,deck,seed);
            game->playGame();
            game->printCsvStats();
            delete(game);
            game=nullptr;

            //Play n_random_simulations of Random Games
            for(int j=0;j<n_random_simulations;j++){
                game = new Game(num_players,Game_Type::RANDOM_GAME,deck,seed);
                game->playGame();
                game->printCsvStats();
                delete(game);
                game=nullptr;
            }

            //Play n_random_simulations of Smart Random Games
            for(int j=0;j<n_random_simulations;j++){
                game = new Game(num_players,Game_Type::SMART_RANDOM_GAME,deck,seed);
                game->playGame();
                game->printCsvStats();
                delete(game);
                game=nullptr;
            }

            //Play n_random_simulations of Dumb Random Games
            for(int j=0;j<n_random_simulations;j++){
                game = new Game(num_players,Game_Type::DUMB_RANDOM_GAME,deck,seed);
                game->playGame();
                game->printCsvStats();
                delete(game);
                game=nullptr;
            }

            delete(deck);
            deck=nullptr;
        }
    }

    
}

/*  Idea for the statistical analysis
        - Proportion of wins for each player in random games (To check it's an even distribution)
            - Take the total of games of n players and type random (N)
            - Take the number of games won by player 0 (W0)
            - Take the number of games won by player 1 (W1)
            - ...
            - Take  the number of games won by player n-1 (W(n-1))
            - Get the Win Ratio for each player: Wx / N
            - Check if that is aproximately 1 / n (Num of Players)
        - Proportion of wins for player 0 in smart random games
            - Take the total of games of n players and type smart random (N)
            - Take the number of games won by player 0 (W0)
            - Get the Win Ratio of player 0: W0 / N
        - Proportion of wins for player 0 in dumb random games
            - Take the total of games of n players and type dumb random (N)
            - Take the number of games won by player 0 (W0)
            - Get the Win Ratio of player 0: W0 / N
        - Proportion of wins for player 0 in smart dumb games
            - Take the total of games of n players and type smart dumb (N)
            - Take the number of games won by player 0 (W0)
            - Get the Win Ratio of player 0: W0 / N
        - Proportion of wins for player 0 in dumb smart games
            - Take the total of games of n players and type dumb smart (N)
            - Take the number of games won by player 0 (W0)
            - Get the Win Ratio of player 0: W0 / N
        - Relation between the winner and the initial player in random games
            - Take the total of games of n players and type random (N)
            - Take in how many the initial player is the winner (P)
            - Apply formula: Percentage = P / N
        - Relation between the winner and the number of initial cards in random games with different hand sizes (3 and 6 players)
            - Take the total of games of n players and type random (N)
            - Take the possible numbers of initial cards (C)
            - For each C, get Frequency of Winners (Fc)
            - For each C, get the win ratio: Fc / N
        - Amount of total turns depending on number of players and game type
            - For each number of players (n)
                - For each game type (G)
                    - Write down the total turns
            - Make a bar chart
        - Variability of winner with the same initial hands in random games
        - Relation between total number of turns and type of game
        - Given the same setup, Percentage of games won by player 0 because of aplying smart strategy
            - For each n of players:
                - Take each seed (S: Number of Seeds):
                    - Take the mode of the winner in random games with that seed (Wr)
                    - Take the mode of the winner in smart random games with that seed (Wsr)
                    - If Wr was different of player 0 and Wsr is player 0 -> Write down "1", otherwise -> Write down "0"
                - Get the percentage: Sum all the 1s and 0s and divide by S
*/

int main(){
    run_statistics();

    return 0;
}