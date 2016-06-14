// Copyright (c) 2015 Kasey W. Schultz
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "TsunamiSquares.h"
#include <time.h>

int main (int argc, char **argv) {
    // Initialize the world (where the squares live), squares and vertices
    tsunamisquares::World                       this_world;
    tsunamisquares::SquareIDSet::const_iterator it;
    tsunamisquares::SquareIDSet                 ids;
    std::ofstream                               out_file;
    clock_t                                     start,end;
    
    
    // -------------------------------------------------------------------------------- //
    ///////////          CONSTANTS (to be moved to sim parameter file)        ////////////    
    // -------------------------------------------------------------------------------- //
    const std::string   out_file_name    = "RAMYA_MAKES_LAND.txt"; // name this relevant
    const std::string   bathy_file       = "Pacific_5184.txt"; // name this relevant
    //const std::string   kml_file         = "local/Pacific_36.kml";
    // const std::string   deformation_file = "local/Channel_Islands_test_bump.txt"; // kasey says we dont need this now
    // until later
    
    // Diffusion constant (fit to a reasonable looking sim)
    double D = 140616.45; //140616.45;
    // Flattening the bathymetry to a constant depth (negative for below sea level)
    double new_depth = -100.0;
    // Bumping up the bottom
    double bump_height = 50.0;
    // Number of times to move squares
    int N_steps = 20; //number of time steps 10 is fine, to see a bit of movement
    // because boundaries aren't defined very well, we limit the time steps whenever the water hits the walls of things
    // Updating intervals, etc.
    int current_step = 0;
    int update_step = 1;
    int save_step = 1;
    double time = 0.0;
    int output_num_digits_for_percent = 3;
    // Header for the simulation output
    const std::string   header = "# time \t lon \t\t lat \t\t water height \t altitude \n";
    
    
    
    // -------------------------------------------------------------------------------- //
    ///////                Simulation Initialization and Loading                   ///////
    // --------------------------------------------------------------------------------//
    start = clock();
    // Read in the bathymetry data
    this_world.clear();
    std::cout << std::endl << "Reading..."   << bathy_file.c_str() << std::endl;
    this_world.read_bathymetry(bathy_file.c_str());
    // Index the neighbors by left/right/top etc.
    std::cout << "Indexing neighbors......";
    this_world.computeNeighbors();
    
    // Compute the time step given the diffusion constant D
    double dt = (double) (int) this_world.square(0).Lx()*this_world.square(0).Ly()/(2*D); //seconds
    
    // Gather model information
    this_world.info();
    int num_lats = this_world.num_lats();
    int num_lons = this_world.num_lons();
    std::cout << "Lons by Lats = (" << num_lons << ", " << num_lats << ")...";
    ids = this_world.getSquareIDs();
    double max_time = N_steps*dt;

//
//    tsunamisquares::SquareIDSet valids = this_world.square(0).get_valid_neighbors();
//    tsunamisquares::SquareIDSet::const_iterator vit;
//    
//    for (vit=valids.begin(); vit!=valids.end(); ++vit) {
//        std::cout << *vit << std::endl;
//    }
    
    // Write KML model
    //std::cout << "Writing KML..."   << kml_file.c_str() << "  ...";
    //this_world.write_file_kml(kml_file.c_str());

    // Flatten the bottom for simple simulation test cases, do not do this for tsunami simulations
    std::cout << "Flattening the bottom...";
    this_world.flattenBottom(new_depth);
    
    // for (int pixel = 0; pixel++; pixel < (int) (num_lons*0.75)){
    //   tsunamisquares::UIndex landcenter = (int) (pixel);
    //   tsunamisquares::UIndex landright   = this_world.square(landcentral).right();
    //   this_world.deformBottom(landright,  bump_height);
    // }
   
    // Put water into squares to bring water level up to sealevel.
    std::cout << "Filling with water..." << std::flush;
    

    // for (int pixel_lons = 0; pixel_lons++; pixel_lons < num_lons){
    //   for ( int pixel_lats = 0; pixel_lats++; pixel_lats < ((int) num_lats*0.5) )
    //     this_world.deformBottom(pixel_lons*pixel_lats,  bump_height*10);
    // }
    
    

    // --------------------------------------------------------------------------------//
    //            Sea Floor Deformation and Initial Conditions                         //
    // --------------------------------------------------------------------------------//
    std::cout << "Deforming the bottom... " << std::endl;
    
    //     ==  DEFORM VIA FILE        ======
    //this_world.deformFromFile(deformation_file.c_str());

   //     ==  DEFORM VIA CENTER BUMP ======
   // Find the 4 center squares and bump them by a constant height upward
   // tsunamisquares::UIndex central = (int) (0.5*num_lons*(num_lats + 1));
   // std::cout << " about the central square " << central << "...";
   
   // tsunamisquares::UIndex left    = this_world.square(central).left();
   // tsunamisquares::UIndex right   = this_world.square(central).right();
   // tsunamisquares::UIndex top     = this_world.square(central).top();
   // tsunamisquares::UIndex bottom  = this_world.square(central).bottom();
   
   // this_world.deformBottom(central,bump_height);
   // this_world.deformBottom(left,   bump_height);
   // this_world.deformBottom(top,    bump_height);
   // this_world.deformBottom(right,  bump_height);
   // this_world.deformBottom(bottom, bump_height);

   //   == DEFORM A LAND BUMP =======

    
    std::cout << " making a bowl shape...";
    // For Pacific_900.txt, each row has 30 pixels. 
    // For Pacific_5184.txt, each row has 72 pixels.
    // center box
    // int landRowLeft = 756; 
    // int landRowRight = 766;
    // for (int gradient = 0; gradient < 5; gradient++){
    //   for (tsunamisquares::UIndex centralDIFF = (landRowLeft); centralDIFF < (landRowRight); centralDIFF++){
    //     this_world.deformBottom(centralDIFF,bump_height - gradient*5);
    //   }
    //   landRowLeft  = landRowLeft + 72 + 1 ;
    //   landRowRight = landRowRight + 72 - 1 ;
    // }

    // landRowLeft = 1404; 
    // landRowRight = 1414;
    // for (int gradient = 0; gradient < 5; gradient++){
    //   for (tsunamisquares::UIndex centralDIFF = (landRowLeft); centralDIFF < (landRowRight); centralDIFF++){
    //     this_world.deformBottom(centralDIFF,bump_height - gradient*5);
    //   }
    //   landRowLeft  = landRowLeft - 72 + 1;
    //   landRowRight = landRowRight - 72 - 1;
    // }

    // tsunamisquares::UIndex centralDIFF = 828;
    // for (int i = 0; i < 8; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height);
    // }

    // centralDIFF =  centralDIFF + 73;
    // for (int i = 0; i < 6; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height - 5*1);
    // }

    // centralDIFF =  centralDIFF + 73;
    // for (int i = 0; i < 4; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height - 5*2);
    // }

    // centralDIFF =  centralDIFF + 73;
    // for (int i = 0; i < 2; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height - 5*3);
    // }

    // centralDIFF = 837;
    // for (int i = 0; i < 8; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height);
    // }

    // centralDIFF =  centralDIFF + 71;
    // for (int i = 0; i < 6; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height - 5*1);
    // }

    // centralDIFF =  centralDIFF + 71;
    // for (int i = 0; i < 4; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height - 5*2);
    // }

    // centralDIFF =  centralDIFF + 71;
    // for (int i = 0; i < 2; i ++ ){
    //   this_world.deformBottom(centralDIFF + 72*i ,bump_height - 5*3);
    // }

    // egdes of sandbox
    // top
    for (int j = 0; j < 72; j++){
      this_world.deformBottom(j, bump_height - 0*5);
    }
    for (int j = (72+1); j < (144-1); j++){
      this_world.deformBottom(j, bump_height - 1*5);
    }
    for (int j = (144+2); j < (216-2); j++){
      this_world.deformBottom(j, bump_height - 2*5);
    }
    for (int j = (216+3); j < (288-3); j++){
      this_world.deformBottom(j, bump_height - 3*5);
    }
    for (int j = (288+4); j < (360-4); j++){
      this_world.deformBottom(j, bump_height - 4*5);
    }
    // bottom
    for (int j = 5112; j < 5185; j++){
      this_world.deformBottom(j, bump_height - 0*5);
    }
    for (int j = (5040+1); j < (5112-1); j++){
      this_world.deformBottom(j, bump_height - 1*5);
    }
    for (int j = (4968+2); j < (5040-2); j++){
      this_world.deformBottom(j, bump_height - 2*5);
    }
    for (int j = (4896+3); j < (4968-3); j++){
      this_world.deformBottom(j, bump_height - 3*5);
    }
    for (int j = (4824+4); j < (4896-4); j++){
      this_world.deformBottom(j, bump_height - 4*5);
    }
    // right
    for (int j = 1; j < 71; j++){
      this_world.deformBottom(j*72, bump_height - 0*5);
    }
    for (int j = 2; j < 70; j++){
      this_world.deformBottom(1 + 72*j, bump_height - 1*5);
    }
    for (int j = 3; j < 69; j++){
      this_world.deformBottom(2 + 72*j, bump_height - 2*5);
    }
    for (int j = 4; j < 68; j++){
      this_world.deformBottom(3 + 72*j, bump_height - 3*5);
    }
    for (int j = 5; j < 67; j++){
      this_world.deformBottom(4 + 72*j, bump_height - 4*5);
    }
    // // left
    for (int j = 1; j < 71; j++){
      this_world.deformBottom(71+ j*72, bump_height - 0*5);
    }
    for (int j = 2; j < 70; j++){
      this_world.deformBottom(70 + 72*j, bump_height - 1*5);
    }
    for (int j = 3; j < 69; j++){
      this_world.deformBottom(69 + 72*j, bump_height - 2*5);
    }
    for (int j = 4; j < 68; j++){
      this_world.deformBottom(68 + 72*j, bump_height - 3*5);
    }
    for (int j = 5; j < 67; j++){
      this_world.deformBottom(67 + 72*j, bump_height - 4*5);
    }

this_world.fillToSeaLevel();

   tsunamisquares::UIndex central = (int) (0.5*num_lons*(num_lats + 1));
   std::cout << " about the central square " << central << "...";
   
   tsunamisquares::UIndex left    = this_world.square(central).left();
   tsunamisquares::UIndex right   = this_world.square(central).right();
   tsunamisquares::UIndex top     = this_world.square(central).top();
   tsunamisquares::UIndex bottom  = this_world.square(central).bottom();
   
   this_world.deformBottom(central,bump_height);
   this_world.deformBottom(left,   bump_height);
   this_world.deformBottom(top,    bump_height);
   this_world.deformBottom(right,  bump_height);
   this_world.deformBottom(bottom, bump_height);


//    // Optional: diffuse the central bump to be less peaked
//    this_world.diffuseSquares(dt);
    
    //----==  DEFORM A STAIRCASE in the middle. Testing the plane fitting ======-------
    // tsunamisquares::UIndex central = (int) (0.5*num_lons*(num_lats + 1));
    // std::cout << " about the central square " << central << "...";
    
    // double mid_bump = this_world.square(central).Lx();
    // double hi_bump = 2.0*mid_bump;
    
    
    // tsunamisquares::UIndex left        = this_world.square(central).left();
    // tsunamisquares::UIndex right       = this_world.square(central).right();
    // tsunamisquares::UIndex top         = this_world.square(central).top();
    // tsunamisquares::UIndex top_left    = this_world.square(central).top_left();
    // tsunamisquares::UIndex top_right   = this_world.square(central).top_right();
    // tsunamisquares::UIndex bottom      = this_world.square(central).bottom();
    // tsunamisquares::UIndex bottom_left = this_world.square(central).bottom_left();
    // tsunamisquares::UIndex bottom_right= this_world.square(central).bottom_right();

    // // Stair case is hi to the left, drops to zero on the right
    // this_world.deformBottom(right,        hi_bump);
    // this_world.deformBottom(top_right,    hi_bump);
    // this_world.deformBottom(bottom_right, hi_bump);
    
    // this_world.deformBottom(central,     mid_bump);
    // this_world.deformBottom(top,         mid_bump);
    // this_world.deformBottom(bottom,      mid_bump);
    
    // this_world.getGradient_planeFit(central);
    
//    double x_result = this_world.fitPointsToPlane(this_world.square(central).get_nearest_neighbors_and_self());
//    std::cout << "Best fit plane to " << this_world.square(central).get_nearest_neighbors_and_self().size() << " squares." << std::endl;
//    std::cout << "grabbed x = (" << x_result[0] << ", " << x_result[1] << ", " << x_result[2] << std::endl;


   // --------------------------------------------------------------------------------//
   // --==                         File I/O Preparation                          --== //   
   // --------------------------------------------------------------------------------//            
   out_file.open(out_file_name.c_str());
   out_file << header.c_str();
   std::cout.precision(output_num_digits_for_percent);
   
   
   
   // --------------------------------------------------------------------------------//
   // --========-           Begin the Simulation; Move the Squares          ----====- //          
   // --------------------------------------------------------------------------------//    
   std::cout << "Moving squares....time_step=" <<dt << "...";
   while (time < max_time) {
       // If this is a writing step, print status
       if (current_step%update_step == 0) {
           std::cout << ".." << (100.0*current_step)/N_steps << "%..";
           std::cout << std::flush;
       }
   
       // Write the current state to file
       if (current_step%save_step == 0) {
           for (it=ids.begin(); it!=ids.end(); ++it) {
               this_world.write_square_ascii(out_file, time, *it);
           }
       }
       // Move the squares
       this_world.moveSquares(dt);
       this_world.diffuseSquares(dt); // may need to turn back on; is a smoothing operation. will take a bump of h20
       time += dt;
       current_step += 1;
   }
   out_file.close();
   
   
   // --------------------------------------------------------------------------------//
   // --========---                    Wrap up and Reporting            ---=======--- //
   // --------------------------------------------------------------------------------//        
   std::cout << std::endl << "Results written to " << out_file_name << std::endl;
   end = clock();
   std::cout.precision(2+output_num_digits_for_percent);
   std::cout << "Total time: " << (float(end)-float(start))/CLOCKS_PER_SEC << " secs." << std::endl << std::endl;
    return 0;
}


