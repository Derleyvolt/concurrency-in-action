#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

using namespace std;

std::queue<string> q_str;

template<class T>
class queue_threadsafe {
public:
	void push(T v) {
		lock_guard<mutex> lg(mu);
		q.push(v);
		cond.notify_one();
	}

	void wait_and_pop(T& v) {
		unique_lock<mutex> ul(mu);
		cond.wait(ul, [this]() { return !q.empty(); });
		v = q.front();
		q.pop();
		ul.unlock();
	}

	bool empty() {
		lock_guard<mutex> lg(mu);
		return q.empty();
	}

	int size() {
		lock_guard<mutex> lg(mu);
		return q.size();
	}

private:
	mutex mu;
	queue<T> q;
	condition_variable cond;
};

queue_threadsafe<string> q;

void consumer() {
	for(;;) {
		string ret;
		q.wait_and_pop(ret);
		cout << this_thread::get_id() << "   " << ret << endl;
	}
}

void producer() {
	while(!q_str.empty()) {
		string str = q_str.front();
		q_str.pop();
		q.push(str);
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

void read_file() {
	ifstream fp("dic.txt", ios::in);

	string s;

	while(fp >> s) {
		q_str.push(s);
	}

	fp.close();
}

int main() {
	read_file();
	vector<thread> thread_arr(5);
	thread tp(producer);
	for(auto& a : thread_arr) a = thread(consumer);
	for(auto& a : thread_arr) a.join();
	tp.join();
	return 0;
}
