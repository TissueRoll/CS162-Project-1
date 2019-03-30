#include <bits/stdc++.h>
using namespace std;

#ifdef DEBUG
#define debug_text(...) printf(__VA_ARGS__)
#else
#define debug_text(...) /*---NOTHING---*/
#endif

struct Process {
	int arrival, length, priority, p_id, first_burst, termination;
	Process() : arrival(0), length(0), priority(0), p_id(-1), first_burst(0), termination(0) {}
	Process(int a, int b, int c, int d) : arrival(a), length(b), priority(c), p_id(d) {}
};

void FCFS(Process p[], int n) {
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{
		if (a.arrival == b.arrival) return a.p_id < b.p_id;
		return a.arrival < b.arrival;
	});
	int current_time = p[0].arrival;
	for (int i = 0; i < n; i++) {
		debug_text("CURRENT TIME: %d\n", current_time);
		current_time = max(current_time, p[i].arrival);
		cout << current_time << ' ' << p[i].p_id << ' ' << p[i].length << 'X' << endl;
		current_time += p[i].length;
	}
}

void SJF(Process p[], int n) {
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
	for (;current_time == p[index].arrival && index < n; index++) pq.push(p[index]);
	while (!pq.empty()) {
		Process t = pq.top(); pq.pop(); // current running process
		current_time = max(current_time, t.arrival);
		cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
		for (; index < n; index++) {
			if (current_time + t.length < p[index].arrival) break;
			pq.push(p[index]);
		}
		current_time += t.length;
		if (pq.size() == 0 && index < n) pq.push(p[index++]);
	}
}

void P(Process p[], int n) { // probably done?
	priority_queue<Process, vector<Process>, 
		function<bool(Process,Process)> > pq
		([](const Process &a, const Process &b) -> bool {
		if (a.priority == b.priority) return a.p_id > b.p_id;
		return a.priority > b.priority;
	});
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{ // sort it by arrival time, and then by priority
		if (a.arrival == b.arrival) {
			if (a.priority == b.priority) return a.p_id < b.p_id;
			else return a.priority < b.priority;
		}
		return a.arrival < b.arrival;
	});
	int index = 0;
	int current_time = p[0].arrival;
	bool ok = 0; int cnt = 0;
	for (;current_time == p[index].arrival && index < n; index++) pq.push(p[index]);
	while (!pq.empty()) { // pq will contain processes that are currently in the ready queue
		Process t = pq.top(); pq.pop(); ok = 0;
		current_time = max(current_time, t.arrival);
		debug_text("--- CURRENT TIME: %d ---\n", current_time);
		debug_text("Process ID: %d || Priority: %d\n", t.p_id, t.priority);
		while (index < n) {
			if (current_time + t.length < p[index].arrival) { // this process will finish before next
				debug_text("CURRENT PROCESS ENDS BEFORE NEXT INDEX\n");
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				current_time += t.length;
				ok = 1;
				break;
			}
			// if above isn't satisfied, then interception happens
			if (t.priority > p[index].priority) {
				debug_text("CURRENT PROCESS INTERRUPTED. PROCESS %d PUSHED\n", p[index].p_id);
				cout << current_time << ' ' << t.p_id << ' ' << p[index].arrival - current_time << endl;
				pq.push(p[index]);
				pq.push({p[index].arrival, current_time + t.length - p[index].arrival, t.priority, t.p_id});
				ok = 1;
				current_time = p[index++].arrival;
				break;
			} else {
				pq.push(p[index++]);
			}
		}
		if (!ok) {
			cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
			current_time += t.length;
		}
		if (pq.size() == 0 && index < n) {
			pq.push(p[index++]); // if empty but theres still some processes
		}
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
	for (;current_time == p[index].arrival && index < n; index++) pq.push(p[index]);
	while (!pq.empty()) { // pq will contain processes that are currently in the ready queue
		ok = 0;
		Process t = pq.top(); pq.pop(); // current running process
		debug_text("--- CURRENT TIME: %d ---\n", t.p_id);
		debug_text("Process ID: %d || Length: %d\n", t.p_id, t.length);
		current_time = max(current_time, t.arrival);
		while (index < n) {
			if (current_time + t.length < p[index].arrival) {
				debug_text("CURRENT PROCESS ENDS BEFORE NEXT INDEX\n");
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				current_time += t.length;
				ok = 1;
				break;
			}
			// implies interception happens
			int tmp = current_time + t.length - p[index].arrival; 
			if (tmp > p[index].length) { // when the new process is better
				debug_text("CURRENT PROCESS INTERRUPTED -- PROCESS %d PUSHED\n", p[index].p_id);
				cout << current_time << ' ' << t.p_id << ' ' << p[index].arrival - current_time << endl;
				pq.push(p[index]);
				pq.push({p[index].arrival, tmp, t.priority, t.p_id});
				current_time = p[index++].arrival;
				ok = 1;
				break;
			} else {
				pq.push(p[index++]);
			}
		}
		if (!ok) {
			cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
			current_time += t.length;
		}
		if (pq.size() == 0 && index < n) {
			pq.push(p[index++]); // if empty but theres still some processes
		}
	}
	debug_text("FINAL TIME: %d\n", current_time);
}

void RR (Process p[], int n, int q) {
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{
		if (a.arrival == b.arrival) return a.p_id < b.p_id;
		return a.arrival < b.arrival;
	});
	queue<Process> que;
	int index = 0;
	int current_time = p[0].arrival;
	bool ok = 0; int cnt = 0;
	for (;current_time == p[index].arrival && index < n; index++) que.push(p[index]);
	// for (int i = 0; i < n; i++) {
	// 	debug_text("CONTENT OF PROCESS %d:\nARRIVAL: %d, LENGTH: %d, PRIORITY: %d\n", p[i].p_id, p[i].arrival, p[i].length, p[i].priority);
	// 	que.push(p[i]);
	// }
	// int current_time = que.front().arrival;
	while (!que.empty()) {
		Process t = que.front(); que.pop();
		current_time = max(current_time, t.arrival);
		debug_text("CURRENT_TIME: %d || LENGTH: %d\n", current_time, t.length);
		if (t.length <= q) {
			cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
			current_time += t.length;
		} else {
			if (index >= n && que.size() == 0) {
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				current_time += t.length;
			} else {
				cout << current_time << ' ' << t.p_id << ' ' << q << "\n";
				que.push({current_time, t.length-q, t.priority, t.p_id});
				current_time += q;
			}
		}
		while (p[index].arrival < current_time && index < n) que.push(p[index++]);
		if (que.size() == 0 && index < n) {
			que.push(p[index++]); // if empty but theres still some processes
		}
	}
}


int main () {
	int tc, ctc = 0; 
	cin >> tc;
	while (tc--) {
		int n, time_quantum; string process;
		cin >> n >> process; 
		time_quantum = -1;
		Process processes[n];
		if (process == "RR") cin >> time_quantum;
		
		for (int i = 0; i < n; i++) {
			int a, b, c; cin >> a >> b >> c;
			processes[i] = {a,b,c,i+1};
		}
		
		cout << (++ctc) << endl;
		if (process == "FCFS") {
			FCFS(processes, n); // O(n lg n)
		} else if (process == "SJF") {
			SJF(processes, n); // O(n lg n)
		} else if (process == "SRTF") {
			SRTF(processes, n); // O(2n lg n)
		} else if (process == "P") {
			P(processes, n); // O(n lg n)
		} else if (process == "RR") {
			RR(processes, n, time_quantum); // O(n)
		}
	}
	
	return 0;
}