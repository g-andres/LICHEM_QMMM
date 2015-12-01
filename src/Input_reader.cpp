/*

###############################################################################
#                                                                             #
#                 LICHEM: Layered Interacting CHEmical Models                 #
#                              By: Eric G. Kratz                              #
#                                                                             #
#                      Symbiotic Computational Chemistry                      #
#                                                                             #
###############################################################################

 Routines for reading and checking the input for LICHEM.

*/

//Various input and error checking functions
void ReadArgs(int& argc, char**& argv, fstream& xyzfile,
              fstream& connectfile, fstream& regionfile, fstream& outfile)
{
  //Function to read arguments
  string dummy; //Generic string
  stringstream call; //Stream for system calls and reading/writing files
  //Read command line arguments
  if (argc == 1)
  {
    //Escape if there are no arguments
    cout << '\n';
    cout << "Missing arguments..." << '\n' << '\n';
    cout << "Usage: lichem -n Ncpus -x Input.xyz -c Connectivity.inp ";
    cout << "-r Regions.inp -o Output.xyz" << '\n';
    cout << '\n';
    cout << "Use -h or --help for detailed instructions.";
    cout << '\n' << '\n';
    cout.flush();
    exit(0);
  }
  dummy = string(argv[1]);
  if (dummy == "-GauExtern")
  {
    //Escape to GauExternal
    ExternalGaussian(argc,argv);
  }
  if (dummy == "-convert")
  {
    //Attempt to create LICHEM input from other formats
    dummy = string(argv[2]);
    if (dummy == "-t")
    {
      //Create LICHEM input from Gaussian input
      TINK2LICHEM(argc,argv);
    }
    if (dummy == "-b")
    {
      //Create BASIS files
      LICHEM2BASIS(argc,argv);
    }
    else
    {
      //Bad arguments
      cout << '\n';
      cout << "Unrecognized file format.";
      cout << '\n';
      cout << '\n';
      cout.flush();
    }
  }
  if (dummy == "-tinker")
  {
    //Attempt to create a TINKER XYZ file from LICHEM input
    LICHEM2TINK(argc,argv);
  }
  if (dummy == "-GlobalPoles")
  {
    //Print multipole moments in the global frame of reference
    ExtractGlobalPoles(argc,argv);
  }
  if ((argc % 2) != 1)
  {
    //Check for help or missing arguments
    dummy = string(argv[1]);
    if ((dummy != "-h") and (dummy != "--help"))
    {
      //Escape if there are missing arguments
      cout << '\n';
      cout << "Odd number of arguments..." << '\n' << '\n';
      cout << "Usage: lichem -n Ncpus -x Input.xyz -c Connectivity.inp ";
      cout << "-r Regions.inp -o Output.xyz" << '\n';
      cout << '\n';
      cout << "Use -h or --help for detailed instructions.";
      cout << '\n' << '\n';
      cout.flush();
      exit(0);
    }
  }
  for (int i=0;i<argc;i++)
  {
    //Read file names and CPUs
    dummy = string(argv[i]);
    if ((dummy == "-h") or (dummy == "--help"))
    {
      //Print helpful information and exit
      cout << '\n';
      cout << "Usage: lichem -n Ncpus -x Input.xyz -c Connectivity.inp ";
      cout << "-r Regions.inp -o Output.xyz" << '\n';
      cout << '\n';
      cout << "Command line arguments:" << '\n' << '\n';
      cout << "  -n    Number of CPUs used for the QM calculation." << '\n';
      cout << '\n';
      cout << "  -x    Input xyz file." << '\n' << '\n';
      cout << "  -c    Connectivity and force field input file." << '\n';
      cout << '\n';
      cout << "  -r    Information about how the system is subdivided" << '\n';
      cout << "        into QM, MM, and psuedo-atom regions." << '\n' << '\n';
      cout << "  -o    Output xyz file for the optimized structures.";
      cout << '\n' << '\n';
      cout.flush();
      exit(0);
    }
    if (dummy == "-n")
    {
      //Read the number of CPUs
      Ncpus = atoi(argv[i+1]);
    }
    if (dummy == "-x")
    {
      //Read the XYZ filename
      xyzfilename = string(argv[i+1]);
      xyzfile.open(argv[i+1],ios_base::in);
    }
    if (dummy == "-c")
    {
      //Read the connectivity filename
      confilename = string(argv[i+1]);
      connectfile.open(argv[i+1],ios_base::in);
    }
    if (dummy == "-r")
    {
      //Read the regions filename
      regfilename = string(argv[i+1]);
      regionfile.open(argv[i+1],ios_base::in);
    }
    if (dummy == "-o")
    {
      //Read the output XYZ filename
      outfile.open(argv[i+1],ios_base::out);
    }
  }
  for (int i=0;i<argc;i++)
  {
    //Detect bad arguments
    dummy = string(argv[i]);
    if (dummy[0] == '-')
    {
      bool BadArgs = 0; //Bad argument found
      if ((dummy != "-n") and (dummy != "-x") and
      (dummy != "-c") and (dummy != "-r") and
      (dummy != "-o"))
      {
        BadArgs = 1;
      }
      if (BadArgs)
      {
        cout << '\n';
        cout << "Unrecognized flag..." << '\n' << '\n';
        cout << "Usage: lichem -n Ncpus -x Input.xyz -c Connectivity.inp ";
        cout << "-r Regions.inp -o Output.xyz" << '\n';
        cout << '\n';
        cout << "Use -h or --help for detailed instructions.";
        cout << '\n' << '\n';
        cout.flush();
        exit(0);
      }
    }
  }
  if (argc != 11)
  {
    //Escape if there are too few arguments
    cout << '\n';
    cout << "Missing arguments..." << '\n' << '\n';
    cout << "Usage: lichem -n Ncpus -x Input.xyz -c Connectivity.inp ";
    cout << "-r Regions.inp -o Output.xyz" << '\n';
    cout << '\n';
    cout << "Use -h or --help for detailed instructions.";
    cout << '\n' << '\n';
    cout.flush();
    exit(0);
  }
  //Make sure input files can be read
  bool DoQuit = 0;
  if (!xyzfile.good())
  {
    //Coordinate file does not exist
    cout << "Error: Could not open xyz file.";
    cout << '\n';
    DoQuit = 1;
  }
  if (!connectfile.good())
  {
    //Connectivity file does not exist
    cout << "Error: Could not open connectivity file.";
    cout << '\n';
    DoQuit = 1;
  }
  if (!regionfile.good())
  {
    //Regions file does not exist
    cout << "Error: Could not open region file.";
    cout << '\n';
    DoQuit = 1;
  }
  if (!outfile.good())
  {
    //No write permissions
    cout << "Error: Could not create output file.";
    cout << '\n';
    DoQuit = 1;
  }
  if (DoQuit)
  {
    //Quit with an error
    cout.flush(); //Print errors
    exit(0);
  }
  return;
};

