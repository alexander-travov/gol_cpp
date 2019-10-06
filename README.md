Conway's Game of Life
=====================

![Pulsar](https://github.com/alexander-travov/gol_cpp/raw/master/pulsar.gif "Pulsar")

This is C++ implementation of the [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

You need at least C++11 compiler.


Building
--------

``` shell
make
./gol
```


Examples
--------

1. Watch evolution from random state:

``` c++
auto field = Field(70, 30);
field.randomize(0.2);

for (;;) {
    cout << field << endl;
    field.update();
    this_thread::sleep_for(chrono::milliseconds(100));
}
```

2. Count repeat period of Pulsar pattern:

``` c++
auto pulsar = Field(20, 20);
pulsar.set_pattern(PULSAR);
auto pulsar_start = pulsar;
int epoch = 0;
do {
    pulsar.update();
    epoch++;
} while (pulsar != pulsar_start);
cout << "Pulsar repeats in " << epoch << " epochs." << endl;
```

3. Meditate on Gosper Glider Gun:

``` c++
auto field = Field(70, 30);
field.set_pattern(GOSPER_GLIDER_GUN);

for (;;) {
    cout << field << endl;
    field.update();
    this_thread::sleep_for(chrono::milliseconds(100));
}
```
