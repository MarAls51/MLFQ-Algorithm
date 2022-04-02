#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>

using namespace std;
#define Q1 8; 
#define Q2 12;


class Process {
public:
	int arrival = 0;
	vector <vector <int>> cpuIO;
	int response = 0;
	int wait = 0;
	int taround = 0;
	int finish = 0;
	int queue = 1;
	int id;
};

void Burst1(Process& p, int btime, int& clock, vector <Process> IO, int& preempt1) {
	//cout << btime << endl;
	clock = clock + btime;
	cout << p.id << " :Q1" << " : " << clock << endl;
	p.cpuIO[0][0] = (p.cpuIO[0][0] - btime);
	if (p.cpuIO[0][0] <= 0) {
		p.cpuIO[0][0] = 0;
		p.queue = 1;
	}
	else
	{
		p.queue = 2;
	}

}


void Burst2(Process& p, int btime, int& preempt1, int& clock, vector <Process>& IO) {
	if (clock + btime <= preempt1) {
		p.cpuIO[0][0] = (p.cpuIO[0][0] - btime);
		clock = clock + btime;
		cout << p.id << " :Q2 --1" << " : " << clock << endl;
		//cout << "executed" << endl;
	}
	else {
		//cout << preempt1 << " : c " << clock << endl;
		btime = preempt1 - clock;
		p.cpuIO[0][0] = (p.cpuIO[0][0] - btime);
		clock = clock + btime;
	}
	if (p.cpuIO[0][0] <= 0) {
		p.cpuIO[0][0] = 0;
		p.queue = 2;
	}
	else
	{
		p.queue = 3;
	}
	//cout << "First element Q2 is" << queue2.front().Pid << endl;
	//cout << "Last element Q2 is" << queue2.back().Pid << endl;
}


void Burst3(Process& p, int btime, int preempt1, int preempt2, int& clock) {

	p.cpuIO[0][0] = (p.cpuIO[0][0] - 8);
	if (p.cpuIO[0][0] <= 0) {
		p.cpuIO[0][0] = 0;
		p.queue = 1;
	}
	else
	{
		p.queue = 2;
	}


}