void InitializeVariables(QMMMSettings& QMMMOpts)
{
  //Initialize all variables at once
  //QM wrapper settings
  QMMMOpts.Func = "N/A";
  QMMMOpts.Basis = "N/A";
  QMMMOpts.RAM = "N/A";
  QMMMOpts.MemMB = 0;
  QMMMOpts.Charge = "N/A";
  QMMMOpts.Spin = "N/A";
  QMMMOpts.BackDir = "N/A";
  //MC, MD, and RP settings
  QMMMOpts.Ensemble = "N/A";
  QMMMOpts.Temp = 0;
  QMMMOpts.Beta = 0;
  QMMMOpts.Press = 0;
  QMMMOpts.Neq = 0;
  QMMMOpts.Nsteps = 0;
  QMMMOpts.Nbeads = 1; //Key for printing
  QMMMOpts.accratio = 0;
  QMMMOpts.Nprint = 0;
  QMMMOpts.dt = 0;
  QMMMOpts.tautemp = 0;
  //OPtimization settings
  QMMMOpts.MaxOptSteps = 0;
  QMMMOpts.MMOptTol = 0;
  QMMMOpts.QMOptTol = 0;
  QMMMOpts.StepScale = 0;
  QMMMOpts.MaxStep = 0;
  //Additional RP settings
  QMMMOpts.Kspring = 0;
  QMMMOpts.TSBead = 0;
  QMMMOpts.Climb = 0;
  QMMMOpts.FrznEnds = 0;
  //Temporary energy storage
  QMMMOpts.Eold = 0;
  QMMMOpts.Ereact = 0;
  QMMMOpts.Eprod = 0;
  QMMMOpts.Ets = 0;
  return;
};

