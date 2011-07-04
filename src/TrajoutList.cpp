// TrajoutList
#include <cstddef> // NULL
#include "TrajoutList.h"
#include "CpptrajStdio.h"
//#include "PtrajMpi.h" //worldsize

// CONSTRUCTOR
TrajoutList::TrajoutList() {
  fileAccess=WRITE;
}

// DESTRUCTOR
TrajoutList::~TrajoutList() { }

/* TrajoutList::Add()
 * Add trajectory to the trajectory list as an output trajectory. 
 * Associate the trajectory with one of the parm files in the 
 * ParmFileList. 
 * trajout <filename> <fileformat> [append] [nobox] [parm <parmfile> | parmindex <#>]
 *         [<range>]
 */
int TrajoutList::Add(char *filenameIn, ArgList *A, AmberParm *parmIn) {
  TrajectoryFile *traj;
  char *filename;

  // Since we need to check if this filename is in use in order to prevent
  // overwrites, determine the filename here.
  if (filenameIn==NULL)
    filename = A->getNextString();
  else
    filename = filenameIn;
  if (filename==NULL) {
    mprinterr("Error: TrajoutList::Add: Called with NULL filename.\n");
    return 1;
  }
  if (FilenameInUse(filename)) {
    mprinterr("Error: trajout: Filename %s already in use.\n",filename);
    return 1;
  }

  traj = new TrajectoryFile();
  if (traj==NULL) {
    mprinterr("Error: TrajoutList::Add: Could not allocate memory for traj.\n");
    return 1;
  }
  traj->SetDebug(debug);
  // Default to AMBERTRAJ; format can be changed via args in the arg list
  if (traj->SetupWrite(filename,A,parmIn,AMBERTRAJ)) {
    mprinterr("Error: trajout: Could not set up trajectory.\n");
    delete traj;
    return 1;
  }

  // Add to trajectory file list
  this->push_back(traj); 

  return 0;
}

/* TrajoutList::Write()
 * Go through each output traj, call write. The first time CurrentParm
 * matches the parm the trajectory was originally set up with it will
 * be opened, no need to call BeginTraj.
 */ 
int TrajoutList::Write(int set, AmberParm *CurrentParm, Frame *F) { 
  std::list<TrajectoryFile*>::iterator traj;

  for (traj = this->begin(); traj != this->end(); traj++) {
    if ( (*traj)->WriteFrame(set, CurrentParm, F->X, F->V, F->box, F->T) ) {
      mprinterr("Error writing output trajectory.\n");
      return 1;
    }
  }

  return 0;
}

/* TrajoutList::Close()
 * Close output trajectories. Called after input traj processing completed.
 */
void TrajoutList::Close() {
  std::list<TrajectoryFile*>::iterator traj;

  for (traj = this->begin(); traj != this->end(); traj++)
    (*traj)->EndTraj();
}

