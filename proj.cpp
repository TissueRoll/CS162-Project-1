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
	int index = 0;
	int current_time = p[0].arrival;
	for (;current_time == p[index].arrival; index++) pq.push(p[index]);
	while (!pq.empty()) {
		Process t = pq.top(); pq.pop(); // current running process
		cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
		for (; index < n; index++) {
			if (current_time + t.length < p[index].arrival) break;
			pq.push(p[index]);
		}
		current_time += t.length;
		if (pq.size() == 0 && index < n) pq.push(p[index++]);
	}
}

void P(Process p[], int n) { // not done
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
		if (a.length == b.length) return a.p_id > b.p_id;
		return a.length > b.length;
	});
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{ // sort it by arrival time, and then by length
		if (a.arrival == b.arrival) {
			if (a.length == b.length) return a.p_id < b.p_id;
			else return a.length < b.length;
		}
		return a.arrival < b.arrival;
	});
	int index = 0;
	int current_time = p[0].arrival;
	bool ok = 0; int cnt = 0;
	for (;current_time == p[index].arrival; index++) pq.push(p[index]);
	while (!pq.empty()) { // pq will contain processes that are currently in the ready queue
		ok = 0;
		Process t = pq.top(); pq.pop(); // current running process
		// cout << "Process: " << t.p_id << ' ' << t.length << endl;
		current_time = max(current_time, t.arrival);
		for (; index < n; index++) { // go through the array if there still more to process
			if (current_time + t.length < p[index].arrival) {
				cout << "Process ID: " << t.p_id << endl;
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				current_time += t.length;
				ok = 1;
				break;
			}
			int tmp = current_time + t.length - p[index].arrival; 
			if (tmp > p[index].length) { // when the new process is better
				cout << current_time << ' ' << t.p_id << ' ' << p[index].arrival - current_time << endl;
				pq.push(p[index]);
				pq.push({p[index].arrival, tmp, t.priority, t.p_id});
				current_time = p[index].arrival;
				ok = 1;
				break;
			} else {
				pq.push(p[index]);
			}
			
		}
		// if (cnt > 15) break;
		if (!ok) {
			cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
			// cout << "abc" << endl;
			// cout << index << endl;
			current_time += t.length;
		}
		if (pq.size() == 0 && index < n) {
			// cout << "def" << endl;
			pq.push(p[index++]); // if empty but theres still some processes
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