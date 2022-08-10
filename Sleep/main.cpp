#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

void sleep(double t)
{
    static double  estimate = 5e-3;
    static double  mean     = 5e-3;
    static double  deltas   = 0;
    static int64_t count    = 1;

    constexpr double unit = 1e9;    // 1 s = 1e9 ns

    while (t > estimate) {
        auto start = high_resolution_clock::now();
        this_thread::sleep_for(milliseconds(1));
        auto end = high_resolution_clock::now();

        double observed = static_cast<double>((end - start).count()) / unit;
        t               -= observed;

        ++count;
        double delta     = observed - mean;
        mean             += delta / static_cast<double>(count);
        deltas           += delta * delta;
        double deviation = sqrt(deltas / static_cast<double>((count - 1)));
        estimate         = mean + deviation;
    }

    auto start = high_resolution_clock::now();
    while (static_cast<double>((high_resolution_clock::now() - start).count()) / unit < t) {}
}

int main()
{
    sleep(0.001);    // sleep 1ms
    return 0;
}
