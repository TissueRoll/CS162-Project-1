#include <bits/stdc++.h>
using namespace std;

#ifdef _DEBUG
#define debug_text(...) printf(__VA_ARGS__)
#else
#define debug_text(...) /*---NOTHING---*/
#endif

struct Process {
	int arrival, length, priority, p_id, first_burst, termination;
	Process() : arrival(0), length(0), priority(0), p_id(-1), first_burst(-1), termination(-1) {}
	Process(int a, int b, int c, int d) : arrival(a), length(b), priority(c), p_id(d), first_burst(-1), termination(-1) {}
};

void compute_metrics(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp, int util_until) {
	double cpu_utilization = 0, waiting_time = 0, turnaround_time = 0, response_time = 0;
	int throughput = 0;
	for (int i = 0; i < n; i++) {
		if (p[i].termination <= throughput_timestamp) throughput++;
		response_time += p[i].first_burst; // the beginning (the line) of the burst is the response time
		// termination (counted by entire cc), length (counted by entire cc), arrival counted by LEFT BOUND
		// thus converting to entire cc means i need to subtract it by 1
		waiting_time += p[i].termination - p[i].length - max(p[i].arrival-(p[i].first_burst == p[i].arrival ? 0 : 1),0);
		turnaround_time += p[i].termination;
	}
	response_time /= n;
	waiting_time /= n;
	turnaround_time /= n;
	cpu_utilization = (double)util_until/(double)cpu_util_timestamp;
	printf("CPU Utilization: %.2f%%\n", cpu_utilization*100);
	printf("Throughput: %d\n", throughput);
	printf("Waiting time: %.2f\n", waiting_time);
	printf("Turnaround time: %.2f\n", turnaround_time);
	printf("Response time: %.2f\n", response_time);
}

void FCFS(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp) {
	Process pc[n];
	for (int i = 0; i < n; i++) pc[i] = p[i];
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{
		if (a.arrival == b.arrival) return a.p_id < b.p_id;
		return a.arrival < b.arrival;
	});
	int current_time = p[0].arrival, util_until = 0;
	for (int i = 0; i < n; i++) {
		debug_text("CURRENT TIME: %d\n", current_time);
		current_time = max(current_time, p[i].arrival);
		pc[p[i].p_id-1].first_burst = current_time; // first_burst = the first cc it runs
		if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + p[i].length) - current_time;
		cout << current_time << ' ' << p[i].p_id << ' ' << p[i].length << 'X' << endl;
		current_time += p[i].length;
		pc[p[i].p_id-1].termination = current_time; // termination = the last cc it runs
	}
	compute_metrics(pc, n, cpu_util_timestamp, throughput_timestamp, util_until);
}

void SJF(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp) {
	priority_queue<Process, vector<Process>, 
		function<bool(Process,Process)> > pq
		([](const Process &a, const Process &b) -> bool {
		if (a.length == b.length) return a.p_id > b.p_id;
		return a.length > b.length;
	});
	Process pc[n];
	for (int i = 0; i < n; i++) pc[i] = p[i];
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{ // sort it by arrival time, and then by length
		if (a.arrival == b.arrival) {
			if (a.length == b.length) return a.p_id < b.p_id;
			else return a.length < b.length;
		}
		return a.arrival < b.arrival;
	});
	int index = 0, util_until = 0;
	int current_time = p[0].arrival;
	while (index < n && current_time == p[index].arrival) pq.push(p[index++]);
	while (!pq.empty()) {
		Process t = pq.top(); pq.pop(); // current running process
		current_time = max(current_time, t.arrival);
		pc[t.p_id-1].first_burst = current_time;
		if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
		cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
		while (index < n && p[index].arrival <= current_time + t.length) pq.push(p[index++]);
		current_time += t.length;
		pc[t.p_id-1].termination = current_time;
		if (pq.size() == 0 && index < n) pq.push(p[index++]);
	}
	compute_metrics(pc, n, cpu_util_timestamp, throughput_timestamp, util_until);
}

