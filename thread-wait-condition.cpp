#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <sstream>
#include <fstream>

using namespace std;

// EXEMPLO USANDO O CLÁSSICO PROBLEMA DO PRODUTOR E CONSUMIDOR.

queue<string> q;

std::mutex m;
condition_variable data_cond;

queue<string> db;

void process_queue() {
	for(;;) {
	    // unique_lock difere do lock_guard apenas no fato de que podemos dar unlock no unique_lock explicitamente
	    // e não apenas quando estivermos saindo do escopo do lock.
		unique_lock<mutex> lg(m);
		// o mutex é unlocked enquanto ela tá bloqueada
		// e a notificação só libera esse wait se pred for true	
		// ter cuidado com spurious wakeup.. é bom que a função pred
		// não tenha side-effects, justamente por conta de como o wait é
		// implementado
		// https://en.wikipedia.org/wiki/Spurious_wakeup
		data_cond.wait(lg, []() { return !q.empty(); } );
		cout << q.front() << endl;
		q.pop();
		lg.unlock();
		this_thread::sleep_for(chrono::milliseconds(500));
	}
}

void fill_queue() {
	while(1) {
		this_thread::sleep_for(chrono::milliseconds(100));
		lock_guard<mutex> lg(m);
		q.push(db.front());	
		db.pop();
		// unlock apenas uma thread
		data_cond.notify_one(); // 2: notify_all()
	}
}

void read_db() {
	ifstream fp("dic.txt", ios::in);

	string s;

	while(fp >> s) {
		db.push(s);
	}
	
	fp.close();
}

int main() {
	read_db();

	thread producer(fill_queue);
	vector<thread> consumers(5);

	for(int i = 0; i < 5; i++) {
		consumers[i] = thread(process_queue);
	}

	producer.join();
	
	for(auto& t : consumers) {
		t.join();
	}
	
	return 0;
}
