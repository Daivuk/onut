#!/bin/sh

# kill processes if they are already running
pkill JSStandAlone
pkill PILaunch

# browse to onut
cd ~/github/onut

# make sure we have latest
git fetch origin
git rebase origin/master

# build the stand alone
cmake -DCMAKE_BUILD_TYPE=Release .
make JSStandAlone -j5

# build the pi launch server
cd PILaunch
cmake -DCMAKE_BUILD_TYPE=Release .
make -j5

# run the server
./bin/linux/PILaunch server ~/onut_run 3333
