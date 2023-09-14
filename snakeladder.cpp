#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include<time.h> 

using namespace std;


ofstream fout;

class gamesquare
{
public:
	int pos = 0;
	map<int, int> neghbours;
	int action = 0;				//Default action unbiased die
	bool isFinal = false;
	int ndices = 3;
	double V = 0.;
	double Qval[3] = {0.};
	gamesquare(int pos1, int endpos, int ndices)
	{
		pos = pos1;
		for(int i = 1; i <= 6; i++)
		{
			if( pos1+i >= endpos) // If die value is more than end positon, then you stay at the same position
			{
				neghbours.insert(pair<int,int>(i,pos));
			}
			else
			{
				neghbours.insert(pair<int,int>(i,pos+i));
			}
		}


		if(pos1 == endpos-1)
		{	
			isFinal = true;
			V = 100.;
		}

		this->ndices = ndices;
	}


	void DisplayGameSquare()
	{
		cout<<"Pos = "<<pos<<endl;
		for(auto i = neghbours.begin(); i != neghbours.end(); i++)
		{
			cout<< i->first << ": "<<i->second<<endl;
		}
	}
};

struct dice
{
public:
	map<int, double> Probab;

};



class snakeladdergame
{
public:
	int totalsquares;
	vector<gamesquare> Board;
	
	int ndices = 3;
	struct dice Dices[3];

	

	vector<pair<int,int>> Listsnakeladder;



	void IntializeDices()
	{
		(Dices[0]).Probab = {
			{1, 1./6.},
			{2, 1./6.},
			{3, 1./6.},
			{4, 1./6.},
			{5, 1./6.},
			{6, 1./6.}
		};

		(Dices[1]).Probab = {
			{1, 0.25},
			{2, 0.25},
			{3, 0.25},
			{4, 0.05},
			{5, 0.05},
			{6, 0.05}
		};

		(Dices[2]).Probab = {
			{1, 0.05},
			{2, 0.05},
			{3, 0.05},
			{4, 0.25},
			{5, 0.25},
			{6, 0.25}
		};

	}

	snakeladdergame(string filename)
	{
		
		//cout<<filename<<endl;
		ifstream fin;
		fin.open(filename);
		

		fin >>	totalsquares;
		for(int i = 0; i < totalsquares; i++)
		{
			gamesquare Node(i, totalsquares, ndices);
			Board.push_back(Node);
		}

		
		int start, finish;
		while(fin >> start >> finish)
		{
			Listsnakeladder.push_back(make_pair(start,finish));
		}


		IntializeDices();
		UpdateBoard();
		//DisplayAllSquares();
		
	}


	void UpdateBoard()
	{
		cout<<"Following are the snakes and the ladders: "<<endl;
		for(auto &it : Listsnakeladder)
		{
			cout<<it.first<<" -> "<<it.second<<endl;
			for(int i = 1; i<= 6 ; i++)
			{
				if(it.first - i < 0)
					break;
				//cout<<it.first - i<<" ";
				//cout<<(Board[it.first - i]).neghbours[i]<<endl;
				(Board[it.first - i]).neghbours[i] = it.second;
			}

		}
		//cout<<"Out of UpdateBoard"<<endl;
	}

	

	int rolldie(int diceno)
	{
		
		double randnum = ((double) rand() / (RAND_MAX)) ;
		//cout<<"randnum = "<<randnum<<endl;
		double lenghtcovered = 0;
		for(int i = 1; i<=6; i++)
		{
			lenghtcovered = lenghtcovered + Dices[diceno].Probab[i];
			//cout<<lenghtcovered<<" "<<endl;
			if(lenghtcovered >= randnum)
			{
				//cout<<"returning "<<i<<endl;
				return i;
			}
		}
	}

	void ValueIteration(int iterations, double gamma)
	{
		for (int iter = 0; iter < iterations; iter++)
		{
			//Storing new values in temporary
			double TempQ[100][3];
			for(int i = 0; i<totalsquares-1; i++)
			{
				for(int act = 0; act<3; act++)
				{
					double sum = 0.;
					for(int dienum = 1; dienum<=6; dienum++)
					{
						sum = sum + ( Dices[act].Probab[dienum] * gamma * (Board[ Board[i].neghbours[dienum] ]).V  );
					}
					TempQ[i][act] = sum;
				}
			}


			//Updating V and Q values
			for(int i = 0; i<totalsquares-1; i++)
			{
				int argmaxQ = Board[i].action;
				for(int act = 0; act<3; act++)
				{
					if(Board[i].Qval[argmaxQ]  <  Board[i].Qval[act])
					{
						argmaxQ = act;
					}
					Board[i].Qval[act] = TempQ[i][act];
				}
				Board[i].action = argmaxQ;
				Board[i].V = Board[i].Qval[argmaxQ];
			}


		}
	}