//////////// Chalkboard ////////////////

// Grab all squares and print
//    ids = this_world.getSquareIDs();
//    
//    for (it=ids.begin(); it!=ids.end(); ++it) {
//        this_world.printSquare(*it);
//    }


    // Creating up sloping beach over the bottom 5 rows
//    assertThrow(num_lats == num_lons, "lats and lons mismatch");
//    
//    for (unsigned int i=0; i< (int) this_world.num_squares(); ++i) {
//        int row = (int)(i/num_lons);
//        if (row == num_lats-5) this_world.deformBottom(i, 50);
//        if (row == num_lats-4) this_world.deformBottom(i, 75);
//        if (row == num_lats-3) this_world.deformBottom(i, 90);
//        if (row == num_lats-2) this_world.deformBottom(i, 101);
//        if (row == num_lats-1) this_world.deformBottom(i, 110);
//        
//    }

    
//    for (unsigned int i=0; i< (int) this_world.num_squares(); ++i) {
//        // Create a wave coming from the top down, first row
//        int row = (int)(i/num_lons);
//        int col = (int)(i%num_lons);
//        if (row== num_lats-8 && col > 9 && col < num_lons-10) {
//            // Wave is 1m hi
//            this_world.deformBottom(i, 10);
//            //this_world.setSquareVelocity(i, tsunamisquares::Vec<2>(0.0, -this_world.square(0).Ly()/100));
//        }
//    }
    
    // Initial conditions
