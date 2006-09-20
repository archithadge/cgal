#include <iostream>
#include <fstream>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include <CGAL/Surface_mesh_simplification/Polyhedron.h>
#include <CGAL/Surface_mesh_simplification/Edge_collapse.h>

// === EXAMPLE SPECIFIC HEADERS BEGINS HERE ===

// Stop-condition policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_pred.h>

// Cost-strategy policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_and_length.h> 

// === EXAMPLE SPECIFIC HEADERS ENDS HERE ===

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Polyhedron_3<Kernel> Surface; 

namespace SMS = CGAL::Surface_mesh_simplification ;

int main( int argc, char** argv ) 
{
  Surface surface; 
  
  std::ifstream is(argv[1]) ; is >> surface ;

  // === CONCRETE USAGE EXAMPLE BEGINS HERE ===
  
  // In this example, wich indicates that the cost and placement values 
  // should be computed on demand with no caching.
  // That is specified by passing the "Set_empty_collapse_data" policy
  // along with the GetCost and GetPlacement policies that do the
  // actual on-deman computation.
  //   
  int r = SMS::edge_collapse(surface
                            ,SMS::Count_ratio_stop_condition<Surface>(0.10)
                             
                            ,SMS::external_edge_index_map(surface)
                            
                            .SMS::set_cache    (SMS::Set_no_cache      <Surface>())
                            .SMS::get_cost     (SMS::Edge_length_cost  <Surface>())
                            .SMS::get_placement(SMS::Midpoint_placement<Surface>())
                            );
      
  // === CONCRETE USAGE EXAMPLE ENDS HERE ===
  
  std::cout << "\nFinished...\n" << r << " edges removed.\n" 
            << (surface.size_of_halfedges()/2) << " final edges.\n" ;
        
  std::ofstream os( argc > 2 ? argv[2] : "out.off" ) ; os << surface ;
  
  return 0 ;      
}

// EOF //
