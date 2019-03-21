#include <bits/stdc++.h>
using namespace std;


/*
FCFS = First Come First Served
SJF = Shortest Job First (non-preemptive)
SRTF = Shortest Remaining Time First  (SJF preemptive)
P = Priority (preemptive),
RR Q = Round-Robin (the number after the  RR represents the time quantum).
*/

struct Process {
	int arrival, length, priority, p_id;
	Process() : arrival(0), length(0), priority(0), p_id(-1) {}
	Process(int a, int b, int c, int d) : arrival(a), length(b), priority(c), p_id(d) {}
};

void FCFS(Process p[], int n) {
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{
		if (a.arrival == b.arrival) return a.p_id < b.p_id;
		return a.arrival < b.arrival;
	});
	for (int i = 0; i < n; i++) {
		cout << p[i].arrival << ' ' << p[i].p_id << ' ' << p[i].length << 'X' << endl;
	}
}

void SJF(Process p[], int n) {
	
}

void P(Process p[], int n) {
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{
		// arrival, then prio, then index
		if (a.arrival == b.arrival) return a.p_id < b.p_id;
		else if (a.priority == b.priority) return a.arrival < b.arrival;
		return a.p_id < b.p_id;
	});
	for (int i = 0; i < n; i++) {
		cout << p[i].arrival << ' ' << p[i].p_id << ' ' << p[i].length << 'X' << endl;
	}
}

// basically always do shortest remaining time
// thus you have to make sure its sorted by remaining time
void SRTF(Process p[], int n) {
	priority_queue<Process, vector<Process>, 
		function<bool(Process,Process)> > pq
		([](const Process &a, const Process &b) -> bool {
		if (a.length == b.length) return a.p_id < b.p_id;
		return a.length < b.length;
	});
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{ // sort it by arrival time, and then by length
		if (a.arrival == b.arrival) {
			if (a.length == b.length) return a.p_id < b.p_id;
			else return a.length < b.length;
		}
		return a.arrival < b.arrival;
	});
	int cur_time = p[0].arrival; // get the earliest time
	int index = 0;
	for(; p[index].arrival == cur_time && index < n; index++) pq.push(p[index]);
	while (!pq.empty()) {
		Process t = pq.top(); pq.pop();
		if (pq.size() > 0) {
			Process nt = pq.top();
			if (cur_time+t.length <= nt.arrival) {
				cout << cur_time << ' ' << t.p_id << ' ' << cur_time+t.length << "X\n";
				cur_time = nt.arrival;
			} else if (cur_time+t.length-nt.arrival <= nt.length) {
				cout << cur_time << ' ' << t.p_id << ' ' << cur_time+t.length << "X\n";
				cur_time += t.length;
			} else {
				cout << cur_time << ' ' << t.p_id << ' ' << nt.arrival << "\n";
				pq.push({nt.arrival, cur_time+t.length-nt.arrival, t.priority, t.p_id});
				cur_time = nt.arrival;
			}
		} else if (index < n) { // assumption here is wala na sa shit
			bool first_higher = 0;
			// pq.push(t);
			for(; !first_higher && index < n; index++) {
				if (cur_time+t.length > p[index].arrival) first_higher = 1;
				pq.push(p[index]);
			}
			
		} else {
			cout << cur_time << ' ' << t.p_id << ' ' << t.length << "X\n";
		}
		
	}
	
}

void RR_Q (Process p[], int n) {
	
}


int main () {
	int tc; 
	cin >> tc;
	while (tc--) {
		int n; string process;
		cin >> n >> process;
		Process processes[n];
		for (int i = 0; i < n; i++) {
			int a, b, c; cin >> a >> b >> c;
			processes[i] = {a,b,c,i+1};
			// cout << processes[i].arrival << ' ' << processes[i].length << ' ' << processes[i].priority << endl;
		}
		cout << tc+1 << endl;
		if (process == "FCFS") {
			FCFS(processes, n); // O(n lg n)
		} else if (process == "SJF") {
			SJF(processes, n); // O(n lg n)
		} else if (process == "SRTF") {
			SRTF(processes, n);
		} else if (process == "P") {
			P(processes, n); // O(n lg n)
		} else if (process == "RRQ") {
			
		}
	}
	
	return 0;
}