void IOpush(Process& p, int preempt1, int preempt2, int& clock, vector <Process>& IO, vector <vector<int>>& preempt) {
	p.arrival = (clock + p.cpuIO[0][1]);
	//cout << p.id << ": called" << endl;
	//cout << p.cpuIO[0][1] << ":called 2" << endl;
	preempt1 = 1000;
	preempt2 = 1000;
	//cout << p.arrival << endl;
	int loc;

	for (loc = 0; loc < preempt.size(); loc++)
	{
		if (preempt[loc][0] == p.id)
		{
			//cout << p.id << endl;
			//cout << p.arrival << "The p" << endl;
			preempt[loc][1] += p.arrival;
			break;
		}
	}
	for (int i = 0; i < IO.size(); i++)
	{
		//cout << IO[i].id << "test2" << endl;
		//cout << preempt[loc][0] << "test" << endl;
		if (IO[i].id == preempt[loc][0])
		{
			//cout << preempt[loc][1] << "The arrival" << endl;
			IO[i].arrival = preempt[loc][1];
			break;
		}
	}
	for (int i = 0; i < IO.size(); i++)
	{
		if (IO[i].arrival < preempt1 && IO[i].queue == 1)
		{
			//cout << IO[i].arrival << " :arrival IO" << endl;
			preempt1 = IO[i].arrival;
		}
		if (IO[i].arrival < preempt2 && IO[i].queue == 2)
		{
			preempt2 = IO[i].arrival;
		}
	}
	if (p.cpuIO[0][0] == NULL)
	{
		p.cpuIO.erase(p.cpuIO.begin());
	}
	/*for (int i = 0; i < preempt.size(); i++)
	{
		cout << preempt[i][0] << " : " << preempt[i][1] << endl;
	}*/
	IO.push_back(p);
}
void MLFQ(queue <Process>& queue1, queue <Process>& queue2, queue <Process>& queue3, vector <Process>& IO, int preempt1, int preempt2, int& clock, vector <vector<int>>& preempt)
{
	// while currentArrivalTime
	int n = 0;
	while (clock <= 150) {

		if (!IO.empty())
		{
			for (int i = 0; i < IO.size(); i++)
			{
				if (IO[i].arrival <= clock)
				{
					//cout << IO[i].id << ":" << IO[i].arrival << " : " << clock << endl;
					//cout << IO[i].cpuIO[0][0] << " : " << IO[i].cpuIO[0][1] << endl;;
					if (IO[i].queue == 1)
					{
						queue1.push(IO[i]);
						IO.erase(IO.begin() + i);
					}
					if (IO[i].queue == 2)
					{
						queue2.push(IO[i]);
						IO.erase(IO.begin() + i);
					}
					if (IO[i].queue == 3)
					{
						queue3.push(IO[i]);
						IO.erase(IO.begin() + i);
					}
				}
			}
		}
		if (!queue1.empty()) {

			if (queue1.front().cpuIO[0][0] < 8) {
				Burst1(queue1.front(), queue1.front().cpuIO[0][0], clock, IO, preempt1);
			}
			else {
				Burst1(queue1.front(), 8, clock, IO, preempt1);
			}
			if (queue1.front().queue == 1) {
				IOpush(queue1.front(), preempt1, preempt2, clock, IO, preempt);
				queue1.pop();
			}
			else {
				Process* temp = new Process;
				temp = &queue1.front();
				queue2.push(*temp);
				queue1.pop();
			}
			//cout << "First element Q2 is" << queue1.front().id << endl;
			//cout << "Last element Q2 is" << queue1.back().id << endl;
		}
		else if (!queue2.empty()) {
			//cout << "First element Q2 is" << queue2.front().Pid << endl;
			//cout << "Last element Q2 is" << queue2.back().Pid << endl;
			if (queue2.front().cpuIO[0][0] < 12)
				Burst2(queue2.front(), queue2.front().cpuIO[0][0], preempt1, clock, IO);
			else
				Burst2(queue2.front(), 12, preempt1, clock, IO);


			if (queue2.front().queue == 2) {
				//Process* temp = &queue1.front(); //needs work
				//IO.push(*temp); //needs work

				IOpush(queue2.front(), preempt1, preempt2, clock, IO, preempt);
				queue2.pop();
			}
			else {
				//Process* temp = &queue2.front();
				queue3.push(queue2.front()); //needs work
				queue2.pop();

			}

		}
		else {
			//Burst3(queue3.front(), preempt1, preempt2, clock);
		}
		n++;
		//cout << "First element in Q1 is" << queue1.front().Pid << endl;
		//cout << "Last element in Q1 is" << queue1.back().Pid << endl;

	}


}
// readyQueue
int main() {

	Process p1, p2, p3, p4, p5, p6, p7, p8, p9; //create all process instances
	//Process* temp;
	int clock = 0;
	int preempt1 = 999; //time that next queue1 preemption occurs, calculated every time a queue1 process gets sent to IO
	int preempt2 = 999; //time that next queue2 preemption occurs, calculated every time a queue2 process gets sent to IO
	//int queue1 = 9, queue2 = 0, queue3 = 0; //counter for number of processes in each queue. Incremented and decremented as processes switch
	queue <Process> queue1;
	queue <Process> queue2;
	queue <Process> queue3;
	vector <Process> IO;
	vector <vector<int>> preempt{ {1,0},{2,0} ,{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0} };
	//priority_queue <int, vector<int>, greater<int>> g = gq;

	vector <vector <int>> val1{ {7,22}, {6,19} ,{12,44} , {8,21} , {10,37}, {5,24}, {6,44}, {7,43}, {8,0} };
	vector <vector <int>> val2{ {14,48}, {15,44} , {17,42}, {22,37}, {19,76}, {14,41}, {16,31}, {17,43}, {18,0} };
	vector <vector <int>> val3{ {8,43}, {7,41} , {6,45}, {8,21}, {9,35}, {14,18}, {5,26}, {3,31}, {6,0} };
	vector <vector <int>> val4{ {13,37}, {4,41} , {5,35}, {12,41}, {8,55}, {15,34}, {6,73}, {5,77}, {3,0} };
	vector <vector <int>> val5{ {6,34}, {7,21} , {5,44}, {6,32}, {7,28}, {3,48}, {11,44}, {6,33}, {3,28}, {4,0} };
	vector <vector <int>> val6{ {9,32}, {4,28} , {5,10}, {6,12}, {7,14}, {9,18}, {12,24}, {15,30}, {8,0} };
	vector <vector <int>> val7{ {14,46}, {17,41} , {11,42}, {15,21}, {4,32}, {7,19}, {16,33}, {10,0} };
	vector <vector <int>> val8{ {4,64}, {5,53} , {6,44}, {4,73}, {6,87}, {5,66}, {8,25}, {6,33}, {9,41}, {7,0} };
	vector <vector <int>> val9{ {13,37}, {8,41} , {7,27}, {12,29}, {5,27}, {6,18}, {3,33}, {4,62}, {6,0} };

	p1.cpuIO = val1;
	p1.id = 1;
	p2.cpuIO = val2;
	p2.id = 2;
	p3.cpuIO = val3;
	p3.id = 3;
	p4.cpuIO = val4;
	p4.id = 4;
	p5.cpuIO = val5;
	p5.id = 5;
	p6.cpuIO = val6;
	p6.id = 6;
	p7.cpuIO = val7;
	p7.id = 7;
	p8.cpuIO = val8;
	p8.id = 8;
	p9.cpuIO = val9;
	p9.id = 9;

	queue1.push(p1);
	queue1.push(p2);
	queue1.push(p3);
	queue1.push(p4);
	queue1.push(p5);
	queue1.push(p6);
	queue1.push(p7);
	queue1.push(p8);
	queue1.push(p9);
	MLFQ(queue1, queue2, queue3, IO, preempt1, preempt2, clock, preempt);
	return 0;
}
