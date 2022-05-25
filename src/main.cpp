#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include <vector>


using namespace std;

class Context;



class Player;

class Gamemode{
    public:
	Gamemode() = default;
	virtual ~Gamemode() = default;
	virtual bool execute(vector<int> numbers) = 0;


};




class FiveEvenNumbers: public Gamemode{
    public:
	bool execute(vector<int> numbers) override{
	    int count = 0;
	    for(auto& i: numbers)
		if(i % 2 == 0)
		    ++count;
		
	    
	    return count >= 5;

	}

};


class FiveOddNumbers: public Gamemode{
    public:
	bool execute(vector<int> numbers) override{
	    int count = 0;
	    for(auto& i: numbers)
		if(i % 2 == 1)
		    ++count;


	    return count >= 5;
	}

};

class OnePrimeNumber: public Gamemode{
    public:
	bool execute(vector<int> numbers) override{

	    for(auto& i: numbers){
		int n_divisores = 2;
		for(int j = 2;j < i;++j)
		    if(i % j == 0)
			++n_divisores;
		if(n_divisores == 2)
		    return true;
	    }


	    return false;



	}

};


class ThreeTenMultiples: public Gamemode{
    public:
	bool execute(vector<int> numbers) override{
	    int count = 0;
	    for(auto& i: numbers)
		if(i % 10 == 0)
		    ++count;


	    return count >= 3;
	}

};


class TwoTwentyFiveMultiples: public Gamemode{
    public:
	bool execute(vector<int> numbers) override{
	    int count = 0;
	    for(auto& i: numbers)
		if(i % 25 == 0)
		    ++count;


	    return count >= 2;
	}

};




class Context{
    private:
	shared_ptr<Gamemode> gamemode;
    public:
	Context() = default;
	virtual ~Context() = default;
	void set_gamemode(shared_ptr<Gamemode> gamemode){ this->gamemode = gamemode;}
	bool check_if_player_winned(vector<int> numbers, int id_gamemode){
	    if(id_gamemode == 1)
		set_gamemode(make_shared<FiveEvenNumbers>());
	    else if(id_gamemode == 2)
		set_gamemode(make_shared<FiveOddNumbers>());
	    else if(id_gamemode == 3)
		set_gamemode(make_shared<OnePrimeNumber>());
	    else if(id_gamemode == 4)
		set_gamemode(make_shared<ThreeTenMultiples>());
	    else if(id_gamemode == 5)
		set_gamemode(make_shared<TwoTwentyFiveMultiples>());
	    else
		throw std::invalid_argument("No se encontro la accion");

	    return gamemode->execute(numbers);

	}


};

class Player{
    private:
	Context context;
	vector<int> numbers_for_this_player;
	int id_gamemode;
	bool winner;
    public:
	void add_number(int i){
	    numbers_for_this_player.push_back(i);
	}
	vector<int>& get_numbers(){ return numbers_for_this_player;}

	bool has_win(){
	    return context.check_if_player_winned(numbers_for_this_player, id_gamemode);
	}


};


class Observer{
    protected:
	shared_ptr<Player> player;
    public:
	Observer(shared_ptr<Player> player){this->player = player;}
	virtual void update(int number) = 0;
	

};

class PlayerObserver: public Observer{
    public:
	PlayerObserver(shared_ptr<Player> player):Observer{player}{}
	void update(int number) override{
	    this->player->add_number(number);
	}
};



class Game: enable_shared_from_this<Game>{
    private:
	static shared_ptr<Game> game;
	vector<shared_ptr<Observer>> observers;
	vector<int> random_numbers;
	int actual_number;
	Game() {}
    public:
	~Game() = default;
	static shared_ptr<Game> get_game(){
	    if(game == nullptr){
		game = make_shared<Game>();
		for(int i = 1; i < 101; ++i)
		    game->random_numbers.push_back(i);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(game->random_numbers.begin(), game->random_numbers.end(), g);
		game->actual_number = 0;

	    }
	    return game;
	}
	void subscribe_player(shared_ptr<Player> player){
	    observers.push_back(make_shared<PlayerObserver>(player));
	}
	void next_number(){
	    for(auto&i : observers)
		i->update(actual_number);
	    ++actual_number;


	}


};






int main(){
 

}
