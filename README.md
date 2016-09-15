# TsunamiSquares
C++ library for modeling tsunamis from Virtual Quake simulated earthquakes using the Tsunami Squares method (Steven N. Ward).  NOAA ETOPO1 combined bathymetry and topology data included.

To compile, use
$ bash setup.sh

Run simulation with
$ ./TsunamiSquares tsunami_params.txt

Visualizations can be created with
$ python tsunamisquares.py


== Parameter Definitions ==
tsunami_params.txt contains simulation parameters which may be changed without recompiling from source.

Parameter						Definition
--------------------------------------------------
out_file_name 					File name for simulation result output
bathy_file 						File with bathymetry/topography data
kml_file 						File for KML output (currently unused)
deformation_file 				File name for initial seafloor uplift data (e.g. from thrust earthquake)
D 								Diffusion constant; 
new_depth 						Used when flattening the bathymetry to a constant depth for testing purposes
								(negative for below sea level)
bump_height 					Used when testing, produces small initial seafloor displacement
N_steps 						Number of time steps in simulation
current_step 					Initial time step
update_step 					Period of printing simulation state to terminal
save_step 						Period of printing simulation state to file
time 							Initial time
output_num_digits_for_percent 	Float precision for output