void ReadLICHEMInput(fstream& xyzfile, fstream& connectfile,
                     fstream& regionfile, vector<QMMMAtom>& Struct,
                     QMMMSettings& QMMMOpts)
{
  //Read input
  string dummy; //Generic string
  if (!GauExternal)
  {
    xyzfile >> Natoms;
    for (int i=0;i<Natoms;i++)
    {
      //Save atom information
      QMMMAtom tmp;
      //Set coordinates
      xyzfile >> tmp.QMTyp;
      Coord tmp2;
      xyzfile >> tmp2.x >> tmp2.y >> tmp2.z;
      tmp.P.push_back(tmp2); //Set up zeroth replica
      //Set ID and regions
      tmp.id = i;
      tmp.QMregion = 0;
      tmp.MMregion = 1;
      tmp.PBregion = 0;
      tmp.BAregion = 0;
      tmp.Frozen = 0;
      //Set electrostatic field
      Mpole tmp3; //Initialize charges and multipoles
      OctCharges tmp4; //Initialize charges and multipoles
      //Add to arrays
      tmp.MP.push_back(tmp3);
      tmp.PC.push_back(tmp4);
      //Save atomic properties
      Struct.push_back(tmp);
    }
  }
  for (int i=0;i<Natoms;i++)
  {
    //Save connectivity information
    int tmp;
    //id MMTyp NumTyp q Nbonds [connectivity]
    connectfile >> tmp; //Atom ID
    if (tmp != Struct[i].id)
    {
      //Escape if connectivity errors are found
      cout << "Error: Atoms in the connectivity file are out of order.";
      cout << '\n';
      cout.flush();
      exit(0); //Escape
    }
    connectfile >> Struct[i].MMTyp >> Struct[i].NumTyp;
    connectfile >> Struct[i].m >> Struct[i].MP[0].q;
    connectfile >> tmp; //Number of bonds
    for (int j=0;j<tmp;j++)
    {
      //Save each bond to the atom's connectivity list
      int AtomID;
      connectfile >> AtomID;
      if (AtomID >= Natoms)
      {
        //Search for more connectivity errors
        cout << "Error: Atom index out of range in connectivity.";
        cout << '\n';
        cout << "Atom " << i << " bonded to non-existant atom ";
        cout << AtomID << '\n';
        cout.flush();
        exit(0); //Escape
      }
      Struct[i].Bonds.push_back(AtomID); //Add bond
    }
  }
  //Collect misc. simulation options
  regionfile >> dummy >> dummy; //Potential type
  if ((dummy == "QM") or (dummy == "qm"))
  {
    //Read QMonly options
    QMonly = 1;
    MMonly = 0;
    QMMM = 0;
    Nqm = Natoms; //Save number of QM atoms
    regionfile >> dummy >> dummy;
    if ((dummy == "psi4") or (dummy == "Psi4")
       or (dummy == "PSI4"))
    {
      PSI4 = 1;
    }
    if ((dummy == "NWChem") or (dummy == "nwchem")
       or (dummy == "NWCHEM") or (dummy == "NWchem"))
    {
      NWChem = 1;
    }
    if ((dummy == "gaussian") or (dummy == "Gaussian") or (dummy == "g09"))
    {
      Gaussian = 1;
    }
    regionfile >> dummy >> QMMMOpts.Func;
    if ((QMMMOpts.Func == "SemiEmpirical") or (QMMMOpts.Func == "SE-SCF") or
       (QMMMOpts.Func == "Semi-Empirical") or (QMMMOpts.Func == "se-scf") or
       (QMMMOpts.Func == "semi-empirical") or (QMMMOpts.Func == "SESCF") or
       (QMMMOpts.Func == "semiempirical") or (QMMMOpts.Func == "sescf") or
       (QMMMOpts.Func == "SemiEmp") or (QMMMOpts.Func == "semiemp"))
    {
      //Flag the method as a semi-empirical Hamiltonian
      QMMMOpts.Func = "SemiEmp";
    }
    regionfile >> dummy >> QMMMOpts.Basis;
    regionfile >> dummy >> QMMMOpts.RAM;
    regionfile >> dummy;
    if ((dummy == "mb") or (dummy == "MB") or
       (dummy == "Mb") or (dummy == "mB"))
    {
      QMMMOpts.MemMB = 1;
    }
    else
    {
      QMMMOpts.MemMB = 0;
    }
    regionfile >> dummy >> QMMMOpts.Charge;
    regionfile >> dummy >> QMMMOpts.Spin;
    //Place all atoms in the QM region
    for (int i=0;i<Natoms;i++)
    {
      Struct[i].QMregion = 1;
      Struct[i].MMregion = 0;
      Struct[i].PBregion = 0;
      Struct[i].BAregion = 0;
    }
  }
  if ((dummy == "QMMM") or (dummy == "qmmm"))
  {
    //Read QMMM options
    QMMM = 1;
    QMonly = 0;
    MMonly = 0;
    regionfile >> dummy >> dummy;
    if ((dummy == "psi4") or (dummy == "Psi4")
       or (dummy == "PSI4"))
    {
      PSI4 = 1;
    }
    if ((dummy == "NWChem") or (dummy == "nwchem")
       or (dummy == "NWCHEM") or (dummy == "NWchem"))
    {
      NWChem = 1;
    }
    if ((dummy == "gaussian") or (dummy == "Gaussian"))
    {
      Gaussian = 1;
    }
    regionfile >> dummy >> QMMMOpts.Func;
    if ((QMMMOpts.Func == "SemiEmpirical") or (QMMMOpts.Func == "SE-SCF") or
       (QMMMOpts.Func == "Semi-Empirical") or (QMMMOpts.Func == "se-scf") or
       (QMMMOpts.Func == "semi-empirical") or (QMMMOpts.Func == "SESCF") or
       (QMMMOpts.Func == "semiempirical") or (QMMMOpts.Func == "sescf") or
       (QMMMOpts.Func == "SemiEmp") or (QMMMOpts.Func == "semiemp"))
    {
      //Flag the method as a semi-empirical Hamiltonian
      QMMMOpts.Func = "SemiEmp";
    }
    regionfile >> dummy >> QMMMOpts.Basis;
    regionfile >> dummy >> QMMMOpts.RAM;
    regionfile >> dummy;
    if ((dummy == "mb") or (dummy == "MB") or
       (dummy == "Mb") or (dummy == "mB"))
    {
      QMMMOpts.MemMB = 1;
    }
    else
    {
      QMMMOpts.MemMB = 0;
    }
    regionfile >> dummy >> QMMMOpts.Charge;
    regionfile >> dummy >> QMMMOpts.Spin;
    regionfile >> dummy >> dummy; //MM wrapper
    if ((dummy == "Tinker") or (dummy == "TINKER")
       or (dummy == "tinker"))
    {
      TINKER = 1;
    }
    if ((dummy == "AMBER") or (dummy == "Amber") or
       (dummy == "amber"))
    {
      AMBER = 1;
    }
    if ((dummy == "LAMMPS") or (dummy == "lammps") or
       (dummy == "Lammps"))
    {
      LAMMPS = 1;
    }
    regionfile >> dummy >> dummy; //Potential type
    if ((dummy == "AMOEBA") or (dummy == "amoeba"))
    {
      //AMOEBA polarizable force field
      AMOEBA = 1;
      if (TINKER)
      {
        ExtractTINKpoles(Struct,0);
      }
    }
    if ((dummy == "Charges") or (dummy == "charges") or
       (dummy == "Charge") or (dummy == "charge") or
       (dummy == "point-charge"))
    {
      //Point-charge force fields
      CHRG = 1;
    }
    if ((dummy == "GEM") or (dummy == "gem") or (dummy == "Gem"))
    {
      //Frozen density
      GEM = 1;
      if (TINKER)
      {
        //Collect TINKER multipoles or GEM-DM
        ExtractTINKpoles(Struct,0);
      }
    }
  }
  if ((dummy == "MM") or (dummy == "mm"))
  {
    //Read MMonly options
    MMonly = 1;
    QMonly = 0;
    QMMM = 0;
    regionfile >> dummy >> dummy; //MM wrapper
    if ((dummy == "Tinker") or (dummy == "TINKER")
       or (dummy == "tinker"))
    {
      TINKER = 1;
    }
    if ((dummy == "AMBER") or (dummy == "Amber") or
       (dummy == "amber"))
    {
      AMBER = 1;
    }
    if ((dummy == "LAMMPS") or (dummy == "lammps") or
       (dummy == "Lammps"))
    {
      LAMMPS = 1;
    }
    regionfile >> dummy >> dummy; //Potential type
    if ((dummy == "AMOEBA") or (dummy == "amoeba"))
    {
      //AMOEBA polarizable force field
      AMOEBA = 1;
      if (TINKER)
      {
        ExtractTINKpoles(Struct,0);
      }
    }
    if ((dummy == "Charges") or (dummy == "charges") or
       (dummy == "Charge") or (dummy == "charge") or
       (dummy == "point-charge"))
    {
      //Point-charge force fields
      CHRG = 1;
    }
    if ((dummy == "GEM") or (dummy == "gem") or (dummy == "Gem"))
    {
      //Frozen density
      GEM = 1;
      if (TINKER)
      {
        //Collect TINKER multipoles or GEM-DM
        ExtractTINKpoles(Struct,0);
      }
    }
  }
  regionfile >> dummy >> dummy; //Calculation type
  if ((dummy == "PIMC") or (dummy == "pimc"))
  {
    //Read MC and PIMC options
    PIMCSim = 1;
    regionfile >> dummy >> dummy; //Ensemble
    if ((dummy == "NVT") or (dummy == "nvt"))
    {
      //Set a consistent name for the ensemble
      QMMMOpts.Ensemble = "NVT";
    }
    if ((dummy == "NPT") or (dummy == "npt"))
    {
      //Set a consistent name for the ensemble
      QMMMOpts.Ensemble = "NPT";
    }
    regionfile >> dummy >> QMMMOpts.Temp;
    QMMMOpts.Beta = 1/(k*QMMMOpts.Temp);
    regionfile >> dummy >> QMMMOpts.Press;
    regionfile >> dummy >> QMMMOpts.Neq;
    regionfile >> dummy >> QMMMOpts.Nsteps;
    regionfile >> dummy >> QMMMOpts.Nbeads;
    regionfile >> dummy >> QMMMOpts.accratio;
    regionfile >> dummy >> QMMMOpts.Nprint;
    for (int i=0;i<Natoms;i++)
    {
      //Create path-integral beads
      for (int j=0;j<(QMMMOpts.Nbeads-1);j++)
      {
        //Pick random displacements
        double randx = (((double)rand())/((double)RAND_MAX));
        double randy = (((double)rand())/((double)RAND_MAX));
        double randz = (((double)rand())/((double)RAND_MAX));
        //Place the first bead at the initial position
        if (j == 0)
        {
          randx = 0.5;
          randy = 0.5;
          randz = 0.5;
        }
        Coord temp; //Bead coordinates
        //Set random bead displacements
        double MassScale = sqrt(12.0/Struct[i].m); //Relative to carbon
        MassScale *= 2*StepMin*CentRatio; //Scale based on settings
        temp.x = Struct[i].P[0].x+(2*(randx-0.5)*MassScale);
        temp.y = Struct[i].P[0].y+(2*(randy-0.5)*MassScale);
        temp.z = Struct[i].P[0].z+(2*(randz-0.5)*MassScale);
        //Save coordinates
        Struct[i].P.push_back(temp);
        //Copy charges from the first replica
        Mpole temp2 = Struct[i].MP[0];
        Struct[i].MP.push_back(temp2);
        OctCharges temp3 = Struct[i].PC[0];
        Struct[i].PC.push_back(temp3);
      }
    }
  }
  if ((dummy == "OPT") or (dummy == "Opt") or (dummy == "opt"))
  {
    //Read energy minimization options
    OptSim = 1;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
  }
  if ((dummy == "Steep") or (dummy == "steep") or
     (dummy == "SD") or (dummy == "sd"))
  {
    //Read SD energy minimization options
    SteepSim = 1;
    regionfile >> dummy >> QMMMOpts.StepScale;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.QMOptTol;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
  }
  if ((dummy == "QuickMin") or (dummy == "Quick") or
     (dummy == "quick") or (dummy == "quickmin") or
     (dummy == "DV") or (dummy == "dv"))
  {
    //Read damped Verlet energy minimization options
    QuickSim = 1;
    regionfile >> dummy >> QMMMOpts.StepScale;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.QMOptTol;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
  }
  if ((dummy == "bfgs") or (dummy == "BFGS") or
     (dummy == "dfp") or (dummy == "DFP"))
  {
    //Read energy minimization options for the DFP optimizer
    DFPSim = 1;
    if ((dummy == "bfgs") or (dummy == "BFGS"))
    {
      //Print BFGS error
      cerr << "Warning: A BFGS optimizer is not implemented.";
      cerr << '\n';
      cerr << " The DFP algorithm will be used instead of BFGS.";
      cerr << '\n' << '\n';
      cerr.flush(); //Print error immediately
    }
    regionfile >> dummy >> QMMMOpts.StepScale;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.QMOptTol;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
  }
  if ((dummy == "NEB") or (dummy == "neb"))
  {
    //Read energy minimization options for climbing image NEB
    NEBSim = 1;
    regionfile >> dummy >> QMMMOpts.Nbeads;
    regionfile >> dummy >> QMMMOpts.StepScale;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.Kspring;
    regionfile >> dummy >> dummy;
    if ((dummy == "Yes") or (dummy == "yes") or (dummy == "YES") or
       (dummy == "True") or (dummy == "true") or (dummy == "TRUE"))
    {
      QMMMOpts.FrznEnds = 1;
    }
    regionfile >> dummy >> QMMMOpts.QMOptTol;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
    //Set initial transition state
    if ((QMMMOpts.Nbeads%2) == 0)
    {
      //Even number of beads
      QMMMOpts.TSBead = (QMMMOpts.Nbeads/2); //Nearly the middle, product side
    }
    else
    {
      //Odd number of beads
      QMMMOpts.TSBead = ((QMMMOpts.Nbeads-1)/2); //Middle bead
    }
    //Duplicate data
    for (int i=0;i<Natoms;i++)
    {
      //Create reaction-path beads
      for (int j=0;j<(QMMMOpts.Nbeads-1);j++)
      {
        //Create replicas
        Coord temp = Struct[i].P[0];
        Struct[i].P.push_back(temp);
        Mpole temp2 = Struct[i].MP[0];
        Struct[i].MP.push_back(temp2);
        OctCharges temp3 = Struct[i].PC[0];
        Struct[i].PC.push_back(temp3);
      }
    }
  }
  if ((dummy == "ESD") or (dummy == "esd") or
     (dummy == "EnsembleSD") or (dummy == "ensembesd"))
  {
    //Read energy minimization options for ensemble steepest descent
    ESDSim = 1;
    regionfile >> dummy >> QMMMOpts.StepScale;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
    regionfile >> dummy >> QMMMOpts.dt;
    regionfile >> dummy >> QMMMOpts.Temp;
    regionfile >> dummy >> QMMMOpts.tautemp;
    regionfile >> dummy >> QMMMOpts.Nsteps;
  }
  if ((dummy == "ENEB") or (dummy == "eneb") or
     (dummy == "EnsembleNEB") or (dummy == "ensembeneb"))
  {
    //Read energy minimization options for ensemble NEB
    ENEBSim = 1;
    regionfile >> dummy >> QMMMOpts.Nbeads;
    regionfile >> dummy >> QMMMOpts.StepScale;
    regionfile >> dummy >> QMMMOpts.MaxStep;
    regionfile >> dummy >> QMMMOpts.Kspring;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
    regionfile >> dummy >> QMMMOpts.dt;
    regionfile >> dummy >> QMMMOpts.Temp;
    regionfile >> dummy >> QMMMOpts.tautemp;
    regionfile >> dummy >> QMMMOpts.Nsteps;
    //Error check
    if ((QMMMOpts.Nbeads%2) != 1)
    {
      //The number of beads must be odd
      QMMMOpts.Nbeads += 1; //Change number of beads
      cerr << "Warning: The number of replicas must be odd.";
      cerr << '\n';
      cerr << " Starting calculations with " << QMMMOpts.Nbeads;
      cerr << " beads.";
      cerr << '\n' << '\n';
      cerr.flush(); //Print error immediately
    }
    //Set transition state
    QMMMOpts.TSBead = ((QMMMOpts.Nbeads-1)/2); //Middle bead
    //Duplicate data
    for (int i=0;i<Natoms;i++)
    {
      //Create reaction-path beads
      for (int j=0;j<(QMMMOpts.Nbeads-1);j++)
      {
        //Create replicas
        Coord temp = Struct[i].P[0];
        Struct[i].P.push_back(temp);
        Mpole temp2 = Struct[i].MP[0];
        Struct[i].MP.push_back(temp2);
        OctCharges temp3 = Struct[i].PC[0];
        Struct[i].PC.push_back(temp3);
      }
    }
  }
  if ((dummy == "SP") or (dummy == "sp") or
     (dummy == "energy") or (dummy == "Energy"))
  {
    //Read energy minimization options
    SinglePoint = 1;
  }
  regionfile >> dummy >> dummy; //PBC options
  if ((dummy == "Yes") or (dummy == "yes") or
     (dummy == "YES") or (dummy == "true") or
     (dummy == "True") or (dummy == "TRUE"))
  {
    //Read box sizes
    PBCon = 1;
    regionfile >> dummy;
    regionfile >> Lx >> Ly >> Lz;
  }
  regionfile >> dummy >> Nqm; //Number of QM atoms
  for (int i=0;i<Nqm;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].QMregion = 1;
    Struct[AtomID].MMregion = 0;
  }
  regionfile >> dummy >> Npseudo; //Number of pseudo-bonds
  for (int i=0;i<Npseudo;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].PBregion = 1;
    Struct[AtomID].MMregion = 0;
  }
  regionfile >> dummy >> Nbound; //Number of boundary-atoms
  for (int i=0;i<Nbound;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].BAregion = 1;
    Struct[AtomID].MMregion = 0;
  }
  if (QMonly)
  {
    //Reset the numbers if regions were specified in the input
    Nqm = Natoms;
    Npseudo = 0;
    Nbound = 0;
    //Redundant, but safe
    for (int i=0;i<Natoms;i++)
    {
      Struct[i].QMregion = 1;
      Struct[i].MMregion = 0;
      Struct[i].PBregion = 0;
      Struct[i].BAregion = 0;
    }
  }
  if (MMonly)
  {
    //Reset the numbers if regions were specified in the input
    Nqm = 0;
    Npseudo = 0;
    Nbound = 0;
    //Redundant, but safe
    for (int i=0;i<Natoms;i++)
    {
      Struct[i].QMregion = 0;
      Struct[i].MMregion = 1;
      Struct[i].PBregion = 0;
      Struct[i].BAregion = 0;
    }
  }
  Nmm = Natoms-Nqm-Npseudo-Nbound; //Set number of MM atoms
  regionfile >> dummy >> Nfreeze; //Number of frozen atoms
  for (int i=0;i<Nfreeze;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].Frozen = 1;
    if (PIMCSim or ENEBSim or NEBSim)
    {
      //Frozen atoms must be the same for all replicas
      #pragma omp parallel for
      for (int j=0;j<QMMMOpts.Nbeads;j++)
      {
        Struct[AtomID].P[j].x = Struct[AtomID].P[0].x;
        Struct[AtomID].P[j].y = Struct[AtomID].P[0].y;
        Struct[AtomID].P[j].z = Struct[AtomID].P[0].z;
      }
      #pragma omp barrier
    }
  }
  //Read initial structures for all beads or create new ones
  if (CheckFile("BeadStartStruct.xyz") and (!GauExternal))
  {
    //Print output
    cout << "Reading restart information...";
    cout << '\n' << '\n';;
    //Open file
    fstream beadfile;
    beadfile.open("BeadStartStruct.xyz",ios_base::in);
    //Read and discard number of atoms
    int AtTest = 0;
    beadfile >> AtTest;
    if (AtTest != (Natoms*QMMMOpts.Nbeads))
    {
      //Print warning if the XYZ file has incorrect dimensions
      cout << "Error: Restart file does not have the correct format!";
      cout << '\n' << '\n';
      cout.flush();
      //Quit
      exit(0);
    }
    //Read atom/bead positions
    for (int i=0;i<Natoms;i++)
    {
      for (int j=0;j<QMMMOpts.Nbeads;j++)
      {
        //Read atom type and discard
        beadfile >> dummy;
        //Read XYZ coordinates
        beadfile >> Struct[i].P[j].x;
        beadfile >> Struct[i].P[j].y;
        beadfile >> Struct[i].P[j].z;
      }
    }
  }
  else if (ENEBSim or NEBSim)
  {
    //Exit with an error
    cout << "Error: No initial reaction path found in the restart file!!!";
    cout << '\n' << '\n';
    cout.flush();
    //Quit
    exit(0);
  }
  //Collect additonal TINKER input
  if (TINKER and (!GauExternal))
  {
    //Classes are not used in the QMMM, but looking for them can spot errors
    FindTINKERClasses(Struct);
  }
  //Check if QM log files should be saved
  if (CheckFile("BACKUPQM"))
  {
    //Read backup directory
    fstream backfile;
    //Set to default value
    QMMMOpts.BackDir = "Old_files";
    //Check directory
    backfile.open("BACKUPQM",ios_base::in);
    if (backfile.good())
    {
      string newname;
      backfile >> newname;
      if (!backfile.eof())
      {
        QMMMOpts.BackDir = newname;
      }
    }
  }
  //Set OpenMP threads based on QM CPUs and total CPUs
  if (!GauExternal)
  {
    //Get total number of processors
    double Procs = double(FindMaxThreads());
    //Set default number of threads
    Nthreads = FindMaxThreads();
    #ifdef _OPENMP
      omp_set_num_threads(Nthreads);
    #endif
    //Sanity check
    if (Ncpus > Nthreads)
    {
      //Assuming only one node is used for QM
      Ncpus = Nthreads;
    }
    //Modify threads for multi-replica simulations
    if ((QMMMOpts.Nbeads > 1) and (!ENEBSim) and (!NEBSim))
    {
      //Divide threads between the beads
      Nthreads = int(floor(Procs/Ncpus));
      //Set number of threads for wrappers
      #ifdef _OPENMP
        omp_set_num_threads(Nthreads);
      #endif
    }
    //Set eigen threads
    setNbThreads(Nthreads);
  }
  return;
};

