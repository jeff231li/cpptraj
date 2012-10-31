#ifndef INC_TRAJ_CHARMMDCD_H
#define INC_TRAJ_CHARMMDCD_H
#include "TrajectoryIO.h"
// Class: Traj_CharmmDcd
/// TrajectoryIO class for reading coordinates from charmm dcd files.
class Traj_CharmmDcd : public TrajectoryIO {
  public :
    Traj_CharmmDcd();
    static TrajectoryIO* Alloc() { return (TrajectoryIO*)new Traj_CharmmDcd(); }
    ~Traj_CharmmDcd();
    // charmm dcd-specific functions
  private:
    int dcdatom;
    int dcdframes;
    int dcdoutsize;
    int dcdheadersize;
    bool isBigEndian;
    bool is64bit;
    unsigned int readSize;
    //bool dcdExtraBlock;
    bool dcd4D;
    int istart;
    int nsavc;
    int namnf;
    int nfreat;
    int *freeat;
    float timestep;
    float *xcoord;
    float *ycoord;
    float *zcoord;

    union headerbyte { unsigned char c[80]; int i[20]; float f[20]; };
    int ReadBlock(int);
    int WriteBlock(int);
    int readDcdHeader();
    int writeDcdHeader();

    // Inherited functions
    bool ID_TrajFormat(CpptrajFile&);
    int setupTrajin(std::string const&, Topology*, TrajInfo&);
    int setupTrajout(std::string const&, Topology*, int, TrajInfo const&,bool);
    int openTraj();
    void closeTraj();
    int readFrame(int,double*,double*,double*,double*);
    int writeFrame(int,double*,double*,double*,double);
    void info();
    int processWriteArgs(ArgList&);

    int readVelocity(int, double*) { return 1; }
    int readIndices(int,int*) { return 1; }
    int processReadArgs(ArgList&) { return 0; }
};
#endif
