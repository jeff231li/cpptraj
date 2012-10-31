#include "TrajectoryFile.h"
#include "CpptrajStdio.h"
// All TrajectoryIO classes go here
#include "Traj_AmberCoord.h"
#ifdef BINTRAJ
  #include "Traj_AmberNetcdf.h"
  #include "Traj_AmberRestartNC.h"
#endif
#include "Traj_PDBfile.h"
#include "Traj_AmberRestart.h"
#include "Traj_Mol2File.h"
#include "Traj_Conflib.h"
#include "Traj_CharmmDcd.h"
#include "Traj_Binpos.h"

// ----- STATIC VARS / ROUTINES ------------------------------------------------ 
const TrajectoryFile::TrajToken TrajectoryFile::TrajArray[] = {
#ifdef BINTRAJ
  { AMBERNETCDF,    "netcdf",    "Amber NetCDF",     ".nc",      Traj_AmberNetcdf::Alloc },
  { AMBERRESTARTNC, "ncrestart", "Amber NC Restart", ".ncrst",   Traj_AmberRestartNC::Alloc },
  { AMBERRESTARTNC, "restartnc", "Amber NC Restart", ".ncrst",   Traj_AmberRestartNC::Alloc },
#endif
  { PDBFILE,        "pdb",       "PDB",              ".pdb",     Traj_PDBfile::Alloc },
  { MOL2FILE,       "mol2",      "Mol2",             ".mol2",    Traj_Mol2File::Alloc },
  { CHARMMDCD,      "dcd",       "Charmm DCD",       ".dcd",     Traj_CharmmDcd::Alloc },
  { CHARMMDCD,      "charmm",    "Charmm DCD",       ".dcd",     Traj_CharmmDcd::Alloc },
  { BINPOS,         "binpos",    "BINPOS",           ".binpos",  Traj_Binpos::Alloc },
  { AMBERRESTART,   "restart",   "Amber Restart",    ".rst7",    Traj_AmberRestart::Alloc },
  { AMBERTRAJ,      "crd",       "Amber Trajectory", ".crd",     Traj_AmberCoord::Alloc },
  { CONFLIB,        "conflib",   "LMOD conflib",     ".conflib", Traj_Conflib::Alloc },
  { UNKNOWN_TRAJ,   0,           "Unknown" ,         0,          0                   }
};

// TrajectoryFile::GetFormatFromArg()
/** Given an arglist, search for one of the file format keywords.
  * Default to AmberTraj if no arglist given or no keywords present. 
  */
TrajectoryFile::TrajFormatType TrajectoryFile::GetFormatFromArg(ArgList& argIn)
{
  for (TokenPtr token = TrajArray; token->Type != UNKNOWN_TRAJ; ++token)
    if (argIn.hasKey( token->Key )) return token->Type;
  return AMBERTRAJ;
}

// TrajectoryFile::GetExtensionForType()
std::string TrajectoryFile::GetExtensionForType(TrajFormatType typeIn) {
  for (TokenPtr token = TrajArray; token->Type != UNKNOWN_TRAJ; ++token)
    if ( token->Type == typeIn )
      return std::string( token->Extension );
  return std::string();
}

// TrajectoryFile::GetTypeFromExtension()
TrajectoryFile::TrajFormatType TrajectoryFile::GetTypeFromExtension( std::string const& extIn)
{
  for (TokenPtr token = TrajArray; token->Type != UNKNOWN_TRAJ; ++token)
    if ( extIn.compare( token->Extension ) == 0 ) return token->Type;
  return UNKNOWN_TRAJ;
}

// TrajectoryFile::FormatString()
const char* TrajectoryFile::FormatString( TrajectoryFile::TrajFormatType tIn ) {
  TokenPtr token;
  for (token = TrajArray; token->Type != UNKNOWN_TRAJ; ++token)
    if ( token->Type == tIn ) return token->Description; 
  return token->Description; // Should be at UNKNOWN
}
// -----------------------------------------------------------------------------

// CONSTRUCTOR
TrajectoryFile::TrajectoryFile() :
  debug_(0),
  trajParm_(0)
{}

// TrajectoryFile::SetDebug()
/** Set debug level. */
void TrajectoryFile::SetDebug(int debugIn) {
  debug_ = debugIn;
  if (debug_>0) mprintf("\tTrajectoryFile debug level set to %i\n",debug_);
}

// TrajectoryFile::SetFileName()
void TrajectoryFile::SetFileName(std::string const& full) {
  trajName_.SetFileName( full.c_str() );
} 

int TrajectoryFile::SetTrajParm( Topology* tparmIn ) {
  if (tparmIn==0) {
    mprinterr("Error: TrajectoryFile: Parm file is NULL.\n");
    return 1;
  }
  trajParm_ = tparmIn;
  return 0;
}

// TrajectoryFile::AllocTrajIO()
/** \param tformat Trajectory format to set up for.
  * \return TrajectoryIO class based on tformat. 0 on error.
  */
TrajectoryIO* TrajectoryFile::AllocTrajIO(TrajFormatType tformat) {
  for (TokenPtr token = TrajArray; token->Type != UNKNOWN_TRAJ; ++token)
    if (token->Type == tformat) return (TrajectoryIO*)token->Alloc;
  return 0;
}
// TrajectoryFile::DetectFormat()
TrajectoryIO* TrajectoryFile::DetectFormat(CpptrajFile& fileIn) {
  // Assume fileIn set up for reading
  for (TokenPtr token = TrajArray; token->Type != UNKNOWN_TRAJ; ++token) {
    TrajectoryIO* trajio = (TrajectoryIO*)token->Alloc;
    // TODO: Set debug
    if ( trajio->ID_TrajFormat( fileIn ) ) return trajio;
    delete trajio;
  }
  return 0;
}

