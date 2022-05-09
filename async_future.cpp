#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <sstream>
#include <fstream>
#include <future>

using namespace std;

queue<string> db;

#define ll long long

ll fib(ll n) {
	if(n == 1 || n == 2)
		return 1;
	return fib(n-1) + fib(n-2);
}

void read_db() {
	ifstream fp("dic.txt", ios::in);

	string s;

	while(fp >> s) {
		db.push(s);
	}
	
	fp.close();
	cout << "Arquivo carregado com sucesso" << endl;
}

void future_example() {
	// async dispara uma thread passando o pointer-to-function fib e o argumento 45
	// do mesmo modo como passamos em std::thread
	// o lance é que async retorna um objeto future que, em algum momento, vai ter o
	// valor de retorno da nossa função associado a ele..
	// enquanto isso podemos fazer alguma outra coisa e, quando quisermos nosso
	// valor de retorno apenas chamamos fu.get().. se o valor de retorno
	// ainda não estiver pronto a thread atual é bloqueada até que o future
	// esteja 'ready'.
	future<ll> fu = async(fib, 45);
	read_db();
	cout << "O numero de fibonacci eh: " << fu.get() << endl;
}

int main() {
	future_example();
	return 0;
}