//    tsunamisquares::UIndex bot_right = (int)(this_world.num_squares()*0.5 + 0.5*sqrt(this_world.num_squares()));
//    tsunamisquares::UIndex bot_left  = bot_right-1;
//    tsunamisquares::UIndex top_left  = bot_left+(int)sqrt(this_world.num_squares());
//    tsunamisquares::UIndex top_right = bot_right+(int)sqrt(this_world.num_squares());
//////    // TODO: Save num_lons and num_lats in the world object
////    std::cout << "Deforming the bottom... " << std::endl;
//    this_world.deformBottom(bot_left,100.0);
//    this_world.deformBottom(top_left,100.0);
//    this_world.deformBottom(top_right,100.0);
//    this_world.deformBottom(bot_right,100.0);
//    
//    // Smooth the initial bump
//    this_world.diffuseSquares(dt);
//    this_world.diffuseSquares(dt);




//  Verifying the neighbor indexing ////////////////////////
//    tsunamisquares::UIndex top_left_corner  = 0;
//    this_world.square(top_left_corner).print_neighbors();
//    
//    tsunamisquares::UIndex top_right_corner = num_lons-1;
//    this_world.square(top_right_corner).print_neighbors();
//    
//    tsunamisquares::UIndex bottom_left_corner = num_lons*(num_lats-1);
//    this_world.square(bottom_left_corner).print_neighbors();
//    
//    tsunamisquares::UIndex bottom_right_corner = num_lons*num_lats - 1;
//    this_world.square(bottom_right_corner).print_neighbors();
//    
//    tsunamisquares::UIndex central = (int)(this_world.num_squares()*0.5);
//    this_world.square(central).print_neighbors();
//    
//    tsunamisquares::UIndex left_border = num_lons;
//    this_world.square(left_border).print_neighbors();
//    
//    tsunamisquares::UIndex top_border = (int) (num_lons*0.5);
//    this_world.square(top_border).print_neighbors();
//    
//    tsunamisquares::UIndex right_border = 2*num_lons - 1;
//    this_world.square(right_border).print_neighbors();
//    
//    tsunamisquares::UIndex bottom_border = bottom_left_corner+top_border;
//    this_world.square(bottom_border).print_neighbors();