	void PolicyIteration(int iterations, double gamma)
	{
		//generateRandomMoveset();

		int timeschanged = 0;
		while(true)
		{
			//Converging the RandomMoveSet's V
			for(int iter = 0; iter<iterations; iter++)
			{	
				//Calculating next iteration value and storing in TemppV
				double tempV[100];
				for(int i = 0; i<totalsquares-1; i++)
				{
					double sum = 0.;
					int this_squares_action = Board[i].action;
					for(int dienum = 1; dienum <= 6; dienum++)
					{
						sum += Dices[this_squares_action].Probab[dienum] * gamma * (Board[ Board[i].neghbours[dienum] ]).V ;
					}
					tempV[i] = sum;
				}
				//Updating V
				for(int i = 0; i<totalsquares-1; i++)
				{
					Board[i].V = tempV[i];
				}
			}

			//Calculating new policy
			int newPolicy[100]; 			//int because need to store action
			double V_newPolicy[100];
			
			for(int i = 0; i<totalsquares-1; i++)
			{
				int argMaxAction = Board[i].action;
				double V_MaxAction = Board[i].V;
				for(int act=0; act<3; act++)
				{
					double sum = 0.;
					for (int dienum = 1; dienum <= 6; dienum++)
					{
						sum += Dices[act].Probab[dienum] * gamma * (Board[ Board[i].neghbours[dienum] ]).V ;
					}
					if(sum > V_MaxAction)
					{
						argMaxAction = act;
						V_MaxAction = sum;
					}
				}
				newPolicy[i] = argMaxAction;
				V_newPolicy[i] = V_MaxAction;
			}

			//Now comparing both policies and updating
			bool isdifferent = false;
			for(int i = 0; i<totalsquares-1; i++)
			{
				if(newPolicy[i] != Board[i].action)
				{
					isdifferent = true;
					Board[i].action = newPolicy[i];
					Board[i].V = V_newPolicy[i];
				}
			}


			if(!isdifferent)
			{
				cout<<"returning with timeschanged = "<<timeschanged<<endl;
				return;
			}
			else
			{
				timeschanged++;
				cout<<"Changing policy for "<<timeschanged<<endl;
			}


		}

	}

	double AverageMoves(int iterations)
	{
		double avg = 0.; 
		for(int i = 0; i< iterations; i++)
		{
			int moves = 0;
			int currentsq = 0;
			while (currentsq !=  totalsquares-1)
			{
				moves += 1;
				int dieval = rolldie(Board[currentsq].action);
				currentsq = (Board[currentsq]).neghbours[dieval];
				//cout<<"Die = "<<dieval<<", Moving to "<<currentsq<<endl;
			}
			avg += moves;
		}
		avg = double(avg)/double(iterations);
		return avg;
	}

	void generateRandomMoveset()
	{
		for(int i = 0; i<totalsquares-1; i++)
		{
			int randact = rand()%3;
			Board[i].action = randact;
		}
	}

	void DisplaySnakesandLadders()
	{
		for(auto &it : Listsnakeladder)
		{
			cout<<it.first<<" -> "<<it.second<<endl;
		}
	}
	void DisplayAllSquares()
	{
		//cout<<"In DisplayAllSquares"<<endl;
		for(vector<gamesquare>::iterator it = Board.begin(); it!= Board.end(); it++)
		{
			it->DisplayGameSquare();
			cout<<endl;
		}
		//Board.front().DisplayGameSquare();
	}

	void DisplayMoves()
	{
		for(vector<gamesquare>::iterator it = Board.begin(); it!= Board.end(); it++)
		{
			cout<<"Pos = "<<it->pos<<"; Action = "<< it->action <<"; self V = "<<it->V<<endl; 
			//it->DisplayGameSquare();
			//cout<<endl<<endl;
		}
	}

	void LogMovesintoFile()
	{
		for(vector<gamesquare>::iterator it = Board.begin(); it!= Board.end(); it++)
		{
			fout<<it->pos<<":"<< it->action <<" "; 
			//it->DisplayGameSquare();
			//cout<<endl<<endl;
		}	
		fout<<endl;
	}

};



int main(int argc, char const *argv[])
{
	srand(time(0));
	fout.open("logs.txt", std::ios_base::app);
	int algorithm = atoi(argv[1]);
	snakeladdergame NewGame(argv[2]);
	double gamma = atof(argv[3]);
	

	cout<<"Algorithm = "<<algorithm<<", Gamma = "<<gamma<<endl;
	if(algorithm == 1)
	{
		fout<<"Value Iteration with gamma = "<<gamma<<endl;
		NewGame.ValueIteration(1000, gamma);
		NewGame.DisplayMoves();
		NewGame.LogMovesintoFile();
		cout<<"Running game with Moves generated with Value Iteration 50000 times..."<<endl;
		cout<<"Average no. of moves = "<<NewGame.AverageMoves(50000)<<endl;
	}	
	else if(algorithm == 2)
	{	
		NewGame.generateRandomMoveset();
		fout<<"Policy Iteration with gamma = "<<gamma<<endl;
		NewGame.PolicyIteration(1000, gamma);
		NewGame.DisplayMoves();
		NewGame.LogMovesintoFile();
		cout<<"Running game with Moves generated with Policy Iteration 50000 times..."<<endl;
		cout<<"Average no. of moves = "<<NewGame.AverageMoves(50000)<<endl;

	}
	else
	{
		//if(argc == 4)
		cout<<"Running the game with unbiased die for 50000 iteration..."<<endl;
		cout<<"Average no. of moves = "<<NewGame.AverageMoves(50000)<<endl;

	}
	return 0;
}