void P(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp) {
	priority_queue<Process, vector<Process>, 
		function<bool(Process,Process)> > pq
		([](const Process &a, const Process &b) -> bool {
		if (a.priority == b.priority) return a.p_id > b.p_id;
		return a.priority > b.priority;
	});
	Process pc[n];
	for (int i = 0; i < n; i++) pc[i] = p[i];
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{ // sort it by arrival time, and then by priority
		if (a.arrival == b.arrival) {
			if (a.priority == b.priority) return a.p_id < b.p_id;
			else return a.priority < b.priority;
		}
		return a.arrival < b.arrival;
	});
	int index = 0, util_until = 0;
	int current_time = p[0].arrival;
	bool ok = 0; int cnt = 0;
	while (index < n && current_time == p[index].arrival) pq.push(p[index++]);
	while (!pq.empty()) { // pq will contain processes that are currently in the ready queue
		Process t = pq.top(); pq.pop(); ok = 0;
		current_time = max(current_time, t.arrival);
		if (pc[t.p_id-1].first_burst == -1) pc[t.p_id-1].first_burst = current_time;
		debug_text("--- CURRENT TIME: %d ---\n", current_time);
		debug_text("Process ID: %d || Priority: %d\n", t.p_id, t.priority);
		while (index < n) {
			if (current_time + t.length < p[index].arrival) { // this process will finish before next
				debug_text("CURRENT PROCESS ENDS BEFORE NEXT INDEX\n");
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
				current_time += t.length;
				pc[t.p_id-1].termination = current_time;
				ok = 1;
				break;
			}
			// if above isn't satisfied, then interception happens
			if (t.priority > p[index].priority) {
				debug_text("CURRENT PROCESS INTERRUPTED. PROCESS %d PUSHED\n", p[index].p_id);
				cout << current_time << ' ' << t.p_id << ' ' << p[index].arrival - current_time << endl;
				if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, p[index].arrival) - current_time;
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
			if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
			current_time += t.length;
			pc[t.p_id-1].termination = current_time;
		}
		if (pq.size() == 0 && index < n) {
			pq.push(p[index++]); // if empty but theres still some processes
		}
	}
	compute_metrics(pc, n, cpu_util_timestamp, throughput_timestamp, util_until);
}

// basically always do shortest remaining time
// thus you have to make sure its sorted by remaining time
void SRTF(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp) {
	priority_queue<Process, vector<Process>, 
		function<bool(Process,Process)> > pq
		([](const Process &a, const Process &b) -> bool {
		if (a.length == b.length) return a.p_id > b.p_id;
		return a.length > b.length;
	});
	Process pc[n];
	for (int i = 0; i < n; i++) pc[i] = p[i];
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{ // sort it by arrival time, and then by length
		if (a.arrival == b.arrival) {
			if (a.length == b.length) return a.p_id < b.p_id;
			else return a.length < b.length;
		}
		return a.arrival < b.arrival;
	});
	int index = 0, util_until;
	int current_time = p[0].arrival;
	bool ok = 0; int cnt = 0;
	while (index < n && current_time == p[index].arrival) pq.push(p[index++]);
	while (!pq.empty()) { // pq will contain processes that are currently in the ready queue
		ok = 0;
		Process t = pq.top(); pq.pop(); // current running process
		debug_text("--- CURRENT TIME: %d ---\n", t.p_id);
		debug_text("Process ID: %d || Length: %d\n", t.p_id, t.length);
		current_time = max(current_time, t.arrival);
		if (pc[t.p_id-1].first_burst == -1) pc[t.p_id-1].first_burst = current_time;
		while (index < n) {
			if (current_time + t.length < p[index].arrival) {
				debug_text("CURRENT PROCESS ENDS BEFORE NEXT INDEX\n");
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
				current_time += t.length;
				pc[t.p_id-1].termination = current_time;
				ok = 1;
				break;
			}
			// implies interception happens
			int tmp = current_time + t.length - p[index].arrival; 
			if (tmp > p[index].length) { // when the new process is better
				debug_text("CURRENT PROCESS INTERRUPTED -- PROCESS %d PUSHED\n", p[index].p_id);
				cout << current_time << ' ' << t.p_id << ' ' << p[index].arrival - current_time << endl;
				if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, p[index].arrival) - current_time;
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
			if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
			current_time += t.length;
			pc[t.p_id-1].termination = current_time;
		}
		if (pq.size() == 0 && index < n) {
			pq.push(p[index++]); // if empty but theres still some processes
		}
	}
	debug_text("FINAL TIME: %d\n", current_time);
	compute_metrics(pc, n, cpu_util_timestamp, throughput_timestamp, util_until);
}

