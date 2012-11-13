#ifndef INC_FRAMELIST_H
#define INC_FRAMELIST_H
#include "TopologyList.h"
// Class: FrameList
/// Hold a series of Frame classes. 
/** Optionally hold a corresponding name and number (in the case of 
  * reference frames, this is the name of the reference coordinate 
  * file and the frame number). 
  */
// TODO: Eventually store a vector of Frames, not Frame*s
class FrameList : public FileList {
  public:
    FrameList();
    ~FrameList();

    Frame *ActiveReference();
    int AddReference(ArgList&, TopologyList&);
    void SetActiveRef(int);
    int AddFrame(Frame *, Topology *);
    Topology *GetFrameParm(int);
    Frame *GetFrame(int idx);
    int ReplaceFrame(int, Frame *, Topology *);
    void List();
    const char *FrameName(int);

    int NumFrames() { return (int)frames_.size(); }
  private:
    std::vector<Frame*> frames_;
    std::vector<Topology*> parms_;
    std::vector<int> nums_;
    std::vector<Topology*> StrippedRefParms_;
    int refFrameNum_;

};
#endif
