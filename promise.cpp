#include <iostream>
#include <mutex>
#include <thread>
#include <future>

using namespace std;

int teste_func(future<int>& arg) {
	int aux = arg.get();
	// TODO..
	return aux;
}

void promisse_example() {
	promise<int> p;
	future<int> fu = p.get_future();
	future<int> ret = async(launch::async, teste_func, ref(fu));

	for(int i = 0; i < 10; i++) {
		cout << i << endl;
	}

	p.set_value(30);
	cout << "Primise: " << ret.get() << endl;
}

int main() {
	promisse_example();
	return 0;
}