void LICHEMErrorChecker(QMMMSettings& QMMMOpts)
{
  //Checks for basic errors and conflicts
  bool DoQuit = 0; //Bool, quit with error
  //General errors
  if (QMMM)
  {
    //Check number of QM and MM atoms
    if ((Nqm+Npseudo) < 1)
    {
      //Make sure there are some atoms in the QM calculation
      cout << " Error: No QM or PB atoms defined for the QMMM calculations.";
      cout << '\n';
      DoQuit = 1;
    }
    if ((Nmm+Nbound) < 1)
    {
      //Make sure there are some atoms in the MM calculations
      cout << " Error: No MM or BA atoms defined for the QMMM calculations.";
      cout << '\n';
      DoQuit = 1;
    }
  }
  if (Ncpus < 1)
  {
    //Checks the number of threads and continue
    cout << " Warning: Calculations cannot run with ";
    cout << Ncpus << " CPUs.";
    cout << '\n';
    if (Jokes)
    {
      cout << " Do you know how computers work?";
    }
    cout << " Ncpus set to 1";
    cout << '\n';
    Ncpus = 1;
    cout.flush(); //Print warning
  }
  //Wrapper errors
  if ((!TINKER) and (!AMBER) and (!LAMMPS) and (!QMonly))
  {
    //Check the MM wrappers
    cout << " Error: No valid MM wrapper selected.";
    cout << '\n';
    cout << "  Select a wrapper if you want to run this type ";
    cout << "of calculation.";
    cout << '\n';
    DoQuit = 1;
  }
  if ((!Gaussian) and (!PSI4) and (!NWChem) and (!MMonly))
  {
    //Check the QM wrappers
    cout << " Error: No valid QM wrapper selected.";
    cout << '\n';
    cout << "  Select a wrapper if you want to run this type ";
    cout << "of calculation.";
    cout << '\n';
    DoQuit = 1;
  }
  if (PSI4 and QMMM)
  {
    //Avoid options that conflict with PSI4 capabilities
    if (OptSim)
    {
      //The PSI4 optimizer cannot incorporate MM forces
      cout << " Error: QMMM PSI4 optimizations can only be performed with";
      cout << '\n';
      cout << " the steepest descent, damped Verlet, or DFP.";
      cout << '\n';
      DoQuit = 1;
    }
    if ((Npseudo != 0) or (Nbound != 0))
    {
      //PSI4 does not currently have pseudopotentials
      cout << " Error: The PSI4 wrapper can only use QM and MM atoms.";
      cout << '\n';
      cout << " Remove the pseudo-bonds and boundary-atoms.";
      cout << '\n';
      DoQuit = 1;
    }
  }
  if (NWChem and QMMM)
  {
    //Avoid options that conflict with NWChem capabilities
    if (OptSim)
    {
      //The NWChem optimizer cannot incorporate MM forces
      cout << " Error: QMMM NWChem optimizations can only be performed with";
      cout << '\n';
      cout << " the steepest descent, damped Verlet, or DFP.";
      cout << '\n';
      DoQuit = 1;
    }
  }
  if (LAMMPS and AMOEBA)
  {
    //Avoid options that conflict with LAMMPS capabilities
    cout << " Error: LAMMPS calculations cannot be performed with";
    cout << '\n';
    cout << " polarizable force fields.";
    cout << '\n';
    DoQuit = 1;
  }
  //Simulation errors
  if ((QMMMOpts.Ensemble == "NPT") and (!PBCon))
  {
    //Check the PBC options
    cout << " Error: NPT simulation without PBC.";
    cout << '\n';
    cout << "  Turn PBC on if you want to run this type ";
    cout << "of calculation.";
    cout << '\n';
    DoQuit = 1;
  }
  if (QMMMOpts.StepScale > 1)
  {
    //Checks the number of threads and continue
    cout << " Warning: The optimization step scale cannot be greater";
    cout << " than 1.";
    cout << '\n';
    cout << " Step scale set to 1.";
    cout << '\n';
    QMMMOpts.StepScale = 1; //Reset step size
    cout.flush(); //Print warning
  }
  if (DoQuit)
  {
    //Quits
    cout << '\n';
    cout.flush();
    exit(0);
  }
  //Sarcastically continue
  cout << "No fatal errors detected.";
  cout << '\n';
  if (Jokes)
  {
    cout << " And there was much rejoicing. Yay...";
    cout << '\n';
    cout << '\n';
    cout.flush();
    if (CheckFile("EASTEREGG"))
    {
      PrintLapin();
    }
  }
  return;
};

