/*

##############################################################################
#                                                                            #
#              FLUKE: Fields Layered Under Kohn-sham Electrons               #
#                             By: Eric G. Kratz                              #
#                                                                            #
##############################################################################

 FLUKE functions for manipulating multipoles.

*/

void ExtractTINKpoles(vector<QMMMAtom>& Struct)
{
  //Parses TINKER parameter files to find multipoles and local frames
  
  return;
};

void RotateCharges(vector<QMMMAtom>& Struct, int Bead)
{
  //Switches from the local frame of reference to the global frame
  //of reference
  
  return;
};

RedMpole Cart2SphHarm(Mpole& poles)
{
  //Converts cartesian multipoles to spherical harmonic multipoles
  RedMpole SHpoles; //Spherical harmonic multipoles

  return SHpoles;
};

OctCharges SphHarm2Charges(RedMpole poles)
{
  //Converts spherical harmonic multipoles to point charges
  OctCharges PCgrid; //New point charge multipoles

  return PCgrid;
};