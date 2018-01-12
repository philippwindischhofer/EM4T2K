------------------------------------------------------------------------------------------------------------
 This is EM4T2K, using the expectation maximization algorithm to reconstruct events in the WAGASCI detector
------------------------------------------------------------------------------------------------------------

# 0.) Introduction:
The purpose is to assess the feasibility of using the expectation maximization algorithm (as implemented in `libgem`) for track finding in events recorded by the WAGASCI detector, and to compare the performance with that of the legacy algorithm used so far, which is based on a cellular automaton. This project consists of two main parts:

- reconstruction: takes as input a raw data file (containing real events or simulated MC events) and applies `libgem` to fit the tracks contained therein. It then saves the tracks into a ROOT tree, using the same format as the legacy reconstruction.
- comparison: takes as input two processed files, the output of the legacy reconstruction algorithm, and the output of the `libgem`-based reconstruction. It uses different metrics to compare the quality of the reconstruction done with both methods.

# 1.) To prepare the environment and compile the code:

- source the file `setup_env.sh`
- compile the code by typing `make all` (Note: you may have to adapt the version of your `gcc` compiler in the Makefile, for development and initial testing, `gcc 4.9` was used. You may also have to adapt the directory in which `libgem` is installed. The default directory used is `../libgem`.)

This will create two executables, `reco` and `comp`.

# 2.) To run the project:

- the first step is to apply `libgem` to the track reconstruction. This part is handled by `reco`. To run it on an input file containing the raw data (for example, `WMMC_Run1_1_wNoise.root`), do
```
./reco WMMC_Run1_1_wNoise.root libgem_reco.root
```
The file `libgem_reco.root` will then contain the reconstructed events.

- the next step is to run `comp` to compare this result against the output of the legacy reconstruction. For the input file used above, this information is contained in `WMMC_Run1_1_wNoise_legacy_reco.root`. Thus, run
```
./comp WMMC_Run1_1_wNoise_legacy_reco.root libgem_reco.root 
```
This will produce, draw and save several plots. By default, it generates the following output files:

- `number_tracks.pdf`: a 2D histogram comparing the number of tracks found by `libgem` and the legacy reconstruction code
- `track_orientation.pdf`: for events where both `libgem` and the legacy reconstruction fitted exactly one track, it shows the distribution of the angle between these two reconstructed tracks
- `vertex_distance.pdf`: for events where both `libgem` and the legacy reconstruction fitted exactly two tracks (i.e. the class of events having a unique intersection point), it computes and shows the distribution of the euclidean distance between the intersection points determined by the two methods
- `vertex_MC_distance.pdf`: for the same class of events, it computes and shows instead the euclidean distance between the vertex determined by `libgem` and the actual truth information

# 3.) To extend and adapt the code:

In order to add new metrics with which to compare different classes of events, you may want to update / extend the class `EventReferee`. The method `CompareEvents()` is called for each event that needs to be compared. The method `Report()` is called once all events have been processed. This method is intended to carry out the relevant plotting operations at the end. The class `EventMetrics` is supposed to contain any auxiliary methods needed to carry out the actual comparison.