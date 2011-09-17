#ifndef INC_CLUSTERLIST_H
#define INC CLUSTERLIST_H
/// Class: ClusterList
/// This class holds all the individual clusters, as well as routines that
/// can be used to perform clustering (metric recalculation, cluster merging,
/// and so on). The distance calculation routines require that a
/// triangle matrix with distances between all frames be previously
/// calculated.
#include <list>
#include "TriangleMatrix.h"
#include "DataSet.h"
class ClusterList {
  public:
    enum LINKAGETYPE {SINGLELINK, AVERAGELINK, COMPLETELINK};

  private:
    int debug;
    // clusterNode: Store individual cluster info; frame numbers, centroid, etc.
    struct clusterNode {
      std::list<int> frameList; // List of frames in the cluster
      int num;                  // Cluster number (index in Distances)
    };
    std::list<clusterNode> clusters;

    int maxframes;                   // Total number of frames being clustered
    TriangleMatrix *FrameDistances;  // Distances between each frame
    TriangleMatrix ClusterDistances; // Distances between each cluster
    LINKAGETYPE Linkage;             // Type of distance calculation for clusters 

    // cluster_cmp: Used to sort the cluster list
    struct cluster_cmp {
      bool operator()(clusterNode first, clusterNode second) const {
        if (first.frameList.size() > second.frameList.size())
          return true;
        else
          return false;
      }
    };
    // Distance calculation routines
    void calcMinDist(std::list<ClusterList::clusterNode>::iterator);
    void calcMaxDist(std::list<ClusterList::clusterNode>::iterator);
    void calcAvgDist(std::list<ClusterList::clusterNode>::iterator);

  public :
    ClusterList();
    ~ClusterList();
  
    void SetLinkage(LINKAGETYPE Lin) { Linkage = Lin; }
    int Nclusters() { return (int) clusters.size(); }

    void Initialize(TriangleMatrix *);
    int AddCluster(std::list<int> *, int);
    void PrintClusters();
    std::list<clusterNode>::iterator GetClusterIt(int);
    int Merge(std::list<clusterNode>::iterator,std::list<clusterNode>::iterator);
    int MergeClosest(double);
    void Cnumvtime(DataSet *);
    void Renumber();
    void Summary(char *);
};
#endif
