#ifndef INC_TRAJ_CONFLIB_H
#define INC_TRAJ_CONFLIB_H
#include "TrajectoryIO.h"
/// Class: Conflib
/// Test TrajectoryIO object for reading conflib file generated by NAB LMOD
class Conflib: public TrajectoryIO {
    double energy;
    double radGyr;
    int timesFound;
    int conflibAtom;

    // Inherited functions
    int setupRead(int);
    int setupWrite(int);
    int openTraj();
    void closeTraj();
    int readFrame(int,double*,double*,double*,double*);
    int writeFrame(int,double*,double*,double*,double);
    void info();

  public:

    Conflib();
    ~Conflib();
};
#endif
