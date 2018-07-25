# Real Time Highway simulator
Simulate autonomous vehicles behaviour in a highway, using pthreads and Qt as graphic library

The user can start a simulation of a highway's portion chosing the vehicles to put on the road:
- Truck [max speed: 60 km/h, can_overtake: no]
- Motorcycle [max speed: 80 km/h, can_overtake: yes]
- Car [max speed: 130 km/h, can_overtake: yes]

It is possibile to start with standard options:
- 50 vehicles [15 Trucks, 25 motorcycles, 10 cars]
- Vehicle tracked: 23
- Horizontal moviment simulation frequency: rare
Otherwise the user can make a custom choice, choosing the presence percentage of each vehicle type

(The order is alway truck, then motorcycles and then cars, to involve more overtaking)

# Dependencies
- Qt (greater than version 4)
- qmake

# Building
```
cd graphic-highway
mkdir build
cd build
qmake ../graphic-highway.pro
make

```

# Run from terminal to let it be verbose
```
./graphic-highway

```


