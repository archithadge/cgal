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

// === EXAMPLE SPECIFIC HEADERS ENDS HERE ===


typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Polyhedron_3<Kernel> Surface; 

// === EXAMPLE SPECIFIC DETAILS BEGINS HERE ===

typedef Kernel::Point_3 Point ;

typedef Surface::Halfedge_handle Halfedge_handle ;

// The following is the Visitor that keeps track of the simplification process.
// In this example the progress is printed real-time and a few statistics are
// recorded (and printed in the end).
//
struct Visitor
{
  Visitor() 
    : collected(0)
    , processed(0)
    , collapsed(0)
    , non_collapsable(0)
    , cost_uncomputable(0) 
    , placement_uncomputable(0) 
  {} 

  // Called on algorithm entry  
  void OnStarted( Surface& aSurface ) {} 
  
  // Called on algorithm exit  
  void OnFinished ( Surface& aSurface ) { std::cerr << "\n" << std::flush ; } 
  
  // Called when the stop condition returned true
  void OnStopConditionReached( Surface& aSurface ) {} 
  
  // Called during the collecting phase for each edge collected.
  void OnCollected( Halfedge_handle const& aEdge
                  , bool                   aIsFixed
                  , Surface&               aSurface
                  )
  {
    ++ collected ;
    std::cerr << "\rEdges collected: " << collected << std::flush ;
  }                
  
  // Called during the processing phase for each edge selected.
  // If aCost is absent the edge won't be collapsed.
  void OnSelected(Halfedge_handle const&  aEdge
                 ,Surface&                aSurface
                 ,boost::optional<double> aCost
                 ,std::size_t             aInitial
                 ,std::size_t             aCurrent
                 )
  {
    ++ processed ;
    if ( !aCost )
      ++ cost_uncomputable ;
      
    if ( aCurrent == aInitial )
      std::cerr << "\n" << std::flush ;
    std::cerr << "\r" << aCurrent << std::flush ;
  }                
  
  // Called during the processing phase for each edge being collapsed.
  // If aPlacement is absent the edge is left uncollapsed.
  void OnCollapsing(Halfedge_handle const&  aEdge
                   ,Surface&                aSurface
                   ,boost::optional<Point>  aPlacement
                   )
  {
    if ( aPlacement )
         ++ collapsed;
    else ++ placement_uncomputable ;
  }                
  
  // Called for each edge which failed the so called link-condition,
  // that is, which cannot be collapsed because doing so would
  // turn the surface into a non-manifold.
  void OnNonCollapsable( Halfedge_handle const& aEdge
                       , Surface&               aSurface
                       )
  {
    ++ non_collapsable;
  }                
  
  std::size_t  collected
             , processed
             , collapsed
             , non_collapsable
             , cost_uncomputable  
             , placement_uncomputable ; 
} ;

// === EXAMPLE SPECIFIC DETAILS ENDS HERE ===

namespace SMS = CGAL::Surface_mesh_simplification ;

int main( int argc, char** argv ) 
{
  Surface surface; 
  
  std::ifstream is(argv[1]) ; is >> surface ;

  // === CONCRETE USAGE EXAMPLE BEGINS HERE ===
  
  Visitor vis ;

  int r = SMS::edge_collapse(surface
                             ,SMS::Count_ratio_stop_condition<Surface>(0.10) 
                             ,SMS::external_edge_index_map(surface)
                             .SMS::visitor(&vis)
                             );

  
  std::cout << "\nEdges collected: " << visitor.collected
            << "\nEdges proccessed: " << visitor.processed
            << "\nEdges collapsed: " << visitor.collapsed
            << std::endl
            << "\nEdges not collapsed due to topological constrians: " 
            << visitor.non_collapsable
            << "\nEdge not collapsed due to cost computation constrians: " 
            << visitor.cost_uncomputable 
            << "\nEdge not collapsed due to placement computation constrians: " 
            << visitor.placement_uncomputable 
            << std::endl ; 
            
  // === CONCRETE USAGE EXAMPLE ENDS HERE ===
  
  std::cout << "\nFinished...\n" << r << " edges removed.\n"  
            << (surface.size_of_halfedges()/2) << " final edges." ;
  
        
  std::ofstream os( argc > 2 ? argv[2] : "out.off" ) ; os << surface ;
  
  return 0 ;      
}

// EOF //