void LICHEMPrintSettings(QMMMSettings& QMMMOpts)
{
  //Prints out the simulation details
  cout << "Setting up simulation..." << '\n';
  cout << '\n';
  cout << "Atoms: " << Natoms << '\n';
  if (QMonly or QMMM)
  {
    //QM regions
    cout << " QM atoms: " << Nqm << '\n';
    cout << "  Charge: " << QMMMOpts.Charge << '\n';
    cout << "  Spin: " << QMMMOpts.Spin << '\n';
  }
  if (MMonly or QMMM)
  {
    //MM regions
    cout << " MM atoms: " << Nmm << '\n';
    if (QMMM)
    {
      cout << " Pseudo-atoms: " << Npseudo << '\n';
      cout << " Boundary-atoms: " << Nbound << '\n';
    }
    if (Nfreeze > 0)
    {
      cout << " Frozen atoms: " << Nfreeze << '\n';
    }
  }
  if (ENEBSim or NEBSim)
  {
    //Print reaction path input for error checking
    cout << " RP beads: " << QMMMOpts.Nbeads << '\n';
    cout << '\n';
    cout << "Simulation mode: ";
    if (QMMM)
    {
      cout << "QMMM";
    }
    if (QMonly)
    {
      cout << "Pure QM";
    }
    if (MMonly)
    {
      cout << "Pure MM";
    }
    cout << " ";
    if (ENEBSim)
    {
      cout << "ensemble ";
    }
    cout << "NEB" << '\n';
  }
  if (PIMCSim)
  {
    //Print PIMC input for error checking
    if (QMMMOpts.Nbeads > 1)
    {
      cout << " PI beads: " << QMMMOpts.Nbeads << '\n';
    }
    cout << '\n';
    cout << "Simulation mode: ";
    if (QMMM)
    {
      cout << "QMMM";
    }
    if (QMonly)
    {
      cout << "Pure QM";
    }
    if (MMonly)
    {
      cout << "Pure MM";
    }
    cout << " " << QMMMOpts.Ensemble;
    if (QMMMOpts.Nbeads > 1)
    {
      cout << " path-integral";
    }
    cout << " Monte Carlo" << '\n';
    cout << " Equilibration MC steps: " << QMMMOpts.Neq << '\n';
    cout << " Production MC steps: " << QMMMOpts.Nsteps << '\n';
  }
  if (OptSim or SteepSim or QuickSim or DFPSim or ESDSim)
  {
    //Print optimization input for error checking
    cout << '\n';
    cout << "Simulation mode: ";
    if (QMMM)
    {
      cout << "QMMM";
    }
    if (QMonly)
    {
      cout << "Pure QM";
    }
    if (MMonly)
    {
      cout << "Pure MM";
    }
    cout << " energy minimization" << '\n';
    if (QMMM or QMonly)
    {
      cout << " QM";
      if (QMMM)
      {
        cout << "MM";
      }
      cout << " minimizer: ";
      if (OptSim)
      {
        cout << "Native QM optimizer" << '\n';
      }
      if (SteepSim)
      {
        cout << "LICHEM steepest descent" << '\n';
      }
      if (QuickSim)
      {
        cout << "LICHEM damped Verlet" << '\n';
      }
      if (DFPSim)
      {
        cout << "LICHEM DFP" << '\n';
      }
      if (ESDSim)
      {
        cout << "Ensemble steepest descent" << '\n';
      }
    }
  }
  if (SinglePoint)
  {
    //Print single-point energy settings for error checking
    cout << '\n';
    cout << "Simulation mode: ";
    if (QMMM)
    {
      cout << "QMMM";
    }
    if (QMonly)
    {
      cout << "Pure QM";
    }
    if (MMonly)
    {
      cout << "Pure MM";
    }
    cout << " single-point energy" << '\n';
  }
  if (QMonly or QMMM)
  {
    //Print QM wrapper input for error checking
    cout << " QM wrapper: ";
    if (PSI4)
    {
      cout << "PSI4" << '\n';
    }
    if (Gaussian)
    {
      cout << "Gaussian" << '\n';
    }
    if (NWChem)
    {
      cout << "NWChem" << '\n';
    }
    cout << " QM method: ";
    if (QMMMOpts.Func != "SemiEmp")
    {
      //Avoid printing method and basis for semi-empirical
      cout << QMMMOpts.Func << "/";
    }
    cout << QMMMOpts.Basis << '\n';
  }
  if (MMonly or QMMM)
  {
    //Print MM wrapper input for error checking
    cout << " MM wrapper: ";
    if (TINKER)
    {
      cout << "TINKER" << '\n';
    }
    if (AMBER)
    {
      cout << "AMBER" << '\n';
    }
    if (LAMMPS)
    {
      cout << "LAMMPS" << '\n';
    }
    if (QMMM)
    {
      //Print QMMM wrapper input for error checking
      cout << " QMMM potential: ";
      if (CHRG)
      {
        cout << "Point-charge force field" << '\n';
      }
      if (AMOEBA)
      {
        cout << "Polarizable force field" << '\n';
      }
      if (GEM)
      {
        cout << "Frozen density force field" << '\n';
      }
    }
  }
  cout << '\n';
  //Print parallelization settings
  cout << "Parallelization and memory settings:" << '\n';
  cout << " OpenMP threads: " << Nthreads << '\n';
  if (QMonly or QMMM)
  {
    if (OptSim and Gaussian)
    {
      //Print modified threading for GauExternal
      if (Ncpus <= 2)
      {
        cout << " Opt. threads: 1" << '\n';
        cout << " QM threads: 1" << '\n';
      }
      else
      {
        cout << " Opt. threads: 2" << '\n';
        cout << " QM threads: " << (Ncpus-2) << '\n';
      }
    }
    else
    {
      cout << " QM threads: " << Ncpus << '\n';
    }
    cout << " QM memory: " << QMMMOpts.RAM << " ";
    if (QMMMOpts.MemMB)
    {
      cout << "MB";
    }
    else
    {
      cout << "GB";
    }
    cout << '\n';
  }
  //Print convergence criteria for optimizations
  if (OptSim or SteepSim or QuickSim or DFPSim or
     ESDSim or ENEBSim or NEBSim)
  {
    cout << '\n';
    cout << "Optimization settings:" << '\n';
    if (!OptSim)
    {
      cout << " Step scale factor: " << QMMMOpts.StepScale;
      cout << '\n';
    }
    cout << " Max. step size: " << QMMMOpts.MaxStep;
    cout << " \u212B" << '\n';
    cout << " Max. steps: " << QMMMOpts.MaxOptSteps;
    if (ENEBSim or NEBSim)
    {
      //Spring constant for the path
      cout << '\n';
      cout << " Spring constant: " << QMMMOpts.Kspring;
      cout << " eV/\u212B\u00B2" << '\n';
      cout << " End points: ";
      if (QMMMOpts.FrznEnds)
      {
        cout << "Frozen";
      }
      else
      {
        cout << "Active";
      }
    }
    cout << '\n' << '\n';
    if (SteepSim or QuickSim or DFPSim or NEBSim)
    {
      cout << "QM convergence criteria:" << '\n';
      cout << "  RMS deviation: " << QMMMOpts.QMOptTol;
      cout << " \u212B" << '\n';
      cout << "  Max. force: " << (20*QMMMOpts.QMOptTol);
      cout << " eV/\u212B" << '\n';
      cout << "  RMS force: " << (10*QMMMOpts.QMOptTol);
      cout << " eV/\u212B" << '\n';
      cout << '\n';
    }
    if (ESDSim or ENEBSim)
    {
      cout << "MD settings:" << '\n';
      cout << " Timestep: " << QMMMOpts.dt;
      cout << " fs" << '\n';
      cout << " Temperature: " << QMMMOpts.Temp;
      cout << " K" << '\n';
      cout << " Thermostat constant, \u03C4: " << QMMMOpts.tautemp;
      cout << " ps" << '\n';
      cout << " MD steps: " << QMMMOpts.Nsteps << '\n';
      cout << '\n';
    }
    else
    {
      cout << "MM convergence criteria:" << '\n';
      cout << "  RMS deviation: " << QMMMOpts.MMOptTol;
      cout << " \u212B" << '\n';
      cout << "  RMS force: " << QMMMOpts.MMOptTol*kcal2eV;
      cout << " eV/\u212B" << '\n';
      cout << '\n';
    }
  }
  cout.flush(); //Flush for output being redirected to a file
  return;
};

void GetQuotes(vector<string>& Quotes)
{
  //Generate random quotes
  string dummy; //Generic string
  dummy = "\'It is difficult to prove that this quote is not random.\'";
  dummy += '\n';
  dummy += "                                           -Eric G. Kratz";
  for (int i=0;i<1000;i++)
  {
    //Add quotes to the list
    Quotes.push_back(dummy);
  }
  return;
};
