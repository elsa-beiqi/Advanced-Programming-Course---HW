#include <iostream>
#include <thread>
#include <mutex>

#include <chrono>
#include <vector>

using namespace std;

// prototype for start function
void start(int, int);

// Start signal that will mark the start of each thread
bool startSignal;

// to keep track of ranking
int currentRank = 0;

// Mutex for printing and vector operations
mutex print, vecMutex, current_racer_mutex;

// Returns a random number \in [min, max]
float random(const float& min, const float& max)
{
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*(max - min) + min;
}


struct Runner {
public:
	// VARIABLES	// DEFINITIONS 
	float speed;	// Runner's speed
	int rid;		// Runner's ID
	int tid;		// Runner's team ID
	bool reached;	// Whether runner has reached destination
	thread* t;		// Runner's thread handle

	// Constructor
	Runner(int rid, int tid)
		:speed(random(4, 8)),
		rid(rid),
		tid(tid),
		reached(false)
	{
		//TODO: Start the corresponding thread here in the constructor
		t = new thread(start, tid, rid);
		// Printing information about it
		print.lock();
		cout << "*******\nTeam id: " << tid << "\tRunner id: " << rid << "\nSpeed: " << speed << "\n*******\n";
		print.unlock();
	}
};


struct Team {
public:
	vector<Runner> teamList;
	bool finished;
	int rank;
	int count;
	int currentRacer;

	Team(int count) :count(count), currentRacer(0), finished(false) {}
};

// to store teams in the race
vector<Team> teams;
vector<int> rankingTeams;


void start(int tid, int rid) {
	// Distance to the center hub
	int dist = 200;

	while (!startSignal) {
		this_thread::yield();
	}

	// Seed for the random generator using thread's id 
	// (not runner id you provide)
	srand(hash<thread::id>()(this_thread::get_id()) % 10000);

	// Getting current Runner and Team object
	vecMutex.lock();
	Team* t = &teams[tid];
	Runner* r = &t->teamList[rid];
	vecMutex.unlock();
	float distance_travelled = 0;

	// Running
	while (distance_travelled < dist) {
		this_thread::sleep_for(chrono::milliseconds(50));
		//TODO: update the travelled distance
		current_racer_mutex.lock();
		distance_travelled += r->speed;
		current_racer_mutex.unlock();
	}
	//running finished
	r->reached = true;

	//TODO: Implement the mechanism to start the next thread (next runner in the team) here
	vecMutex.lock();
	if(rid < 2){
		t->teamList.push_back(Runner(rid+1, tid));
		t->currentRacer = t->currentRacer + 1; // Updating currentRacer
	}
	vecMutex.unlock();
	
}

//TODO: - implement a function to print the rankings
void printRanking() {
	cout << "\n\n---- RANKS ----\n" << endl;
	for(int i = 0; i < rankingTeams.size(); i++){
		cout << "[" << i+1 << "]" << " - Team " << rankingTeams[i] << endl;
	}
}


// --------------------------------------------------------------------------------------------
// You don't need to change anyting in the main, however you are free to do any change you need
int main() {
	// Start signal, initally not fired
	startSignal = false;

	// Number of participants
	int numberOfTeams = 4;
	int runnersInTeams = 3;

	for (int i = 0; i < numberOfTeams; i++) {
		teams.push_back(Team(runnersInTeams));
		teams[i].teamList.push_back(Runner(0, i));
	}

	cout << "\n******************************************************************************\n"
		<< "Race starting with the signal now\t\t\t" << endl
		<< "******************************************************************************\n";

	startSignal = true;

	while (currentRank != numberOfTeams) {
		current_racer_mutex.lock(); // For the currentRacer, since it is updated outside
		for (int i = 0; i < numberOfTeams; i++) {
			if (currentRank == numberOfTeams) {
				break;
			}
			else if (teams[i].currentRacer == runnersInTeams - 1) {
				vecMutex.lock();
				if (teams[i].teamList[runnersInTeams - 1].reached && !teams[i].finished) {
					print.lock();
					teams[i].finished = true;
					cout << "Team " << i << " has finished the race\t\t\t" << endl;
					rankingTeams.push_back(i);
					currentRank++;
					teams[i].rank = currentRank;
					print.unlock();
					if (teams[i].teamList[0].t->joinable()) {
						teams[i].teamList[0].t->join();
					}
				}
				vecMutex.unlock();
			}
		}
		current_racer_mutex.unlock();
	}
	printRanking();

	system("pause");
	return 0;
}