void RR (Process p[], int n, int q, int cpu_util_timestamp, int throughput_timestamp) {
	Process pc[n];
	for (int i = 0; i < n; i++) pc[i] = p[i];
	sort(p, p+n, [](const Process &a, const Process &b) -> bool{
		if (a.arrival == b.arrival) return a.p_id < b.p_id;
		return a.arrival < b.arrival;
	});
	queue<Process> que;
	int index = 0, util_until = 0;
	int current_time = p[0].arrival;
	bool ok = 0; int cnt = 0;
	while (index < n && current_time == p[index].arrival) que.push(p[index++]);
	while (!que.empty()) {
		Process t = que.front(); que.pop();
		current_time = max(current_time, t.arrival);
		if (pc[t.p_id-1].first_burst == -1) pc[t.p_id-1].first_burst = current_time;
		debug_text("CURRENT_TIME: %d || LENGTH: %d\n", current_time, t.length);
		if (t.length <= q) {
			cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
			if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
			current_time += t.length;
			// while (index < n && p[index].arrival <= current_time) que.push(p[index++]);
			pc[t.p_id-1].termination = current_time;
		} else {
			if (index >= n && que.size() == 0) {
				cout << current_time << ' ' << t.p_id << ' ' << t.length << "X\n";
				if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + t.length) - current_time;
				current_time += t.length;
				pc[t.p_id-1].termination = current_time;
			} else {
				cout << current_time << ' ' << t.p_id << ' ' << q << "\n";
				if (cpu_util_timestamp >= current_time) util_until += min(cpu_util_timestamp, current_time + q) - current_time;
				current_time += q;
				while (index < n && p[index].arrival <= current_time) que.push(p[index++]);
				que.push({current_time, t.length-q, t.priority, t.p_id});
			}
		}
		while (index < n && p[index].arrival < current_time) que.push(p[index++]);
		if (que.size() == 0 && index < n) {
			que.push(p[index++]); // if empty but theres still some processes
		}
	}
	compute_metrics(pc, n, cpu_util_timestamp, throughput_timestamp, util_until);
}


int main () {
	int tc, ctc = 0; 
	cin >> tc;
	while (tc--) {
		int n, time_quantum, cpu_util_timestamp, throughput_timestamp; string process;
		cin >> n >> process; 
		time_quantum = -1;
		Process processes[n];
		if (process == "RR") cin >> time_quantum;
		cin >> cpu_util_timestamp >> throughput_timestamp;
		for (int i = 0; i < n; i++) {
			int a, b, c; cin >> a >> b >> c;
			processes[i] = {a,b,c,i+1};
		}
		
		cout << (++ctc) << endl;
		if (process == "FCFS") {
			FCFS(processes, n, cpu_util_timestamp, throughput_timestamp); // O(n lg n)
		} else if (process == "SJF") {
			SJF(processes, n, cpu_util_timestamp, throughput_timestamp); // O(n lg n)
		} else if (process == "SRTF") {
			SRTF(processes, n, cpu_util_timestamp, throughput_timestamp); // O(2n lg n)
		} else if (process == "P") {
			P(processes, n, cpu_util_timestamp, throughput_timestamp); // O(n lg n)
		} else if (process == "RR") {
			RR(processes, n, time_quantum, cpu_util_timestamp, throughput_timestamp); // O(n)
		}
	}
	
	return 0;
}