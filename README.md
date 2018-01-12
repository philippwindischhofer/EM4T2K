------------------------------------------------------------------------------------------------------------
 This is EM4T2K, using the expectation maximization algorithm to reconstruct events in the WAGASCI detector
------------------------------------------------------------------------------------------------------------

0.) Introduction: The purpose is to assess the feasibility of using the expectation maximization algorithm (as implemented in libgem) for track finding in events recorded by the WAGASCI detector, and to compare the performance with that of the legacy algorithm used so far, which is based on a cellular automaton. This project consists of two main parts:
    * reconstruction: takes as input a raw data file (containing real events or simulated MC events) and applies libgem to fit the tracks contained therein. It then saves the tracks into a ROOT tree, using the same format as the legacy reconstruction.
    * comparison: takes as input two processed files, the output of the legacy reconstruction algorithm, and the output of the libgem-based reconstruction. It uses different metrics to compare the quality of the reconstruction done with both methods.

1.) To prepare the environment and run the code:
    

2.) To extend and adapt the code:
