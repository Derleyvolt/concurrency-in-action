#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

// duration<tipo, tick do relógio>
// tipo                      = double se o tick for fracionário
// tick do relógio           = unidade de tempo medido em segundos
// tick.first / tick.second  = segundos necessários pra um tick do relógio 

template<typename T = duration<int, std::ratio<1, 1>>>
class Timer {
public:
    // é possível modificar duration ou
    // usar os typedefs STL 
    Timer(T arg) : time_lapse(arg) {
        tp = steady_clock::now();
    }
	
    // construtor padrão pra utilização em segundos
	Timer(int sec) : time_lapse(seconds(sec)) {
	    tp = steady_clock::now();
	}

    bool ready() const {
        auto stop = steady_clock::now();
        return duration_cast<T>(stop-tp).count() >= time_lapse.count();
    }

private:
    time_point<steady_clock> tp;
    T time_lapse;
};

int main() {
    Timer timer(5); 
    // Timer timer(seconds(5))
    // Timer timer(milliseconds(5))

    while(not timer.ready()) {
        cout << "Esperando.." << endl;
        this_thread::sleep_for(seconds(1));
    }
    return 0;
}
