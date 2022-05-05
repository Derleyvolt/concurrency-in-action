#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

#define it vector<int>::iterator

void sum(it start, it end, int& result) {
	result += accumulate(start, end, 0);
}

int sum_threaded(vector<int> arr) {
	int len = distance(arr.begin(), arr.end());
	int num_thread = thread::hardware_concurrency(); // número de threads que podem verdadeiramente rodar em paralelo no SO
	int per_thread = len/num_thread;

	std::vector<std::thread> t_arr(num_thread);

	int result = 0;

	for(int i = 0; i < num_thread; i++) {
		if(i+1 < num_thread)
			t_arr[i] = thread(sum, arr.begin()+i*per_thread, arr.begin()+(i+1)*per_thread, ref(result));
		else
			t_arr[i] = thread(sum, arr.begin()+i*per_thread, arr.end(), ref(result));
	}

	for(auto& t : t_arr) t.join();
	return result;
}

int main() {
	vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int answ = sum_threaded(arr);
	cout << answ << endl;
	return 0;
}
