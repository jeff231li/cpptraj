#ifndef INC_TRAJ_CONFLIB_H
#define INC_TRAJ_CONFLIB_H
#include "TrajectoryIO.h"
// Class: Traj_Conflib
/// Test TrajectoryIO object for reading conflib file generated by NAB LMOD
class Traj_Conflib: public TrajectoryIO {
  public:

    Traj_Conflib();
    static TrajectoryIO* Alloc() { return (TrajectoryIO*)new Traj_Conflib(); }
  private:
    double energy_;
    double radGyr_;
    int timesFound_;
    int conflibAtom_;

    // Inherited functions
    bool ID_TrajFormat(CpptrajFile&);
    int setupTrajin(std::string const&, Topology*, TrajInfo&);
    int setupTrajout(std::string const&, Topology*, int, TrajInfo const&,bool);
    int openTraj();
    void closeTraj();
    int readFrame(int,double*,double*,double*,double*);
    int writeFrame(int,double*,double*,double*,double);
    void info();
    int readVelocity(int, double*) { return 1; }
    int readIndices(int,int*) { return 1; }
    int processWriteArgs(ArgList&) { return 0; }
    int processReadArgs(ArgList&) { return 0; }
};
#endif
